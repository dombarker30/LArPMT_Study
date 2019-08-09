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

  std::cout << "latest_timestampsec: " << latest_timestampsec << std::endl;

  return latest_timestampsec;

};

//Function to initalise figures from the online mointoring.
void PixelData::OnlineMointoring::OnlineDataBaseHist::InitialiseFigure(std::string MetricName, std::string MetricTitle){

  std::cout << "Started Initalising Graphs" << std::endl;

  if(Enteries == 0){
    std::cerr << "No Entries" << std::endl;
  }

  int bins = 1;

  if(Enteries != 0){ 
    bins = 1 + 3.322*log(Enteries) + 20;
  }

  //Setup the average graphs for the ROIs
  std::string MetricNameROIs = MetricName + "ROIs";
  std::string MetricNameROIsGraph = MetricName + "ROIsGraph";
  
  const char* MetricROIsName_Char = MetricNameROIs.c_str(); 
  const char* MetricROIsNameGraph_Char = MetricNameROIsGraph.c_str();
    
  const char* MetricTitle_Char = MetricTitle.c_str();

  float binwidth = (float) (MetricsROIs_AvgMax_Map[MetricNameROIs]- MetricsROIs_AvgMin_Map[MetricNameROIs])/(float) bins; 
  float start    = (float) (MetricsROIs_AvgMin_Map[MetricNameROIs] - (binwidth*10));
  float end      = (float) (MetricsROIs_AvgMax_Map[MetricNameROIs] + (binwidth*10));

  if(MetricsROIs_AvgMin_Map[MetricNameROIs] ==  std::numeric_limits<float>::infinity()){start = 0;}
  if(MetricsROIs_AvgMax_Map[MetricNameROIs] == -std::numeric_limits<float>::infinity()){end   = 10;}

  this->MetricsROIs_AvgHist_Map[MetricNameROIs]    = new TH1F(MetricROIsName_Char,MetricROIsName_Char,bins,start,end);
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetXaxis()->SetTitle(MetricTitle_Char);
  this->MetricsROIs_AvgHist_Map[MetricNameROIs]->GetYaxis()->SetTitle("Enteries");

  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]   = new TGraphErrors();
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetTitle(MetricROIsNameGraph_Char);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetName(MetricROIsNameGraph_Char);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetXaxis()->SetTitle("Time");
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetYaxis()->SetTitle(MetricTitle_Char);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetLineColor(kBlack);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetLineWidth(5);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetXaxis()->SetTimeDisplay(1);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetXaxis()->SetNdivisions(-503);
  this->MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M %F 1970-01-01 00:00:00");

  this->MetricsROIs_AvgMax_Map[MetricNameROIs] = -99999;
  this->MetricsROIs_AvgMin_Map[MetricNameROIs] = 99999;

  this->MetricsROIs_MultiGraph_Map[MetricNameROIs] = new TMultiGraph();
  std::string multi_title_string_roi = "ROI;Time;" + MetricName;
  const char* multi_title_string_roi_char = multi_title_string_roi.c_str();
  this->MetricsROIs_MultiGraph_Map[MetricNameROIs]->SetTitle(multi_title_string_roi_char);

  //Setup the average graphs for the Pixels
  std::string MetricNamePixels =  MetricName + "Pixels";
  const char* MetricPixelsName_Char = MetricNamePixels.c_str(); 

  std::string MetricNamePixelsGraph =  MetricName + "PixelsGraph";
  const char* MetricPixelsNameGraph_Char = MetricNamePixelsGraph.c_str(); 


  binwidth = (MetricsPixels_AvgMax_Map[MetricNamePixels]- MetricsPixels_AvgMin_Map[MetricNamePixels])/bins; 
  start    = MetricsPixels_AvgMin_Map[MetricNamePixels] - (binwidth*10);
  end      = MetricsPixels_AvgMax_Map[MetricNamePixels] + (binwidth*10);

  if(MetricsPixels_AvgMin_Map[MetricNamePixels] ==  std::numeric_limits<float>::infinity()){start = 0;}
  if(MetricsPixels_AvgMax_Map[MetricNamePixels] == -std::numeric_limits<float>::infinity()){end   = 10;}

  this->MetricsPixels_AvgHist_Map[MetricNamePixels]    = new TH1F(MetricPixelsName_Char,MetricPixelsName_Char,bins,start,end);
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetXaxis()->SetTitle(MetricTitle_Char);
  this->MetricsPixels_AvgHist_Map[MetricNamePixels]->GetYaxis()->SetTitle("Enteries");

  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]   = new TGraphErrors();
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetTitle(MetricPixelsNameGraph_Char); 
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetName(MetricPixelsNameGraph_Char);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetXaxis()->SetTitle("Time");
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetYaxis()->SetTitle(MetricTitle_Char);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetLineColor(kBlack);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetLineWidth(5);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetXaxis()->SetTimeDisplay(1);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetXaxis()->SetNdivisions(-503);
  this->MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M %F 1970-01-01 00:00:00");

  this->MetricsPixels_MultiGraph_Map[MetricNamePixels] = new TMultiGraph();
  std::string multi_title_string_pixel = "Pixel;Time;" + MetricName;
  const char* multi_title_string_pixel_char = multi_title_string_pixel.c_str();
  this->MetricsPixels_MultiGraph_Map[MetricNamePixels]->SetTitle(multi_title_string_pixel_char);

  //Set up the canvas 
  const char* MetricName_Char = MetricName.c_str(); 
  this->Metrics_Canvas_Map[MetricName]     = new TCanvas(MetricName_Char,MetricName_Char,900,500);
  this->Metrics_Canvas_Map[MetricName]->Divide(2,2);
							      
  //Setup the avaerage graphs for the ROIs
  for(int i=0; i<64; ++i){

    std::string ChannelName_string  = MetricName + " Channel " + std::to_string(i);
    const char* ChannelName_name    = ChannelName_string.c_str();

    std::string title_string = "Graph " + MetricName + " Channel " +  std::to_string(i);
    const char* title_name = title_string.c_str();

    binwidth = (Metrics_Max_ChanMap[i][MetricName]- Metrics_Min_ChanMap[i][MetricName])/bins; 
    start    = Metrics_Min_ChanMap[i][MetricName] - (binwidth*10);
    end      = Metrics_Max_ChanMap[i][MetricName] + (binwidth*10);

    if(Metrics_Min_ChanMap[i][MetricName] ==  std::numeric_limits<float>::infinity()){start = 0;}
    if(Metrics_Max_ChanMap[i][MetricName] == -std::numeric_limits<float>::infinity()){end   = 10;}

    this->Metrics_Hist_ChanMap[i][MetricName]    = new TH1F(ChannelName_name,ChannelName_name,bins,start,end);
    this->Metrics_Hist_ChanMap[i][MetricName]->GetXaxis()->SetTitle(MetricTitle_Char);
    this->Metrics_Hist_ChanMap[i][MetricName]->GetYaxis()->SetTitle("Enteries");
    this->Metrics_Hist_ChanMap[i][MetricName]->SetTitle(title_name);

    this->Metrics_Graph_ChanMap[i][MetricName] = new TGraphErrors();
    this->Metrics_Graph_ChanMap[i][MetricName]->GetXaxis()->SetTitle("Time");
    this->Metrics_Graph_ChanMap[i][MetricName]->GetYaxis()->SetTitle(MetricTitle_Char);
    this->Metrics_Graph_ChanMap[i][MetricName]->SetTitle(title_name);
    this->Metrics_Graph_ChanMap[i][MetricName]->SetName(title_name);
    this->Metrics_Graph_ChanMap[i][MetricName]->GetXaxis()->SetTimeDisplay(1);
    this->Metrics_Graph_ChanMap[i][MetricName]->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M %F 1970-01-01 00:00:00");
  }

  //Add the metric list 
  this->MetricVector.push_back(MetricName);
  std::cout << " Finished Initialising Graphs" << std::endl;
  return;
};

//Function to initalise figures from the online mointoring.
void PixelData::OnlineMointoring::OnlineDataBaseHist::InitialiseLimits(std::string MetricName){

  std::string MetricNamePixels =  MetricName + "Pixels";
  std::string MetricNameROIs = MetricName + "ROIs";

  this->MetricsPixels_AvgMax_Map[MetricNamePixels] = -std::numeric_limits<float>::infinity();
  this->MetricsPixels_AvgMin_Map[MetricNamePixels] = std::numeric_limits<float>::infinity();

  this->MetricsROIs_AvgMax_Map[MetricNameROIs] = -std::numeric_limits<float>::infinity();
  this->MetricsROIs_AvgMin_Map[MetricNameROIs] = std::numeric_limits<float>::infinity();

  for(int i=0; i<64; ++i){  
    this->Metrics_Max_ChanMap[i][MetricName] = -std::numeric_limits<float>::infinity();
    this->Metrics_Min_ChanMap[i][MetricName] = std::numeric_limits<float>::infinity();
  }
  

  //Add the metric list
  this->MetricVector.push_back(MetricName);   
  return;
}


void PixelData::OnlineMointoring::OnlineDataBaseHist::GetAllLimits(float time_avg, int latest_timestamp, std::vector<std::string> MetricNames, pqxx::nontransaction& N){
  
  Enteries = 0;

  const char * sql;

  //use the channel map to find if ROI or Pixels       
  std::map<int,std::string> channelMap = DAQ::ChannelMap::InitChannelMap();

  std::cout << "Getting Limits" << std::endl;
  
  //Access the last values added to the database
  std::string sql_string = "SELECT * FROM OnlineParams WHERE (" + std::to_string(latest_timestamp) + " - timestampsecond) < " + std::to_string(time_avg);
  
  sql = sql_string.c_str();

  //Execute SQL query.
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::result GetInfo( N.exec( sql ));  

    //Before filling the historgrams find the correct limits for things.
      for (pqxx::result::const_iterator c = GetInfo.begin(); c != GetInfo.end(); ++c){
	
	int         channel         = c[2].as<int>();
	int         timestampsecond = c[0].as<int>();
	
	std::string channelID = DAQ::ChannelMap::GetChannelID(channel+1, channelMap);
	

	//Lets not do it forever please                      
	if(timestampsecond < (latest_timestamp - time_avg)){break;}
	
	++Enteries;
	
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[0],c[0].as<int>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[1],c[1].as<int>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[2],c[2].as<int>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[3],c[3].as<float>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[4],c[4].as<int>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[5],c[5].as<float>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[6],c[6].as<float>(), channelID, channel);
	PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(MetricNames[7],c[7].as<int>(), channelID, channel);
	
      }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to get the limits " << std::endl;       
    return;
  }

  std::cout << "Number of entries used is: " << Enteries << std::endl;

}

//Function to fill the histograms with the values from the database 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigures(float time_avg, int latest_timestamp, std::vector<std::string> MetricNames, pqxx::nontransaction& N){
  
  if(Enteries == 0){
    std::cerr << "No Entries" << std::endl;
    return;
  }


  const char * sql;

  //use the channel map to find if ROI or PPixels                                                                                                                                  
  std::map<int,std::string> channelMap = DAQ::ChannelMap::InitChannelMap();

  std::cout << "Filling Figures" << std::endl;

  //Access the last values added to the database
  std::string sql_string = "SELECT * FROM OnlineParams WHERE (" + std::to_string(latest_timestamp) + " - timestampsecond) < " + std::to_string(time_avg);

  sql = sql_string.c_str();

  //Execute SQL query.
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::result GetInfo( N.exec( sql ));  


    //Go back over the last x mins and average over  
    for (pqxx::result::const_iterator c = GetInfo.begin(); c != GetInfo.end(); ++c){
      
      int         channel         = c[2].as<int>();
      int         timestampsecond = c[0].as<int>();

      std::string channelID = DAQ::ChannelMap::GetChannelID(channel+1, channelMap);
      
      //Lets not do it forever please                      
      if(timestampsecond < (latest_timestamp - time_avg)){break;}

      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[0],c[0].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[1],c[1].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[2],c[2].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[3],c[3].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[4],c[4].as<int>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[5],c[5].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[6],c[6].as<float>(), channelID, channel);
      PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(MetricNames[7],c[7].as<int>(), channelID, channel);

    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to get list for filling " << std::endl;       
    return;
  }

  std::cout << "Finished Filling Graphs" << std::endl;
  return;
};

//Fill the histograms 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(std::string MetricName, int value, std::string channelID, int channel){

  if(value==-99999){return;}

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    std::string MetricNamePixels = MetricName + "Pixels";
    MetricsPixels_AvgHist_Map[MetricNamePixels]->Fill(value);
  }
  else{
    std::string MetricNameROIS = MetricName + "ROIs";
    MetricsROIs_AvgHist_Map[MetricNameROIS]->Fill(value);
  }
  Metrics_Hist_ChanMap[channel][MetricName]->Fill(value);
  return;
};


//Fill the histograms 
void PixelData::OnlineMointoring::OnlineDataBaseHist::FillFigure(std::string MetricName, float value, std::string channelID, int channel){

  if(value==-99999){return;}  

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    std::string MetricNamePixels = MetricName + "Pixels";
    MetricsPixels_AvgHist_Map[MetricNamePixels]->Fill(value);
  }
  else{
    std::string MetricNameROIs = MetricName + "ROIs";
    MetricsROIs_AvgHist_Map[MetricNameROIs]->Fill(value);
  }

  Metrics_Hist_ChanMap[channel][MetricName]->Fill(value);
  return;
};

void PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(std::string MetricName, int value, std::string channelID, int channel){

  if(value==-99999){return;}

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    std::string MetricNamePixels = MetricName + "Pixels";
    if(MetricsPixels_AvgMax_Map[MetricNamePixels] < value){MetricsPixels_AvgMax_Map[MetricNamePixels] = value;} 
    if(MetricsPixels_AvgMin_Map[MetricNamePixels] > value){MetricsPixels_AvgMin_Map[MetricNamePixels] = value;}
  }
  else{
    std::string MetricNameROIs = MetricName + "ROIs";
    if(MetricsROIs_AvgMax_Map[MetricNameROIs] < value){MetricsROIs_AvgMax_Map[MetricNameROIs] = value;} 
    if(MetricsROIs_AvgMin_Map[MetricNameROIs] > value){MetricsROIs_AvgMin_Map[MetricNameROIs] = value;}
  }
  if(Metrics_Max_ChanMap[channel][MetricName] < value){Metrics_Max_ChanMap[channel][MetricName] = value;} 
  if(Metrics_Min_ChanMap[channel][MetricName] > value){Metrics_Min_ChanMap[channel][MetricName] = value;}
  return;
};

void PixelData::OnlineMointoring::OnlineDataBaseHist::GetLimits(std::string MetricName, float value, std::string channelID, int channel){

  if(value==-99999){return;}

  //Check to see if its an ROI or Pixel 
  int chanStrLength = channelID.size();
  if(chanStrLength>5){
    std::string MetricNamePixels = MetricName + "Pixels";
    if(MetricsPixels_AvgMax_Map[MetricNamePixels] < value){MetricsPixels_AvgMax_Map[MetricNamePixels] = value;} 
    if(MetricsPixels_AvgMin_Map[MetricNamePixels] > value){MetricsPixels_AvgMin_Map[MetricNamePixels] = value;}
  }
  else{
    std::string MetricNameROIs = MetricName + "ROIs";
    if(MetricsROIs_AvgMax_Map[MetricNameROIs] < value){MetricsROIs_AvgMax_Map[MetricNameROIs] = value;} 
    if(MetricsROIs_AvgMin_Map[MetricNameROIs] > value){MetricsROIs_AvgMin_Map[MetricNameROIs] = value;}
  }
  if(Metrics_Max_ChanMap[channel][MetricName] < value){Metrics_Max_ChanMap[channel][MetricName] = value;} 
  if(Metrics_Min_ChanMap[channel][MetricName] > value){Metrics_Min_ChanMap[channel][MetricName] = value;}
  return;
};

void PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigures(std::vector<std::string> MetricVector, pqxx::connection& conn, uint32_t& time){
  
  std::cout << "Plotting Figures" << std::endl;
  
  for(int i=0; i<MetricVector.size(); ++i){
    PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigure(MetricVector[i], conn, time);
  }

};
 
void PixelData::OnlineMointoring::OnlineDataBaseHist::PlotFigure(std::string MetricName, pqxx::connection& conn, uint32_t& time){

  const char * sql;

  int latest_timestamp =  std::time(nullptr);

  std::string Time_AVG = "";
  std::string Time_RMS = "";

  float MetricPixels_Mean = 0;
  float MetricPixels_RMS = 0;
  float MetricROIs_Mean = 0;
  float MetricROIs_RMS = 0;
  
  //Calculate mean and RMS 
  std::string MetricNamePixels = MetricName + "Pixels";  
  std::string MetricNameROIs = MetricName + "ROIs";   

  if(Enteries!=0){

    //Get the Average time val 
    Time_AVG = std::to_string(MetricsPixels_AvgHist_Map["timestampsecondPixels"]->GetMean());
    Time_RMS = std::to_string(MetricsPixels_AvgHist_Map["timestampsecondPixels"]->GetRMS()); 

    //Sort out the Average values 

    MetricPixels_Mean = MetricsPixels_AvgHist_Map[MetricNamePixels]->GetMean();  
    MetricPixels_RMS  = MetricsPixels_AvgHist_Map[MetricNamePixels]->GetRMS();
    MetricROIs_Mean = MetricsROIs_AvgHist_Map[MetricNameROIs]->GetMean();  
    MetricROIs_RMS  = MetricsROIs_AvgHist_Map[MetricNameROIs]->GetRMS();

    if(MetricsPixels_AvgMax_Map[MetricNamePixels] != -std::numeric_limits<float>::infinity()        ||
       MetricsPixels_AvgMin_Map[MetricNamePixels] != std::numeric_limits<float>::infinity()         ||
       MetricsPixels_AvgMax_Map["timestampsecondPixels"] != -std::numeric_limits<float>::infinity() ||
       MetricsPixels_AvgMin_Map["timestampsecondPixels"] != std::numeric_limits<float>::infinity()  || 
       MetricsPixels_AvgHist_Map[MetricNameROIs]->GetEntries() != 0){
      
    
      //Send to PostGres Tables  
      std::string sql_sendgraphpixels_string = "INSERT INTO OnlineParamsAvgPixels" + MetricName + " Values (" + std::to_string(MetricPixels_Mean) + "," + std::to_string(MetricPixels_RMS) + "," + Time_AVG + "," + Time_RMS + ")";
    
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
    }

    if(MetricsROIs_AvgMax_Map[MetricNameROIs] != -std::numeric_limits<float>::infinity()          ||
       MetricsROIs_AvgMin_Map[MetricNameROIs] != std::numeric_limits<float>::infinity()           ||
       MetricsROIs_AvgMax_Map["timestampsecondPixels"] != -std::numeric_limits<float>::infinity() ||
       MetricsROIs_AvgMin_Map["timestampsecondPixels"] != std::numeric_limits<float>::infinity()  || 
       MetricsROIs_AvgHist_Map[MetricNameROIs]->GetEntries() != 0){

      //Send to PostGres Tables 
      std::string sql_sendgraphrios_string = "INSERT INTO OnlineParamsAvgROIs" + MetricName + " Values (" + std::to_string(MetricROIs_Mean) + "," + std::to_string(MetricROIs_RMS) +  "," + Time_AVG + "," + Time_RMS + ");";

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
    }
  }

  //Fill the TGraphs wih posgress values
  std::string sql_recgraphpixels_string = "Select * FROM OnlineParamsAvgPixels" + MetricName + " WHERE (" + std::to_string(latest_timestamp) + "- timeavg) < " + std::to_string(time)  + " ORDER BY timeavg;";
  sql = sql_recgraphpixels_string.c_str();
  try{
    pqxx::work w(conn);
    pqxx::result GetInfo = w.exec(sql);

    
    //Loop over results and set the point on the graph.
    for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
      MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetPoint(MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetN(),res[2].as<float>(),res[0].as<float>());
      MetricsPixels_AvgGraph_Map[MetricNamePixels]->SetPointError(MetricsPixels_AvgGraph_Map[MetricNamePixels]->GetN()-1,res[0].as<float>(),res[1].as<float>());
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to recieve row to table of average pixels metric " << MetricName << std::endl;       
    return;
  }

  std::string sql_recgraphrois_string = "Select * FROM OnlineParamsAvgRois" + MetricName + " WHERE (" + std::to_string(latest_timestamp) + "- timeavg) < " + std::to_string(time)  + " ORDER BY timeavg;";
  sql = sql_recgraphrois_string.c_str();
  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(conn);
    pqxx::result GetInfo = w.exec(sql);

    //Loop over results and set the point on the graph.
    for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
      MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetPoint(MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetN(),res[2].as<float>(),res[0].as<float>());
      MetricsROIs_AvgGraph_Map[MetricNameROIs]->SetPointError(MetricsROIs_AvgGraph_Map[MetricNameROIs]->GetN()-1,res[3].as<float>(),res[1].as<float>());
    }
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to recieve row to table of average rois metric " << MetricName << std::endl;       
    return;
  }

  //Move to the Channels folder in the TFile       
  OutputFile->cd();

  //Write the TGraphs to file
  MetricsROIs_AvgGraph_Map[MetricNameROIs]->Write();
  MetricsPixels_AvgGraph_Map[MetricNamePixels]->Write();

  //Add the MultiGraph 
  MetricsROIs_MultiGraph_Map[MetricNameROIs]->Add(MetricsROIs_AvgGraph_Map[MetricNameROIs]);
  MetricsPixels_MultiGraph_Map[MetricNamePixels]->Add(MetricsPixels_MultiGraph_Map[MetricNamePixels]);

  //Write Histograms to file 
  MetricsPixels_AvgHist_Map[MetricNamePixels]->Write();
  MetricsROIs_AvgHist_Map[MetricNameROIs]  ->Write();

  //Sort out the channel values
  for(int i=0; i<64; ++i){ 

    //Move to the correct channel folder
    std::string ChanFolder_String = "Channels/Channel " + std::to_string(i);
    const char * ChanFolder_name = ChanFolder_String.c_str();
    OutputFile->cd(ChanFolder_name);

    //Calculate mean and RMS
    float Metric_Chan_Mean = 0;
    float Metric_Chan_RMS  = 0;

    //Write the Histograms 
    Metrics_Hist_ChanMap[i][MetricName]->Write();

    if(Enteries != 0){
      Metric_Chan_Mean = Metrics_Hist_ChanMap[i][MetricName]->GetMean();
      Metric_Chan_RMS  = Metrics_Hist_ChanMap[i][MetricName]->GetRMS();

      //Send to PostGres Table  
      std::string sql_changraph_string = "INSERT INTO OnlineParamsAvg" + MetricName + "Chan"+ std::to_string(i) + " Values (" + std::to_string(Metric_Chan_Mean) + "," + std::to_string(Metric_Chan_RMS) + "," + Time_AVG + "," + Time_RMS + ");";

      if(Metrics_Max_ChanMap[i][MetricName] != -std::numeric_limits<float>::infinity() || 
	 Metrics_Min_ChanMap[i][MetricName] != std::numeric_limits<float>::infinity() || 
	 MetricsPixels_AvgMax_Map["timestampsecondPixels"] != -std::numeric_limits<float>::infinity() ||
	 MetricsPixels_AvgMin_Map["timestampsecondPixels"] != std::numeric_limits<float>::infinity()  ||
	 Metrics_Hist_ChanMap[i][MetricName]->GetEntries() != 0){

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
      }
    }

    //Fill the TGraph with postgres values
    std::string sql_recchangraph_string = "Select * FROM OnlineParamsAvg" + MetricName + "Chan" + std::to_string(i) + " WHERE (" + std::to_string(latest_timestamp) + "- timeavg) < " + std::to_string(time)  + " ORDER BY timeavg;";
    sql = sql_recchangraph_string.c_str();
    try{
      std::cout << "Running command: " << sql << std::endl;
      pqxx::work w(conn);
      pqxx::result GetInfo = w.exec(sql);
      
      //Loop over results and set the point on the graph.
      for (pqxx::result::const_iterator res = GetInfo.begin(); res != GetInfo.end(); ++res){
	 Metrics_Graph_ChanMap[i][MetricName]->SetPoint(Metrics_Graph_ChanMap[i][MetricName]->GetN(),res[2].as<float>(),res[0].as<float>());
	 Metrics_Graph_ChanMap[i][MetricName]->SetPointError( Metrics_Graph_ChanMap[i][MetricName]->GetN()-1,res[3].as<float>(),res[1].as<float>());
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
      MetricsPixels_MultiGraph_Map[MetricNamePixels]->Add(Metrics_Graph_ChanMap[i][MetricName]);
    }
    else{
      MetricsROIs_MultiGraph_Map[MetricNameROIs]->Add(Metrics_Graph_ChanMap[i][MetricName]);
    }
  }

  //Move back outside
  OutputFile->cd();

  //Fill the canvas
  Metrics_Canvas_Map[MetricName]->cd(1); 
  MetricsPixels_AvgHist_Map[MetricNamePixels]->Draw();
  Metrics_Canvas_Map[MetricName]->cd(2); 
  MetricsROIs_AvgHist_Map[MetricNameROIs]->Draw();
  Metrics_Canvas_Map[MetricName]->cd(3);
  MetricsPixels_MultiGraph_Map[MetricNamePixels]->Draw("A pmc plc");
  MetricsPixels_MultiGraph_Map[MetricNamePixels]->GetXaxis()->SetTimeDisplay(1);
  MetricsPixels_MultiGraph_Map[MetricNamePixels]->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M %F 1970-01-01 00:00:00"); 
  MetricsPixels_MultiGraph_Map[MetricNamePixels]->GetXaxis()->SetNdivisions(-404);

  gPad->BuildLegend();
  Metrics_Canvas_Map[MetricName]->cd(4);
  MetricsROIs_MultiGraph_Map[MetricNameROIs]->Draw("A pmc plc");
  MetricsROIs_MultiGraph_Map[MetricNameROIs]->GetXaxis()->SetTimeDisplay(1);
  MetricsROIs_MultiGraph_Map[MetricNameROIs]->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M %F 1970-01-01 00:00:00");
  MetricsROIs_MultiGraph_Map[MetricNameROIs]->GetXaxis()->SetNdivisions(-404);

  gPad->BuildLegend();
  Metrics_Canvas_Map[MetricName]->Update();
  Metrics_Canvas_Map[MetricName]->Write();

  //Save the canvas 
  std::string Path(gDirectory->GetPath());
  std::string NameString = Path.substr(0, Path.size()-2);
  NameString += "_" + MetricName + ".png";
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
    + std::to_string(tstruct.tm_sec)
    + ".root";
  const char*filename = filename_string.c_str();
  
  //And create
  this->OutputFile = new TFile(filename,"RECREATE");
  this->OutputFile->cd();
  this->OutputFile->mkdir("Channels");
  for(int i=0; i<64; ++i){  
    std::string ChanFolder_String = "Channels/Channel " + std::to_string(i);
    const char * ChanFolder_name = ChanFolder_String.c_str();
    this->OutputFile->mkdir(ChanFolder_name);
  }
  this->OutputFile->cd();
  return;
  
};

PixelData::OnlineMointoring::OnlineDataBaseHist::OnlineDataBaseHist(){
  this->SetupOutputFile();
};
