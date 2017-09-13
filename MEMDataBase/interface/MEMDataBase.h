#ifndef MEMDATABASE_H
#define MEMDATABASE_H

#include "TFile.h"
#include "TString.h"
#include "MEMDataBase/MEMDataBase/interface/DataBaseSample.h"
#include "MEMDataBase/MEMDataBase/interface/DataBaseMEMResult.h"


class MEMDataBase{

public:
  int GetValue();
  MEMDataBase();
  MEMDataBase(const TString dataBaseDirectory_);
  MEMDataBase(const TString dataBaseDirectory_, const std::vector<TString> mem_strings_);
  ~MEMDataBase();

  void AddSample(const TString sampleName_, const TString indexfilename_="");
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber);
  bool AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg);
  bool AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, std::vector<Double_t> p_vec, Double_t p_sig, Double_t p_bkg, Double_t blr_eth, Double_t blr_eth_transformed);
  
  void SaveDataBase();
private:
  TString dataBaseDirectory;
  std::vector<TString> sampleNames;
  std::vector<DataBaseSample*> sampleDataBases;
  std::vector<TString> mem_strings;
  
};

#endif