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
#include<fstream>
#include<iostream>
using namespace std;
void Floor(TH1D* hist){
	for(int i=0;i<hist->GetNbinsX();i++){
        if(!(hist->GetBinContent(i+1)>1.E-6)){
            hist->SetBinContent(i+1, 1.E-6);
        }
	}
}
void derive_qcd_nom(TString qcd_dir, TString cut){    
    TFile* file = new TFile(qcd_dir+"/QCD_"+cut+".root", "recreate");
    TFile* qcd_file = TFile::Open(qcd_dir + "/QCD_"+cut+"_C.root");
    TH1D* hist = (TH1D*)qcd_file->Get("QCD");
    hist->SetName("QCD_sub");
    Floor(hist);
    file->cd();
    hist->Write();
    delete hist;
    file->Close();
}
void derive_qcd_sys(TString qcd_dir, TString cut){
    TString qcd_name[] = {"_C", "_B", "_D"}; 
    TFile* file = new TFile(qcd_dir+"/QCD_"+cut+".root", "update");
    TH1D* hist_qcds[3];
    TH1D* hist_num[3];
    for(int i=0; i<3; i++){
        TFile* qcd_file = TFile::Open(qcd_dir + "/QCD_"+cut+qcd_name[i]+".root");
        if(qcd_name[i] == "_B"){
            hist_qcds[i] = (TH1D*)qcd_file->Get("QCD_MC");
            hist_num[i] = (TH1D*)qcd_file->Get("QCD_CG_MC");
        }
        else{
            hist_qcds[i] = (TH1D*)qcd_file->Get("QCD");
            hist_num[i] = (TH1D*)qcd_file->Get("QCD_CG");
        }
        Floor(hist_qcds[i]);
        Floor(hist_num[i]);
    }
    
    //double qcdn = (hist_num[1]->GetSumOfWeights()/hist_num[2]->GetSumOfWeights())*(hist_num[0]->GetSumOfWeights());
    //qcdn = (qcdn - hist_qcds[0]->GetSumOfWeights())/hist_qcds[0]->GetSumOfWeights();
    /*double b_num = hist_qcds[1]->GetSumOfWeights();
    double d_num = hist_qcds[1]->GetSumOfWeights();*/
    hist_qcds[1]->Scale(1.0/hist_qcds[1]->GetSumOfWeights());
    hist_qcds[2]->Scale(1.0/hist_qcds[2]->GetSumOfWeights());
    hist_qcds[1]->Divide(hist_qcds[2]);
    TH1D* hist_up = (TH1D*)hist_qcds[0]->Clone();
    hist_up->SetName("QCD_qcdsUp_sub");
    TH1D* hist_dn = (TH1D*)hist_qcds[0]->Clone();
    hist_dn->SetName("QCD_qcdsDown_sub");
    hist_up->Multiply(hist_qcds[1]);
    hist_dn->Divide(hist_qcds[1]);
    file->cd();
    hist_up->Write();
    hist_dn->Write();
    delete hist_qcds[0];delete hist_qcds[1];delete hist_qcds[2];
    delete hist_num[0];delete hist_num[1];delete hist_num[2];
    delete hist_up;delete hist_dn;
    file->Close();
}

void add_qcd(int i, int j){
    TString qcd_dir = "/home/yksong/code/ttbar/output/2018/QCD_ES/QCD_em";
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString div_name[4] = {"_y0", "_y1", "_y2", "_y3"};
    derive_qcd_nom(qcd_dir, cutsName[i]+div_name[j]);
    derive_qcd_sys(qcd_dir, cutsName[i]+div_name[j]);

} 
