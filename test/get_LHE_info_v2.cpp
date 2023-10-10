#include "get_LHE_info_v2.h"

void read_LHE(TString input, TString outputFile)
{
    if (!input.Contains("TT"))
        return;
    TChain *chain = new TChain("Events");
    chain->Add(input);
    TFile* output = new TFile(outputFile, "recreate");
    cout << input << " is reading and processing" << endl;
    chain->SetBranchAddress("LHEPart_eta", LHEPart_eta);
    chain->SetBranchAddress("LHEPart_mass", LHEPart_mass);
    chain->SetBranchAddress("LHEPart_phi", LHEPart_phi);
    chain->SetBranchAddress("LHEPart_pt", LHEPart_pt);
    chain->SetBranchAddress("LHEPart_pdgId", LHEPart_pdgId);
    chain->SetBranchAddress("nLHEPart", &nLHEPart);
    TTree *mytree = new TTree("mytree", " tree with branches of mytree");
    mytree->Branch("top_mass", &top_mass, "top_mass/F");
    mytree->Branch("antitop_mass", &antitop_mass, "antitop_mass/F");
    for (int entry = 0; entry < 10000; entry++)
    {
        LHE_n = 0;
        chain->GetEntry(entry);
        for (int i = 3; i < nLHEPart; i++)
        {
            if (LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == -2)
                LHE_n++;
            if (LHEPart_pdgId[i] == 3 || LHEPart_pdgId[i] == -3 || LHEPart_pdgId[i] == 4 || LHEPart_pdgId[i] == -4)
                LHE_n++;
            if (LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == 13 || LHEPart_pdgId[i] == -13)
                LHE_n++;
            if (LHEPart_pdgId[i] == 15 || LHEPart_pdgId[i] == -15)
                LHE_n++;
            //cout<<LHE_n<<endl;
            //cout << LHEPart_pdgId[i] << endl;
        }
        
        if (LHE_n == 2)
        {
            //cout<<39<<" "<<entry<< endl;
            //leptonic decay t -> W+b , W+ -> e+, mu+, tau+ nu
            for(int i = 0; i < nLHEPart; i++){
            if (LHEPart_pdgId[i] == 5) index_b = i;
            else if (LHEPart_pdgId[i] == -5) index_antib = i;
            else if (LHEPart_pdgId[i] == 11|| LHEPart_pdgId[i] == 13||LHEPart_pdgId[i] == 15)
                index_lepn = i;
            else if (LHEPart_pdgId[i] == 12 || LHEPart_pdgId[i] == 14 || LHEPart_pdgId[i] == 16)
                index_nun = i;
            else if (LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13 || LHEPart_pdgId[i] == -15)
                index_lepp = i;
            else if (LHEPart_pdgId[i] == -12 ||LHEPart_pdgId[i] == -14 || LHEPart_pdgId[i] == -16)
                index_nup = i;
        } 
        p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b],LHEPart_phi[index_b], LHEPart_mass[index_b]);
        p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib],LHEPart_mass[index_antib]);
        p4_lepp.SetPtEtaPhiM(LHEPart_pt[index_lepp], LHEPart_eta[index_lepp],LHEPart_phi[index_lepp], LHEPart_mass[index_lepp]);
        p4_lepn.SetPtEtaPhiM(LHEPart_pt[index_lepn], LHEPart_eta[index_lepn],LHEPart_phi[index_lepn], LHEPart_mass[index_lepn]);
        p4_nup.SetPtEtaPhiM(LHEPart_pt[index_nup], LHEPart_eta[index_nup],LHEPart_phi[index_nup], LHEPart_mass[index_nup]);
        p4_nun.SetPtEtaPhiM(LHEPart_pt[index_nun], LHEPart_eta[index_nun],LHEPart_phi[index_nun], LHEPart_mass[index_nun]);
        p4_top = p4_b + p4_lepp + p4_nun;
        p4_antitop = p4_antib + p4_lepn + p4_nup; 
    
        }
        if (LHE_n == 3)
        {
            //semi_leptonid decay, ttbar->W+bW-bbar, W+->e+, mu+, tau+ nu, W- -> ubrard, cbars; W+ ->udbar, csbar, W- -> e-, mu-, tau- nu
             for (int i = 0; i < nLHEPart; i++){
            if(LHEPart_pdgId[i] == 5)
                index_b = i;
            else if(LHEPart_pdgId[i] == -5)
                index_antib = i;
            else if(LHEPart_pdgId[i] == 2||LHEPart_pdgId[i] == 4||LHEPart_pdgId[i] == -2||LHEPart_pdgId[i] == -4)
                index_up = i;
            else if (LHEPart_pdgId[i] == 1||LHEPart_pdgId[i] == 3||LHEPart_pdgId[i] == -1||LHEPart_pdgId[i] == -3)
                index_down = i;
            else if (LHEPart_pdgId[i] == 11||LHEPart_pdgId[i] == 13||LHEPart_pdgId[i] == 15||LHEPart_pdgId[i]== -11||LHEPart_pdgId[i] == -13||LHEPart_pdgId[i] == -15)
                index_lep = i;
            else if (LHEPart_pdgId[i] == 12||LHEPart_pdgId[i] == 14||LHEPart_pdgId[i] == 16||LHEPart_pdgId[i] == -12||LHEPart_pdgId[i] == -14||LHEPart_pdgId[i] == -16)
                index_nu = i;
        }
        p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b], LHEPart_phi[index_b], LHEPart_mass[index_b]);
        p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib], LHEPart_mass[index_antib]);
        p4_up.SetPtEtaPhiM(LHEPart_pt[index_up], LHEPart_eta[index_up], LHEPart_phi[index_up], LHEPart_mass[index_up]);
        p4_down.SetPtEtaPhiM(LHEPart_pt[index_down], LHEPart_eta[index_down], LHEPart_phi[index_down], LHEPart_mass[index_down]);
        p4_lep.SetPtEtaPhiM(LHEPart_pt[index_lep], LHEPart_eta[index_lep], LHEPart_phi[index_lep], LHEPart_mass[index_lep]);
        p4_nu.SetPtEtaPhiM(LHEPart_pt[index_nu], LHEPart_eta[index_nu], LHEPart_phi[index_nu], LHEPart_mass[index_nu]);
        if(LHEPart_pdgId[index_lep] > 0){
            p4_antitop = p4_antib + p4_lep + p4_nu;
            p4_top = p4_b + p4_up + p4_down;
           
        } 
        else{
            p4_top = p4_b + p4_lep + p4_nu;
            p4_antitop = p4_antib + p4_up + p4_down;
           
        }
        }
        if (LHE_n == 4)
        {
            //cout<<99<<endl;
            //hadronic decay, ttbar->W+bW-bbar, W+-> udbar, csbar, W- -> ubard, cbars
            for(int i = 0; i < nLHEPart; i++) {
            if (LHEPart_pdgId[i] == 5) index_b = i;
            else if (LHEPart_pdgId[i] == -5) index_antib = i;
            else if (LHEPart_pdgId[i] == 2|| LHEPart_pdgId[i] == 4)
                index_up = i;
            else if (LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
                index_upbar = i;
            else if (LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == 3)
                index_down = i;
            else if (LHEPart_pdgId[i] == -1 ||LHEPart_pdgId[i] == -3)
                index_downbar = i;
        } 
        p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b],LHEPart_phi[index_b], LHEPart_mass[index_b]);
        p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib],LHEPart_phi[index_antib],LHEPart_mass[index_antib]);
        p4_up.SetPtEtaPhiM(LHEPart_pt[index_up], LHEPart_eta[index_up],LHEPart_phi[index_up], LHEPart_mass[index_up]);
        p4_upbar.SetPtEtaPhiM(LHEPart_pt[index_upbar], LHEPart_eta[index_upbar],LHEPart_phi[index_upbar], LHEPart_mass[index_upbar]);
        p4_down.SetPtEtaPhiM(LHEPart_pt[index_down], LHEPart_eta[index_down],LHEPart_phi[index_down], LHEPart_mass[index_down]);
        p4_downbar.SetPtEtaPhiM(LHEPart_pt[index_downbar], LHEPart_eta[index_downbar],LHEPart_phi[index_downbar], LHEPart_mass[index_downbar]);
        p4_top = p4_b + p4_up + p4_downbar;
        p4_antitop = p4_antib + p4_upbar + p4_down; 
        }
        if (input.Contains("TT"))
        {
            top_pt = p4_top.Pt();
            top_eta = p4_top.Eta();
            top_phi = p4_top.Phi();
            top_mass = p4_top.M();
            antitop_pt = p4_antitop.Pt();
            antitop_eta = p4_antitop.Eta();
            antitop_phi = p4_antitop.Phi();
            antitop_mass = p4_antitop.M();
            //cout << top_pt << " " << top_eta << " " << top_phi << " " << top_mass << endl;
        }
        mytree->Fill();
    }
    output->cd();
    mytree->Write();
    delete mytree;
    output->Close();
    delete chain;
}
void get_LHE_info_v2(){
    TString inputfile[5]={
        "TT1.root",
        "TT2.root",
        "TT3.root",
        "TT4.root",
        "TT5.root",
    };
    TString input_dir = "/home/tangmin/EFT_ttbar/test/input/";
    TString output_dir = "/home/tangmin/EFT_ttbar/test/output/v2/";
    for (int i = 0; i < 5; i++){
        read_LHE(input_dir+inputfile[i], output_dir+"new_"+inputfile[i]);
    }
}