#include "MEMDataBase/MEMDataBase/interface/DataBaseSample.h"
#include <stdexcept>

DataBaseSample::DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_, const TString indexFile_){
  
  dataBaseDirectory=dataBaseDirectory_;
  sampleName=sampleName_;
  
  if(indexFile_==""){indexFile=sampleName+TString("_index.txt"); std::cout<<indexFile<<std::endl; }
  else {
    indexFile=indexFile_;
  }
    std::ifstream idfilestream(dataBaseDirectory_+TString("/")+indexFile);
    TString dumpline;
     
//     bool readline=true;
    while(true){
//       std::cout<<"bla"<<std::endl;
      idfilestream>>dumpline;
      if(idfilestream.eof())break;

//       std::cout<<dumpline<<std::endl;
      FileNames.push_back(dumpline);
      TObjArray *obja=dumpline.Tokenize("_");
      Int_t arraylength=obja->GetEntries();
      
      TString thisrun=((TObjString*)(obja->At(arraylength-6)))->String();
      TString thisminlumi=((TObjString*)(obja->At(arraylength-5)))->String();
      TString thismaxlumi=((TObjString*)(obja->At(arraylength-4)))->String();
      TString thisminevent=((TObjString*)(obja->At(arraylength-3)))->String();
      TString thismaxevent=((TObjString*)(obja->At(arraylength-2)))->String();
      
//       std::cout<<thisrun<<" "<<thisminlumi<<" "<<thismaxlumi<<" "<<thisminevent<<" "<<thismaxevent<<std::endl;
      AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
     }
   idfilestream.close();
//   std::cout<<"Index file read: "<<indexFile<<std::endl; 
//   }
  
//   std::cout<<"empty Index file created: "<<indexFile<<std::endl; 
  
  currentOpenFileName="";
  openMode="READ";
}

void DataBaseSample::AddRunLumiEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool elementExists=false;
  for(auto & irun : runCollection){
    if(irun.GetRunNumber()==runNumber_){
      irun.AddLumiAndEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
      elementExists=true;
      break;
    }
  }
  if(!elementExists){
   runCollection.push_back(RunCollection(runNumber_));
   runCollection.back().AddLumiAndEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
  }
}

bool DataBaseSample::RemoveEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool removed=false;
  for(auto & irun : runCollection){
    if(irun.GetRunNumber()==runNumber_){
      removed=irun.RemoveEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
      break;
    }
  }
  return removed;
}

TString DataBaseSample::GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=sampleName+"_";
  
//   RunCollection* thisRunCollection=nullptr;
  
  for(auto & irun : runCollection){
    
    if(irun.GetRunNumber()==runNumber){
      relevantFileName+=TString::LLtoa(runNumber,10)+"_";
      TString substring=irun.GetLumiAndEventIntervalString(lumiSection,eventNumber);
      if(substring!=""){ 
	relevantFileName+=substring;
	relevantFileName+="_.root";
      }
      else{
	relevantFileName="";
      }
      
      break;    
    }
  }
  
//   std::cout<<" Event with "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<" is in "<<relevantFileName<<std::endl;
  return relevantFileName;
  
}

DataBaseMEMResult DataBaseSample::GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
  
  DataBaseMEMResult thisMEM;

  if(relevantFileName=="" || relevantFileName==sampleName+"_"){
//     std::cout<<"did not find event in database"<<std::endl;
    return thisMEM;
  }
  
  if(relevantFileName!=currentOpenFileName){OpenTree(relevantFileName);}
  
  
  //getStart ievt
  
  Int_t startEvt=0;
  for(auto const & it : lookUpMap){
//     std::cout<<it.second<<" "<<it.first<<std::endl;
    if(it.second>=eventNumber){
      break;
    }
    else{ startEvt=it.first;}
  }
  
//   std::cout<<"starting at "<<startEvt<<std::endl;
  
  bool found=false;
  for(Int_t ievt=startEvt; ievt<startEvt+LookUpStepSize+10; ievt++){
    currentOpenTree->GetEntry(ievt);
    if(brEvent==eventNumber and brLumi==lumiSection and brRun==runNumber){
      found=true;
      break;
    }
  }
  
  if(found){
    thisMEM.p=br_p;
    thisMEM.p_sig=br_p_sig;
    thisMEM.p_bkg=br_p_bkg;
    thisMEM.p_AbsoluteStatUp=br_p_AbsoluteStatUp;
    thisMEM.p_AbsoluteStatDown=br_p_AbsoluteStatDown;
    thisMEM.p_AbsoluteScaleUp=br_p_AbsoluteScaleUp;
    thisMEM.p_AbsoluteScaleDown=br_p_AbsoluteScaleDown;
    thisMEM.p_AbsoluteFlavMapUp=br_p_AbsoluteFlavMapUp;
    thisMEM.p_AbsoluteFlavMapDown=br_p_AbsoluteFlavMapDown;
    thisMEM.p_AbsoluteMPFBiasUp=br_p_AbsoluteMPFBiasUp;
    thisMEM.p_AbsoluteMPFBiasDown=br_p_AbsoluteMPFBiasDown;
    thisMEM.p_FragmentationUp=br_p_FragmentationUp;
    thisMEM.p_FragmentationDown=br_p_FragmentationDown;
    thisMEM.p_SinglePionECALUp=br_p_SinglePionECALUp;
    thisMEM.p_SinglePionECALDown=br_p_SinglePionECALDown;
    thisMEM.p_SinglePionHCALUp=br_p_SinglePionHCALUp;
    thisMEM.p_SinglePionHCALDown=br_p_SinglePionHCALDown;
    thisMEM.p_FlavorQCDUp=br_p_FlavorQCDUp;
    thisMEM.p_FlavorQCDDown=br_p_FlavorQCDDown;
    thisMEM.p_TimePtEtaUp=br_p_TimePtEtaUp;
    thisMEM.p_TimePtEtaDown=br_p_TimePtEtaDown;
    thisMEM.p_RelativeJEREC1Up=br_p_RelativeJEREC1Up;
    thisMEM.p_RelativeJEREC1Down=br_p_RelativeJEREC1Down;
    thisMEM.p_RelativeJEREC2Up=br_p_RelativeJEREC2Up;
    thisMEM.p_RelativeJEREC2Down=br_p_RelativeJEREC2Down;
    thisMEM.p_RelativeJERHFUp=br_p_RelativeJERHFUp;
    thisMEM.p_RelativeJERHFDown=br_p_RelativeJERHFDown;
    thisMEM.p_RelativePtBBUp=br_p_RelativePtBBUp;
    thisMEM.p_RelativePtBBDown=br_p_RelativePtBBDown;
    thisMEM.p_RelativePtEC1Up=br_p_RelativePtEC1Up;
    thisMEM.p_RelativePtEC1Down=br_p_RelativePtEC1Down;
    thisMEM.p_RelativePtEC2Up=br_p_RelativePtEC2Up;
    thisMEM.p_RelativePtEC2Down=br_p_RelativePtEC2Down;
    thisMEM.p_RelativePtHFUp=br_p_RelativePtHFUp;
    thisMEM.p_RelativePtHFDown=br_p_RelativePtHFDown;
    thisMEM.p_RelativeFSRUp=br_p_RelativeFSRUp;
    thisMEM.p_RelativeFSRDown=br_p_RelativeFSRDown;
    thisMEM.p_RelativeStatFSRUp=br_p_RelativeStatFSRUp;
    thisMEM.p_RelativeStatFSRDown=br_p_RelativeStatFSRDown;
    thisMEM.p_RelativeStatECUp=br_p_RelativeStatECUp;
    thisMEM.p_RelativeStatECDown=br_p_RelativeStatECDown;
    thisMEM.p_RelativeStatHFUp=br_p_RelativeStatHFUp;
    thisMEM.p_RelativeStatHFDown=br_p_RelativeStatHFDown;
    thisMEM.p_PileUpDataMCUp=br_p_PileUpDataMCUp;
    thisMEM.p_PileUpDataMCDown=br_p_PileUpDataMCDown;
    thisMEM.p_PileUpPtRefUp=br_p_PileUpPtRefUp;
    thisMEM.p_PileUpPtRefDown=br_p_PileUpPtRefDown;
    thisMEM.p_PileUpPtBBUp=br_p_PileUpPtBBUp;
    thisMEM.p_PileUpPtBBDown=br_p_PileUpPtBBDown;
    thisMEM.p_PileUpPtEC1Up=br_p_PileUpPtEC1Up;
    thisMEM.p_PileUpPtEC1Down=br_p_PileUpPtEC1Down;
    thisMEM.p_PileUpPtEC2Up=br_p_PileUpPtEC2Up;
    thisMEM.p_PileUpPtEC2Down=br_p_PileUpPtEC2Down;
    thisMEM.p_PileUpPtHFUp=br_p_PileUpPtHFUp;
    thisMEM.p_PileUpPtHFDown=br_p_PileUpPtHFDown;
    thisMEM.p_RelativeBalUp=br_p_RelativeBalUp;
    thisMEM.p_RelativeBalDown=br_p_RelativeBalDown;
    thisMEM.p_JERUp=br_p_JERUp;
    thisMEM.p_JERDown=br_p_JERDown;

    thisMEM.p_err_sig=br_p_err_sig;
    thisMEM.p_err_bkg=br_p_err_bkg;
    thisMEM.n_perm_sig=br_n_perm_sig;
    thisMEM.n_perm_bkg=br_n_perm_bkg;
    
//     std::cout<<"FOUND p= "<<thisMEM.p<<" for "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
  else{
//     std::cout<<"NO ENTRY FOR: "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
    
  return thisMEM;
}

bool DataBaseSample::CloseTree(){
  if(currentOpenFileName=="" or currentOpenTreeFile==nullptr or currentOpenTree==nullptr){
//     std::cout<<"nothing to do"<<std::endl;
    return true;
  }
  
//   std::cout<<"closing"<<std::endl;
//      std::cout<<currentOpenFileName<<" "<<currentOpenTree<<" "<<currentOpenTreeFile<<std::endl;

  if(openMode=="UPDATE"){
//        std::cout<<currentOpenFileName<<" "<<currentOpenTree<<" "<<currentOpenTreeFile<<std::endl;

//     std::cout<<"sorting"<<std::endl;
//     std::cout<<currentOpenTree<<std::endl;
//     std::cout<<currentOpenTree->GetTitle()<<std::endl;
//     std::cout<<currentOpenTree->GetEntries()<<std::endl;

    Int_t nentries = currentOpenTree->GetEntries();
//     std::cout<<nentries<<std::endl;
    currentOpenTree->Draw("event","","goff");
    Int_t *myid = new Int_t[nentries];
    std::cout<<"starting sorting"<<std::endl;
    TMath::Sort(nentries,currentOpenTree->GetV1(),myid,false);    
    TFile* newtreefile = new TFile(dataBaseDirectory+"/"+currentOpenFileName+"_buffer","RECREATE");
    TTree *tsorted = (TTree*)currentOpenTree->CloneTree(0);
    std::cout<<"filling sorted events"<<std::endl;
    for (Int_t i=0;i<nentries;i++) {
      currentOpenTree->GetEntry(myid[i]);
      tsorted->Fill();
    }
    std::cout<<"DONE filling sorted events"<<std::endl;
    
    tsorted->Write();
    delete [] myid;
    
    newtreefile->Close();
    delete newtreefile;
  }
  currentOpenTreeFile->Close();
  delete currentOpenTreeFile;
  
  if(openMode=="UPDATE"){
    TString oldname=dataBaseDirectory+TString("/")+currentOpenFileName;
    TString newname=dataBaseDirectory+TString("/")+currentOpenFileName+TString("_buffer");
//     std::cout<<"names "<<oldname<<" "<<newname<<std::endl;
    remove(oldname);
    rename(newname, oldname);
  }
    
  currentOpenFileName="";
  currentOpenTreeFile=nullptr;
  currentOpenTree=nullptr;
  return true;
  
}

bool DataBaseSample::OpenTree(TString filename){
//   std::cout<<"about to open "<<filename<<std::endl;
  
  CloseTree();
  currentOpenFileName=filename;  
  currentOpenTreeFile = new TFile(dataBaseDirectory+"/"+filename, openMode);

  if((currentOpenTreeFile == nullptr) || currentOpenTreeFile->IsZombie())
  {
    throw std::runtime_error("DataBaseSample::OpenTree() -- failed to access input .root file: "+dataBaseDirectory+"/"+filename);
  }

//   std::cout<<currentOpenFileName<<" "<<currentOpenTreeFile<<std::endl;
  currentOpenTree=(TTree*)currentOpenTreeFile->Get("MVATree");
//   std::cout<<currentOpenTree<<std::endl;
//   std::cout<<currentOpenTree->GetEntries()<<std::endl;
  currentOpenTree->SetBranchAddress("run",&brRun);
  currentOpenTree->SetBranchAddress("lumi",&brLumi);
  currentOpenTree->SetBranchAddress("event",&brEvent);
  currentOpenTree->SetBranchAddress("p",&br_p);
  currentOpenTree->SetBranchAddress("p_sig",&br_p_sig);
  currentOpenTree->SetBranchAddress("p_bkg",&br_p_bkg);
  currentOpenTree->SetBranchAddress("p_AbsoluteStatUp",&br_p_AbsoluteStatUp);
  currentOpenTree->SetBranchAddress("p_AbsoluteStatDown",&br_p_AbsoluteStatDown);
  currentOpenTree->SetBranchAddress("p_AbsoluteScaleUp",&br_p_AbsoluteScaleUp);
  currentOpenTree->SetBranchAddress("p_AbsoluteScaleDown",&br_p_AbsoluteScaleDown);
  currentOpenTree->SetBranchAddress("p_AbsoluteFlavMapUp",&br_p_AbsoluteFlavMapUp);
  currentOpenTree->SetBranchAddress("p_AbsoluteFlavMapDown",&br_p_AbsoluteFlavMapDown);
  currentOpenTree->SetBranchAddress("p_AbsoluteMPFBiasUp",&br_p_AbsoluteMPFBiasUp);
  currentOpenTree->SetBranchAddress("p_AbsoluteMPFBiasDown",&br_p_AbsoluteMPFBiasDown);
  currentOpenTree->SetBranchAddress("p_FragmentationUp",&br_p_FragmentationUp);
  currentOpenTree->SetBranchAddress("p_FragmentationDown",&br_p_FragmentationDown);
  currentOpenTree->SetBranchAddress("p_SinglePionECALUp",&br_p_SinglePionECALUp);
  currentOpenTree->SetBranchAddress("p_SinglePionECALDown",&br_p_SinglePionECALDown);
  currentOpenTree->SetBranchAddress("p_SinglePionHCALUp",&br_p_SinglePionHCALUp);
  currentOpenTree->SetBranchAddress("p_SinglePionHCALDown",&br_p_SinglePionHCALDown);
  currentOpenTree->SetBranchAddress("p_FlavorQCDUp",&br_p_FlavorQCDUp);
  currentOpenTree->SetBranchAddress("p_FlavorQCDDown",&br_p_FlavorQCDDown);
  currentOpenTree->SetBranchAddress("p_TimePtEtaUp",&br_p_TimePtEtaUp);
  currentOpenTree->SetBranchAddress("p_TimePtEtaDown",&br_p_TimePtEtaDown);
  currentOpenTree->SetBranchAddress("p_RelativeJEREC1Up",&br_p_RelativeJEREC1Up);
  currentOpenTree->SetBranchAddress("p_RelativeJEREC1Down",&br_p_RelativeJEREC1Down);
  currentOpenTree->SetBranchAddress("p_RelativeJEREC2Up",&br_p_RelativeJEREC2Up);
  currentOpenTree->SetBranchAddress("p_RelativeJEREC2Down",&br_p_RelativeJEREC2Down);
  currentOpenTree->SetBranchAddress("p_RelativeJERHFUp",&br_p_RelativeJERHFUp);
  currentOpenTree->SetBranchAddress("p_RelativeJERHFDown",&br_p_RelativeJERHFDown);
  currentOpenTree->SetBranchAddress("p_RelativePtBBUp",&br_p_RelativePtBBUp);
  currentOpenTree->SetBranchAddress("p_RelativePtBBDown",&br_p_RelativePtBBDown);
  currentOpenTree->SetBranchAddress("p_RelativePtEC1Up",&br_p_RelativePtEC1Up);
  currentOpenTree->SetBranchAddress("p_RelativePtEC1Down",&br_p_RelativePtEC1Down);
  currentOpenTree->SetBranchAddress("p_RelativePtEC2Up",&br_p_RelativePtEC2Up);
  currentOpenTree->SetBranchAddress("p_RelativePtEC2Down",&br_p_RelativePtEC2Down);
  currentOpenTree->SetBranchAddress("p_RelativePtHFUp",&br_p_RelativePtHFUp);
  currentOpenTree->SetBranchAddress("p_RelativePtHFDown",&br_p_RelativePtHFDown);
  currentOpenTree->SetBranchAddress("p_RelativeFSRUp",&br_p_RelativeFSRUp);
  currentOpenTree->SetBranchAddress("p_RelativeFSRDown",&br_p_RelativeFSRDown);
  currentOpenTree->SetBranchAddress("p_RelativeStatFSRUp",&br_p_RelativeStatFSRUp);
  currentOpenTree->SetBranchAddress("p_RelativeStatFSRDown",&br_p_RelativeStatFSRDown);
  currentOpenTree->SetBranchAddress("p_RelativeStatECUp",&br_p_RelativeStatECUp);
  currentOpenTree->SetBranchAddress("p_RelativeStatECDown",&br_p_RelativeStatECDown);
  currentOpenTree->SetBranchAddress("p_RelativeStatHFUp",&br_p_RelativeStatHFUp);
  currentOpenTree->SetBranchAddress("p_RelativeStatHFDown",&br_p_RelativeStatHFDown);
  currentOpenTree->SetBranchAddress("p_PileUpDataMCUp",&br_p_PileUpDataMCUp);
  currentOpenTree->SetBranchAddress("p_PileUpDataMCDown",&br_p_PileUpDataMCDown);
  currentOpenTree->SetBranchAddress("p_PileUpPtRefUp",&br_p_PileUpPtRefUp);
  currentOpenTree->SetBranchAddress("p_PileUpPtRefDown",&br_p_PileUpPtRefDown);
  currentOpenTree->SetBranchAddress("p_PileUpPtBBUp",&br_p_PileUpPtBBUp);
  currentOpenTree->SetBranchAddress("p_PileUpPtBBDown",&br_p_PileUpPtBBDown);
  currentOpenTree->SetBranchAddress("p_PileUpPtEC1Up",&br_p_PileUpPtEC1Up);
  currentOpenTree->SetBranchAddress("p_PileUpPtEC1Down",&br_p_PileUpPtEC1Down);
  currentOpenTree->SetBranchAddress("p_PileUpPtEC2Up",&br_p_PileUpPtEC2Up);
  currentOpenTree->SetBranchAddress("p_PileUpPtEC2Down",&br_p_PileUpPtEC2Down);
  currentOpenTree->SetBranchAddress("p_PileUpPtHFUp",&br_p_PileUpPtHFUp);
  currentOpenTree->SetBranchAddress("p_PileUpPtHFDown",&br_p_PileUpPtHFDown);
  currentOpenTree->SetBranchAddress("p_RelativeBalUp",&br_p_RelativeBalUp);
  currentOpenTree->SetBranchAddress("p_RelativeBalDown",&br_p_RelativeBalDown);
  currentOpenTree->SetBranchAddress("p_JERUp",&br_p_JERUp);
  currentOpenTree->SetBranchAddress("p_JERDown",&br_p_JERDown);
  currentOpenTree->SetBranchAddress("p_err_sig",&br_p_err_sig);
  currentOpenTree->SetBranchAddress("p_err_bkg",&br_p_err_bkg);
  currentOpenTree->SetBranchAddress("n_perm_sig",&br_n_perm_sig);
  currentOpenTree->SetBranchAddress("n_perm_bkg",&br_n_perm_bkg);

//   std::cout<<"opened tree "<<dataBaseDirectory<<"/"<<filename<<std::endl;
  lookUpMap.clear();
  
  currentTreeEntries=currentOpenTree->GetEntries();
  for(Int_t ievt=0; ievt<currentTreeEntries; ievt++){
    if(ievt%LookUpStepSize==0){
      currentOpenTree->GetEntry(ievt);
      Int_t testEventNumber=brEvent;
      
//       std::cout<<"set up looUp table "<<ievt<<" "<<testEventNumber<<std::endl;
      lookUpMap[ievt]=testEventNumber;
    }
  }
  
  return true;
}

void DataBaseSample::PrintStructure(){
  std::cout<<"-------------------------------------"<<std::endl;
  std::cout<<"sample name "<<sampleName<<std::endl;
  std::cout<<"sample dataBaseDirectory "<<dataBaseDirectory<<std::endl;
  std::cout<<"sample index file "<<indexFile<<std::endl;
  
  for(auto & irun : runCollection){
    irun.PrintStructure();    
    
  }
}

bool DataBaseSample::SaveDataBase(){
  if(openMode=="UPDATE"){
   std::ofstream idfilestream(dataBaseDirectory+TString("/")+indexFile);
   for(unsigned int ib=0; ib<FileNames.size(); ib++){
     idfilestream<<FileNames.at(ib);
     idfilestream<<"\n";
   }
   idfilestream.close();
   std::cout<<"index file written"<<std::endl;
   }
   
   CloseTree();
   
   return true;
}

DataBaseSample::~DataBaseSample(){
//    if(currentOpenTreeFile!=nullptr){
//     currentOpenTreeFile->Close();
//     delete currentOpenTreeFile;
//    }  
  
//    std::cout<<"destructor called"<<std::endl;
   
   CloseTree();
//    std::cout<<"destructor succeeded"<<std::endl;

}


bool DataBaseSample::AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_JERUp, Double_t p_JERDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
//   std::cout<<eventNumber<<" "<<relevantFileName<<std::endl;
  
  if(relevantFileName=="" or relevantFileName==sampleName+"_"){
//     std::cout<<"collection for event does not exist yes. Creating it"<<std::endl;
    //get ranges for lumiinterval
    Long64_t maxRangeForLumi=100000;
    Long64_t lowerlumival=lumiSection-lumiSection%maxRangeForLumi;
    Long64_t upperlumival=lowerlumival+maxRangeForLumi-1;
    AddRunLumiEventCollection(runNumber, lowerlumival, upperlumival, -99999999999,99999999999);

    
    relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
    CreateNewTree(relevantFileName);
  }
  if(relevantFileName!=""){
    if(relevantFileName!=currentOpenFileName){openMode="UPDATE"; OpenTree(relevantFileName);}
    AddEventToTree(runNumber, lumiSection, eventNumber, p, p_sig, p_bkg, p_AbsoluteStatUp, p_AbsoluteStatDown,p_AbsoluteScaleUp,p_AbsoluteScaleDown,p_AbsoluteFlavMapUp,p_AbsoluteFlavMapDown,p_AbsoluteMPFBiasUp,p_AbsoluteMPFBiasDown,p_FragmentationUp,p_FragmentationDown,p_SinglePionECALUp,p_SinglePionECALDown,p_SinglePionHCALUp,p_SinglePionHCALDown,p_FlavorQCDUp,p_FlavorQCDDown,p_TimePtEtaUp,p_TimePtEtaDown,p_RelativeJEREC1Up,p_RelativeJEREC1Down,p_RelativeJEREC2Up,p_RelativeJEREC2Down,p_RelativeJERHFUp,p_RelativeJERHFDown,p_RelativePtBBUp,p_RelativePtBBDown,p_RelativePtEC1Up,p_RelativePtEC1Down,p_RelativePtEC2Up,p_RelativePtEC2Down,p_RelativePtHFUp,p_RelativePtHFDown,p_RelativeFSRUp,p_RelativeFSRDown,p_RelativeStatFSRUp,p_RelativeStatFSRDown,p_RelativeStatECUp,p_RelativeStatECDown,p_RelativeStatHFUp,p_RelativeStatHFDown,p_PileUpDataMCUp,p_PileUpDataMCDown,p_PileUpPtRefUp,p_PileUpPtRefDown,p_PileUpPtBBUp,p_PileUpPtBBDown,p_PileUpPtEC1Up,p_PileUpPtEC1Down,p_PileUpPtEC2Up,p_PileUpPtEC2Down,p_PileUpPtHFUp,p_PileUpPtHFDown,p_RelativeBalUp,p_RelativeBalDown,p_JERUp,p_JERDown,p_err_sig, p_err_bkg,n_perm_sig,n_perm_bkg);  
  }

//   CloseTree();
  return true; 
}

void DataBaseSample::AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_AbsoluteStatUp, Double_t p_AbsoluteStatDown, Double_t p_AbsoluteScaleUp, Double_t p_AbsoluteScaleDown, Double_t p_AbsoluteFlavMapUp, Double_t p_AbsoluteFlavMapDown, Double_t p_AbsoluteMPFBiasUp, Double_t p_AbsoluteMPFBiasDown, Double_t p_FragmentationUp, Double_t p_FragmentationDown, Double_t p_SinglePionECALUp, Double_t p_SinglePionECALDown, Double_t p_SinglePionHCALUp, Double_t p_SinglePionHCALDown, Double_t p_FlavorQCDUp, Double_t p_FlavorQCDDown, Double_t p_TimePtEtaUp, Double_t p_TimePtEtaDown, Double_t p_RelativeJEREC1Up, Double_t p_RelativeJEREC1Down, Double_t p_RelativeJEREC2Up, Double_t p_RelativeJEREC2Down, Double_t p_RelativeJERHFUp, Double_t p_RelativeJERHFDown, Double_t p_RelativePtBBUp, Double_t p_RelativePtBBDown, Double_t p_RelativePtEC1Up, Double_t p_RelativePtEC1Down, Double_t p_RelativePtEC2Up, Double_t p_RelativePtEC2Down, Double_t p_RelativePtHFUp, Double_t p_RelativePtHFDown, Double_t p_RelativeFSRUp, Double_t p_RelativeFSRDown, Double_t p_RelativeStatFSRUp, Double_t p_RelativeStatFSRDown, Double_t p_RelativeStatECUp, Double_t p_RelativeStatECDown, Double_t p_RelativeStatHFUp, Double_t p_RelativeStatHFDown, Double_t p_PileUpDataMCUp, Double_t p_PileUpDataMCDown, Double_t p_PileUpPtRefUp, Double_t p_PileUpPtRefDown, Double_t p_PileUpPtBBUp, Double_t p_PileUpPtBBDown, Double_t p_PileUpPtEC1Up, Double_t p_PileUpPtEC1Down, Double_t p_PileUpPtEC2Up, Double_t p_PileUpPtEC2Down, Double_t p_PileUpPtHFUp, Double_t p_PileUpPtHFDown, Double_t p_RelativeBalUp, Double_t p_RelativeBalDown, Double_t p_JERUp, Double_t p_JERDown, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg){
    
   brRun=runNumber;
   brLumi=lumiSection;
   brEvent=eventNumber;
  
   br_p=p;
   br_p_sig=p_sig;
   br_p_bkg=p_bkg;
   br_p_AbsoluteStatUp=p_AbsoluteStatUp;
   br_p_AbsoluteStatDown=p_AbsoluteStatDown;
   br_p_AbsoluteScaleUp=p_AbsoluteScaleUp;
   br_p_AbsoluteScaleDown=p_AbsoluteScaleDown;
   br_p_AbsoluteFlavMapUp=p_AbsoluteFlavMapUp;
   br_p_AbsoluteFlavMapDown=p_AbsoluteFlavMapDown;
   br_p_AbsoluteMPFBiasUp=p_AbsoluteMPFBiasUp;
   br_p_AbsoluteMPFBiasDown=p_AbsoluteMPFBiasDown;
   br_p_FragmentationUp=p_FragmentationUp;
   br_p_FragmentationDown=p_FragmentationDown;
   br_p_SinglePionECALUp=p_SinglePionECALUp;
   br_p_SinglePionECALDown=p_SinglePionECALDown;
   br_p_SinglePionHCALUp=p_SinglePionHCALUp;
   br_p_SinglePionHCALDown=p_SinglePionHCALDown;
   br_p_FlavorQCDUp=p_FlavorQCDUp;
   br_p_FlavorQCDDown=p_FlavorQCDDown;
   br_p_TimePtEtaUp=p_TimePtEtaUp;
   br_p_TimePtEtaDown=p_TimePtEtaDown;
   br_p_RelativeJEREC1Up=p_RelativeJEREC1Up;
   br_p_RelativeJEREC1Down=p_RelativeJEREC1Down;
   br_p_RelativeJEREC2Up=p_RelativeJEREC2Up;
   br_p_RelativeJEREC2Down=p_RelativeJEREC2Down;
   br_p_RelativeJERHFUp=p_RelativeJERHFUp;
   br_p_RelativeJERHFDown=p_RelativeJERHFDown;
   br_p_RelativePtBBUp=p_RelativePtBBUp;
   br_p_RelativePtBBDown=p_RelativePtBBDown;
   br_p_RelativePtEC1Up=p_RelativePtEC1Up;
   br_p_RelativePtEC1Down=p_RelativePtEC1Down;
   br_p_RelativePtEC2Up=p_RelativePtEC2Up;
   br_p_RelativePtEC2Down=p_RelativePtEC2Down;
   br_p_RelativePtHFUp=p_RelativePtHFUp;
   br_p_RelativePtHFDown=p_RelativePtHFDown;
   br_p_RelativeFSRUp=p_RelativeFSRUp;
   br_p_RelativeFSRDown=p_RelativeFSRDown;
   br_p_RelativeStatFSRUp=p_RelativeStatFSRUp;
   br_p_RelativeStatFSRDown=p_RelativeStatFSRDown;
   br_p_RelativeStatECUp=p_RelativeStatECUp;
   br_p_RelativeStatECDown=p_RelativeStatECDown;
   br_p_RelativeStatHFUp=p_RelativeStatHFUp;
   br_p_RelativeStatHFDown=p_RelativeStatHFDown;
   br_p_PileUpDataMCUp=p_PileUpDataMCUp;
   br_p_PileUpDataMCDown=p_PileUpDataMCDown;
   br_p_PileUpPtRefUp=p_PileUpPtRefUp;
   br_p_PileUpPtRefDown=p_PileUpPtRefDown;
   br_p_PileUpPtBBUp=p_PileUpPtBBUp;
   br_p_PileUpPtBBDown=p_PileUpPtBBDown;
   br_p_PileUpPtEC1Up=p_PileUpPtEC1Up;
   br_p_PileUpPtEC1Down=p_PileUpPtEC1Down;
   br_p_PileUpPtEC2Up=p_PileUpPtEC2Up;
   br_p_PileUpPtEC2Down=p_PileUpPtEC2Down;
   br_p_PileUpPtHFUp=p_PileUpPtHFUp;
   br_p_PileUpPtHFDown=p_PileUpPtHFDown;
   br_p_RelativeBalUp=p_RelativeBalUp;
   br_p_RelativeBalDown=p_RelativeBalDown;
   br_p_JERUp=p_JERUp;
   br_p_JERDown=p_JERDown;
   br_p_err_sig=p_err_sig;
   br_p_err_bkg=p_err_bkg;
   br_n_perm_sig=n_perm_sig;
   br_n_perm_bkg=n_perm_bkg;
   
   currentOpenTree->Fill();
//    std::cout<<"filled "<<eventNumber<<std::endl;

   currentTreeEntries=currentOpenTree->GetEntries();
//    std::cout<<currentOpenTree<<std::endl;
//    std::cout<<currentOpenTree->GetEntries()<<std::endl;
   if(currentTreeEntries>=maxEventsPerTree){
     SplitCollection(currentOpenFileName);
     
   }


}


bool DataBaseSample::CreateNewTree(TString filename){
//   TString newname=dataBaseDirectory+TString("/")+filename;
//   remove(newname);
  TFile* newtreefile= new TFile(dataBaseDirectory+"/"+filename,"RECREATE");

  TTree* newtree = new TTree("MVATree","MVATree");  
  newtree->Branch("run",&brRun,"run/L");
  newtree->Branch("lumi",&brLumi,"lumi/L");
  newtree->Branch("event",&brEvent,"event/L");
  newtree->Branch("p",&br_p,"p/D");
  newtree->Branch("p_sig",&br_p_sig,"p_sig/D");
  newtree->Branch("p_bkg",&br_p_bkg,"p_bkg/D");
  newtree->Branch("p_AbsoluteStatUp",&br_p_AbsoluteStatUp,"AbsoluteStatUp_p/D");
  newtree->Branch("p_AbsoluteStatDown",&br_p_AbsoluteStatDown,"AbsoluteStatDown_p/D");
  newtree->Branch("p_AbsoluteScaleUp",&br_p_AbsoluteScaleUp,"AbsoluteScaleUp_p/D");
  newtree->Branch("p_AbsoluteScaleDown",&br_p_AbsoluteScaleDown,"AbsoluteScaleDown_p/D");
  newtree->Branch("p_AbsoluteFlavMapUp",&br_p_AbsoluteFlavMapUp,"AbsoluteFlavMapUp_p/D");
  newtree->Branch("p_AbsoluteFlavMapDown",&br_p_AbsoluteFlavMapDown,"AbsoluteFlavMapDown_p/D");
  newtree->Branch("p_AbsoluteMPFBiasUp",&br_p_AbsoluteMPFBiasUp,"AbsoluteMPFBiasUp_p/D");
  newtree->Branch("p_AbsoluteMPFBiasDown",&br_p_AbsoluteMPFBiasDown,"AbsoluteMPFBiasDown_p/D");
  newtree->Branch("p_FragmentationUp",&br_p_FragmentationUp,"FragmentationUp_p/D");
  newtree->Branch("p_FragmentationDown",&br_p_FragmentationDown,"FragmentationDown_p/D");
  newtree->Branch("p_SinglePionECALUp",&br_p_SinglePionECALUp,"SinglePionECALUp_p/D");
  newtree->Branch("p_SinglePionECALDown",&br_p_SinglePionECALDown,"SinglePionECALDown_p/D");
  newtree->Branch("p_SinglePionHCALUp",&br_p_SinglePionHCALUp,"SinglePionHCALUp_p/D");
  newtree->Branch("p_SinglePionHCALDown",&br_p_SinglePionHCALDown,"SinglePionHCALDown_p/D");
  newtree->Branch("p_FlavorQCDUp",&br_p_FlavorQCDUp,"FlavorQCDUp_p/D");
  newtree->Branch("p_FlavorQCDDown",&br_p_FlavorQCDDown,"FlavorQCDDown_p/D");
  newtree->Branch("p_TimePtEtaUp",&br_p_TimePtEtaUp,"TimePtEtaUp_p/D");
  newtree->Branch("p_TimePtEtaDown",&br_p_TimePtEtaDown,"TimePtEtaDown_p/D");
  newtree->Branch("p_RelativeJEREC1Up",&br_p_RelativeJEREC1Up,"RelativeJEREC1Up_p/D");
  newtree->Branch("p_RelativeJEREC1Down",&br_p_RelativeJEREC1Down,"RelativeJEREC1Down_p/D");
  newtree->Branch("p_RelativeJEREC2Up",&br_p_RelativeJEREC2Up,"RelativeJEREC2Up_p/D");
  newtree->Branch("p_RelativeJEREC2Down",&br_p_RelativeJEREC2Down,"RelativeJEREC2Down_p/D");
  newtree->Branch("p_RelativeJERHFUp",&br_p_RelativeJERHFUp,"RelativeJERHFUp_p/D");
  newtree->Branch("p_RelativeJERHFDown",&br_p_RelativeJERHFDown,"RelativeJERHFDown_p/D");
  newtree->Branch("p_RelativePtBBUp",&br_p_RelativePtBBUp,"RelativePtBBUp_p/D");
  newtree->Branch("p_RelativePtBBDown",&br_p_RelativePtBBDown,"RelativePtBBDown_p/D");
  newtree->Branch("p_RelativePtEC1Up",&br_p_RelativePtEC1Up,"RelativePtEC1Up_p/D");
  newtree->Branch("p_RelativePtEC1Down",&br_p_RelativePtEC1Down,"RelativePtEC1Down_p/D");
  newtree->Branch("p_RelativePtEC2Up",&br_p_RelativePtEC2Up,"RelativePtEC2Up_p/D");
  newtree->Branch("p_RelativePtEC2Down",&br_p_RelativePtEC2Down,"RelativePtEC2Down_p/D");
  newtree->Branch("p_RelativePtHFUp",&br_p_RelativePtHFUp,"RelativePtHFUp_p/D");
  newtree->Branch("p_RelativePtHFDown",&br_p_RelativePtHFDown,"RelativePtHFDown_p/D");
  newtree->Branch("p_RelativeFSRUp",&br_p_RelativeFSRUp,"RelativeFSRUp_p/D");
  newtree->Branch("p_RelativeFSRDown",&br_p_RelativeFSRDown,"RelativeFSRDown_p/D");
  newtree->Branch("p_RelativeStatFSRUp",&br_p_RelativeStatFSRUp,"RelativeStatFSRUp_p/D");
  newtree->Branch("p_RelativeStatFSRDown",&br_p_RelativeStatFSRDown,"RelativeStatFSRDown_p/D");
  newtree->Branch("p_RelativeStatECUp",&br_p_RelativeStatECUp,"RelativeStatECUp_p/D");
  newtree->Branch("p_RelativeStatECDown",&br_p_RelativeStatECDown,"RelativeStatECDown_p/D");
  newtree->Branch("p_RelativeStatHFUp",&br_p_RelativeStatHFUp,"RelativeStatHFUp_p/D");
  newtree->Branch("p_RelativeStatHFDown",&br_p_RelativeStatHFDown,"RelativeStatHFDown_p/D");
  newtree->Branch("p_PileUpDataMCUp",&br_p_PileUpDataMCUp,"PileUpDataMCUp_p/D");
  newtree->Branch("p_PileUpDataMCDown",&br_p_PileUpDataMCDown,"PileUpDataMCDown_p/D");
  newtree->Branch("p_PileUpPtRefUp",&br_p_PileUpPtRefUp,"PileUpPtRefUp_p/D");
  newtree->Branch("p_PileUpPtRefDown",&br_p_PileUpPtRefDown,"PileUpPtRefDown_p/D");
  newtree->Branch("p_PileUpPtBBUp",&br_p_PileUpPtBBUp,"PileUpPtBBUp_p/D");
  newtree->Branch("p_PileUpPtBBDown",&br_p_PileUpPtBBDown,"PileUpPtBBDown_p/D");
  newtree->Branch("p_PileUpPtEC1Up",&br_p_PileUpPtEC1Up,"PileUpPtEC1Up_p/D");
  newtree->Branch("p_PileUpPtEC1Down",&br_p_PileUpPtEC1Down,"PileUpPtEC1Down_p/D");
  newtree->Branch("p_PileUpPtEC2Up",&br_p_PileUpPtEC2Up,"PileUpPtEC2Up_p/D");
  newtree->Branch("p_PileUpPtEC2Down",&br_p_PileUpPtEC2Down,"PileUpPtEC2Down_p/D");
  newtree->Branch("p_PileUpPtHFUp",&br_p_PileUpPtHFUp,"PileUpPtHFUp_p/D");
  newtree->Branch("p_PileUpPtHFDown",&br_p_PileUpPtHFDown,"PileUpPtHFDown_p/D");
  newtree->Branch("p_RelativeBalUp",&br_p_RelativeBalUp,"RelativeBalUp_p/D");
  newtree->Branch("p_RelativeBalDown",&br_p_RelativeBalDown,"RelativeBalDown_p/D");
  newtree->Branch("p_JERUp",&br_p_JERUp,"JERUp_p/D");
  newtree->Branch("p_JERDown",&br_p_JERDown,"JERDown_p/D");
  newtree->Branch("p_err_sig",&br_p_err_sig,"p_err_sig/D");
  newtree->Branch("p_err_bkg",&br_p_err_bkg,"p_err_bkg/D");
  newtree->Branch("n_perm_sig",&br_n_perm_sig,"n_perm_sig/D");
  newtree->Branch("n_perm_bkg",&br_n_perm_bkg,"n_perm_bkg/D");
  newtree->AutoSave();
  newtreefile->Close();
  delete newtreefile;
  
//   std::cout<<"created new Tree "<<filename<<std::endl;
  FileNames.push_back(filename);
  return true;
}


bool DataBaseSample::SplitCollection(TString oldfilename){
  
  CloseTree();
  openMode="UPDATE";
  OpenTree(oldfilename);
  
  Int_t oldentries=currentOpenTree->GetEntries();
  
  //get splitting point
  Long64_t midevent;
  currentOpenTree->GetEntry(int(oldentries/2));
//   TString oldfilename=currentOpenFileName;
  midevent=brEvent;
  
  std::cout<<"splitting tree "<<oldfilename<<" at "<<midevent<<std::endl;
  
  TObjArray *obja=oldfilename.Tokenize("_");
  Int_t arraylength=obja->GetEntries();

  
  TString thissamplename=sampleName;
  TString thisrun=((TObjString*)(obja->At(arraylength-6)))->String();
  // TODO cout this stuff
  TString thisminlumi=((TObjString*)(obja->At(arraylength-5)))->String();
  TString thismaxlumi=((TObjString*)(obja->At(arraylength-4)))->String();
  TString thisminevent=((TObjString*)(obja->At(arraylength-3)))->String();
  TString thismaxevent=((TObjString*)(obja->At(arraylength-2)))->String();
  
//   AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
  TString newfilenameDown=thissamplename+TString("_")+thisrun+TString("_")+thisminlumi+TString("_")+thismaxlumi+TString("_")+thisminevent+TString("_")+TString::LLtoa(midevent,10)+TString("_.root");
  TString newfilenameUp=thissamplename+TString("_")+thisrun+TString("_")+thisminlumi+TString("_")+thismaxlumi+TString("_")+TString::LLtoa(midevent+1,10)+TString("_")+thismaxevent+TString("_.root");
  std::cout<<"into "<<newfilenameDown<<" and "<<newfilenameUp<<std::endl;
  
  bool removedCollection=RemoveEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
  if(removedCollection and false){std::cout<<"removed event interval"<<std::endl;}
  AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),midevent);
  AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), midevent+1,thismaxevent.Atoll());

  for(unsigned int fni=0; fni<FileNames.size(); fni++){
    if(FileNames.at(fni)==oldfilename){
      FileNames.erase(FileNames.begin()+fni);
//       std::cout<<"erased filename from list"<<std::endl;
      break;
    } 
  }
  FileNames.push_back(newfilenameDown);
  FileNames.push_back(newfilenameUp);
  
  TFile* newFileUp = new TFile(dataBaseDirectory+TString("/")+newfilenameUp,"RECREATE");
  TTree* newTreeUp=(TTree*)currentOpenTree->CloneTree(0);
  TFile* newFileDown = new TFile(dataBaseDirectory+TString("/")+newfilenameDown,"RECREATE");
  TTree* newTreeDown=(TTree*)currentOpenTree->CloneTree(0);
  
  for(int ievt=0; ievt<oldentries; ievt++){
    currentOpenTree->GetEntry(ievt);
    if(brEvent<=midevent){
//       std::cout<<brEvent<<" down"<<std::endl;
      newTreeDown->Fill(); 
    }
    else{
//       std::cout<<brEvent<<" up"<<std::endl;
      newTreeUp->Fill();
    }
  }
  
  newFileDown->cd();
  newTreeDown->Write();
  newFileDown->Close();
  newFileUp->cd();
  newTreeUp->Write();
  newFileUp->Close();
  
  CloseTree();
  
  delete newFileDown;
  delete newFileUp;
  delete obja;
  
  TString oldfn=dataBaseDirectory+TString("/")+oldfilename;
  remove(oldfn);
  
  std::cout<<"splitting done"<<std::endl;
  return true;
}
