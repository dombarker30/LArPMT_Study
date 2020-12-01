//Decoder Includes                                                                                                                         
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

//Channel Map
#include "/home/argonshef/CAENDAQ/PixelDAQ/include/ChannelMap.hh"


//Root Includes    
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TF1.h"

//C++ Includes 
#include <fstream> 
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <getopt.h>

//Code Takes the RMS of the data and averages them.

int RMSCalculator(std::string& filename,int& current_eventnum, int process_events);

int main(int argc, char* argv[]){

  std::vector<std::string> inputfiles;
  std::vector<std::string> txtfiles;

  int process_events = -1; 
  int current_eventnum = 0;

  //Get the options
  static struct option long_options[] = {
    {"inputfile"     , required_argument,  0,  'i' },
    {"filelist"      , required_argument,  0,  's' },
    {"process_events", no_argument,  0,  'n' },
    {0, 0, 0, 0}
  };

  int opt = 0;
  int c = 0;
  while ((opt = getopt_long_only(argc, argv,"", long_options, &c )) != -1) {
    switch (opt) {
    case 'i' : inputfiles.push_back(optarg);
      break;
    case 's' : txtfiles.push_back(optarg);
      break;
    case 'n' : process_events = std::stoi(optarg);
      break;
    default: 
      std::cout << "Usage:  -s .txtfile or/and -i .root. -n for the number of events to be analysed. " << std::endl;
      return 1;
    }
  }

    if(inputfiles.size() == 0 && txtfiles.size() == 0){
    std::cout << "please give an inputfile with -i *.root or -s .txt" << std::endl;
    return 1;
  }

  //Check to see if the there is a one to one for tags and files
  if (argc - optind < 0) {
    std::cout << "Usage: " << argv[0] << " -s .txtfile or/and -i .root " << std::endl;
    return 1;
  }

  //Loop over the files and make the root files.
  for(int i=0; i<inputfiles.size(); ++i){
    
    size_t length = strlen(inputfiles[i].c_str());
    
    size_t num_space = std::count(inputfiles[i].begin(), inputfiles[i].end(),' ');
    if(num_space != 0 ){
      std::cerr << "Space found in input file. Please check" << std::endl;
      return 1;
    }
    
    size_t suff = inputfiles[i].find(".");
    if(inputfiles[i].substr(suff,length) != ".root"){
      std::cerr << "Input file is not a .root. Please check." << std::endl;
      return 1;
    }  
    
    int err = RMSCalculator(inputfiles[i],current_eventnum,process_events);

    if(err != 0){
      std::cerr << "Error in analysis" << std::endl;
      return 1;
    }
  }

  //Loop over the text files and make a root file.
  for(int i=0; i<txtfiles.size(); ++i){
    std::string txtfile = txtfiles[i];    
    std::ifstream txtfile_stream;
    txtfile_stream.open(txtfile);
    if (txtfile_stream.is_open()){
      std::string inputfile;
      while (txtfile_stream.good()){

	  std::getline(txtfile_stream, inputfile);

	  size_t length = strlen(inputfile.c_str());

	  size_t num_space = std::count(inputfiles[i].begin(), inputfiles[i].end(),' ');
	  if(num_space != 0){
	    std::cerr << "Space found in input file. Please check" << std::endl;
	    return 1;
	  }

	  size_t suff = inputfile.find(".");
	  if(inputfile.substr(suff,length) != ".root"){
	    std::cerr << "Input file is not a .root. Please check." << std::endl;
	    return 1;
	  }  

	  int err = RMSCalculator(inputfile,current_eventnum,process_events);
	  if(err != 0){
	    std::cerr << "Error in analysis" << std::endl;
	    return 1;
	  }
      }
    }
    else{
      std::cerr << "Could not open the .txt file" << std::endl;
      return 1;
    }
  }
  return 0;
}

  
int RMSCalculator(std::string& filename,int& current_eventnum, int process_events){

  if(current_eventnum > process_events && process_events > 0){return 0;}

  ++current_eventnum;


  //File Inputs
  const char* filechar = filename.c_str();
  TFile *inputfile = new TFile(filechar);
  TTree *EventTree = (TTree*)inputfile->Get("EventTree");

  //Event Holder
  PixelData::TPC::Event *event = new PixelData::TPC::Event();
  TBranch *branch  = EventTree->GetBranch("Events");
  branch->SetAddress(&event);

  //FFT histograms.  
  TH1D* PixelAvg =  new TH1D("PixelAvg","PixelAvg",8000,-8000,8000);
  TH1D* ROIAvg =  new TH1D("ROIAvg","ROIAvg",8000,-8000,8000);
  TGraphErrors* PixelGraph = new TGraphErrors();
  TGraphErrors* ROIGraph = new TGraphErrors();

  std::map<int,std::vector<double >> means;

  for(Long64_t i=0;i<EventTree->GetEntries();i++){


    //Get the Event Information
    branch->GetEntry(i);

    std::cout << "Event Number: " << event->GetEventNumber() << std::endl;

    //Get the channel Information
    const std::vector<PixelData::TPC::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::TPC::ChannelInfo>::const_iterator channel=Channels.begin(); channel!=Channels.end(); ++channel){

      const int NADC            = channel->GetNADC();
      const float Pedestal      = channel->GetPedestal();
      const std::vector<double> Waveform = channel->GetWaveform();
      std::string channelID = channel->GetChannelID();
      double num = channel->GetChannelNumber();


      TH1D* Temp =  new TH1D("Temp","Temp",NADC,0,NADC);

      for(auto const& tick: Waveform){
	//Add the waveform to a histogram
	int chanStrLength = channelID.size();
	if(chanStrLength>5){
	  PixelAvg->Fill((tick - Pedestal)*217.6688058036);
	}
	else{
	  ROIAvg->Fill((tick - Pedestal)*217.6688058036); 
	}
	
	Temp->Fill((tick - Pedestal)*217.6688058036);
      }

      TF1 *PedFit = new TF1("PedFit","gaus");
      Temp->Fit(PedFit,"Q");
      float PedMean = PedFit->GetParameter(1);
      float PedRMS = PedFit->GetParameter(2);

      Temp->Clear();

      //Repeat the process 
      for(auto const& tick: Waveform){
	//Add the waveform to a histogram
	if(tick - Pedestal < 5*PedRMS){
	  Temp->Fill((tick - Pedestal)*217.6688058036);
	}
      }

      TF1 *PedFit2 = new TF1("PedFit2","gaus");
      Temp->Fit(PedFit2,"Q");
      float PedMean2 = PedFit2->GetParameter(1);
      float PedRMS2 = PedFit2->GetParameter(2);

      means[num].push_back(PedRMS2);
      delete Temp;
      delete PedFit;
      delete PedFit2;
    }
  }

  double totpix = 0;
  double totpix_i = 0;
  double totroi = 0;
  double totroi_i = 0;
  double std_devpix = 0;
  double std_devroi = 0;
  for(auto const& chan: means){
    float tot = 0;
    float totsqr = 0;
    for(auto const& mean_val: chan.second){
      tot += mean_val;
      totsqr += mean_val*mean_val;
    }
    float n = chan.second.size();
    float std_dev = TMath::Sqrt(totsqr/n - (tot/n)*(tot/n));
    
    if(chan.first<35){
      totpix += tot/n;
      std_devpix += std_dev;
      ++totpix_i;
      PixelGraph->SetPoint(PixelGraph->GetN(),chan.first,tot/n);
      PixelGraph->SetPointError(PixelGraph->GetN()-1,0,std_dev);
    }
    else{
      totroi += tot/n;
      std_devroi += std_dev;
      ++totroi_i;
      ROIGraph->SetPoint(ROIGraph->GetN(),chan.first,tot/n);
      ROIGraph->SetPointError(ROIGraph->GetN()-1,0,std_dev);
    }
  }

  std::cout << "mean RMS for pixels is: " << totpix/totpix_i << " +- " << std_devpix/totpix_i << std::endl;

  std::cout << "Mean RMS for ROI is: " << totroi/totroi_i << " +- " << std_devroi/totroi_i << std::endl;
  

  size_t suff = filename.find(".");
  TFile* outputfile = new TFile(TString::Format("%s_RMS.root",filename.substr(0,suff).c_str()),"RECREATE");
  outputfile->cd();

  ROIGraph->Write();
  PixelGraph->Write();
  PixelAvg->Write();
  ROIAvg->Write();

  return 0;

}
