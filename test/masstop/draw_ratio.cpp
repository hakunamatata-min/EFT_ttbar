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
void draw_ratio(){
    TString mtop[] = {"EW_kappa_mt172", "EW_kappa_mt171", "EW_kappa_mt173"};
    TString ratio[] = {"171/172", "173/172"};
    TString leg_s[] = {"mt171.5", "mt173.5"};
    TGraph* h1[3];
    TH1D* h2[2];
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
        }
        for(int m=0; m<2; m++){
            h2[m] = new TH1D(ratio[m], "", 6, -0.25, 2.75);
            for(int i=0; i<6; i++){
                h2[m]->SetBinContent(i+1, h1[m+1]->GetPointY(i)/h1[0]->GetPointY(i));
            }
            c2->cd();
            if(m==0){
                h2[m]->Draw("hist");
                //h1->SetStats(kFALSE);
                h2[m]->GetXaxis()->SetTitle("#kappa");
                h2[m]->GetYaxis()->SetTitle("ratio");
                h2[m]->SetTitle("");
                h2[m]->GetXaxis()->CenterTitle();
                h2[m]->GetYaxis()->CenterTitle();
                h2[m]->GetXaxis()->SetTitleSize(0.05);
                h2[m]->GetYaxis()->SetTitleSize(0.05);
                h2[m]->GetYaxis()->SetTitleOffset(1.4);
                h2[m]->GetYaxis()->SetRangeUser(0.98, 1.02);
                h2[m]->GetXaxis()->SetRangeUser(-0.25, 2.75);
            }
            else{
                h2[m]->Draw("hsame");
            }
            h2[m]->SetLineColor(color[m]);
            h2[m]->SetLineWidth(1);

            TLegendEntry* leg_entry=leg->AddEntry(h2[m],leg_s[m]);
        }
        c2->cd();
        leg->Draw("same");
        c2->Print(Form("ratio_bin%d.pdf", bin));
        delete h2[0];
        delete h2[1];
    }   
}
