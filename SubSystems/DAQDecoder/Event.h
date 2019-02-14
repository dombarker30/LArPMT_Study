//##########################################
// Class to hold CRT events. Each CRT event 
// will hold 2 channels which will hold the 
// corresponding peak heights and times...
//#########################################
#ifndef AEVENT_H_
#define AEVENT_H_

#include <vector> 
#include "ChannelInfo.h"

namespace PixelData{
  namespace SubSystems{
    class Event{
      
    public:
      Event(){
	eventnumber = -99999; 
      }
      
      const int EventNumber(){return eventnumber;}
      const std::vector<PixelData::SubSystems::ChannelInfo> GetChannels(){return channelvector;}
      
      void SetEventNumber(int event_num){eventnumber = event_num;}
      void AddChannel(PixelData::SubSystems::ChannelInfo channel){channelvector.push_back(channel);} 
      void Clear(){eventnumber = -999999; channelvector.clear();} 
      
      
    private: 
      int eventnumber;
      std::vector<PixelData::SubSystems::ChannelInfo> channelvector;
    
      PixelData::SubSystems::ChannelInfo channel1; 
      PixelData::SubSystems::ChannelInfo channel2;

    };
  }
}
    
#endif /* EVENT_H_ */
