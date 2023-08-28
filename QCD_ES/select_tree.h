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

class read_object{
public:
    read_object(TString input, Bool_t type);
    UInt_t nj, ne, nm, ng;
};

class select_tree{
private:
    TString input;
    int year;
    int category;
    bool type; //0:MC; 1:data
    TChain* chain;
    TFile* output;
    Float_t btag_criteria;
    TLorentzVector *mom_jets, mom_lep;
    UInt_t jet_num;
    UInt_t nBtag;
    Bool_t lep_flavour;
    Int_t lep_c;
    Float_t MET_pt, MET_phi;
    UInt_t nMuon, nElectron, nJet, nGenJet;
    UInt_t nm, ne, nj, ng;
    Float_t *Electron_eta, *Electron_mass, *Electron_pt, *Electron_phi;
    Float_t *Muon_mass, *Muon_phi, *Muon_pt, *Muon_eta;
    Int_t *Muon_charge, *Electron_charge;
    Float_t *Jet_btagDeepB, *Jet_btagDeepFlavB, *Jet_eta, *Jet_mass, *Jet_phi, *Jet_pt;
    Float_t *GenJet_pt, *GenJet_eta, *GenJet_phi, *GenJet_mass;
    Int_t *Jet_partonFlavour, *Jet_hadronFlavour;
    Float_t *Muon_pfRelIso04_all;
    Int_t *Electron_cutBased, *Jet_jetId;
    Bool_t *Muon_tightId, *Muon_looseId;
    UChar_t *Muon_highPtId;
    Float_t *Electron_deltaEtaSC, *Electron_dxy, *Electron_dz;
    Bool_t HLT_Ele32_WPTight_Gsf, HLT_IsoMu27, HLT_IsoMu24, HLT_Mu50, HLT_OldMu100, HLT_TkMu100, HLT_Ele115_CaloIdVT_GsfTrkIdT;
    Bool_t HLT_TkMu50, HLT_Photon175, HLT_IsoTkMu24, HLT_Ele27_WPTight_Gsf, HLT_Ele35_WPTight_Gsf, HLT_Photon200;
    Bool_t HLT_Mu27, HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
    ULong64_t event;
    Float_t Generator_weight;
    UInt_t luminosityBlock, run;
    Float_t LHEPart_eta[9], LHEPart_mass[9], LHEPart_phi[9], LHEPart_pt[9];
    Int_t LHEPart_pdgId[9], LHEPart_status[9];
    UInt_t nLHEPart;
    Int_t Pileup_nPU, PV_npvsGood;

    Float_t L1PreFiringWeight_Nom, L1PreFiringWeight_Up, L1PreFiringWeight_Dn; 
    Bool_t Flag_met[11];
    UInt_t nFlag_met;
    Bool_t met_match;

    Float_t lepton_mass, lepton_phi, lepton_eta, lepton_pt;
    Float_t *jet_btagDeepB, *jet_btagDeepFlavB, *jet_eta, *jet_pt, *jet_phi, *jet_mass;
    Int_t *jet_partonFlavour, *jet_hadronFlavour;
    Float_t max_score;
    Float_t MtW;
    Float_t electron_deltaEtaSC;
    Float_t rectop_pt, mass_tt, rapidity_tt, recantitop_pt;
    Float_t mass_tlep, mass_thad, neutrino_pz;
    Float_t mass_wlep, mass_whad, mass_t, mass_at;
    Float_t mass_bjj, mass_jj, mass_lb;
    Double_t like, chi;
    Float_t M_tt_gen, delta_rapidity_gen;
    Int_t lep_charge;
    Float_t top_pt, top_eta, top_phi, top_mass; 
    Float_t antitop_pt, antitop_eta, antitop_phi, antitop_mass;
    int index_b, index_antib, index_up, index_down, index_lep, index_nu;
    TLorentzVector p4_b, p4_antib, p4_up, p4_down, p4_lep, p4_nu, p4_top,p4_antitop;
    int index_lepn, index_nun, index_lepp, index_nup;
    TLorentzVector  p4_lepn, p4_nun, p4_lepp, p4_nup;
    int  index_upbar, index_downbar;
    TLorentzVector  p4_upbar, p4_downbar;
    Float_t ctstar;
    int index;
    Int_t LHE_nhad, LHE_nlep;
    Int_t *Electron_vidNestedWPBitmap;
    Float_t *Electron_pfRelIso03_all;
    Bool_t select_jet();
    Bool_t select_lep();
    void loop(TTree* tree1, TTree* tree2);
    Bool_t is_lep_from_jet(TLorentzVector mom_lep);
    void read_LHE();
    Bool_t loose_noiso(Int_t i);
    Bool_t tight_noiso(Int_t i);
    Int_t iso_select(Int_t i);
public:
    select_tree(TString inputfile, TString outputFile, int s_year, int s_cate, bool s_type, int num_j, int num_e, int num_m, int num_g = 0);//type: 0:MC; 1:data
    void write();
    ~select_tree(); 
};
