#ifndef DATABASEMEMRESULT_H
#define DATABASEMEMRESULT_H

#include <vector>
#include "TString.h"
class DataBaseMEMResult{
public:
  
  DataBaseMEMResult();
  DataBaseMEMResult(const std::vector<TString>& mem_strings_);
  ~DataBaseMEMResult();
  
  //likelihood ratio
  double p;
  std::vector<double> p_vec;

  //individual signal and background probabilities
  double p_sig;
  double p_bkg;

  //Integration uncertainties of the probabilities
  double p_err_sig;
  double p_err_bkg;
  
  //Number of permutations per hypothesis 
  double n_perm_sig;
  double n_perm_bkg;
  
  //Btag likelihood ratios
  double blr_eth;
  double blr_eth_transformed;
};


#endif
