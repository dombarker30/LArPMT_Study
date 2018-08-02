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
#include <stdexcept>

#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TApplication.h"
#include "TLine.h"
#include "TFile.h"
#include "TTree.h"

#include "AgMD2.h"

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
};

void printHeader(Header);
int run(std::string filename, std::vector<int> chans, bool draw);

int main(int argc, char* argv[])
{

  std::string fname = argv[1];
  std::vector<int> chans(8,0);
  bool draw = false;
  if (argc>10)
    if (std::string(argv[10]) == "draw")
      draw = true;
  chans[0] = std::atoi(argv[2]);
  chans[1] = std::atoi(argv[3]);
  chans[2] = std::atoi(argv[4]);
  chans[3] = std::atoi(argv[5]);
  chans[4] = std::atoi(argv[6]);
  chans[5] = std::atoi(argv[7]);
  chans[6] = std::atoi(argv[8]);
  chans[7] = std::atoi(argv[9]);
      
  TApplication app("gpm_ana",&argc,argv);
  app.ExitOnException();
  std::cout << draw << "  filename given = " << fname << std::endl;
  run(fname,chans,draw);
  app.Run();
  return 0;
}

int run(std::string filename, std::vector<int> chans, bool draw)
{
  // 1=GPM_Anode1,2=GPM_Anode2,3=GPM_Top1,4=GPM_Top2,5=PMT_Raw,6=PMT_Shaped
  
  int ret = 0;

  size_t suff = filename.find(".");
  std::ifstream filein(filename.c_str(),std::ios::in|std::ios::binary);
  if (!filein)
    {
      std::cout << "Cannot open input file " << filename << std::endl;
      return 1;
    }

  TFile * fileout = TFile::Open(TString::Format("%s.root",filename.substr(0,suff).c_str()),"RECREATE");
  Int_t channel;
  Int_t source;//1=GPM_Anode1,2=GPM_Anode2,3=GPM_Top1,4=GPM_Top2,5=PMT_Raw,6=PMT_Shaped
  Float_t baseVolt;
  Float_t baseAdc;
  Float_t baseRmsVolt;
  Float_t baseRmsAdc;
  Float_t amplitudeVolt;
  Float_t amplitudeAdc;
  Float_t maxVolt;
  Float_t maxAdc;
  Float_t peaktimeSec;
  Float_t peaktimeTdc;
  Float_t timestamp;
  std::vector<float> waveform;

  TTree * tree = new TTree("pulsetree","Pulse Information");
  tree->Branch("channel",&channel,"channel/I");
  tree->Branch("source",&source,"source/I");
  tree->Branch("baseVolt",&baseVolt,"baseVolt/F");
  tree->Branch("baseAdc",&baseAdc,"baseAdc/F");
  tree->Branch("baseRmsVolt",&baseRmsVolt,"baseRmsVolt/F");
  tree->Branch("baseRmsAdc",&baseRmsAdc,"baseRmsAdc/F");
  tree->Branch("amplitudeVolt",&amplitudeVolt,"amplitudeVolt/F");
  tree->Branch("amplitudeAdc",&amplitudeAdc,"amplitudeAdc/F");
  tree->Branch("maxVolt",&maxVolt,"maxVolt/F");
  tree->Branch("maxAdc",&maxAdc,"maxAdc/F");
  tree->Branch("peaktimeSec",&peaktimeSec,"peaktimeSec/F");
  tree->Branch("peaktimeTdc",&peaktimeTdc,"peaktimeTdc/F");
  tree->Branch("waveform", &waveform);
  tree->Branch("timestamp",&timestamp);

  Header head;

  std::map<ViUInt8,std::vector<Float_t> > dataChannelMap;
  std::map<ViUInt8,Header> headerMap;

  std::map<int,TH1I*> histMap;
  TCanvas * canv;
  if (draw)
    {
      for (int c = 0; c < (int)(chans.size()); ++c)
	{
	  if (chans[c] == 0) continue;
	  histMap[fabs(chans[c])] = new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i",c+1),1,0,1);
	  /*
	  if (fabs(chans[c]) == 1) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, GPM Anode 1",c+1),1,0,1));
	  else if (fabs(chans[c]) == 2) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, GPM Anode 2",c+1),1,0,1));
	  else if (fabs(chans[c]) == 3) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, GPM Top 1",c+1),1,0,1));
	  else if (fabs(chans[c]) == 4) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, GPM Top 2",c+1),1,0,1));
	  else if (fabs(chans[c]) == 5) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, PMT Raw",c+1),1,0,1));
	  else if (fabs(chans[c]) == 6) histVec.push_back(new TH1I(TString::Format("h%i",c+1),TString::Format("Channel%i, PMT Shaped",c+1),1,0,1));
	  */
	}
      canv = new TCanvas("c","Data",1200,800);
      canv->Divide((int)(histMap.size()));
    }
  
  int pulsenumber;
  int numana = 0;
 
  while (!filein.eof())
    {
      filein.read(reinterpret_cast<char*>(&head),sizeof(Header));
      //      printHeader(head);
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
      
      bool complete = false;
      for (size_t c = 0; c < chans.size(); ++c)
	{
	  if (chans[c] == 0) continue;
	  if (dataChannelMap.find(c+1) != dataChannelMap.end())
	    {
	      for (size_t ca = 0; ca < chans.size(); ++ca)
		{
		  if (chans[ca] == 0) continue; // if (chans[c] == 0 || c == ca) continue;
		  if (dataChannelMap.find(ca+1) != dataChannelMap.end())
		    {
		      if (headerMap[c+1].eventNumber == headerMap[ca+1].eventNumber) complete = true; //std::cout << "test" << std::endl;
		    }
		}
	    }
	}
      
      if (complete)
	{
	  int i=1;
	  for (int ichan = 0; ichan < (int)chans.size(); ++ichan)
	    {
	      if (chans[ichan] == 0) continue;
	      std::vector<Float_t> wf = dataChannelMap[ichan+1];
	      int wf_size = wf.size();
	      if (draw) {
		canv->cd(i);
		std::cout << "histMap.size=" << histMap.size() << "  ichan=" << ichan << std::endl;
		std::cout << wf_size << std::endl;
		histMap[chans[ichan]]->SetBins(wf_size,0,wf_size);
		for (size_t point = 0; point < wf.size(); point++)
		  {
		    histMap[chans[ichan]]->SetBinContent(point+1,wf[point]);
		  }
		histMap[chans[ichan]]->Draw();
	      }
	      
	      float scalefactor = headerMap[ichan+1].scaleFactor;
	      float scaleoffset = headerMap[ichan+1].scaleOffset;
	      
	      
	      baseAdc = TMath::Mean(wf.begin(),wf.begin()+wf_size*0.25);
	      baseVolt = scalefactor*baseAdc+scaleoffset;
	      baseRmsAdc = TMath::StdDev(wf.begin(),wf.begin()+wf_size*0.25);
	      baseRmsVolt = scalefactor*baseRmsAdc+scaleoffset;
	      
	      int numavg = 5;
	      if (fabs(chans[ichan]) == 5) numavg = 1;
	      float maximum = -9999;
	      float minimum = 9999;
	      float maxpeaktime = -9999;
	      float minpeaktime = -9999;
	      for (size_t v = 0; v < wf.size(); ++v)
		{
		  if (wf[v] > maximum)
		    {
		      maximum = wf[v];
		      maxpeaktime = v;
		    }
		  if (wf[v] < minimum)
		    {
		      minimum = wf[v];
		      minpeaktime = v;
		    }
		}
	      peaktimeTdc = (chans[ichan] == 6) ? minpeaktime : maxpeaktime;
	      
	      maxAdc = 0;
	      int count = 0;
	      for (size_t i = peaktimeTdc-numavg; i < peaktimeTdc+numavg; ++i)
		{
		  maxAdc += wf[i];
		  count++;
		}
	      maxAdc /= count;
	      maxVolt = scalefactor*maxAdc+scaleoffset;
	      amplitudeAdc = fabs(maxAdc-baseAdc);
	      amplitudeVolt = fabs(maxVolt-baseVolt);
	      peaktimeSec = headerMap[ichan+1].initialXOffset+headerMap[ichan+1].xIncrement*peaktimeTdc;
	      
	      channel = ichan+1;
	      source = chans[ichan];

	      waveform = wf;
	      
	      tree->Fill();
	      
	      if (draw)
		{
		  double height = amplitudeAdc;
		  double base = baseAdc;
		  int trig = peaktimeTdc;
		  //histMap[chans[ichan]]->Draw();
		  TLine *ltrig = new TLine(histMap[chans[ichan]]->GetBinCenter(trig),-125,histMap[chans[ichan]]->GetBinCenter(trig),125);
		  ltrig->Draw();
		  TLine *lbase = new TLine(histMap[chans[ichan]]->GetBinCenter(0),base,histMap[chans[ichan]]->GetBinCenter(histMap[chans[ichan]]->GetNbinsX()),base);
		  lbase->Draw();
		  TLine *lpeak = new TLine(histMap[chans[ichan]]->GetBinCenter(0),base+height,histMap[chans[ichan]]->GetBinCenter(histMap[chans[ichan]]->GetNbinsX()),base+height);
		  lpeak->Draw();
		}
	      ++i;
	    }
	  
	  
	  if (draw)
	    {
	      canv->Update();
	      canv->WaitPrimitive();
	      canv->SaveAs(TString::Format("canvas%i.C",pulsenumber));
	    }
	  
	  pulsenumber = head.eventNumber;
	  timestamp = head.Timestamp;
	  if (numana%100==0) std::cout << numana << " -- Writing event " << head.eventNumber << " to file." << std::endl;
	  numana++;
	  
	  dataChannelMap.clear();
	  headerMap.clear();
	}
      //if (numana > 2000) break;
    }
   
  std::cout << "Last recorded event: " << pulsenumber << std::endl;
  
  filein.close();
  tree->Write();
  fileout->Close();
  gApplication->Terminate(ret);
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
