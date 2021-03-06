#include "PhysicalChannelMap.hh"

std::pair<int,int> GetXYCoords(std::string channelID){
  int x,y = -10;
  int chanStrLength = channelID.size();
  
  //See if it is a pixel or ROI string. (Think this will be quicker than `if(channelID.BeginsWith("Pixel"))`).
  if(chanStrLength>5){
    int chanNum = std::stoi(channelID.substr(5,chanStrLength-1));
    x = (chanNum%6)*2;
    if(x == 0){x=12;}
    
    if(chanNum<7){y=12;}
    else if(chanNum<13){y=10;}
    else if(chanNum<19){y=8;}
    else if(chanNum<25){y=6;}
    else if(chanNum<31){y=4;}
    else{y=2;}
    
    
  }
  //Else it is an ROI string (should probs put a test here for bad strings).
  else{
    int chanNum = std::stoi(channelID.substr(3,chanStrLength-1));
    switch (chanNum) {
        case 1: x=1;y=8;break;
        case 2: x=1;y=6;break;
        case 3: x=1;y=4;break;
        case 4: x=3;y=9;break;
        case 5: x=3;y=7;break;
        case 6: x=3;y=5;break;
        case 7: x=3;y=3;break;
        case 8: x=4;y=1;break;
        case 9: x=4;y=11;break;
        case 10: x=5;y=9;break;
        case 11: x=5;y=7;break;
        case 12: x=5;y=5;break;
        case 13: x=5;y=3;break;
        case 14: x=6;y=1;break;
        case 15: x=6;y=11;break;
        case 16: x=7;y=9;break;
        case 17: x=7;y=7;break;
        case 18: x=7;y=5;break;
        case 19: x=7;y=3;break;
        case 20: x=8;y=1;break;
        case 21: x=8;y=11;break;
        case 22: x=9;y=9;break;
        case 23: x=9;y=7;break;
        case 24: x=9;y=5;break;
        case 25: x=9;y=3;break;
        case 26: x=11;y=8;break;
        case 27: x=11;y=6;break;
        case 28: x=11;y=4;break;
      }
  }
  std::pair<int,int> xy(x,y);
  return(xy);
}
