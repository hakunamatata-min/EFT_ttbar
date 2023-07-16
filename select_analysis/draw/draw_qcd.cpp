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
using namespace std;

double p2weight=0.65/0.3;
void sete0(TH1D* h1){
    for(int i=0; i<h1->GetNbinsX(); i++){
        h1->SetBinError(i,0);
    }
}
void get_error(TString var_name, int p, double* up_vs, double* down_vs, TString file_name){
    TString pro[] = {"_TT","_DY","_ST","_WJets"};
    TString sys[] = {"_jes","_jer","_unclus","_sl","_sj","_L1PF","_muR","_muF","_ISR","_FSR","_mtop","_hdamp","_TuneCP5","_nnlo"};
    
    
    
    TFile *file = TFile::Open(file_name);
    TH1D *up, *down, *h1;
    double nom_v;
    double up_v, up_max;
    double down_v, down_min;
    for(int s=0; s<14; s++){
        if(p>0 && s>9)
            continue;
        //cout<<p<<" "<<s<<endl;
        up = (TH1D*)file->Get(var_name+pro[p]+sys[s]+"_up");
        down = (TH1D*)file->Get(var_name+pro[p]+sys[s]+"_down");
        h1 = (TH1D*)file->Get(var_name+pro[p]+"_nom");
        const int n = h1->GetNbinsX();
        for(int b=0; b<n; b++){
            up_v = up->GetBinContent(b+1);
            down_v = down->GetBinContent(b+1);
            nom_v = h1->GetBinContent(b+1);
            up_max = max(up_v, down_v);
            down_min = min(up_v, down_v);
            if(up_max <= nom_v){
                up_vs[b] += 0;
                down_vs[b] += (nom_v - down_min) * (nom_v - down_min);
            }
            else if(down_min >= nom_v){
                up_vs[b] += (up_max- nom_v) * (up_max - nom_v);
                down_vs[b] += 0;
            }
            else{
                up_vs[b] += (up_max- nom_v) * (up_max - nom_v);
                down_vs[b] += (nom_v - down_min) * (nom_v - down_min);
            }
        }
    }
}
void set_rel_error(TGraphAsymmErrors* hg, TH1D* h1, TString var_name, double bin_len, TString sys_file){
    const int n = h1->GetNbinsX();
    double up_vs[n], down_vs[n];
    double nom;
    for(int i=0; i<n; i++){
        up_vs[i] = 0;
        down_vs[i] = 0;
    }
    for(int p=0; p<1; p++){
        get_error(var_name, p, up_vs, down_vs, sys_file);
    }
    for(int i=0; i<n; i++){
        nom = h1->GetBinContent(i+1);
        hg->SetPointEXlow(i, bin_len/2.0);
        hg->SetPointEXhigh(i, bin_len/2.0);
        hg->SetPointEYhigh(i, sqrt(up_vs[i])/nom);
        hg->SetPointEYlow(i, sqrt(down_vs[i])/nom);
    }
}
void set_abs_error(TGraphAsymmErrors* hg, TH1D* h1, TString var_name, double bin_len, TString sys_file){
    const int n = h1->GetNbinsX();
    double up_vs[n], down_vs[n];
    for(int i=0; i<n; i++){
        up_vs[i] = 0;
        down_vs[i] = 0;
    }
    for(int p=0; p<4; p++){
        get_error(var_name, p, up_vs, down_vs, sys_file);
    }
    for(int i=0; i<n; i++){
        hg->SetPointEXlow(i, bin_len/2.0);
        hg->SetPointEXhigh(i, bin_len/2.0);
        hg->SetPointEYhigh(i, sqrt(up_vs[i]));
        hg->SetPointEYlow(i, sqrt(down_vs[i]));
    }
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
void format_stack(THStack *hstack, TString xtitle) {
    hstack->GetXaxis()->SetNdivisions(505);
    hstack->GetXaxis()->SetLabelFont(42);
    hstack->GetXaxis()->SetLabelOffset(0.007);
    hstack->GetXaxis()->SetLabelSize(0.00);
    hstack->GetXaxis()->SetTitleSize(0.04);
    hstack->GetXaxis()->SetTitleOffset(1.3);
    hstack->GetXaxis()->SetTitleFont(42);
    hstack->GetYaxis()->SetNdivisions(505);
    hstack->GetYaxis()->SetLabelFont(42);
    hstack->GetYaxis()->SetLabelOffset(0.007);
    hstack->GetYaxis()->SetLabelSize(0.05);
    hstack->GetYaxis()->SetTitleSize(0.06);
    hstack->GetYaxis()->SetTitleOffset(1.1);
    hstack->GetYaxis()->SetTitleFont(42);
    hstack->GetXaxis()->CenterTitle();
    hstack->GetYaxis()->CenterTitle();
    hstack->GetXaxis()->SetTitle("");
    hstack->GetYaxis()->SetTitle("Events/bin");
    //cout<<hstack->GetMaximum()<<endl;
    //hstack->GetYaxis()->SetRangeUser(0, hstack->GetMaximum() * 1.4);
    hstack->SetMaximum(hstack->GetMaximum() * 1.4);
}
void format_pad(TPad* pad1, TPad* pad2, TPad* pad3){
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();
    pad1->cd();
    pad1->SetTopMargin(0.1);
    pad1->SetBottomMargin(0.02);
    pad1->SetLeftMargin(0.15);
    //pad1->SetTopMargin(0);
    pad2->cd();
    //pad2->SetTopMargin(0);
    pad2->SetTopMargin(0.03);
    pad2->SetBottomMargin(0.35);
    pad2->SetLeftMargin(0.15);

    pad3->cd();
    //pad2->SetTopMargin(0);
    pad3->SetTopMargin(0.03);
    pad3->SetBottomMargin(0.35);
    pad3->SetLeftMargin(0.15);
    pad3->SetFillColor(0);
    pad3->SetFillStyle(4000);
    pad3->SetFrameFillStyle(0);
}
void format_th(TH1D* h1, TString xtitle){
    int ydivisions=505;
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
    h1->GetXaxis()->SetTitleOffset(1.0);
    h1->GetYaxis()->SetTitleOffset(1.1/p2weight);
    h1->GetXaxis()->SetLabelSize(0.11);
    h1->GetYaxis()->SetLabelSize(0.05*p2weight);
    h1->GetYaxis()->SetRangeUser(0.5, 1.5);
}
void draw_pre(int c, int l, int year, TString width){//2, 0
    TString legend[] = {"tt","DY","single top","V+jets","QCD"};
    TString pro[] = {"_TT", "_DY", "_ST", "_WJets"};
    TString legendd = "data";
    TString xtitle[] = {"lnL","M_{t}","M_{#bar{t}}","M_{Wl}","M_{Wh}","M_{th}","M_{tl}","P_{T}^{l}","P_{T}^{leading-jet}","jet_num","p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    TString title[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    TString xvars[] = {"likelihood","mass_t","mass_at","mass_wlep","mass_whad","mass_thad","mass_tlep","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt", "rapidity_tt"};
    Double_t xup[] = {50, 450, 450, 140, 140, 450, 450, 250, 400, 7, 500, 1500, 4};
    Double_t xdown[] = {13, 50, 50, 50, 50, 50, 50, 0, 0, 3, 50, 200, -4};
    Int_t bins[]={37, 40, 40, 36, 36, 40, 40, 20, 20, 4, 20, 20, 16};
    float lumi=59.83;
    int is_draw[] = {0, 1, 0, 0, 0,};
    TString path = Form("../%d/",year);
    int edge[]={0,3,11,16,20,29};//23,31}; 
    int color[] = {2, 46, 9, 29, 8, kYellow, 93};
    TString cate[] = {"1","(jet_num==3)","(jet_num>=4)"};
    TString cate_name[] = {"_semi","_3jets","_4jets"};
    TString lep[] = {"*1","*(!lep_flavour)","*(lep_flavour)"};
    TString lep_name[] = {"","_E","_M"};
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    //TString other_con2 = "*1";
    TString sys_file = path+"sys_root/sys"+lep_name[l]+cate_name[c]+width+"_.root";
    TFile *file = TFile::Open(sys_file);
    TFile* qcd = TFile::Open(path+"QCD_ES/QCD_1D/QCD"+lep_name[l]+cate_name[c]+"_C.root");
    //cout<<lep_name[l]+cate_name[c]<<endl;
    TChain *mytree;
    TH1D *nmc, *hist, *h1[5];
    TH1D *hdata, *hdatad, *hmc, *hmcd;
    TGraphAsymmErrors *hmcdg, *hmcg;
    Double_t nums, events;
    Double_t bin_len;
    for(int i = 0; i < 5; i++){
        //if(i == 5)
        //    cate[cate_num] = cate[cate_num] + "*(mass_tt>500)";
        //if(i>=2 && i<10)
        //    continue;
        if(!is_draw[i])
            continue;
        bin_len = (xup[i] - xdown[i])/(1.0 * bins[i]);
        auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600); // temporary canvas
        auto c2 = new TCanvas("c2", "c2", 8, 30, 650, 650);
        TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.32,0.95,0.97);
        TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.02,0.95,0.32);
        TPad *pad3 = new TPad("pad3","This is pad3",0.05,0.02,0.95,0.32);
        c2->cd();
        format_pad(pad1, pad2, pad3);
        TLegend *leg = new TLegend(0.70, .65, 1.00, .90);
        format_leg(leg);
        format_canvas(c2);
        TChain* tree2 = new TChain("mytree");
        tree2->Add(path+"/data/new_data_*.root");
        hdata = new TH1D("data", "", bins[i], xdown[i], xup[i]);
        hdatad = new TH1D("datad", "", bins[i], xdown[i], xup[i]);
        tree2->Draw(xvars[i]+">>data",cate[c]+other_con1+other_con2+lep[l]);
        tree2->Draw(xvars[i]+">>datad",cate[c]+other_con1+other_con2+lep[l]);
        delete tree2;
        THStack* hstack = new THStack("hstack", "");
        nums = 0;
        events = hdata->GetSumOfWeights();
        h1[4] = (TH1D*)qcd->Get(title[i]);
        h1[4]->SetFillColor(color[4]);
        hstack->Add(h1[4]);
        //cout<<legend[4]<<": "<<h1[4]->GetSumOfWeights()<<endl;
        hmc = (TH1D*)h1[4]->Clone();
        nums+=h1[4]->GetSumOfWeights();
        for (int k = 3; k >= 0; k--){
            h1[k] = (TH1D*)file->Get(title[i]+pro[k]+"_nom");
            //cout<<legend[k]<<": "<<h1[k]->GetSumOfWeights()<<endl;
            hmc->Add(h1[k]);
            nums+=h1[k]->GetSumOfWeights();
            h1[k]->SetFillColor(color[k]);
            hstack->Add(h1[k]);
        }
        hmcd=(TH1D*)hmc->Clone();
        sete0(hmc);
        hdatad->Divide(hmc);
        hmcd->Divide(hmc);
        hmcdg = new TGraphAsymmErrors(hmcd);
        hmcg = new TGraphAsymmErrors(hmc);
        set_rel_error(hmcdg, hmc, title[i], bin_len, sys_file);
        set_abs_error(hmcg, hmc, title[i], bin_len, sys_file);
        cout<<nums<<" "<<events<<endl;
        pad1->cd();
        hstack->Draw("hist");
        format_stack(hstack, xtitle[i]);
        for(int k=0;k<5;k++)
            leg->AddEntry(h1[k], legend[k], "f");
        leg->AddEntry(hdata,legendd,"p");
        pad1->cd();
        leg->Draw("same");
        hdata->Draw("PSame");
        hdata->SetMarkerSize(0.8);
        hdata->SetMarkerStyle(21);
        hmcg->Draw("2Same");
        hmcg->SetFillColor(1);
        hmcg->SetFillStyle(3005);

        pad2->cd();
        gStyle->SetOptStat(0);
        format_th(hdatad,xtitle[i]);
        hdatad->Draw("P");        
        pad2->cd();
        hmcdg->Draw("2Same");
        hmcdg->SetFillColor(1);
        hmcdg->SetFillStyle(3005);
        hmcd->Draw("LSame");
        sete0(hmcd);
        hmcd->SetLineStyle(7);
        hmcd->SetLineColor(2);
        
        pad3->cd();
        gStyle->SetOptStat(0);
        TH1D* hc = (TH1D*)hdatad->Clone();
        hc->SetName("clone");
        hc->Draw("PY+"); 
        hc->GetYaxis()->SetTitleSize(0);
        c2->Print(path+"output_qcd/"+title[i]+lep_name[l]+cate_name[c]+width+".pdf");
        for(int k=0;k<5;k++)
            delete h1[k];
        delete hdata;
        delete hdatad;
        delete hmc;
        delete hmcd;
        delete hmcg;
        delete hmcdg;
        delete hstack;
        delete leg;
        delete pad1;
        delete pad2;
        delete pad3;
        delete c1;
        delete c2;
    }
}
void draw_qcd(){
    for(int i=1; i<3; i++){
        for(int j=1; j<3; j++){
            draw_pre(i,j,2018, "_widthx0p55");
            draw_pre(i,j,2018, "_widthx1p45");
        }
    }
}