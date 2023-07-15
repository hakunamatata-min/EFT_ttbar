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
void add_weight_mtop(TString indir, TString fileName){
    TH2F *hist_m[5], *hist_y[5];
    TString dir="/afs/cern.ch/user/y/yuekai/ttbar/EW_weight/mtop_roots/";
    //TString dir="/afs/cern.ch/user/y/yuekai/ttbar/EW_weight/correction_roots/";
    Int_t Cpq3[5]={ 0, 0, 0, 0, 0};
    Int_t Cpu[5]={  0, 1, 0, 0, 2};
    Int_t ReCup[5]={0, 0, 1, 0, 0};
    Int_t ImCup[5]={0, 0, 0, 1, 0};                 
    for(Int_t i=0;i<5;i++){
        TString file;
        if(fileName.Contains("171p"))
            file.Form("ci%d%d%d%d_%d.5.root",Cpq3[i],Cpu[i],ReCup[i],ImCup[i],171);
        else if(fileName.Contains("173p"))
            file.Form("ci%d%d%d%d_%d.5.root",Cpq3[i],Cpu[i],ReCup[i],ImCup[i],173);
        else
            file.Form("ci%d%d%d%d_%d.5.root",Cpq3[i],Cpu[i],ReCup[i],ImCup[i],172);
        TFile* fhist=TFile::Open(dir+file);
        hist_m[i]=(TH2F*)fhist->Get("id7");
        hist_y[i]=(TH2F*)fhist->Get("id13");
    }
    fileName="new_"+fileName+".root";
    TFile *file=new TFile(indir+fileName,"update");
    TTree *mytree=(TTree*) file->Get("mytree");
    Float_t M_tt, delta_rapidity;
    mytree->SetBranchAddress("M_tt_gen",&M_tt);
    mytree->SetBranchAddress("delta_rapidity_gen",&delta_rapidity);      
    Float_t weight_m[5], weight_y[5];
    TBranch *branch_m[5], *branch_y[5];
    for(int i=0; i<5; i++){
        TString weight_name_m=Form("mtt_ci%d%d%d%d",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        branch_m[i]=mytree->Branch(weight_name_m,&weight_m[i],weight_name_m+"/F");
        TString weight_name_y=Form("ytt_ci%d%d%d%d",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        branch_y[i]=mytree->Branch(weight_name_y,&weight_y[i],weight_name_y+"/F");
    }  
    Int_t entries=mytree->GetEntries();
    cout<<"total number of events: "<<entries<<endl;
    for(Int_t i=0;i<entries;i++){
        mytree->GetEntry(i);
        for(Int_t i=0;i<5;i++){
            Int_t mbin=hist_m[i]->FindBin(M_tt);//oberflow will be 0.0
            weight_m[i]=1.0+hist_m[i]->GetBinContent(mbin);
            branch_m[i]->Fill();
            Int_t ybin=hist_y[i]->FindBin(delta_rapidity);//oberflow will be 0.0
            weight_y[i]=1.0+hist_y[i]->GetBinContent(ybin);
            branch_y[i]->Fill();
            //cout<<"weight[i]: "<<weight[i]<<endl;
        }
    }
    mytree->Write("",TObject::kOverwrite);
    delete mytree;
    file->Close();        
}
