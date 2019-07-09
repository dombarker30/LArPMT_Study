//####################################################
//## Name:        OnlineDataBaseHist                 #
//## Date:        28.02.2019                         #
//## Author:      Dominic Barker                     #
//##                                                 #
//## Description: This class is used to inialise and #
//##              fill histogram and time graphs of  #
//##              the metrics for online mointoring  #
//####################################################

#ifndef ONLINEDATABASEHIST_H 
#define ONLINEDATABASEHIST_H 

//C++ Includes
#include <iostream>
#include <stdlib.h> 
#include <string>
#include <ctime>
#include <map>

//Postgres Includes
#include <pqxx/pqxx> 

//Root Includes 
#include "TH1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h" 

namespace PixelData{
  namespace OnlineMointoring{ 

    class OnlineDataBaseHist{

    public:
      OnlineDataBaseHist();
      ~OnlineDataBaseHist(){};

      int  LastestTimeStamp(pqxx::nontransaction& N); //Get the latest event time
      void InitialiseFigure(std::string MetricName, std::string MetricTitle, int bins, float low, float high); //Create the figures
      void FillFigures(float time_avg, int lastesttimestamp, std::vector<std::string> MetricNames, pqxx::nontransaction& N); //Fill the figures from info from the database 
      void FillFigure(std::string MetricName, float value, std::string channelID, int channel); //Fill the figures for the metric
      void FillFigure(std::string MetricName, int value, std::string channelID, int channel);   //Fill the figures for the metric  
      void PlotFigures(std::vector<std::string> MetricNames, pqxx::connection& conn); //Plot the figures
      void PlotFigure(std::string MetricName, pqxx::connection& conn);
      void SetupOutputFile(); //Create an output file the date created variable
      std::vector<std::string> GetMetricVector(){return this->MetricVector;}

      TFile* OutputFile;

      //Vector of the Metric Strings added 
      std::vector<std::string> MetricVector;
      
      //Average Histograms and Graphs for the pixels and ROIS.
      std::map<std::string,TH1F*>         MetricsROIs_AvgHist_Map;
      std::map<std::string,TGraphErrors*> MetricsROIs_AvgGraph_Map;
      std::map<std::string,TMultiGraph*>  MetricsROIs_MultiGraph_Map;
      std::map<std::string,TH1F*>         MetricsPixels_AvgHist_Map;
      std::map<std::string,TGraphErrors*> MetricsPixels_AvgGraph_Map;
      std::map<std::string,TMultiGraph*>  MetricsPixels_MultiGraph_Map;
      std::map<std::string,TCanvas*>      Metrics_Canvas_Map;

      //Initalise Histograms and Graphs per channel.
      std::map<int,std::map<std::string,TH1F*> >         Metrics_Hist_ChanMap;   
      std::map<int,std::map<std::string,TGraphErrors*> > Metrics_Graph_ChanMap;   


    }; 
  }
}

#endif /* ONLINEDATABASEHIST_H */
