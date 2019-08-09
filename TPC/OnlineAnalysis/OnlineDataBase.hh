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
      
      ~OnlineDataBase(){
	delete P;
	delete N; 
	delete C;
      }
      
      int  InitaliseDatabase();
      int  SendToDatabase(PixelData::TPC::OnlineMonitor& online);
      int  CloseDataBase();
      void Complete();
    private:

      pqxx::pipeline* P;
      pqxx::connection* C;
      pqxx::nontransaction* N;
    };
  }
}

#endif
