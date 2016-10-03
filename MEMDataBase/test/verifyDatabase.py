# 
#Example that shows how to write the mem database using input ntuples
#
import ROOT
import sys
from array import array

#load necesssary library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

#Path with write access where the database should be stored
pathToDataBase=sys.argv[1]

# Iternal identifier for sample
# e.g. ttbarInlusive
# or ttHbbJESUP
# DO NOT USE UNDERSCORES _ IN THIS NAME
samplename=sys.argv[2]

#List of input ntuples containing the MEMs
listOfInputTrees=sys.argv[3:]

# arrays to hold branches
run=array("l",[0])
lumi=array("l",[0])
event=array("l",[0])
systematic=array("l",[0])
blr_eth=array("d",[0])
blr_eth_transformed=array("d",[0])
bdt=array("d",[0])
p=array("d",[0])
p_sig=array("d",[0])
p_bkg=array("d",[0])

# create a new database at the specified path
myDataBase=ROOT.MEMDataBase(pathToDataBase)

# add the sample to the database
myDataBase.AddSample(samplename)
myDataBase.AddSample(samplename,samplename+"_indext.txt")
myDataBase.PrintStructure()

# add input ntuples to chain
# KIT ntuples are called MVATree
intree=ROOT.TChain("tree")
for intreefilename in listOfInputTrees:
  print "adding ", intreefilename, " to chain"
  intree.Add(intreefilename)

# seet branch addresses
intree.SetBranchAddress("event",event)
intree.SetBranchAddress("run",run)
intree.SetBranchAddress("lumi",lumi)
intree.SetBranchAddress("systematic",systematic)
intree.SetBranchAddress("blr_eth",blr_eth)
intree.SetBranchAddress("blr_eth_transformed",blr_eth_transformed)
intree.SetBranchAddress("bdt",bdt)
intree.SetBranchAddress("mem_p",p)
intree.SetBranchAddress("mem_p_sig",p_sig)
intree.SetBranchAddress("mem_p_bkg",p_bkg)

nprobs=0
#loop over events
nEntries=intree.GetEntries()
for ievt in range(nEntries):
  if ievt%1000==0:
    print "reading event ", ievt, "with id ", event[0]
  intree.GetEntry(ievt)
  result=myDataBase.GetMEMResult(samplename, run[0],lumi[0],event[0])
  isGood=False
  if result.p==systematic[0] and result.p_sig==p[0] and result.p_bkg==p_sig[0] and result.p_err_sig==p_bkg[0] and result.p_err_bkg==blr_eth[0] and result.n_perm_sig==blr_eth_transformed[0] and result.n_perm_bkg==bdt[0]:
    isGood=True
  if not isGood:
    nprobs+=1
    print "PROBLEM!!!!!!"
    print run[0],lumi[0],event[0],systematic[0],	p[0],           p_sig[0],  p_bkg[0],     blr_eth[0],   blr_eth_transformed[0],  bdt[0]
    print run[0],lumi[0],event[0], result.p, result.p_sig, result.p_bkg, result.p_err_sig, result.p_err_bkg, result.n_perm_sig, result.n_perm_bkg

print nprobs
# myDataBase.AddEvent( sampleName, run, lumi, event, p, p_sig, p_bkg, p_err_sig, p_err_bkg, n_perm_sig, n_perm_bkg )
