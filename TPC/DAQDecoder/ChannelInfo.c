#include "ChannelInfo.h"
#include <stddef.h>

PixelData::TPC::ChannelInfo::ChannelInfo(){
  ChannelNumber  = -99999;
  NADC           = -99999;
  Pedestal       = -99999;
};

PixelData::TPC::ChannelInfo::ChannelInfo(int ChanNum, std::string ChanID, int NumADC, float Ped, std::vector<int> WF){
  ChannelNumber = ChanNum;
  ChannelID     = ChanID;
  NADC          = NumADC;
  Pedestal      = Ped;
  Waveform      = WF;
};

void PixelData::TPC::ChannelInfo::Clear(){
  ChannelNumber  = -99999;
  ChannelID.clear();
  NADC           = -99999;
  Pedestal       = -99999;
  Waveform.clear();
};
