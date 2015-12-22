import ROOT
import sys

path=sys.argv[1]
samplename=sys.argv[2]
indexfilename=sys.argv[3]
testlogfile=sys.argv[4]

indexfile=open(path+"/"+indexfilename,"r")
treefilelist=list(indexfile)
#path="/nfs/dust/cms/user/kelmorab/MEDataBase/"
allOk=True

for f in treefilelist:
  print "checking ", f.replace("\n","")
  tf=ROOT.TFile(path+"/"+f.replace("\n",""),"READ")
  t=tf.Get("MVATree")
  
  namelist=f.replace("#.root\n","").split("#")
  #print namelist
  minlumiFromName=namelist[2]
  maxlumiFromName=namelist[3]
  minEventFromName=namelist[4]
  maxEventFromName=namelist[5]
  
  Ok=True
  
  minlumiFromTree=t.GetMinimum("lumi")
  maxlumiFromTree=t.GetMaximum("lumi")
  mineventFromTree=t.GetMinimum("event")
  maxeventFromTree=t.GetMaximum("event")
  
  if minlumiFromTree<int(minlumiFromName) or maxlumiFromTree>int(maxlumiFromName) or maxeventFromTree>int(maxEventFromName) or mineventFromTree<int(minEventFromName):
    Ok=False
    allOk=False
  print "Entries ", t.GetEntries(), Ok
  tf.Close()
  
indexfile.close()

if allOk:
  print "first test all ok"
else:
  print "problem"
  exit(0)
  
print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")
t=ROOT.MEMDataBase(path)
t.AddSample(samplename)
t.PrintStructure()

allOk=True

testfile=open(testlogfile,"r")
testlist=list(testfile)

for test in testlist:
  ll=test.replace("\n","").split(",")
  testrun=int(ll[0])
  testlumi=int(ll[1])
  testevent=int(ll[2])
  testp=float(ll[3])
  
  result=t.GetMEMResult(samplename,testrun,testlumi,testevent)
  resultp=result.p
  if abs(resultp-testp) >= 0.01:
    allOk=False
    print "problem at", ll
    exit(0)
  


  
  

  