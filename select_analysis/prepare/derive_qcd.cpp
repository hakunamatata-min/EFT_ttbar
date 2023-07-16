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
    for(int i=1; i<=h1->GetNbinsX(); i++){
        Double_t num = h1->GetBinContent(i);
        if(num<0)
            h1->SetBinContent(i, 0);
    }
}
void derive(TString path, TString cut, TString cut_name, int l, int g, int nbins, double* xbins){
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
    //Double_t xbins[2][20] = {{0,340,360,380,400,420,440,460,480,500,520,540,570,600,640,700,3000}, {0,450,500,570,630,700,820,3000}};
    //int nbins[2] = {16, 7};
    //TString div_con[] = {"*(rapidity_tt<1.4 && rapidity_tt>-1.4)", "*(rapidity_tt>1.4 || rapidity_tt<-1.4)"};
    //int bin_num = nbins[0] + nbins[1];
    float lumi=59.83;
    TString cg = cg_n[g];
    int edge[]={0,20,29}; 
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    //TString cate[] = {"1","(jet_num==3)","(jet_num>=4)"};
    //TString lep[] = {"*1","*(!lep_flavour)","*(lep_flavour)"};
    TString date_name[][3] = {{"","",""},{"","",""},{"","E","S"},{"","E","S"}};
    //TString lep_name[] = {"","_E","_M"};
    //TString cate_name[] = {"_semi","_3jets","_4jets"};
    TString edge_name[] = {"other", "QCD"};
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    //TString other_con2 = "*1";
    TH1D *hdata, *hmc_b[3], *hmc_qa;
    TFile* file = new TFile(path+"/QCD/QCD_"+cut_name+"_"+cg+".root", "recreate");
    Double_t pre_scale[][3] = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1536.28, 474.95}, {1.0, 1536.28, 474.95}};
    TChain* tree2 = new TChain("mytree");
    tree2->Add(path+"/data/"+cg+"/new_data"+date_name[g][l]+"*.root");
    TChain *tree_b[nsample], *tree1_b[nsample], *tree_a[nsample], *tree1_a[nsample];
    for(int j=0; j<nsample; j++){
        tree_b[j]=new TChain("mytree");
        tree1_b[j]=new TChain("rawtree"); 
        tree_b[j]->Add(path+"/MC/"+cg+"/"+fileNames[j]);
        tree1_b[j]->Add(path+"/MC/"+cg+"/"+fileNames[j]);
    }
    for(int j=edge[1]; j<edge[2]; j++){
        tree_a[j]=new TChain("mytree");
        tree1_a[j]=new TChain("rawtree"); 
        tree_a[j]->Add(path+"/MC/A/"+fileNames[j]);
        tree1_a[j]->Add(path+"/MC/A/"+fileNames[j]);
    }
    auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600);
    hdata = new TH1D("QCD", "", nbins, xbins);
    c1->cd();
    tree2->Draw("mass_tt>>QCD", cut+other_con1+other_con2);
    for(int k=0; k<2; k++){
        for(int j=edge[k]; j<edge[k+1]; j++){
            TH1D* nmc=new TH1D("nmc","",50,0,100);
            c1->cd();
            tree1_b[j]->Draw("nJet>>nmc","Generator_weight");
            float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
            TString weight=Form("%f*Generator_weight*SF_btag_it*SF_lepton*"+cut+other_con1+other_con2,weight1);
            if(j<3)
                weight = weight + "*nnlo_wt";
            c1->cd();
            TH1D* hist = new TH1D("hist", "", nbins, xbins);
            hist ->Sumw2();
            tree_b[j]->Draw("mass_tt>>hist", weight);

            
            if(j==edge[k]){
                hmc_b[k]=(TH1D*)hist->Clone();
                hmc_b[k]->SetName(edge_name[k]+"_MC");
            }
            else
                hmc_b[k]->Add(hist);                
            delete nmc;
            delete hist;
            //cout<<j<<" "<<hmc_b[k]->GetSumOfWeights()<<endl;
        }
    }
    
    cout<<"finished B region"<<endl;
    hdata->Scale(pre_scale[g][l]);
    cout<<hdata->GetSumOfWeights()<<endl;
    //double ratio=0;
    //cout<<hdata->GetSumOfWeights()<<" "<<hmc_b[0]->GetSumOfWeights()<<" "<<hmc_b[1]->GetSumOfWeights()<<endl;
    hdata->Add(hmc_b[0],-1.0);//shape
    for(int j=edge[1]; j<edge[2]; j++){
        TH1D* nmc=new TH1D("nmc","",50,0,100);
        nmc->Sumw2();
        c1->cd();
        tree1_a[j]->Draw("nJet>>nmc","Generator_weight");
        float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
        TString weight=Form("%f*Generator_weight*SF_btag*SF_lepton*"+cut+other_con1+other_con2,weight1);
        c1->cd();
        TH1D* hist = new TH1D("hist", "", nbins, xbins);
        hist->Sumw2();
        tree_a[j]->Draw("mass_tt>>hist", weight);

        if(j==edge[1]){
            hmc_qa=(TH1D*)hist->Clone();
            hmc_qa->SetName("QCD_A");
        }
        else
            hmc_qa->Add(hist);
        //cout<<hmc_qa->GetSumOfWeights()<<endl;       
        delete nmc;
        delete hist;
    }
    cout<<"finished A region"<<endl;
    //ratio = ratio/hmc_b[1]->GetSumOfWeights();//norm
    set0(hdata);
    TH1D* h1 = (TH1D*)hdata->Clone();
    TH1D* h2 = (TH1D*)hmc_b[1]->Clone();
    h1->SetName("QCD_CG");
    h2->SetName("QCD_CG_MC");
    cout<<hmc_qa->GetSumOfWeights()<<endl;
    hdata->Scale(hmc_qa->GetSumOfWeights()/hmc_b[1]->GetSumOfWeights());
    hmc_b[1]->Scale(hmc_qa->GetSumOfWeights()/hmc_b[1]->GetSumOfWeights());
    file->cd();
    hmc_b[1]->Write();    
    hdata->Write();
    h1->Write();
    h2->Write();
    delete hmc_b[0];
    delete hmc_b[1];
    delete hmc_qa;
    delete hdata;
    delete c1;

    file->Close();
}
void derive_qcd(int i, int j, int g){
    //TString cg[] = {"A", "B", "C", "D"};
    TString cuts[] = {"(jet_num == 3 && likelihood<20.0 && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3 && likelihood<20.0 && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString div_name[4] = {"_y0", "_y1", "_y2", "_y3"};
    TString div_con[4] = {"*(fabs(rapidity_tt)<0.4)", "*(fabs(rapidity_tt)<1.0 && fabs(rapidity_tt)>0.4)",
                            "*(fabs(rapidity_tt)<2.0 && fabs(rapidity_tt)>1.0)", "*(fabs(rapidity_tt)>2.0)"};
    int nbins[] = {9, 11, 10, 11};
    double xbins[][20] = {{0,300,340,380,420,460,500,600,800,3000}, {0,300,350,400,450,500,550,600,700,800,1000,3000}, 
                        {0,400,450,500,550,600,650,700,800,1000,3000}, {0,450,550,650,700,750,800,900,1000,1200,1400,3000}};
    TString path = "/home/yksong/code/ttbar/output/2018/QCD_ES";
    int lep[] = {1, 1, 2, 2};

    derive(path, cuts[i]+div_con[j], cutsName[i]+div_name[j], lep[i], g, nbins[j], xbins[j]);
    
} 