#ifndef DATABASEMEMRESULT_H
#define DATABASEMEMRESULT_H

class DataBaseMEMResult{
public:
  
  DataBaseMEMResult();
  ~DataBaseMEMResult();
  
  //likelihood ratio
  double Weight_muRupmuFup;

  //individual signal and background probabilities
  double Weight_muRdownmuFdown;
  double Weight_NNPDFid260067;

  //Integration uncertainties of the probabilities
  double Weight_NNPDFid260005;
//   double p_err_bkg;
//   
//   //Number of permutations per hypothesis 
//   double n_perm_sig;
//   double n_perm_bkg;
};


#endif