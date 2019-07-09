//Decoder Includes                                                                                                                         
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

//Root Includes    
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"

//C++ Includes 
#include <fstream> 
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

int AnalysisExample(std::string& filename);

int main(int argc, char* argv[]){

  std::vector<std::string> inputfiles;
  std::vector<std::string> txtfiles;

  int c;

  //Check to see if a input file is a .root (i) or a .txt file (s) or both 
  while ((c=getopt(argc, argv, "i:s:")) != -1){
    switch (c) {
    case 'i':
      inputfiles.push_back(optarg);
      break;
    case 's':
      txtfiles.push_back(optarg);
      break;
    default:
      std::cerr << "You must give an input file" << std::endl; 
      return 1;
    }
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
    
    int err = AnalysisExample(inputfiles[i]);

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

	  int err = AnalysisExample(inputfile);
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

  
int AnalysisExample(std::string& filename){

  //File Inputs
  const char* filechar = filename.c_str();
  TFile *inputfile = new TFile(filechar);
  TTree *EventTree = (TTree*)inputfile->Get("EventTree");

  //Event Holder
  PixelData::TPC::Event *event = new PixelData::TPC::Event();
  TBranch *branch  = EventTree->GetBranch("Events");
  branch->SetAddress(&event);

  for(Long64_t i=0;i<EventTree->GetEntries();i++){

    //Get the Event Information
    branch->GetEntry(i);

    std::cout << "Event Number: " << event->GetEventNumber() << std::endl;

    //Get the channel Information
    const std::vector<PixelData::TPC::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::TPC::ChannelInfo>::const_iterator channel=Channels.begin(); channel!=Channels.end(); ++channel){

      const int NADC            = channel->GetNADC();
      const float Pedestal      = channel->GetPedestal();
      const std::vector<int> Waveform = channel->GetWaveform();

      
      
      std::cout << "NADC: " << NADC << " Pedestal: " << Pedestal << std::endl;

      for(int adc_it=0; adc_it<NADC; ++adc_it){
    	std::cout << " ADC at tick: " << adc_it << " is " << Waveform[adc_it] << std::endl;
      }
    }
  }
}
