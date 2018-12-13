//#################################################################################
//Written by Dominic Barker 20/06/18
// This code matches up triggered events in order to rate of muons in the lab. 
// It uses the time difference in the electronics that is found using calibration.cc
// to run give it a file that has been created by waveform.cc 
//##################################################################################

//C++ Includes                                                           
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

//Root Includes                                                          
#include "TMath.h"
#include "TApplication.h"
#include "TLine.h"
#include "TFile.h"
#include "TTree.h"

//Decoder Includes                                                           
#include "../DAQDecoder/ChannelInfo.h"
#include "../DAQDecoder/Event.h"

int main(int argc,char **argv){

  //Check to see if the file name, pmt threshold  and time were given. 
  if( argc != 5 ) {
    std::cout << "Sorry wrong setup to run this script please use the following setup: ./calibration.cc inputfilename pmt1threshold pmt2threshold time. " << std::endl;
    std::cout << "Where the pmt threholds are the cut off on the data of each threshold in millivolts and time is time the run lasted (this is printed out at the the completation of the run by runGPM as start time and end time), take the difference." << std::endl;   
    return 1; 
  }

  //Parameters from calibration.cc
  float mean = 0;
  float RMS  = 0; 

  //Parameters from the run (The start and end time are read out) 
  float time = std::stof(argv[4]); 

  //User dependent parameters in millivolts
  float pmt2_threshold = std::stof(argv[3]);
  float pmt1_threshold = std::stof(argv[2]);

  //Open the file
  const char* name = argv[1];
  TFile *file_in = new TFile(name);
  TTree *tree = (TTree*)file_in->Get("pulsetree");
  

  //Places holders for event information                                 
  PixelData::SubSystems::Event *event = new PixelData::SubSystems::Event();
  std::cout << "test 4" << std::endl;
  TBranch *branch  = tree->GetBranch("event");
  std::cout << "test 5" << std::endl;
  branch->SetAddress(&event);

  float peaktime_pmt1=-99999; 
  float peaktime_pmt2=-99999;

  std::cout << "test" << std::endl;

  int matches = 0; 

  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end(); ++channel){
      if(channel->channel == 1 && channel->maxVolt > pmt1_threshold){
        peaktime_pmt1 = channel->peaktimeSec;
      }
      if(channel->channel == 2 && channel->maxVolt > pmt2_threshold){
        peaktime_pmt2 = channel->peaktimeSec;
      }
      
      //Print out the timestamp.
      std::cout << "There are: " << channel->timestamp.first << " seconds since 1970 and " << channel->timestamp.second << " nanoseconds in that second" << std::endl;
    }
    
      //If they are not initialised correctly this is also set to -99999 
      if(peaktime_pmt1!=-99999 && peaktime_pmt2!=-99999){ 
	float time_diff = TMath::Abs(peaktime_pmt1 - peaktime_pmt2); 
	if(time_diff < mean + 3*RMS){++matches;}
      }
  }

  //Calculate the rate 
  float rate = matches/time;
  std::cout << "rate: " << rate << std::endl; 
 
  
  return 0; 
}
