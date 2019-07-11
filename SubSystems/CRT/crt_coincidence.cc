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

int main(){
//int main(int argc,char **argv){
//
//  //Check to see if the file name, pmt threshold  and time were given. 
//  if( argc != 0 ) {
//    std::cout << "Sorry wrong setup to run this script please use the following setup: ./calibration.cc inputfilename pmt1threshold pmt2threshold. " << std::endl;
//    std::cout << "Where the pmt threholds are the cut off on the data of each threshold in millivolts and time is time the run lasted (this is printed out at the the completation of the run by runGPM as start time and end time), take the difference." << std::endl;   
//    return 1; 
//  }

  //Parameters from calibration.cc
  
  double params[4];
  int lineNum = 0;

  std::string line;
  std::ifstream paramsIn ("crt_calibrationOut.txt");
  while(getline(paramsIn,line))
    {
      //std::cout<<line<<std::endl;
      params[lineNum] = std::stod(line);
      ++lineNum;
    }
  
  float mean = params[0];
  float RMS = params[1];
  float pmt1_threshold = params[2];
  float pmt2_threshold = params[3];

  std::cout<<"Mean: "<<mean<<" and RMS: "<<RMS<<std::endl;
  std::cout<<"Thresholds: PMT1: "<<pmt1_threshold<<" and PMT2: "
	   <<pmt2_threshold<<std::endl;
  
  //float mean = 5.79815;
  //float RMS = 3.62032;

  //User dependent parameters in volts
  //float pmt2_threshold = std::stof(argv[3]);
  //float pmt1_threshold = std::stof(argv[2]);

  //Open the file
  //const char* name = argv[1];
  TFile *file_in = new TFile("calibrationfile.root");
  TTree *tree = (TTree*)file_in->Get("pulsetree");
  
  int start_time;
  int end_time;
  int matches = 0;
  //Places holders for event information                                 
  PixelData::SubSystems::Event *event = new PixelData::SubSystems::Event();
  TBranch *branch  = tree->GetBranch("event");
  branch->SetAddress(&event);

  for(Long64_t eventNum=0;eventNum<tree->GetEntries();eventNum++){
    branch->GetEntry(eventNum);

    int eventNumber = event->EventNumber();

    float peaktime_pmt1=-99999;
    float peaktime_pmt2=-99999;

    float peaktime_pmt1_nocut = -99999;
    float peaktime_pmt2_nocut = -99999;

    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();

    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end(); ++channel){

      if(channel->channel == 3){
	float peakHeight = TMath::Abs(channel->maxVolt - channel->baseVolt);
	if (peakHeight > pmt1_threshold){
          peaktime_pmt1 = channel->peaktimeSec;
        } else {
          std::cout<<"PMT1: Peak: "<<peakHeight<<" and Threshold: "
                   <<pmt1_threshold<<std::endl;	
        }
      }

      if(channel->channel == 4){
	float peakHeight = TMath::Abs(channel->maxVolt - channel->baseVolt);
	if (peakHeight > pmt2_threshold){
          peaktime_pmt2 = channel->peaktimeSec;
        } else {
          std::cout<<"PMT2: Peak: "<<peakHeight<<" and Threshold: "
                   <<pmt2_threshold<<std::endl;	
        }
      }

      if(eventNum==0 && channel->channel == 4){
	start_time = channel->timestamp.first; 
      }

      if(eventNum==tree->GetEntries()-1 && channel->channel == 4){
	end_time = channel->timestamp.first;
      }

      //Print out the timestamp.
      //std::cout << "There are: " << channel->timestamp.first << " seconds since 1970 and " << channel->timestamp.second << " nanoseconds in that second" << std::endl;
    }
    
    //If they are not initialised correctly this is also set to -99999 
    if(peaktime_pmt1==-99999 || peaktime_pmt2==-99999){
      std::cout<<"No peaks found in event: "<<eventNumber<<std::endl; 
      //std::cout<<"PeakTimes: PMT1:"<<peaktime_pmt1<<" and PMT2: "
      //         <<peaktime_pmt2<<std::endl;
    } else {
      float time_diff = (peaktime_pmt1 - peaktime_pmt2)*1e9; 
      float true_time_diff = TMath::Abs(mean - time_diff);
      if(true_time_diff <  5*RMS){
	++matches;
      } else {
	std::cout<<"No Coincidence: Event: "<<eventNumber<<" PMT1: "<< peaktime_pmt1<<" PMT2: "
	         <<peaktime_pmt2<<" Diff: "<<true_time_diff<<std::endl;	
      }
    }
  }

  float time = end_time - start_time; 
 
  std::cout << "\n" << "start_time: " << start_time << " end_time:" << end_time << " time: " << time/60 <<" mins" << std::endl;

  //Calculate the rate 
  float rate = matches/time;
  float rateUncert = rate/TMath::Sqrt(matches);
  std::cout << "Cosmic rate: " << rate << " +- " << rateUncert << " Hz"  << std::endl; 
 
  float dark = tree->GetEntries() - matches;
  float darkRate = dark/time;
  float darkUncert = darkRate/TMath::Sqrt(dark);
  std::cout << "enteries: " << tree->GetEntries() << " dark sig: " << dark << " dark rate: " << darkRate << " +- " << darkUncert << " Hz" << std::endl;

  return 0; 
}
