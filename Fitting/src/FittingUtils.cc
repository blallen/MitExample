#include "MitExample/Fitting/interface/FittingUtils.h"
#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "TTree.h"
#include "TLorentzVector.h"

RooDataSet*
mithep::FittingUtils::createDataSet(TTree* _source, RooArgSet* _argset, char const* _name/* = "dataset"*/, char const* _title/* = "T&P dataset"*/)
{
  mithep::TnPEvent event;
  event.setAddress(*_source);

  // obtain variables of interest from the arg set
  RooRealVar* mass_noID(dynamic_cast<RooRealVar*>(_argset->find("mass_noID")));
  if (!mass_noID)
    return 0;
  RooRealVar* mass_passID(dynamic_cast<RooRealVar*>(_argset->find("mass_passID")));
  if (!mass_passID)
    return 0;

  RooDataSet* dataset(new RooDataSet(_name, _title, *_argset));

  long iEntry(0);
  while (_source->GetEntry(iEntry++) > 0) {
    for (unsigned iP(0); iP != event.nPairs; ++iP) {
      auto&& pair(event.at(iP));
      mithep::Tag& tag(pair.first);
      mithep::Probe& probe(pair.second);

      // Apply cuts

      if ( tag.charge == probe.charge) continue;
      
      TLorentzVector pTag(tag.px, tag.py, tag.pz, tag.energy);
      TLorentzVector pProbe(probe.px, probe.py, probe.pz, probe.energy);

      // Set variable values

      mass_noID->setVal((pTag + pProbe).M());
      if (probe.passID) mass_passID->setVal((pTag + pProbe).M());
      dataset->add(*_argset);
    }
  }

  return dataset;
}
