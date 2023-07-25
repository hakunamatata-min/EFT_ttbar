//build 2D RooHistPdf and datacard file
//for ttbar and its background
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
using namespace RooFit;

class prepare{
private:
    TFile* file;
    int year;
    TString dir, outputDir, qcd_dir, pdf_dir;
    //Double_t mtt_edges[9];
    //Double_t ytt_edges[10];
    double xbins[20];
    int nbins;
    //int bin_num;
    TString fileNames[20];
    Float_t cross_sections[20];
    Float_t K_Factors[20];
    TString cut, cut_name;
    int edge_i[8], edge_f[8];
    TString sys_n[30], sys[30], sys_up[30], sys_down[30];
    TString process[9];
    TString EW[5];
    TString weight_nom, weight_up, weight_dn;
    TString file_up, file_dn;
    TString tree_up, tree_dn;
    
    void Floor(TH1D* hist);
    void give_sys_name(TString file, TString weight, int s, int c);
    void renew_weight(TString* weight, TString file, int f);
    void draw(TH1D* h1, TString file, TString tree, TString weight);
    void set_dir();
    void draw(int c);
    void draw(int c, int s);
    void draw_data();
public:
    prepare(TString cut_s, TString cut_name_s, int year_s, double *xbin, int nbin);
    ~prepare();
};