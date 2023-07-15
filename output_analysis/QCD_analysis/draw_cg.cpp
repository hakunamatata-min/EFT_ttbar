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
void draw(int dir_num, int c, int l, TString tex){
    const int nsample = 29;
    TString fileNames[nsample] = {  
                            "new_TTToSemiLeptonic_TuneCP5_13TeV-powheg.root",
                            "new_TTTo2L2Nu_TuneCP5_13TeV-powheg.root",
                            "new_TTToHadronic_TuneCP5_13TeV-powheg.root",

                            "new_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                            "new_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM.root",                           
                            "new_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM.root",
                        
                            "new_ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo.root",
                            "new_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                            "new_ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin.root",
                            "new_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",
                            "new_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg.root",                                                                               
                            
                            "new_W1JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            "new_W2JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            "new_W3JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            "new_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            //"new_WJetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            /*"new_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM.root",
                            "new_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM.root",*/

                            "new_QCD_HT50to100_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT100to200_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT200to300_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT300to500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT500to700_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT700to1000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1000to1500_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT1500to2000_TuneCP5_PSWeights_13TeV-madgraph.root",
                            "new_QCD_HT2000toInf_TuneCP5_PSWeights_13TeV-madgraph.root",};
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3,	
                                    //53870.0,
                                    //1264.0,1345.7, 359.7, 48.9, 12.1, 5.5, 1.3, 0.032,//LO
                                    186100000.0, 23590000, 1555000, 324500, 30310, 6444, 1127, 109.8, 21.98};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21,//1.21,1.21,1.21,1.21,
                                0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    TString legend[] = {"tt","DY","single top","V+jets","QCD"};
    TString legendd = "data";
    TString xtitle[] = {"M_{tl}","P_{T}^{l}","P_{T}^{leading-jet}","jet_num","p_{T}^{t}","M_{t#bar{t}}","#Deltay_{t#bar{t}}"};
    TString title[] = {"mass_tlep","lepton_pt","leading_pt","jet_num","top_pt","Mtt", "deltay"};
    TString xvars[] = {"mass_tlep","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt", "rapidity_tt"};
    Double_t xup[] = {400, 250, 400, 7, 500, 1500, 3};
    Double_t xdown[] = {100, 0, 0, 3, 0, 300, -3};
    Int_t bins[]={20, 20, 20, 4, 20, 20, 20};
    TString date_name[][3] = {{"","",""},{"","",""},{"","E","S"},{"","E","S"}};
    Double_t pre_scale[][3] = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1536.28, 474.95}, {1.0, 1536.28, 474.95}};
    float lumi=59.83;
    int edge[]={0,3,11,16,20,29};//23,31}; 
    int color[] = {97, 46, 9, 29, 209, kYellow, 93};
    for(int i=0;i<nsample;i++)
        fileNames[i]=fileNames[i].ReplaceAll(".root","*.root");
    TString lep[] = {"*1","*(!lep_flavour)","*(lep_flavour)"};
    TString lep_name[] = {"","_E","_M"};
    TString cate[] = {"*1","*(jet_num>=4)","*(jet_num==3)"};
    TString cate_name[] = {"","_4jets","_3jets"};
    TString dir_name[]={"A","B","C","D"};
    TString other_con1 = "((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    //TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    //TString other_con1 = "*1";
    TString other_con2 = "*1";
    TChain *mytree;
    TH1D *nmc, *hist, *h1[5];
    TH1D *hdata, *hdatad, *hdata0, *hmc;
    TH1D* hratio[3];
    Double_t nums, events;
    for(int i = 5; i < 7; i++){
        //if(i == 6)
        //    lep[0] = lep[0] + "*(mass_tt>500)";
        auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600); // temporary canvas
        auto c2 = new TCanvas("c2", "c2", 8, 30, 650, 650);
        TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.32,0.95,0.97);
    	TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.02,0.95,0.32);
        c2->cd();
    	format_pad(pad1, pad2);
        TLegend *leg = new TLegend(0.70, .65, 1.00, .90);
        format_leg(leg);
        format_canvas(c2);
        TChain* tree2 = new TChain("mytree");
        tree2->Add("./data/"+dir_name[dir_num]+"/new_data"+date_name[dir_num][l]+"*.root");
        hdata0 = new TH1D("data0", "", bins[i], xdown[i], xup[i]);
        tree2->Draw(xvars[i]+">>data0",other_con1+other_con2+lep[l]+cate[c]);
        hdata0->Scale(pre_scale[dir_num][l]);
        delete tree2;

        THStack* hstack = new THStack("hstack", "");
        for (int k = 4; k >= 0; k--){
            for(int j=edge[k];j<edge[k+1];j++){
                TChain* tree=new TChain("mytree");
                TChain* tree1=new TChain("rawtree"); 
                tree->Add("./MC/"+dir_name[dir_num]+"/"+fileNames[j]);
                tree1->Add("./MC/"+dir_name[dir_num]+"/"+fileNames[j]);
                nmc=new TH1D("nmc","",50,0,100);
                nmc->Sumw2();
                c1->cd();
                tree1->Draw("nJet>>nmc","Generator_weight");
                //cout<<nmc->GetSumOfWeights()<<endl;   
                float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
                //cout<<weight1<<endl;
                TString weight=Form("%f*Generator_weight*SF_btag_it*SF_lepton*"+other_con1+other_con2+lep[l]+cate[c],weight1);
                if(k==0)
                    weight = weight + "*nnlo_wt";
                c1->cd();
                hist = new TH1D("CG","", bins[i], xdown[i], xup[i]);
                hist->Sumw2();
                tree->Draw(xvars[i]+">>CG", weight);
                //cout<<fileNames[j]<<": "<<hist->GetSumOfWeights()<<endl;
                if(j==edge[k]){
                    h1[k]=(TH1D*)hist->Clone();
                    h1[k]->SetName(legend[k]);
                }
                else
                    h1[k]->Add(hist);                
                delete tree;
                delete tree1;
                delete nmc;
                delete hist;
            }
            //cout<<legend[k]<<": "<<h1[k]->GetSumOfWeights()<<endl;
            if(k==4)
                hmc = (TH1D*) h1[k]->Clone();
            else
                hmc->Add(h1[k]);
            h1[k]->SetFillColor(color[k]);
            hstack->Add(h1[k]);
        }
        hdatad=(TH1D*)hdata0->Clone();
        hdatad->SetName("hdatad");
        hdata=(TH1D*)hdata0->Clone();
        hdata->SetName("hdata");

        //set0(hmc);
        set0(hdata);
        double max = hdata->GetMaximum()*1.3;
        hdatad->Divide(hmc);
        set0(hdatad);
        /*TPaveText *lt = new TPaveText(0.45,0.75,0.55,0.85,"NDC");
        format_text(lt);
        lt->AddText(tex);*/
        pad1->cd();
        hstack->Draw("hist");
        //lt->Draw("same");

        format_stack(hstack, xtitle[i],max);
        for(int k=0;k<5;k++)
            leg->AddEntry(h1[k], legend[k], "f");
        leg->AddEntry(hdata,legendd,"p");
        pad1->cd();
        leg->Draw("same");
        hdata->Draw("PSame");
        hdata->SetMarkerSize(0.8);
        hdata->SetMarkerStyle(21);
    
        pad2->cd();
        gStyle->SetOptStat(0);
        format_th(hdatad,xtitle[i]);
        hdatad->Draw("P");
        
        pad2->cd();

        for(int r=0; r<3; r++){
            hratio[r]=new TH1D(Form("ratio%d",r),"",bins[i], xdown[i], xup[i]);
            set_ratio(hratio[r], 0.5*(r+1));
            hratio[r]->Draw("LSame");
            hratio[r]->SetLineStyle(7);
            hratio[r]->SetLineColor(1);
        }
       
        c2->Print("./qcd_pdf/"+dir_name[dir_num]+"/"+title[i]+lep_name[l]+cate_name[c]+".pdf");
        for(int k=0; k<5; k++)
            delete h1[k];
        for(int k=0; k<3; k++)
            delete hratio[k];
        delete hdata;
        delete hdatad;
        delete hdata0;
        delete hmc;
        delete hstack;
        delete leg;
        delete pad1;
        delete pad2;
        delete c1;
        delete c2;
    }
}
void draw_cg(){
    int dir[] = {1, 2, 3};
    TString tex1[] = {"", "e","#mu"};
    TString tex2[] = {"region B", "region C","region D"};
    for(int i=0; i<3; i++)
        draw(dir[i], 1, 2, "");
    /*for(int i=0; i<3; i++){
        for(int j=1; j<3; j++){
            draw(dir[i], 1, j, tex1[j]+", "+"4+jets, "+tex2[i]);
            draw(dir[i], 2, j, tex1[j]+", "+"3jets, "+tex2[i]);
        }
    }*/
} 