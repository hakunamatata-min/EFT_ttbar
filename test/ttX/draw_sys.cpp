#include<TROOT.h>
#include<TLine.h>
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
#include<TPaveText.h>
using namespace std;
double p2weight=0.65/0.3;
void seterror0(TH1D* h1){
    for(int i=1; i<=h1->GetNbinsX(); i++){
        h1->SetBinError(i,0);
    }
}
void format_text(TPaveText* lumi){
    lumi->SetFillColor(0);
    lumi->SetTextFont(42);
    lumi->SetTextSize(0.035);
    lumi->SetTextColor(13);
    lumi->SetBorderSize(0);
}
void format_leg(TLegend* leg){
    leg->SetNColumns(1);
    leg->SetColumnSeparation(0.5);
    leg->SetEntrySeparation(0.0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(2);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
}
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
void format_pad(TPad* pad1, TPad* pad2){
    pad1->Draw();
    pad2->Draw();
    pad1->cd();
    pad1->SetTopMargin(0.1);
    pad1->SetBottomMargin(0.02);
    pad1->SetLeftMargin(0.15);
    //pad1->SetTopMargin(0);
    pad2->cd();
    //pad2->SetTopMargin(0);
    pad2->SetTopMargin(0.03);
    pad2->SetBottomMargin(0.45);
    pad2->SetLeftMargin(0.15);
}
void format_th_pad2(TH1D* h1, TString xtitle, double* low, double* high, int s){
    int ydivisions=505;
    double range[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0.25, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0.09, 0.03,
                      0, 0 ,0 ,0};
    float up = h1->GetMaximum();
    float down = h1->GetMinimum();
    up = fabs(up-1);
    down = fabs(down-1);
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(0.4);
    h1->SetStats(kFALSE);
    h1->GetXaxis()->SetTitle(xtitle);
    h1->GetYaxis()->SetTitle("Ratio");
    h1->SetTitle("");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetNdivisions(ydivisions);
    h1->GetXaxis()->SetTitleSize(0.12);
    h1->GetYaxis()->SetTitleSize(0.06*p2weight);
    h1->GetXaxis()->SetTitleOffset(1.8);
    h1->GetYaxis()->SetTitleOffset(1.1/p2weight);
    h1->GetXaxis()->SetLabelSize(0.12);
    h1->GetYaxis()->SetLabelSize(0.05*p2weight);
    if(range[s] != 0){
        h1->GetYaxis()->SetRangeUser(-range[s], +range[s]);
        *low = -range[s];
        *high = range[s];
        return;
    }
    if(up>0.1 || down>0.1){
        h1->GetYaxis()->SetRangeUser(-0.15,0.15);
        *low = -0.15;
        *high = 0.15;
        return;
    }
    if(up > down){
        h1->GetYaxis()->SetRangeUser(-1.5*up, +1.5*up);
        *low = -1.3*up;
        *high = +1.3*up;
    }  
    else{
        h1->GetYaxis()->SetRangeUser(-1.5*down, +1.5*down);
        *low = -1.3*down;
        *high = +1.3*down;
    }
}
double format_th_pad1(TH1D* h1, TString xtitle){
    int ydivisions=505;
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(0.4);
    h1->SetStats(kFALSE);
    h1->GetXaxis()->SetTitle(xtitle);
    h1->GetYaxis()->SetTitle("Events/bin");
    h1->SetTitle("");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetNdivisions(ydivisions);
    h1->GetXaxis()->SetTitleSize(0.0);
    h1->GetYaxis()->SetTitleSize(0.06);
    h1->GetXaxis()->SetTitleOffset(1.0);
    h1->GetYaxis()->SetTitleOffset(1.20);
    h1->GetXaxis()->SetLabelSize(0.0);
    h1->GetYaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetRangeUser(0.0, 80000);
    return h1->GetMaximum();
    //h1->GetYaxis()->SetRangeUser(0.0, 800000);
}
void format_line(TLine* l1){
    l1->SetLineStyle(9);
    l1->SetLineWidth(2);
    l1->SetLineColor(1);
}
void set_th_lable(TH1D* h1, int* nbins){
    double xbins[][20] = {{300,340,360,380,400,420,440,460,480,500,520,540,570,600,640,700,3000}, 
                          {300,450,500,570,600,700,820,3000}};
    h1->SetLabelSize(0.11);
    for(int i=0; i<2; i++){
        for(int j=nbins[i]; j<nbins[i+1]; j++){
            h1->GetXaxis()->SetBinLabel(j+1, TString::Format("%.0f-%.0f", xbins[i][j-nbins[i]], xbins[i][j-nbins[i]+1]));
        }
    }
    h1->LabelsDeflate("X");
    h1->LabelsDeflate("Y");
    h1->LabelsOption("v");
}
void draw_pre(TString cutname, int year){
    double low, high;
    TString sys, sys_path;

    TString syss[] = {"jes","jer","unclus","SF_lepton","SF_btag", Form("SF_btag%d", year), "SF_ltag", Form("SF_ltag%d", year), "pdf", "alphas", "L1PF", "PU", "muR1","muF1","muR2","muF2","muR3","muF3","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un", "qcds"};
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    TString process[]={"ttbar_ci0000", "ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "ttbar_ci0200", "DYJets","STop", "WJets", "QCD"};
    TString sys_paths[] = {"jes/","jer/","unclus/","sl/", "sb_co/","sb_un/","sl_co/","sl_un/","pdf/","alphas/", "L1PF/", "PU/", "muR/","muF/","muR/","muF/","muR/","muF/","ISR/","FSR/","mtop/","hdamp/","TuneCP5/","nnlo/", "EW_un/", "qcds/"};
    TString jes_source_paths[] = {"Absolute/", "Absolute_un/", "FlavorQCD/", "BBEC1/", "EC2/", "HF/", "BBEC1_un/", "EC2_un/", "RelativeBal/", "RelativeSample_un/"};
    TString filename = "ttbar_"+cutname+Form("_%d.root", year);
    TString inpath = "../../combine/";
    TString outpath = "./sys_pdf/";
    TFile* file = TFile::Open(inpath+"datacard_ttx/"+filename);
    TH1D *hmc1, *hmc2, *hmc3;
    TH1D *hd1, *hd2, *hd3;
    int color[] = {2, 1, 4};
    TString xtitle = "M_{t#bar{t}}";
    TString legend[] = {"nom", "up", "down"};
    int div = 16;
    int nbins[] = {0, 16, 23};
    TString cut[2] = {"|#Deltay|<1.4", "|#Deltay|>1.4"};
    TLine *l1, *l2;
    TPaveText *t[2];
    //if(h1 == NULL)
    //    cout<<"right"<<endl;
    for(int c=0; c<8; c++){
        for(int s=0; s<34; s++){
            if(s < 10){
                sys = "jes_" + jes_source[s];
                sys_path = "jes_"  + jes_source_paths[s];
            }
            else{
                sys = syss[s-9];
                sys_path = sys_paths[s-9];
            }
            TCanvas* c2 = new TCanvas("c1", "c1", 8, 30, 650, 650);
            TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.32,0.95,0.97);
            TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.02,0.95,0.32);
            c2->cd();
            format_pad(pad1, pad2);
            TLegend *leg = new TLegend(0.70, .60, 0.95, .75);
            format_leg(leg);
            format_canvas(c2);
            hmc1 = (TH1D*)file->Get(process[c]);
            hmc2 = (TH1D*)file->Get(process[c]+"_"+sys+"Up");
            hmc3 = (TH1D*)file->Get(process[c]+"_"+sys+"Down");
            if(hmc2 == NULL){
                delete pad1; delete pad2; delete leg;
                delete hmc1; delete hmc2; delete hmc3;
                delete c2;
                continue;
            }
            TH1D* hd = (TH1D*)hmc1->Clone();
            hd1 = (TH1D*)hmc1->Clone();
            hd2 = (TH1D*)hmc2->Clone();
            hd3 = (TH1D*)hmc3->Clone();
            hd1->Add(hd, -1);
            hd2->Add(hd, -1);
            hd3->Add(hd, -1);
            hd1->Divide(hd);
            hd2->Divide(hd);
            hd3->Divide(hd);

            pad1->cd();
            hmc1->Draw("hist");
            double high1 = format_th_pad1(hmc1, xtitle);
            hmc1->SetLineColor(color[0]);
            hmc2->Draw("histSame");
            hmc2->SetLineColor(color[1]);
            hmc3->Draw("histSame");
            hmc3->SetLineColor(color[2]);
            hmc1->SetLineWidth(2);
            hmc2->SetLineWidth(2);
            hmc3->SetLineWidth(2);
            leg->AddEntry(hmc1, legend[0], "l");
            leg->AddEntry(hmc2, legend[1], "l");
            leg->AddEntry(hmc3, legend[2], "l");
            leg->Draw("Same");
            
            l1 = new TLine(div, 0, div, high1);
            format_line(l1);
            pad1->cd();
            l1->Draw("same");

            for(int tex=0; tex<2; tex++){
                t[tex] = new TPaveText(0.35+0.38*tex, 0.77, 0.44+0.38*tex, 0.87, "NDC");
                format_text(t[tex]);
                t[tex]->AddText(cut[tex]);
                t[tex]->Draw("same");
            }

            pad2->cd();
            hd2->Draw("hist");
            hd2->SetLineColor(color[1]);
            hd2->SetLineWidth(2);
            format_th_pad2(hd2, xtitle, &low, &high, s);
            set_th_lable(hd2, nbins);
            seterror0(hd2);
            hd3->Draw("hSame");
            seterror0(hd3);
            hd3->SetLineColor(color[2]);
            hd3->SetLineWidth(2);
            hd1->Draw("hSame");
            seterror0(hd1);
            hd1->SetLineStyle(9);
            hd1->SetLineColor(color[0]);
            hd1->SetLineWidth(2);
            l2 = new TLine(div, low, div, high);
            format_line(l2);
            l2->Draw("same");

            c2->Print(outpath+sys_path+Form("%d/", year)+cutname+"/"+process[c]+".pdf");
            delete l1; delete l2;
            for(int tex=0; tex<2; tex++)
                delete t[tex];
            delete leg;
            delete hmc1;
            delete hmc2;
            delete hmc3;
            delete hd1;
            delete hd2;
            delete hd3;
            delete pad1; delete pad2;
            delete c2;
        }
    }
}
void draw_sys(){
    TString cutname[] = {"M_4jets","M_3jets","E_4jets","E_3jets"};
    int years[] = {2015, 2016, 2017, 2018};
    //TString filenames[] = {"ttbar_M_4jets.root","ttbar_M_3jets.root","ttbar_E_4jets.root","ttbar_E_3jets.root"};
    for(int i=0; i<4; i++){
        for(int y=0; y<4; y++){
            draw_pre( cutname[i], years[y]);
        }
    }
}