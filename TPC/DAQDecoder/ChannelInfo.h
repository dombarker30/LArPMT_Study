//##################################
// This class holds the channel 
// information to be placed in root
//##################################
#ifndef CHANNELINFO_H_
#define CHANNELINFO_H_

#include <vector>
#include <stdint.h>

namespace PixelData{
  namespace TPC{ 

    class ChannelInfo{

    public:
      
      ChannelInfo();
      ~ChannelInfo(){};
      ChannelInfo(int ChanNum, int NumADC, float Ped, std::vector<int> WF);

      const std::vector<int> GetWaveform()      const {return Waveform;}
      const float            GetPedestal()      const {return Pedestal;}
      const int              GetChannelNumber() const {return ChannelNumber;}
      const int              GetNADC()          const {return NADC;}

      void Clear();

    private: 

      int               ChannelNumber;
      int               NADC;
      float             Pedestal;
      std::vector<int>  Waveform;
      
    }; 
  }
}

#endif /* CHANNELINFO_H_ */
