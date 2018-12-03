#include <iostream>
#include <string.h>
#include <cmath>

#include "TROOT.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TF1.h"
#include "TF1Convolution.h"
#include "TDirectory.h"
#include "TCanvas.h"

#include "TRandom.h"


int main (int argc, char *argv[]){

  //Make sure on output file and input is specified 
  if(argc != 3) {
    std::cout << "Please specify input and output .root filenames" << std::endl;
    std::cout << "To run the script the command line must have the form: ./distribution_fit file_in.root file_out.root" << std::endl;
    return 1;
  }

  char* inputFileName = argv[1];
  char* outputFileName = argv[2];

  //Defined for the run in the daq
  float Bit = 8; //Bits of the DAQ
  float NBits = std::pow(2,Bit);
  float FSR = 1; //Volts paramterisied in params.txt
  float Offset = 0.25; //Volts paramterised in params.txt 
  float ampVoltMax = FSR - Offset;

  //Define hitstograme that will be fitted
  TH1F* AmpADC_hist = new TH1F("AmpADC_hist","Amplitdue (ADC)",NBits,0,NBits);
  TH1F* AmpVolt_hist = new TH1F("AmpVolt_hist","Ampltidue (Volt)",NBits,0,ampVoltMax); 



  //------------------------ Reading input file -------------------------                                                                                                      

  TFile *f_in = new TFile(inputFileName, "READ");
  if (!f_in) { std::cout << "Unable to open the root file." << std::endl; return 1; }
  std::cout << "------- Reading file \"" << inputFileName << "\" -------" << std::endl;
  
  TTree *t1 = (TTree*)f_in->Get("pulsetree");
  
  Int_t channel, source;
  Float_t amplitudeAdc, amplitudeVolt;  

  Float_t ampADC_min = 9999;
  Float_t ampVolt_min = 9999;
  
  t1->SetBranchAddress("amplitudeVolt",&amplitudeVolt);
  t1->SetBranchAddress("amplitudeAdc",&amplitudeAdc);
  
  //Reading tree ---                                            
  Long64_t nentries = t1->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    t1->GetEntry(i);
    AmpADC_hist->Fill(amplitudeAdc);
    AmpVolt_hist->Fill(amplitudeVolt);
    
    if(amplitudeAdc < ampADC_min){ampADC_min = amplitudeAdc;}
    if(amplitudeVolt < ampVolt_min){ampVolt_min = amplitudeVolt;}
  }
  
  f_in->Close();
  
  //---------------------- Fit the Spectrum ------------------------------
  
  TFile *f_out = new TFile(outputFileName, "RECREATE");    
  if (!f_out) { std::cout << "Unable to open the output root file." << std::endl; return 1; }
  std::cout << "------- Fitting Distribution " <<  "-------" << std::endl;

  AmpADC_hist->SetAxisRange(ampADC_min, NBits, "X");
  AmpVolt_hist->SetAxisRange(ampVolt_min, ampVoltMax, "X");

  //Get the initial guesses by fitting seperately.
  TF1 *exp_fitADC = new TF1("exp_fitADC","expo");
  AmpADC_hist->Fit("exp_fitADC");
  float exp_gradADC = exp_fitADC->GetParameter(1);
  float exp_constADC = exp_fitADC->GetParameter(0);

  TF1 *exp_fitVolt = new TF1("exp_fitVolt","expo");
  AmpVolt_hist->Fit("exp_fitVolt");
  float exp_gradVolt = exp_fitVolt->GetParameter(1);
  float exp_constVolt = exp_fitVolt->GetParameter(0);


  TF1 *spectrum_fitADC = new TF1("spectrum_fitADC","gaus(0)+expo(3)"); 
  //Set the initial guess of the guassian height width and mean by eye. 
  spectrum_fitADC->SetParameters(100,65,25,exp_constADC,exp_gradADC);
  
  TF1 *spectrum_fitVolt = new TF1("spectrum_fitVolt","gaus(0)+expo(3)"); 
  //Set the initial guess of the guassian height width and mean by eye. 
  spectrum_fitVolt->SetParameters(100,65,25,exp_constVolt,exp_gradVolt);


  //fit
  TCanvas *ADCSpectrum_c = new TCanvas("ADCSpectrum_c","ADC Spectrum",900,600);
  ADCSpectrum_c->cd();
  AmpADC_hist->Fit("spectrum_fitADC");
  AmpADC_hist->Write();

  TCanvas *VoltSpectrum_c = new TCanvas("VoltSpectrum_c","Volt Spectrum",900,600);
  VoltSpectrum_c->cd();
  AmpVolt_hist->Fit("spectrum_fitVolt");
  AmpVolt_hist->Write();


  return 0;
}
