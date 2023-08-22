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
#include<TPaveText.h>
using namespace std;
double p2weight=0.65/0.3;
void set0(TH1D* h1){
    for(int i=0; i<h1->GetNbinsX(); i++){
        h1->SetBinError(i+1,0);
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
void format_text(TPaveText* lumi){
    lumi->SetFillColor(0);
    lumi->SetTextFont(42);
    lumi->SetTextSize(0.05);
    lumi->SetTextColor(1);
    lumi->SetBorderSize(0);
}
void format_stack(THStack *hstack, TString xtitle, double max) {
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
    //hstack->GetYaxis()->SetRangeUser(0, max);
    //cout<<hstack->GetMaximum()<<endl;
    hstack->SetMaximum(max);
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
    //	pad2->SetTopMargin(0);
        pad2->SetTopMargin(0.03);
        pad2->SetBottomMargin(0.35);
        pad2->SetLeftMargin(0.15);
}
void format_th(TH1D* h1, TString xtitle){
    int ydivisions=505;
    h1->SetMarkerStyle(21);
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
    h1->GetYaxis()->SetRangeUser(0.0, 2.0);
}
void set_ratio(TH1D* h1, double a){
    for(int i=1; i<=h1->GetNbinsX(); i++)
        h1->SetBinContent(i, a);
}
void draw(TString cut_name, int g, int year, bool isEnriched, TString tex){
    TString Enrich_name[2] = {"", "_En"};
    TString legend[] = {"tt","DY","single top","V+jets","QCD"};
    TString legendd = "data";
    TString cg_n[] = {"A", "B", "C", "D"};
    TString cg = cg_n[g];
    TString xtitle[] = {"p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    TString title[] = {"mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    Double_t xup[] = {400, 250, 400, 7, 500, 1500, 3};
    Double_t xlow[] = {100, 0, 0, 3, 0, 300, -3};
    Int_t bins[]={20, 20, 20, 4, 20, 20, 20};
    int color[] = {97, 46, 9, 29, 209, kYellow, 93};

    TH1D *hist, *hmc;
    TH1D *hdata, *hdatad;
    TH1D* hratio[3];
    Double_t nums, events;
    TString path = Form("./output/%d/",year);
    TFile* file = TFile::Open(path+"QCD_"+cut_name+"_"+cg+Enrich_name[isEnriched]+"_CG.root");
    for(int var = 5; var < 7; var++){
        //if(i == 6)
        //    lep[0] = lep[0] + "*(mass_tt>500)";
        auto c2 = new TCanvas("c2", "c2", 8, 30, 650, 650);
        TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.32,0.95,0.97);
    	TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.02,0.95,0.32);
        c2->cd();
    	format_pad(pad1, pad2);
        TLegend *leg = new TLegend(0.70, .65, 1.00, .90);
        format_leg(leg);
        format_canvas(c2);
        hdata = (TH1D*)file->Get("data");
        THStack* hstack = new THStack("hstack", "");
        hmc = new TH1D("MC", "", bins[var], xlow[var], xup[var]);
        for (int k = 4; k >= 0; k--){
            hist = (TH1D*)file->Get(title[var]);
            hist->SetFillColor(color[k]);
            hstack->Add(hist);
            leg->AddEntry(hist, legend[k], "f");
            delete hist;
        }
        hdatad=(TH1D*)hdata->Clone();
        hdatad->SetName("datad");
        hdatad->Divide(hmc);
        set0(hdatad);
        set0(hdata);
        /*TPaveText *lt = new TPaveText(0.45,0.75,0.55,0.85,"NDC");
        format_text(lt);
        lt->AddText(tex);*/
        pad1->cd();
        hstack->Draw("hist");
        //lt->Draw("same");
        double max = hdata->GetMaximum()*1.3;
        format_stack(hstack, xtitle[var],max);
        leg->AddEntry(hdata, legendd, "p");
        pad1->cd();
        leg->Draw("same");
        hdata->Draw("PSame");
        hdata->SetMarkerSize(0.8);
        hdata->SetMarkerStyle(21);
    
        pad2->cd();
        gStyle->SetOptStat(0);
        format_th(hdatad,xtitle[var]);
        hdatad->Draw("P");
        
        pad2->cd();
        for(int r=0; r<3; r++){
            hratio[r]=new TH1D(Form("ratio%d",r),"",bins[var], xlow[var], xup[var]);
            set_ratio(hratio[r], 0.5*(r+1));
            hratio[r]->Draw("LSame");
            hratio[r]->SetLineStyle(7);
            hratio[r]->SetLineColor(1);
        }
    
        c2->Print(Form("./pdf/%d/", year)+title[var]+cut_name+"/"+cg+".pdf");
        for(int k=0; k<3; k++)
            delete hratio[k];
        delete hdata;
        delete hdatad;
        delete hmc;
        delete hstack;
        delete leg;
        delete pad1;
        delete pad2;
        delete c2;
    }
}
void draw_cg( int g, int year, bool isEnriched){
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString tex1[] = {"", "e","#mu"};
    TString tex2[] = {"region B", "region C","region D"};
    for(int i=0; i<4; i++)
        draw(cutsName[i], g, year, isEnriched, "");
    /*for(int i=0; i<3; i++){
        for(int j=1; j<3; j++){
            draw(dir[i], 1, j, tex1[j]+", "+"4+jets, "+tex2[i]);
            draw(dir[i], 2, j, tex1[j]+", "+"3jets, "+tex2[i]);
        }
    }*/
} 