


#ifndef OnlineEventDisplay_hh
#define OnlineEventDisplay_hh

//DAQ Includes
#include "/home/argonshef/CAENDAQ/PixelDAQ/include/ChannelMap.hh"

//C++ Includes
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

//ROOT Includes
#include "TROOT.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TStyle.h"

namespace PixelData{
  namespace TPC{

    TCanvas* PixelXYPlot(std::vector<std::string> channelIDs, std::vector<int> numHitsEvent, std::vector<float> maxPeakHeights, int eventNum, std::map<int,std::string> channelMap);

    TCanvas* PixelXYTPlot(std::vector<std::string> channelIDs, std::vector<int> numHitsEvent, std::vector<float> maxPeakHeights, std::vector<float> maxPeakTimes, int eventNum, std::map<int,std::string> channelMap);

  }
}

#endif //OnlineEventDisplay_hh
