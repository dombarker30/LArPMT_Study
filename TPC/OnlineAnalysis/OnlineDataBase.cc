#include "OnlineDataBase.hh"
#include <string>
 
int PixelData::OnlineMointoring::OnlineDataBase::InitaliseDatabase(){
  
  try {
    C = new pqxx::connection("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C->is_open()) {
      std::cout << "Opened database successfully: " << C->dbname() << std::endl;
      N = new pqxx::nontransaction(*C);
      P = new pqxx::pipeline(*N);
      P->retain(64); //Retain 64 entries before sending it the database.
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
    + std::to_string(online.GetTimeStampS()) + "," + std::to_string(online.GetTimeStampN()) + "," + std::to_string(online.GetChannel()) + "," + std::to_string(online.GetAvgPeakHeight()) + "," + std::to_string(online.GetMaxPeakHeight()) + "," + std::to_string(online.GetBaseRMS()) + "," + std::to_string(online.GetBaseline()) + "," + std::to_string(online.GetNumHits()) + ")";
                    
  sql = sql_sendgraphpixels_string.c_str();

  try{
    P->insert(sql);
  }
  catch(const std::exception &e){
    std::cerr << e.what() << std::endl;
    std::cout << " Failed to send online monitoring to database " << std::endl;
    return -1;
  }
  return 0;
};

void PixelData::OnlineMointoring::OnlineDataBase::Complete(){
  P->complete();
}

int PixelData::OnlineMointoring::OnlineDataBase::CloseDataBase(){
  C->disconnect();
  return 0;
}
