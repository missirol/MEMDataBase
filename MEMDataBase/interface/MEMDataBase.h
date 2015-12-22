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
  ~MEMDataBase();

  void AddSample(const TString sampleName_, const TString indexfilename_="");
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber);
  bool AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Float_t p, Float_t p_sig, Float_t p_bkg, Float_t p_err_sig, Float_t p_err_bkg, Float_t n_perm_sig, Float_t n_perm_bkg);
  
  void SaveDataBase();
private:
  TString dataBaseDirectory;
  std::vector<TString> sampleNames;
  std::vector<DataBaseSample*> sampleDataBases;
  
};

#endif