// AUTO-GENERATED CODE - EDIT Tag.h / Probe.h and run scripts/updateClasses.py

#include "MitExample/DataFormats/interface/Probe.h"
#include "MitExample/DataFormats/interface/TnPEvent.h"

mithep::Probe::Probe(TnPEvent& _evt, UInt_t _idx) :
  charge(_evt.probeCharge[_idx]),
  pt(_evt.probePt[_idx]),
  eta(_evt.probeEta[_idx]),
  phi(_evt.probePhi[_idx]),
  px(_evt.probePx[_idx]),
  py(_evt.probePy[_idx]),
  pz(_evt.probePz[_idx]),
  energy(_evt.probeEnergy[_idx])
{
}
