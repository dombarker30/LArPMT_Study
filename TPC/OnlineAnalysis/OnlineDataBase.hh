#ifndef OnlineDataBase_H 
#define OnlineDataBase_H 

#include <iostream>
#include <string> 
#include <pqxx/pqxx> 
#include "OnlineMonitor.hh"

namespace PixelData{
  namespace OnlineMointoring{

    class OnlineDataBase{

    public:
      
      OnlineDataBase(){
	this->InitaliseDatabase();
      };
      
      int InitaliseDatabase();
      int SendToDatabase(PixelData::TPC::OnlineMonitor& online);
      
    private:

      pqxx::connection* C;
    };
  }
}

#endif
