#include "SendToDatabase.hh"

pqxx::connection PixelData::OnlineMointoring::OnlineDataBase::InitaliseDatabase(){
  
  try {
    pqxx::connection C("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C.is_open()) {
      std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    } else {
      std::cout << "Can't open database" << std::endl;
      return 1;
    }
  } 
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
};

int PixelData::OnlineMointoring::OnlineDataBase::SendToDatabase(pqxx::connection& C, PixelData::TPC::OnlineMonitor& online){
  
  const char * sql;
  std::string sql_sendgraphpixels_string = "INSERT INTO OnlineParamsAvgPixels VALUES (" 
    + std::to_string(online
                    
  sql = sql_sendgraphpixels_string.c_str();

  try{
    std::cout << "Running command: " << sql << std::endl;
    pqxx::work w(conn);
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

