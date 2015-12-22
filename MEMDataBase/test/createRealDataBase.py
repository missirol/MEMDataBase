import ROOT
print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

rand=ROOT.TRandom3(100)

# writable path
t=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

# samplename
t.AddSample("ttH")
  
#t.AddEvent("ttH",1,1,111,0.5,0.4,0.4,0.4,0.4,12,12)
#t.AddEvent("ttH",1,1,114,0.6,0.4,0.4,0.4,0.4,12,12)
#t.AddEvent("ttH",1,1,113,0.7,0.4,0.4,0.4,0.4,12,12)
#t.AddEvent("ttH",1,22,2291,0.8,0.4,0.4,0.4,0.4,12,12)
#t.AddEvent("ttH",2,109,3000,0.8,0.4,0.4,0.4,0.4,12,12)

for i in range(100):
  # AddEvent( sampleName, run, lumi, event, p, p_sig, p_bkg, p_err_sig, p_err_bkg, n_perm_sig, n_perm_bkg )
  t.AddEvent("ttH",1,1,100+i,0.5,0.4,0.4,0.4,0.4,12,12)

t.PrintStructure()

# IMPORTANT TO SAVE
t.SaveDataBase()

