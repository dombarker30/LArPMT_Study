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

//Code Take the fourier tranform of the data and averages them.

int FourierTransform(std::string& filename,int& current_eventnum, int process_events);

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
    
    int err = FourierTransform(inputfiles[i],current_eventnum,process_events);

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

	  int err = FourierTransform(inputfile,current_eventnum,process_events);
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

  
int FourierTransform(std::string& filename,int& current_eventnum, int process_events){

  if(current_eventnum > process_events && process_events > 0){return 0;}

  ++current_eventnum;


  //File Inputs
  const char* filechar = filename.c_str();
  TFile *inputfile = new TFile(filechar);
  TTree *EventTree = (TTree*)inputfile->Get("EventTree");

  //Event Holder
  PixelData::TPC::Event *event = new PixelData::TPC::Event();
  TBranch *branch  = EventTree->GetBranch("Events");
  branch->SetAddress(&event);

  //FFT histograms.  
  std::map<int,TH1D*> fft_avg_ch;
  std::map<int,int> avg_ch;

  std::vector<TH1D*> wf_hists;

  for(Long64_t i=0;i<EventTree->GetEntries();i++){

    if(i==10){continue;}

    //Get the Event Information
    branch->GetEntry(i);

    std::cout << "Event Number: " << event->GetEventNumber() << std::endl;

    //Get the channel Information
    const std::vector<PixelData::TPC::ChannelInfo> Channels = event->GetChannels();
    for(std::vector<PixelData::TPC::ChannelInfo>::const_iterator channel=Channels.begin(); channel!=Channels.end(); ++channel){

      const int NADC            = channel->GetNADC();
      const float Pedestal      = channel->GetPedestal();
      const std::vector<double> Waveform = channel->GetWaveform();

      //Initial average FFT
      if(fft_avg_ch.find(channel->GetChannelNumber()) == fft_avg_ch.end()){
	std::string chan_string = "Average FFT for Channel " + std::to_string(channel->GetChannelNumber()); 
	const char* chan_name = chan_string.c_str(); 
	fft_avg_ch[channel->GetChannelNumber()] =  new TH1D(chan_name,chan_name,NADC,0,NADC);  
	for(int i=0; i<NADC; ++i){
	  fft_avg_ch[channel->GetChannelNumber()]->SetBinContent(i,0);
	}
      }

      //Waveform  histograms. Slow don't care. 
      TH1D* wf_hist = new TH1D("wf_hist","wf_hist",NADC,0,NADC); 

      int adc_it=0;
      for(auto const& tick: Waveform){
	//Add the waveform to a histogram
	wf_hist->Fill(adc_it,(tick - Pedestal)*217.6688);
	++adc_it;
      }
   
      //   if(channel->GetChannelNumber() == 1){ 
      //	TH1D* clone = (TH1D*) wf_hist->Clone();
      //	wf_hists.push_back(clone);
      //      }

      //FFt Histogram
      TH1 * fft =0;
      TVirtualFFT::SetTransform(0);
      fft = wf_hist->FFT(fft, "MAG");


      //Add to the average fft.
      fft_avg_ch[channel->GetChannelNumber()]->Add(fft);
      ++avg_ch[channel->GetChannelNumber()];

      delete wf_hist;
      delete fft;

    }
  }

  size_t suff = filename.find(".");
  TFile* outputfile = new TFile(TString::Format("%s_FFT.root",filename.substr(0,suff).c_str()),"RECREATE");
  outputfile->cd();

  //  for(auto const& hist: wf_hists){
  //    hist->Write();
  //  }

  std::map<int,TH1D*> fft_avg_ch_scaled;
  for(auto const& chan: fft_avg_ch){
    
    chan.second->Scale(1/((float) avg_ch[chan.first]* (float) chan.second->GetNbinsX()));

    std::string chan_string = "Average scaled FFT for Channel " + std::to_string(chan.first); 
    const char* chan_name = chan_string.c_str(); 
    fft_avg_ch_scaled[chan.first] =  new TH1D(chan_name,chan_name,chan.second->GetNbinsX(),0,1/4.096e-6);    
    for(int i=0; i<chan.second->GetNbinsX(); ++i){
      fft_avg_ch_scaled[chan.first]->SetBinContent(i,chan.second->GetBinContent(i));
    }
  }

    for(auto const& chan: fft_avg_ch_scaled){
      chan.second->Write();
    }

  return 0;

}
