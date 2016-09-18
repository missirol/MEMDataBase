import ROOT
ROOT.gDirectory.cd('PyROOT:/')
from array import array

dataBaseDirectory="/nfs/dust/cms/user/kelmorab/MEDataBase/"
sample="ttbar"
filelist=[]


rand=ROOT.TRandom3(0)

RelRun=1

minLumi=100
maxLumi=200

minEvent=10
maxEvent=9999999

maxEntries=10000

thisTree=ROOT.TTree("MVATree","MVATree")

p=array("f",[0])
p_sig=array("f",[0])
p_bkg=array("f",[0])
p_err_s=array("f",[0])
p_err_b=array("f",[0])
n_perm_s=array("f",[0])
n_perm_b=array("f",[0])

lumi=array("i",[0])
event=array("i",[0])
run=array("i",[0])

thisTree.Branch("run",run,"run/I")
thisTree.Branch("event",event,"event/I")
thisTree.Branch("lumi",lumi,"lumi/I")

thisTree.Branch("p",p,"p/F")
thisTree.Branch("p_sig",p_sig,"p_sig/F")
thisTree.Branch("p_bkg",p_bkg,"p_bkg/F")
thisTree.Branch("p_err_sig",p_err_s,"p_err_sig/F")
thisTree.Branch("p_err_bkg",p_err_b,"p_err_bkg/F")
thisTree.Branch("n_perm_sig",n_perm_s,"n_perm_sig/F")
thisTree.Branch("n_perm_bkg",n_perm_b,"n_perm_bkg/F")

#clonedTree=thisTree.Clone()

firstEvent=minEvent

for ievt in range(minEvent, maxEvent):
  if ievt%1000==0:
    print ievt
  #lumi[0]=int(rand.Uniform(minLumi,maxLumi))
  lumi[0]=101
  run[0]=RelRun
  event[0]=ievt
  
  p[0]=rand.Rndm()
  p_sig[0]=rand.Rndm()
  p_bkg[0]=rand.Rndm()
  p_err_s[0]=rand.Rndm()
  p_err_b[0]=rand.Rndm()
  n_perm_s[0]=rand.Rndm()
  n_perm_b[0]=rand.Rndm()
  
  thisTree.Fill()
  
  if ievt-firstEvent >= maxEntries:
    thisFileName=sample+"_"+str(RelRun)+"_"+str(minLumi)+"_"+str(maxLumi)+"_"+str(firstEvent)+"_"+str(ievt)+"_.root"
    filelist.append(thisFileName)
    firstEvent=ievt+1
    thisFile=ROOT.TFile(dataBaseDirectory+thisFileName,"RECREATE")
    thisFile.cd()
    thisTree.Write()
    thisFile.Close()
    print "print writing ", thisFileName
    
    thisTree=ROOT.TTree("MVATree","MVATree")
    
    thisTree.Branch("run",run,"run/I")
    thisTree.Branch("event",event,"event/I")
    thisTree.Branch("lumi",lumi,"lumi/I")

    thisTree.Branch("p",p,"p/F")
    thisTree.Branch("p_sig",p_sig,"p_sig/F")
    thisTree.Branch("p_bkg",p_bkg,"p_bkg/F")
    thisTree.Branch("p_err_sig",p_err_s,"p_err_sig/F")
    thisTree.Branch("p_err_bkg",p_err_b,"p_err_bkg/F")
    thisTree.Branch("n_perm_sig",n_perm_s,"n_perm_sig/F")
    thisTree.Branch("n_perm_bkg",n_perm_b,"n_perm_bkg/F")
    
    
    
thisFileName=sample+"#"+str(RelRun)+"#"+str(minLumi)+"#"+str(maxLumi)+"#"+str(firstEvent)+"#"+str(ievt)+"#.root"
filelist.append(thisFileName)
thisFile=ROOT.TFile(dataBaseDirectory+thisFileName,"RECREATE")
thisFile.cd()
thisTree.Write()
thisFile.Close()


indexfile=open(dataBaseDirectory+sample+"_index.txt","w")
for l in filelist:
  indexfile.write(l+"\n")
indexfile.close()

## continue this
















  
