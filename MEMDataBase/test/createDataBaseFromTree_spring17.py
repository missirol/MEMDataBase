# 
#Example that shows how to write the mem database using input ntuples
#
import ROOT
import sys
from jet_corrections import *
from array import array

Cvectordouble = getattr(ROOT, "std::vector<Double_t>")
CvectorTString = getattr(ROOT, "std::vector<TString>")

#print jet_corrections

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
blr_2b=array("d",[0])
blr_4b=array("d",[0])
bdt=array("d",[0])
p=array("d",[0])
p_sig=array("d",[0])
p_bkg=array("d",[0])
p_vec=[array("d",[0]) for i in range(len(jet_corrections)*2+1)]

print p_vec

mem_strings=["mem_p"]
mem_strings+=["mem_"+corr+ud+"_p" for corr in jet_corrections for ud in ["up","down"]]
mem_strings_vec = CvectorTString()
print mem_strings
for mem_string in mem_strings:
    mem_strings_vec.push_back(ROOT.TString(mem_string))
    

print "len mem_strings="+str(len(mem_strings))
print "len p_vec="+str(len(p_vec))

# create a new database at the specified path
myDataBase=ROOT.MEMDataBase(pathToDataBase,mem_strings_vec)

# add the sample to the database
myDataBase.AddSample(samplename)

# add input ntuples to chain
# KIT ntuples are called MVATree
intree=ROOT.TChain("tree")
for intreefilename in listOfInputTrees:
  print "adding ", intreefilename, " to chain"
  intree.Add(intreefilename)
  
print intree.GetEntries()

# seet branch addresses
intree.SetBranchAddress("event",event)
intree.SetBranchAddress("run",run)
intree.SetBranchAddress("lumi",lumi)
intree.SetBranchAddress("systematic",systematic)
intree.SetBranchAddress("blr_4b",blr_4b)
intree.SetBranchAddress("blr_2b",blr_2b)
#intree.SetBranchAddress("bdt",bdt)
#intree.SetBranchAddress("mem_p",p)
intree.SetBranchAddress("mem_p_sig",p_sig)
intree.SetBranchAddress("mem_p_bkg",p_bkg)
#intree.SetBranchAddress("mem_AbsoluteStatup",p_vec[1])
for i in range(len(mem_strings)):
    intree.SetBranchAddress(mem_strings[i],p_vec[i])
    print "set branch adress for "+mem_strings[i]

#loop over events
nEntries=intree.GetEntries()
for ievt in range(nEntries):
  if ievt%1000==0:
    print "reading event ", ievt, "with id ", event[0]
  intree.GetEntry(ievt)
  p_vector = Cvectordouble()
  for i in range(len(p_vec)):
      p_vector.push_back(p_vec[i][0])
  if ievt%1000==0:
      print samplename,run[0],lumi[0],event[0],p_sig[0],p_bkg[0],blr_4b[0],blr_2b[0],[p_vector[i] for i in range(len(p_vec))]
  # add event to database
  # the event is stored with the sample identifier and the run+lumi+eventNumber triple
  # store are the outut values of the MEM calculation
  myDataBase.AddEvent(samplename,run[0],lumi[0],event[0],p_vector,p_sig[0],p_bkg[0],blr_4b[0],blr_2b[0])

# you can use this to viusalize the database
myDataBase.PrintStructure()

# IMPORTANT save the databse
myDataBase.SaveDataBase()

# You can later add additional events to the database and the same sample identifier
# BUT better not add events which are already there, this would lead to undefined behaviour