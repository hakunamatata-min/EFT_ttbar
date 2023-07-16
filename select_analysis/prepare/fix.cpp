#include<TROOT.h>
#include<TStyle.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TLegend.h>
#include<TF1.h>
#include<TH1D.h>
#include<TCanvas.h>
#include<TH2F.h>
#include<THStack.h>
#include<string.h>
#include<TGraph.h>
#include<TGraphAsymmErrors.h>
#include<fstream>
#include<iostream>
#include<RooFit.h>
#include<RooRealVar.h>
#include<TKey.h>
using namespace std;
void Floor(TH1D* hist){
    for (int i=0;i<hist->GetNbinsX();i++){
        if(!(hist->GetBinContent(i+1)>1.E-6)){
            hist->SetBinContent(i+1, 1.E-6);
        }
    }
}
void test(TString cut_name){
    TFile *file = new TFile("ttbar_"+cut_name+".root", "update");
    TString qcd_dir = "/home/yksong/code/ttbar/output/2018/QCD_ES/QCD";
    //TString cut_name = "M_4jets";
    TFile* qcd_file = TFile::Open(qcd_dir + "/QCD_"+cut_name+"_C.root");
    int bin_num = 23;
    //TH1D* hist = new TH1D("QCD_1", "", bin_num, 0, bin_num);
    TH1D* hist = (TH1D*)qcd_file->Get("QCD");
    Floor(hist);
    file->cd();
    hist->Write();
    double yield = hist->GetSumOfWeights();
    delete hist;
    file->Close();
    cout<<std::fixed<<std::setprecision(3)<<cut_name<<" "<<yield<<endl;
}
void fix_pre(TString cut_name){
    TString path = "../2018/";
    TString sys[] = {"jes","jer","unclus","SF_lepton","SF_btag","L1PF","muR1","muF1","muR2","muF2","muR3","muF3","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un","pdf", "qcds"};
    TString process[]={"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "DYJets","STop", "WJets", "QCD", "data_obs"};
    TFile* file = TFile::Open(path+"datacard/"+"ttbar_"+cut_name+".root");
    TFile* fix_file = new TFile("ttbar_"+cut_name+".root", "recreate");
    TH1D *hmc1, *hmc2, *hmc3;
    for(int c=0; c<9; c++){
        if(c != 7){
            hmc1 = (TH1D*)file->Get(process[c]);
            fix_file->cd();
            hmc1->Write();
            delete hmc1;
        }
        for(int s=0; s<21; s++){
            hmc2 = (TH1D*)file->Get(process[c]+"_"+sys[s]+"Up");
            hmc3 = (TH1D*)file->Get(process[c]+"_"+sys[s]+"Down");
            if(hmc2 == NULL){
                delete hmc2; delete hmc3;
                continue;
            }
            fix_file->cd();
            hmc2->Write();
            hmc3->Write();
            delete hmc2; delete hmc3;
        }
    }
    fix_file->Close();
    test(cut_name);
}
void fix(){
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    for(int i=0; i<4; i++)
        fix_pre(cutsName[i]); 
}