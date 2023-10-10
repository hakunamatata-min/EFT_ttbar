#include "get_LHE_info.h"

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
    for (int entry = 0; entry < chain->GetEntries(); entry++)
    {
        LHE_n = 0;
        chain->GetEntry(entry);
        for (int i = 0; i < nLHEPart; i++)
        {
            if (LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == -2)
                LHE_n++;
            if (LHEPart_pdgId[i] == 3 || LHEPart_pdgId[i] == -3 || LHEPart_pdgId[i] == 4 || LHEPart_pdgId[i] == -4)
                LHE_n++;
            if (LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13 || LHEPart_pdgId[i] == -13)
                LHE_n++;
            if (LHEPart_pdgId[i] == 15 || LHEPart_pdgId[i] == -15)
                LHE_n++;
        }
        if (LHE_n == 2)
        {
            //leptonic decay t -> W+b , W+ -> e+, mu+, tau+ nu
            for (int i = 0; i < nLHEPart; i++)
            {
                if (LHEPart_pdgId[i] == 5)
                    index_b = i;
                else if (LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13 || LHEPart_pdgId[i] == -15)
                    index_antilep = i;
                else if (LHEPart_pdgId[i] == 12 || LHEPart_pdgId[i] == 14 || LHEPart_pdgId[i] == 16)
                    index_nub = i;
                else if (LHEPart_pdgId[i] == -5)
                    index_antib = i;
                else if (LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == 13 || LHEPart_pdgId[i] == 15)
                    index_lep = i;
                else if (LHEPart_pdgId[i] == -12 || LHEPart_pdgId[i] == -14 || LHEPart_pdgId[i] == -16)
                    index_nuantib = i;
            }
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b], LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib], LHEPart_phi[index_antib], LHEPart_mass[index_antib]);
            p4_lep.SetPtEtaPhiM(LHEPart_pt[index_lep], LHEPart_eta[index_lep], LHEPart_phi[index_lep], LHEPart_mass[index_lep]);
            p4_antilep.SetPtEtaPhiM(LHEPart_pt[index_antilep], LHEPart_eta[index_antilep], LHEPart_phi[index_antilep], LHEPart_mass[index_antilep]);
            p4_nub.SetPtEtaPhiM(LHEPart_pt[index_nub], LHEPart_eta[index_nub], LHEPart_phi[index_nub], LHEPart_mass[index_nub]);
            p4_nuantib.SetPtEtaPhiM(LHEPart_pt[index_nuantib], LHEPart_eta[index_nuantib], LHEPart_phi[index_nuantib], LHEPart_mass[index_nuantib]);
            p4_top = p4_b + p4_antilep + p4_nub;
            p4_antitop = p4_antib + p4_lep + p4_nuantib;
        }
        if (LHE_n == 3)
        {
            //semi_leptonid decay, ttbar->W+bW-bbar, W+->e+, mu+, tau+ nu, W- -> ubrard, cbars; W+ ->udbar, csbar, W- -> e-, mu-, tau- nu
            for (int i = 0; i < nLHEPart; i++)
            {
                if (LHEPart_pdgId[i] == 5)
                    index_b = i;
                else if (LHEPart_pdgId[i] == -5)
                    index_antib = i;
                else if (LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == 4 || LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
                    index_q1 = i;
                else if (LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == 3 || LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == -3)
                    index_q2 = i;
                else if (LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == 13 || LHEPart_pdgId[i] == 15 || LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13 || LHEPart_pdgId[i] == -15)
                    index_lep = i;
                else if (LHEPart_pdgId[i] == 12 || LHEPart_pdgId[i] == 14 || LHEPart_pdgId[i] == 16 || LHEPart_pdgId[i] == -12 || LHEPart_pdgId[i] == -14 || LHEPart_pdgId[i] == -16)
                    index_nu = i;
            }
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b], LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib], LHEPart_phi[index_antib], LHEPart_mass[index_antib]);
            p4_lep.SetPtEtaPhiM(LHEPart_pt[index_lep], LHEPart_eta[index_lep], LHEPart_phi[index_lep], LHEPart_mass[index_lep]);
            p4_nu.SetPtEtaPhiM(LHEPart_pt[index_nu], LHEPart_eta[index_nu], LHEPart_phi[index_nu], LHEPart_mass[index_nu]);
            p4_q1.SetPtEtaPhiM(LHEPart_pt[index_q1], LHEPart_eta[index_q1], LHEPart_phi[index_q1], LHEPart_mass[index_q1]);
            p4_q2.SetPtEtaPhiM(LHEPart_pt[index_q2], LHEPart_eta[index_q2], LHEPart_phi[index_q2], LHEPart_mass[index_q2]);
            if (LHEPart_pdgId[index_lep] > 0)
            {
                p4_antitop = p4_antib + p4_lep + p4_nu;
                p4_top = p4_b + p4_q1 + p4_q2;
            }
            else
            {
                p4_top = p4_b + p4_lep + p4_nu;
                p4_antitop = p4_antib + p4_q1 + p4_q2;
            }
        }
        if (LHE_n == 4)
        {
            //hadronic decay, ttbar->W+bW-bbar, W+-> udbar, csbar, W- -> ubard, cbars
            for (int i = 0; i < nLHEPart; i++)
            {
                if (LHEPart_pdgId[i] == 5)
                    index_b = i;
                else if (LHEPart_pdgId[i] == -5)
                    index_antib = i;
                else if (LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == 4)
                    index_q1 = i;
                else if (LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == -3)
                    index_q2 = i;
                else if (LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
                    index_q3 = i;
                else if (LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == -3)
                    index_q4 = i;
            }
            p4_b.SetPtEtaPhiM(LHEPart_pt[index_b], LHEPart_eta[index_b], LHEPart_phi[index_b], LHEPart_mass[index_b]);
            p4_antib.SetPtEtaPhiM(LHEPart_pt[index_antib], LHEPart_eta[index_antib], LHEPart_phi[index_antib], LHEPart_mass[index_antib]);
            p4_q1.SetPtEtaPhiM(LHEPart_pt[index_q1], LHEPart_eta[index_q1], LHEPart_phi[index_q1], LHEPart_mass[index_q1]);
            p4_q2.SetPtEtaPhiM(LHEPart_pt[index_q2], LHEPart_eta[index_q2], LHEPart_phi[index_q2], LHEPart_mass[index_q2]);
            p4_q3.SetPtEtaPhiM(LHEPart_pt[index_q3], LHEPart_eta[index_q3], LHEPart_phi[index_q3], LHEPart_mass[index_q3]);
            p4_q4.SetPtEtaPhiM(LHEPart_pt[index_q4], LHEPart_eta[index_q4], LHEPart_phi[index_q4], LHEPart_mass[index_q4]);
            p4_top = p4_b + p4_q1 + p4_q2;
            p4_antitop = p4_antib + p4_q3 + p4_q4;
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
        }
        mytree->Fill();
    }
    output->cd();
    mytree->Write();
    delete mytree;
    output->Close();
    delete chain;
}
void get_LHE_info(){
    TString inputfile[5]={
        "TT1.root",
        "TT2.root",
        "TT3.root",
        "TT4.root",
        "TT5.root",
    };
    TString input_dir = "/home/tangmin/EFT_ttbar/test/input/";
    TString output_dir = "/home/tangmin/EFT_ttbar/test/output/";
    for (int i = 0; i < 5; i++){
        read_LHE(input_dir+inputfile[i], output_dir+"new_"+inputfile[i]);
    }
}