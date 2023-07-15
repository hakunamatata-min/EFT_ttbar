#include<TROOT.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1F.h>
#include<TH2F.h>
#include<iostream>
using namespace std;
class RECO{
private:
    int num_jets;
    Int_t lep_charge;
    static TLorentzVector* mom_jets;
    static TLorentzVector mom_lep;
    int jets_index[4]; //0: b_lep, 1: b_had, 2: l_jet1, 3: l_jet2
    int chi2_index[4]; //0: b_lep, 1: b_had, 2: l_jet1, 3: l_jet2 //only for jet_num>=4
    int* btag_sort(Float_t* btag_score, int *index);
    void chi2_sort(int* index);
    void like_sort(int* index);
    static Double_t likelihood(Double_t *pz, Double_t* pars);
    static Double_t likelihood_3jet(Double_t *pz, Double_t* pars);
    void reco_top(); 
    void reco_chi();
public:
    static Double_t nu_px;
    static Double_t nu_py;
    static Double_t nu_pz;
    Double_t mass_bjj, mass_jj, mass_lb;
    Double_t mass_tlep, mass_thad, mass_whad, mass_wlep;
    Double_t mass_t, mass_at;
    Double_t rectop_pt, mass_tt, rapidity_tt, recantitop_pt;
    Double_t like;
    Double_t chi;
    bool diff();
    RECO(int jet_num, TLorentzVector* mom_alljets, TLorentzVector mom_lepton, Int_t lep_c, Float_t* btag_score, Float_t MET_pt, Float_t MET_phi);
    ~RECO();
};
Double_t RECO::nu_px=0;
Double_t RECO::nu_py=0;
Double_t RECO::nu_pz=0;
TLorentzVector* RECO::mom_jets = NULL;
TLorentzVector RECO::mom_lep = TLorentzVector(0,0,0,0);