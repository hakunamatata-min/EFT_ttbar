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
#include<TPaveText.h>
using namespace std;
double p2weight=0.65/0.3;
void renew_weight(TString dir, TString* weight, TString file, int f, int year){ //global weight
    const int nsample = 45;
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3,	
                                    //53870.0,
                                    //1264.0,1345.7, 359.7, 48.9, 12.1, 5.5, 1.3, 0.032,//LO
                                    186100000.0, 23590000, 1555000, 324500, 30310, 6444, 1127, 109.8, 21.98,
                                    6401000.0, 1993000.0, 364000.0, 66600.0, 16620.0, 1101.0,
                                    1367000.0, 381700.0, 87740.0, 21280.0, 7000.0, 622.6, 58.9, 18.12, 3.318, 1.085};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21,//1.21,1.21,1.21,1.21,
                                0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double lumi_s[4]={19.5, 16.8, 41.48, 59.83};
    double lumi = lumi_s[year-2015];
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600);
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(dir+file);
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
void derive(TString cut, TString cut_name, int g, int year, bool isEnriched){
    const int nsample = 45;
    TString Enrich_name[2] = {"", "_En"};
    TString fileNames[nsample] = {  
                            "new_TTToSemiLeptonic_TuneCP5_13TeV-powheg.root",
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
                            //"new_WJetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            /*"new_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM.root",*/

                            "new_QCD_HT50to100_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT100to200_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT200to300_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT300to500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT500to700_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT700to1000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1000to1500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1500to2000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT2000toInf_TuneCP5_PSWeights_13TeV-madgraph.root",

                            "new_QCD_Pt-30to50_EMEnriched_TuneCP5_13TeV.root",
                            "new_QCD_Pt-50to80_EMEnriched_TuneCP5_13TeV.root",
                            "new_QCD_Pt-80to120_EMEnriched_TuneCP5_13TeV.root",
                            "new_QCD_Pt-120to170_EMEnriched_TuneCP5_13TeV.root",
                            "new_QCD_Pt-170to300_EMEnriched_TuneCP5_13TeV.root",
                            "new_QCD_Pt-300toInf_EMEnriched_TuneCP5_13TeV.root",
                            
                            "new_QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-170To300_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-300To470_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-470To600_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-600To800_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-800To1000_MuEnrichedPt5_TuneCP5_13TeV.root",
                            "new_QCD_Pt-1000_MuEnrichedPt5_TuneCP5_13TeV.root"};
    

    TString title[] = {"mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    TString xvars[] = {"mass_tlep","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt", "rapidity_tt"};
    Double_t xup[] = {400, 250, 400, 7, 500, 1500, 3};
    Double_t xlow[] = {100, 0, 0, 3, 0, 300, -3};
    Int_t bins[]={20, 20, 20, 4, 20, 20, 20};

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
    
    int edge_dn[]={0, 3, 11, 16, 20};//23,31};
    int edge_up[]={3, 11, 16, 20, 29};
    if(isEnriched){
        if(cut_name.Contains("E")){
            edge_dn[4] = 29;
            edge_up[4] = 35;
        }
        else if(cut_name.Contains("M")){
            edge_dn[4] = 35;
            edge_up[4] = 45;
        }
    }

    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    
    TString cg_n[] = {"A", "B", "C", "D"};
    TString cg = cg_n[g];

    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";
    TChain *mytree;
    TH1D  *hist, *h1;
    TH1D *hdata;
    TString path = Form("./output/%d/",year);
    TFile* file = new TFile(path+"QCD_"+cut_name+"_"+cg+Enrich_name[isEnriched]+"_CG.root", "recreate");
    for(int var=5; var<7; var++){
        auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600); // temporary canvas
        TChain* data_tree = new TChain("mytree");
        data_tree->Add(path+"data/"+cg+"/new_data*"+dataset+"*.root");
        hdata = new TH1D("data", "", bins[var], xlow[var], xup[var]);
        data_tree->Draw(xvars[var]+">>data", cut+other_con1+other_con2);
        hdata->Scale(pre_scale);
        file->cd();
        hdata->Write();
        delete data_tree;
        
        for(int k=0; k<5; k++){
            h1 = new TH1D(title[var], "", bins[var], xlow[var], xup[var]);
            h1->Sumw2();
            for(int j=edge_dn[k]; j<edge_up[k]; j++){
                TChain* tree=new TChain("mytree");
                tree->Add(path+"MC/"+cg+"/"+fileNames[j]);

                TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom*" + cut + other_con1 + other_con2;
                if(k==0)
                    weight = weight + "*nnlo_wt";
                renew_weight(path+"MC/"+cg+"/", &weight, fileNames[j], j, year);
                c1->cd();
                hist = new TH1D("hist","", bins[var], xlow[var], xup[var]);
                hist->Sumw2();
                tree->Draw(xvars[var]+">>hist", weight);
                //cout<<fileNames[j]<<": "<<hist->GetSumOfWeights()<<endl;
                h1->Add(hist);                
                delete tree;
                delete hist;
            }
            file->cd();
            h1->Write();
            delete h1;
        }
        delete c1;
        file->Close();
    }
}
void draw_cg(int i, int g, int year, bool isEnriched){
    TString cuts[] = {"(jet_num == 3 && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3  && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};

    for(int i=0; i<3; i++)
       derive(cuts[i], cutsName[i], g, year, isEnriched);
} 