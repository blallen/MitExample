import ROOT
ROOT.gROOT.SetBatch(True)

ROOT.gSystem.Load('libMitAnaDataTree.so')
ROOT.gSystem.Load('libMitPhysicsMods.so')
ROOT.gSystem.Load('libMitExampleMods.so')

mithep = ROOT.mithep

analysis = mithep.Analysis()
analysis.SetOutputName('ntuples_noid.root')

analysis.AddFile('/mnt/hadoop/cms/store/user/paus/filefi/032/SingleElectron+Run2012A-22Jan2013-v1+AOD/3EE27797-3773-E211-B2A6-00266CF27430.root')
analysis.SetProcessNEvents(10000)

hltMod = mithep.HLTMod()
hltMod.SetBitsName('HLTBits')
hltMod.SetTrigObjsName('SingleElectronTriggerObjects')
hltMod.AddTrigger('HLT_Ele27_WP80_v*')

goodPVMod = mithep.GoodPVFilterMod()
goodPVMod.SetMinVertexNTracks(0)
goodPVMod.SetMinNDof(4)
goodPVMod.SetMaxAbsZ(24.0)
goodPVMod.SetMaxRho(2.0)
goodPVMod.SetIsMC(False)
goodPVMod.SetVertexesName('PrimaryVertexes')

# EGamma tag and probe
'''eleIdMod = mithep.ElectronIDMod()
eleIdMod.SetPtMin(30.)
eleIdMod.SetEtaMax(2.5)
eleIdMod.SetApplyEcalFiducial(True)
eleIdMod.SetIDType('CustomTight')
eleIdMod.SetIsoType('PFIso')
eleIdMod.SetApplyConversionFilterType1(False)
eleIdMod.SetApplyConversionFilterType2(False)
eleIdMod.SetChargeFilter(False)
eleIdMod.SetApplyD0Cut(True)
eleIdMod.SetApplyDZCut(True)
eleIdMod.SetWhichVertex(0)
eleIdMod.SetNExpectedHitsInnerCut(2)
eleIdMod.SetElectronsFromBranch(True)
eleIdMod.SetInputName('Electrons')
eleIdMod.SetGoodElectronsName('TightElectrons')
eleIdMod.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)

phoIdMod = mithep.PhotonIDMod()
phoIdMod.SetPtMin(10.0)
phoIdMod.SetOutputName('MediumPhotonsNoEVeto')
phoIdMod.SetIDType('EgammaMedium')
phoIdMod.SetIsoType('MITPUCorrected')
phoIdMod.SetApplyElectronVeto(False)
phoIdMod.SetApplyPixelSeed(False)
phoIdMod.SetApplyConversionId(False)
phoIdMod.SetApplyFiduciality(True)
phoIdMod.SetIsData(True)
phoIdMod.SetPhotonsFromBranch(True)'''

# e+ e- tag and probe
eleIdMod_Tag = mithep.ElectronIDMod()
eleIdMod_Tag.SetPtMin(30.)
eleIdMod_Tag.SetEtaMax(2.5)
eleIdMod_Tag.SetApplyEcalFiducial(True)
eleIdMod_Tag.SetIDType('CustomTight')
eleIdMod_Tag.SetIsoType('PFIso')
eleIdMod_Tag.SetApplyConversionFilterType1(False)
eleIdMod_Tag.SetApplyConversionFilterType2(False)
eleIdMod_Tag.SetChargeFilter(False)
eleIdMod_Tag.SetApplyD0Cut(True)
eleIdMod_Tag.SetApplyDZCut(True)
eleIdMod_Tag.SetWhichVertex(0)
eleIdMod_Tag.SetNExpectedHitsInnerCut(2)
eleIdMod_Tag.SetElectronsFromBranch(True)
eleIdMod_Tag.SetInputName('Electrons')
eleIdMod_Tag.SetGoodElectronsName('TightElectrons')
eleIdMod_Tag.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)

eleIdMod_Probe = mithep.ElectronIDMod()
eleIdMod_Probe.SetPtMin(10.)
eleIdMod_Probe.SetEtaMax(2.5)
eleIdMod_Probe.SetApplyEcalFiducial(True)
#eleIdMod_Probe.SetIDType('CustomLoose')
eleIdMod_Probe.SetIDType('NoId')
eleIdMod_Probe.SetIsoType('PFIso')
eleIdMod_Probe.SetApplyConversionFilterType1(False)
eleIdMod_Probe.SetApplyConversionFilterType2(False)
eleIdMod_Probe.SetChargeFilter(False)
eleIdMod_Probe.SetApplyD0Cut(True)
eleIdMod_Probe.SetApplyDZCut(True)
eleIdMod_Probe.SetWhichVertex(0)
eleIdMod_Probe.SetNExpectedHitsInnerCut(2)
eleIdMod_Probe.SetElectronsFromBranch(True)
eleIdMod_Probe.SetInputName('Electrons')
eleIdMod_Probe.SetGoodElectronsName('LooseElectrons')
eleIdMod_Probe.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)

ntuplesMod = mithep.NtuplesMod('NtuplesMod', 'Flat ntuples producer')
ntuplesMod.SetTagElectronsName('TightElectrons')
#ntuplesMod.SetProbePhotonsName('MediumPhotonsNoEVeto')
ntuplesMod.SetProbeElectronsName('LooseElectrons')
ntuplesMod.SetTriggerObjectsName('SingleElectronTriggerObjects')
ntuplesMod.SetTriggerMatchName('hltEle27WP80TrackIsoFilter')

### Add mods for analysis ###

analysis.AddSuperModule(hltMod)
hltMod.Add(goodPVMod)

# Egamma tag and probe
'''goodPVMod.Add(eleIdMod)
eleIdMod.Add(phoIdMod)
phoIdMod.Add(ntuplesMod)'''

# e+ e- tag and probe
goodPVMod.Add(eleIdMod_Tag)
eleIdMod_Tag.Add(eleIdMod_Probe)
eleIdMod_Probe.Add(ntuplesMod)

analysis.Run(False)
