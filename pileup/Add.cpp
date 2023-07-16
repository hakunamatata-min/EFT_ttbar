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
void Add(int year){
    TString files[] = { "Mu_TTToSemiLeptonic_TuneCP5_13TeV-powheg.root",
                        "Mu_TTTo2L2Nu_TuneCP5_13TeV-powheg.root",
                        "Mu_TTToHadronic_TuneCP5_13TeV-powheg.root",

                        "Mu_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        "Mu_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM.root",                           
                        "Mu_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                    
                        "Mu_ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo.root",
                        "Mu_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                        "Mu_ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                        "Mu_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",
                        "Mu_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",                                                                               
                        
                        "Mu_W1JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                        "Mu_W2JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                        "Mu_W3JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                        "Mu_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM.root"};
    TH1D* h2;
    TFile* file;
    TFile* mu_file = new TFile(Form("./%d/Mu_MC.root", year), "RECREATE");
    TH1D* mu_hist = new TH1D("mu_MC", "", 99, 0 ,99);
    for(int i=0; i<20; i++){
        file = TFile::Open(Form("/home/yksong/code/output/pileup/%d", year)+files[i]+".root");
        h2 = (TH1D*)file->Get("h1_mu_MC");
        mu_hist->Add(h2);
        delete h2;
        delete file;
    }
    mu_file->cd();
    mu_hist->Write();
    mu_file->Close();
}