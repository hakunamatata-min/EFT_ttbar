#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
void pileup_add_branch(TString inputFile){
    TFile* reweight = TFile::Open("/afs/cern.ch/user/r/repan/work/yuekai/condor/select/2017/cal/reweight.root");
    TH1D* hist = (TH1D*) reweight->Get("reweightHist");
    TH1D* reweightHist = (TH1D*) hist->Clone();
    TH1D* hist_up = (TH1D*) reweight->Get("reweightHist_up");
    TH1D* reweightHist_up = (TH1D*) hist_up->Clone();
    TH1D* hist_down = (TH1D*) reweight->Get("reweightHist_down");
    TH1D* reweightHist_down = (TH1D*) hist_down->Clone();
    TFile* input=new TFile(inputFile,"update");
    TTree* inputTree=(TTree*) input->Get("Events");
    Float_t  weight, weight_up, weight_down;
    Int_t nPU;
    inputTree->SetBranchAddress("Pileup_nPU",&nPU);
    TBranch *branch, *branch_up, *branch_down;
    branch=inputTree->Branch("pileupweight",&weight,"pileupweight/F");
    branch_up=inputTree->Branch("pileupweight_up",&weight_up,"pileupweight_up/F");
    branch_down=inputTree->Branch("pileupweight_down",&weight_down,"pileupweight_down/F");
    Int_t numEntries = inputTree->GetEntries();
    cout<<"total number of events: "<<numEntries<<endl;
    for (Int_t i = 0; i < numEntries; i++) {
        inputTree->GetEntry(i);
        weight = reweightHist->GetBinContent(reweightHist->GetXaxis()->FindBin(nPU));
        weight_up = reweightHist_up->GetBinContent(reweightHist_up->GetXaxis()->FindBin(nPU));
        weight_down = reweightHist_down->GetBinContent(reweightHist_down->GetXaxis()->FindBin(nPU));
        branch->Fill();
        branch_up->Fill();
        branch_down->Fill();
    }
    inputTree->Write("",TObject::kOverwrite);
    delete inputTree;
    input->Close();
}
