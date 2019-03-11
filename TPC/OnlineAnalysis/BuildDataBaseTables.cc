//Build Tables for the database.
//Dominic Barker 04/03/2019

#include <iostream>
#include <string>
#include <vector> 
#include <pqxx/pqxx> 


using namespace pqxx;

int main(int argc, char* argv[]) {

  try{
    pqxx::connection C("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C.is_open()) {
      std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    } 
    else {
      std::cout << "Can't open database" << std::endl;
      return 1;
    }

    std::vector<std::string> Metrics = {"timestampsecond","timestampnanosecond","channel","eventnumber","avgpeakheight","maxpeakheight","rms","baseline","numhits"}; 

    const char * sql;
    
    for(int j=0; j<Metrics.size(); ++j){
      
      std::string MetricName = Metrics[j];

      //Do the Pixel average  
      std::string sql_create_avgpixelstable ="CREATE TABLE OnlineParamsAvgPixels" + MetricName +  "pixels (mean float , RMS float , timeavg float, timerms float);"; 
      sql = sql_create_avgpixelstable.c_str();
      std::cout << sql << std::endl;
      pqxx::work wp(C);
      wp.exec(sql);
      wp.commit();
      std::cout << "Table created" << std::endl;

      //Do the Roi average  
      std::string sql_create_avgroistable ="CREATE TABLE OnlineParamsAvgROIs" + MetricName +  "rois (mean float , RMS float , timeavg float, timerms float);"; 
      sql = sql_create_avgroistable.c_str();
      std::cout << sql << std::endl;
      pqxx::work wr(C);
      wr.exec(sql);
      wr.commit();
      std::cout << "Table created" << std::endl;


      //Do the channels 
      for(int i=0; i<64; ++i){
	
	int channel = i;

	std::string sql_create_channeltable = "CREATE TABLE OnlineParamsAvg" + MetricName + "Chan" + std::to_string(channel) + "(mean float , RMS float , timeavg float, timerms float);"; 
	
	sql = sql_create_channeltable.c_str();
	
	std::cout << sql << std::endl;
	
	pqxx::work w(C);
	w.exec(sql);
	w.commit();
	std::cout << "Table created" << std::endl;
	C.disconnect ();
      } 
    }
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cout << "Failed to send" << std::endl;
    return 1;
  }
}
