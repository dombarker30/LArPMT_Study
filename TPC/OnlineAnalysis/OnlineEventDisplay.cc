#include "OnlineEventDisplay.hh"

/*

Much of this is shamelessly stolen from Lohan's pixelplane.C

*/

//main function for testing the plotting works.
/*
int main(){
  
  int track = 1;
  std::map<int,std::string> channelMap = DAQ::ChannelMap::InitChannelMap();
  std::vector<std::string> channelIDs;
  std::vector<bool> numHitsEvent;
  std::vector<double> maxADCs;
  std::vector<int> maxPeakTimes;

  for(int i=1; i<65; i++){
    std::string channelStr;
    if(i<37){
      channelStr = "Pixel" + std::to_string(i);
      
	}
    else{
      channelStr = "ROI" + std::to_string(i-36);
    }
    channelIDs.push_back(channelStr);
    if (track == 1){
	if(i==1 || i==8||i==22||i==29||i==36||i==40||i==47||i==54||i==61){
	  numHitsEvent.push_back(1);
	  maxADCs.push_back(i);
	  maxPeakTimes.push_back(10);
	}else if(i==15){
	  numHitsEvent.push_back(1);
	  maxADCs.push_back(i);
	  maxPeakTimes.push_back(10);

	}else{
	  numHitsEvent.push_back(0);
	  maxADCs.push_back(0);
	  maxPeakTimes.push_back(-99999);
	}
    }else{
	  numHitsEvent.push_back(1);
	  maxADCs.push_back(10);
	  maxPeakTimes.push_back(10);
    }
    //std::cout<<channelStr<<std::endl;
  }
  int eventNum = 1;

  TCanvas* XYCanvas = pixelXY(channelIDs, numHitsEvent, maxADCs, eventNum, channelMap);
  TCanvas* XYTCanvas = pixelXYT(channelIDs, numHitsEvent, maxADCs, maxPeakTimes, eventNum, channelMap);

  if(track ==1){
    XYCanvas->SaveAs("TestXYCanvasTrack.jpg");
    XYTCanvas->SaveAs("TestXYTCanvasTrack.jpg");
  }else{
    XYCanvas->SaveAs("TestXYCanvas.jpg");
    XYTCanvas->SaveAs("TestXYTCanvas.jpg");
  }

  return(0);
}
*/

TCanvas* PixelData::TPC::PixelXYPlot(std::vector<std::string> channelIDs, std::vector<int> numHitsEvent, std::vector<float> maxPeakHeights, int eventNum, std::map<int,std::string> channelMap)
{
  
  TCanvas* cXY = new TCanvas("XY", "Pixels XY", 1800, 900);
  cXY->Divide(2,1);
  
  // Define histogram
  int n_bins_pixelx = 13;
  int n_bins_pixely = 13;
  int pixelx_min = 1;
  int pixelx_max = 14;
  int pixely_min = 1;
  int pixely_max = 14;
  
  int n_bins_roix = 13;
  int n_bins_roiy = 13;
  int roix_min = 0;
  int roix_max = 13;
  int roiy_min = 0;
  int roiy_max = 13;
  
  delete gROOT->FindObject("pixelx_pixely");
  TH2F *pixelx_pixely = new TH2F("pixelx_pixely", "pixelx_pixely", n_bins_pixelx, pixelx_min, pixelx_max, n_bins_pixely, pixely_min, pixely_max);
  
  delete gROOT->FindObject("roix_roiy");
  TH2F *roix_roiy = new TH2F("roix_roiy", "roix_roiy", n_bins_roix, roix_min, roix_max, n_bins_roiy, roiy_min, roiy_max);
  
  //loop through channels of event
  for(int i=0; i<channelIDs.size(); i++){
    std::string channelID = channelIDs[i];
    //std::cout << "channelID: " << channelID << std::endl;
    
    if(numHitsEvent[i]==0){
      maxPeakHeights[i] = 1;
    }
    std::pair<int,int> xy = DAQ::ChannelMap::GetXYCoords(channelID); 
    int x = xy.first;
    int y = xy.second;
      //  }else{
      //continue;
      //}
    //std::cout << "x = " << x << ". y = " << y << std::endl;
    //if(channelID.BeginsWith("Pixel")){
    if(channelID.size() > 5){
      pixelx_pixely->Fill(x,y,maxPeakHeights[i]);
    }
    else{
      roix_roiy->Fill(x,y,maxPeakHeights[i]);
    }
  }

  std::string pixelTitleStr = "Pixel Event Map - Event " + std::to_string(eventNum);
  const char* pixelTitleChar = pixelTitleStr.c_str();;
  std::string roiTitleStr = "ROI Event Map - Event " + std::to_string(eventNum);
  const char* roiTitleChar = roiTitleStr.c_str();;
 
  cXY->cd(1);
  pixelx_pixely->SetStats(0);
  pixelx_pixely->GetYaxis()->SetTitleOffset(1.5);
  pixelx_pixely->GetXaxis()->SetTitleOffset(1.5);
  pixelx_pixely->SetTitle(pixelTitleChar);
  pixelx_pixely->GetXaxis()->SetTitle("Pixel X");
  pixelx_pixely->GetYaxis()->SetTitle("Pixel Y");
  //gStyle->SetPalette(1);
  pixelx_pixely->Draw("COLZ");
  
  cXY->cd(2);
  roix_roiy->SetStats(0);
  roix_roiy->GetYaxis()->SetTitleOffset(1.5);
  roix_roiy->GetXaxis()->SetTitleOffset(1.5);
  roix_roiy->SetTitle(roiTitleChar);
  roix_roiy->GetXaxis()->SetTitle("ROI X");
  roix_roiy->GetYaxis()->SetTitle("ROI Y");
  //gStyle->SetPalette(1);
  roix_roiy->Draw("COLZ");
    
  cXY->Update();
  return cXY;
}


TCanvas* PixelData::TPC::PixelXYTPlot(std::vector<std::string> channelIDs, std::vector<int> numHitsEvent, std::vector<float> maxPeakHeights, std::vector<float> maxPeakTimes, int eventNum, std::map<int,std::string> channelMap)
{
  
  TCanvas* cXYT = new TCanvas("XYT", "Pixels XYT", 1800, 900);
  cXYT->Divide(2,1);
  
  // Define histogram
  int n_bins_pixelx = 13;
  int n_bins_pixely = 13;
  int pixelx_min = 1;
  int pixelx_max = 14;
  int pixely_min = 1;
  int pixely_max = 14;

  int n_bins_roix = 13;
  int n_bins_roiy = 13;
  int roix_min = 0;
  int roix_max = 13;
  int roiy_min = 0;
  int roiy_max = 13;

  int n_bins_t = 20;
  int t_min = 0;//*std::min_element(std::begin(maxPeakTimes),std::end(maxPeakTimes));t_min--;
  int t_max = 12;//*std::max_element(std::begin(maxPeakTimes),std::end(maxPeakTimes));t_max++;


  int binx, biny, bint, bin;
  
  delete gROOT->FindObject("pixelx_pixely_pixelt");
  TH3F *pixelx_pixely_pixelt = new TH3F("pixelx_pixely_pixelt", "pixelx_pixely_pixelt", n_bins_pixelx, pixelx_min, pixelx_max, n_bins_pixely, pixely_min, pixely_max, n_bins_t, t_min, t_max);
  
  delete gROOT->FindObject("roix_roiy_roit");
  TH3F *roix_roiy_roit = new TH3F("roix_roiy_roit", "roix_roiy_roit", n_bins_roix, roix_min, roix_max, n_bins_roiy, roiy_min, roiy_max, n_bins_t, t_min, t_max);
  
  //loop through channels of event
  for(int i=0; i<channelIDs.size(); i++){
    std::string channelID = channelIDs[i];
    //std::cout << "channelID: " << channelID << std::endl;
    
    if(numHitsEvent[i]==0){
      maxPeakHeights[i] = -10;
    }
    std::pair<int,int> xy = DAQ::ChannelMap::GetXYCoords(channelID); 
    int x = xy.first;
    int y = xy.second;
      //  }else{
      //continue;
      //}
    //std::cout << "x = " << x << ". y = " << y << std::endl;
    //if(channelID.BeginsWith("Pixel")){
  




    if(channelID.size() > 5){
      pixelx_pixely_pixelt->Fill(x,y,maxPeakTimes[i],maxPeakHeights[i]);
      /*
      binx = x-1;
      biny = y-1;
      bint = n_bins_t*(maxPeakTimes[i] - t_min) / (t_max - t_min) + 1;
     
      bin = pixelx_pixely_pixelt->GetBin(binx, biny, bint);
     
      double content = pixelx_pixely_pixelt->GetBinContent(bin); 			
      content += maxPeakHeights[i];// - ev.minAdc;
    
      pixelx_pixely_pixelt->SetBinContent(bin, content);
      pixelx_pixely_pixelt->SetEntries(pixelx_pixely_pixelt->GetEntries() - 1);
      */
    }
    else{
      roix_roiy_roit->Fill(x,y,maxPeakTimes[i],maxPeakHeights[i]);
    }
  }
  

  std::string pixelTitleStr = "Pixel Event Map - Event " + std::to_string(eventNum);
  const char* pixelTitleChar = pixelTitleStr.c_str();;
  std::string roiTitleStr = "ROI Event Map - Event " + std::to_string(eventNum);
  const char* roiTitleChar = roiTitleStr.c_str();;
 
  cXYT->cd(1);
  pixelx_pixely_pixelt->SetStats(0);
  pixelx_pixely_pixelt->GetXaxis()->SetTitleOffset(1.5);
  pixelx_pixely_pixelt->GetYaxis()->SetTitleOffset(1.5);
  pixelx_pixely_pixelt->GetZaxis()->SetTitleOffset(1.5);
  pixelx_pixely_pixelt->SetTitle(pixelTitleChar);
  pixelx_pixely_pixelt->GetXaxis()->SetTitle("Pixel X");
  pixelx_pixely_pixelt->GetYaxis()->SetTitle("Pixel Y");
  pixelx_pixely_pixelt->GetZaxis()->SetTitle("Time (ticks)");
  gStyle->SetPalette(1);
  pixelx_pixely_pixelt->Draw("BOX2Z");
  
  cXYT->cd(2);
  roix_roiy_roit->SetStats(0);
  roix_roiy_roit->GetXaxis()->SetTitleOffset(1.5);
  roix_roiy_roit->GetYaxis()->SetTitleOffset(1.5);
  roix_roiy_roit->GetZaxis()->SetTitleOffset(1.5);
  roix_roiy_roit->SetTitle(roiTitleChar);
  roix_roiy_roit->GetXaxis()->SetTitle("ROI X");
  roix_roiy_roit->GetYaxis()->SetTitle("ROI Y");
  roix_roiy_roit->GetZaxis()->SetTitle("Time (ticks)");
  gStyle->SetPalette(1);
  roix_roiy_roit->Draw("BOX2Z");
    
  cXYT->Update();
  return cXYT;
}
