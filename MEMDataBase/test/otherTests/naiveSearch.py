import ROOT
import sys
from array import array

rand=ROOT.TRandom3(100)


filelist=sys.argv[1:]

lumi=array("i",[0])
event=array("i",[0])
p=array("f",[0])

for itest in range(10):
  print " test ", itest
  ievt=int(rand.Uniform(10,9999999))
  print ievt
  found=False
  
  for f in filelist:
    #print f
    infile=ROOT.TFile(f,"READ")
    intree=infile.Get("MVATree")
    
    #intree.SetBranchAddress("lumi",lumi)
    intree.SetBranchAddress("p",p)
    intree.SetBranchAddress("event",event)
    
    nenetries=intree.GetEntries()
    
    for ie in range(nenetries):
      #if ie%1000==0:
	#print "at event ", ie
      intree.GetEntry(ie)
      if event[0]==ievt:
        print "foudn event ", ievt, p[0]
        found=True
        break
    if found==True:
      infile.Close()
      break
      
    infile.Close()