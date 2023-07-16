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
void add_pre(TString file_n1, TString file_n2, TString file_n, TString path){
    TFile *file1 = TFile::Open(path+file_n1);
    TFile *file2 = TFile::Open(path+file_n2);
    TFile *file = new TFile(path+file_n, "RECREATE");
    //TString var_name[] = {"top_pt", "Mtt", "deltay"};
    TString var_name[] = {"QCD"};
    for(int i=0; i<1; i++){
        TH1D *h1 = (TH1D*)file1->Get(var_name[i]);
        h1->SetName("h1");
        TH1D *h2 = (TH1D*)file1->Get(var_name[i]);
        h2->SetName("h2");
        TH1D* h3 = (TH1D*)h1->Clone();
        h3->SetName(var_name[i]);
        h3->Add(h2);
        file->cd();
        h3->Write();
        delete h1;
        delete h2;
        delete h3;
    }
    file->Close();
}
void add_cd(){
    TString file_n1[] = {"QCD_E_4jets_B.root", "QCD_E_3jets_B.root", "QCD_E_4jets_C.root", "QCD_E_3jets_C.root", "QCD_E_3jets_D.root", "QCD_E_4jets_D.root"};
    TString file_n2[] = {"QCD_M_4jets_B.root", "QCD_M_3jets_B.root", "QCD_M_4jets_C.root", "QCD_M_3jets_C.root", "QCD_M_3jets_D.root", "QCD_M_4jets_D.root"};
    TString file_n[] = {"QCD_4jets_B.root", "QCD_3jets_B.root", "QCD_4jets_C.root", "QCD_3jets_C.root", "QCD_3jets_D.root", "QCD_4jets_D.root"};
    TString path = "./QCD_2D/";
    for(int i=0; i<6; i++)
        add_pre(file_n1[i], file_n2[i], file_n[i], path);
    add_pre("QCD_4jets_B.root", "QCD_3jets_B.root", "QCD_semi_B.root", path);
    add_pre("QCD_4jets_C.root", "QCD_3jets_C.root", "QCD_semi_C.root", path);
    add_pre("QCD_4jets_D.root", "QCD_3jets_D.root", "QCD_semi_D.root", path);
}