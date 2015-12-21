
#include "MEMDataBase/MEMDataBase/interface/DataBaseRunCollection.h"


//run collection members

RunCollection::RunCollection(const Long64_t runNumber_){
 runNumber=runNumber_; 
}

RunCollection::~RunCollection(){}


void RunCollection::AddLumiAndEventInterval(const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool elementExists=false;
  for(auto & intvl : lumiCollections){
    if(intvl.GetMinLumi()==minLumi_ and intvl.GetMaxLumi()==maxLumi_){
      intvl.AddEventInterval(minEvent_,maxEvent_);
      elementExists=true;
      break;
    }
  }
  if(!elementExists){
   lumiCollections.push_back(LumiCollection(minLumi_, maxLumi_));
   lumiCollections.back().AddEventInterval(minEvent_,maxEvent_);
   std::sort(lumiCollections.begin(), lumiCollections.end(), LumiCollection::minIsSmaller );
  } 
}

void RunCollection::PrintStructure(){
  std::cout<<"run "<<runNumber<<std::endl;
  for(auto & intvl : lumiCollections){
    intvl.PrintStructure(); 
  }
  
}


TString RunCollection::GetLumiAndEventIntervalString(const Long64_t lumiSection, const Long64_t eventNumber){
   TString lumistring=""; 
  
   for(auto & intvl : lumiCollections){
//      std::cout<<lumiSection<<" "<<intvl.GetMinLumi()<<" "<<intvl.GetMaxLumi()<<std::endl;
     if(lumiSection<=intvl.GetMaxLumi() and lumiSection>=intvl.GetMinLumi()){
//        std::cout<<"found lumi"<<std::endl;
       lumistring+=TString::LLtoa(intvl.GetMinLumi(),10);
       lumistring+="_";
       lumistring+=TString::LLtoa(intvl.GetMaxLumi(),10);
       lumistring+="_";
//        std::cout<<lumistring<<std::endl;
       TString substring = intvl.GetEventIntervalString(eventNumber);
       if(substring!=""){
	 lumistring+=substring;
       }
       break;
     }
   }
  
//   std::cout<<"lumistring "<<lumistring<<std::endl;
  return lumistring;
}
