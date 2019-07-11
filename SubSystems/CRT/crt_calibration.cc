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
  std::cout<<"Test"<<std::endl;
  std::ofstream paramsOut ("crt_calibrationOut.txt");

  //User depedent parameters
  float pmt2_threshold =0.2;
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
  std::map<int,TH1F*> IntegralHist_map;
  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();

    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end(); ++channel){

      if(i==0){
	//Create the histograms
	std::stringstream ChargeHist_sstm;
	std::string ChargeHist_string = "Charge Distrubtion Channel" + std::to_string(channel->channel);;
	const char* ChargeHist_name =  ChargeHist_string.c_str(); 
	ChargeHist_map[channel->channel] = new TH1F(ChargeHist_name,ChargeHist_name,500,-1,1);

	std::string IntegralHist_string = "Integral Hist Channel" + std::to_string(channel->channel);
	const char* IntegralHist_name   = IntegralHist_string.c_str(); 
	IntegralHist_map[channel->channel] = new TH1F(IntegralHist_name,IntegralHist_name,500,0,2000);
      }
      ChargeHist_map[channel->channel]  ->Fill(channel->maxVolt - channel->baseVolt);
      IntegralHist_map[channel->channel]->Fill(channel->integral);
    }
  }

  //Write the Charge distribution to file 
  for(std::map<int,TH1F*>::iterator hist_iter=ChargeHist_map.begin(); hist_iter!=ChargeHist_map.end(); ++hist_iter){
    (hist_iter)->second->GetXaxis()->SetTitle("Voltage (V)");
    (hist_iter)->second->GetYaxis()->SetTitle("Enteries");
    (hist_iter)->second->Write();
    (hist_iter)->second->Delete();
  }

  //Write the Integral histogram to file. 
  for(std::map<int,TH1F*>::iterator hist_iter=IntegralHist_map.begin(); hist_iter!=IntegralHist_map.end(); ++hist_iter){
    (hist_iter)->second->GetXaxis()->SetTitle("Integral (ADC*ns)");
    (hist_iter)->second->GetYaxis()->SetTitle("Enteries");
    (hist_iter)->second->Write();
    (hist_iter)->second->Delete();
  }

  bool FillHist1 = false;
  bool FillHist2 = false; 
      
  TH1D *TimeHist = new TH1D("Time Difference","Time Difference",10001,-5000,5000);
  //Loop over enteries
  for(Long64_t i=0;i<tree->GetEntries();i++){
    
    float peaktime_pmt1;
    float peaktime_pmt2;

    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end();++channel){
      //Look at the first channel (PMT yellow) and as long as the signal is greater than the dark current cut off (worked out above) then lets say its a muon 
      if(channel->channel == 3 && TMath::Abs(channel->maxVolt - channel->baseVolt) > pmt1_threshold){
	peaktime_pmt1 = channel->peaktimeSec;
	FillHist1 = true;
      }
      if(channel->channel == 4 && TMath::Abs(channel->maxVolt - channel->baseVolt) > pmt2_threshold){
	peaktime_pmt2 = channel->peaktimeSec;
	FillHist2 = true;
      }  
    }
    bool FillHist = FillHist1 && FillHist2; 
    if(FillHist){TimeHist->Fill((peaktime_pmt1-peaktime_pmt2)*1e9);} //std::cout << "peaktime_pmt1-peaktime_pmt2: " << (peaktime_pmt1-peaktime_pmt2)*1e9 << std::endl;}
    event->Clear();
  }

  //Fit a guassian to data to get limits
  float MP     = TimeHist->GetXaxis()->GetBinCenter(TimeHist->GetMaximumBin());
  float RMS    = TimeHist->GetRMS();
  float Height = TimeHist->GetBinContent(TimeHist->GetMaximumBin());  
  TF1 *time_fit = new TF1("time_fit","gaus");
  float MPMin = MP - 5;
  float MPMax = MP + 5; 
  time_fit->SetParameter(0,Height);
  time_fit->SetParameter(1,MP);
  time_fit->SetParameter(2,5);
  time_fit->SetParLimits(0,Height-100,Height+100);
  time_fit->SetParLimits(1,MP-5,MP+5); //Allow to vary by +-5ns 
  time_fit->SetParLimits(2,0,10);
  TimeHist->Fit(time_fit,"B");
  float mean = time_fit->GetParameter(1);
  RMS = time_fit->GetParameter(2);

  //Most Proable is the time difference due to the electrons and RMS gives some indication on the error.
  TimeHist->GetXaxis()->SetTitle("Time Difference (ns)");
  TimeHist->GetYaxis()->SetTitle("Enteries");
  TimeHist->Write();

  std::cout << "MP: " << MP << std::endl;
  std::cout << "mean: " << mean << " RMS: " << RMS << " mean2: " << TimeHist->GetMean() << std::endl;


  if (paramsOut.is_open()){ 
    paramsOut<<mean<<"\n";
    paramsOut<<RMS<<"\n";
    paramsOut<<pmt1_threshold<<"\n";
    paramsOut<<pmt2_threshold<<"\n";
    paramsOut.close();
  }

  //Look for a new threshold to run with 
  TH1D *ThresholdHist_CRT3 = new TH1D("Threshold Hist 3","Threshold Hist 3",500,-1,1);
  TH1D *ThresholdHist_CRT4 = new TH1D("Threshold Hist 4","Threshold Hist 4",500,-1,1);

  

  //Loop over enteries
  for(Long64_t i=0;i<tree->GetEntries();i++){
    branch->GetEntry(i);
    std::vector<PixelData::SubSystems::ChannelInfo> Channels = event->GetChannels();
    
    float peaktime_pmt1;
    float peaktime_pmt2;
    
    float maxvoltage_pmt1;
    float maxvoltage_pmt2;
  
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end();++channel){
      if(channel->channel == 3){
	peaktime_pmt1 = channel->peaktimeSec;
	maxvoltage_pmt1  = channel->maxVolt - channel->baseVolt;
      }
      if(channel->channel == 4){
	peaktime_pmt2 = channel->peaktimeSec;
	maxvoltage_pmt2  = channel->maxVolt - channel->baseVolt;
      }
    }
    
    for(std::vector<PixelData::SubSystems::ChannelInfo>::iterator channel=Channels.begin();channel!=Channels.end();++channel){
      if(TMath::Abs((peaktime_pmt1-peaktime_pmt2)*1e9 - mean) < 3*RMS){
	
	if(channel->channel == 3){
	  ThresholdHist_CRT3->Fill(channel->maxVolt - channel->baseVolt);
	}
	if(channel->channel == 4){
	  ThresholdHist_CRT4->Fill(channel->maxVolt - channel->baseVolt);
	}
      }
    }
  }

  //Draw the threshold histograms
  ThresholdHist_CRT3->GetXaxis()->SetTitle("Peak Voltage (V)");
  ThresholdHist_CRT3->GetYaxis()->SetTitle("Enteries");
  ThresholdHist_CRT3->Write();

  ThresholdHist_CRT4->GetXaxis()->SetTitle("Peak Voltage (V)");
  ThresholdHist_CRT4->GetYaxis()->SetTitle("Enteries");
  ThresholdHist_CRT4->Write();


  file_out->Close();

  return 0; 
}
