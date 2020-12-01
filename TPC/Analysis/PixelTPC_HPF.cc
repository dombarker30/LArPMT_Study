//Decoder Includes                                                                                                                         
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

//Root Includes    
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TVirtualFFT.h"
#include "TMath.h"
#include "TH1.h"

//C++ Includes 
#include <fstream> 
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <getopt.h>

//A low pass filter on the code.

//#############################
//######## WARNING !!! ########
//#############################

//Dom Has been lazy and not coded easily change of parameters for the removal. 
//It is assumed there is a decimation of 2^8 of the DAQ hence the 1/4.096e-6 = 62.5e6/2^6

int HPF(std::string& filename,int& current_eventnum, int process_events);

int main(int argc, char* argv[]){

  std::vector<std::string> inputfiles;
  std::vector<std::string> txtfiles;

  int process_events = -1; 
  int current_eventnum = 0;

  //Get the options
  static struct option long_options[] = {
    {"inputfile"     , required_argument,  0,  'i' },
    {"filelist"      , required_argument,  0,  's' },
    {"process_events", no_argument,  0,  'n' },
    {0, 0, 0, 0}
  };

  int opt = 0;
  int c = 0;
  while ((opt = getopt_long_only(argc, argv,"", long_options, &c )) != -1) {
    switch (opt) {
    case 'i' : inputfiles.push_back(optarg);
      break;
    case 's' : txtfiles.push_back(optarg);
      break;
    case 'n' : process_events = std::stoi(optarg);
      break;
    default: 
      std::cout << "Usage:  -s .txtfile or/and -i .root. -n for the number of events to be analysed. " << std::endl;
      return 1;
    }
  }

    if(inputfiles.size() == 0 && txtfiles.size() == 0){
    std::cout << "please give an inputfile with -i *.root or -s .txt" << std::endl;
    return 1;
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
    
    int err = HPF(inputfiles[i],current_eventnum,process_events);

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

	  int err = HPF(inputfile,current_eventnum,process_events);
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

  
int HPF(std::string& filename,int& current_eventnum, int process_events){


  //Parameters to remove the noise.
  double fFrequencyCutOff = 400e3; //Below this frequency peaks above the threshold are not removed. Heed warning.
  double fPeakCutOff      = 10;    //ADC cut off for the peaks.
  

  if(current_eventnum > process_events && process_events > 0){return 0;}

  ++current_eventnum;

  
  PixelData::TPC::Event* Event;

  //Output file
  size_t suff = filename.find(".");
  TFile * outputfile = new TFile(TString::Format("%s_HPF.root",filename.substr(0,suff).c_str()),"RECREATE");
  TTree * outputtree = new TTree("EventTree","Events");
  outputtree->Branch("Events",&Event,16000,99);


  //File Inputs
  const char* filechar = filename.c_str();
  TFile *inputfile = new TFile(filechar);
  TTree *EventTree = (TTree*)inputfile->Get("EventTree");

  //Event Holder
  PixelData::TPC::Event *event  = new PixelData::TPC::Event();
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
      const std::vector<double> Waveform = channel->GetWaveform();

      //Get the RMS 
      double sum_x = 0; 
      double sum_2x = 0; 
      for(auto const& tick: Waveform){
	sum_x += tick;
	sum_2x += tick*tick;
      }
      double n_std = Waveform.size();
      double RMS = TMath::Sqrt(((sum_2x/n_std) - (sum_x/n_std)*(sum_x/n_std)));


      //Waveform  histograms. Slow don't care. 
      TH1D* wf_hist = new TH1D("wf_hist","wf_hist",NADC+200,0,NADC+200); 

      int adc_it=100;
      for(auto const& tick: Waveform){
	  //Add the waveform to a histogram
	  wf_hist->Fill(adc_it,(tick - Pedestal));
	  ++adc_it;
      }

      //Padd out the waveform to stop bad man fft effects.
      for(adc_it=0;adc_it<100;++adc_it){
	wf_hist->Fill(adc_it,0);
      }
      for(adc_it = NADC+100 ;adc_it<NADC+200;++adc_it){
	wf_hist->Fill(adc_it,0);
      }

      //Get the magnitude
      TH1 * fft =0;
      TVirtualFFT::SetTransform(0);
      fft = wf_hist->FFT(fft, "MAG");
      
      //Get the fft arrays
      TVirtualFFT *fft_vert = TVirtualFFT::GetCurrentTransform();
      Double_t *re_full = new Double_t[NADC+200];
      Double_t *im_full = new Double_t[NADC+200];
      fft_vert->GetPointsComplex(re_full,im_full);

      //Normalise to the correct ADC
      for(int i=0; i<NADC+200; ++i){
	*(re_full+i) /= TMath::Sqrt(NADC+200);
	*(im_full+i) /= TMath::Sqrt(NADC+200);
      }

      //Remove loud peaks 
      for(int i=0; i<NADC+200; ++i){

	//Repad
	if(i<100 || i>NADC+100){
	  *(re_full+i) = 0;
	  *(im_full+i) = 0;
	  continue;
	}


	//If its below some frequency don't consider for removal
	if(i/4.096e-6 < fFrequencyCutOff){continue;}
	if(NADC/4.096e-6 - i/4.096e-6 < fFrequencyCutOff){continue;}

	//Remove peaks greater than the cut off
	if(fft->GetBinContent(i+1)/TMath::Sqrt(NADC+200) > (fPeakCutOff + RMS)){
	  *(re_full+i) = 0;
	  *(im_full+i) = 0;
	}
      }
	  

      //Now let's make a backward transform:
      Int_t n = NADC+200;
      TVirtualFFT *fft_back = TVirtualFFT::FFT((Int_t) 1,&n, "C2R M K");
      fft_back->SetPointsComplex(re_full,im_full);
      fft_back->Transform();

      //Fill the waveform
      TH1 *hb = 0;
      hb = TH1::TransformHisto(fft_back,hb,"Re");
      std::vector<double> waveform;
      for(int i=0; i<NADC+200; ++i){
	//Ignore the padding
	if(i<100 || i>NADC+100){continue;}

	//Boom Done it mate.
	waveform.push_back(hb->GetBinContent(i+1)/TMath::Sqrt(NADC+200));
      }

      //Refill the channel data with the knew waveform
      Event = event;
      int channelnum = channel->GetChannelNumber();
      Event->ClearChannel(channelnum);
      std::string channelID =  channel->GetChannelID();
      PixelData::TPC::ChannelInfo ChannelData = PixelData::TPC::ChannelInfo(channelnum, channelID, NADC, 0, waveform);
      Event->AddChannel(ChannelData);

      //Delete everything
      delete wf_hist;
      delete fft;
      delete hb;
      delete fft_back;

    }
    outputtree->Fill();
  }

  outputfile->cd();
  outputtree->Write();
  outputfile->Close();
  outputfile->Delete();
  

  return 0;

}
