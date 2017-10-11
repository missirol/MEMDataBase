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

bool MEMDataBase::AddEvent(TString sample, Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg){
  for(unsigned int isample=0; isample<sampleNames.size();isample++){
    if(sampleNames.at(isample)==sample){
      sampleDataBases.at(isample)->AddEvent(runNumber,lumiSection,eventNumber, p, p_sig, p_bkg, p_AbsoluteStatUp, p_AbsoluteStatDown,p_AbsoluteScaleUp,p_AbsoluteScaleDown,p_AbsoluteFlavMapUp,p_AbsoluteFlavMapDown,p_AbsoluteMPFBiasUp,p_AbsoluteMPFBiasDown,p_FragmentationUp,p_FragmentationDown,p_SinglePionECALUp,p_SinglePionECALDown,p_SinglePionHCALUp,p_SinglePionHCALDown,p_FlavorQCDUp,p_FlavorQCDDown,p_TimePtEtaUp,p_TimePtEtaDown,p_RelativeJEREC1Up,p_RelativeJEREC1Down,p_RelativeJEREC2Up,p_RelativeJEREC2Down,p_RelativeJERHFUp,p_RelativeJERHFDown,p_RelativePtBBUp,p_RelativePtBBDown,p_RelativePtEC1Up,p_RelativePtEC1Down,p_RelativePtEC2Up,p_RelativePtEC2Down,p_RelativePtHFUp,p_RelativePtHFDown,p_RelativeFSRUp,p_RelativeFSRDown,p_RelativeStatFSRUp,p_RelativeStatFSRDown,p_RelativeStatECUp,p_RelativeStatECDown,p_RelativeStatHFUp,p_RelativeStatHFDown,p_PileUpDataMCUp,p_PileUpDataMCDown,p_PileUpPtRefUp,p_PileUpPtRefDown,p_PileUpPtBBUp,p_PileUpPtBBDown,p_PileUpPtEC1Up,p_PileUpPtEC1Down,p_PileUpPtEC2Up,p_PileUpPtEC2Down,p_PileUpPtHFUp,p_PileUpPtHFDown,p_RelativeBalUp,p_RelativeBalDown,p_err_sig, p_err_bkg,n_perm_sig,n_perm_bkg);
    }
  }
  
  return true;
}
