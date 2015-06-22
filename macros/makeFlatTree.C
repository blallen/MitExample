#include <iostream>
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TMath.h"
#include "MitExample/DataFormats/interface/TnPEvent.h"
#include "TLorentzVector.h"
// #include "RooDataSet.h"
// #include "RooRealVar.h"
// #include "RooWorkspace.h"

using namespace mithep;
// using namespace RooFit;

void makeFlatTree(TString inName = "/home/ballen/cms/hist/ntuples/merged/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim.root",
                     TString outName = "/home/ballen/cms/root/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_flat.root") {
  
  std::cout << "input file: " << inName << endl;
  std::cout << "output file: " << outName << endl;

  TFile *fIn = new TFile(inName+TString(".root"));
  TTree *events = (TTree*)fIn->FindObjectAny("events");
  unsigned nEvents = events->GetEntries();
  
  Int_t tagCharge[64]; // might convert to Int_t at some point
  Float_t tagPt[64];
  Float_t tagEta[64];
  Float_t tagPhi[64];
  Float_t tagEnergy[64];
  
  Int_t probeCharge[64]; // might convert to Int_t at some point
  Float_t probePt[64];
  Float_t probeEta[64];
  Float_t probePhi[64];
  Float_t probeEnergy[64];
  UInt_t  probePassID[64];

  UInt_t  nPairs;
  UInt_t  nVertices;
  
  events->SetBranchAddress("tag.charge",&tagCharge);
  events->SetBranchAddress("tag.pt",&tagPt);
  events->SetBranchAddress("tag.eta",&tagEta);
  events->SetBranchAddress("tag.phi",&tagPhi);
  events->SetBranchAddress("tag.energy",&tagEnergy);

  events->SetBranchAddress("probe.charge",&probeCharge);
  events->SetBranchAddress("probe.pt",&probePt);
  events->SetBranchAddress("probe.eta",&probeEta);
  events->SetBranchAddress("probe.phi",&probePhi);
  events->SetBranchAddress("probe.energy",&probeEnergy);
  events->SetBranchAddress("probe.passID",&probePassID);

  events->SetBranchAddress("nPairs",&nPairs);
  events->SetBranchAddress("nVertices",&nVertices);
  
  TFile *fOut = new TFile(outName+TString("_flat.root"),"RECREATE");
  TTree *tOut = new TTree("pairs","pairs");

  Float_t mass;
  Float_t eta;
  Float_t pt;
  Float_t phi;
  UInt_t passID;
  UInt_t OS;

  tOut->Branch("mass",&mass);
  tOut->Branch("passID",&passID);
  tOut->Branch("OS",&OS);
  tOut->Branch("pt",&pt);
  tOut->Branch("eta",&eta);
  tOut->Branch("phi",&phi);
  tOut->Branch("nVertices",&nVertices);
  
  for (unsigned i=0; i<nEvents; i++) {
    events->GetEntry(i);
    if ( i % 1000000 == 0) std::cout << "Event " << i << endl;
    for (unsigned j=0;j<nPairs; j++) {
      

      TLorentzVector vTag;
      vTag.SetPtEtaPhiE(tagPt[j],tagEta[j],tagPhi[j],tagEnergy[j]);
      TLorentzVector vProbe;
      vProbe.SetPtEtaPhiE(probePt[j],probeEta[j],probePhi[j],probeEnergy[j]);
      
      mass = (vTag+vProbe).M();
      OS = (tagCharge[j] == probeCharge[j]) ? 0 : 1;
      passID = probePassID[j];
      pt = probePt[j];
      eta = probeEta[j];
      phi = probePhi[j];
      
      tOut->Fill();
    }
  }
  
  fOut->cd();
  tOut->Write("pairs");
  fOut->Close(); 

  std::cout << "Done" << endl;

}
