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

void derive_qcd_nom(TString output, TString indir, TString cut, TString year){
    TFile* file = new TFile(output, "recreate");
    TFile* qcd_file = TFile::Open(indir+"C/ttbar"+cut+year+".root");
    TH1D* hist = (TH1D*)qcd_file->Get("QCD_derived");
    hist->SetName("QCD");

    file->cd();
    hist->Write();
    delete hist;
    file->Close();
}
void derive_qcd_sys(TString output, TString indir, TString cut, TString year){
    TString cg_name[] = {"C/", "B/", "D/"}; 
    TFile* file = new TFile(output, "update");
    TH1D *hqcd_MC, *hqcds[3];
    TFile* qcd_file[3];
    for(int i=0; i<3; i++){
        qcd_file[i] = TFile::Open(indir+ cg_name[i]+ "ttbar"+cut+year+".root");    
        if(cg_name[i].Contains("D")){//normalized QCD shape to MC
            hqcds[i] = (TH1D*)qcd_file[i]->Get("QCD_other_removed");
            hqcd_MC = (TH1D*)qcd_file[i]->Get("QCD_MC_CG");
            hqcds[i]->Scale(hqcd_MC->GetSumOfWeights()/hqcds[i]->GetSumOfWeights());
            delete hqcd_MC;
        }
        else if(cg_name[i].Contains("B"))//WJets dominants, using QCD MC
            hqcds[i] = (TH1D*)qcd_file[i]->Get("QCD_MC_CG");
        else//Control Region
            hqcds[i] = (TH1D*)qcd_file[i]->Get("QCD_other_removed");  
        hqcds[i]->SetName("QCD_shape_not_nomalized"+cg_name[i]);
        cout<<hqcds[0]->GetName()<<endl;
        
    }
    // cout<<"111"<<endl;
    cout<<hqcds[0]<<endl;

    cout<<hqcds[0]->GetName()<<endl;
    TH1D* hist_up = (TH1D*)hqcds[0]->Clone();
    hist_up->SetName("QCD_qcdsUp_sub");
    TH1D* hist_dn = (TH1D*)hqcds[0]->Clone();
    hist_dn->SetName("QCD_qcdsDown_sub");
    // cout<<"222"<<endl;

    hqcds[1]->Divide(hqcds[2]);
    hist_up->Multiply(hqcds[1]);
    hist_dn->Divide(hqcds[1]);
    file->cd();
    hist_up->Write();
    hist_dn->Write();
    //delete hqcd_MC;
    delete hist_up;delete hist_dn;
    delete hqcds[0];delete hqcds[1];delete hqcds[2];
    file->Close();
    qcd_file[0]->Close();
    qcd_file[1]->Close();
    qcd_file[2]->Close();
    
}

void add_qcd(){
    TString indir;
    // TString type[3] = {"flat/", "no/", "smooth/"};
    TString output;
    TString year[] = {"_2015", "_2016", "_2017", "_2018"};
    TString cutsName[] = {"_E_3jets", "_E_4jets", "_M_3jets", "_M_4jets"};
    // for(int t=0; t<3; t++){
        for(int i=0; i<4; i++){
            for(int y=0; y<4; y++){
                indir = Form("/home/tangmin/EFT_ttbar_v1/combine/datacard/original/");
                output = "/home/tangmin/EFT_ttbar_v1/combine/datacard/add/ttbar"+cutsName[i]+year[y]+".root";
                derive_qcd_nom(output, indir, cutsName[i],year[y]);
                derive_qcd_sys(output, indir, cutsName[i],year[y]);
            }
        }
    // }
} 