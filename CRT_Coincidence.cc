#include "TFile.h"
#include "TTree.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <iostream>

void CRT_Coincidence()
{
  
  TFile *File = TFile::Open("/home/argonshef/LArAnalysis/CRTData/LArPMT20181114_141042.root");

  if ( File->IsOpen() ) printf("File opened successfully\n");
  
  TTreeReader pulseReader("pulsetree",File);
  TTreeReaderArray<int> channel(pulseReader, "channel");

  //TTreeReaderValue<Float_t> timestamp(pulseReader,"timestamp");
  //TTreeReaderValue<Float_t> maxVolt(pulseReader,"maxVolt");
  
  
  while (pulseReader.Next()){
    //    printf("%i\n",channel[0]);
    // printf("Ch. %i, time: %d, max volts: %d\n",*channel, *timestamp, *maxVolt);
  }
  // for i in channel.GetSize()
  Int_t nEvents = channel.GetSize();
  printf("%i\n",nEvents);
  
  

  File->Close();

  return;
}
