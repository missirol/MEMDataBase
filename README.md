MEMDataBase
===================================

* Database to store MEM output values.
* The idea is to calculate the MEM once for each event and store it here with a unique sample, run, lumi, event identifier.
* Then the MEM can be retrived using these identifiers.
* It is important to quickly find the correct event out of O(100M) other events while keeping the memory footprint low.
* For this the database is hierarchically structured according to sample, run, lumi, event. And restructured when adding additional events.
* Because of this creation is somewhat slow.
* There are certainly better database solutions, but i am no specialist and wanted to learn something doing it this way :D

Setup
-----

This is setup as a CMSSW package so you need CMSSW. But the code actually only need ROOT and can be used without CMSSW.
Last tested with CMSSW_8_0_19
~~~
cd $CMSSW_BASE/src
git clone https://github.com/kit-cn-cms/MEMDataBase.git
scram b 
~~~

Usage
-----
The database will be compiled as library which can be importet to python using 
~~~
import ROOT
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")
~~~

Writing
-----
Initialize the Database to a writable path (that already exists)
~~~
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")
~~~
Create the top level hierarchy for a specific sample and systematic. 
Use an unique identifier which CANNOT contain underscores.
e.g. ttbarInclusiveJESDOWN 
~~~
myDataBase.AddSample("ttbarInclusiveJESDOWN")
~~~
Similarly for other samples. You can also later add eventsand other samples.   
But you can not replace already existing events.   

Now add events with identifiers and MEM information
~~~
myDataBase.AddEvent( sampleName, run, lumi, event, p, p_sig, p_bkg, p_err_sig, p_err_bkg, n_perm_sig, n_perm_bkg )
~~~
Finally save the database
~~~
myDataBase.SaveDataBase()
~~~

More examples in https://github.com/kit-cn-cms/MEMDataBase/tree/master/MEMDataBase/test

Reading
-----
Initialize database
~~~
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/kelmorab/MEDataBase")
~~~
Add samples with identifiers
~~~
myDataBase.AddSample("ttHbbJESUP","ttHbbJESUP_index.txt")
~~~
The index file contains a list of internal files and is created automatically with the name "sample_index.txt" during the write step. Just use the argument like this.

Retrieve MEM 
~~~
result=myDataBase.GetMEMResult("ttHbbJESUP", run, lumi, event)
~~~
result is a simple class https://github.com/kit-cn-cms/MEMDataBase/blob/master/MEMDataBase/interface/DataBaseMEMResult.h   
Get MEM p
~~~
result.p
~~~
More examples in https://github.com/kit-cn-cms/MEMDataBase/tree/master/MEMDataBase/test

Usage in CMSSW C++
-----
You can use the database classes directly in C++ code in CMSSW by calling this class   
https://github.com/kit-cn-cms/MEMDataBase/blob/master/MEMDataBase/interface/MEMDataBase.h

Usage without CMSSW
-----
The database only needs ROOT and can be used without CMSSW.
For this you need to include the header files and maybe need to change include paths in the database code.
Then compile and link as for other custom C++ classes.
But usage in CMSSW is the easiest way.
