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
void derive_eff(TString inputFile){

    TFile *outfile=new TFile(inputFile,"update");
    
    TH2F* h2_dcEff_Num_b = (TH2F*)outfile->Get("h2_dcEff_Num_b");
    TH2F* h2_dcEff_Num_c = (TH2F*)outfile->Get("h2_dcEff_Num_c");
    TH2F* h2_dcEff_Num_udsg = (TH2F*)outfile->Get("h2_dcEff_Num_udsg");
    TH2F* h2_djEff_Num_b = (TH2F*)outfile->Get("h2_djEff_Num_b");
    TH2F* h2_djEff_Num_c = (TH2F*)outfile->Get("h2_djEff_Num_c");
    TH2F* h2_djEff_Num_udsg = (TH2F*)outfile->Get("h2_djEff_Num_udsg");

    TH2F* h2_dcEff_Denom_b = (TH2F*)outfile->Get("h2_dcEff_Denom_b");
    TH2F* h2_dcEff_Denom_c = (TH2F*)outfile->Get("h2_dcEff_Denom_c");
    TH2F* h2_dcEff_Denom_udsg = (TH2F*)outfile->Get("h2_dcEff_Denom_udsg");
    TH2F* h2_djEff_Denom_b = (TH2F*)outfile->Get("h2_djEff_Denom_b");
    TH2F* h2_djEff_Denom_c = (TH2F*)outfile->Get("h2_djEff_Denom_c");
    TH2F* h2_djEff_Denom_udsg = (TH2F*)outfile->Get("h2_djEff_Denom_udsg");

    
    TH2F* h2_dcEff_b = (TH2F*)h2_dcEff_Num_b->Clone();
    TH2F* h2_dcEff_c = (TH2F*)h2_dcEff_Num_c->Clone();
    TH2F* h2_dcEff_udsg = (TH2F*)h2_dcEff_Num_udsg->Clone();
    TH2F* h2_djEff_b = (TH2F*)h2_djEff_Num_b->Clone();
    TH2F* h2_djEff_c = (TH2F*)h2_djEff_Num_c->Clone();
    TH2F* h2_djEff_udsg = (TH2F*)h2_djEff_Num_udsg->Clone();

    h2_dcEff_b->SetName("h2_dcEff_b");
    h2_dcEff_c->SetName("h2_dcEff_c");
    h2_dcEff_udsg->SetName("h2_dcEff_udsg");
    h2_djEff_b->SetName("h2_djEff_b");
    h2_djEff_c->SetName("h2_djEff_c");
    h2_djEff_udsg->SetName("h2_djEff_udsg");

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
    outfile->Close();
}