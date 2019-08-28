#ifndef DATABASESAMPLE_H
#define DATABASESAMPLE_H

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "MEMDataBase/MEMDataBase/interface/DataBaseMEMResult.h"
#include "MEMDataBase/MEMDataBase/interface/DataBaseRunCollection.h"
#include <map>
#include <cstdio>
#include "TMath.h"

class DataBaseSample{

public:
  DataBaseSample();
  DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_, const TString indexFile_="");
  ~DataBaseSample();
  
  void AddRunLumiEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
  
  bool AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_RelativeSampleUp, Double_t p_RelativeSampleDown, Double_t p_JERUp, Double_t p_JERDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg, Double_t blr_4b, Double_t blr_2b);
  bool SaveDataBase();

  void set_verbose(const bool val=true){ verbose_ = val; }
  bool verbose() const { return verbose_; }

private:

  bool verbose_;

  TFile* currentOpenTreeFile;
  TTree* currentOpenTree;
  std::vector<RunCollection> runCollection;
  
  std::vector<TString> FileNames;
  
  TString currentOpenFileName;

  Int_t currentTreeEntries;
  TString openMode;
  
  static const int maxEventsPerTree=100000;
  static const int LookUpStepSize=1000;
  
  TString GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
//   bool GetRelevantRunAndLumi(const Long64_t lumiSection, const Long64_t eventNumber);
  void AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_RelativeSampleUp, Double_t p_RelativeSampleDown, Double_t p_JERUp, Double_t p_JERDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg, Double_t blr_4b, Double_t blr_2b);

  bool RemoveEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  
  bool OpenTree(TString filename);
  bool CreateNewTree(TString filename);
  bool SplitCollection(TString filename);
  bool CloseTree();

  
  
  TString dataBaseDirectory;
  TString indexFile;
  TString sampleName;
//   std::vector<TString> sampleNames;
  
//   Long64_t lastRun;

  Long64_t brRun;
  Long64_t brLumi;
  Long64_t brEvent;
  
  Double_t br_p;
  Double_t br_p_sig;
  Double_t br_p_bkg;
  Double_t br_p_AbsoluteStatUp;
  Double_t br_p_AbsoluteStatDown;
  Double_t br_p_AbsoluteScaleUp;
  Double_t br_p_AbsoluteScaleDown;
  Double_t br_p_AbsoluteFlavMapUp;
  Double_t br_p_AbsoluteFlavMapDown;
  Double_t br_p_AbsoluteMPFBiasUp;
  Double_t br_p_AbsoluteMPFBiasDown;
  Double_t br_p_FragmentationUp;
  Double_t br_p_FragmentationDown;
  Double_t br_p_SinglePionECALUp;
  Double_t br_p_SinglePionECALDown;
  Double_t br_p_SinglePionHCALUp;
  Double_t br_p_SinglePionHCALDown;
  Double_t br_p_FlavorQCDUp;
  Double_t br_p_FlavorQCDDown;
  Double_t br_p_TimePtEtaUp;
  Double_t br_p_TimePtEtaDown;
  Double_t br_p_RelativeJEREC1Up;
  Double_t br_p_RelativeJEREC1Down;
  Double_t br_p_RelativeJEREC2Up;
  Double_t br_p_RelativeJEREC2Down;
  Double_t br_p_RelativeJERHFUp;
  Double_t br_p_RelativeJERHFDown;
  Double_t br_p_RelativePtBBUp;
  Double_t br_p_RelativePtBBDown;
  Double_t br_p_RelativePtEC1Up;
  Double_t br_p_RelativePtEC1Down;
  Double_t br_p_RelativePtEC2Up;
  Double_t br_p_RelativePtEC2Down;
  Double_t br_p_RelativePtHFUp;
  Double_t br_p_RelativePtHFDown;
  Double_t br_p_RelativeFSRUp;
  Double_t br_p_RelativeFSRDown;
  Double_t br_p_RelativeStatFSRUp;
  Double_t br_p_RelativeStatFSRDown;
  Double_t br_p_RelativeStatECUp;
  Double_t br_p_RelativeStatECDown;
  Double_t br_p_RelativeStatHFUp;
  Double_t br_p_RelativeStatHFDown;
  Double_t br_p_PileUpDataMCUp;
  Double_t br_p_PileUpDataMCDown;
  Double_t br_p_PileUpPtRefUp;
  Double_t br_p_PileUpPtRefDown;
  Double_t br_p_PileUpPtBBUp;
  Double_t br_p_PileUpPtBBDown;
  Double_t br_p_PileUpPtEC1Up;
  Double_t br_p_PileUpPtEC1Down;
  Double_t br_p_PileUpPtEC2Up;
  Double_t br_p_PileUpPtEC2Down;
  Double_t br_p_PileUpPtHFUp;
  Double_t br_p_PileUpPtHFDown;
  Double_t br_p_RelativeBalUp;
  Double_t br_p_RelativeBalDown;
  Double_t br_p_RelativeSampleUp;
  Double_t br_p_RelativeSampleDown;
  Double_t br_p_JERUp;
  Double_t br_p_JERDown;

  Double_t br_p_err_sig;
  Double_t br_p_err_bkg;
  Double_t br_n_perm_sig;
  Double_t br_n_perm_bkg;

  Double_t blr_4b;
  Double_t blr_2b;

  std::map<Int_t, Long64_t> lookUpMap;

  Long64_t inputRun;
  Long64_t relevantRun;
  
  Long64_t inputMinLumi;
  Long64_t inputMaxLumi;
  Long64_t relevantMinLumi;
  Long64_t relevantMaxLumi;
  
  Long64_t inputMaxEvent;
  Long64_t inputMinEvent;
};



#endif
