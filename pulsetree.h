//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 15 14:02:13 2018 by ROOT version 6.08/00
// from TTree pulsetree/Pulse Information
// found on file: /home/argonshef/LArAnalysis/CRTData/LArPMT20181114_141042.root
//////////////////////////////////////////////////////////

#ifndef pulsetree_h
#define pulsetree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class pulsetree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           channel;
   Int_t           source;
   Float_t         baseVolt;
   Float_t         baseAdc;
   Float_t         baseRmsVolt;
   Float_t         baseRmsAdc;
   Float_t         amplitudeVolt;
   Float_t         amplitudeAdc;
   Float_t         maxVolt;
   Float_t         maxAdc;
   Float_t         peaktimeSec;
   Float_t         peaktimeTdc;
   vector<float>   *waveform;
   Float_t         timestamp;
   Float_t         integral;

   // List of branches
   TBranch        *b_channel;   //!
   TBranch        *b_source;   //!
   TBranch        *b_baseVolt;   //!
   TBranch        *b_baseAdc;   //!
   TBranch        *b_baseRmsVolt;   //!
   TBranch        *b_baseRmsAdc;   //!
   TBranch        *b_amplitudeVolt;   //!
   TBranch        *b_amplitudeAdc;   //!
   TBranch        *b_maxVolt;   //!
   TBranch        *b_maxAdc;   //!
   TBranch        *b_peaktimeSec;   //!
   TBranch        *b_peaktimeTdc;   //!
   TBranch        *b_waveform;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_integral;   //!

   pulsetree(TTree *tree=0);
   virtual ~pulsetree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pulsetree_cxx
pulsetree::pulsetree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/argonshef/LArAnalysis/CRTData/LArPMT20181114_141042.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/argonshef/LArAnalysis/CRTData/LArPMT20181114_141042.root");
      }
      f->GetObject("pulsetree",tree);

   }
   Init(tree);
}

pulsetree::~pulsetree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pulsetree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pulsetree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pulsetree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   waveform = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("source", &source, &b_source);
   fChain->SetBranchAddress("baseVolt", &baseVolt, &b_baseVolt);
   fChain->SetBranchAddress("baseAdc", &baseAdc, &b_baseAdc);
   fChain->SetBranchAddress("baseRmsVolt", &baseRmsVolt, &b_baseRmsVolt);
   fChain->SetBranchAddress("baseRmsAdc", &baseRmsAdc, &b_baseRmsAdc);
   fChain->SetBranchAddress("amplitudeVolt", &amplitudeVolt, &b_amplitudeVolt);
   fChain->SetBranchAddress("amplitudeAdc", &amplitudeAdc, &b_amplitudeAdc);
   fChain->SetBranchAddress("maxVolt", &maxVolt, &b_maxVolt);
   fChain->SetBranchAddress("maxAdc", &maxAdc, &b_maxAdc);
   fChain->SetBranchAddress("peaktimeSec", &peaktimeSec, &b_peaktimeSec);
   fChain->SetBranchAddress("peaktimeTdc", &peaktimeTdc, &b_peaktimeTdc);
   fChain->SetBranchAddress("waveform", &waveform, &b_waveform);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("integral", &integral, &b_integral);
   Notify();
}

Bool_t pulsetree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pulsetree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pulsetree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pulsetree_cxx
