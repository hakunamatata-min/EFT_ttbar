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
using namespace RooFit;
std::vector<TString> process_names; //names of sigal and bkg
std::vector<int> process_id;  //process ID; minus and zero for sigal; positive for bkg
std::vector<TString> bin_arr;   //category name
std::vector<float> yield_array;  //rate(event yeild)
std::vector<TString> cms_lumi;
std::vector<TString> sysNames;
std::vector<std::vector<TString>> sysnum;
void Clear(){
    process_names.clear();
    process_id.clear();  
    bin_arr.clear();   //category name
    yield_array.clear();  //rate(event yeild)
    cms_lumi.clear();
    sysNames.clear();
    sysnum.clear();
}
void writeline(vector<TString> arr, ofstream& card){
    for (int i=0;i<arr.size();i++)
        card<< arr[i]<<"\t";
    card<<endl;
}

void writeline(vector<int> arr, ofstream& card){
    for (int i=0;i<arr.size();i++)
        card<< arr[i]<<"\t";
    card<<endl;
}
void writeline(vector<float> arr, ofstream& card){
    //	gStyle->SetPaintTextFormat("2.2f");
    card<< std::fixed;
    card<< std::setprecision(3);
    for (int i=0;i<arr.size();i++)
        card<< arr[i]<<"\t";
    card<<endl;
}
void push(int c, double yield, TString name, TString category, int num_sig, int year){
    yield_array.push_back(yield);
    process_names.push_back(name);
    process_id.push_back(c-num_sig+1);
    bin_arr.push_back(category);
    TString uns[] = {"1.012", "1.012", "1.023", "1.025"};
    cms_lumi.push_back(uns[year-2015]);
}
void push_sys(TString sys_n, int begin, int end, int num_pro){
    std::vector<TString> sys_num;
    for(int c=0; c<num_pro; c++){
        if(c<begin || c>=end)
            sys_num.push_back("-");
        else
            sys_num.push_back("1");
    }
    sysnum.push_back(sys_num);
    sysNames.push_back(sys_n);
}
void write_card(ofstream& card, TString category, int num_pro){
    card <<"Datacard for event category: "<< category << endl;
    card<< "imax 1 number of channels"<<endl;
    card<< Form("jmax %d number of processes minus 1", num_pro-1)<<endl;
    card<< "kmax * number of nuisance parameters"<<endl;
    card<<"---------------------------------"<<endl;
    card<<endl;
    card<< "shapes * "<< category << " "<< category+".root $PROCESS $PROCESS_$SYSTEMATIC" <<endl;
    card<<"---------------------------------"<<endl;
    card<< "bin           "<< category <<endl;
    card<< "observation   "<< "-1"<<endl;
    card<<"---------------------------------"<<endl;
    card<<endl;
    card<<"bin \t";
    writeline(bin_arr, card);
    card<<"process \t";
    writeline(process_names, card);
    card<<"process \t";
    writeline(process_id, card);
    card<<"rate \t";
    writeline(yield_array, card);
    /*card<<"sig_norm"<<"\t lnN \t";
    writeline(sig_norm);
    card<<"DYJets_norm"<<"\t lnN \t";
    writeline(DYJets_norm);
    card<<"STop_norm"<<"\t lnN \t";
    writeline(STop_norm);
    card<<"WJets_norm"<<"\t lnN \t";
    writeline(WJets_norm);*/
    card<<"cms_lumi"<<"\t lnN \t";
    writeline(cms_lumi, card);
    //card<<"qcdn"<<"\t lnN \t";
    //writeline(qcd_n, card);
    for(int s=0; s<sysNames.size(); s++){
        card<<sysNames[s]<<"\t shape \t";
        writeline(sysnum[s], card);
    }
}

void write(TString cut_name, int t, int year){
    Clear();
    TString type_nus[] = {"no/", "smooth/", "flat/"};
    TString path = "./datacard/"+type_nus[t];
    TString input = path+"ttbar"+cut_name+Form("_%d.root", year);
    TString sys[] = {"jes","jer","unclus","SF_lepton","SF_btag", Form("SF_btag%d", year), "SF_ltag", Form("SF_ltag%d", year), "pdf", "alphas", "L1PF", "PU", "muR1","muF1","muR2","muF2","muR3","muF3","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un", "qcds"};
    TString sys_n;
    if(cut_name.Contains("E"))
        sys[4] = "SF_Elec";
    else
        sys[4] = "SF_Muon";
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    

    int num_sig = 5;
    auto process = std::vector<TString>{"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "ttbar_ci0200", "DYJets","STop", "WJets", "QCD"};;
    int num_pro = process.size() - 1;
    int beg[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, num_sig, num_sig, num_sig+1, num_sig+1, num_sig+2, num_sig+2, 0, 0, 0, 0, 0, 0, 0, num_pro-1};
    int end[] = {num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_pro, num_sig+1, num_sig+1, num_sig+2, num_sig+2, num_sig+3, num_sig+3, num_pro, num_pro, num_sig, num_sig, num_sig, num_sig, num_sig, num_pro};

    double yield;
    ofstream card;
    TString category="ttbar"+cut_name+Form("_%d", year);
    card.open (path+category+".txt");
    cout<<path+category+".txt"<<endl;
    
    TFile *file;
    TH1D *h1;
    file = TFile::Open(input);
    cout<<input<<endl;
    for(int c=0; c<num_pro; c++){//no QCD now
        h1 = (TH1D*)file->Get(process[c]);
        yield = h1->GetSumOfWeights();
        //cout<<yield<<endl;
        if(!process[c].Contains("data"))
            push(c, yield, process[c], category, num_sig, year);
        delete h1;
    }
    for(int s=0; s<25; s++){
        if(s == 0){
            for(int sour=0; sour<10; sour++)
                push_sys(sys[s]+"_"+jes_source[sour], beg[s], end[s], num_pro);
        }
        push_sys(sys[s], beg[s], end[s], num_pro);
    }
    write_card(card, category, num_pro);
    card.close();
    file->Close();
}

void write_datacard(){
    TString cut_name[4]={"_E_3jets", "_E_4jets", "_M_3jets", "_M_4jets"};
    int year[] = {2015, 2016, 2017, 2018};
    for(int i=0; i<4; i++){
        for(int y=0; y<4; y++){
            for(int t=0; t<3; t++){
                write(cut_name[i], t, year[y]);
            }
        }
    }
}