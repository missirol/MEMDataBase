import ROOT
import sys
from array import array

#load necesssary library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

path=sys.argv[1]
samplename=sys.argv[2]
listOfInputTrees=sys.argv[3:]

run=array("i",[0])
lumi=array("i",[0])
event=array("i",[0])

p=array("f",[0])
p_sig=array("f",[0])
p_bkg=array("f",[0])
p_err_sig=array("f",[0])
p_err_bkg=array("f",[0])
n_perm_sig=array("f",[0])
n_perm_bkg=array("f",[0])

myDataBase=ROOT.MEMDataBase(path)
myDataBase.AddSample(samplename)

for intreefilename in listOfInputTrees:
  print "reading in ", intreefilename
  infile=ROOT.TFile(intreefilename,"READ")
  intree=infile.Get("MVATree")
  #intree.SetBranchAddress("run",run)
  #intree.SetBranchAddress("lumi",lumi)
  run=1
  lumi=1
  intree.SetBranchAddress("Evt_ID",event)
  intree.SetBranchAddress("MEM_p",p)
  intree.SetBranchAddress("MEM_p_sig",p_sig)
  intree.SetBranchAddress("MEM_p_bkg",p_bkg)
  intree.SetBranchAddress("MEM_p_err_sig",p_err_sig)
  intree.SetBranchAddress("MEM_p_err_bkg",p_err_bkg)
  intree.SetBranchAddress("MEM_n_perm_sig",n_perm_sig)
  intree.SetBranchAddress("MEM_n_perm_bkg",n_perm_bkg)
  
  nEntries=intree.GetEntries()
  for ievt in range(nEntries):
    if ievt%1000==0:
      print "reading event ", ievt, "with id ", event[0]
    intree.GetEntry(ievt)
    myDataBase.AddEvent(samplename,run,lumi,event[0],p[0],p_sig[0],p_bkg[0],p_err_sig[0], p_err_bkg[0], n_perm_sig[0], n_perm_bkg[0])

myDataBase.PrintStructure()
myDataBase.SaveDataBase()
  