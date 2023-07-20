#include<TROOT.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1F.h>
#include<TH2F.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;
void add_weight_branch_tree(TString fileName, TString tree_name){
    TH2F* hist[9];
    //TString dir="/home/yksong/code/ttbar/EW_weight/correction_roots/";
    TString dir="./correction_roots/";
    Int_t Cpq3[9]={ 0, 0, 0, 0, 0, 2, 0, 0, 0 };
    Int_t Cpu[9]={  0, 2, 1, 0, 0, 0, 2, 0, 0 };
    Int_t ReCup[9]={0, 0, 0, 1, 0, 0, 0, 2, 0 };
    Int_t ImCup[9]={0, 0, 0, 0, 1, 0, 0, 0, 2 };                 
    for(Int_t i=0; i<5; i++){
        TString file;
        file.Form("EWci%d%d%d%d.root",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        TFile* fhist=TFile::Open(dir+file);
        hist[i]=(TH2F*)fhist->Get("h2");
    }
    TFile *file=new TFile(fileName,"update");
    TTree *mytree=(TTree*) file->Get(tree_name);
    Float_t M_tt, delta_rapidity;
    mytree->SetBranchAddress("M_tt_gen",&M_tt);
    mytree->SetBranchAddress("delta_rapidity_gen",&delta_rapidity);      
    Float_t weight[9];TBranch* branch[9];
    for(int i=0; i<5; i++){
        TString weight_name=Form("weight_ci%d%d%d%d",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        branch[i]=mytree->Branch(weight_name,&weight[i],weight_name+"/F");
    }  
    Int_t entries=mytree->GetEntries();
    cout<<"total number of events: "<<entries<<endl;
    for(Int_t i=0;i<entries;i++){
        mytree->GetEntry(i);
        for(Int_t i=0; i<5; i++){
            Int_t nbin=hist[i]->FindBin(M_tt,delta_rapidity);
            weight[i]=1.0+hist[i]->GetBinContent(nbin);
            branch[i]->Fill();
            //cout<<"weight[i]: "<<weight[i]<<endl;
        }
    }
    mytree->Write("",TObject::kOverwrite);
    delete mytree;
    file->Close();        
}
void add_weight_branch(TString inputFile, bool is_sys, int year){
    TString trees[]={"jerUp", "jerDown", "unclusUp", "unclusDown"};
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    add_weight_branch_tree(inputFile, "mytree");
    if(!is_sys){
        for(int i=0; i<6; i++){
            add_weight_branch_tree(inputFile, trees[i]);
        }
        for(int i=0; i<10; i++){
            add_weight_branch_tree(inputFile, "jes_"+jes_source[i]+"Up");
            add_weight_branch_tree(inputFile, "jes_"+jes_source[i]+"Down");
        }
    }

    cout<<"EW weights has been added to "<<inputFile<<endl;
}
