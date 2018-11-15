#define pulsetree_cxx
#include "pulsetree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void pulsetree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L pulsetree.C
//      root> pulsetree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nEntries = fChain->GetEntriesFast();

   std::vector<int> vChannel;
   std::vector<float> vMaxVoltage;
   std::vector<float> vPeakTime;
   std::vector<float> vTimestamp;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nEntries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      vChannel.push_back(channel);
      vMaxVoltage.push_back(maxVolt);
      vPeakTime.push_back(peaktimeSec);
      vTimestamp.push_back(timestamp);

   }
   int loopy = 1;
   int loopysum=0;
  
   for(int i=0;i<nEntries;i=i+1){
     
     printf("Ch. %i, %.09f, %f\n",vChannel[i],vPeakTime[i],vMaxVoltage[i]);
      
     if(vTimestamp[i]==vTimestamp[i+1]){
       loopy++;
     }else{
       printf("%i\n",loopy);
       loopysum+=loopy;
       loopy = 1;
     }
   }
   //printf("%i\n",loopysum);
   //std::cout << "\n\n" << nEntries << "\n\n";
}
