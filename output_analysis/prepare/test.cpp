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
int main(){
    std::vector<int> a = {1, 2, 3};
    a = {4, 5};
    //a.erase(a.begin()+2);
    for(int i=0; i<a.size(); i++){
        cout<<a[i]<<endl;
    }
    cout<<a.size()<<endl;
}