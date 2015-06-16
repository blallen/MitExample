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

void makeFlatTree(TString fileName = "/home/ballen/cms/hist/ntuples/merged/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim.root",
                     TString fileOutName = "/home/ballen/cms/hist/ntuples/merged/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_flat.root") {
  
  std::cout << "input file: " << fileName << endl;
  std::cout << "output file: " << fileOutName << endl;

  TFile * fIn = new TFile(fileName);
  TTree * events = (TTree*)fIn->FindObjectAny("events");
  unsigned nEvents = events->GetEntries();
  Float_t tagPt[64];
  Float_t tagEta[64];
  Float_t tagPhi[64];
  Float_t tagEnergy[64];
  Float_t probePt[64];
  Float_t probeEta[64];
  Float_t probePhi[64];
  Float_t probeEnergy[64];
  UInt_t  probePassID[64];
  UInt_t  nPairs;
  /*
    UInt_t nVertices;
  */
  
  events->SetBranchAddress("tag.pt",&tagPt);
  events->SetBranchAddress("tag.eta",&tagEta);
  events->SetBranchAddress("tag.phi",&tagPhi);
  events->SetBranchAddress("tag.energy",&tagEnergy);
  events->SetBranchAddress("probe.pt",&probePt);
  events->SetBranchAddress("probe.eta",&probeEta);
  events->SetBranchAddress("probe.phi",&probePhi);
  events->SetBranchAddress("probe.energy",&probeEnergy);
  events->SetBranchAddress("probe.passID",&probePassID);
  events->SetBranchAddress("nPairs",&nPairs);
  /*
    events->SetBranchAddress("nVertices",&nVertices);
  */

  Float_t mass; 
  Float_t eta;
  Float_t pt;
  Float_t phi;
  Int_t passID;
  TFile *fOut = new TFile(fileOutName,"RECREATE");
  TTree *tOut = new TTree("pairs","pairs");
  tOut->Branch("mass",&mass);
  tOut->Branch("pt",&pt);
  tOut->Branch("eta",&eta);
  tOut->Branch("phi",&phi);
  tOut->Branch("passID",&passID);
  // tOut->Branch("nVertices",&nVertices);
  for (unsigned i=0; i<nEvents; i++) {
    events->GetEntry(i);
    if ( i % 1000000 == 0) std::cout << "Event " << i << endl;
    for (unsigned j=0;j<nPairs; j++) {
      TLorentzVector vTag;
      vTag.SetPtEtaPhiE(tagPt[j],tagEta[j],tagPhi[j],tagEnergy[j]);
      TLorentzVector vProbe;
      vProbe.SetPtEtaPhiE(probePt[j],probeEta[j],probePhi[j],probeEnergy[j]);
      mass = (vTag+vProbe).M();
      pt = probePt[j];
      eta = probeEta[j];
      phi = probePhi[j];
      if (probePassID[j]) std::cout << "Event " << i << " passed id" << endl;
      std::cout << "probe pass ID = " << probePassID[j] << endl;
      passID = probePassID[j];
      tOut->Fill();
    }
  }
  
  fOut->cd();
  tOut->Write("pairs");
  fOut->Close(); 

  std::cout << "Done" << endl;

}
