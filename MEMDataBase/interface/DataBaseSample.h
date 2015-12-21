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

class DataBaseSample{

public:
  DataBaseSample();
  DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_, const TString indexFile_="");
  ~DataBaseSample();
  
  void AddRunLumiEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
  
private:

  std::vector<RunCollection> runCollection;
  
  std::vector<TString> FileNames;
  
  TString currentOpenFileName;
  TFile* currentOpenTreeFile;
  TTree* currentOpenTree;
  Int_t currentTreeEntries;
  
  static const int maxEventsPerTree=10000;
  
  TString GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
  bool OpenTree(TString filename);
  
  TString dataBaseDirectory;
  TString indexFile;
  TString sampleName;
//   std::vector<TString> sampleNames;
  
//   Long64_t lastRun;

  Int_t brRun;
  Int_t brLumi;
  Int_t brEvent;
  
  Float_t br_p;
  Float_t br_p_sig;
  Float_t br_p_bkg;
  Float_t br_p_err_sig;
  Float_t br_p_err_bkg;
  Float_t br_n_perm_sig;
  Float_t br_n_perm_bkg;

  std::map<Int_t, Long64_t> lookUpMap;
  
      
};



#endif