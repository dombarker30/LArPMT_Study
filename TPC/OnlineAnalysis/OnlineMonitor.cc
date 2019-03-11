#include "OnlineMonitor.hh"
#include "TH1.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <math.h>

PixelData::TPC::OnlineMonitor::OnlineMonitor(){
  Channel       = -99999;
  Baseline      = -99999;
  BaseRMS       = -99999;
  AvgPeakHeight = -99999;
  MaxPeakHeight = -99999;
  MaxPeakTime   = -99999;
  NumHits       = -99999;
  EventNum      = -99999;
  TimeStampS    = -99999;
  TimeStampN    = -99999;

};

PixelData::TPC::OnlineMonitor::OnlineMonitor(int Chan, float Base, float RMS, float AvgPeak, float MaxPeak, int MaxTime, int Hits, int Evt, int TimeS, int TimeN){
  Channel       = Chan;
  Baseline      = Base;
  BaseRMS       = RMS;
  AvgPeakHeight = AvgPeak;
  MaxPeakHeight = MaxPeak;
  MaxPeakTime   = MaxTime;
  NumHits       = Hits;
  EventNum      = Evt;
  TimeStampS    = TimeS;
  TimeStampN    = TimeN;
};


PixelData::TPC::OnlineMonitor::OnlineMonitor(int Chan, float Base, float RMS, int Evt, int TimeS, int TimeN){
  Channel       = Chan;
  Baseline      = Base;
  BaseRMS       = RMS;
  AvgPeakHeight = -99999;
  MaxPeakHeight = -99999;
  MaxPeakTime   = -99999;
  NumHits       = -99999;
  EventNum      = Evt;
  TimeStampS    = TimeS;
  TimeStampN    = TimeN;

};


PixelData::TPC::OnlineMonitor PixelData::TPC::RunOffline(const std::vector<int> &waveform, int Evt, int Chan, int Time, bool CalcPeaks, bool SecondPass, bool verbose){
  //std::cout<<"##### Called Offline #####"<<std::endl;
  //auto t1 = std::chrono::high_resolution_clock::now();

  OnlineMonitor first; 
  if (CalcPeaks){ 
    if (SecondPass){
      first = RunSecondPass(waveform, Evt, Chan, Time, verbose);
    } else {
      first = RunPeaks(waveform, Evt, Chan, Time, verbose);
    }
  } else {
    first = RunBaseLine(waveform, Evt, Chan, Time, verbose);
  }

  //auto t2 = std::chrono::high_resolution_clock::now();
  //auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  //std::cout<<"Time taken: "<<duration<<std::endl;

  return first;
}

PixelData::TPC::OnlineMonitor PixelData::TPC::RunOnline(uint16_t *waveform,int nADC, int Evt, int Chan, int Time, bool CalcPeaks, bool SecondPass, bool verbose){
//PixelData::TPC::OnlineMonitor PixelData::TPC::RunOnline(unsigned short *waveform, bool CalcPeaks, bool SecondPass, bool verbose){     
  // std::cout<<"##### Called Online #####"<<std::endl;
  //auto t1 = std::chrono::high_resolution_clock::now();                         

  //std::cout<<nADC<<std::endl;

  OnlineMonitor first;
  //const int size = sizeof(waveform)/sizeof(waveform[0]);
  const std::vector<int> waveformV(waveform, waveform+nADC);

  //std::cout<<waveformV.size()<<std::endl;

  if (CalcPeaks){
    if (SecondPass){
      first = RunSecondPass(waveformV, Evt, Chan, Time, verbose);
    } else {
      first = RunPeaks(waveformV, Evt, Chan, Time, verbose);
    }
  } else {
    first = RunBaseLine(waveformV, Evt, Chan, Time, verbose);
  }
  /*
  auto t2 = std::chrono::high_resolution_clock::now();                   
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();        
  std::cout<<"Time taken: "<<duration<<std::endl;                              */                    
  return first;
}


PixelData::TPC::OnlineMonitor  PixelData::TPC::RunBaseLine(const std::vector<int> &wfm, int Evt, int Chan, int Time, bool fverbose){
  //std::cout<<"Called: "<< wfm.size()<<std::endl;
  TH1F* Ped = new TH1F("PedHist","PedHist",4096,0000,4095);
  for (int i=0;i<wfm.size(); ++i){
    Ped->Fill(wfm.at(i));
  }
  float PedMean = Ped->GetMean();
  float PedRMS  = Ped->GetRMS();

  if (fverbose){
    std::cout<<"Entries: "<<Ped->GetEntries()<<std::endl;
    std::cout<<"Ped: "<<PedMean<<std::endl;
    std::cout<<"RMS: "<<PedRMS<<std::endl;
  }

  OnlineMonitor online = OnlineMonitor(Chan,PedMean,PedRMS,Evt,Time,Time);
  Ped->Delete();
  return online;
}
  
PixelData::TPC::OnlineMonitor  PixelData::TPC::RunPeaks(const std::vector<int> &wfm, int Evt, int Chan, int Time, bool fverbose){
  // Have many options: Need to test for speed on actual waveforms
  // - Run PedStart then Ped or just use Ped for both
  // - Start the second loop at either 0 or trig
  // - Fill ped in second loop always or only when !Peak

  TH1F* PedStart = new TH1F("PedStartHist","PedStartHist",4096,0,4095);
  TH1F* Ped = new TH1F("PedHist","PedHist",4096,0,4095);

  int trig = wfm.size()/10;
  if (fverbose)  std::cout<<"#####  First Pass    #####"<<std::endl;

  for (int i=0;i<trig; ++i){
    PedStart->Fill(wfm.at(i));
  }

  float PedStartMean = PedStart->GetMean();
  float PedStartRMS  = PedStart->GetRMS();

  if (fverbose)
    {
      std::cout<<"Entries: "<<PedStart->GetEntries()<<std::endl;
      std::cout<<"Ped: "<<PedStartMean<<std::endl;
      std::cout<<"RMS: "<<PedStartRMS<<std::endl;
    }
  PedStart->Delete();

  float MaxPeak   = -99999;
  float AvgPeak   = -99999;
  int   NumPeaks  = -99999;
  int   MaxTime   = -99999;

  float PeakSum    = 0;
  float PeakHeight = -99999;
  int PeakCounter  = 0;
  bool isPeak      = false;

  if (fverbose)
    { 
      std::cout<<"##### Second Pass    #####"<<std::endl;
      std::cout<<"##### Peaks          #####"<<std::endl;
    }

  for (int i=trig;i<wfm.size(); ++i){
    //std::cout<<i<<std::endl;
    if (abs(PedStartMean-wfm.at(i))>(2*PedStartRMS)) // Start Peak
      {
	isPeak = true;
	//std::cout<<"Peak at: "<<i<<" with height: "<<wfm.at(i)<<std::endl;
	if (abs(PedStartMean-wfm.at(i))>PeakHeight)
	  {
	    PeakHeight = abs(PedStartMean-wfm.at(i));
	  }
      } 
    // End Peak if wfm drops below threshold
    else if (isPeak && (abs(PedStartMean-wfm.at(i))<(1*PedStartRMS)))
      {
	PeakSum  += PeakHeight;  
        PeakCounter++;
	if (PeakHeight>MaxPeak) 
	  {
	    MaxPeak = PeakHeight;
	    MaxTime  = i;
	  }
	PeakHeight = -99999;
	isPeak = false; 
      }
      else if (!isPeak) // Only fill ped outside of peaks
      {
	Ped->Fill(wfm.at(i));
      }
  }
  
  
  float PedMean = Ped->GetMean();
  float PedRMS  = Ped->GetRMS();
  if (fverbose)
    {
      std::cout<<"##### Averages       #####"<<std::endl;
      std::cout<<"Entries: "<<Ped->GetEntries()<<std::endl;
      std::cout<<"Ped: "<<PedMean<<std::endl;
      std::cout<<"RMS: "<<PedRMS<<std::endl;
    }
  if (PeakCounter==0) {
    if (fverbose)
      {
	std::cout<<"No peaks found"<<std::endl;
      }
  }else{ 
    AvgPeak  = PeakSum/PeakCounter;
    NumPeaks = PeakCounter;
    //MaxPeak  = PeakHeight;
    if (fverbose)
      {
	std::cout<<"MaxPeak: "<<MaxPeak<<std::endl;
	std::cout<<"MaxTime: "<<MaxTime<<std::endl;
	std::cout<<"AvgPeak: "<<AvgPeak<<std::endl;
	std::cout<<"NumPeaks: "<<NumPeaks<<std::endl;
      }
  };

  OnlineMonitor online = OnlineMonitor(Chan,PedMean,PedRMS,AvgPeak,MaxPeak,MaxTime,NumPeaks,Evt,Time,Time);

  Ped->Delete();

  //OnlineMonitor online = OnlineMonitor(2,2,2,2,2,2,2,2,2);

  if (fverbose) std::cout<<"#####      DONE      ##### \n "<<std::endl;
  return online;
};

PixelData::TPC::OnlineMonitor  PixelData::TPC::RunSecondPass(const std::vector<int> &wfm, int Evt, int Chan, int Time, bool fverbose){
  OnlineMonitor online;
  return online;
};


