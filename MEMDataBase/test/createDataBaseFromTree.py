import ROOT
import sys
from array import array
from glob import glob

#load necesssary library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

path=sys.argv[1]
samplename=sys.argv[2]
#listOfInputTrees=sys.argv[3:]
inwl=sys.argv[3]
print inwl
listOfInputTrees=glob(inwl)
print listOfInputTrees
raw_input()


njets=array("i",[0])
nbtagsM=array("i",[0])
higgstag=array("f",[0])
toptag=array("f",[0])

run=array("i",[0])
lumi=array("i",[0])
event=array("i",[0])

Weight_muRupmuFup=array("f",[0])
Weight_muRdownmuFdown=array("f",[0])
Weight_NNPDFid260067=array("f",[0])
Weight_NNPDFid260005=array("f",[0])
#p_err_bkg=array("f",[0])
#n_perm_sig=array("f",[0])
#n_perm_bkg=array("f",[0])

myDataBase=ROOT.MEMDataBase(path)
myDataBase.AddSample(samplename)

for intreefilename in listOfInputTrees:
  print "reading in ", intreefilename
  infile=ROOT.TFile(intreefilename,"READ")
  intree=infile.Get("MVATree")
  
  intree.SetBranchStatus("*",0)
  intree.SetBranchStatus("Evt_Run",1)
  intree.SetBranchStatus("Evt_Lumi",1)
  intree.SetBranchStatus("Evt_ID",1)
  intree.SetBranchStatus("Weight_muRupmuFup",1)
  intree.SetBranchStatus("Weight_muRdownmuFdown",1)
  intree.SetBranchStatus("Weight_NNPDFid260067",1)
  intree.SetBranchStatus("Weight_NNPDFid260005",1)
  intree.SetBranchStatus("N_Jets",1)
  intree.SetBranchStatus("N_BTagsM",1)
  intree.SetBranchStatus("BoostedTopHiggs_TopHadCandidate_TopMVAOutput",1)
  intree.SetBranchStatus("BoostedTopHiggs_HiggsCandidate_HiggsTag",1)    
  
  
  intree.SetBranchAddress("Evt_Run",run)
  intree.SetBranchAddress("Evt_Lumi",lumi)
  intree.SetBranchAddress("Evt_ID",event)
  intree.SetBranchAddress("Weight_muRupmuFup",Weight_muRupmuFup)
  intree.SetBranchAddress("Weight_muRdownmuFdown",Weight_muRdownmuFdown)
  intree.SetBranchAddress("Weight_NNPDFid260067",Weight_NNPDFid260067)
  intree.SetBranchAddress("Weight_NNPDFid260005",Weight_NNPDFid260005)
  intree.SetBranchAddress("N_Jets",njets)
  intree.SetBranchAddress("N_BTagsM",nbtagsM)
  intree.SetBranchAddress("BoostedTopHiggs_TopHadCandidate_TopMVAOutput",toptag)
  intree.SetBranchAddress("BoostedTopHiggs_HiggsCandidate_HiggsTag",higgstag)      
  
  #intree.SetBranchAddress("MEM_p_err_bkg",p_err_bkg)
  #intree.SetBranchAddress("MEM_n_perm_sig",n_perm_sig)
  #intree.SetBranchAddress("MEM_n_perm_bkg",n_perm_bkg)
  
  nEntries=intree.GetEntries()
  for ievt in range(nEntries):
    if ievt%1000==0:
      print "reading event ", ievt, "with id ", event[0]
    intree.GetEntry(ievt)
    if not((njets[0]>=6 and nbtagsM[0]>=2) or (njets[0]>=4 and nbtagsM[0]>=3) or (toptag[0]>=-0.485 and higgstag[0]>=0.8925)):
      continue
    
    myDataBase.AddEvent(samplename,run[0],lumi[0],event[0],Weight_muRupmuFup[0],Weight_muRdownmuFdown[0],Weight_NNPDFid260067[0],Weight_NNPDFid260005[0])

myDataBase.PrintStructure()
myDataBase.SaveDataBase()
  