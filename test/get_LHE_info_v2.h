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

UInt_t nLHEPart;
Int_t LHE_n;
Int_t LHEPart_pdgId[9];
Float_t LHEPart_eta[9], LHEPart_mass[9], LHEPart_phi[9], LHEPart_pt[9];
int index_b, index_antib, index_antilep, index_lep, index_nub, index_nuantib,index_q1, index_q2, index_nu, index_q3, index_q4;
TLorentzVector p4_b, p4_antib, p4_top, p4_antitop, p4_lep, p4_antilep, p4_nub, p4_nuantib;
TLorentzVector p4_nu, p4_q1, p4_q2, p4_q3, p4_q4;
Float_t top_pt, top_eta, top_phi, top_mass; 
Float_t antitop_pt, antitop_eta, antitop_phi, antitop_mass;
int index_lepn, index_nun, index_lepp, index_nup, index_upbar, index_downbar, index_up, index_down;
TLorentzVector p4_lepn, p4_nun, p4_lepp, p4_nup, p4_downbar, p4_upbar, p4_down, p4_up;