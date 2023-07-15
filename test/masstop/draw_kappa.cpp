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
#include<TCanvas.h>
#include<TGraph.h>
#include<TLegend.h>
#include<iostream>
using namespace std;
void format_canvas(TCanvas* c){
    c->SetFillColor(0);
    c->SetBorderMode(0);
    c->SetBorderSize(2);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLeftMargin(0.17);
    c->SetRightMargin(0.12);
    c->SetTopMargin(0.07);
    c->SetBottomMargin(0.13);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
}
void format_leg(TLegend* leg){
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.033);
    leg->SetNColumns(2);
}
void draw_kappa(){
    TString mtop[] = {"EW_kappa_mt171", "EW_kappa_mt172", "EW_kappa_mt173"};
    TString leg_s[] = {"mt171.5", "mt172.5", "mt173.5"};
    TGraph* h1[3];
    int color[]={2,1,4,226,6,kOrange+2,kViolet+1,kAzure+10,93};
    TFile* file = TFile::Open("EW_mtop.root");
    for(int bin=1; bin<6; bin++){
        auto c2=new TCanvas("c2","",30, 30, 600, 450); //draw on this canvas
        format_canvas(c2);
        c2->cd();
        auto leg=new TLegend(.28,.65,.85,.90);	
        format_leg(leg);
        for(int m=0; m<3; m++){
            h1[m] = (TGraph*)file->Get(mtop[m]+Form("_bin%d", bin));
            c2->cd();
            if(m==0){
                h1[m]->Draw("PAC");
                //h1->SetStats(kFALSE);
                h1[m]->GetXaxis()->SetTitle("#kappa");
                h1[m]->GetYaxis()->SetTitle("R_{EW}");
                h1[m]->SetTitle("");
                h1[m]->GetXaxis()->CenterTitle();
                h1[m]->GetYaxis()->CenterTitle();
                h1[m]->GetXaxis()->SetTitleSize(0.05);
                h1[m]->GetYaxis()->SetTitleSize(0.05);
                h1[m]->GetYaxis()->SetTitleOffset(1.4);
                h1[m]->GetYaxis()->SetRangeUser(0.90, 1.25);
                h1[m]->GetXaxis()->SetRangeUser(-0.25, 2.75);
            }
            else{
                h1[m]->Draw("PCsame");
            }
            h1[m]->SetLineColor(color[m]);
            h1[m]->SetLineWidth(1);
            h1[m]->SetMarkerStyle(kCircle);
            h1[m]->SetMarkerSize(0.6);
            TLegendEntry* leg_entry=leg->AddEntry(h1[m],leg_s[m]);
        }
        c2->cd();
        leg->Draw("same");
        c2->Print(Form("R_EW_bin%d.pdf", bin));
    }   
}
