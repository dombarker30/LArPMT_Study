//#################################################################################
//Written by Dominic Barker 20/06/18
// This code matches up triggered events in order to rate of muons in the lab. 
// It uses the time difference in the electronics that is found using calibration.cc
// to run give it a file that has been created by KeySightDecooder.cc. 
// This is only a rough estimate a better analysis is needed.  
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
  if( argc != 4 ) {
    std::cout << "Sorry wrong setup to run this script please use the following setup: ./calibration.cc inputfilename pmt1threshold pmt2threshold. " << std::endl;
    std::cout << "Where the pmt threholds are the cut off on the data of each threshold in millivolts and time is time the run lasted (this is printed out at the the completation of the run by runGPM as start time and end time), take the difference." << std::endl;   
    return 1; 
  }

  //Parameters from calibration.cc
  
  std::ifstream paramsIn ("crt_calibrationOut.txt");

  double params[2];
  int i = 0;

  std::string line;
  while(getline(paramsIn,line))
    {
      paramsIn>>params[i];
      std::cout<<line<<"\n";
      i++;
    }
  
  float mean = params[0];
  float RMS = params[1];

  std::cout<<"Mean: "<<mean<<" and RMS: "<<RMS<<std::endl;

  //For channel 3 Trigger face down  
  //  float mean = 0.929146;
  //float RMS  = 2.92153; 
  
  //For channel 4 trigger face down 
  //  float mean = 2.13021;
  //float RMS = 3.27154; 

  //For channel four on the rigt 
  //float mean = 5.91323;
  //float RMS = 4.19436;

  //float mean = 4.6328;
  //float RMS = 4.06337;
    

  //  float mean = 5.95852;
  //float RMS = 4.02545;
 
 //User dependent parameters in volts
  float pmt2_threshold = std::stof(argv[3]);
  float pmt1_threshold = std::stof(argv[2]);

  //Open the file
  const char* name = argv[1];
  TFile *file_in = new TFile(name);
  TTree *tree = (TTree*)file_in->Get("pulsetree");
  
  int start_time;
  int end_time;

  //Places holders for event information                                 
  PixelData::SubSystems::Event *event = new PixelData::SubSystems::Event();
  TBranch *branch  = tree->GetBranch("event");
  branch->SetAddress(&event);

  int matches = 0; 

  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);

    float peaktime_pmt1=-99999;
    float peaktime_pmt2=-99999;

    float peaktime_pmt1_nocut = -99999;
    float peaktime_pmt2_nocut = -99999;

    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();

    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end(); ++channel){
      if(channel->channel == 3 && TMath::Abs(channel->maxVolt - channel->baseVolt) > pmt1_threshold){
        peaktime_pmt1 = channel->peaktimeSec;
      }
      if(channel->channel == 4 && TMath::Abs(channel->maxVolt - channel->baseVolt) > pmt2_threshold){
        peaktime_pmt2 = channel->peaktimeSec;
      }

      if(i==0 && channel->channel == 4){
	start_time = channel->timestamp.first; 
      }

      if(i==tree->GetEntries()-1 && channel->channel == 4){
	end_time = channel->timestamp.first;
      }

      //Print out the timestamp.
      //std::cout << "There are: " << channel->timestamp.first << " seconds since 1970 and " << channel->timestamp.second << " nanoseconds in that second" << std::endl;
    }
    
      //If they are not initialised correctly this is also set to -99999 
      if(peaktime_pmt1!=-99999 && peaktime_pmt2!=-99999){ 
	float time_diff = (peaktime_pmt1 - peaktime_pmt2)*1e9; 
	if(TMath::Abs(mean - time_diff) <  3*RMS){++matches;}
      }
  }

  float time = end_time - start_time; 
  std::cout << "start_time: " << start_time << " end_time:" << end_time << " time: " << time/60 << std::endl;

  //Calculate the rate 
  float rate = matches/time;
  std::cout << "rate: " << rate << std::endl; 
 
  float dark = tree->GetEntries() - matches;
  std::cout << "enteries: " << tree->GetEntries() << " dark sig: " << dark << " dark rate: " << dark/time << std::endl;

  return 0; 
}
