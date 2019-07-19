//#######################################################
//## Name:        PixelTPC_DAQDecoder                   #
//## Author:      Dominic Barker                        #
//## Date:        Nov 2018                              #
//##                                                    #
//## Description: Module to take in binary DAQ files    #
//##              and create a root file with event     #
//##              objects.                              #
//#######################################################


//DAQ Includes 
#include "/home/argonshef/CAENDAQ/PixelDAQ/include/PixelReadout.hh"
#include "/home/argonshef/CAENDAQ/PixelDAQ/include/ChannelMap.hh"

//Analysis Includes 
#include "Event.h"
#include "ChannelInfo.h"

//Root Includes
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TF1.h"

//C++ Includes 
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

int MakerootFile(std::string& filename);

int main(int argc, char* argv[]){
  std::cout<<"test"<<std::endl;
  std::vector<std::string> inputfiles;
  std::vector<std::string> txtfiles;
  
  int c;
  //Check to see if a input file is used (i) or a .txt file (s) or both 
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
    std::cout << "Usage: " << argv[0] << " -s .txt or/and -i .dat " << std::endl;
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
    if(inputfiles[i].substr(suff,length) != ".dat"){
      std::cerr << "Input file is not a .dat. Please check." << std::endl;
      return 1;
    }  
    
    int err = MakerootFile(inputfiles[i]);
    if(err != 0){
      std::cerr << "Error in making the root file" << std::endl;
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
	  if(inputfile.substr(suff,length) != ".dat"){
	    std::cerr << "Input file is not a .dat. Please check." << std::endl;
	    return 1;
	  }  

	  int err = MakerootFile(inputfile);
	  if(err != 0){
	    std::cerr << "Error in making the root file" << std::endl;
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

int MakerootFile(std::string& filename){

  //######################
  //### Initialisation ###
  //######################
  std::cout<<"TEST"<<std::endl;
  gROOT->SetBatch(kTRUE);

  size_t suff = filename.find(".");
  //Open the binary file
  std::ifstream filein(filename.c_str(),std::ios::in|std::ios::binary);
  if (!filein)
    {
      std::cout << "Cannot open input file " << filename << std::endl;
      return 1;
    }

  //Holder for new root events (int is event number) 
  std::map<int,PixelData::TPC::Event*>  Events;
  PixelData::TPC::Event* Event;

  //Output file
  TFile * outputfile = new TFile(TString::Format("%s.root",filename.substr(0,suff).c_str()),"RECREATE");
  TTree * outputtree = new TTree("EventTree","Events");
  outputtree->Branch("Events",&Event,16000,99);

  //Holder for DAQ binary event header and header
  DAQ::PixelReadout::EventHeader *eventheader = new  DAQ::PixelReadout::EventHeader();
  DAQ::PixelReadout::Header *header = new  DAQ::PixelReadout::Header();

  //Read the file header 
  filein.read(reinterpret_cast<char*>(header),sizeof(DAQ::PixelReadout::Header));

  int numboards = header->NumBoards;

  std::map<int,std::string> channelMap = DAQ::ChannelMap::InitChannelMap();

  //#######################
  //### Writing to File ###
  //#######################

  //Loop over all the events in file
  while (!filein.eof()){

    //Get the header info and data for the event 
    filein.read(reinterpret_cast<char*>(eventheader),sizeof(DAQ::PixelReadout::EventHeader));

    //Reads the last one twice this stops that.
    if(filein.eof()){break;}

    //Check to see if the event number already exists.
    if(Events.find(eventheader->EventNumber) == Events.end()){
    
      // Set the event number and timestamp.
      Events[eventheader->EventNumber] = new PixelData::TPC::Event();
      Events[eventheader->EventNumber]->SetTimestamp(eventheader->Timestamp);
      Events[eventheader->EventNumber]->SetEventNumber(eventheader->EventNumber);
    }

    //Add the board if doesn't already exist.
    if(std::find(Events[eventheader->EventNumber]->BoardIDs.begin(),Events[eventheader->EventNumber]->BoardIDs.end(),eventheader->BoardBaseAddress) == Events[eventheader->EventNumber]->BoardIDs.end()){
      Events[eventheader->EventNumber]->AddBoardID(eventheader->BoardBaseAddress);
    }

    //Reads the last one twice this stops that.
    if(filein.eof()){break;}
    
    //Read the channel data
    for(uint16_t ch=0; ch<eventheader->NumChannels; ++ch){
      
      
      //uint16_t  *DataChannel = new uint16_t[9];
      uint16_t  *DataChannel = new uint16_t[(Int_t)eventheader->ChSize];
      filein.read(reinterpret_cast<char*>(DataChannel),sizeof(uint16_t)*eventheader->ChSize);

      TH1I* PedHist = new TH1I("PedHist","PedHist",4096,0,4096);

      std::vector<int> waveform;

      uint32_t nadc = eventheader->ChSize;
      waveform.reserve(nadc);

      //Calculate the channel pedestal and number of ADC
      for(int adc_it=0; adc_it<nadc; ++adc_it){
	PedHist->Fill((int)DataChannel[adc_it]);
	waveform.push_back((int)DataChannel[adc_it]);
      }

      //Fit a guassian around the ADC to try and find the pedestal. 
      float PedMP = PedHist->GetXaxis()->GetBinCenter(PedHist->GetMaximumBin());
      TF1 *PedFit = new TF1("PedFit","gaus");
      PedFit->SetParameter(1,PedMP);
      PedFit->SetParLimits(1,PedMP-5,PedMP+5);
      PedHist->Fit(PedFit,"Q");
      float PedMean = PedFit->GetParameter(1);
      float PedRMS = PedFit->GetParameter(2);

      //Do the Second fit to remove affects of possible signal.
      PedHist->Clear();
      for(int adc_it=0; adc_it<nadc; ++adc_it){
	if(TMath::Abs(DataChannel[adc_it] - PedMean < 5*PedRMS)){
	  PedHist->Fill((int)DataChannel[adc_it]);
	}
      }

      //Fit a guassian around the ADC to try and find the pedestal. 
      PedMP = PedHist->GetXaxis()->GetBinCenter(PedHist->GetMaximumBin());
      PedFit->SetParameter(1,PedMP);
      PedFit->SetParLimits(1,PedMP-5,PedMP+5);
      PedHist->Fit(PedFit,"Q");
      
      float pedestal = PedFit->GetParameter(1);

      PedFit->Delete();
      PedHist->Delete();

      //Find wich board the event came from and label the channel.
      int channel = ch + eventheader->NumChannels*0;
      std::string channelID = DAQ::ChannelMap::GetChannelID(ch+1, channelMap);
      PixelData::TPC::ChannelInfo ChannelData = PixelData::TPC::ChannelInfo(channel, channelID, (int) nadc, pedestal, waveform);

      //std::cout << "Channel:  " << channel << std::endl;
      //std::cout << "nadc:     " << nadc << std::endl;
      //std::cout << "Pedestal: " << pedestal << std::endl;
      
      Events[eventheader->EventNumber]->AddChannel(ChannelData);
      delete[] DataChannel;
      
    }

    //Add to tree if all the boards have been filled.
    if(Events[eventheader->EventNumber]->BoardIDs.size() == numboards){
      Event = Events[eventheader->EventNumber];
      outputtree->Fill();
      Event->Clear();
      delete Events[eventheader->EventNumber];
    }
  }

  filein.close();
  delete header;
  delete eventheader;
  outputfile->cd();
  outputtree->Write();
  outputfile->Close();

  //Deletes tree along with the file 
  outputfile->Delete();
  return 0;
}
