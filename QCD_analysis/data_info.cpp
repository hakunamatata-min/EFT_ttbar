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

void data_info(int i, int g, int year){
    // TFile* file = TFile::Open("/home/yksong/code/EFT-ttbar/QCD_analysis/output/2015/data/new_data_0_B.root");
    // TTree* tree = (TTree*)file->Get("mytree");
    // Double_t likelihood;
    // Float_t  rapidity_tt, mass_tt;
    // tree->SetBranchAddress("likelihood",&likelihood);
    // tree->SetBranchAddress("rapidity_tt",&rapidity_tt);
    // tree->SetBranchAddress("mass_tt",&mass_tt);
    // for (int entry = 0; entry < 100; entry++){
    //     tree->GetEntry(entry);
    //     cout<<mass_tt<<" "<<rapidity_tt<<" "<<likelihood<<endl;
    // }
    TH3D *hdata;
    TH1D *hdata1d;
    //hdata = new TH3D("QCD_other_removed", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);//final QCD;
    hdata1d= new TH1D("h1", "", 50, 0, 50);
    TString cuts[] = {"(jet_num == 3 && (!lep_flavour) && likelihood < 20.0)","(jet_num >= 4  && (!lep_flavour))",
                     "(jet_num == 3  && lep_flavour && likelihood < 20.0)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};

    int nbins[] = {9, 11, 10, 11};
    int xyz_bins[] = {270, 40, 37};
    // double xyz_range[] = {300, 3000, 0, 4.0, 13, 50};
    double xyz_range[] = {0, 50, 0, 50, 0, 50};
    //derive(cuts[i], cutsName[i], g, year, xyz_bins, xyz_range, 0);
    TString input_path = Form("/home/yksong/code/EFT-ttbar/QCD_analysis/output/%d/",year);
    TString output_path = Form("./output/%d/Enriched/",year);

            TFile* file = new TFile("QCD_3D.root");

    TChain* data_tree = new TChain();

    // data_tree->Add(input_path+"data/"+"new_data*"+cg+".root");
    data_tree->Add("/home/yksong/code/EFT-ttbar/QCD_analysis/output/2015/data/new_data_0_B.root/mytree");

    // cout<<data_tree->GetEntries()<<endl;
    //data_tree->GetEntry(0);
    // data_tree->Print();
    //cout<<input_path<<"data/new_data*"<<cg<<".root"<<endl;
    // auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600);
    // c1->cd();
    
    // data_tree->Draw("likelihood:1:1000 >>QCD_other_removed", cut+other_con1+other_con2);
    //data_tree->Draw("likelihood:likelihood:likelihood >>QCD_other_removed",cut);
    data_tree->Draw("MtW>>h1");


    //cout<<hdata->GetSumOfWeights()<<endl;
    //    cout<<hdata->GetEntries()<<endl;
            cout<<hdata1d->GetEntries()<<endl;
            cout<<hdata1d->GetSumOfWeights()<<endl;
    //delete hdata;
    //delete hqcd;
    //delete hqcd_MCde;
    //file->Close();

}