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

  //Systematic variations for the likelihood ratio

  double p_AbsoluteStatUp;
  double p_AbsoluteStatDown;
  double p_AbsoluteScaleUp;
  double p_AbsoluteScaleDown;
  double p_AbsoluteFlavMapUp;
  double p_AbsoluteFlavMapDown;
  double p_AbsoluteMPFBiasUp;
  double p_AbsoluteMPFBiasDown;
  double p_FragmentationUp;
  double p_FragmentationDown;
  double p_SinglePionECALUp;
  double p_SinglePionECALDown;
  double p_SinglePionHCALUp;
  double p_SinglePionHCALDown;
  double p_FlavorQCDUp;
  double p_FlavorQCDDown;
  double p_TimePtEtaUp;
  double p_TimePtEtaDown;
  double p_RelativeJEREC1Up;
  double p_RelativeJEREC1Down;
  double p_RelativeJEREC2Up;
  double p_RelativeJEREC2Down;
  double p_RelativeJERHFUp;
  double p_RelativeJERHFDown;
  double p_RelativePtBBUp;
  double p_RelativePtBBDown;
  double p_RelativePtEC1Up;
  double p_RelativePtEC1Down;
  double p_RelativePtEC2Up;
  double p_RelativePtEC2Down;
  double p_RelativePtHFUp;
  double p_RelativePtHFDown;
  double p_RelativeFSRUp;
  double p_RelativeFSRDown;
  double p_RelativeStatFSRUp;
  double p_RelativeStatFSRDown;
  double p_RelativeStatECUp;
  double p_RelativeStatECDown;
  double p_RelativeStatHFUp;
  double p_RelativeStatHFDown;
  double p_PileUpDataMCUp;
  double p_PileUpDataMCDown;
  double p_PileUpPtRefUp;
  double p_PileUpPtRefDown;
  double p_PileUpPtBBUp;
  double p_PileUpPtBBDown;
  double p_PileUpPtEC1Up;
  double p_PileUpPtEC1Down;
  double p_PileUpPtEC2Up;
  double p_PileUpPtEC2Down;
  double p_PileUpPtHFUp;
  double p_PileUpPtHFDown;
  double p_RelativeBalUp;
  double p_RelativeBalDown;
  double p_JERUp;
  double p_JERDown;

  //Integration uncertainties of the probabilities
  double p_err_sig;
  double p_err_bkg;
  
  //Number of permutations per hypothesis 
  double n_perm_sig;
  double n_perm_bkg;
};


#endif
