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

void derive_mu(TString dir, TString file, TString output){
    TFile* pu_file = new TFile(dir+"/Mu_"+output, "RECREATE"); 

    TH1D* mu_MC= new TH1D("h1_mu_MC", "", 99, 0 ,99);
    TChain* rawtree = new TChain("Events");
    rawtree->Add(file);
    rawtree->Draw("Pileup_nPU>>h1_mu_MC");

    pu_file->cd();
    mu_MC->Write();    
    pu_file->Close();
}