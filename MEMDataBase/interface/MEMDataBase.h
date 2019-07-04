#ifndef MEMDATABASE_H
#define MEMDATABASE_H

#include <vector>

#include <TFile.h>
#include <TString.h>

#include "MEMDataBase/MEMDataBase/interface/DataBaseSample.h"
#include "MEMDataBase/MEMDataBase/interface/DataBaseMEMResult.h"

class MEMDataBase {

 public:

  MEMDataBase();
  MEMDataBase(const TString dataBaseDirectory_);
  ~MEMDataBase();

  int GetValue();

  void AddSample(const TString sampleName_, const TString indexfilename_="");
  void AddSample(const TString sampleName, const TString indexfilename, const TString sampleAlias);

  void PrintStructure();

  DataBaseMEMResult GetMEMResult(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber);

  bool AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_RelativeSampleUp, Double_t p_RelativeSampleDown, Double_t p_JERUp, Double_t p_JERDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg);

  void SaveDataBase();

  const std::vector<TString>& samples() const { return sampleNames; }

 private:
  TString dataBaseDirectory;
  std::vector<TString> sampleNames;
  std::vector<DataBaseSample*> sampleDataBases;
};

#endif
