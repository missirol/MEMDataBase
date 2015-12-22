import ROOT
rand=ROOT.TRandom3(100)

#load necesssary library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

# initialize database to writeable path
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

# add a sample by name
myDataBase.AddSample("ttH")
 
# add events to database 
# myDataBase.AddEvent( sampleName, run, lumi, event, p, p_sig, p_bkg, p_err_sig, p_err_bkg, n_perm_sig, n_perm_bkg )
for i in range(100):
  myDataBase.AddEvent("ttH",1,1,100+i,0.5,0.4,0.4,0.4,0.4,12,12)

#print structure of database
myDataBase.PrintStructure()

# IMPORTANT! SAVE database 
myDataBase.SaveDataBase()

