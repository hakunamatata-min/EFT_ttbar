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
//int main(){
void eff_get(TString indir, TString inputfile, int year){
    TString output = "btageff_"+inputfile+".root";
    /*TString inputfile[]={
        "TTsemi.root",
        "TTdilepton.root",
        "TThad.root",
    };*/
    Float_t dj_nums[] = {0.2589,0.2489,0.3040,0.2783};
    Float_t dc_nums[] = {0.6001,0.5847,0.4506,0.4168};
    TChain *mytree = new TChain("Events");
    mytree->Add(indir+"/"+inputfile+"*.root");
    TFile *outfile=new TFile(indir+"/"+output,"RECREATE");
    Float_t ptbins[]={20,30,50,70,100,140,200,300,600,10000};
    Float_t etabins[]={0., 0.6, 1.2, 2.1, 2.5};
    Float_t dc_vaule = dc_nums[year-2015];
    Float_t dj_vaule = dj_nums[year-2015];
    TH2F *h2_dcEff_Denom_b = new TH2F("h2_dcEff_Denom_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_Num_b = new TH2F("h2_dcEff_Num_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_Denom_c = new TH2F("h2_dcEff_Denom_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_Num_c = new TH2F("h2_dcEff_Num_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_Denom_udsg = new TH2F("h2_dcEff_Denom_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_Num_udsg = new TH2F("h2_dcEff_Num_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_b = new TH2F("h2_dcEff_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_c = new TH2F("h2_dcEff_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_dcEff_udsg = new TH2F("h2_dcEff_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Denom_b = new TH2F("h2_djEff_Denom_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Num_b = new TH2F("h2_djEff_Num_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Denom_c = new TH2F("h2_djEff_Denom_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Num_c = new TH2F("h2_djEff_Num_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Denom_udsg = new TH2F("h2_djEff_Denom_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_Num_udsg = new TH2F("h2_djEff_Num_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_b = new TH2F("h2_djEff_b", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_c = new TH2F("h2_djEff_c", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    TH2F *h2_djEff_udsg = new TH2F("h2_djEff_udsg", ";#eta;p_{T} [GeV]", 4, etabins, 9, ptbins);
    Float_t jet_pt[35], jet_eta[35], jet_dj[35], jet_dc[35];
    UInt_t njet;
    Int_t jet_fla[35];
    mytree->SetBranchAddress("nJet", &njet);
    mytree->SetBranchAddress("Jet_pt", jet_pt);
    mytree->SetBranchAddress("Jet_eta", jet_eta);
    mytree->SetBranchAddress("Jet_btagDeepB", jet_dc);
    mytree->SetBranchAddress("Jet_btagDeepFlavB", jet_dj);
    mytree->SetBranchAddress("Jet_partonFlavour", jet_fla);
    for(int i=0; i<mytree->GetEntries(); i++){
        mytree->GetEntry(i);
        for(int j=0; j<njet; j++){
            if(jet_pt[j]>20 && jet_pt[j]<10000 && abs(jet_eta[j])<2.5){
                if(jet_fla[j]==5 || jet_fla[j]==-5){
                    h2_dcEff_Denom_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                    h2_djEff_Denom_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                    if(jet_dc[j]>dc_vaule){
                        h2_dcEff_Num_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_dcEff_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                    if(jet_dj[j]>dj_vaule){
                        h2_djEff_Num_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_djEff_b->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                }
                else if(jet_fla[j]==4 || jet_fla[j]==-4){
                    h2_dcEff_Denom_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                    h2_djEff_Denom_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                    if(jet_dc[j]>dc_vaule){
                        h2_dcEff_Num_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_dcEff_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                    if(jet_dj[j]>dj_vaule){
                        h2_djEff_Num_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_djEff_c->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                }
                else{
                    h2_dcEff_Denom_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                    h2_djEff_Denom_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                    if(jet_dc[j]>dc_vaule){
                        h2_dcEff_Num_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_dcEff_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                    if(jet_dj[j]>dj_vaule){
                        h2_djEff_Num_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                        h2_djEff_udsg->Fill(abs(jet_eta[j]), jet_pt[j]);
                    }
                }
            }
        }
    }
    h2_djEff_udsg->Divide(h2_djEff_Denom_udsg);
    h2_djEff_c->Divide(h2_djEff_Denom_c);
    h2_djEff_b->Divide(h2_djEff_Denom_b);
    h2_dcEff_udsg->Divide(h2_dcEff_Denom_udsg);
    h2_dcEff_c->Divide(h2_dcEff_Denom_c);
    h2_dcEff_b->Divide(h2_dcEff_Denom_b);
    
    outfile->cd();
    h2_djEff_udsg->Write();
    h2_djEff_b->Write();
    h2_djEff_c->Write();
    h2_dcEff_udsg->Write();
    h2_dcEff_b->Write();
    h2_dcEff_c->Write();

    h2_djEff_Denom_b->Write();
    h2_djEff_Denom_udsg->Write();
    h2_djEff_Denom_c->Write();
    h2_dcEff_Denom_b->Write();
    h2_dcEff_Denom_c->Write();
    h2_dcEff_Denom_udsg->Write();
    
    h2_djEff_Num_udsg->Write();
    h2_djEff_Num_b->Write();
    h2_djEff_Num_c->Write();
    h2_dcEff_Num_udsg->Write();
    h2_dcEff_Num_b->Write();
    h2_dcEff_Num_c->Write();
    
    outfile->Close();
}