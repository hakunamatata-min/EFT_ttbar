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
double p2weight=0.65/0.3;
void sum_h1(TH1D* hs, TH1D* h1, TH1D *h2){
    for(int i=0; i<h1->GetNbinsX(); i++){
        hs->SetBinContent(i+1, h1->GetBinContent(i+1));
    }
    for(int i=0; i<h2->GetNbinsX(); i++){
        hs->SetBinContent(h1->GetNbinsX()+i+1, h2->GetBinContent(i+1));
    }
}
void renew_weight(TString* weight, TString file, int j, TString path){
    const int nsample = 3;
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,};
    float lumi=59.83;
    auto c1 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(path+"MC/"+file);
    TH1D *nmc=new TH1D("nmc","",50,0,100);
    nmc->Sumw2();
    c1->cd();
    tree1->Draw("nJet>>nmc","Generator_weight");
    //cout<<nmc->GetSumOfWeights()<<endl;
    float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
    //cout<<weight1<<endl;
    *weight = Form("%f*", weight1) + (*weight);
    delete tree1;
    delete nmc;
    delete c1;
}
void draw_pre(int year, TString other_con1, TString other_con2, TString file_name){
    const int nsample = 3;
    TString fileNames[nsample] = {  
                            "new_TTToSemiLeptonic_TuneCP5_13TeV-powheg.root",
                            "new_TTTo2L2Nu_TuneCP5_13TeV-powheg.root",
                            "new_TTToHadronic_TuneCP5_13TeV-powheg.root"
                            };
    TString EW[] = {"_ci0000","_ci0010","_ci0020","_ci0001","_ci0002","_ci0100","_ci0200"};
    TString weight_ew[] = {"weight_ci0000","weight_ci0010","weight_ci0020","weight_ci0001","weight_ci0002","weight_ci0100","weight_ci0200"};
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    TString file_up, file_down; 
    //TString div_con[] = {"*(fabs(rapidity_tt)<0.4)", "*(fabs(rapidity_tt)<1.0 && fabs(rapidity_tt)>0.4)","*(fabs(rapidity_tt)<2.0 && fabs(rapidity_tt)>1.0)", "*(fabs(rapidity_tt)>2.0)"};
    //Int_t bins[] = {65, };
    //double xup[] = {2000, 3.0};
    //double xdown[] = {300, };
    TString title[] = {"deltay_mtt"}; 
    TString weight_up, weight_down;
    TH2D *hmc0, *hmc1;
    TH2D *hist0, *hist1; 
    TString path = Form("../%d/",year);
    TFile* file = new TFile(path+file_name, "recreate");
    for(int w=0; w<7; w++){
        for(int j=0; j<3; j++){
            TString weight = "Generator_weight*SF_btag*SF_lepton*L1PreFiringWeight_Nom*nnlo_wt"+other_con1+other_con2;
            TChain* tree=new TChain("mytree");
            hist1 = new TH2D("CG1","", 85, 300, 2000, 60, -3.0, 3.0);
            hist1->Sumw2();
            renew_weight(&weight, fileNames[j], j, path);
            tree->Add(path+"MC/"+fileNames[j]);
            if(w == 0){
                hist0 = new TH2D("CG","", 85, 300, 2000, 60, -3.0, 3.0);
                hist0->Sumw2();
                tree->Draw("rapidity_tt:mass_tt>>CG", weight);
            }
            tree->Draw("rapidity_tt:mass_tt>>CG1", weight+"*"+weight_ew[w]);
            //cout<<hist1->GetSumOfWeights()<<" "<<hist2->GetSumOfWeights()<<endl;
            if(j==0){
                if(w == 0){
                    hmc0 = (TH2D*)hist0->Clone();
                    hmc0->SetName(title[0]+"_nom");
                }
                hmc1=(TH2D*)hist1->Clone();
                hmc1->SetName(title[0]+EW[w]+"_nom");
            }
            else{
                if(w == 0){
                    hmc0->Add(hist0);
                }
                hmc1->Add(hist1);
            } 
            if(w == 0)
                delete hist0;             
            delete tree;
            delete hist1;
        }
        cout<<weight_ew[w]<<"_"<<" finished"<<endl;
        file->cd();
        if(w == 0){
            hmc0->Write();
            delete hmc0;
        }
        hmc1->Write();
        delete hmc1;
    }
    file->Close();
}
void divide_bin(){
    TString other_con3 = "*(jet_num>=4 || (jet_num==3 && jet_pt[0]>50))";
    TString other_con4 = "*(lep_flavour || ((!lep_flavour) && lepton_pt>34))";
    //draw_pre(2018, other_con3, other_con4, "EWun.root");
    draw_pre(2018, other_con3, other_con4, "ytt_mtt.root");
}