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
/*void Floor(TH2D* h1){
    double value;
    for(int ix=0; ix<h1->GetNbinsX(); ix++){
        for(int iy=0; iy<h1->GetNbinsY(); iy++){
            value = h1->GetBinContent(ix+1, iy+1);
            if(abs(value) < 1e-4){
                value = 1e-4;
                h1->SetBinContent(ix+1, iy+1, value);
            }                
        }
    }
}*/
void derive_ratio(){
    const int nbinsx=14;
    const int nbinsy=40;
    const double ylow=-5.0;
    const double yhigh=5.0;
    double xbins[]={345,360,380,400,450,500,550,600,650,700,750,800,900,1200,2000};
    TFile* nnlo_file = TFile::Open("NNLO.root");
    TFile* ra_file = new TFile("ratio.root", "RECREATE");
    TFile* lo_file = TFile::Open("./EW_LO_none.root");
    TH2D* h2_nnlo = (TH2D*)nnlo_file->Get("NNLO");
    TH2D* h2_lo = (TH2D*)lo_file->Get("h2_LO");
    h2_lo->SetName("LO");
    h2_lo->Scale(477.478/h2_lo->GetSumOfWeights());

    //h2_lo->Smooth();
    //h2_nnlo->Smooth();
    //Floor(h2_lo);
    //Floor(h2_nnlo);
    //TH2D* h2_ratio =  new TH2D("ratio", "", nbinsx, xbins, nbinsy, ylow, yhigh);
    TH2D* h2_ratio = (TH2D*)h2_nnlo->Clone();
    h2_ratio->SetName("ratio");
    h2_ratio->Add(h2_lo, -1.0);
    h2_ratio->Divide(h2_nnlo);

    ra_file->cd();
    h2_lo->Write();
    h2_nnlo->Write();
    h2_ratio->Write();
    ra_file->Close();
}