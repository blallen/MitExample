// AUTO-GENERATED CODE - EDIT Tag.h / Probe.h and run scripts/updateClasses.py

#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "TTree.h"
#include "TString.h"

#include <stdexcept>
#include <vector>
#include <iostream>

mithep::TnPEvent::TnPEvent() :
  nPairs(0)
{
  for (unsigned iP(0); iP != NMAX; ++iP) {
    tag_[iP] = new Tag(*this, iP);
    probe_[iP] = new Probe(*this, iP);
  }
}

mithep::TnPEvent::~TnPEvent()
{
  for (unsigned iP(0); iP != NMAX; ++iP) {
    delete tag_[iP];
    delete probe_[iP];
  }
}

mithep::TnPEvent::TnPPair
mithep::TnPEvent::at(UInt_t _idx) const
{
  if (_idx < nPairs)
    return TnPPair(*tag_[_idx], *probe_[_idx]);
  else
    throw std::out_of_range("TnPEvent::at()");
}

mithep::TnPEvent::TnPPair
mithep::TnPEvent::addNew()
{
  TnPPair pair(*tag_[nPairs], *probe_[nPairs]);
  ++nPairs;

  return pair;
}

void
mithep::TnPEvent::bookBranches(TTree& _tree)
{
  _tree.Branch("nPairs", &nPairs, "nPairs/i");

  _tree.Branch("tag.charge", tagCharge, "charge[nPairs]/F");
  _tree.Branch("tag.pt", tagPt, "pt[nPairs]/F");
  _tree.Branch("tag.eta", tagEta, "eta[nPairs]/F");
  _tree.Branch("tag.phi", tagPhi, "phi[nPairs]/F");
  _tree.Branch("tag.px", tagPx, "px[nPairs]/F");
  _tree.Branch("tag.py", tagPy, "py[nPairs]/F");
  _tree.Branch("tag.pz", tagPz, "pz[nPairs]/F");
  _tree.Branch("tag.energy", tagEnergy, "energy[nPairs]/F");
  _tree.Branch("probe.charge", probeCharge, "charge[nPairs]/F");
  _tree.Branch("probe.pt", probePt, "pt[nPairs]/F");
  _tree.Branch("probe.eta", probeEta, "eta[nPairs]/F");
  _tree.Branch("probe.phi", probePhi, "phi[nPairs]/F");
  _tree.Branch("probe.px", probePx, "px[nPairs]/F");
  _tree.Branch("probe.py", probePy, "py[nPairs]/F");
  _tree.Branch("probe.pz", probePz, "pz[nPairs]/F");
  _tree.Branch("probe.energy", probeEnergy, "energy[nPairs]/F");
  _tree.Branch("probe.passID", probePassid, "passID[nPairs]/i");
}

void
mithep::TnPEvent::setAddress(TTree& _tree)
{
  _tree.SetBranchAddress("nPairs", &nPairs);

  std::vector<TString> notFound;

  if (_tree.GetBranchStatus("tag.charge"))
    _tree.SetBranchAddress("tag.charge", tagCharge);
  else if (!_tree.GetBranch("tag.charge"))
    notFound.push_back("tag.charge");
  if (_tree.GetBranchStatus("tag.pt"))
    _tree.SetBranchAddress("tag.pt", tagPt);
  else if (!_tree.GetBranch("tag.pt"))
    notFound.push_back("tag.pt");
  if (_tree.GetBranchStatus("tag.eta"))
    _tree.SetBranchAddress("tag.eta", tagEta);
  else if (!_tree.GetBranch("tag.eta"))
    notFound.push_back("tag.eta");
  if (_tree.GetBranchStatus("tag.phi"))
    _tree.SetBranchAddress("tag.phi", tagPhi);
  else if (!_tree.GetBranch("tag.phi"))
    notFound.push_back("tag.phi");
  if (_tree.GetBranchStatus("tag.px"))
    _tree.SetBranchAddress("tag.px", tagPx);
  else if (!_tree.GetBranch("tag.px"))
    notFound.push_back("tag.px");
  if (_tree.GetBranchStatus("tag.py"))
    _tree.SetBranchAddress("tag.py", tagPy);
  else if (!_tree.GetBranch("tag.py"))
    notFound.push_back("tag.py");
  if (_tree.GetBranchStatus("tag.pz"))
    _tree.SetBranchAddress("tag.pz", tagPz);
  else if (!_tree.GetBranch("tag.pz"))
    notFound.push_back("tag.pz");
  if (_tree.GetBranchStatus("tag.energy"))
    _tree.SetBranchAddress("tag.energy", tagEnergy);
  else if (!_tree.GetBranch("tag.energy"))
    notFound.push_back("tag.energy");
  if (_tree.GetBranchStatus("probe.charge"))
    _tree.SetBranchAddress("probe.charge", probeCharge);
  else if (!_tree.GetBranch("probe.charge"))
    notFound.push_back("probe.charge");
  if (_tree.GetBranchStatus("probe.pt"))
    _tree.SetBranchAddress("probe.pt", probePt);
  else if (!_tree.GetBranch("probe.pt"))
    notFound.push_back("probe.pt");
  if (_tree.GetBranchStatus("probe.eta"))
    _tree.SetBranchAddress("probe.eta", probeEta);
  else if (!_tree.GetBranch("probe.eta"))
    notFound.push_back("probe.eta");
  if (_tree.GetBranchStatus("probe.phi"))
    _tree.SetBranchAddress("probe.phi", probePhi);
  else if (!_tree.GetBranch("probe.phi"))
    notFound.push_back("probe.phi");
  if (_tree.GetBranchStatus("probe.px"))
    _tree.SetBranchAddress("probe.px", probePx);
  else if (!_tree.GetBranch("probe.px"))
    notFound.push_back("probe.px");
  if (_tree.GetBranchStatus("probe.py"))
    _tree.SetBranchAddress("probe.py", probePy);
  else if (!_tree.GetBranch("probe.py"))
    notFound.push_back("probe.py");
  if (_tree.GetBranchStatus("probe.pz"))
    _tree.SetBranchAddress("probe.pz", probePz);
  else if (!_tree.GetBranch("probe.pz"))
    notFound.push_back("probe.pz");
  if (_tree.GetBranchStatus("probe.energy"))
    _tree.SetBranchAddress("probe.energy", probeEnergy);
  else if (!_tree.GetBranch("probe.energy"))
    notFound.push_back("probe.energy");
  if (_tree.GetBranchStatus("probe.passID"))
    _tree.SetBranchAddress("probe.passID", probePassid);
  else if (!_tree.GetBranch("probe.passID"))
    notFound.push_back("probe.passID");

  for (unsigned iN(0); iN != notFound.size(); ++iN)
    std::cerr << "Branch " << notFound[iN] << " not found in input" << std::endl;
}
