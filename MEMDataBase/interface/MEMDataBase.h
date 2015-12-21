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

  void AddSample(const TString sampleName_);
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber);
  
private:
  TString dataBaseDirectory;
  std::vector<TString> sampleNames;
  std::vector<DataBaseSample*> sampleDataBases;
  
  
};

#endif