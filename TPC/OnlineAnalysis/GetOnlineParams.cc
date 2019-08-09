//####################################################
//## Name:        GetOnlineParams                    #
//## Author:      Dominic Barker                     #
//## Date:        26.02.2019                         #
//## Description: Access the online database and     #
//##              create root histograms for display #
//##              run with one input: the number of  #
//##              mins you want to average the       #
//##              values over                        # 
//####################################################

//Thanks SQL example for easy copy.
 
//C++ Includes
#include <iostream>
#include <stdlib.h> 
#include <string>
#include <ctime>

//Postgres Includes
#include <pqxx/pqxx> 

//Database class includes
#include "OnlineDataBaseHist.hh"

int main(int argc, char* argv[]) {

  if(argc != 3){
    std::cerr << "please give one input and that is the time you want to average over in mins and the time in hours you want to go back in" << std::endl;
    return 1;
  }

  //Get the time in mins you want to average over and make the histograms. 
  float time_avg = std::atoi(argv[1])*60;

  //Get the time to go back in hours 
  uint32_t time =  std::atoi(argv[2])*60*60;

  //Get the database 
  try {
    pqxx::connection C("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C.is_open()) {
      std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    } else {
      std::cout << "Can't open database" << std::endl;
      return 1;
    }

    
    //Create a non-transactional object. 
    pqxx::nontransaction N(C);

    //Initalise the Online Mointoring database class.
    PixelData::OnlineMointoring::OnlineDataBaseHist OnlineHistograms;

    OnlineHistograms.InitialiseLimits("timestampsecond");
    OnlineHistograms.InitialiseLimits("timestampnanosecond");
    OnlineHistograms.InitialiseLimits("channel");
    OnlineHistograms.InitialiseLimits("avgpeakheight");
    OnlineHistograms.InitialiseLimits("maxpeakheight");
    OnlineHistograms.InitialiseLimits("rms");
    OnlineHistograms.InitialiseLimits("baseline");
    OnlineHistograms.InitialiseLimits("numhits");

    //Get the most recent timestamp to work back from. 
    int latesttimestamp =  std::time(nullptr);
    
    //Get the metric vector.
    std::vector<std::string> MetricVector = OnlineHistograms.GetMetricVector();

    //Get the limits 
    OnlineHistograms.GetAllLimits(time_avg,latesttimestamp,MetricVector,N);

    //Initalise the Online Figures.
    OnlineHistograms.InitialiseFigure("timestampsecond","Time Stampstamp (s)");
    OnlineHistograms.InitialiseFigure("timestampnanosecond","Nanosecond in the timestamp");
    OnlineHistograms.InitialiseFigure("channel","Channel Number");
    OnlineHistograms.InitialiseFigure("avgpeakheight","Average Peak Height");
    OnlineHistograms.InitialiseFigure("maxpeakheight","Max Peak Height (ADC)");
    OnlineHistograms.InitialiseFigure("rms","RMS (ADC)");
    OnlineHistograms.InitialiseFigure("baseline","Baseline (ADC)");
    OnlineHistograms.InitialiseFigure("numhits","Hit Number");
    
    //Fill the figures 
    OnlineHistograms.FillFigures(time_avg,latesttimestamp,MetricVector,N);

    N.commit();

    //Plot the figures 
    OnlineHistograms.PlotFigures(MetricVector,C,time);

  }
  catch(const std::exception &e){     
    std::cerr << e.what() << std::endl;                                           
    std::cout << "Failed" << std::endl;
    return -1; 
  }

  return 0;
}

