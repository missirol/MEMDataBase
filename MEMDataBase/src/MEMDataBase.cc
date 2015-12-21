#include "MEMDataBase/MEMDataBase/interface/MEMDataBase.h"

int MEMDataBase::GetValue(){
  return 10.0;
}

MEMDataBase::MEMDataBase(){
  dataBaseDirectory="/nfs/dust/cms/user/kelmorab/MEDataBase";
  
}

MEMDataBase::MEMDataBase(const TString dataBaseDirectory_){
  dataBaseDirectory=dataBaseDirectory_;
}


MEMDataBase::~MEMDataBase(){
}

void MEMDataBase::AddSample(const TString sampleName_){
 sampleNames.push_back(sampleName_);
 sampleDataBases.push_back(new DataBaseSample(sampleName_, dataBaseDirectory, "ttbar_index.txt"));
  
}

void MEMDataBase::PrintStructure(){
  for(unsigned int ib=0; ib<sampleDataBases.size();ib++){
    sampleDataBases.at(ib)->PrintStructure();
  }
    
}

DataBaseMEMResult MEMDataBase::GetMEMResult(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber){
  for(unsigned int isample=0; isample<sampleNames.size();isample++){
    if(sampleNames.at(isample)==sample){
      return sampleDataBases.at(isample)->GetMEMResult(runNumber,lumiSection,eventNumber);
    }
  }
  std::cout<<"did not find sample "<<sample<<std::endl;
  DataBaseMEMResult dummyResult;
  return dummyResult;
}
