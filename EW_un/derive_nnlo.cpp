#include<TROOT.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1F.h>
#include<TH2F.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;
void derive_nnlo(TString dir, TString file, TString output){
    Int_t LHE_nhad, LHE_nlep;
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
    Float_t LHEPart_eta[9], LHEPart_mass[9], LHEPart_phi[9], LHEPart_pt[9];
    Int_t LHEPart_pdgId[9], LHEPart_status[9];
    UInt_t nLHEPart;
    TChain *chain = new TChain("Events");
    chain->Add(file);
    chain->SetBranchAddress("LHEPart_eta", LHEPart_eta);
    chain->SetBranchAddress("LHEPart_mass", LHEPart_mass);
    chain->SetBranchAddress("LHEPart_phi", LHEPart_phi);
    chain->SetBranchAddress("LHEPart_pt", LHEPart_pt);
    chain->SetBranchAddress("LHEPart_pdgId", LHEPart_pdgId);
    chain->SetBranchAddress("nLHEPart", &nLHEPart);

    Double_t nnlo_wt;
    TFile* nfile = TFile::Open("./NNLO_to_Powheg_Ratios_Summer20UL_CombineYears.root");
    TH2D* h2_nom = (TH2D*) nfile->Get("MATRIX_xsec_central_over_AllYears_Summer20UL_Normalized_Ratios");

    const int nbinsx=14;
	const int nbinsy=40;
	const double ylow=-5.0;
	const double yhigh=5.0;
	double xbins[]={345,360,380,400,450,500,550,600,650,700,750,800,900,1200,2000};
    TH2D *h2_NNLO = new TH2D("h2_NNLO","", nbinsx, xbins, nbinsy, ylow, yhigh);
    TFile* nnlo_file = new TFile(dir+"/NNLO_"+output, "RECREATE");
    
    for(int entry=0; entry<chain->GetEntries(); entry++){
        chain->GetEntry(entry);    
        LHE_nhad = 0;
        LHE_nlep = 0;
        for (int i = nLHEPart-6; i < nLHEPart; i++) {
            if(LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == 4 || LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
                LHE_nhad++;
            if(LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == 3 || LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == -3)
                LHE_nhad++;
            if(LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == 13 || LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13)
                LHE_nlep++;
            if(LHEPart_pdgId[i] == 15 || LHEPart_pdgId[i] == -15)
                LHE_nlep++;
        }
        if (LHE_nhad==2 && LHE_nlep==1) {
            // get information of ttbar process at parton level from LHEPart
            for (int i = 0; i < nLHEPart; i++){
                if(LHEPart_pdgId[i] == 5)
                    index_b = i;
                else if(LHEPart_pdgId[i] == -5)
                    index_antib = i;
                else if(LHEPart_pdgId[i] == 2||LHEPart_pdgId[i] == 4||LHEPart_pdgId[i] == -2||LHEPart_pdgId[i] == -4)
                    index_up = i;
                else if (LHEPart_pdgId[i] == 1||LHEPart_pdgId[i] == 3||LHEPart_pdgId[i] == -1||LHEPart_pdgId[i] == -3)
                    index_down = i;
                else if (LHEPart_pdgId[i] == 11||LHEPart_pdgId[i] == 13||LHEPart_pdgId[i] == 15||LHEPart_pdgId[i]== -11||LHEPart_pdgId[i] == -13||LHEPart_pdgId[i] == -15)
                    index_lep = i;
                else if (LHEPart_pdgId[i] == 12||LHEPart_pdgId[i] == 14||LHEPart_pdgId[i] == 16||LHEPart_pdgId[i] == -12||LHEPart_pdgId[i] == -14||LHEPart_pdgId[i] == -16)
                    index_nu = i;
            }
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b], LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib], LHEPart_mass[index_antib]);
            p4_up.SetPtEtaPhiM(LHEPart_pt[index_up], LHEPart_eta[index_up], LHEPart_phi[index_up], LHEPart_mass[index_up]);
            p4_down.SetPtEtaPhiM(LHEPart_pt[index_down], LHEPart_eta[index_down], LHEPart_phi[index_down], LHEPart_mass[index_down]);
            p4_lep.SetPtEtaPhiM(LHEPart_pt[index_lep], LHEPart_eta[index_lep], LHEPart_phi[index_lep], LHEPart_mass[index_lep]);
            p4_nu.SetPtEtaPhiM(LHEPart_pt[index_nu], LHEPart_eta[index_nu], LHEPart_phi[index_nu], LHEPart_mass[index_nu]);
            if(LHEPart_pdgId[index_lep] > 0){
                p4_antitop = p4_antib + p4_lep + p4_nu;
                p4_top = p4_b + p4_up + p4_down;
                lep_charge = -1;
            } 
            else{
                p4_top = p4_b + p4_lep + p4_nu;
                p4_antitop = p4_antib + p4_up + p4_down;
                lep_charge = 1;
            }
        }
        if(LHE_nhad==0 && LHE_nlep==2){
            // get information of ttbar process at parton level from LHEPart
            for(int i = 0; i < nLHEPart; i++){
                if (LHEPart_pdgId[i] == 5) index_b = i;
                else if (LHEPart_pdgId[i] == -5) index_antib = i;
                else if (LHEPart_pdgId[i] == 11|| LHEPart_pdgId[i] == 13||LHEPart_pdgId[i] == 15)
                    index_lepn = i;
                else if (LHEPart_pdgId[i] == 12 || LHEPart_pdgId[i] == 14 || LHEPart_pdgId[i] == 16)
                    index_nun = i;
                else if (LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13 || LHEPart_pdgId[i] == -15)
                    index_lepp = i;
                else if (LHEPart_pdgId[i] == -12 ||LHEPart_pdgId[i] == -14 || LHEPart_pdgId[i] == -16)
                    index_nup = i;
            } 
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b],LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib],LHEPart_mass[index_antib]);
            p4_lepp.SetPtEtaPhiM(LHEPart_pt[index_lepp], LHEPart_eta[index_lepp],LHEPart_phi[index_lepp], LHEPart_mass[index_lepp]);
            p4_lepn.SetPtEtaPhiM(LHEPart_pt[index_lepn], LHEPart_eta[index_lepn],LHEPart_phi[index_lepn], LHEPart_mass[index_lepn]);
            p4_nup.SetPtEtaPhiM(LHEPart_pt[index_nup], LHEPart_eta[index_nup],LHEPart_phi[index_nup], LHEPart_mass[index_nup]);
            p4_nun.SetPtEtaPhiM(LHEPart_pt[index_nun], LHEPart_eta[index_nun],LHEPart_phi[index_nun], LHEPart_mass[index_nun]);
            p4_top = p4_b + p4_lepp + p4_nun;
            p4_antitop = p4_antib + p4_lepn + p4_nup; 
        }
        if(LHE_nhad==4 && LHE_nlep==0){
            for(int i = 0; i < nLHEPart; i++) {
                if (LHEPart_pdgId[i] == 5) index_b = i;
                else if (LHEPart_pdgId[i] == -5) index_antib = i;
                else if (LHEPart_pdgId[i] == 2|| LHEPart_pdgId[i] == 4)
                    index_up = i;
                else if (LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
                    index_upbar = i;
                else if (LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == 3)
                    index_down = i;
                else if (LHEPart_pdgId[i] == -1 ||LHEPart_pdgId[i] == -3)
                    index_downbar = i;
            } 
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b],LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib],LHEPart_mass[index_antib]);
            p4_up.SetPtEtaPhiM(LHEPart_pt[index_up], LHEPart_eta[index_up],LHEPart_phi[index_up], LHEPart_mass[index_up]);
            p4_upbar.SetPtEtaPhiM(LHEPart_pt[index_upbar], LHEPart_eta[index_upbar],LHEPart_phi[index_upbar], LHEPart_mass[index_upbar]);
            p4_down.SetPtEtaPhiM(LHEPart_pt[index_down], LHEPart_eta[index_down],LHEPart_phi[index_down], LHEPart_mass[index_down]);
            p4_downbar.SetPtEtaPhiM(LHEPart_pt[index_downbar], LHEPart_eta[index_downbar],LHEPart_phi[index_downbar], LHEPart_mass[index_downbar]);
            p4_top = p4_b + p4_up + p4_downbar;
            p4_antitop = p4_antib + p4_upbar + p4_down; 
        }
        top_pt = p4_top.Pt();
        top_eta = p4_top.Eta();
        top_phi = p4_top.Phi();
        top_mass = p4_top.M();
        antitop_pt = p4_antitop.Pt();
        antitop_eta = p4_antitop.Eta();
        antitop_phi = p4_antitop.Phi();
        antitop_mass = p4_antitop.M();
        TLorentzVector p4_top_cms = p4_top;
        TLorentzVector p4_cms_lab = p4_antitop + p4_top;
        TVector3 p3_cms = p4_cms_lab.BoostVector();
        p4_top_cms.Boost(-p3_cms);
        TVector3 p3_top_cms = p4_top_cms.Vect();
        TVector3 p3_cms_lab = p4_cms_lab.Vect();
        ctstar = p3_top_cms.Dot(p3_cms_lab)/(p3_top_cms.Mag() * p3_cms_lab.Mag());
        M_tt_gen = (p4_top + p4_antitop).M();
        delta_rapidity_gen = p4_top.Rapidity() - p4_antitop.Rapidity();
        
        int bin = h2_nom->FindBin(M_tt_gen, ctstar);
        nnlo_wt = h2_nom->GetBinContent(bin);
        h2_NNLO->Fill(M_tt_gen, delta_rapidity_gen, nnlo_wt);
    }
    nnlo_file->cd();
    h2_NNLO->Write();
    nnlo_file->Close();
}