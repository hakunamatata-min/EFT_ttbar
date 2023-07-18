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
void format_canvas(TCanvas *c) {
    c->SetFillColor(0);
    c->SetBorderMode(0);
    c->SetBorderSize(2);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLeftMargin(0.18);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.15);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
}
void check(){
    TFile* f1 = TFile::Open("EW_LO_none.root");
    TFile* f2 = TFile::Open("EW_LO.root");
    TH2F *h2_lo_none, *h2_lo, *diff, *diff_rel;
    TFile *file = new TFile("Diff.root", "recreate");
    h2_lo = (TH2F*)f2->Get("h2_LO");
    h2_lo->SetName("virt_exact_lo");
    h2_lo_none = (TH2F*)f1->Get("h2_LO");
    h2_lo_none->SetName("lo_none");
    diff = (TH2F*)h2_lo->Clone();
    diff->SetName("Diff");
    diff_rel = (TH2F*)h2_lo->Clone();
    diff_rel->SetName("Diff_rel");
    diff->Add(h2_lo_none, -1.0);
    diff_rel->Add(h2_lo_none, -1.0);
    diff_rel->Divide(h2_lo_none);
    file->cd();
    h2_lo->Write();
    h2_lo_none->Write();
    diff->Write();
    diff_rel->Write();
    file->Close();
}