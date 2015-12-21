#include "MEMDataBase/MEMDataBase/interface/DataBaseSample.h"

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

DataBaseSample::DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_, const TString indexFile_){
  
  dataBaseDirectory=dataBaseDirectory_;
  sampleName=sampleName_;
  
  if(indexFile_==""){indexFile=sampleName+TString("_index.txt");  }
  else {
    
    indexFile=indexFile_;
    std::ifstream idfilestream(dataBaseDirectory_+TString("/")+indexFile);
    TString dumpline;
     
//     bool readline=true;
    while(true){
//       std::cout<<"bla"<<std::endl;
      idfilestream>>dumpline;
      if(idfilestream.eof())break;

//       std::cout<<dumpline<<std::endl;
     
      TObjArray *obja=dumpline.Tokenize("_");
      
      TString thisrun=((TObjString*)(obja->At(1)))->String();
      TString thisminlumi=((TObjString*)(obja->At(2)))->String();
      TString thismaxlumi=((TObjString*)(obja->At(3)))->String();
      TString thisminevent=((TObjString*)(obja->At(4)))->String();
      TString thismaxevent=((TObjString*)(obja->At(5)))->String();
      
//       std::cout<<thisrun<<" "<<thisminlumi<<" "<<thismaxlumi<<" "<<thisminevent<<" "<<thismaxevent<<std::endl;
      AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
     }
   idfilestream.close();
  }
  
  std::cout<<"Index file read: "<<indexFile<<std::endl; 
  
  currentOpenFileName="";
}

TString DataBaseSample::GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=sampleName+"_";
  
//   RunCollection* thisRunCollection=nullptr;
  
  for(auto & irun : runCollection){
    
    if(irun.GetRunNumber()==runNumber){
      relevantFileName+=TString::LLtoa(runNumber,10)+"_";
      TString substring=irun.GetLumiAndEventIntervalString(lumiSection,eventNumber);
      if(substring!="") relevantFileName+=substring;
      else{
	relevantFileName="";
      }
      relevantFileName+="_.root";
      break;    
    }
  }
  
//   std::cout<<" Event with "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<" is in "<<relevantFileName<<std::endl;
  return relevantFileName;
  
}

DataBaseMEMResult DataBaseSample::GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
  
  DataBaseMEMResult thisMEM;

  if(relevantFileName==""){
    std::cout<<"did not find event in database"<<std::endl;
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
  
  std::cout<<"starting at "<<startEvt<<std::endl;
  
  bool found=false;
  for(Int_t ievt=startEvt; ievt<currentTreeEntries; ievt++){
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
    thisMEM.p_err_sig=br_p_err_sig;
    thisMEM.p_err_bkg=br_p_err_bkg;
    thisMEM.n_perm_sig=br_n_perm_sig;
    thisMEM.n_perm_bkg=br_n_perm_sig;
    
    std::cout<<"FOUND p= "<<thisMEM.p<<" for "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
  else{
    std::cout<<"NO ENTRY FOR: "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
    
  return thisMEM;
}

bool DataBaseSample::OpenTree(TString filename){
//   std::cout<<"about to open "<<filename<<std::endl;
  
  if(currentOpenFileName!=""){  
  currentOpenTreeFile->Close();
  delete currentOpenTreeFile;
  }
    
  currentOpenFileName=filename;

  currentOpenTreeFile = new TFile(dataBaseDirectory+"/"+filename);
//   std::cout<<currentOpenTreeFile<<std::endl;
  
  currentOpenTree=(TTree*)currentOpenTreeFile->Get("MVATree");
  
  currentOpenTree->SetBranchAddress("run",&brRun);
  currentOpenTree->SetBranchAddress("lumi",&brLumi);
  currentOpenTree->SetBranchAddress("event",&brEvent);
  currentOpenTree->SetBranchAddress("p",&br_p);
  currentOpenTree->SetBranchAddress("p_sig",&br_p_sig);
  currentOpenTree->SetBranchAddress("p_bkg",&br_p_bkg);
  currentOpenTree->SetBranchAddress("p_err_sig",&br_p_err_sig);
  currentOpenTree->SetBranchAddress("p_err_bkg",&br_p_err_bkg);
  currentOpenTree->SetBranchAddress("n_perm_sig",&br_n_perm_sig);
  currentOpenTree->SetBranchAddress("n_perm_bkg",&br_n_perm_bkg);

  std::cout<<"opened tree "<<dataBaseDirectory<<"/"<<filename<<std::endl;
  lookUpMap.clear();
  
  currentTreeEntries=currentOpenTree->GetEntries();
  for(Int_t ievt=0; ievt<currentTreeEntries; ievt++){
    if(ievt%100==0){
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

DataBaseSample::~DataBaseSample(){
   if(currentOpenTreeFile!=nullptr){
    currentOpenTreeFile->Close();
    delete currentOpenTreeFile;
   }
  
  
}



