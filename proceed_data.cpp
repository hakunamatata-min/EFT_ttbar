#include<TROOT.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1F.h>
#include<TH2F.h>
#include<iostream>
#include<map>
using namespace std;
typedef pair<pair<UInt_t, UInt_t>, ULong64_t> id;
//int main(){
void proceed_data(){
    map<id, bool> event_map;
    pair<id, bool> event_id; 
    TChain oldtree("mytree");
    oldtree.Add("new_*.root");
    UInt_t luminosityBlock, run;
    ULong64_t event;    
    Float_t Score_csvv2, Score_deepcsv, Score_deepjet;
    Float_t MET_phi, MET_pt, jet_btagDeepB[50], jet_btagDeepFlavB[50], jet_btagCSVV2[50],jet_eta[50], jet_pt[50], jet_phi[50], jet_mass[50], electron_deltaEtaSC;
    Float_t lepton_eta, lepton_pt, lepton_phi, lepton_mass;
    Float_t mass_bjj, mass_jj, mass_lb;
    Float_t rectop_pt, recantitop_pt, rapidity_tt, mass_tt, neutrino_pz, mass_thad, mass_tlep, MtW;
    Float_t mass_wlep, mass_whad, mass_t, mass_at;
    Bool_t lep_flavour, HLT_IsoMu24, HLT_Ele32_WPTight_Gsf;
    Double_t  minimum, chi;
    UInt_t jet_num, nBtag;
    oldtree.SetBranchAddress("luminosityBlock",&luminosityBlock);
    oldtree.SetBranchAddress("event",&event);
    oldtree.SetBranchAddress("run",&run);
    oldtree.SetBranchAddress("MET_phi", &MET_phi);
    oldtree.SetBranchAddress("MET_pt", &MET_pt);
    oldtree.SetBranchAddress("lepton_eta", &lepton_eta);
    oldtree.SetBranchAddress("lepton_pt", &lepton_pt);
    oldtree.SetBranchAddress("lepton_phi", &lepton_phi);
    oldtree.SetBranchAddress("lepton_mass", &lepton_mass);
    oldtree.SetBranchAddress("lep_flavour", &lep_flavour);
    oldtree.SetBranchAddress("jet_num", &jet_num);
    oldtree.SetBranchAddress("nBtag", &nBtag);
    oldtree.SetBranchAddress("jet_btagDeepB", jet_btagDeepB);
    oldtree.SetBranchAddress("jet_btagDeepFlavB", jet_btagDeepFlavB);
    oldtree.SetBranchAddress("jet_eta", jet_eta);
    oldtree.SetBranchAddress("jet_pt", jet_pt);
    oldtree.SetBranchAddress("jet_phi", jet_phi);
    oldtree.SetBranchAddress("jet_mass", jet_mass);
    oldtree.SetBranchAddress("electron_deltaEtaSC",&electron_deltaEtaSC);
    oldtree.SetBranchAddress("rectop_pt", &rectop_pt);
    oldtree.SetBranchAddress("recantitop_pt", &recantitop_pt);
    oldtree.SetBranchAddress("rapidity_tt", &rapidity_tt);
    oldtree.SetBranchAddress("mass_tt", &mass_tt);
    oldtree.SetBranchAddress("neutrino_pz", &neutrino_pz);
    oldtree.SetBranchAddress("mass_thad", &mass_thad);
    oldtree.SetBranchAddress("mass_tlep", &mass_tlep);
    oldtree.SetBranchAddress("mass_whad", &mass_whad);
    oldtree.SetBranchAddress("mass_wlep", &mass_wlep);
    oldtree.SetBranchAddress("mass_t", &mass_t);
    oldtree.SetBranchAddress("mass_at", &mass_at);
    oldtree.SetBranchAddress("mass_jj", &mass_jj);
    oldtree.SetBranchAddress("mass_bjj", &mass_bjj);
    oldtree.SetBranchAddress("mass_lb", &mass_lb);
    oldtree.SetBranchAddress("MtW",&MtW);
    oldtree.SetBranchAddress("likelihood",&minimum);
    oldtree.SetBranchAddress("chi2",&chi);
    int num_f = 0;
    pair<map<id, bool>::iterator, bool> isexsit;
    TFile* outFile;
    TTree* mytree;
    cout<<oldtree.GetEntries()<<endl;
    for(int entry=0; entry<oldtree.GetEntries();entry++){
        oldtree.GetEntry(entry);
        event_id.first.first.first=run;
        event_id.first.first.second=luminosityBlock;
        event_id.first.second=event;
        event_id.second=true;
        isexsit = event_map.insert(event_id);
        if(entry%510000==0){
            cout<<num_f<<endl;
            outFile=new TFile(Form("new_data_%d_A.root",num_f),"RECREATE");
            mytree=new TTree("mytree", "selected data");
            mytree->Branch("MET_phi", &MET_phi, "MET_phi/F");
            mytree->Branch("MET_pt", &MET_pt, "MET_pt/F");
            mytree->Branch("lepton_eta", &lepton_eta, "lepton_eta/F");
            mytree->Branch("lepton_pt", &lepton_pt, "lepton_pt/F");
            mytree->Branch("lepton_phi", &lepton_phi, "lepton_phi/F");
            mytree->Branch("lepton_mass", &lepton_mass, "lepton_mass/F");
            mytree->Branch("lep_flavour", &lep_flavour, "lep_flavour/O");
            mytree->Branch("jet_num", &jet_num,"jet_num/i"); // number of jets satisfy the  seletion criteria
            mytree->Branch("nBtag", &nBtag, "nBtag/i");
            mytree->Branch("jet_btagDeepB", jet_btagDeepB, "jet_btagDeepB[jet_num]/F");
            mytree->Branch("jet_btagDeepFlavB", jet_btagDeepFlavB, "jet_btagDeepFlavB[jet_num]/F");
            mytree->Branch("jet_eta", jet_eta, "jet_eta[jet_num]/F");
            mytree->Branch("jet_pt", jet_pt, "jet_pt[jet_num]/F");
            mytree->Branch("jet_phi", jet_phi, "jet_phi[jet_num]/F");
            mytree->Branch("jet_mass", jet_mass, "jet_mass[jet_num]/F");
            mytree->Branch("electron_deltaEtaSC",&electron_deltaEtaSC, "electron_deltaEtaSC/F");
            mytree->Branch("rectop_pt", &rectop_pt, "rectop_pt/F");
            mytree->Branch("recantitop_pt", &recantitop_pt, "recantitop_pt/F");
            mytree->Branch("rapidity_tt", &rapidity_tt, "rapidity_tt/F");
            mytree->Branch("mass_tt", &mass_tt, "mass_tt/F");
            mytree->Branch("neutrino_pz", &neutrino_pz, "neutrino_pz/F");
            mytree->Branch("mass_thad", &mass_thad, "mass_thad/F");
            mytree->Branch("mass_tlep", &mass_tlep, "mass_tlep/F");
            mytree->Branch("mass_whad", &mass_whad, "mass_whad/F");
            mytree->Branch("mass_wlep", &mass_wlep, "mass_wlep/F");
            mytree->Branch("mass_t", &mass_t, "mass_t/F");
            mytree->Branch("mass_at", &mass_at, "mass_at/F");
            mytree->Branch("MtW",&MtW,"MtW/F");
            mytree->Branch("mass_bjj", &mass_bjj, "mass_bjj/F");
            mytree->Branch("mass_jj", &mass_jj, "mass_jj/F");
            mytree->Branch("mass_lb", &mass_lb, "mass_lb/F");
            mytree->Branch("likelihood",&minimum,"minimum/D" );
            mytree->Branch("chi2", &chi, "chi2/D");
            num_f++;
        }
        if(isexsit.second)
            mytree->Fill();
        if((entry+1)%510000==0 || entry==oldtree.GetEntries()-1){
            outFile->cd();
            mytree->Write();
            delete mytree;
            outFile->Close();
            //delete outFile;
        }
    }
}
