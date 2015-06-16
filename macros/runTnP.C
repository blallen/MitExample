#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TCanvas.h"

// RooFit Includes
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "RooPlot.h"

using namespace RooFit;

TH1F* MassPlot(TTree *pairs, bool requireID, TString varName) { 

  Float_t mass;
  // T var;
  Int_t passID;

  pairs->SetBranchAddress("mass"+TString(requireID), &mass);
  // pairs->SetBranchAddress(varName, &var);
  pairs->SetBranchAddress("passID", &passID);

  std::cout << "got branches" << endl;

  TH1F* hMass = new TH1F("mass", "mass_{\ell\ell}", 100, 0., 200.);
  Long64_t nEvents = pairs->GetEntries();

  for (Long64_t i=0; i<nEvents; i++) {
    pairs->GetEntry(i);
    if (requireID)
      if (!passID) continue;
    hMass->Fill(mass);
  }
  return hMass;
}

Double_t FitMassPlot(TH1F* hMass, TFile *storage, Long64_t nEvents) {
  RooRealVar mass("mass", "mass", 0., 200.);
  // RooAbsPdf *model = buildModel(mass);

  RooRealVar *m0 = new RooRealVar("m0", "m0", 90., 50., 130.);
  RooRealVar *width  = new RooRealVar("width", "width", 1.0, 0.0, 10.0);
  // RooBreitWigner *zpeak = new RooBreitWigner("zpeak", "zpeak", mass, *m0, *width);
  RooBreitWigner *signal = new RooBreitWigner("signal", "signal", mass, *m0, *width);

  // Detector Smearing
  RooRealVar *mean = new RooRealVar("mean", "mean", 0., -0.00009, 0.00009);
  RooRealVar *sigma = new RooRealVar("sigma", "sigma", 1., -100., 100.);
  RooRealVar *alpha = new RooRealVar("alpha", "alpha", 1000., 0., 10000.);
  RooRealVar *n = new RooRealVar("n", "n", 1, 0, 30000 );
  RooCBShape *smearing = new RooCBShape("smearing", "smearing", mass, *mean, *sigma, *alpha, *n);

  // smeared signal
  // RooFFTConvPdf *signal = new RooFFTConvPdf("signal", "Model: BreitWigner (x) Resolution: Crystal Ball", mass, *smearing, *zpeak);

  // background
  RooRealVar *lambda = new RooRealVar("lambda", "lambda", 0., -100., 0.);
  RooExponential *background = new RooExponential("background", "background", mass, *lambda);
  
  // full model
  RooRealVar *nsig = new RooRealVar("nsig", "nsig", 2000., 0., nEvents);
  RooRealVar *nbkg = new RooRealVar("nbkg", "nbkg", 2000., 0., nEvents);
  RooAbsPdf *model = (RooAbsPdf*) new RooAddPdf("model", "model", RooArgList(*signal, *background), RooArgList(*nsig, *nbkg));

  RooDataHist *datahist = (RooDataHist*) new RooDataHist("datahist", "datahist", mass, hMass);
  // RooFitResult *result = (RooFitResult*) model->fitTo(*datahist, Extended(kTRUE), Minimizer("Minuit2", "migrad"));
  model->fitTo(*datahist, Extended(kTRUE), Minimizer("Minuit2", "migrad"));
  storage->cd();
  // result->Write("fitresult");

  RooPlot *frame = mass.frame();
  datahist->plotOn(frame);
  model->plotOn(frame, Name("model"));

  TCanvas *canvas = new TCanvas();
  frame->Draw();
  canvas->Write();

  mass.setRange("Zpeak", 80., 100.);
  mass.setRange("all",0.,200.);
  RooArgSet argset = RooArgSet(mass);

  Long_t fPeak = signal->createIntegral( argset, "Zpeak")->getVal() / signal->createIntegral(argset, "all")->getVal();
  Long_t nPeak = fPeak * nsig->getVal();

  return nPeak;
}

Double_t CalcEff(TTree *pairs, TFile *storage){
  TH1F* hPass = MassPlot( pairs, true, "dummy");
  std::cout << "made passing mass plot" << endl;
  TH1F* hTotal = MassPlot( pairs, false, "dummy");
  std::cout << "made total mass plot" << endl;

  Long64_t nEvents = pairs->GetEntries();

  Double_t nPass = FitMassPlot(hPass, storage, nEvents);
  std::cout << "got number of pass pairs: " << nPass << endl;
  Double_t nTotal = FitMassPlot(hTotal, storage, nEvents);
  std::cout << "got number of total pairs: " << nTotal << endl;
  Double_t eff = nPass/nTotal;

  return eff;
}


void runTnP(TString inName = "/home/ballen/cms/hist/ntuples/merged/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_flat.root", TString outName = "/home/ballen/cms/root/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_plots.root") {

  std::cout << "can't do anything" << endl;

  TFile *inFile = new TFile(inName);
  
  std::cout << "opened file" << endl;

  TTree *pairs = (TTree*)inFile->FindObjectAny("events");
  pairs->GetEntries();
  std::cout << "have trees" << endl;

  TFile *outFile = new TFile(outName, "recreate");

  Double_t eff = CalcEff( pairs, outFile);

  std::cout << "Efficiency is " << eff << endl;

  return;
}


/*
RooAddPdf* buildModel(RooRealVar& mass, Long64_t nEvents) {
  // Z signal
  RooRealVar *m0 = new RooRealVar("m0", "m0", 90., 50., 130.);
  RooRealVar *width  = new RooRealVar("width", "width", 1.0, 0.0, 10.0);
  RooBreitWigner *zpeak = new RooBreitWigner("zpeak", "zpeak", mass, *m0, *width);

  // Detector Smearing
  RooRealVar *mean = new RooRealVar("mean", "mean", 0., -0.00009, 0.00009);
  RooRealVar *sigma = new RooRealVar("sigma", "sigma", 1., -100., 100.);
  RooRealVar *alpha = new RooRealVar("alpha", "alpha", 1000., 0., 10000.);
  RooRealVar *n = new RooRealVar("n", "n", 1, 0, 30000 );
  RooCBShape *smearing = new RooCBShape("smearing", "smearing", mass, *mean, *sigma, *alpha, *n);

  // smeared signal
  RooFFTConvPdf *signal = new RooFFTConvPdf("signal", "Model: BreitWigner (x) Resolution: Crystal Ball", mass, *smearing, *zpeak);

  // background
  RooRealVar *lambda = new RooRealVar("lambda", "lambda", 0., -100., 0.);
  RooExponential *background = new RooExponential("background", "background", mass, *lambda);
  
  // full model
  RooRealVar *nsig = new RooRealVar("nsig", "nsig", 2000., 0., nEvents);
  RooRealVar *nbkg = new RooRealVar("nbkg", "nbkg", 2000., 0., nEvents);
  RooAddPdf *model = new RooAddPdf("model", "model", RooArgList(*signal, *background), RooArgList(*nsig, *nbkg));

  return model;
} 
*/
