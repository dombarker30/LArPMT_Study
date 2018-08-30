#include <iostream>
#include <string.h>
#include <cmath>

#include "TROOT.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TF1.h"
#include "TF1Convolution.h"
#include "TDirectory.h"
#include "TCanvas.h"

#include "TRandom.h"


void WriteEventWaveform(std::vector<float> event_nums){
  

