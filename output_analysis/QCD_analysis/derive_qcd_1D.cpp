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

void set0(TH1D* h1){
    for(int i=0; i<h1->GetNbinsX(); i++){
        Double_t num = h1->GetBinContent(i);
        if(num<0)
            h1->SetBinContent(i, 0);
    }
}
void derive(int c, int l, int g){
    const int nsample = 29;
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

                            "new_QCD_HT50to100_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT100to200_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT200to300_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT300to500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT500to700_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT700to1000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1000to1500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1500to2000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT2000toInf_TuneCP5_PSWeights_13TeV-madgraph.root",};
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
    TString cg_n[] = {"A", "B", "C", "D"};
    TString xtitle[] = {"lnL","M_{t}","M_{#bar{t}}","M_{Wl}","M_{Wh}","M_{th}","M_{tl}","P_{T}^{l}","P_{T}^{leading-jet}","jet_num","p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    TString title[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    TString xvars[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt", "rapidity_tt"};
    Double_t xup[] = {50, 450, 450, 140, 140, 450, 450, 250, 400, 7, 500, 1500, 4};
    Double_t xdown[] = {13, 50, 50, 50, 50, 50, 50, 0, 0, 3, 50, 200, -4};
    Int_t bins[]={37, 40, 40, 36, 36, 40, 40, 20, 20, 4, 20, 20, 16};
    float lumi=59.83;
    TString cg = cg_n[g];
    int edge[]={0,20,29}; 
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    TString cate[] = {"1","(jet_num==3)","(jet_num>=4)"};
    TString lep[] = {"*1","*(!lep_flavour)","*(lep_flavour)"};
    TString date_name[][3] = {{"","",""},{"","",""},{"","E","S"},{"","E","S"}};
    TString lep_name[] = {"","_E","_M"};
    TString cate_name[] = {"_semi","_3jets","_4jets"};
    TString edge_name[] = {"other", "qcd"};
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    //TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    TString other_con2 = "*1";
    TH1D *hdata, *hdata1, *hmc_b[3], *hmc_qa;
    TFile* file = new TFile("./QCD_1D/QCD"+lep_name[l]+cate_name[c]+"_"+cg+".root", "recreate");
    Double_t pre_scale[][3] = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1536.28, 474.95}, {1.0, 1536.28, 474.95}};
    TChain* tree2 = new TChain("mytree");
    tree2->Add("./data/"+cg+"/new_data"+date_name[g][l]+"*.root");
    TChain *tree_b[nsample], *tree1_b[nsample], *tree_a[nsample], *tree1_a[nsample];
    for(int j=0; j<nsample; j++){
        tree_b[j]=new TChain("mytree");
        tree1_b[j]=new TChain("rawtree"); 
        tree_b[j]->Add("./MC/"+cg+"/"+fileNames[j]);
        tree1_b[j]->Add("./MC/"+cg+"/"+fileNames[j]);
    }
    for(int j=edge[1]; j<edge[2]; j++){
        tree_a[j]=new TChain("mytree");
        tree1_a[j]=new TChain("rawtree"); 
        tree_a[j]->Add("./MC/A/"+fileNames[j]);
        tree1_a[j]->Add("./MC/A/"+fileNames[j]);
    }
    for(int i=0; i<8; i++){
        auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600);
        hdata = new TH1D(title[i], "", bins[i], xdown[i], xup[i]);
        tree2->Draw(xvars[i]+">>"+title[i],cate[c]+lep[l]+other_con1+other_con2);
        for(int k=0; k<2; k++){
            for(int j=edge[k]; j<edge[k+1]; j++){
                TH1D* nmc=new TH1D("nmc","",50,0,100);
                nmc->Sumw2();
                c1->cd();
                tree1_b[j]->Draw("nJet>>nmc","Generator_weight");
                float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
                TString weight=Form("%f*Generator_weight*SF_btag_it*SF_lepton*"+cate[c]+lep[l]+other_con1+other_con2,weight1);
                if(j<3)
                    weight = weight + "*nnlo_wt";
                c1->cd();
                TH1D* hist = new TH1D("CG","", bins[i], xdown[i], xup[i]);
                hist->Sumw2();
                tree_b[j]->Draw(xvars[i]+">>CG", weight);
                if(j==edge[k]){
                    hmc_b[k]=(TH1D*)hist->Clone();
                    hmc_b[k]->SetName(title[i]+edge_name[k]+"_B");
                }
                else
                    hmc_b[k]->Add(hist);                
                delete nmc;
                delete hist;
                //cout<<j<<" "<<hmc_b[k]->GetSumOfWeights()<<endl;
            }
        }
        
        cout<<"finished B region of "<<title[i]<<endl;
        hdata->Scale(pre_scale[g][l]);
        //double ratio=0;
        //cout<<hdata->GetSumOfWeights()<<" "<<hmc_b[0]->GetSumOfWeights()<<" "<<hmc_b[1]->GetSumOfWeights()<<endl;
        hdata->Add(hmc_b[0],-1.0);//shape
        for(int j=edge[1]; j<edge[2]; j++){
            TH1D* nmc=new TH1D("nmc","",50,0,100);
            nmc->Sumw2();
            c1->cd();
            tree1_a[j]->Draw("nJet>>nmc","Generator_weight");
            float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
            TString weight=Form("%f*Generator_weight*SF_btag*SF_lepton*"+cate[c]+lep[l]+other_con1+other_con2,weight1);
            c1->cd();
            TH1D* hist = new TH1D("CG","", bins[i], xdown[i], xup[i]);
            hist->Sumw2();
            tree_a[j]->Draw(xvars[i]+">>CG", weight); 
            //ratio += hist->GetSumOfWeights();   
            if(j==edge[1]){
                hmc_qa=(TH1D*)hist->Clone();
                hmc_qa->SetName(title[i]+"_A");
            }
            else
                hmc_qa->Add(hist);
            //cout<<hmc_qa->GetSumOfWeights()<<endl;       
            delete nmc;
            delete hist;
        }
        cout<<"finished A region of "<<title[i]<<endl;
        //ratio = ratio/hmc_b[1]->GetSumOfWeights();//norm
        set0(hdata);
        hdata->Scale(hmc_qa->GetSumOfWeights()/hmc_b[1]->GetSumOfWeights());
        
        file->cd();
        hdata->Write();
        delete hmc_b[0];
        delete hmc_b[1];
        delete hmc_qa;
        delete hdata;
        delete c1;
    }
    file->Close();
}
void derive_qcd_1D(){
    //TString cg[] = {"A", "B", "C", "D"};
    for(int i=2; i<3; i++){
        for(int j=1; j<3; j++){
            derive(1, j, i);
            derive(2, j, i);
        }
    }
}