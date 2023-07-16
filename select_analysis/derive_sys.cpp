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
void seterror0(TH1D* h1){
    for(int i=1; i<=h1->GetNbinsX(); i++){
        h1->SetBinError(i,0);
    }
}
void give_name(TString file, TString weight, int s, int c, TString* tree_up, TString* tree_down, TString* file_up, TString* file_down, TString* weight_up, TString* weight_down){
    TString sys[] = {"jes","jer","unclus","SF_lepton","SF_btag","L1PreFiringWeight_Nom","muR","muF","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt"};
    TString sys_up[] = {"jesUp","jerUp","unclusUp","SF_lepton_up","SF_btag_up","L1PreFiringWeight_Up","muR_up","muF_up","ISR_up","FSR_up","mtop173p5","hdampUP","TuneCP5up","nnlo_wt_up"};
    TString sys_down[] = {"jesDown","jerDown","unclusDown","SF_lepton_down","SF_btag_down","L1PreFiringWeight_Dn","muR_down","muF_down","ISR_down","FSR_down","mtop171p5","hdampDOWN","TuneCP5down","nnlo_wt_down"};
    if(s<3){
        *tree_up = sys_up[s];
        *tree_down = sys_down[s];
        *file_up = file;
        *file_down = file;
        *weight_up = weight;
        *weight_down = weight;
    }
    else if(s<6 || s==13){
        *tree_up = "mytree";
        *tree_down = "mytree";
        *file_up = file;
        *file_down = file;
        *weight_up = weight;
        weight_up->ReplaceAll(sys[s],sys_up[s]);
        *weight_down = weight;
        weight_down->ReplaceAll(sys[s],sys_down[s]);
    }
    else if(s<10){
        *tree_up = "mytree";
        *tree_down = "mytree";
        *file_up = file;
        *file_down = file;
        *weight_up = weight + "*" + sys_up[s];
        *weight_down = weight + "*" + sys_down[s];
    }
    else if(s<12){
        *tree_up = "mytree";
        *tree_down = "mytree";
        *file_up = file;
        file_up->ReplaceAll("TuneCP5",sys_up[s]+"_TuneCP5");
        *file_down = file;
        file_down->ReplaceAll("TuneCP5",sys_down[s]+"_TuneCP5");
        *weight_up = weight;
        *weight_down = weight;
    }
    else{
        *tree_up = "mytree";
        *tree_down = "mytree";
        *file_up = file;
        file_up->ReplaceAll("TuneCP5",sys_up[s]);
        *file_down = file;
        file_down->ReplaceAll("TuneCP5",sys_down[s]);
        *weight_up = weight;
        *weight_down = weight;
    }
}
void renew_weight(TString* weight, TString file, int j, TString path){
    const int nsample = 20;
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3,};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21};
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
void derive_pre(int cn, int l, int year, TString width){
    const int nsample = 20;
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
                            };
    fileNames[0].ReplaceAll("TTToSemiLeptonic", "TTToSemiLeptonic_"+width);
    fileNames[1].ReplaceAll("TTTo2L2Nu", "TTTo2L2Nu_"+width);
    TString sys[] = {"_jes","_jer","_unclus","_sl","_sj","_L1PF","_muR","_muF","_ISR","_FSR","_mtop","_hdamp","_TuneCP5","_nnlo","EW_un"};
    bool care[] = {1,1,0,0,1,0,0,0,0,0,1,1,0,1,1};
    TString pro[] = {"_TT", "_DY", "_ST", "_WJets"};
    int edge[]={0,3,11,16,20,29};//23,31}; 
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    TString file_up, file_down; 
    TString cate[] = {"1","(jet_num==3)","(jet_num>=4)"};
    TString lep[] = {"*1","*(!lep_flavour)","*(lep_flavour)"};
    TString cate_name[] = {"_semi","_3jets","_4jets"};
    TString lep_name[] = {"","_E","_M"};
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    TString xtitle[] = {"-2lnL","M_{t}","M_{#bar{t}}","M_{Wl}","M_{Wh}","M_{th}","M_{tl}","P_{T}^{l}","P_{T}^{leading-jet}","jet_num","p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    TString title[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    TString xvars[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt", "rapidity_tt"};
    int is_draw[] = {0, 1, 0, 0, 1,};
    Double_t xup[] = {50, 450, 450, 140, 140, 450, 450, 250, 400, 7, 500, 1500, 3};
    Double_t xdown[] = {13, 50, 50, 50, 50, 50, 50, 30, 30, 3, 50, 300, -3};
    Int_t bins[]={37, 40, 40, 36, 36, 40, 40, 22, 20, 4, 20, 24, 24};
    int color[] = {2, 1, 4};
    TString legend[] = {"nom", "up", "down"};
    TString tree_up, tree_down, weight_up, weight_down;
    TH1D *hmc1, *hmc2, *hmc3;
    TH1D *hist1, *hist2, *hist3; 
    TH1D *hd1, *hd2, *hd3;
    TString path = Form("./%d/",year);
    TFile* file = new TFile(path+"sys_root/sys"+lep_name[l]+cate_name[cn]+"_"+width+"_.root", "update");
    for(int i = 1; i < 2; i++){
        if(!is_draw[i])
            continue;
        //if(i>=2 && i<10)
            //continue;
        for(int s=0; s<14; s++){
            cout<<"process: "<<xvars[i]<<" "<<sys[s]<<endl;
            //if(!care[s])
            //    continue;
            for(int c=0; c<4; c++){
                if(s>9 && c>0)
                    continue;
                auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600); // temporary canvas
                c1->cd();
                for(int j=edge[c]; j<edge[c+1]; j++){
                    TString weight = cate[cn]+lep[l]+"*Generator_weight*SF_btag*SF_lepton*L1PreFiringWeight_Nom"+other_con1+other_con2;
                    if(c == 0)
                        weight = weight+"*nnlo_wt";
                    give_name(fileNames[j], weight, s, c, &tree_up, &tree_down, &file_up, &file_down, &weight_up, &weight_down);
                    renew_weight(&weight, fileNames[j], j, path);
                    renew_weight(&weight_up, file_up, j, path);
                    renew_weight(&weight_down, file_down, j, path);
                    /*cout<<weight_up<<endl;
                    cout<<weight_down<<endl;
                    cout<<weight<<endl;*/
                    TChain* tree=new TChain("mytree");
                    TChain* up_tree = new TChain(tree_up);
                    TChain* down_tree = new TChain(tree_down);
                    tree->Add(path+"MC/"+fileNames[j]);
                    up_tree->Add(path+"MC/"+file_up);
                    down_tree->Add(path+"MC/"+file_down);
                    if(s == 0){
                        hist1 = new TH1D("CG1","", bins[i], xdown[i], xup[i]);
                        hist1->Sumw2();
                        c1->cd();
                        tree->Draw(xvars[i]+">>CG1", weight);

                    }                    
                    hist2 = new TH1D("CG2","", bins[i], xdown[i], xup[i]);
                    hist3 = new TH1D("CG3","", bins[i], xdown[i], xup[i]);
                    
                    hist2->Sumw2();
                    hist3->Sumw2();
                    c1->cd();
                    up_tree->Draw(xvars[i]+">>CG2", weight_up);
                    down_tree->Draw(xvars[i]+">>CG3", weight_down);
                    //cout<<hist1->GetSumOfWeights()<<" "<<hist2->GetSumOfWeights()<<endl;
                    if(j==edge[c]){
                        if(s == 0){
                            hmc1=(TH1D*)hist1->Clone();
                            hmc1->SetName(title[i]+pro[c]+"_nom");
                        }
                        hmc2=(TH1D*)hist2->Clone();
                        hmc2->SetName(title[i]+pro[c]+sys[s]+"_up");
                        hmc3=(TH1D*)hist3->Clone();
                        hmc3->SetName(title[i]+pro[c]+sys[s]+"_down");
                    }
                    else{
                        if(s == 0)
                            hmc1->Add(hist1);
                        hmc2->Add(hist2);
                        hmc3->Add(hist3);
                    }              
                    delete tree;
                    delete up_tree;
                    delete down_tree;
                    if(s == 0)
                        delete hist1;
                    delete hist2;
                    delete hist3;
                }
                file->cd();
                if(s == 0){
                    hmc1->Write();
                    delete hmc1;
                }
                
                hmc2->Write();
                hmc3->Write();
                
                delete hmc2;
                delete hmc3;
                delete c1;

            }
        }
    }
    file->Close();
}
void derive_sys(int i, int j, TString width){
    derive_pre(i, j, 2018, width);
}