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

void derive_pu(int year){
    TString file_name[3] = {"-69200ub-99bins.root", "-66000ub-99bins.root", "-72400ub-99bins.root"};
    TString pu_name[3] = {"pu_nom", "pu_down", "pu_up"};
    TString files = Form("/eos/user/y/yuekai/ttbar/select/%d/MC/*", year);
    //TString files = "./new_TT*";
    TString path = Form("./%d/", year);
    TH1D *pu_h1[3], *mu_data[3];
    TFile* file[3];
    TChain* rawtree = new TChain("rawtree");
    rawtree->Add(files);
    TH1D* mu_MC= new TH1D("mu_MC", "", 99, 0 ,99);
    rawtree->Draw("Pileup_nPU>>mu_MC");
    mu_MC->Scale(1.0/mu_MC->GetSumOfWeights());

    TFile* nfile = new TFile(Form("./%d/pu_weight.root", year), "recreate");
    nfile->cd();
    mu_MC->Write();
    for(int i=0; i<3; i++){
        pu_h1[i] = new TH1D(pu_name[i], "", 99, 0 ,99);
        file[i] = TFile::Open(Form("./%d/PileupHistogram-goldenJSON-13tev-%d",year,year)+file_name[i]);
        mu_data[i] = (TH1D*)file[i]->Get("pileup");
        mu_data[i]->SetName(Form("mu_data_%d", i));
        mu_data[i]->Scale(1.0/mu_data[i]->GetSumOfWeights());
        for(int bin=0; bin<99; bin++){
            if(mu_MC->GetBinContent(bin+1)>0){
                pu_h1[i]->SetBinContent(bin+1, mu_data[i]->GetBinContent(bin+1)/mu_MC->GetBinContent(bin+1));
            }
            else
                pu_h1[i]->SetBinContent(bin+1, 1.0);
        }
        nfile->cd();
        pu_h1[i]->Write();
    }
    nfile->Close();
}