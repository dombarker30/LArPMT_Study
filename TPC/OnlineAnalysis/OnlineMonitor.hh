//##################################
// This class holds the Online 
// information to be placed in root
//##################################
#ifndef ONLINEMONITOR_H_
#define ONLINEMONITOR_H_

#include <vector>
#include <string>
#include <stdint.h>

namespace PixelData{
  namespace TPC{ 

    class OnlineMonitor{

    public:
      
      OnlineMonitor();
      ~OnlineMonitor(){};
      OnlineMonitor(int Channel, std::string ChannelID, float Baseline, float BaseRMS, float AvgPeakHeight, float MaxPeakHeight, int MaxPeakTime, int NumHits,int EventNum, int TimeStampS, int TimeStampN);

      OnlineMonitor(int Channel, std::string ChannelID, float Baseline, float BaseRMS,int EventNum, int TimeStampS, int TimeStampN);


      const int              GetChannel()       const {return Channel;};
      std::string            GetChannelID()     const {return ChannelID;};
      const float            GetBaseline()      const {return Baseline;};
      const float            GetBaseRMS()       const {return BaseRMS;};
      const float            GetAvgPeakHeight() const {return AvgPeakHeight;};
      const float            GetMaxPeakHeight() const {return MaxPeakHeight;};
      const float            GetMaxPeakTime()   const {return MaxPeakTime;};
      const int              GetNumHits()       const {return NumHits;};
      const int              GetEventNum()      const {return EventNum;};
      const int              GetTimeStampS()    const {return TimeStampS;};
      const int              GetTimeStampN()    const {return TimeStampN;};

      //PixelData::TPC::Onlinemonitor RunOnline(bool SecondPass = false);

    private: 

      float       Baseline;
      float       BaseRMS;
      float       AvgPeakHeight;
      float       MaxPeakHeight;
      float       MaxPeakTime;
      int         NumHits;
      int         Channel;
      std::string ChannelID;
      int         EventNum; 
      int         TimeStampS; // Time in seconds
      int         TimeStampN; // Time in nanoseconds

    }; 
    OnlineMonitor RunOffline(const std::vector<int> &waveform, int Evt, int Chan, std::string ChanID, int Time, bool CalcPeaks = true, bool SecondPass = false, bool verbose = false);
    OnlineMonitor RunOnline(uint16_t *waveform, int nADC, int Evt, int Chan, std::string ChanID, int Time, bool CalcPeaks = true, bool SecondPass = false, bool verbose = false);
    //OnlineMonitor RunOnline(unsigned short *waveform, bool CalcPeaks = true, bool SecondPass = false, bool verbose = false);

    OnlineMonitor RunBaseLine(const std::vector<int> &waveform, int Evt, int Chan, std::string ChanID, int Time, bool verbose = false);
    OnlineMonitor RunPeaks(const std::vector<int> &waveform, int Evt, int Chan, std::string ChanID, int Time, bool verbose = false);
    OnlineMonitor RunSecondPass(const std::vector<int> &waveform, int Evt, int Chan, std::string ChanID, int Time, bool verbose = false);
  }
}

#endif /* ONLINEMONITOR_H_ */
