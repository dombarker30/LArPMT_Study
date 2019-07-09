/**********************************************
Prerequisites:
 -- ROOT 6 (built with python support)
 -- AgMD2 driver from Keysight (version <=1.12 for linux -- NOT VERSION 2+)
 -- C++11

Build with: 
 make clean; make

Run: (no dots in the path/filename other than just before the extension)
 ./waveform /full/path/to/binarydatafile.dat c1 c2 c3 c4 c5 c6 c7 c8 draw
or
 ./waveform /full/path/to/binarydatafile.dat c1 c2 c3 c4 c5 c6 c7 c8

Purpose:
  This program parses the binary file output from the AgMD2_GPM DAQ software.
The binary file is set up like this
<<header>><<waveform_c1>><<header>><<waveform_c2>>...<<header>><<waveform_c4>>
where the header is always 80 bytes itself, and the header defines the size
of each waveform which follows. The waveform contains data that is only 1 byte 
(8 bits) long (hence why it is stored in a char array) per data point. This 
byte must be cast to an integral type, then converted to a voltage using the 
header data. 
  To see the values in the header, activate the printHeader(Header) function
during binary file import. But, take note that the std::cout-ing will take 
up lots of CPU.
  Using the flag "draw" after the binary file path will enable ROOT plotting.
This feature is possibly broken (27/04/17).

(19/06/2018)                                                                                                                                                                       
Editted the file so that we have an event branch in root which channels as sub-branches. Just linking things uo more. It creates an event object (Event.h) by creating ChannelInfo objects (ChannelInfo.h).I haven't got rid of all the legacy code.   
**********************************************/

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <sys/stat.h>
#include <time.h>
#include <stdexcept>

#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"

#include "AgMD2.h"
#include "Event.h"
#include "ChannelInfo.h"

struct Header
{
  ViInt64 memsize;
  ViInt64 actualPoints;
  ViInt64 firstValidPoint;
  ViReal64 initialXOffset;
  ViReal64 initialXTimeSeconds;
  ViReal64 initialXTimeFraction;
  ViReal64 xIncrement;
  ViReal64 scaleFactor;
  ViReal64 scaleOffset;
  ViUInt8 channelNumber;
  int eventNumber;
  std::pair<int,int> Timestamp;
};

void printHeader(Header);
int run(std::string filename, std::vector<int> chans, bool draw, bool fVerbose, int num_events);

int main(int argc, char* argv[]){

  gROOT->SetBatch();

  std::string fname = argv[1];
  std::vector<int> chans(8,0);
  
  chans[0] = std::atoi(argv[2]);
  chans[1] = std::atoi(argv[3]);
  chans[2] = std::atoi(argv[4]);
  chans[3] = std::atoi(argv[5]);
  chans[4] = std::atoi(argv[6]);
  chans[5] = std::atoi(argv[7]);
  chans[6] = std::atoi(argv[8]);
  chans[7] = std::atoi(argv[9]);

  bool draw       = false;
  bool fVerbose   = false;
  int  num_events = -1;

  int c;
  while ((c = getopt (argc, argv, "n:d:D:v:V")) != -1){
    switch (c){
      case 'n':
        num_events = std::atoi(optarg);
	break;
      case 'd':
	std::cout << "Setting to draw" << std::endl;
        draw = true;
	break;
      case 'D':
	std::cout << "Setting to draw" << std::endl;
	draw = true;
	break;
      case 'v':
	std::cout << "Setting to talk" << std::endl;
	fVerbose = true;
	break;
      case 'V':
	std::cout << "Setting to talk" << std::endl;
	fVerbose = true;
      case '?':
	std::cout << "Unknown option" << std::endl;
        return 1;
    }
  }
  
  run(fname,chans,draw,fVerbose,num_events);
  return 0;
}

int run(std::string filename, std::vector<int> chans, bool draw, bool fVerbose, int num_events)
{
  // 1=GPM_Anode1,2=GPM_Anode2,3=GPM_Top1,4=GPM_Top2,5=PMT_Raw,6=PMT_Shaped
  
  

  gROOT->SetBatch(kTRUE);
  
  int ret = 0;
  int event_num = 0;

  size_t suff = filename.find(".");
  std::ifstream filein(filename.c_str(),std::ios::in|std::ios::binary);
  if (!filein)
    {
      std::cout << "Cannot open input file " << filename << std::endl;
      return 1;
    }
  
  PixelData::SubSystems::Event *event = new PixelData::SubSystems::Event();
  
  //Output Files 
  TFile * fileout_draw = TFile::Open(TString::Format("%s_Draw.root",filename.substr(0,suff).c_str()),"RECREATE");
  TFile * fileout = TFile::Open(TString::Format("%s.root",filename.substr(0,suff).c_str()),"RECREATE");

  //Channel Info Parmas 
  Int_t channel;
  Int_t source;
  Float_t baseVolt;
  Float_t baseAdc;
  Float_t baseRmsVolt;
  Float_t baseRmsAdc;
  Float_t integral;
  Float_t integralCharge;
  Float_t maxVolt;
  Float_t maxAdc;
  Float_t peaktimeSec;
  Float_t peaktimeTdc;

  std::pair<int,int> timestamp;
  std::vector<float> waveform;
  

  TTree * tree = new TTree("pulsetree","Pulse Information");
  tree->Branch("event",&event,16000,2);

  Header head;
  
  std::map<ViUInt8,std::vector<Float_t> > dataChannelMap;
  std::map<ViUInt8,Header> headerMap;
  
  int pulsenumber;
  int numana = 0;
  
  int usedchannels = 0;
  for (size_t c = 0; c < chans.size(); ++c){
    if (chans[c] == 0) continue;
    ++usedchannels;
  }

  while (!filein.eof()){
    
    if(num_events > 0){
      if(event_num == num_events){break;}
    }

    //Read the datat into the file 
    filein.read(reinterpret_cast<char*>(&head),sizeof(Header));
    //    printHeader(head);
    headerMap.erase(head.channelNumber);
    headerMap.emplace(head.channelNumber,head);
    ViChar * data = new ViChar[head.memsize];
    filein.read(reinterpret_cast<char*>(data),head.memsize*sizeof(ViChar));
    dataChannelMap.erase(head.channelNumber);
    std::vector<Float_t> empty(head.actualPoints,0);
    dataChannelMap.emplace(head.channelNumber,empty);
    
    for (int j = 0; j < head.actualPoints; j++)
      {
	int val = (float)data[j+head.firstValidPoint];
	if (val > 127 || val < -128)
	  {
	    std::cout << "out of range value " << val << std::endl;
	  }
	dataChannelMap[head.channelNumber][j] = val;
      }
    delete[] data;
        
    //Check to see if the all the channels have been filled.
    bool complete = false;
    if(dataChannelMap.size() == usedchannels){complete = true;}

    if (complete){

      ++event_num; 

      std::map<int,TH1I*> histMap;
      std::map<int,TH1I*> pedhist_map1;
      std::map<int,TH1I*> pedhist_map2;
      std::map<int,TPad*> padMap_wf;
      std::map<int,TPad*> padMap_ped;
        
      //Set up the canvases
      TCanvas * canv;
      if (draw){

	for (int c = 0; c < (int)(chans.size()); ++c){
	  if (chans[c] == 0) continue;
	  
	  histMap[fabs(chans[c])] = new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i Waveform for Event %i",c+1,headerMap[c+1].eventNumber),1,0,1);
	}
	
	std::stringstream sstm;
	sstm << "Canvas For Event: " << head.eventNumber;
	std::string graph_string = sstm.str();
	const char* graph_name = graph_string.c_str();
	canv = new TCanvas(graph_name,graph_name,900,500);
	canv->Divide(1,2*histMap.size());                                                                             
	canv->Draw();
	int j=0;
	for(int c = 0; c < (int)(chans.size()); ++c){
	    if (chans[c] == 0) continue;
            
	    padMap_wf[fabs(chans[c])] = (TPad*)canv->cd((2*j)+1);
	    padMap_wf[fabs(chans[c])]->Draw();
	    padMap_ped[fabs(chans[c])] = (TPad*)canv->cd((2*j)+2);
	    padMap_ped[fabs(chans[c])]->Divide(2,1);
	    padMap_ped[fabs(chans[c])]->Draw();
	    ++j;
	}
      }

      for(int ichan = 0; ichan < (int)chans.size(); ++ichan){
	
	if (chans[ichan] == 0) continue;

	std::vector<Float_t> wf = dataChannelMap[ichan+1];
	int wf_size = wf.size();

	float scalefactor = headerMap[ichan+1].scaleFactor;
	float scaleoffset = headerMap[ichan+1].scaleOffset;
	float triggerdelay = ((headerMap[ichan+1].initialXOffset)/(1e-9));
	
	float integration_cut = 0;
	float int_start = 0;

	if (draw) {
	  histMap[chans[ichan]]->SetBins(wf_size,0,wf_size);
	  for (size_t point = 0; point < wf.size(); point++){
	    histMap[chans[ichan]]->SetBinContent(point+1,wf[point]);
	  }
	}

	      
	//float quick_baseline = TMath::Mean(wf.begin(),wf.begin()+(wf_size*TMath::Abs(triggerdelay))-100);
	//	baseVolt = scalefactor*baseAdc+scaleoffset;
	//baseRmsAdc = TMath::StdDev(wf.begin(),wf.begin()+wf_size*0.1);
	//baseRmsVolt = scalefactor*baseRmsAdc+scaleoffset;
	
	int numavg = 1;
	int iterations = 2; 
	float maximum = -9999;
	float max_ped = -9999;
	float min_ped = 9999;
	float minimum = 9999;
	float maxpeaktime = -9999;
	float minpeaktime = -9999;
	float baseAdcrms = -9999;

	//Keep a track of the total integration                                                                                                                                    
	float intcounter = 0;
	float int_tot = 0;

	std::string pedhist_string = "ped_h " + std::to_string(chans[ichan]); 
	const char* pedhist_name = pedhist_string.c_str();
	pedhist_map1[chans[ichan]] = new TH1I(pedhist_name,"Pedestal Histogram",2048+1,-1024-0.5,1024+0.5);


	//Iterative Loop to calculate the integration size proper size.
	for(int iter=0; iter<iterations;++iter){
	  
	  //Keep a track of the total integration 
	  intcounter = 0; 
	  int_tot = 0;

	  //Find the mins and maxes and fill the pedestal histogram.
	  for (size_t v = 0; v <wf.size(); ++v){
      
	    if(v > int_start && v<integration_cut){
	      int_tot += wf[v];
	      intcounter++;
	    }
  
	    if (wf[v] > maximum){
	      maximum = wf[v];
	      maxpeaktime = v;
	    }
	    if (wf[v] < minimum){
		minimum = wf[v];
		minpeaktime = v;
	    }
	    	    
	    if(v < int_start || v > integration_cut){
	      pedhist_map1[chans[ichan]]->Fill(wf[v]);
	      if(wf[v]>max_ped){max_ped=wf[v];}
	      if(wf[v]<min_ped){min_ped=wf[v];}
	    }
	  }
	  
	  if (draw){
	    fileout_draw->cd();
	    padMap_ped[chans[ichan]]->cd(1);
	    pedhist_map1[chans[ichan]]->GetXaxis()->SetRangeUser(min_ped-0.5,max_ped+0.5);
	    pedhist_map1[chans[ichan]]->Draw();
	  }
	  
	  float ped_RMS = pedhist_map1[chans[ichan]]->GetRMS();
	  float ped_mode = pedhist_map1[chans[ichan]]->GetXaxis()->GetBinCenter(pedhist_map1[chans[ichan]]->GetMaximumBin());
	
	  //Fit a Gaussian to the pedestal 
	  TF1 *ped_fit = new TF1("ped_fit","gaus");
	  ped_fit->SetParameter(1,ped_RMS);
	  ped_fit->SetParLimits(1,min_ped,max_ped);
	
	  //std::cout << "test1" << std::endl;
	  pedhist_map1[chans[ichan]]->Fit(ped_fit,"Q");
	  //std::cout << "test2" << std::endl;
	  float ped_fit_mean = ped_fit->GetParameter(1);
	  float ped_fit_stddev = ped_fit->GetParameter(2);
	  
	  std::string pedhist2_string = "ped_h2 " + std::to_string(chans[ichan]);
	  const char* pedhist2_name = pedhist2_string.c_str();
	  pedhist_map2[chans[ichan]] = new TH1I(pedhist2_name,"Reduced Pedestal Histogram",TMath::Abs(max_ped-min_ped)+1,min_ped-0.5,max_ped+0.5);
	  
	  //Fill only values into the second pedestal fit that fall within the std dev limits 
	  for (size_t v=0; v<int_start; ++v){
	    if(TMath::Abs(wf[v] - ped_fit_mean) < (3*ped_fit_stddev)){
	      pedhist_map2[chans[ichan]]->Fill(wf[v]);
	    }
	  }
	  for(size_t v=integration_cut; v<wf.size(); ++v){
	    if(TMath::Abs(wf[v] - ped_fit_mean) < (3*ped_fit_stddev)){
	      pedhist_map2[chans[ichan]]->Fill(wf[v]);
	    }
	  }
	  
	  if(draw){
	    padMap_ped[chans[ichan]]->cd(2);
	    pedhist_map2[chans[ichan]]->Draw();
	    canv->Update();
	    fileout->cd();
	  }

	  //Fit the second pedestal graph 
	  TF1 *ped_fit2 = new TF1("ped_fit2","gaus");
	  ped_fit2->SetParameter(1,ped_fit_mean);
	  ped_fit2->SetParLimits(1,min_ped,max_ped);
	  ped_fit2->SetParameter(2,ped_fit_stddev);
	  //std::cout << "test3" << std::endl;
	  pedhist_map2[chans[ichan]]->Fit(ped_fit2,"Q");
	  //std::cout << "test4" << std::endl;
	  if(fVerbose){
	    std::cout << "First Mode: " << ped_mode << "second mode: " << pedhist_map2[chans[ichan]]->GetXaxis()->GetBinCenter(pedhist_map2[chans[ichan]]->GetMaximumBin()) << " baseAdc: " << ped_fit2->GetParameter(1);		    
	  }
	  
	  baseAdc = ped_fit2->GetParameter(1);
	  baseAdcrms = ped_fit2->GetParameter(2);
	  
	  ped_fit->Delete();
	  ped_fit2->Delete();
	  
	  //Check to see if the signal was positive or negative 
	  if(TMath::Abs(maximum - baseAdc) > TMath::Abs(minimum - baseAdc)){peaktimeTdc = maxpeaktime;}
	  else{peaktimeTdc = minpeaktime;}
	  
	  //Calculate the new integration limits
	  if((iter+1) != iterations){

	    pedhist_map1[fabs(chans[ichan])]->Clear();
	    pedhist_map2[fabs(chans[ichan])]->Clear();

	    //For int_start
	    float wf_avg  = 9999;
	    int   tick    = 0;
	    int   i       = 0;
	    std::vector<float> wf_vals = {0,0,0,0,0};
	    while(TMath::Abs(wf_avg-baseAdc) > 3*baseAdcrms || tick<5){ 
	      ++tick;
	      wf_vals[i/5] = wf[peaktimeTdc - tick];
	      wf_avg     = TMath::Mean(wf_vals.begin(),wf_vals.end());
	      i += 5;
	      if(i==25){i=0;}
	    }
	    int_start = peaktimeTdc - tick - 100; 
	    if(int_start < 0){int_start = 0;}
	    
	    //For Integration cut 
	    wf_avg  = 9999;
	    tick    = 0;
	    i       = 0;
	    wf_vals.clear();
	    wf_vals = {0,0,0,0,0};
	    while(TMath::Abs(wf_avg-baseAdc) > 3*baseAdcrms || tick<5){ 
	      ++tick;
	      wf_vals[i/5] = wf[peaktimeTdc + tick];
	      wf_avg     = TMath::Mean(wf_vals.begin(),wf_vals.end());
	      i += 5;
	      if(i==25){i=0;}
	    }
	    integration_cut = peaktimeTdc + tick + 300; 
	    if(integration_cut > wf.size()){integration_cut = wf.size();}
	  }
	}//iteration loop 
       
	//maxAdc = 0;
	//  int count = 0;
	//  for (size_t i = peaktimeTdc-numavg; i < peaktimeTdc+numavg; ++i){
	//    maxAdc += wf[i];
	//    count++;
	//  }
	  
	//maxAdc /= count;
	
	maxAdc         = wf[peaktimeTdc];
	integral       = TMath::Abs(int_tot - (intcounter*baseAdc));
	integralCharge = integral*scalefactor*(2e-11);//convert ADC to V then divide by 1e9/50 
	baseVolt       = scalefactor*baseAdc+scaleoffset;
	baseRmsAdc     = baseAdcrms;
	baseRmsVolt    = scalefactor*baseRmsAdc+scaleoffset;
	maxVolt        = scalefactor*maxAdc+scaleoffset;
	peaktimeSec    = headerMap[ichan+1].initialXOffset+headerMap[ichan+1].xIncrement*peaktimeTdc;
	
	channel = ichan+1;
	source = chans[ichan];
	
	waveform = wf;
	
	timestamp.first = head.Timestamp.first;
	timestamp.second = head.Timestamp.second;
	
	PixelData::SubSystems::ChannelInfo channelinfo = PixelData::SubSystems::ChannelInfo(channel,baseVolt,baseAdc, baseRmsAdc,baseRmsVolt, maxVolt,maxAdc, peaktimeSec, peaktimeTdc,wf,timestamp,integral,integralCharge);
	event->AddChannel(channelinfo);
      
	if (draw){
	  
	  fileout_draw->cd();
	  padMap_wf[chans[ichan]]->cd();
	  histMap[chans[ichan]]->Draw();
	  
	  
	  double height = fabs(maxAdc-baseAdc);
	  double base = baseAdc;
	  int trig = peaktimeTdc;
	  TLine *ltrig = new TLine(histMap[chans[ichan]]->GetBinCenter(trig),-125,histMap[chans[ichan]]->GetBinCenter(trig),125);
	  ltrig->SetLineColor(kGreen);
	  ltrig->Draw("same");
	  TLine *lbase = new TLine(histMap[chans[ichan]]->GetBinCenter(0),base,histMap[chans[ichan]]->GetBinCenter(histMap[chans[ichan]]->GetNbinsX()),base);
	  lbase->Draw("same");
	  TLine *lpeakh = new TLine(histMap[chans[ichan]]->GetBinCenter(0),base+height,histMap[chans[ichan]]->GetBinCenter(histMap[chans[ichan]]->GetNbinsX()),base+height);
	  lpeakh->Draw("same");
	  TLine *lpeakv = new TLine(trig,-125,trig,125);
	  lpeakv->Draw("same");
	  TLine *lpedcut1 = new TLine(int_start,-125,int_start,125);
	  TLine *lpedcut2 = new TLine(integration_cut,-125,integration_cut,125);
	  
	  lpedcut1->SetLineColor(kRed);
	  lpedcut2->SetLineColor(kRed);
	  lpedcut1->Draw("same");
	  lpedcut2->Draw("same");
	  canv->Update();
	}
      }//Channel Loop

      fileout->cd();
      event->SetEventNumber(head.eventNumber);
      tree->Fill();
      event->Clear();

      if (draw){
	fileout_draw->cd();
	canv->Update();
	canv->Write();
	delete canv;
	for (int c = 0; c < (int)(chans.size()); ++c){
	  if (chans[c] == 0) continue;
	  histMap[fabs(chans[c])]->Delete();
	}
	fileout->cd();
      }

      for (int c = 0; c < (int)(chans.size()); ++c){
	if (chans[c] == 0) continue;
	pedhist_map1[fabs(chans[c])]->Delete();
	pedhist_map2[fabs(chans[c])]->Delete();
      }
      
      pulsenumber = head.eventNumber;
      
      if (numana%100==0) std::cout << numana << " -- Writing event " << head.eventNumber << " to file." << std::endl;
      numana++;
      
      dataChannelMap.clear();
      headerMap.clear();
    }
  }
  
  std::cout << "Last recorded event: " << pulsenumber << std::endl;
  
  filein.close();
  tree->Write();
  fileout->Close();
  fileout_draw->Close();
  
  return ret;
}



void printHeader(Header head)
{
  std::cout << std::endl;
  std::cout << std::setw(24) << std::right << "memsize = "
	    << std::setw(10) << std::left << head.memsize << " | "
	    << std::setw(31) << std::right << "sizeof(memsize) = "
	    << std::setw(2)  << std::left << sizeof(head.memsize) << " | "
	    << std::setw(29) << std::right << "type(memsize) = "
	    << std::left << typeid(head.memsize).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "actualPoints = "
	    << std::setw(10) << std::left << head.actualPoints << " | "
	    << std::setw(31) << std::right << "sizeof(actualPoints) = "
	    << std::setw(2)  << std::left << sizeof(head.actualPoints) << " | "
	    << std::setw(29) << std::right << "type(actualPoints) = "
	    << std::left << typeid(head.actualPoints).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "firstValidPoint = "
	    << std::setw(10) << std::left << head.firstValidPoint << " | "
	    << std::setw(31) << std::right << "sizeof(firstValidPoint) = "
	    << std::setw(2)  << std::left << sizeof(head.firstValidPoint) << " | "
	    << std::setw(29) << std::right << "type(firstValidPoint) = "
	    << std::left << typeid(head.firstValidPoint).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "initialXOffset = "
	    << std::setw(10) << std::left << head.initialXOffset << " | "
	    << std::setw(31) << std::right << "sizeof(initialXOffset) = "
	    << std::setw(2)  << std::left << sizeof(head.initialXOffset) << " | "
	    << std::setw(29) << std::right << "type(initialXOffset) = "
	    << std::left << typeid(head.initialXOffset).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "initialXTimeSeconds = "
	    << std::setw(10) << std::left << head.initialXTimeSeconds << " | "
	    << std::setw(31) << std::right << "sizeof(initialXTimeSeconds) = "
	    << std::setw(2)  << std::left << sizeof(head.initialXTimeSeconds) << " | "
	    << std::setw(29) << std::right << "type(initialXTimeSeconds) = "
	    << std::left << typeid(head.initialXTimeSeconds).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "initialXTimeFraction = "
	    << std::setw(10) << std::left << head.initialXTimeFraction << " | "
	    << std::setw(31) << std::right << "sizeof(initialXTimeFraction) = "
	    << std::setw(2)  << std::left << sizeof(head.initialXTimeFraction) << " | "
	    << std::setw(29) << std::right << "type(initialXTimeFraction) = "
	    << std::left << typeid(head.initialXTimeFraction).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "xIncrement = "
	    << std::setw(10) << std::left << head.xIncrement << " | "
	    << std::setw(31) << std::right << "sizeof(xIncrement) = "
	    << std::setw(2)  << std::left << sizeof(head.xIncrement) << " | "
	    << std::setw(29) << std::right << "type(xIncrement) = "
	    << std::left << typeid(head.xIncrement).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "scaleFactor = "
	    << std::setw(10) << std::left << head.scaleFactor << " | "
	    << std::setw(31) << std::right << "sizeof(scaleFactor) = "
	    << std::setw(2)  << std::left << sizeof(head.scaleFactor) << " | "
	    << std::setw(29) << std::right << "type(scaleFactor) = "
	    << std::left << typeid(head.scaleFactor).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "scaleOffset = "
	    << std::setw(10) << std::left << head.scaleOffset << " | "
	    << std::setw(31) << std::right << "sizeof(scaleOffset) = "
	    << std::setw(2)  << std::left << sizeof(head.scaleOffset) << " | "
	    << std::setw(29) << std::right << "type(scaleOffset) = "
	    << std::left << typeid(head.scaleOffset).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "channelNumber = "
	    << std::setw(10) << std::left << (int)head.channelNumber << " | "
	    << std::setw(31) << std::right << "sizeof(channelNumber) = "
	    << std::setw(2)  << std::left << sizeof(head.channelNumber) << " | "
	    << std::setw(29) << std::right << "type(channelNumber) = "
	    << std::left << typeid(head.channelNumber).name()
	    << std::endl;
  std::cout << std::setw(24) << std::right << "eventNumber = "
	    << std::setw(10) << std::left << head.eventNumber << " | "
	    << std::setw(31) << std::right << "sizeof(eventNumber) = "
	    << std::setw(2)  << std::left << sizeof(head.eventNumber) << " | "
	    << std::setw(29) << std::right << "type(eventNumber) = "
	    << std::left << typeid(head.eventNumber).name()
	    << std::endl;
  std::cout << std::endl;
}
