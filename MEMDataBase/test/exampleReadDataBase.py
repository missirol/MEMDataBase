#
# example how to retrieve information from the database
#
from jet_corrections import *
import ROOT

CvectorTString = getattr(ROOT, "std::vector<TString>")

#load library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

mem_strings=["mem_p"]
mem_strings+=["mem_"+corr+ud+"_p" for corr in jet_corrections for ud in ["up","down"]]
mem_strings_vec = CvectorTString()
#print mem_strings
# fill the string in a vector to pass to the database
for mem_string in mem_strings:
    mem_strings_vec.push_back(ROOT.TString(mem_string))

# initialize with path to database
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/mwassmer/MEM_database_spring17/test_database/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/",mem_strings_vec)

# load sample by identifier
# The second argument defaults to samplename_index.txt
# this text file simply holds a list of database files, nothing to concern you with
myDataBase.AddSample("ttHTobbM125TuneCUETP8M2ttHtranche313TeVpowhegpythia8","ttHTobbM125TuneCUETP8M2ttHtranche313TeVpowhegpythia8_index.txt")

#print structure
myDataBase.PrintStructure()

run=1
lumi=7391
event=1031032
# retrive MEM for this event
# return an object of class DataBaseMEMResult
# myDataBase.GetMEMResult( samplename, run, lumi, event )
result=myDataBase.GetMEMResult("ttHTobbM125TuneCUETP8M2ttHtranche313TeVpowhegpythia8", run, lumi, event)

print "run lumi event, MEM p", run, lumi, event, result.p_vec[0]
print "MEM p_sig", result.p_sig
print "MEM p_bkg", result.p_bkg
print "MEM p_err_sig",  result.p_err_sig
print "MEM p_err_bkg",  result.p_err_bkg
print "MEM n_perm_sig",  result.n_perm_sig
print "MEM n_perm_bkg",  result.n_perm_bkg
#
