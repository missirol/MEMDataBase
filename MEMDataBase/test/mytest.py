import ROOT
print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")
#print "ok"

rand=ROOT.TRandom3(100)


t=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

#print t.GetValue()


t.AddSample("ttbar","ttbar_index.txt")
t.PrintStructure()

#test the readout

for itest in range(10000):
  print " test ", itest
  ievt=int(rand.Uniform(10,9999999))
  result=t.GetMEMResult("ttbar",1,101,ievt)
  print "MEM ", ievt, result.p