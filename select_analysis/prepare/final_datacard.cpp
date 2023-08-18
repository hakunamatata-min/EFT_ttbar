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
void clear(){
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
void sum_TH1D(TH1D* hs, TH1D** h1, int* start, int num){
    for(int f=0; f<num; f++){
        for(int i=0; i<h1[f]->GetNbinsX(); i++){
            hs->SetBinContent(i+1+start[f], h1[f]->GetBinContent(i+1));
            hs->SetBinError(i+1+start[f], h1[f]->GetBinError(i+1));
        }
    }
    hs->ResetStats();
}
void set_val(TH1D* h1, double val){
    for(int i=0; i<h1->GetNbinsX(); i++)
        h1->SetBinContent(i+1, val);
}
void copy(TH1D* h0, TH1D* h1){
    for(int i=0; i<h0->GetNbinsX(); i++){
        h0->SetBinContent(i+1, h1->GetBinContent(i+1));
        //h0->SetBinError(i+1, h1->GetBinError(i+1));
    }

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
void sum_TH1D(TH1D* hs_up, TH1D* hs_dn, TH1D** h0, TH1D** h1, TH1D** h2, int* start, int num, int option){
    TH1D *hd_up[4], *hd_dn[4], *h_nom[4];
    double norm_up = 0, norm_dn = 0, norm_nom = 0;
    for(int f=0; f<num; f++){
        hd_up[f] = new TH1D(Form("hd_up_%d", f), "", h0[f]->GetNbinsX(), 0, h0[f]->GetNbinsX());
        hd_dn[f] = new TH1D(Form("hd_dn_%d", f), "", h0[f]->GetNbinsX(), 0, h0[f]->GetNbinsX());
        h_nom[f] = new TH1D(Form("h_nom_%d", f), "", h0[f]->GetNbinsX(), 0, h0[f]->GetNbinsX());
        copy(hd_up[f], h1[f]);
        copy(hd_dn[f], h2[f]);
        copy(h_nom[f], h0[f]);
        //cout<<hd_up[f]->GetNbinsX()<<" "<<hd_dn[f]->GetNbinsX()<<" "<<h0[f]->GetNbinsX()<<endl;
        //cout<<hd_up[f]->GetNbinsY()<<" "<<hd_dn[f]->GetNbinsY()<<" "<<h0[f]->GetNbinsY()<<endl;
        hd_up[f]->Divide(h_nom[f]);
        hd_dn[f]->Divide(h_nom[f]);
        //cout<<h1[f]->GetSumOfWeights()<<endl;
        norm_up += h1[f]->GetSumOfWeights();
        norm_dn += h2[f]->GetSumOfWeights();
        norm_nom += h0[f]->GetSumOfWeights();
    }
    if(option == 1){
        for(int f=0; f<num; f++){
            hd_up[f]->Smooth();
            hd_dn[f]->Smooth();
        }
    }
    else if(option == 2){
        for(int f=0; f<num; f++){
            set_val(hd_up[f], norm_up/norm_nom);
            set_val(hd_dn[f], norm_dn/norm_nom);
        }
    }
    for(int f=0; f<num; f++){
        for(int i=0; i<h1[f]->GetNbinsX(); i++){
            hs_up->SetBinContent(i+1+start[f], h_nom[f]->GetBinContent(i+1)*hd_up[f]->GetBinContent(i+1));
            hs_dn->SetBinContent(i+1+start[f], h_nom[f]->GetBinContent(i+1)*hd_dn[f]->GetBinContent(i+1));
            hs_up->SetBinError(i+1+start[f], h1[f]->GetBinError(i+1));
            hs_dn->SetBinError(i+1+start[f], h2[f]->GetBinError(i+1));
        }
        delete hd_up[f];
        delete hd_dn[f];
        delete h_nom[f];
    }
    hs_up->ResetStats();
    hs_dn->ResetStats();
}  
void sum(TString cut_name, int t, int year, int like_cut, int* ycut, int nycut, int** mbin, int* nbins){
    clear();
    TString path = "../../combine/datacard";
    TString path2 = Form("../output/%d/datacard/", year);
    TString sys[] = {"jes","jer","unclus","SF_lepton","SF_btag", Form("SF_btag%d", year), "SF_ltag", Form("SF_ltag%d", year), "pdf", "alphas", "L1PF", "PU", "muR1","muF1","muR2","muF2","muR3","muF3","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un", "qcds"};
    TString sys_n;
    if(cut_name.Contains("E"))
        sys[4] = "SF_Elec";
    else
        sys[4] = "SF_Muon";
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    TString type_nus[] = {"no/", "smooth/", "flat/"};

    int option[][30] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};
    int num_sig = 5;
    auto process = std::vector<TString>{"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "ttbar_ci0200", "DYJets","STop", "WJets", "QCD", "data_obs"};;
    int num_pro = process.size();
    int beg[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, num_sig, num_sig, num_sig+1, num_sig+1, num_sig+2, num_sig+2, 0, 0, 0, 0, 0, 0, 0, num_pro-2};
    int end[] = {num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_pro-2, num_sig+1, num_sig+1, num_sig+2, num_sig+2, num_sig+3, num_sig+3, num_pro-1, num_pro-1, num_sig, num_sig, num_sig, num_sig, num_sig, num_pro-1};

    double yield;
    ofstream card;
    TString category="ttbar"+cut_name+Form("_%d", year);
    card.open (path+"/"+type_nus[t]+category+".txt");
    cout<<path+"/"+type_nus[t]+category+".txt"<<endl;
    TFile* sum_file = new TFile(path+"/"+type_nus[t]+category+".root", "recreate");
    
    const int num = nycut;
    TH3D *hmc31[num], *hmc32[num], *hmc33[num];
    TH1D *hmc1[num], *hmc2[num], *hmc3[num];
    TFile *file;
    TH1D *h1, *h2, *h3;
    int bin_num = 0;
    int start[num];
    bool no_sys;
    TH1D* hd[num];
    for(int i=0; i<num; i++){
        start[i] = bin_num;
        bin_num += nbins[i];      
    }
    file = TFile::Open(path2+"ttbar"+cut_name+".root");
    
    for(int c=0; c<num_pro; c++){
        h1 = new TH1D(process[c], "", bin_num, 0, bin_num);
        for(int f=0; f<num; f++){
            hmc31[f] = (TH3D*)file->Get(process[c]+"_sub");
            hmc31[f]->SetName(Form("hmc31_%d", f));
            hmc1[f] = new TH1D;
            get_TH1D(hmc1[f], Form("hmc1_%d", f), hmc31[f], like_cut, ycut[f]+1, ycut[f+1], mbin[f], nbins[f]);
        }
        sum_TH1D(h1, hmc1, start, num);
        yield = h1->GetSumOfWeights();
        //cout<<yield<<endl;
        if(!(process[c].Contains("QCD")||process[c].Contains("data")))
            push(c, yield, process[c], category, num_sig, year);
        sum_file->cd();
        h1->Write();
        delete h1;
        for(int s=0; s<0; s++){
            if(s==0){

            }
            //if(sys[s].Contains("pdf"))//no pdf now
            //    continue;
            if(c==0){
                push_sys(sys[s], beg[s], end[s], num_pro);
            }
            if(c<beg[s] || c>=end[s])
                continue;
            no_sys = false;
            h2 = new TH1D(process[c]+"_"+sys[s]+"Up", "", bin_num, 0 ,bin_num);
            h3 = new TH1D(process[c]+"_"+sys[s]+"Down", "", bin_num, 0 ,bin_num);
            for(int f=0; f<num; f++){
                cout<<process[c]+"_"+sys[s]+"Up_sub"<<endl;
                hmc32[f] = (TH3D*)file->Get(process[c]+"_"+sys[s]+"Up_sub");
                hmc33[f] = (TH3D*)file->Get(process[c]+"_"+sys[s]+"Down_sub");
                /*if(hmc32[f] == NULL){
                    delete hmc32[f]; delete hmc33[f];
                    no_sys = true;
                    break;
                }*/
                hmc32[f]->SetName(Form("hmc2_%d", f));
                hmc33[f]->SetName(Form("hmc3_%d", f));
                hmc2[f] = new TH1D;
                hmc3[f] = new TH1D;
                get_TH1D(hmc2[f], Form("hmc2_%d", f), hmc32[f], like_cut, ycut[f], ycut[f+1], mbin[f], nbins[f]);
                get_TH1D(hmc3[f], Form("hmc3_%d", f), hmc33[f], like_cut, ycut[f], ycut[f+1], mbin[f], nbins[f]);
            }
            /*if(no_sys == true){
                delete h2; delete h3;
                continue;
            }*/
            sum_TH1D(h2, h3, hmc1, hmc2, hmc3, start, num, option[t][s]);
            sum_file->cd();
            h2->Write();
            h3->Write();
            for(int f=0; f<num; f++){
                delete hmc32[f]; delete hmc33[f];
                delete hmc2[f]; delete hmc3[f];
            }
            delete h2; delete h3;
        }
        for(int f=0; f<num; f++){
            delete hmc1[f]; delete hmc31[f];
        }
    }
    write_card(card, category, num_pro);
    card.close();
    file->Close();
    sum_file->Close();
}
void final_datacard(){
    TString cut_name[4]={"_E_3jets", "_E_4jets", "_M_3jets", "_M_4jets"};
    int year[] = {2015, 2016, 2017, 2018};
    
    double likelihood_cut = 1000;
    int like_cut;
    if(likelihood_cut <= 50.0 && likelihood_cut>=13.0)
        like_cut = int(likelihood_cut-13.0);
    else
        like_cut = -1;
    
    const int nycut = 4;
    int ycut[nycut+1];
    double ycut_user[nycut] = {0.0, 0.4, 1.0, 2.0};
    for(int i=1; i<nycut; i++)
        ycut[i] = int((ycut_user[i]-0.0)/0.1);
    ycut[nycut] = 41;
    ycut[0] = -1;

    int *xbins[20];
    int nbins[] = {9, 11, 10, 11};
    double xbins_user[nycut][20] = {{0,300,340,380,420,460,500,600,800,3000}, {0,300,350,400,450,500,550,600,700,800,1000,3000}, 
                               {0,400,450,500,550,600,650,700,800,1000,3000}, {0,450,550,650,700,750,800,900,1000,1200,1400,3000}};
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
    for(int i=3; i<4; i++){
        for(int y=2; y<3; y++){
            for(int t=0; t<1; t++){
                sum(cut_name[i], t, year[y], like_cut, ycut, nycut, xbins, nbins);
            }
        }
    }
    for(int i=0; i<nycut; i++)
        delete[] xbins[i];
}