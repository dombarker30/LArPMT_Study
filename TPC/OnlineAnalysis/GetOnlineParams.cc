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

//Postgres Includes
#include <pqxx/pqxx> 

//Database class includes
#include "OnlineDataBaseHist.hh"

int main(int argc, char* argv[]) {

  //Get the time in mins you want to average over and make the histograms. 
  float time_avg = std::atoi(argv[1])*60;

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

    //Initalise the Online Figures.
    OnlineHistograms.InitialiseFigure("timestampsecond","Time Stampstamp (s)",100,0,1);
    OnlineHistograms.InitialiseFigure("timestampnanosecond","Nanosecond in the timestamp",100,0,1);
    OnlineHistograms.InitialiseFigure("channel","Channel Number",64,0,64);
    OnlineHistograms.InitialiseFigure("eventnumber", "Event Number",100,0,100);
    OnlineHistograms.InitialiseFigure("averagepeakheightpixels","Average Peak Height Pixels",100,0,100);
    OnlineHistograms.InitialiseFigure("maxpeakheightpixels","Max Peak Height (ADC)",100,0,100);
    OnlineHistograms.InitialiseFigure("rmspixels","RMS (ADC)",50,0,5);
    OnlineHistograms.InitialiseFigure("baselinepixels","Baseline (ADC)",50,0,10);
    OnlineHistograms.InitialiseFigure("numberhitspixels","Hit Number",50,0,50);
    
    //Get the most recent timestamp to work back from. 
    int latesttimestamp = OnlineHistograms.LastestTimeStamp(N);

    //Get the metric vector.
    std::vector<std::string> MetricVector = OnlineHistograms.GetMetricVector();

    //Fill the figures 
    OnlineHistograms.FillFigures(time_avg,latesttimestamp,MetricVector,N);

    //Plot the figures 
    OnlineHistograms.PlotFigures(MetricVector,C);

  }
  catch(const std::exception &e){     
    std::cerr << e.what() << std::endl;                                           
    std::cout << "Failed" << std::endl;
    return -1; 
  }

  return 0;
}

