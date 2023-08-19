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

void set0(TH3D* h1){
    for(int i=0; i<=h1->GetNbinsX()+1; i++){
        for(int j=0; j<=h1->GetNbinsY()+1; j++){
            for(int k=0; k<=h1->GetNbinsZ()+1; k++){
                if(h1->GetBinContent(i, j, k) < 0){
                    h1->SetBinContent(i, 0);
                    h1->SetBinError(i, 0);
                }
            }
        }
    }
}
void renew_weight(TString dir, TString* weight, TString file, int f, int year){ //global weight
    const int nsample = 29;
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3,	
                                    //53870.0,
                                    //1264.0,1345.7, 359.7, 48.9, 12.1, 5.5, 1.3, 0.032,//LO
                                    186100000.0, 23590000, 1555000, 324500, 30310, 6444, 1127, 109.8, 21.98};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21,//1.21,1.21,1.21,1.21,
                                0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double lumi_s[4]={19.5, 16.8, 41.48, 59.83};
    double lumi = lumi_s[year-2015];
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600);
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(dir+"/MC/"+file);
    TH1D *nmc=new TH1D("nmc","",50,0,100);
    //nmc->Sumw2();
    c0->cd();
    tree1->Draw("nJet>>nmc","Generator_weight");
    //cout<<nmc->GetSumOfWeights()<<endl;
    float weight1=cross_sections[f]*lumi/(nmc->GetSumOfWeights())*K_Factor[f]*1000;
    //cout<<weight1<<endl;
    *weight = Form("%f*", weight1) + (*weight);
    delete tree1;
    delete nmc;
    delete c0;
}
void derive(TString cut, TString cut_name, int g, int year, int* xyz_bins, double* xyz_range){
    const int nsample = 29;
    int xbins = xyz_bins[0];
    int ybins = xyz_bins[1];
    int zbins = xyz_bins[2];
    double xlow = xyz_range[0];
    double xup = xyz_range[1];
    double ylow = xyz_range[2];
    double yup = xyz_range[3];
    double zlow = xyz_range[4];
    double zup = xyz_range[5];
    TString fileNames[nsample] = {"new_TTToSemiLeptonic_TuneCP5_13TeV-powheg.root",
                                  "new_TTTo2L2Nu_TuneCP5_13TeV-powheg.root",
                                  "new_TTToHadronic_TuneCP5_13TeV-powheg.root",

                                  "new_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                                  "new_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM.root",                           
                                  "new_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            
                                  "new_ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo.root",
                                  "new_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                                  "new_ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                                  "new_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",
                                  "new_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",                                                                               
                                
                                  "new_W1JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                                  "new_W2JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                                  "new_W3JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                                  "new_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",

                                  "new_QCD_HT50to100_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT100to200_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT200to300_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT300to500_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT500to700_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT700to1000_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT1000to1500_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT1500to2000_TuneCP5_PSWeights_13TeV-madgraph.root",
                                  "new_QCD_HT2000toInf_TuneCP5_PSWeights_13TeV-madgraph.root",};
    
    TString cg_n[] = {"A", "B", "C", "D"};
    TString cg = cg_n[g];
    int edge[]={0, 20, 29}; 
    TString edge_name[] = {"other", "QCD"};
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    
    //Lepton_triggers in C and D regions have prescales
    TString dataset = "";
    if(g>1 && cut_name.Contains("M"))
        dataset = "M";
    else if(g>1 && cut_name.Contains("E"))
        dataset = "E"; 
    Double_t pre_scale_year[][2] = {{572.37, 143.42}, {572.37, 143.42}, {1085.83, 224.41}, {1536.28, 474.95}};
    Double_t pre_scale;
    if(g < 2)
        pre_scale = 1.0;
    else if(cut_name.Contains("M"))
        pre_scale = pre_scale_year[year-2015][1];
    else
        pre_scale = pre_scale_year[year-2015][0];
    
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";

    TH3D *hdata, *hmc_b[2], *hmc_qa;
    hdata = new TH3D("QCD_other_removed", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);//final QCD;
    hmc_b[0] = new TH3D("other_MC_CG", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    hmc_b[1] = new TH3D("QCD_MC_CG", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    hmc_qa = new TH3D("QCD_MC_SG", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    hmc_b[0]->Sumw2();
    hmc_b[1]->Sumw2();
    hmc_qa->Sumw2();

    TString input_path = Form("../../QCD_analysis/output/%d/",year);
    TString output_path = Form("../output/%d/QCD",year);

    TFile* file = new TFile(output_path+"QCD_"+cut_name+"_"+cg+".root", "recreate");
    TChain* data_tree = new TChain("mytree");
    data_tree->Add(input_path+"data/"+cg+"/new_data_"+dataset+"*.root");
    auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600);
    c1->cd();
    data_tree->Draw("likelihood:fabs(rapidity_tt):mass_tt>>QCD_derived", cut+other_con1+other_con2);
    hdata->Scale(pre_scale);
    set0(hdata);

    TChain* MC_tree;
    for(int k=0; k<2; k++){
        for(int j=edge[k]; j<edge[k+1]; j++){
            MC_tree = new TChain("mytree");
            MC_tree->Add(input_path+"MC/"+cg+"/"+fileNames[j]);
            TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom*" + cut + other_con1 + other_con2;
            if(j < 3)
                weight = weight + "*nnlo_wt";
            renew_weight(input_path+"MC/"+cg+"/", &weight, fileNames[j], j, year);
            c1->cd();
            TH3D* hist = new TH3D("hist", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
            hist ->Sumw2();
            MC_tree->Draw("likelihood:fabs(rapidity_tt):mass_tt>>hist", weight);
            hmc_b[k]->Add(hist);                
            delete hist;
            delete MC_tree;
        }
    }
    cout<<"finished Control region"<<endl;
    //cout<<hdata->GetSumOfWeights()<<endl;
    hdata->Add(hmc_b[0], -1.0);//shape

    for(int j=edge[1]; j<edge[2]; j++){
        MC_tree = new TChain("mytree");
        MC_tree->Add(input_path+"MC/A/"+fileNames[j]);
        TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom*" + cut + other_con1 + other_con2;
        renew_weight(input_path+"MC/A/", &weight, fileNames[j], j, year);
        c1->cd();
        TH3D* hist = new TH3D("hist", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
        hist ->Sumw2();
        MC_tree->Draw("likelihood:fabs(rapidity_tt):mass_tt>>hist", weight);
        hmc_qa->Add(hist);      
        delete hist;
        delete MC_tree;
    }
    //cout<<hmc_qa->GetSumOfWeights()<<endl;
    cout<<"finished A region"<<endl;
    TH3D* hqcd = (TH3D*)hdata->Clone();
    hqcd->SetName("QCD_derived");
    TH3D* hqcd_MCde = (TH3D*)hmc_b[1]->Clone();
    hqcd_MCde->SetName("QCD_derived_MC");

    hqcd->Scale(hmc_qa->GetSumOfWeights()/hmc_b[1]->GetSumOfWeights());
    hqcd_MCde->Scale(hmc_qa->GetSumOfWeights()/hmc_b[1]->GetSumOfWeights());  
    
    file->cd();
    hmc_b[1]->Write();      //QCD_MC_CG (QCD MC samples in CG)
    hdata->Write();         //data_other_removed (data-prompt MCs in CG)
    hqcd->Write();          //QCD_derived (SG)
    hqcd_MCde->Write();     //QCD_derived_MC (SG, using MC of CG)

    delete hmc_b[0];
    delete hmc_b[1];
    delete hmc_qa;
    delete hdata;
    delete hqcd;
    delete hqcd_MCde;
    delete c1;
    file->Close();
}
void derive_qcd_3D(int i, int g, int year){
    //TString cg[] = {"A", "B", "C", "D"};
    TString cuts[] = {"(jet_num == 3 && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3  && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};

    int nbins[] = {9, 11, 10, 11};
    int xyz_bins[] = {270, 40, 37};
    double xyz_range[] = {300, 3000, 0, 4.0, 13, 50};
    derive(cuts[i], cutsName[i], g, year, xyz_bins, xyz_range);
} 