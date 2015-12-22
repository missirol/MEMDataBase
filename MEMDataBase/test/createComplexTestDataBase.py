import ROOT
print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

rand=ROOT.TRandom3(100)
logfile=open("testDataBaseLog.txt","w")


t=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

t.AddSample("ttH")

for i in range(1000):
  if i%1000==0:
    print "at ", i
  #lumi=int(rand.Uniform(0,30))
  lumi=1
  event=i
  p=float(rand.Rndm())
  if rand.Rndm()>0.6:
    print "skipping"
    continue
  t.AddEvent("ttH",1,lumi,event,p,0.4,0.4,0.4,0.4,12,12)
  logfile.write(str(1)+","+str(lumi)+","+str(event)+","+str(p)+"\n")
  
t.PrintStructure()
t.SaveDataBase()

logfile.close()
