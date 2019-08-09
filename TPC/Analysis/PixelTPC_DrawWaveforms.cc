//Decoder Includes                                                                                                                         
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

//Root Includes    
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h" 
#include "TGraph.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TApplication.h"

//C++ Includes 
#include <fstream> 
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

//Keyboard hit 
#include "keyb.h"

int DrawWaveforms(std::string& filename, bool fSave);

int main(int argc, char* argv[]){

  std::vector<std::string> inputfiles;
  std::vector<std::string> txtfiles;

  bool fSave = false;

  int process_events = -1; 

  int c;

  //  gStyle->SetTitleW(0.6);

  //Check to see if a input file is a .root (i) or a .txt file (s) or both 
  while ((c=getopt(argc, argv, "i:s:n:save:")) != -1){
    switch (c) {
    case 'i':
      inputfiles.push_back(optarg);
      break;
    case 's':
      txtfiles.push_back(optarg);
      break;
    case 'n':
      process_events = std::stoi(optarg);
    case 'save':
      fSave = true;
    default:
      std::cerr << "You must give an input file" << std::endl; 
      return 1;
    }
  }

  //Check to see if the there is a one to one for tags and files
  if (argc - optind < 0) {
    std::cout << "Usage: " << argv[0] << " -s .txtfile or/and -i .root " << std::endl;
    std::cout << "Once running the code will print out the waveforms for each channel." << std::endl;
    std::cout << "There are several inputs once running n=next event, q=quit and e=go to event number e" << std::endl; 
      
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

    TApplication theApp("App",&argc, argv);    
    int err = DrawWaveforms(inputfiles[i],fSave);

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

	  TApplication theApp("App",&argc, argv);
	  int err = DrawWaveforms(inputfile,fSave);

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

  
int DrawWaveforms(std::string& filename, bool fSave){

  //File Inputs
  const char* filechar = filename.c_str();
  TFile *inputfile = new TFile(filechar);
  TTree *EventTree = (TTree*)inputfile->Get("EventTree");

  //Event Holder
  PixelData::TPC::Event *event = new PixelData::TPC::Event();
  TBranch *branch  = EventTree->GetBranch("Events");
  branch->SetAddress(&event);

  //Map of the channel waveforsm
  std::map<std::string,TGraph*> WaveformGraph;

  //Output file
  size_t suff = filename.find(".");  
  TFile * outputfile;
  if(fSave) outputfile = new TFile(TString::Format("%s_Draw.root",filename.substr(0,suff).c_str()),"RECREATE");


  //Event loop 
  int i=0;

  while(true){

    inputfile->cd();
    
    //Get the Event Information
    std::cout << "i: " << i << std::endl;
    branch->GetEntry(i);
    
    std::cout << " Event Number: " << event->GetEventNumber() << std::endl;

    std::string Canvas_string = "Event " + std::to_string(event->GetEventNumber()) + " Waveforms";
    const char* Canvas_name = Canvas_string.c_str();
    TCanvas *c = new TCanvas(Canvas_name,Canvas_name);
    c->SetCanvasSize(800,10000);
    c->SetWindowSize(800,500);
    c->Divide(2,36,0.001,0.001);
    
    //Get the channel Information
    const std::vector<PixelData::TPC::ChannelInfo> Channels = event->GetChannels();
    
    for(std::vector<PixelData::TPC::ChannelInfo>::const_iterator channel=Channels.begin(); channel!=Channels.end(); ++channel){
      
      const int NADC            = channel->GetNADC();
      const float Pedestal      = channel->GetPedestal();
      const std::vector<int> Waveform = channel->GetWaveform();
      
      //Create the histograms
      std::string Graph_string = "Waveform Channel: " + channel->GetChannelID();
      const char* Graph_name = Graph_string.c_str();
      WaveformGraph[Graph_string]  = new TGraph();
      WaveformGraph[Graph_string]->SetTitle(Graph_name);
      WaveformGraph[Graph_string]->GetXaxis()->SetTitle("Time (tick)");
      WaveformGraph[Graph_string]->GetYaxis()->SetTitle("Amplitude (ADC)");
      // WaveformGraph[Graph_string]->GetXaxis()->SetTickSize(0.01);
      // WaveformGraph[Graph_string]->GetXaxis()->SetTitleSize(0.9);
      // WaveformGraph[Graph_string]->GetXaxis()->SetLabelSize(0.9);
      // WaveformGraph[Graph_string]->GetYaxis()->SetTickSize(0.01);
      // WaveformGraph[Graph_string]->GetYaxis()->SetTitleSize(0.9);
      // WaveformGraph[Graph_string]->GetYaxis()->SetLabelSize(0.6);
      // WaveformGraph[Graph_string]->GetYaxis()->SetLabelOffset(0.05);
      
      //Decide where the histogram should 
      std::string channelID = channel->GetChannelID();
      int chanStrLength = channelID.size();
      if(chanStrLength>5){
	int chanNum = std::stoi(channelID.substr(5,chanStrLength-1));
	(TPad*) c->cd(chanNum*2-1);
      }
      //Else it is an ROI string (should probs put a test here for bad strings).
      else{
	int chanNum = std::stoi(channelID.substr(3,chanStrLength-1));
        (TPad*) c->cd(chanNum*2);
      }

      for(int adc_it=0; adc_it<NADC; ++adc_it){
	WaveformGraph[Graph_string]->SetPoint(WaveformGraph[Graph_string]->GetN(),adc_it,Waveform[adc_it]);
      }
      
      WaveformGraph[Graph_string]->SetLineColor(4);
      WaveformGraph[Graph_string]->Draw();
      c->Update();
    }

    c->cd();
    c->Draw("a");
    gPad->Modified();
    gPad->Update();

    if(fSave){ 
      outputfile->cd();
      c->Write();
    }

    std::cout << "Event Complete" << std::endl;

    //Check to see if we have moved on 
    while(!kbhit()){
      gSystem->ProcessEvents();
    }

      char ch;
      ch = getch();
      switch(ch){

      case 'e': 
	std::cout << "Enter an Event number: ";
	std::cin >> i;
	gPad->Close();
	break; 

      case 'q': 
	gPad->Close();
	return 0;

      case 'n':
	gPad->Close();
	if(i==EventTree->GetEntries()){return 0;}
	++i;
	break;
      }
  }
}

