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
void add_pdf_weight(TString pdf_dir, TString outputDir, TString cut){
    TString process[]={"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "ttabr_ci0200", "DYJets","STop", "WJets", "QCD"};
    TFile* file = new TFile(outputDir+"/ttbar_"+cut+".root", "update");
    TH1D *hist_up, *hist_dn;
    for(int c=0; c<8; c++){
        TFile* pdf_file = TFile::Open(pdf_dir+"/pdfw_"+cut+Form("_%d.root",c));
        cout<<"pdfw_"+cut+Form("_%d.root",c)<<endl;
        hist_up = (TH1D*)pdf_file->Get(process[c]+"_pdfUp_sub");
        hist_dn = (TH1D*)pdf_file->Get(process[c]+"_pdfDown_sub");
        if(c==4){
            hist_up->SetName("ttbar_ci0200_pdfUp_sub");
            hist_dn->SetName("ttbar_ci0200_pdfDown_sub");
        }
        //hist_up->Scale(137.1/59.83);
        //hist_dn->Scale(137.1/59.83);
        file->cd();
        hist_up->Write();
        hist_dn->Write();
        delete hist_up;
        delete hist_dn;
    }
    file->Close();
}
void add_pdf(){
    int year[] = {2015, 2016, 2017, 2018};   
    TString div_name[4] = {"_y0", "_y1", "_y2", "_y3"};
	TString cut_name[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    for(int y=1; y<4; y++){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                TString pdf_dir = Form("/home/yksong/code/ttbar/output/%d/pdf_w", year[y]);
                TString outputDir = Form("/home/yksong/code/ttbar/output/%d/datacard", year[y]);
                add_pdf_weight(pdf_dir, outputDir, cut_name[i]+div_name[j]);
            }
        }
    }
}