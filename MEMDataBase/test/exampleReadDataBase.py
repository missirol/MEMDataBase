import ROOT
#load library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

# initialize with path to database
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

# load sample, second argument defaults to samplename_index.txt
myDataBase.AddSample("ttH","ttH_index.txt")

#print structure
myDataBase.PrintStructure()

# return an object of class GetMEMResult
# myDataBase.GetMEMResult( samplename, run, lumi, event )
result=myDataBase.GetMEMResult("ttH",1,101,ievt)
print "MEM p", ievt, result.p
print "MEM p_err_bkg", ievt, result.p_err_bkg
#etc