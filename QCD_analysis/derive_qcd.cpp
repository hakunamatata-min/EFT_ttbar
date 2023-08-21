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
#include<TH2D.h>
#include<TH3D.h>
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
void sum_TH1D(TH1D* hs, TH1D** h1, int* start, int num){
    for(int f=0; f<num; f++){
        for(int i=0; i<h1[f]->GetNbinsX(); i++){
            hs->SetBinContent(i+1+start[f], h1[f]->GetBinContent(i+1));
            hs->SetBinError(i+1+start[f], h1[f]->GetBinError(i+1));
        }
    }
    hs->ResetStats();
}
void get_TH1D(TH1D* h1, TString h1_name, TH3D* h3, int like_cut, int ycut_low, int ycut_up, int* xbins, int nbins){
    h1->SetBins(nbins, 0, nbins);
    h1->SetName(h1_name);
    TH1D* h3_1 = h3->ProjectionX("3_px", ycut_low, ycut_up, 0, like_cut);

    double value;
    double err2;
    for(int i=0; i<nbins; i++){
        value = 0;
        err2 = 0;
        for(int bin=xbins[i]; bin<xbins[i+1]; bin++){
            value += h3_1->GetBinContent(bin+1);
            err2 += h3_1->GetBinError(bin+1)*h3_1->GetBinError(bin+1);
        }
        h1->SetBinContent(i+1, value);
        h1->SetBinError(i+1, sqrt(err2));
    }
    //cout<<h1->GetSumOfWeights()<<endl;
    delete h3_1;
}
void derive_qcd(TString file){
    //choose to divide
    double likelihood_cut = 1000;
    const int nycut = 4;
    int nbins[] = {9, 11, 10, 11};
    double xbins_user[nycut][20] = {{0,300,340,380,420,460,500,600,800,3000}, {0,300,350,400,450,500,550,600,700,800,1000,3000}, 
                               {0,400,450,500,550,600,650,700,800,1000,3000}, {0,450,550,650,700,750,800,900,1000,1200,1400,3000}};
    //convert
    int like_cut;
    if(likelihood_cut <= 50.0 && likelihood_cut>=13.0)
        like_cut = int(likelihood_cut-13.0);
    else
        like_cut = -1;

    int ycut[nycut+1];
    double ycut_user[nycut] = {0.0, 0.4, 1.0, 2.0};
    for(int i=1; i<nycut; i++)
        ycut[i] = int((ycut_user[i]-0.0)/0.1);
    ycut[nycut] = 41;
    ycut[0] = -1;

    int *xbins[20];
    for(int i=0; i<nycut; i++){
        xbins[i] = new int[nbins[i]+1];
        for(int j=0; j<nbins[i]+1; j++){
            if(xbins_user[i][j]<300)
                xbins[i][j] = -1;
            else if(xbins_user[i][j]>3000)
                xbins[i][j] = 271;
            else
                xbins[i][j] = int((xbins_user[i][j]-300)/10);
        }
    }
  
    TFile *f = new TFile(file);
    TList *list = f->GetListOfKeys();
    TFile *outFile = new TFile(file.ReplaceAll("_3D", ""), "recreate");
    TKey *key;
    TIter iter(list); //or TIter iter(list->MakeIterator());
    static TString classname("TH3D");
    int start[nycut];
    int bin_num = 0;
    for(int i=0; i<nycut; i++){
        start[i] = bin_num;
        bin_num += nbins[i];      
    }
    while((key = (TKey*)iter())) {
        if (key->GetClassName() == classname) {
        TH3D* hist3 = (TH3D*)key->ReadObj();
        if(hist3) {
            TH1D* hists = new TH1D;
            TH1D* hist1 = new TH1D[4];
            for(int f=0; f<nycut; f++){
                get_TH1D(&hist1[f], hist3->GetName(), hist3, like_cut, ycut[f]+1, ycut[f+1], xbins[f], nbins[f]);
                sum_TH1D();
            }
            outFile->WriteTObject(hist1);
            delete hist3;
            delete[] hist1;
        }
        }
    }
    outFile->cd();      
    outFile->Write();
    outFile->Close();
}