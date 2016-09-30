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
syst=sys.argv[3]
#List of input ntuples containing the MEMs
listOfInputTrees=sys.argv[4:]

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

#loop over events
nEntries=intree.GetEntries()
for ievt in range(nEntries):
  if ievt%1000==0:
    print "reading event ", ievt, "with id ", event[0]
  intree.GetEntry(ievt)
  # add event to database
  # the event is stored with the sample identifier and the run+lumi+eventNumber triple
  # store are the outut values of the MEM calculation
  #print syst,systematic[0]
  if systematic[0]==long(syst):
    #print "true"
    myDataBase.AddEvent(samplename,run[0],lumi[0],event[0],systematic[0],p[0],p_sig[0],p_bkg[0],blr_eth[0],blr_eth_transformed[0],bdt[0])

# you can use this to viusalize the database
myDataBase.PrintStructure()

# IMPORTANT save the databse
myDataBase.SaveDataBase()

# You can later add additional events to the database and the same sample identifier
# BUT better not add events which are already there, this would lead to undefined behaviour