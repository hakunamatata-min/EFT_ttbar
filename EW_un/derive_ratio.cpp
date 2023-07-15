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
void derive_ratio(){
    const int nbinsx=14;
    const int nbinsy=40;
    const double ylow=-5.0;
    const double yhigh=5.0;
    double xbins[]={345,360,380,400,450,500,550,600,650,700,750,800,900,1200,2000};
    TFile* nnlo_file = TFile::Open("NNLO.root");
    TFile* ra_file = new TFile("ratio.root", "RECREATE");
    TFile* lo_file = TFile::Open("./EW_LO.root");
    TH2D* h2_nnlo = (TH2D*)nnlo_file->Get("NNLO");
    TH2D* h2_lo = (TH2D*)lo_file->Get("h2_LO");
    h2_lo->Scale(477.478/h2_lo->GetSumOfWeights());
    TH2D* h2_ratio =  new TH2D("ratio", "", nbinsx, xbins, nbinsy, ylow, yhigh);
    double nnlo, lo, ratio;
    for(int i=0; i<h2_ratio->GetNbinsX(); i++){
        for(int j=0; j<h2_ratio->GetNbinsY(); j++){
            int bin = h2_ratio->GetBin(i+1, j+1);
            nnlo = h2_nnlo->GetBinContent(bin);
            lo = h2_lo->GetBinContent(bin);
            if(nnlo > 1e-4)
                ratio = (nnlo - lo)/nnlo;
            else
                ratio = 0;
            h2_ratio->SetBinContent(bin, ratio);
        }
    }
    ra_file->cd();
    h2_ratio->Write();
    ra_file->Close();
}