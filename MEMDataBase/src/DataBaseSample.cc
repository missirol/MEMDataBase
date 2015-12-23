#include "MEMDataBase/MEMDataBase/interface/DataBaseSample.h"



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
   std::cout<<"Index file read: "<<indexFile<<std::endl; 
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
  
//   std::cout<<"starting at "<<startEvt<<std::endl;
  
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
    TMath::Sort(nentries,currentOpenTree->GetV1(),myid,false);    
    TFile* newtreefile = new TFile(dataBaseDirectory+"/"+currentOpenFileName+"_buffer","RECREATE");
    TTree *tsorted = (TTree*)currentOpenTree->CloneTree(0);
    for (Int_t i=0;i<nentries;i++) {
      currentOpenTree->GetEntry(myid[i]);
      tsorted->Fill();
    }
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
  currentOpenTree->SetBranchAddress("p_err_sig",&br_p_err_sig);
  currentOpenTree->SetBranchAddress("p_err_bkg",&br_p_err_bkg);
  currentOpenTree->SetBranchAddress("n_perm_sig",&br_n_perm_sig);
  currentOpenTree->SetBranchAddress("n_perm_bkg",&br_n_perm_bkg);

//   std::cout<<"opened tree "<<dataBaseDirectory<<"/"<<filename<<std::endl;
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
  
   std::cout<<"destructor called"<<std::endl;
   
   CloseTree();
   std::cout<<"destructor succeeded"<<std::endl;

}


bool DataBaseSample::AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Float_t p, Float_t p_sig, Float_t p_bkg, Float_t p_err_sig, Float_t p_err_bkg, Float_t n_perm_sig, Float_t n_perm_bkg){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
//   std::cout<<eventNumber<<" "<<relevantFileName<<std::endl;
  
  if(relevantFileName=="" or relevantFileName==sampleName+"_"){
//     std::cout<<"collection for event does not exist yes. Creating it"<<std::endl;
    //get ranges for lumiinterval
    Long64_t lowerlumival=lumiSection-lumiSection%10;
    Long64_t upperlumival=lowerlumival+9;
    AddRunLumiEventCollection(runNumber, lowerlumival, upperlumival, 0,999999999);
    
    relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
    CreateNewTree(relevantFileName);
  }
  if(relevantFileName!=""){
    if(relevantFileName!=currentOpenFileName){openMode="UPDATE"; OpenTree(relevantFileName);}
    AddEventToTree(runNumber, lumiSection, eventNumber, p, p_sig, p_bkg, p_err_sig, p_err_bkg,n_perm_sig,n_perm_bkg);   
  }

//   CloseTree();
  return true; 
}

void DataBaseSample::AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Float_t p, Float_t p_sig, Float_t p_bkg, Float_t p_err_sig, Float_t p_err_bkg, Float_t n_perm_sig, Float_t n_perm_bkg){
    
   brRun=runNumber;
   brLumi=lumiSection;
   brEvent=eventNumber;
  
   br_p=p;
   br_p_sig=p_sig;
   br_p_bkg=p_bkg;
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
  newtree->Branch("run",&brRun,"run/I");
  newtree->Branch("lumi",&brLumi,"lumi/I");
  newtree->Branch("event",&brEvent,"event/I");
  newtree->Branch("p",&br_p,"p/F");
  newtree->Branch("p_sig",&br_p_sig,"p_sig/F");
  newtree->Branch("p_bkg",&br_p_bkg,"p_bkg/F");
  newtree->Branch("p_err_sig",&br_p_err_sig,"p_err_sig/F");
  newtree->Branch("p_err_bkg",&br_p_err_bkg,"p_err_bkg/F");
  newtree->Branch("n_perm_sig",&br_n_perm_sig,"n_perm_sig/F");
  newtree->Branch("n_perm_bkg",&br_n_perm_bkg,"n_perm_bkg/F");
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