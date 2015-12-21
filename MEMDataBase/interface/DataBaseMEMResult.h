#ifndef DATABASEMEMRESULT_H
#define DATABASEMEMRESULT_H

class DataBaseMEMResult{
public:
  
  DataBaseMEMResult();
  ~DataBaseMEMResult();
  
  //likelihood ratio
  double p;

  //individual signal and background probabilities
  double p_sig;
  double p_bkg;

  //Integration uncertainties of the probabilities
  double p_err_sig;
  double p_err_bkg;
  
  //Number of permutations per hypothesis 
  double n_perm_sig;
  double n_perm_bkg;
};


#endif