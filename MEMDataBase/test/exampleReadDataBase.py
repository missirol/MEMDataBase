#
# example how to retrieve information from the database
#

import ROOT
#load library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

# initialize with path to database
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

# load sample by identifier
# The second argument defaults to samplename_index.txt
# this text file simply holds a list of database files, nothing to concern you with
myDataBase.AddSample("ttHbbJESUP","ttHbbJESUP_index.txt")

#print structure
myDataBase.PrintStructure()

run=1
lumi=101
event=123456
# retrive MEM for this event
# return an object of class DataBaseMEMResult
# myDataBase.GetMEMResult( samplename, run, lumi, event )
result=myDataBase.GetMEMResult("ttHbbJESUP", run, lumi, event)

print "run lumi event, MEM p", run, lumi, event, result.p
print "MEM p_sig", result.p_sig
print "MEM p_bkg", result.p_bkg
print "MEM p_err_sig", ievt, result.p_err_sig
print "MEM p_err_bkg", ievt, result.p_err_bkg
print "MEM n_perm_sig", ievt, result.n_perm_sig
print "MEM n_perm_bkg", ievt, result.n_perm_bkg
#