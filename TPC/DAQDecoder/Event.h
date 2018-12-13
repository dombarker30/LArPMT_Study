//##########################################
//### Class to hold TPC events. 
//#########################################
#ifndef EVENT_H_
#define EVENT_H_

#include <vector> 
#include "ChannelInfo.h"

namespace PixelData{
  namespace TPC{

    class Event{

    public:

      Event(){
	EventNumber = -999999;
	TimeStamp   = -999999;
      };

      ~Event(){};

      const uint32_t GetEventNumber()                              const {return EventNumber;}
      const uint32_t GetTimeStamp()                                const {return TimeStamp;}
      const std::vector<PixelData::TPC::ChannelInfo> GetChannels() const {return ChannelVector;}

      void SetEventNumber(uint32_t EventNum){EventNumber = EventNum;}
      void SetTimestamp(uint32_t TS){TimeStamp = TS;}
      void AddBoardID(uint32_t boardid){BoardIDs.push_back(boardid);}
      void AddChannel(PixelData::TPC::ChannelInfo channel){ChannelVector.push_back(channel);} 
      void Clear(){
	EventNumber = -999999;
	TimeStamp   = -999999;
	BoardIDs.clear();
	ChannelVector.clear();
      } 
      
    private:
      uint32_t EventNumber;
      uint32_t TimeStamp;
      std::vector<PixelData::TPC::ChannelInfo> ChannelVector;

    public:
      std::vector<uint32_t> BoardIDs;
    };
  }
} 

#endif /* EVENT_H_ */ 
