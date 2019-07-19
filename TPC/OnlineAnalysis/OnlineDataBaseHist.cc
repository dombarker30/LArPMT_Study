#include "OnlineDataBaseHist.hh"

//Function to get the latest event's time in seconds from the database
int PixelData::OnlineMointoring::OnlineDataBaseHist::LastestTimeStamp(pqxx::nontransaction& N){

  int latest_timestampsec = -99999; 

  char * sql;

  //SQL command for the latest timestamp
  sql = "SELECT timestampsecond FROM OnlineParams ORDER BY timestampsecond DESC LIMIT 1;";

  //Execute SQL query for the last timestamp.
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::result LastTimeStamp(N.exec(sql)); 
    //Get the latest timestamp. This loop should only containt one object. Sorry this poor
    for (pqxx::result::const_iterator c = LastTimeStamp.begin(); c != LastTimeStamp.end(); ++c){
      latest_timestampsec = c[0].as<int>();
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to get the last time stamp" << std::endl;       
    return -1;
  }

  return latest_timestampsec;

};

//Function to initalise figures from the online mointoring.
void PixelData::OnlineMointoring::OnlineDataBaseHist::InitialiseFigure(std::string MetricName, std::string MetricTitle, int bins, float low, float high){

  //Setup the average graphs for the ROIs
  std::string MetricNameROIs = MetricName + "ROIs";
  const char* MetricROIsName_Char = MetricNameROIs.c_str(); 

  const char* MetricTitle_Char = MetricTitle.c_str();

  this->MetricsROIs_AvgHist_Map[MetricNameROIs]    = new TH1F(MetricROIsName_Char,MetricROIsName_Char,bins,low,high);
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetXaxis()->SetTitle(MetricTitle_Char);
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetYaxis()->SetTitle("Enteries");

  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]   = new TGraphErrors();
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetXaxis()->SetTitle("Time");
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetYaxis()->SetTitle(MetricTitle_Char);
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->SetTitle("Avg");

  this->MetricsROIs_MultiGraph_Map[MetricNameROIs] = new TMultiGraph();
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetXaxis()->SetTitle("Time");
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetYaxis()->SetTitle(MetricTitle_Char);
    
  //Setup the average graphs for the Pixels
  std::string MetricNamePixels =  MetricName + "Pixels";
  const char* MetricPixelsName_Char = MetricNamePixels.c_str(); 

  this->MetricsPixels_AvgHist_Map[MetricNamePixels]    = new TH1F(MetricPixelsName_Char,MetricPixelsName_Char,bins,low,high);
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetXaxis()->SetTitle(MetricTitle_Char);
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetYaxis()->SetTitle("Enteries");

  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]   = new TGraphErrors();
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetXaxis()->SetTitle("Time");
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetYaxis()->SetTitle(MetricTitle_Char);
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->SetTitle("Avg");

  this->MetricsPixels_MultiGraph_Map[MetricNamePixels] = new TMultiGraph();
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetXaxis()->SetTitle("Time");
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetYaxis()->SetTitle(MetricTitle_Char);


  //Set up the canvas 
  const char* MetricName_Char = MetricName.c_str(); 
  this->Metrics_Canvas_Map[MetricName]     = new TCanvas(MetricName_Char,MetricName_Char,900,500);
  this->Metrics_Canvas_Map[MetricName]->Divide(2,2);
							      
  //Setup the avaerage graphs for the ROIs
  for(int i=0; i<64; ++i){

    std::string ChannelName_string  = MetricName + " Channel " + std::to_string(i);
    const char* ChannelName_name    = ChannelName_string.c_str();

    std::string title_string = std::to_string(i);
    const char* title_name = title_string.c_str();

    this->Metrics_Hist_ChanMap[i][MetricName]  = new TH1F(ChannelName_name,ChannelName_name,bins,low,high);
    this->Metrics_Hist_ChanMap[i][MetricName]->GetXaxis()->SetTitle(MetricTitle_Char);
    this->Metrics_Hist_ChanMap[i][MetricName]->GetYaxis()->SetTitle("Enteries");
    this->Metrics_Hist_ChanMap[i][MetricName]->SetTitle(title_name);
    this->Metrics_Graph_ChanMap[i][MetricName] = new TGraphErrors();
    this->Metrics_Graph_ChanMap[i][MetricName]->GetXaxis()->SetTitle("Time");
    this->Metrics_Graph_ChanMap[i][MetricName]->GetYaxis()->SetTitle(MetricTitle_Char);
    this->Metrics_Graph_ChanMap[i][MetricName]->SetTitle(title_name);

  }

  //Add the metric list 
  this->MetricVector.push_back(MetricName);

  return;
};

//Function to fill the histograms with the values from the database 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigures(float time_avg, int latest_timestamp, std::vector<std::string> MetricNames, pqxx::nontransaction& N){
  
  char * sql;

  //Access the last values added to the database
  sql = "SELECT * FROM OnlineParams ORDER BY timestampsecond";

  //Execute SQL query.
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::result GetInfo( N.exec( sql ));  
    
    //Go back over the last x mins and average over  
    for (pqxx::result::const_iterator c = GetInfo.begin(); c != GetInfo.end(); ++c){
      
      
      std::string channelID       = c[3].as<std::string>();
      int         channel         = c[2].as<int>();
      int         timestampsecond = c[0].as<int>();
      
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[0],c[0].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[1],c[1].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[2],c[2].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[3],c[3].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[4],c[4].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[5],c[5].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[6],c[6].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[7],c[7].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[8],c[8].as<float>(), channelID, channel);
      
      //Lets not do it forever please
      if(timestampsecond < (latest_timestamp - time_avg)){break;}
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to get list for filling " << std::endl;       
    return;
  }

  
  return;
};

//Fill the histograms 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(std::string MetricName, int value, std::string channelID, int channel){

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    MetricsPixels_AvgHist_Map[MetricName]->Fill(value);
  }
  else{
    MetricsROIs_AvgHist_Map[MetricName]->Fill(value);
  }

  Metrics_Hist_ChanMap[channel][MetricName]->Fill(value);

  return;
};

//Fill the histograms 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(std::string MetricName, float value, std::string channelID, int channel){

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    MetricsPixels_AvgHist_Map[MetricName]->Fill(value);
  }
  else{
    MetricsROIs_AvgHist_Map[MetricName]->Fill(value);
  }

  Metrics_Hist_ChanMap[channel][MetricName]->Fill(value);

  return;
};


void PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigures(std::vector<std::string> MetricVector, pqxx::connection& conn){

  for(int i=0; i<MetricVector.size(); ++i){
    PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigure(MetricVector[i], conn);
  }

};
 
void PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigure(std::string MetricName, pqxx::connection& conn){

  const char * sql;

  //Get the Average time val 
  std::string Time_AVG = std::to_string(MetricsPixels_AvgHist_Map["timestampsecond"]->GetMean());
  std::string Time_RMS = std::to_string(MetricsPixels_AvgHist_Map["timestampsecond"]->GetRMS()); 

  //Sort out the Average values 

  //Calculate mean and RMS 
  float MetricPixels_Mean = MetricsPixels_AvgHist_Map[MetricName]->GetMean();  
  float MetricPixels_RMS  = MetricsPixels_AvgHist_Map[MetricName]->GetRMS();
  float MetricROIs_Mean = MetricsROIs_AvgHist_Map[MetricName]->GetMean();  
  float MetricROIs_RMS  = MetricsROIs_AvgHist_Map[MetricName]->GetRMS();

  //Send to PostGres Tables 
  std::string sql_sendgraphpixels_string = "INSERT INTO OnlineParamsAvgPixels" + MetricName + " Values (" + std::to_string(MetricPixels_Mean) + "," + std::to_string(MetricPixels_RMS) + "," + Time_AVG + "," + Time_RMS + ");";

  sql = sql_sendgraphpixels_string.c_str(); 
  
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(conn);
    pqxx::result res = w.exec(sql);
    w.commit();
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to send row to table of average pixel metric " << MetricName << std::endl;       
    return;
  }

  //Send to PostGres Tables 
  std::string sql_sendgraphrios_string = "INSERT INTO OnlineParamsAvgPixels" + MetricName + " Values (" + std::to_string(MetricPixels_Mean) + "," + std::to_string(MetricPixels_RMS) +  "," + Time_AVG + "," + Time_RMS + ");";

  sql = sql_sendgraphrios_string.c_str(); 
  
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(conn);
    pqxx::result res = w.exec(sql);
    w.commit();
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to send row to table of average roi metric " << MetricName << std::endl;       
    return;
  }

  //Fill the TGraphs wih posgress values
  std::string sql_recgraphpixels_string = "Select * FROM OnlineParamsAvgPixels" + MetricName + "ORDER BY timeavg;";
  sql = sql_recgraphpixels_string.c_str();
  try{
    pqxx::work w(conn);
    pqxx::result GetInfo = w.exec(sql);

    //Loop over results and set the point on the graph.
    for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
      MetricsPixels_AvgGraph_Map[MetricName]->SetPoint(MetricsPixels_AvgGraph_Map[MetricName]->GetN()-1,res[0].as<float>(),res[3].as<float>());
      MetricsPixels_AvgGraph_Map[MetricName]->SetPointError(MetricsPixels_AvgGraph_Map[MetricName]->GetN(),res[2].as<float>(),res[4].as<float>());
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to recieve row to table of average pixels metric " << MetricName << std::endl;       
    return;
  }

    std::string sql_recgraphrois_string = "Select * FROM OnlineParamsAvgRois" + MetricName + "ORDER BY timeavg;";
  sql = sql_recgraphrois_string.c_str();
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(conn);
    pqxx::result GetInfo = w.exec(sql);

    //Loop over results and set the point on the graph.
    for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
      MetricsROIs_AvgGraph_Map[MetricName]->SetPoint(MetricsROIs_AvgGraph_Map[MetricName]->GetN()-1,res[0].as<float>(),res[3].as<float>());
      MetricsROIs_AvgGraph_Map[MetricName]->SetPointError(MetricsROIs_AvgGraph_Map[MetricName]->GetN(),res[2].as<float>(),res[4].as<float>());
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to recieve row to table of average rois metric " << MetricName << std::endl;       
    return;
  }



  //Write the TGraphs to file
  MetricsROIs_AvgGraph_Map[MetricName]->Write();
  MetricsPixels_AvgGraph_Map[MetricName]->Write();

  //Add the MultiGraph 
  MetricsROIs_MultiGraph_Map[MetricName]->Add(MetricsROIs_AvgGraph_Map[MetricName]);
  MetricsPixels_MultiGraph_Map[MetricName]->Add(MetricsPixels_MultiGraph_Map[MetricName]);

  //Write Histograms to file 
  MetricsPixels_AvgHist_Map[MetricName]->Write();
  MetricsROIs_AvgHist_Map[MetricName]  ->Write();

  //Move to the Channels folder in the TFile
  OutputFile->cd("Channels");

  //Sort out the channel values
  for(int i=0; i<64; ++i){ 

    //Move to the correct channel folder
    std::string ChanFolder_String = "Channel " + std::to_string(i);
    const char * ChanFolder_name = ChanFolder_String.c_str();
    OutputFile->cd(ChanFolder_name);

    //Calculate mean and RMS
    float Metric_Chan_Mean = Metrics_Hist_ChanMap[i][MetricName]->GetMean();
    float Metric_Chan_RMS  = Metrics_Hist_ChanMap[i][MetricName]->GetRMS();

    //Write the Histograms 
    Metrics_Hist_ChanMap[i][MetricName]->Write();

    //Send to PostGres Table  
    std::string sql_changraph_string = "INSERT INTO OnlineParamsAvg" + MetricName + "Chan"+ std::to_string(i) + " Values (" + std::to_string(Metric_Chan_Mean) + "," + std::to_string(Metric_Chan_RMS) + "," + Time_AVG + "," + Time_RMS + ");";

    sql = sql_changraph_string.c_str(); 
      
    try{
      std::cout << "Running command: " << sql << std::endl;
      pqxx::work w(conn);
      pqxx::result SendInfo = w.exec(sql);
      w.commit();
    }
    catch(const std::exception &e){
      std::cerr << e.what() << std::endl;
      std::cout <<" Failed to send row to table of average Channel " << i << " Metric: " << MetricName << std::endl; 
      return;
    }

    //Fill the TGraph with postgres values
    std::string sql_recchangraph_string = "Select * FROM OnlineParamsAvgRois" + MetricName + "Chan" + std::to_string(i) + "ORDER BY timeavg;";
    sql = sql_recchangraph_string.c_str();
    try{
      std::cout << "Running command: " << sql << std::endl;
      pqxx::work w(conn);
      pqxx::result GetInfo = w.exec(sql);
      
      //Loop over results and set the point on the graph.
      for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
	MetricsROIs_AvgGraph_Map[MetricName]->SetPoint(MetricsROIs_AvgGraph_Map[MetricName]->GetN()-1,res[0].as<float>(),res[3].as<float>());
	MetricsROIs_AvgGraph_Map[MetricName]->SetPointError(MetricsROIs_AvgGraph_Map[MetricName]->GetN(),res[2].as<float>(),res[4].as<float>());
      }
    }
    catch(const std::exception &e){
      std::cerr << e.what() << std::endl;
      std::cout << " Failed to recieve row to table of average rois metric " << MetricName << " On channel" << i << std::endl;       
      return;
    }
    
    //Write the TGraph 
    Metrics_Graph_ChanMap[i][MetricName]->Write();

    //use the channel map to find if ROI or PPixels 
    std::map<int,std::string> channelMap = DAQ::ChannelMap::InitChannelMap();
    std::string channelID = DAQ::ChannelMap::GetChannelID(i+1, channelMap);
    std::size_t found_pixel = channelID.find("Pixel");

    //Add to the MultiGraph 
    if(found_pixel != std::string::npos){
      MetricsPixels_MultiGraph_Map[MetricName]->Add(Metrics_Graph_ChanMap[i][MetricName]);
    }
    else{
      MetricsROIs_MultiGraph_Map[MetricName]->Add(Metrics_Graph_ChanMap[i][MetricName]);
    }
  }

  //Move back outside
  OutputFile->cd();

  //Fill the canvas
  Metrics_Canvas_Map[MetricName]->cd(1); 
  MetricsPixels_AvgHist_Map[MetricName]->Draw();
  Metrics_Canvas_Map[MetricName]->cd(3); 
  MetricsROIs_AvgHist_Map[MetricName]->Draw();
  Metrics_Canvas_Map[MetricName]->cd(2);
  MetricsPixels_MultiGraph_Map[MetricName]->Draw("A pmc plc");
  gPad->BuildLegend();
  Metrics_Canvas_Map[MetricName]->cd(2);
  MetricsROIs_MultiGraph_Map[MetricName]->Draw("A pmc plc");
  gPad->BuildLegend();
  Metrics_Canvas_Map[MetricName]->Update();
  
  //Save the canvas 
  std::string Path(gDirectory->GetPath());
  std::string NameString = Path.substr(0, Path.size()-2);
  NameString += "_" + MetricName + "png";
  const char* Name = NameString.c_str();
  Metrics_Canvas_Map[MetricName]->SaveAs(Name);
  

};


//Function to name the output file.
void PixelData::OnlineMointoring::OnlineDataBaseHist::SetupOutputFile(){

  //Get the time name part of the file. 
  time_t now = time(0);
  struct tm tstruct;
  tstruct = *localtime(&now);
  std::string filename_string = "OnlineData_" + std::to_string(tstruct.tm_year) 
    + std::to_string(tstruct.tm_mon) 
    + std::to_string(tstruct.tm_mday) + "_" 
    + std::to_string(tstruct.tm_hour) 
    + std::to_string(tstruct.tm_min) 
    + std::to_string(tstruct.tm_sec);
  const char*filename = filename_string.c_str();
  
  //And create
  this->OutputFile = new TFile(filename,"RECREATE");
  this->OutputFile->mkdir("Channels");
  this->OutputFile->cd("Channels");
  for(int i=0; i<64; ++i){  
    std::string ChanFolder_String = "Channel " + std::to_string(i);
    const char * ChanFolder_name = ChanFolder_String.c_str();
    this->OutputFile->mkdir(ChanFolder_name);
  }
  this->OutputFile->cd();
  return;

};

PixelData::OnlineMointoring::OnlineDataBaseHist::OnlineDataBaseHist(){
  this->SetupOutputFile();
};
