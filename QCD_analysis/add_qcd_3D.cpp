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
#include<TH3D.h>
#include<TCanvas.h>
#include<TH2F.h>
#include<THStack.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;

void derive_qcd_nom(TString dir, TString cut){
    TFile* file = new TFile(dir+"datacard/ttbar_"+cut+".root", "update");
    TFile* qcd_file = TFile::Open(dir+"QCD/QCD_"+cut+"_C.root");
    TH3D* hist = (TH3D*)qcd_file->Get("QCD_derived");
    hist->SetName("QCD_sub");

    file->cd();
    hist->Write();
    delete hist;
    file->Close();
}
void derive_qcd_sys(TString dir, TString cut){
    TString cg_name[] = {"_C", "_B", "_D"}; 
    TFile* file = new TFile(dir+"datacard/QCD_"+cut+".root", "update");
    TH3D *hqcd_MC, *hqcds[3];
    TFile* qcd_file;
    for(int i=0; i<3; i++){
        qcd_file = TFile::Open(dir + "QCD/QCD_"+cut+cg_name[i]+".root");    
        if(cg_name[i].Contains("D")){//normalized QCD shape to MC
            hqcds[i] = (TH3D*)qcd_file->Get("data_other_removed");
            hqcd_MC = (TH3D*)qcd_file->Get("QCD_MC_CG");
            hqcds[i]->Scale(hqcd_MC->GetSumOfWeights()/hqcds[i]->GetSumOfWeights());
            delete hqcd_MC;
        }
        else if(cg_name[i].Contains("B"))//WJets dominants, using QCD MC
            hqcds[i] = (TH3D*)qcd_file->Get("QCD_MC_CG");
        else//Control Region
            hqcds[i] = (TH3D*)qcd_file->Get("data_other_removed");  
        hqcds[i]->SetName("QCD_shape_not_nomalized"+cg_name[i]);
        qcd_file->Close();
    }
    
    TH3D* hist_up = (TH3D*)hqcds[0]->Clone();
    hist_up->SetName("QCD_qcdsUp_sub");
    TH3D* hist_dn = (TH3D*)hqcds[0]->Clone();
    hist_dn->SetName("QCD_qcdsDown_sub");

    hqcds[1]->Divide(hqcds[2]);
    hist_up->Multiply(hqcds[1]);
    hist_dn->Divide(hqcds[1]);
    file->cd();
    hist_up->Write();
    hist_dn->Write();
    delete hqcd_MC;
    delete hist_up;delete hist_dn;
    delete hqcds[0];delete hqcds[1];delete hqcds[2];
    file->Close();
}

void add_qcd(int i, int j, int year){
    TString dir = Form("../output/%d", year);
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    derive_qcd_nom(dir, cutsName[i]);
    derive_qcd_sys(dir, cutsName[i]);

} 
