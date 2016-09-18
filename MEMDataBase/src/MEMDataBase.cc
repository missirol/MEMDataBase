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

void MEMDataBase::SaveDataBase(){
  std::cout<<"saving database"<<std::endl;
  for(unsigned int i=0; i<sampleDataBases.size(); i++){
    sampleDataBases.at(i)->SaveDataBase();
  }
    
}
  

MEMDataBase::~MEMDataBase(){
  for(unsigned int i=0; i<sampleDataBases.size(); i++){
//     std::cout<<sampleDataBases.at(i)<<std::endl;
    delete sampleDataBases.at(i);
//     std::cout<<i<<std::endl;
  }
    
}

void MEMDataBase::AddSample(const TString sampleName_, const TString indexfilename_){
 sampleNames.push_back(sampleName_);
//  TString indexfilename=sampleName_+"_index.txt";
 sampleDataBases.push_back(new DataBaseSample(sampleName_, dataBaseDirectory, indexfilename_));
  
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

bool MEMDataBase::AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Float_t Weight_muRupmuFup, Float_t Weight_muRdownmuFdown, Float_t Weight_NNPDFid260067, Float_t Weight_NNPDFid260005){
  for(unsigned int isample=0; isample<sampleNames.size();isample++){
    if(sampleNames.at(isample)==sample){
      sampleDataBases.at(isample)->AddEvent(runNumber,lumiSection,eventNumber, Weight_muRupmuFup, Weight_muRdownmuFdown, Weight_NNPDFid260067, Weight_NNPDFid260005);
    }
  }
  
  return true;
}