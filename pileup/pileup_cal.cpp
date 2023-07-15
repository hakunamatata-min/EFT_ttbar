#include<TROOT.h>
#include<TFile.h>
#include<TChain.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1D.h>
#include<TCanvas.h>
#include<string.h>
void pileup_cal(){//2, 0
    const int nsample = 29;
    TString fileNames[nsample] = {"/store/mc/RunIISummer20UL17NanoAODv9/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/280000/B26DF967-A99A-994F-B191-68B71EC81F22.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/2510000/BECA664B-8911-5746-B733-DC1E0C020E9D.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/80000/C4FE9594-9E21-DE42-991F-01C32BA06CC9.root",

    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/280000/5E333AC4-26CE-0F49-AA19-04B086A29C29.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/270000/A08CF40E-307C-B34D-B8AF-59772AD264AD.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/120000/33403F08-6176-DE46-805E-56651ACC2283.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/280000/70EA586B-7581-C144-A199-4B162CA114C5.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/280000/0C786043-E54E-8A46-955F-0B696E2801BC.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/120000/93159D45-0DCB-994E-AE43-9D448E52E9EB.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/260000/76A66F8D-C968-8A46-8384-69BD22E7C7B9.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/260000/EACF3C5B-B242-FE40-A4AE-6F61D812A6B9.root",

    "/store/mc/RunIISummer20UL17NanoAODv9/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/280000/6F22770C-6A6E-314B-B8A2-44D41F8EFD5A.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/80000/E11DEF70-F1A0-BE4F-B427-7F7578E0C2B5.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/80000/14C689A2-1832-A246-9976-2E84A672C88C.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v2/260000/C76E099F-2A32-C342-93C3-271D35B0B4B9.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v2/40000/FBBA7E43-712F-3447-9BCA-E4D88064CB30.root",

    "/store/mc/RunIISummer20UL17NanoAODv9/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/70000/69CA1FEA-C813-6A4C-8657-65D4B9B73414.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/70000/9E1CC478-12C6-D945-8593-6E97ACA67AE6.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/130000/ACB2117E-CC6B-EF48-AC1A-607956A384E6.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v2/260000/C22CDFD4-E1D3-6A48-B1E2-D9C1B1847348.root",

    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT50to100_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/260000/AF36722F-4991-D046-B5BC-C9DBAEEEC9E3.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT100to200_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/30000/DED4A2C0-0064-9D48-9EAF-A7C4E5725AA6.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT200to300_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/2520000/BB912BED-5EFE-2640-A555-2F1C55EA37B7.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT300to500_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/260000/2183C909-2445-204B-A77B-72247DCB0866.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT500to700_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/270000/4350E11F-6F1F-C341-9218-12E08AA70941.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT700to1000_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/2520000/7142B942-CB85-254C-90AC-4FC5F1ECEEBA.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT1000to1500_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/2540000/5ED13A51-99D7-3847-8E1D-1EA074009F1A.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT1500to2000_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/260000/F5249DE6-BBD4-B04E-845D-46CB9F0EE738.root",
    "/store/mc/RunIISummer20UL17NanoAODv9/QCD_HT2000toInf_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/106X_mc2017_realistic_v9-v1/30000/25BFB7C2-8771-3447-9520-4C0B1765EC62.root",
    };
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
                                1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    TString legend[] = {"tt","DY","single top","V+jets","QCD"};
    TString xtitle = "nPU";
    TString title = "nPU";
    TString xvars= "Pileup_nPU";
    int edge[]={0,3,11,16,20,29};//23,31}; 
    int color[] = {2, 46, 9, 29, 8, kYellow, 93};
    TFile* outputFile = new TFile("reweight.root", "RECREATE");
    TChain *Events;
    TH1D *h1[5],*nmc, *hist, *MCPUHist;
    Double_t nums, events;
    float lumi=41.48;
    auto c1 = new TCanvas("c1", "c1", 8, 30, 600, 600); // temporary canvas
    for (int k = 4; k >= 0; k--){
        for(int j=edge[k];j<edge[k+1];j++){
            TChain* tree=new TChain("Events");
            tree->Add("root://cms-xrd-global.cern.ch/"+fileNames[j]);
            nmc=new TH1D("nmc","",99,0,99);
            nmc->Sumw2();
            c1->cd();
            tree->Draw("nJet>>nmc","Generator_weight");
            //cout<<nmc->GetSumOfWeights()<<endl;   
            float weight1=cross_sections[j]*lumi/(nmc->GetSumOfWeights())*K_Factor[j]*1000;
            //cout<<weight1<<endl;
            TString weight=Form("%f*Generator_weight",weight1);
            c1->cd();
            hist = new TH1D("nPU","", 99 , 0 , 99);
            hist->Sumw2();
            tree->Draw(xvars+">>nPU", weight);
            if(j==edge[k]){
                h1[k]=(TH1D*)hist->Clone();
            }
            else
                h1[k]->Add(hist);                
            delete tree;
            delete nmc;
            delete hist;
        }
        cout<<legend[k]<<": "<<h1[k]->GetSumOfWeights()<<endl;
        if (k==4)
            MCPUHist = (TH1D*)h1[k]->Clone();
        else
            MCPUHist->Add(h1[k]);
        cout<<k<<endl;
    }
    MCPUHist->Scale(1.0 / MCPUHist->Integral()); 
    cout<<MCPUHist->Integral()<<endl;
    TFile* target_down=TFile::Open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2017-66000ub-99bins.root");
    TFile* target=TFile::Open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2017-69200ub-99bins.root");
    TFile* target_up=TFile::Open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2017-72400ub-99bins.root");
    TH1D* hi = (TH1D*) target->Get("pileup");
    TH1D* targetPUHist = (TH1D*) hi->Clone(TString::Format("%d",rand()));  
    TH1D* hi_down = (TH1D*) target_down->Get("pileup");
    TH1D* targetPUHist_down = (TH1D*) hi_down->Clone(TString::Format("%d",rand()));  
    TH1D* hi_up = (TH1D*) target_up->Get("pileup");
    TH1D* targetPUHist_up = (TH1D*) hi_up->Clone(TString::Format("%d",rand())); 
    cout<<targetPUHist->Integral()<<endl;
    cout<<targetPUHist_up->Integral()<<endl;
    cout<<targetPUHist_down->Integral()<<endl; 
    for(Int_t i=0; i<99; i++){
        if((targetPUHist->GetBinContent(i))!= (targetPUHist_up->GetBinContent(i))){
            cout<<"Histgrams(up) differ before normalization"<<endl;
            break;
        }
        if(i==98){
            cout<<"Histgrams(up) are the same before normalization"<<endl;
        }
    }
    for(Int_t i=0; i<99; i++){
        if((targetPUHist->GetBinContent(i))!= (targetPUHist_down->GetBinContent(i))){
            cout<<"Histgrams(down) differ before normalization"<<endl;
            break;
        }
        if(i==98){
            cout<<"Histgrams(down) are the same before normalization"<<endl;
        }
    }
    targetPUHist_up->Scale(1.0 / targetPUHist_up->Integral());
    targetPUHist_down->Scale(1.0 / targetPUHist_down->Integral());
    targetPUHist->Scale(1.0 / targetPUHist->Integral());
    for(Int_t i=0; i<99; i++){
        if((targetPUHist->GetBinContent(i))!= (targetPUHist_up->GetBinContent(i))){
            cout<<"Histgrams(up) differ after normalization"<<endl;
            break;
        }
        if(i==98){
            cout<<"Histgrams(up) are the same after normalization"<<endl;
        }
    }
    for(Int_t i=0; i<99; i++){
        if((targetPUHist->GetBinContent(i))!= (targetPUHist_down->GetBinContent(i))){
            cout<<"Histgrams(down) differ after normalization"<<endl;
            break;
        }
        if(i==98){
            cout<<"Histgrams(down) are the same after normalization"<<endl;
        }
    }
    TH1D* reweightHist = new TH1D("reweightHist", "Pileup reweighting factors", 99, 0, 99);
    TH1D* reweightHist_up = new TH1D("reweightHist_up", "Pileup reweighting factors", 99, 0, 99);
    TH1D* reweightHist_down = new TH1D("reweightHist_down", "Pileup reweighting factors", 99, 0, 99);
    reweightHist->Divide(targetPUHist,MCPUHist);
    reweightHist_up->Divide(targetPUHist_up,MCPUHist);
    reweightHist_down->Divide(targetPUHist_down,MCPUHist);
    /*TFile* input=TFile::Open("root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL16NanoAODAPVv9/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_mcRun2_asymptotic_preVFP_v11-v1/70000/938B566B-9040-4C46-90DE-5F1A5185204C.root");
    TTree* inputTree=(TTree*) input->Get("Events");
    TH1D* inputPUHist = new TH1D("inputPUHist", "Input pileup distribution", 99, 0, 99);
    inputTree->Draw("Pileup_nPU>>inputPUHist");
    TH1D* outputPUHist = new TH1D("outputPUHist", "Reweighted pileup distribution", 99, 0, 99);
    Int_t numEntries = inputTree->GetEntries();
    for (Int_t i = 0; i < numEntries; i++) {
        Int_t nPU;
        inputTree->SetBranchAddress("Pileup_nPU", &nPU);
        inputTree->GetEntry(i);
        
        //Int_t nTrueInt = inputTree->Pileup_nTrueInt;
        Float_t weight = reweightHist->GetBinContent(reweightHist->GetXaxis()->FindBin(nPU));
        outputPUHist->Fill(nPU, weight);
        //afPUnintHist->Fill(nTrueInt, weight);
    }*/
    outputFile->cd();
    MCPUHist->Write();
    //inputPUHist->Write();
    //outputPUHist->Write();
    reweightHist->Write();
    reweightHist_up->Write();
    reweightHist_down->Write();
    targetPUHist->Write();
    outputFile->Close();
    //input->Close();
    target->Close();
    for(int k=0;k<5;k++)
       delete h1[k];
    delete c1;
}