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

# create a new database at the specified path
myDataBase=ROOT.MEMDataBase(pathToDataBase)

# add the sample to the database
myDataBase.AddSample(samplename)

# add input ntuples to chain
# KIT ntuples are called MVATree
intree=ROOT.TChain("MVATree")
for intreefilename in listOfInputTrees:
  print "adding ", intreefilename, " to chain"
  intree.Add(intreefilename)

# seet branch addresses
intree.SetBranchAddress("run",run)
intree.SetBranchAddress("lumi",lumi)
intree.SetBranchAddress("Evt_ID",event)
intree.SetBranchAddress("MEM_p",p)
intree.SetBranchAddress("MEM_p_sig",p_sig)
intree.SetBranchAddress("MEM_p_bkg",p_bkg)
intree.SetBranchAddress("MEM_p_err_sig",p_err_sig)
intree.SetBranchAddress("MEM_p_err_bkg",p_err_bkg)
intree.SetBranchAddress("MEM_n_perm_sig",n_perm_sig)
intree.SetBranchAddress("MEM_n_perm_bkg",n_perm_bkg)

#loop over events
nEntries=intree.GetEntries()
for ievt in range(nEntries):
  if ievt%1000==0:
    print "reading event ", ievt, "with id ", event[0]
  intree.GetEntry(ievt)
  # add event to database
  # the event is stored with the sample identifier and the run+lumi+eventNumber triple
  # store are the outut values of the MEM calculation
  myDataBase.AddEvent(samplename,run,lumi,event[0],p[0],p_sig[0],p_bkg[0],p_err_sig[0], p_err_bkg[0], n_perm_sig[0], n_perm_bkg[0])

# you can use this to viusalize the database
myDataBase.PrintStructure()

# IMPORTANT save the databse
myDataBase.SaveDataBase()

# You can later add additional events to the database and the same sample identifier
# BUT better not add events which are already there, this would lead to undefined behaviour