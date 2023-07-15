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
double p2weight=1;
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
void format_leg(TLegend* leg){
    leg->SetNColumns(1);
    leg->SetColumnSeparation(0.5);
    leg->SetEntrySeparation(0.0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.03);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
}
void format_th(TH1D* h1, TString xtitle){
    //int ydivisions=505;
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(0.4);
    h1->SetStats(kFALSE);
    h1->GetXaxis()->SetTitle(xtitle);
    h1->GetYaxis()->SetTitle("Events/bin");
    h1->SetTitle("");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    //h1->GetYaxis()->SetNdivisions(ydivisions);
    h1->GetXaxis()->SetTitleSize(0.06);
    h1->GetYaxis()->SetTitleSize(0.06);
    h1->GetXaxis()->SetTitleOffset(1.0);
    h1->GetYaxis()->SetTitleOffset(1.20);
    h1->GetXaxis()->SetLabelSize(0.03);
    h1->GetYaxis()->SetLabelSize(0.04);
    h1->GetYaxis()->SetRangeUser(0.0, h1->GetMaximum()*1.5);
    //h1->GetYaxis()->SetRangeUser(-0.2, 0.25);
}
void draw_pre(TString behind, TString dir){
    TString cg[] = {"_B", "_C", "_D"};
    TString cg_n[] = {"B", "C", "D"};
    TString title[] = {"top_pt", "Mtt", "deltay"};
    int color[] = {2, 1, 4};
    TFile* file;
    TH1D* h1;
    TString xtitle[] = {"p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    for(int i=0; i<3; i++){
        auto c2 = new TCanvas("c2", "c2", 8, 30, 650, 650);
        c2->cd();
        TLegend *leg = new TLegend(0.70, .65, 1.00, .90);
        format_leg(leg);
        format_canvas(c2);
        for(int f=0; f<3; f++){
            file = new TFile("./QCD_1D/QCD"+behind+cg[f]+".root");
            h1 = (TH1D*)file->Get(title[i]);
            c2->cd();
            cout<<h1->GetSumOfWeights()<<endl;
            if(f==0){
                h1->Draw("hist");
                format_th(h1, xtitle[i]);
            }
            else
                h1->Draw("histSame");
            h1->SetLineColor(color[f]);
            leg->AddEntry(h1, cg_n[f]);
            //delete h1;
            //delete file;
        }
        c2->cd();
        leg->Draw();
        c2->Print(dir+title[i]+".pdf");
        delete leg;
        delete c2;
    }
}
void draw_cg_diff(){
    TString behind[] = {"_4jets", "_3jets", "_ s4jets"};
    TString dir[] = {"./CG/CG_4jets/", "./CG/CG_3jets/", "CG/"};
    for(int i=0; i<2; i++)
        draw_pre(behind[i], dir[i]);
}