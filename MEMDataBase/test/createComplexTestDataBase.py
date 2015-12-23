import ROOT
print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

rand=ROOT.TRandom3(100)
logfile=open("testDataBaseLog.txt","w")
samplename="ttH"
samplename2="ttbar"

t=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")

t.AddSample(samplename)
t.AddSample(samplename2)

for j in range(20):
  for i in range(50000):
    if i%1000==0:
      print samplename, "at ", i
    lumi=j
    #lumi=1
    #event=int(rand.Uniform(0,999999))
    event=j*100000+i
    p=float(rand.Rndm())
    if rand.Rndm()>0.6:
      #print "skipping"
      continue
    t.AddEvent(samplename,1,lumi,event,p,0.4,0.4,0.4,0.4,12,12)
    logfile.write(samplename+","+str(1)+","+str(lumi)+","+str(event)+","+str(p)+"\n")

for j in range(20):  
  for i in range(50000):
    if i%1000==0:
      print samplename2, "at ", i
    lumi=j
    #lumi=1
    #event=int(rand.Uniform(0,999999))
    event=j*100000+i
    p=float(rand.Rndm())
    if rand.Rndm()>0.6:
      #print "skipping"
      continue
    t.AddEvent(samplename2,1,lumi,event,p,0.4,0.4,0.4,0.4,12,12)
    logfile.write(samplename2+","+str(1)+","+str(lumi)+","+str(event)+","+str(p)+"\n")
    
t.PrintStructure()
t.SaveDataBase()

logfile.close()
