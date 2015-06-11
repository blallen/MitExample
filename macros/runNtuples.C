#include <TSystem.h>
#include <TProfile.h>
#include "MitCommon/Utils/interface/Utils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/PhysicsMod/interface/RunLumiSelectionMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitPhysics/Mods/interface/GoodPVFilterMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"

#include "MitExample/Mods/interface/NtuplesMod.h"

TString getCatalogDir(const char* dir);
TString getJsonFile(const char* dir);

//--------------------------------------------------------------------------------------------------
void runNtuples(const char *fileset    = "0000",
		       const char *skim       = "noskim",
		       const char *dataset    = "r12a-smu-j22-v1",
		       const char *book       = "t2mit/filefi/032",
		       const char *catalogDir = "/home/cmsprod/catalog",
		       const char *outputName = "MonoJet",
		       int         nEvents    = 100)
{
  //------------------------------------------------------------------------------------------------
  // some parameters get passed through the environment
  //------------------------------------------------------------------------------------------------
  TString cataDir  = getCatalogDir(catalogDir);
  TString mitData  = Utils::GetEnv("MIT_DATA");
  TString json     = Utils::GetEnv("MIT_PROD_JSON");
  TString jsonFile = getJsonFile("/home/cmsprod/cms/json");
  Bool_t  isData   = (json.CompareTo("~") != 0);
  printf("\n Initialization worked. Data?: %d\n\n",isData);  

  std::cout<<"*********** Is data?? **********"<<isData<<std::endl;

  //------------------------------------------------------------------------------------------------
  // some global setups
  //------------------------------------------------------------------------------------------------
  using namespace mithep;
  gDebugMask  = (Debug::EDebugMask) (Debug::kGeneral | Debug::kTreeIO);
  gDebugLevel = 3;

  // Caching and how
  Int_t local = 1, cacher = 1;

  // local =   0 - as is,
  //           1 - /mnt/hadoop (MIT:SmartCache - preload one-by-one)
  //           2 - /mnt/hadoop (MIT:SmartCache - preload complete fileset)
  //           3 - ./          (xrdcp          - preload one-by-one)
  // cacher =  0 - no file by file caching
  //           1 - file by file caching on

  //------------------------------------------------------------------------------------------------
  // set up information
  //------------------------------------------------------------------------------------------------
  RunLumiSelectionMod *runLumiSel = new RunLumiSelectionMod;
  runLumiSel->SetAcceptMC(!isData);
  runLumiSel->SetAbortIfNotAccepted(kFALSE);   // accept all events if there is no valid JSON file

  // only select on run- and lumisection numbers when valid json file present
  if (json.CompareTo("~") != 0 && json.CompareTo("-") != 0) {
    printf(" runBoostedV() - adding jsonFile: %s\n",jsonFile.Data());
    runLumiSel->AddJSONFile(jsonFile.Data());
  }
  if (json.CompareTo("-") == 0) {
    printf("\n WARNING -- Looking at data without JSON file: always accept.\n\n");
    runLumiSel->SetAbortIfNotAccepted(kFALSE);   // accept all events if there is no valid JSON file
  }
  printf("\n Run lumi worked. \n\n");

  //------------------------------------------------------------------------------------------------
  // HLT info: trigger not applied (neither data nor MC, store info to apply selection offline)
  //------------------------------------------------------------------------------------------------
  HLTMod *hltMod = new HLTMod("HltMod");
  hltMod->AddTrigger("HLT_Ele27_WP80_v*");
  hltMod->SetBitsName("HLTBits");
  hltMod->SetTrigObjsName("MyHltObjs");
  hltMod->SetAbortIfNotAccepted(isData);
  hltMod->SetPrintTable(kFALSE);

  //------------------------------------------------------------------------------------------------
  // select events with a good primary vertex
  //------------------------------------------------------------------------------------------------
  GoodPVFilterMod *goodPvFilterMod = new GoodPVFilterMod;
  goodPvFilterMod->SetMinVertexNTracks(0);
  goodPvFilterMod->SetMinNDof         (4.0);
  goodPvFilterMod->SetMaxAbsZ         (24.0);
  goodPvFilterMod->SetMaxRho          (2.0);
  goodPvFilterMod->SetIsMC(!isData);
  goodPvFilterMod->SetVertexesName("PrimaryVertexes");

  //------------------------------------------------------------------------------------------------
  // object id and cleaning sequence
  //------------------------------------------------------------------------------------------------
  //-----------------------------------
  // Lepton Selection
  //-----------------------------------

  // e+ e- tag and probe
  ElectronIDMod *eleIdModTag = new ElectronIDMod();
  eleIdModTag->SetPtMin(30.);
  eleIdModTag->SetEtaMax(2.5);
  eleIdModTag->SetApplyEcalFiducial(kTRUE);
  eleIdModTag->SetIDType("CustomTight");
  eleIdModTag->SetIsoType("PFIso");
  eleIdModTag->SetApplyConversionFilterType1(kFALSE);
  eleIdModTag->SetApplyConversionFilterType2(kFALSE);
  eleIdModTag->SetChargeFilter(kFALSE);
  eleIdModTag->SetApplyD0Cut(kTRUE);
  eleIdModTag->SetApplyDZCut(kTRUE);
  eleIdModTag->SetWhichVertex(0);
  eleIdModTag->SetNExpectedHitsInnerCut(2);
  eleIdModTag->SetElectronsFromBranch(kTRUE);
  eleIdModTag->SetInputName("Electrons");
  eleIdModTag->SetGoodElectronsName("TightElectrons");
  eleIdModTag->SetRhoType(RhoUtilities::CMS_RHO_RHOKT6PFJETS);

  ElectronIDMod *eleIdModProbe = new ElectronIDMod();
  eleIdModProbe->SetPtMin(10.);
  eleIdModProbe->SetEtaMax(2.5);
  eleIdModProbe->SetApplyEcalFiducial(kTRUE);
  eleIdModProbe->SetIDType("NoId");
  eleIdModProbe->SetIsoType("PFIso");
  eleIdModProbe->SetApplyConversionFilterType1(kFALSE);
  eleIdModProbe->SetApplyConversionFilterType2(kFALSE);
  eleIdModProbe->SetChargeFilter(kFALSE);
  eleIdModProbe->SetApplyD0Cut(kTRUE);
  eleIdModProbe->SetApplyDZCut(kTRUE);
  eleIdModProbe->SetWhichVertex(0);
  eleIdModProbe->SetNExpectedHitsInnerCut(2);
  eleIdModProbe->SetElectronsFromBranch(kTRUE);
  eleIdModProbe->SetInputName("Electrons");
  eleIdModProbe->SetGoodElectronsName("NoIdElectrons");
  eleIdModProbe->SetRhoType(RhoUtilities::CMS_RHO_RHOKT6PFJETS);
  
  ElectronIDMod *eleIdModPass = new ElectronIDMod();
  eleIdModPass->SetPtMin(10.);
  eleIdModPass->SetEtaMax(2.5);
  eleIdModPass->SetApplyEcalFiducial(kTRUE);
  eleIdModPass->SetIDType("CustomLoose");
  eleIdModPass->SetIsoType("PFIso");
  eleIdModPass->SetApplyConversionFilterType1(kFALSE);
  eleIdModPass->SetApplyConversionFilterType2(kFALSE);
  eleIdModPass->SetChargeFilter(kFALSE);
  eleIdModPass->SetApplyD0Cut(kTRUE);
  eleIdModPass->SetApplyDZCut(kTRUE);
  eleIdModPass->SetWhichVertex(0);
  eleIdModPass->SetNExpectedHitsInnerCut(2);
  eleIdModPass->SetElectronsFromBranch(kTRUE);
  eleIdModPass->SetInputName("Electrons");
  eleIdModPass->SetGoodElectronsName("LooseElectrons");
  eleIdModPass->SetRhoType(RhoUtilities::CMS_RHO_RHOKT6PFJETS);

  NtuplesMod *ntuplesMod = new NtuplesMod;
  ntuplesMod->SetTagElectronsName("TightElectrons");
  ntuplesMod->SetPassElectronsName("LooseElectrons");
  ntuplesMod->SetProbeElectronsName("NoIdElectrons");
  ntuplesMod->SetTriggerObjectsName("MyHltObjs");
  ntuplesMod->SetTriggerMatchName("hltEle27WP80TrackIsoFilter");

  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  // this is how it always starts
  runLumiSel       ->Add(goodPvFilterMod);
  goodPvFilterMod  ->Add(hltMod);
  // electrons
  hltMod           ->Add(eleIdModTag);
  eleIdModTag      ->Add(eleIdModProbe);
  eleIdModProbe    ->Add(eleIdModPass);
  // ntuplize
  eleIdModPass     ->Add(ntuplesMod);

  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseCacher(cacher);
  //ana->SetUseHLT(kTRUE);
  //ana->SetKeepHierarchy(kTRUE);
  ana->SetSuperModule(runLumiSel);
  ana->SetPrintScale(100);
  if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);

  //------------------------------------------------------------------------------------------------
  // organize input
  //------------------------------------------------------------------------------------------------
  TString skimdataset = TString(dataset)+TString("/") +TString(skim);
  TString bookstr = book;
  Catalog *c = new Catalog(cataDir.Data());
  Dataset *d = NULL;
  if (TString(skim).CompareTo("noskim") == 0)
    d = c->FindDataset(bookstr,dataset,fileset,local);
  else
    d = c->FindDataset(bookstr,skimdataset.Data(),fileset,local);
  ana->AddDataset(d);  //
  //ana->AddFile("/mnt/hadoop/cms/store/user/paus/filefi/032/SingleElectron+Run2012A-22Jan2013-v1+AOD/4EA4EB35-5373-E211-B60F-0030487D5DB5.root");

  //------------------------------------------------------------------------------------------------
  // organize output
  //------------------------------------------------------------------------------------------------
  TString rootFile = TString(outputName);
  rootFile += TString("_") + TString(dataset) + TString("_") + TString(skim);
  if (TString(fileset) != TString(""))
    rootFile += TString("_") + TString(fileset);
  rootFile += TString(".root");
  ana->SetOutputName(rootFile.Data());
  ana->SetCacheSize(0);

  //------------------------------------------------------------------------------------------------
  // Say what we are doing
  //------------------------------------------------------------------------------------------------
  printf("\n==== PARAMETER SUMMARY FOR THIS JOB ====\n");
  printf("\n JSON file: %s\n",jsonFile.Data());
  printf("\n Rely on Catalog: %s\n",cataDir.Data());
  printf("  -> Book: %s  Dataset: %s  Skim: %s  Fileset: %s <-\n",book,dataset,skim,fileset);
  printf("\n Root output: %s\n\n",rootFile.Data());
  printf("\n========================================\n");

  //------------------------------------------------------------------------------------------------
  // run the analysis after successful initialisation
  //------------------------------------------------------------------------------------------------
  ana->Run(kFALSE);

  return;
}

//--------------------------------------------------------------------------------------------------
TString getCatalogDir(const char* dir)
{
  TString cataDir = TString("./catalog");
  Long_t *id=0,*size=0,*flags=0,*mt=0;

  printf(" Try local catalog first: %s\n",cataDir.Data());
  if (gSystem->GetPathInfo(cataDir.Data(),id,size,flags,mt) != 0) {
    cataDir = TString(dir);
    if (gSystem->GetPathInfo(cataDir.Data(),id,size,flags,mt) != 0) {
      printf(" Requested local (./catalog) and specified catalog do not exist. EXIT!\n");
      return TString("");
    }
  }
  else {
    printf(" Local catalog exists: %s using this one.\n",cataDir.Data());
  }

  return cataDir;
}

//--------------------------------------------------------------------------------------------------
TString getJsonFile(const char* dir)
{
  TString jsonDir  = TString("./json");
  TString json     = Utils::GetEnv("MIT_PROD_JSON");
  Long_t *id=0,*size=0,*flags=0,*mt=0;

  printf(" Try local json first: %s\n",jsonDir.Data());
  if (gSystem->GetPathInfo(jsonDir.Data(),id,size,flags,mt) != 0) {
    jsonDir = TString(dir);
    if (gSystem->GetPathInfo(jsonDir.Data(),id,size,flags,mt) != 0) {
      printf(" Requested local (./json) and specified json directory do not exist. EXIT!\n");
      return TString("");
    }
  }
  else
    printf(" Local json directory exists: %s using this one.\n",jsonDir.Data());

  // Construct the full file name
  TString jsonFile = jsonDir + TString("/") + json;
  if (gSystem->GetPathInfo(jsonFile.Data(),id,size,flags,mt) != 0) {
    printf(" Requested jsonfile (%s) does not exist. EXIT!\n",jsonFile.Data());
    return TString("");
  }
  else
    printf(" Requested jsonfile (%s) exist. Moving on now!\n",jsonFile.Data());

  return jsonFile;
}
