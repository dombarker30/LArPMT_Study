//##################################
// This class holds the channel for
// the subsystems CRT and PMT 
// information to be placed in root
//##################################
#ifndef ACHANNELINFO_H_
#define ACHANNELINFO_H_

#include <vector>
#include <map>

namespace PixelData{
  namespace SubSystems{
    class ChannelInfo{

    public:

      Int_t     channel;
      Float_t   baseVolt;
      Float_t   baseAdc;
      Float_t   baseRmsVolt;
      Float_t   baseRmsAdc;
      Float_t   maxVolt;
      Float_t   maxAdc;
      Float_t   peaktimeSec;
      Float_t   peaktimeTdc;
      std::vector<Float_t>  waveform;
      std::pair<int,int> timestamp;
      Float_t   integral; 
      Float_t   integralCharge;
    
      ChannelInfo(){
	channel  = -99999;
	baseVolt = -99999;
	baseAdc  = -99999;
	baseRmsVolt = -99999;
	baseRmsAdc = -99999;
	maxVolt = -99999;
	maxAdc = -99999; 
	peaktimeSec = -99999;
	peaktimeTdc = -99999;
	timestamp = std::make_pair(-99999,-99999);
	integral = -99999;
	integralCharge = -99999;
      }
    
      ChannelInfo(Int_t chan, Float_t baseVoltage,Float_t baselineAdc, Float_t baseRmsAdcs, Float_t baseRmsVolts, Float_t maxVolts, Float_t maxAdcs, Float_t peaktimeSecs, Float_t peaktimeTdcs, std::vector<Float_t> wf, std::pair<int,int> ts, Float_t sum, Float_t sumCharge){
	channel = chan;
	baseVolt = baseVoltage;
	baseAdc = baselineAdc;
	baseRmsAdc = baseRmsAdcs;
	baseRmsVolt = baseRmsVolts;
	maxVolt = maxVolts;
	maxAdc = maxAdcs;
	peaktimeSec = peaktimeSecs;
	peaktimeTdc = peaktimeTdcs;
	waveform = wf; 
	timestamp.first = ts.first;
	timestamp.second = ts.second;
	integral = sum;
	integralCharge = sumCharge;
      }
    };
  }
}
    
#endif /* CHANNELINFO_H_ */
