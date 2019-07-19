#include "OnlineDataBase.hh"
#include <string>
 
int PixelData::OnlineMointoring::OnlineDataBase::InitaliseDatabase(){
  
  try {
    C = new pqxx::connection("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C->is_open()) {
      std::cout << "Opened database successfully: " << C->dbname() << std::endl;
      return 0;
    } 
    else {
      throw "Can't open database";
    }
  } 
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
};

int PixelData::OnlineMointoring::OnlineDataBase::SendToDatabase(PixelData::TPC::OnlineMonitor& online){
  
  const char * sql;
  std::string sql_sendgraphpixels_string = "INSERT INTO OnlineParams VALUES (" 
    + std::to_string(online.GetTimeStampN()) + "," + std::to_string(online.GetTimeStampS()) + "," + online.GetChannelID() + "," + std::to_string(online.GetAvgPeakHeight()) + "," + std::to_string(online.GetMaxPeakHeight()) + "," + std::to_string(online.GetBaseRMS()) + "," + std::to_string(online.GetBaseline()) + "," + std::to_string(online.GetNumHits()) + ")";
                    
  sql = sql_sendgraphpixels_string.c_str();

  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(*C);
    pqxx::result res = w.exec(sql);
    w.commit();
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to send online monitoring to database " << std::endl;
    return -1;
  }

  return 0;
};

