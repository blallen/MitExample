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
#include "RooPolynomial.h"

using namespace RooFit;

TH1F* MassPlot(TTree *pairs, bool requireID, TString varName, Float_t minVal, Float_t maxVal) { 

  // Float_t mass;
  // T var;
  // Int_t passID;

  // pairs->SetBranchAddress("mass"+TString(requireID), &mass);
  // pairs->SetBranchAddress(varName, &var);
  // pairs->SetBranchAddress("passID", &passID);

  // std::cout << "got branches" << endl;

  TString plotName = TString::Format("mass_%s_%.2f_%.2f_",varName.Data(),minVal,maxVal)+TString(requireID ? "passID" : "noID");
  TH1F *hMass = new TH1F(plotName, "mass_{\ell\ell}", 200, 0., 200.);
  
  TString cut = TString::Format("( %s > %f ) && ( %s < %f)", varName.Data(), minVal, varName.Data(), maxVal);
  if (requireID) cut = cut + TString(" && (passID == 1)");
  pairs->Draw(TString("mass>>")+plotName,cut,"goff");

  /*
  Long64_t nEvents = pairs->GetEntries();
  for (Long64_t i=0; i<nEvents; i++) {
    pairs->GetEntry(i);
    if (requireID)
      if (!passID) continue;
    hMass->Fill(mass);
  }
  */
  return hMass;
}

Double_t FitMassPlot(TH1F* hMass, /*TFile *storage,*/ Long64_t nEvents) {
  RooRealVar mass("mass", "mass", 0., 200.);
  // RooAbsPdf *model = buildModel(mass);

  RooRealVar *m0 = new RooRealVar("m0", "m0", 90., 50., 130.);
  RooRealVar *width  = new RooRealVar("width", "width", 1.0, 0.0, 10.0);
  RooBreitWigner *zpeak = new RooBreitWigner("zpeak", "zpeak", mass, *m0, *width);
  RooBreitWigner *signal = new RooBreitWigner("signal", "signal", mass, *m0, *width);

  // Detector Smearing
  RooRealVar *mean = new RooRealVar("mean", "mean", 0., -0.00009, 0.00009);
  RooRealVar *sigma = new RooRealVar("sigma", "sigma", 1., -100., 100.);
  RooRealVar *alpha = new RooRealVar("alpha", "alpha", 1000., 0., 10000.);
  RooRealVar *n = new RooRealVar("n", "n", 1, 0, 30000 );
  RooCBShape *smearing = new RooCBShape("smearing", "smearing", mass, *mean, *sigma, *alpha, *n);

  // smeared signal
  // RooFFTConvPdf *signal = new RooFFTConvPdf("signal", "Model: BreitWigner (x) Resolution: Crystal Ball", mass, *smearing, *zpeak);
  // RooFFTConvPdf *model = new RooFFTConvPdf("signal", "Model: BreitWigner (x) Resolution: Crystal Ball", mass, *smearing, *zpeak);

  // background
  RooRealVar *lambda = new RooRealVar("lambda", "lambda", 0., -100., 0.);
  // RooExponential *background = new RooExponential("background", "background", mass, *lambda);
  RooRealVar *c0 = new RooRealVar("c0","c0",0.,-50.,50.);
  RooRealVar *c1 = new RooRealVar("c1","c1",0.,-50.,50.);
  RooRealVar *c2 = new RooRealVar("c2","c2",0.,-50.,50.);
  RooRealVar *c3 = new RooRealVar("c3","c3",0.,-50.,50.);
  RooRealVar *c4 = new RooRealVar("c4","c4",0.,-50.,50.);
  RooPolynomial *background = new RooPolynomial("background", "background", mass, RooArgList(*c0, *c1, *c2, *c3, *c4));

  // full model
  RooRealVar *nsig = new RooRealVar("nsig", "nsig", 2000., 0., nEvents);
  RooRealVar *nbkg = new RooRealVar("nbkg", "nbkg", 0., 0., nEvents);
  RooAbsPdf *model = (RooAbsPdf*) new RooAddPdf("model", "model", RooArgList(*signal, *background), RooArgList(*nsig, *nbkg));
  // RooAbsPdf *model = (RooAbsPdf*) new RooAddPdf("model", "model", RooArgList(*signal), RooArgList(*nsig));

  RooDataHist *datahist = (RooDataHist*) new RooDataHist("datahist", "datahist", mass, hMass);
  // RooFitResult *result = (RooFitResult*) model->fitTo(*datahist, Extended(kTRUE), Minimizer("Minuit2", "migrad"));
  model->fitTo(*datahist, Extended(kTRUE), Minimizer("Minuit2", "migrad"));
  // storage->cd();
  // result->Write("fitresult");

  RooPlot *frame = mass.frame();
  datahist->plotOn(frame);
  model->plotOn(frame, Name("model"));
  model->paramOn(frame);

  TCanvas *canvas = new TCanvas();
  frame->Draw("goff");
  canvas->SaveAs(TString(TString("plots/")+hMass->GetName())+TString("_BW.pdf"));

  mass.setRange("Zpeak", 80., 100.);
  mass.setRange("all",0.,200.);
  RooArgSet argset = RooArgSet(mass);

  Double_t fPeak = signal->createIntegral( argset, "Zpeak")->getVal() / signal->createIntegral(argset, "all")->getVal();
  Double_t nPeak = fPeak * nsig->getVal();

  return nPeak;
}

Double_t CalcEff(TTree *pairs, TString varName, Float_t minVal, Float_t maxVal){
  TH1F* hPass = MassPlot( pairs, true, varName, minVal, maxVal);
  std::cout << "made passing mass plot" << endl;
  TH1F* hTotal = MassPlot( pairs, false, varName, minVal, maxVal);
  std::cout << "made total mass plot" << endl;

  Long64_t nEvents = pairs->GetEntries();

  Double_t nPass = FitMassPlot(hPass, nEvents);
  std::cout << "got number of pass pairs: " << nPass << endl;
  Double_t nTotal = FitMassPlot(hTotal, nEvents);
  std::cout << "got number of total pairs: " << nTotal << endl;
  Double_t eff = nPass/nTotal;

  return eff;
}

TH1F* DiffEffPlot(TTree *pairs, TFile *storage, TString varName, Float_t nBins, Float_t minVal, Float_t maxVal) {
  Double_t step = (maxVal - minVal) / nBins;
  TH1F* hDiffEff = new TH1F(varName, TString("Eff for ")+varName, nBins, minVal, maxVal);
  for (int i = 0; i < nBins; i++) {
    Double_t eff = CalcEff(pairs, varName, (minVal + i*step), (minVal + (i+1)*step));
    hDiffEff->SetBinContent(i+1, eff);
  }
  storage->cd();
  hDiffEff->Write();
  return hDiffEff;
}

void runTnP(TString inName = "/home/ballen/cms/root/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_flat.root", TString outName = "/home/ballen/cms/root/ntuples_SingleElectron+Run2012A-22Jan2013-v1+AOD_noskim_plots.root") {

  std::cout << "can't do anything" << endl;

  TFile *inFile = new TFile(inName);
  
  std::cout << "opened file" << endl;

  TTree *pairs = (TTree*)inFile->FindObjectAny("pairs");
  
  std::cout << "have trees " << pairs->GetEntries() << endl;

  TFile *outFile = new TFile(outName, "recreate");

  TH1F* hPtEff = DiffEffPlot( pairs, outFile, "pt", 9., 10., 100.);
  TH1F* hEtaEff = DiffEffPlot( pairs, outFile, "eta", 48, -2.4, 2.4);
  TH1F* hPhiEff = DiffEffPlot( pairs, outFile, "phi", 64, -3.2, 3.2);

  // std::cout << "Efficiency is " << eff << endl;
  std::cout << "done" << endl;
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
