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
#include<TGraph.h>
#include<TGraphAsymmErrors.h>
#include<fstream>
#include<iostream>
#include<RooFit.h>
#include<RooRealVar.h>
#include<TKey.h>
using namespace std;
void smooth_div(TH1D* h1, TH1D *hd[2]){
    int nbins[2] = {16, 7};
    for(int i=0; i<nbins[0]; i++)
        hd[0]->SetBinContent(i+1, h1->GetBinContent(i+1));
    for(int i=0; i<nbins[1]; i++)
        hd[1]->SetBinContent(i+1, h1->GetBinContent(nbins[0]+i+1));
}
void smooth(TH1D* h1, TH1D* hn){
    TH1D *h1_d[2], *hn_d[2]; 
    int nbins[2] = {16, 7};
    for(int i=0; i<2; i++){
        h1_d[i] = new TH1D(Form("h1_d%d",i), "", nbins[i], 0, nbins[i]);
        hn_d[i] = new TH1D(Form("hn_d%d",i), "", nbins[i], 0, nbins[i]);
    }
    smooth_div(h1, h1_d);
    smooth_div(hn, hn_d);
    for(int i=0; i<2; i++){
        h1_d[i]->Divide(hn_d[i]);
        h1_d[i]->Smooth();
    }
    double ratio;
    double nom;
    for(int i=0; i<nbins[0]; i++){
        nom = hn_d[0]->GetBinContent(i+1);
        ratio = h1_d[0]->GetBinContent(i+1);
        h1->SetBinContent(i+1, nom*ratio);
    }
    for(int i=0; i<nbins[1]; i++){
        nom = hn_d[1]->GetBinContent(i+1);
        ratio = h1_d[1]->GetBinContent(i+1);
        h1->SetBinContent(nbins[0]+i+1, nom*ratio);
    }
}