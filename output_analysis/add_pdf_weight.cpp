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
using namespace std;
void Convert(TH2D** histo, TH1D* rehist_max,TH1D* rehist_min, int num){
	Double_t max, min, error_max, error_min;
	int k=1;
	for (int i=0;i<histo[0]->GetNbinsX();i++){
		for (int j=0;j<histo[0]->GetNbinsY();j++){
			Int_t nbin=histo[0]->GetBin(i+1,j+1);
			max=histo[0]->GetBinContent(nbin);
			min=histo[0]->GetBinContent(nbin);
			error_max=histo[0]->GetBinError(nbin);
			error_min=histo[0]->GetBinError(nbin);
			for(int m=1;m<num;m++){
				if(max<histo[m]->GetBinContent(nbin)){
					max=histo[m]->GetBinContent(nbin);
					error_max=histo[m]->GetBinError(nbin);
				}
				if(min>histo[m]->GetBinContent(nbin)){
					min=histo[m]->GetBinContent(nbin);
					error_min=histo[m]->GetBinError(nbin);
				}
			}
			rehist_max->SetBinError(k,error_max);
			rehist_max->SetBinContent(k,max);
			rehist_min->SetBinError(k,error_min);
			rehist_min->SetBinContent(k++,min);
		}
	}
}
void check(TH1D* h1){
	for(int i=0;i<h1->GetNbinsX();i++){
		cout<<h1->GetBinContent(i)<<endl;
	}
}
void check(TH2D* h1){
	for(int i=0;i<h1->GetNbinsX();i++){
		for(int j=0;j<h1->GetNbinsX();j++){
			Int_t nbin=h1->GetBin(i+1,j+1);
			cout<<h1->GetBinContent(nbin)<<endl;
		}
	}
}
void renew_weight(TString* weight, TString file, int j, TString path){
    const int nsample = 20;
    Float_t cross_sections[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3,};
    Float_t K_Factor[nsample]={1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21};
    float lumi=59.83;
    auto c1 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(path+"MC/"+file);
    TH1D *nmc=new TH1D("nmc","",50,0,100);
    nmc->Sumw2();
    c1->cd();
    tree1->Draw("nJet>>nmc","Generator_weight");
    //cout<<nmc->GetSumOfWeights()<<endl;
    float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
    //cout<<weight1<<endl;
    *weight = Form("%f*", weight1) + (*weight);
    delete tree1;
    delete nmc;
    delete c1;
}
void add_pre(TString cut_off, TString filename, int year){
	const int nsample = 20;
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
                            "new_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM.root"};
	TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
	
	
	TString process[]={"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "DYJets","STop", "WJets", "QCD"};
	//TString dir="/eos/user/y/yuekai/output3/";
	int edge_i[] = {0, 0, 0, 0, 3, 11, 16};//23,31}; 
    int edge_f[] = {3, 3, 3, 3, 11, 16, 20,}; 
	float lumi = 59.83;
	const int mbin=4, ybin=5;
    Double_t mtt_edges[5]={0, 400, 500, 700, 2000};
	Double_t ytt_edges[6]={-3.0, -1.2, -0.3, 0.3, 1.2, 3.0};
	TString EW_weight[4]={"weight_ci0000","weight_ci0100","weight_ci0010","weight_ci0001"};
	UInt_t nweight;
	TH1D *h1_max[10], *h1_min[10]; 
	TH1D *h2_max, *h2_min;
	TH2D *hist[150];
	TString path = Form("./%d/",year);
	TFile *file = new TFile(path+"pdf_w/"+filename,"recreate");
	//fileNames[0]=fileNames[0].ReplaceAll(".root","_1*.root");
	for(int i=0;i<nsample;i++)
		fileNames[i]=fileNames[i].ReplaceAll(".root","_1.root");
	for(int i=0; i<7; i++){
		cout<<"this is "<<process[i]<<endl;
		for(int j=edge_i[i]; j<edge_f[i]; j++){
			cout<<"\t\t this is "<<fileNames[j]<<endl;
			TChain *tree = new TChain("mytree");
			tree->Add(path+"MC/"+fileNames[j]);
			tree->SetBranchAddress("nLHEPdfWeight",&nweight);
			tree->GetEntry(1);
			TString weight = "Generator_weight*SF_btag*SF_lepton*L1PreFiringWeight_Nom"+other_con1+other_con2;
			renew_weight(&weight, fileNames[j], j, path);
            if(i < 4)
                weight = weight+"*nnlo_wt*"+EW_weight[i];
			for(int num=0;num<nweight;num++){
				hist[num]=new TH2D(Form("hist%d",num), "", mbin, mtt_edges, ybin, ytt_edges);
				hist[num]->Sumw2();
				tree->Draw(Form("rapidity_tt:mass_tt>>hist%d",num),weight+Form("*LHEPdfWeight[%d]*",num)+cut_off);
			}
			h2_max=new TH1D("h2_max", "", mbin*ybin, 0, mbin*ybin);
			h2_min=new TH1D("h2_min", "", mbin*ybin, 0, mbin*ybin);
			h2_max->Sumw2();
			h2_min->Sumw2();
			Convert(hist, h2_max, h2_min, nweight);
			if(j == edge_i[i]){
				h1_max[i]=(TH1D*)h2_max->Clone();
				h1_max[i]->SetName(process[i]+"_pdfUp");
				h1_min[i]=(TH1D*)h2_min->Clone();
				h1_min[i]->SetName(process[i]+"_pdfDown");
			}
			else{
				h1_max[i]->Add(h2_max);
				h1_min[i]->Add(h2_min);
			}
			delete tree;
			delete h2_min;
			delete h2_max;
			for(int num=0; num<nweight; num++)
				delete hist[num];
		}
	}
	file->cd();
	for(int i=0; i<7; i++){
		h1_max[i]->Write();
		h1_min[i]->Write();
		delete h1_max[i];
		delete h1_min[i];
	}
	file->Close();
}
void add_pdf_weight(int i){
	TString cuts[] = {"(jet_num == 3 && likelihood<20.0 && (!lep_flavour))","(jet_num >= 4 && likelihood<20.0 && (!lep_flavour))",
		"(jet_num == 3 && likelihood<20.0 && lep_flavour)",  "(jet_num >= 4 && likelihood<20.0 && lep_flavour)"};
	TString filename[4]={"pdfw_E_3jets.root", "pdfw_E_4jets.root", "pdfw_M_3jets.root", "pdfw_M_4jets.root"};
	int year = 2018;
	cout<<"start to calculate "<<i<<":"<<endl;
	add_pre(cuts[i], filename[i], year);
}