#ifndef MITEXAMPLE_MODS_NTUPLESMOD_H
#define MITEXAMPLE_MODS_NTUPLESMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/ElectronCol.h"
#include "MitAna/DataTree/interface/TriggerObjectCol.h"
#include "MitAna/DataTree/interface/VertexFwd.h"

#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "TTree.h"
#include "TString.h"

namespace mithep {
  
  class NtuplesMod : public BaseMod {
  public:
    NtuplesMod(char const* name = "NtuplesMod", char const* title = "Flat-tree ntuples producer");
    void SetTagElectronsName(char const* _name) { fTagElectronsName = _name; }
    void SetPassElectronsName(char const* _name) { fPassElectronsName = _name; }
    void SetProbeElectronsName(char const* _name) { fProbeElectronsName = _name; }
    void SetTriggerObjectsName(char const* _name) { fTriggerObjectsName = _name; }
    void SetTriggerMatchName(char const* _name) { fTriggerMatchName = _name; }
    void SetPVName(char const* _name) { fPVName = _name; }

  protected:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    TString fTagElectronsName;
    TString fPassElectronsName;
    TString fProbeElectronsName;
    TString fTriggerObjectsName;
    TString fTriggerMatchName;
    TString fPVName;

    ElectronCol const* fTagElectrons;
    ElectronCol const* fPassElectrons;
    ElectronCol const* fProbeElectrons;
    TriggerObjectCol const* fTriggerObjects;
    VertexCol const* fVertices;
    
    TnPEvent fEvent;
    TTree* fNtuples;

    ClassDef(NtuplesMod, 0)
  };

}

#endif
