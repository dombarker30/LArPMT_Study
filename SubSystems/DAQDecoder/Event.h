//##########################################
// Class to hold CRT events. Each CRT event 
// will hold 2 channels which will hold the 
// corresponding peak heights and times...
//#########################################
#include <vector> 
#include "ChannelInfo.h"

class Event{

 public:
  Event();
  int EventNumber(){return event_number;}
  std::vector<ChannelInfo> Channels(){return channel_vector;}
  void Set_EventNumber(int event_num){event_number = event_num;}
  void Add_Channel(ChannelInfo channel){channel_vector.push_back(channel);} 
  void Clear(){event_number = -999999; channel_vector.clear();} 

 private: 
  int event_number;
  std::vector<ChannelInfo> channel_vector;
  
};


Event::Event(){
  event_number = -99999;
}
