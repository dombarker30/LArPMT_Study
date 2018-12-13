//####################################################################
//Written by Dominic Barker 20/06/18   
//This is a quick calbriation module to measure time difference 
//and peak height between the two PMT electronics readout chains. It
//uses the calibration file which is just the two CRT panels on top of each other 
//if we need the waveform.cc again I think we should change the root 
//output structure.
//###################################################################

//C++ Includes 
#include <fstream>
#include <iostream>
#include <vector> 
#include <sstream>
#include <string>

//Root Includes 
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TFile.h"
#include "TTree.h"

//Decoder Includes 
#include "../DAQDecoder/ChannelInfo.h"
#include "../DAQDecoder/Event.h"

int main(){

  //User depedent parameters
  float pmt2_threshold =0.01;
  float pmt1_threshold =0.01;

  //Read the waveform file 
  TFile *file_in = new TFile("calibrationfile.root");
  TTree *tree = (TTree*)file_in->Get("pulsetree");

  //Places holders for event information
  PixelData::SubSystems::Event *event = new PixelData::SubSystems::Event();
  TBranch *branch  = tree->GetBranch("event");

  branch->SetAddress(&event);

  //File for writing.
  TFile *file_out = new TFile("calibrationfile_hist.root","RECREATE");
  //I believe usually dark current pulse coincidences are rather rare but I need to check the rate and compare. Also the muons are significantly larger so we can make a reasonable cut on triggers

  //Loop over enteries in order to get the max charge histograms and the trigger threshold
  std::map<int,TH1F*> ChargeHist_map;
  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end(); ++channel){

      if(i==0){
	//Create the histograms
	std::stringstream ChargeHist_sstm;
	std::string ChargeHist_string = ChargeHist_sstm.str();
	const char*  ChargeHist_name =  ChargeHist_string.c_str(); 
	ChargeHist_map[channel->channel] = new TH1F(ChargeHist_name,ChargeHist_name,200,-2.5,2.5);
      }
      //      std::cout << "channel->maxVolt: " << channel->maxVolt << std::endl;
      ChargeHist_map[channel->channel]->Fill(channel->maxVolt);
    }
  }

  //Write the Charge distribution to file 
  for(std::map<int,TH1F*>::iterator hist_iter=ChargeHist_map.begin();hist_iter!=ChargeHist_map.end();++hist_iter){
    std::stringstream sstm;
    sstm << "Charge distribution, Channel" <<  hist_iter->first;
    std::string string = sstm.str();
    const char* name = string.c_str();
    (hist_iter)->second->SetTitle(name);
    (hist_iter)->second->GetXaxis()->SetTitle("Charge (V)");
    (hist_iter)->second->GetYaxis()->SetTitle("Enteries");
    (hist_iter)->second->Write();
    (hist_iter)->second->Delete();
  }



  float peaktime_pmt1;
  float peaktime_pmt2;
  bool FillHist1 = false;
  bool FillHist2 = false; 
      
  TH1D *TimeHist = new TH1D("Time Difference","Time Difference",2001,-1000,1000);
  //Loop over enteries
  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end();++channel){
      //Look at the first channel (PMT yellow) and as long as the signal is greater than the dark current cut off (worked out above) then lets say its a muon 
      if(channel->channel == 3 && TMath::Abs(channel->maxVolt) > pmt1_threshold){
	peaktime_pmt1 = channel->peaktimeSec;
	FillHist1 = true;
      }
      if(channel->channel == 4 && TMath::Abs(channel->maxVolt) > pmt2_threshold){
	peaktime_pmt2 = channel->peaktimeSec;
	FillHist2 = true;
      }  
    }
    bool FillHist = FillHist1 && FillHist2; 
    if(FillHist){TimeHist->Fill((peaktime_pmt1-peaktime_pmt2)*1e9); std::cout << "peaktime_pmt1-peaktime_pmt2: " << (peaktime_pmt1-peaktime_pmt2)*1e9 << std::endl;}
    event->Clear();
  }

  //Fit a guassian to data to get limits
  float MP = TimeHist->GetXaxis()->GetBinCenter(TimeHist->GetMaximumBin());
  float RMS = TimeHist->GetRMS();
  TF1 *time_fit = new TF1("time_fit","gaus");
  time_fit->SetParameter(1,MP);
  time_fit->SetParLimits(1,MP-5,MP+5); //Allow to vary by +-5ns 
  TimeHist->Fit(time_fit,"Q");
  float mean = time_fit->GetParameter(1);
  RMS = time_fit->GetParameter(2);

  //Most Proable is the time difference due to the electrons and RMS gives some indication on the error.
  TimeHist->GetXaxis()->SetTitle("Time Difference (ns)");
  TimeHist->GetYaxis()->SetTitle("Enteries");
  TimeHist->Write();


  std::cout << "mean: " << mean << " RMS: " << RMS << " mean2: " << TimeHist->GetMean() << std::endl;

  file_out->Close();

  return 0; 
}
