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
void un_weight_add(TString input){
    Double_t nnlo_ratio;
    Float_t M_tt_gen, delta_rapidity_gen;
    //TString dir = "/afs/cern.ch/user/y/yuekai/ttbar/condor/EW_un";
    TString dir = "./";
    Int_t Cpq3[9]={ 0, 0, 0, 0, 0, 2, 0, 0, 0 };
    Int_t Cpu[9]={  0, 2, 1, 0, 0, 0, 2, 0, 0 };
    Int_t ReCup[9]={0, 0, 0, 1, 0, 0, 0, 2, 0 };
    Int_t ImCup[9]={0, 0, 0, 0, 1, 0, 0, 0, 2 };  
    Float_t weight_un_up[9], weight_un_dn[9];
    TBranch *un_up[9], *un_dn[9];
    Float_t weight_nom[9];
    TFile* file = new TFile(input, "update");
    cout<<"add EW uncertainties on mytree"<<endl;
    TTree* mytree = (TTree*)file->Get("mytree");
    //TBranch* nnlo = mytree->Branch("nnlo_ratio", &nnlo_ratio, "nnlo_ratio/D"); 
    mytree->SetBranchAddress("M_tt_gen", &M_tt_gen);
    mytree->SetBranchAddress("delta_rapidity_gen", &delta_rapidity_gen);
    TFile* ra_file = TFile::Open(dir+"ratio.root");
    TH2D* h2_ratio = (TH2D*)ra_file->Get("ratio");
    for(int i=0; i<5; i++){
        TString weight_name_nom=Form("weight_ci%d%d%d%d",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        TString weight_name_up=Form("weight_ci%d%d%d%d_EWUp",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        TString weight_name_dn=Form("weight_ci%d%d%d%d_EWDown",Cpq3[i],Cpu[i],ReCup[i],ImCup[i]);
        un_up[i] = mytree->Branch(weight_name_up, &weight_un_up[i], weight_name_up+"/F");
        un_dn[i] = mytree->Branch(weight_name_dn, &weight_un_dn[i], weight_name_dn+"/F");
        mytree->SetBranchAddress(weight_name_nom, &weight_nom[i]); 
    }
    for(int i=0; i<mytree->GetEntries(); i++){
        mytree->GetEntry(i);
        if(M_tt_gen<=2000 && M_tt_gen>=345){
            int bin = h2_ratio->FindBin(M_tt_gen, delta_rapidity_gen);
            nnlo_ratio = h2_ratio->GetBinContent(bin);
        }
        else
            nnlo_ratio = 0;
        for(int b=0; b<5; b++){
            weight_un_up[b] = 1 + nnlo_ratio*(weight_nom[b]-1);
            weight_un_dn[b] = 1 - nnlo_ratio*(weight_nom[b]-1);
            un_up[b]->Fill();
            un_dn[b]->Fill();
        }
    }
    file->cd();
    mytree->Write("",TObject::kOverwrite);
    file->Close();
}