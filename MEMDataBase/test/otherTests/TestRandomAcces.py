import ROOT
import sys
rand=ROOT.TRandom3(100)

path="/nfs/dust/cms/user/kelmorab/MEDataBase"
samples=["ttH","ttbar"]
testlogfile="testDataBaseLog.txt"


print ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")
myDB=ROOT.MEMDataBase(path)
for s in samples:
  myDB.AddSample(s)

myDB.PrintStructure()

allOk=True

testfile=open(testlogfile,"r")
testlist=list(testfile)
nEntries=len(testlist)
nTests=100000

print nEntries, "events in test database"
print "testing random access"
##raw_input()


for itest in range(nTests):
  print str(itest)+" / "+str(nTests)
  idx=int(rand.Uniform(0,nEntries))
  test=testlist[idx]
  ll=test.replace("\n","").split(",")
  thissample=ll[0]
  testrun=int(ll[1])
  testlumi=int(ll[2])
  testevent=int(ll[3])
  testp=float(ll[4])
  
  result=myDB.GetMEMResult(thissample,testrun,testlumi,testevent)
  resultp=result.p
  if abs(resultp-testp) >= 0.01:
    allOk=False
    print "problem at", ll
    exit(0)
  else:
    print test


  
  

  