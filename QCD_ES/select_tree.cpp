#include "select_tree.h"
#include "reco.cpp"
//type: 0:MC, 1:data
read_object::read_object(TString input, Bool_t type){
    TChain* chain = new TChain("Events");
    UInt_t nMuon, nJet, nElectron, nGenJet;
    chain->Add(input);
    cout<<input<<" is reading and processing"<<endl;
    cout<<"total number of events: "<<chain->GetEntries()<<endl;
    chain->SetBranchAddress("nMuon", &nMuon);
    chain->SetBranchAddress("nJet", &nJet);
    chain->SetBranchAddress("nElectron", &nElectron);
    if(!type)
        chain->SetBranchAddress("nGenJet", &nGenJet);
    nm = 0, ne = 0, nj = 0, ng = 0;
    for(int num=0; num<chain->GetEntries(); num++){
        chain->GetEntry(num);
        if(nm < nMuon)
            nm = nMuon;
        if(ne < nElectron)
            ne = nElectron;
        if(nj < nJet)
            nj=nJet;
        if(!type){
            if(ng < nGenJet)
                ng = nGenJet;
        }
    }
    delete chain;
}

Bool_t select_tree::loose_noiso(Int_t i){
    Bool_t flag=true;
    int bitmap = Electron_vidNestedWPBitmap[i];
    for(int i=0;i<10;i++){
        if(i!=7){
            if((bitmap>>(3*i))%8<2){
                flag=false;
                break;
            }
        }
    }
    return flag;
}
Bool_t select_tree::tight_noiso(Int_t i){
    Bool_t flag=true;
    int bitmap = Electron_vidNestedWPBitmap[i];
    for(int i=0;i<10;i++){
        if(i!=7){
            if((bitmap>>(3*i))%8<4){
                flag=false;
                break;
            }
        }
    }
    return flag;
}
Int_t select_tree::iso_select(Int_t i)
{
    Float_t eta_sc = Electron_deltaEtaSC[i]+Electron_eta[i];
    Float_t iso_value = Electron_pfRelIso03_all[i];
    Float_t pt = Electron_pt[i];
    if(category<2){
        if(fabs(eta_sc)<=1.479){
            if(iso_value<0.0287+0.506/pt)
                return 2;
                //selection
            else if(iso_value<0.112+0.506/pt)
                return 1;
                //veto
            else
                return 0;
        }
        else{
            if(iso_value<0.0445+0.963/pt)
                return 2;
            else if(iso_value<0.108+0.963/pt)
                return 1;
            else
                return 0;
        }
    }
    else{
        if(fabs(eta_sc)<=1.479){
            if(iso_value>0.0287+0.506/pt)
                return 2;
            else
                return 1;
        }
        else{
            if(iso_value>0.0445+0.963/pt)
                return 2;
            else
                return 1;
        }
    }
}
Bool_t select_tree::is_lep_from_jet(TLorentzVector mom_lep){
    Bool_t flag=false;
    if(category>1)
        return false;
    for(int i=0; i<jet_num; i++){
        if(mom_lep.DeltaR(mom_jets[i])<0.4){
            flag=true;
            break;
        }
    }
    return flag;
}
select_tree::select_tree(TString inputFile, TString outputFile, int sample_year, int s_cate, bool s_type, int num_j, int num_e, int num_m, int num_g){
    input = outputFile;
    year = sample_year;
    type = s_type;
    category = s_cate;
    Float_t btag_num[]={0.2589,0.2489,0.3040,0.2783};
    btag_criteria = btag_num[year-2015];
    TString cate[]={"_A.root","_B.root","_C.root","_D.root"};
    outputFile = outputFile.ReplaceAll(".root",cate[category]); 
    nj = num_j;
    ne = num_e;
    nm = num_m;
    ng = num_g;
    chain = new TChain("Events");
    chain->Add(inputFile);
    output = new TFile(outputFile, "RECREATE");
    chain->SetBranchAddress("nMuon", &nMuon);
    chain->SetBranchAddress("nJet", &nJet);
    chain->SetBranchAddress("nElectron", &nElectron);

    if(type == false){
        GenJet_pt = new Float_t[ng];
        GenJet_mass = new Float_t[ng];
        GenJet_phi = new Float_t[ng];
        GenJet_eta = new Float_t[ng];
        jet_partonFlavour = new Int_t[nj];
        Jet_partonFlavour = new Int_t[nj];
        jet_hadronFlavour = new Int_t[nj];
        Jet_hadronFlavour = new Int_t[nj];
        chain->SetBranchAddress("nGenJet", &nGenJet);
        chain->SetBranchAddress("GenJet_eta", GenJet_eta);
        chain->SetBranchAddress("GenJet_pt", GenJet_pt);
        chain->SetBranchAddress("GenJet_phi", GenJet_phi);
        chain->SetBranchAddress("GenJet_mass", GenJet_mass);
        chain->SetBranchAddress("Jet_partonFlavour", Jet_partonFlavour);
        chain->SetBranchAddress("Jet_hadronFlavour", Jet_hadronFlavour);
        chain->SetBranchAddress("Generator_weight",&Generator_weight);
        chain->SetBranchAddress("L1PreFiringWeight_Nom", &L1PreFiringWeight_Nom);
        chain->SetBranchAddress("L1PreFiringWeight_Up", &L1PreFiringWeight_Up);
        chain->SetBranchAddress("L1PreFiringWeight_Dn", &L1PreFiringWeight_Dn);
        chain->SetBranchAddress("Pileup_nPU", &Pileup_nPU);
    }

    Electron_eta = new Float_t[ne];
    Electron_mass = new Float_t[ne];
    Electron_pt = new Float_t[ne];
    Electron_phi = new Float_t[ne];
    Electron_charge = new Int_t[ne];
    Electron_cutBased = new Int_t[ne];
    Electron_deltaEtaSC = new Float_t[ne];
    Electron_dxy = new Float_t[ne];
    Electron_dz = new Float_t[ne];
    Electron_pfRelIso03_all = new Float_t[ne];
    Electron_vidNestedWPBitmap = new Int_t[ne];
    
    Muon_mass = new Float_t[nm];
    Muon_phi = new Float_t[nm];
    Muon_pt = new Float_t[nm];
    Muon_eta = new Float_t[nm];
    Muon_charge = new Int_t[nm];
    Muon_pfRelIso04_all = new Float_t[nm];
    Muon_tightId = new Bool_t[nm];
    Muon_looseId = new Bool_t[nm];
    Muon_highPtId = new UChar_t[nm];

    Jet_btagDeepB = new Float_t[nj];
    Jet_btagDeepFlavB = new Float_t[nj];
    Jet_mass = new Float_t[nj];
    Jet_pt = new Float_t[nj];
    Jet_eta = new Float_t[nj];
    Jet_phi = new Float_t[nj];
    Jet_jetId = new Int_t[nj];

    jet_btagDeepB = new Float_t[nj];
    jet_btagDeepFlavB = new Float_t[nj];
    jet_mass = new Float_t[nj];
    jet_pt = new Float_t[nj];
    jet_eta = new Float_t[nj];
    jet_phi = new Float_t[nj];
    mom_jets = new TLorentzVector[nj];

    if(input.Contains("TT")){
        chain->SetBranchAddress("LHEPart_eta", LHEPart_eta);
        chain->SetBranchAddress("LHEPart_mass", LHEPart_mass);
        chain->SetBranchAddress("LHEPart_phi", LHEPart_phi);
        chain->SetBranchAddress("LHEPart_pt", LHEPart_pt);
        chain->SetBranchAddress("LHEPart_pdgId", LHEPart_pdgId);
        chain->SetBranchAddress("nLHEPart", &nLHEPart);
    }
    chain->SetBranchAddress("MET_pt", &MET_pt);
    chain->SetBranchAddress("MET_phi", &MET_phi);
    chain->SetBranchAddress("Electron_phi", Electron_phi);
    chain->SetBranchAddress("Electron_pt", Electron_pt);
    chain->SetBranchAddress("Electron_mass", Electron_mass);
    chain->SetBranchAddress("Electron_eta", Electron_eta);
    chain->SetBranchAddress("nElectron", &nElectron);
    chain->SetBranchAddress("Electron_charge", Electron_charge);
    chain->SetBranchAddress("nMuon", &nMuon);
    chain->SetBranchAddress("nJet", &nJet);
    chain->SetBranchAddress("Muon_eta", Muon_eta);
    chain->SetBranchAddress("Muon_pt", Muon_pt);
    chain->SetBranchAddress("Muon_phi", Muon_phi);
    chain->SetBranchAddress("Muon_mass", Muon_mass);
    chain->SetBranchAddress("Muon_charge", Muon_charge);
    chain->SetBranchAddress("Jet_btagDeepB", Jet_btagDeepB);
    chain->SetBranchAddress("Jet_btagDeepFlavB", Jet_btagDeepFlavB);
    chain->SetBranchAddress("Jet_eta", Jet_eta);
    chain->SetBranchAddress("Jet_pt", Jet_pt);
    chain->SetBranchAddress("Jet_mass", Jet_mass);
    chain->SetBranchAddress("Jet_phi", Jet_phi);

    chain->SetBranchAddress("Muon_pfRelIso04_all",Muon_pfRelIso04_all);
    chain->SetBranchAddress("Muon_tightId",Muon_tightId);
    chain->SetBranchAddress("Muon_looseId",Muon_looseId);
    chain->SetBranchAddress("Muon_highPtId",Muon_highPtId);
    chain->SetBranchAddress("Electron_cutBased",Electron_cutBased);
    chain->SetBranchAddress("Electron_vidNestedWPBitmap",Electron_vidNestedWPBitmap);
    chain->SetBranchAddress("Electron_pfRelIso03_all",Electron_pfRelIso03_all);
    chain->SetBranchAddress("Jet_jetId",Jet_jetId);
    chain->SetBranchAddress("Electron_deltaEtaSC",Electron_deltaEtaSC);
    chain->SetBranchAddress("Electron_dz",Electron_dz);
    chain->SetBranchAddress("Electron_dxy",Electron_dxy);
    chain->SetBranchAddress("PV_npvsGood",&PV_npvsGood);
    
    chain->SetBranchAddress("Flag_goodVertices", &Flag_met[0]);
    chain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_met[1]);
    chain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_met[2]);
    chain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_met[3]);
    chain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_met[4]);
    chain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_met[5]);
    chain->SetBranchAddress("Flag_BadPFMuonDzFilter", &Flag_met[6]);
    chain->SetBranchAddress("Flag_hfNoisyHitsFilter", &Flag_met[7]);
    chain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_met[8]);
    chain->SetBranchAddress("Flag_eeBadScFilter", &Flag_met[9]);

    chain->SetBranchAddress("luminosityBlock",&luminosityBlock);
    chain->SetBranchAddress("event",&event);
    chain->SetBranchAddress("run",&run);
    chain->SetBranchAddress("HLT_Mu27", &HLT_Mu27);//new
    chain->SetBranchAddress("HLT_Ele23_CaloIdM_TrackIdM_PFJet30", &HLT_Ele23_CaloIdM_TrackIdM_PFJet30);//new
    if(year==2018){
        chain->SetBranchAddress("HLT_Ele32_WPTight_Gsf", &HLT_Ele32_WPTight_Gsf);
        chain->SetBranchAddress("HLT_Ele115_CaloIdVT_GsfTrkIdT", &HLT_Ele115_CaloIdVT_GsfTrkIdT);
        
        chain->SetBranchAddress("HLT_IsoMu24", &HLT_IsoMu24);
        chain->SetBranchAddress("HLT_Mu50", &HLT_Mu50);
        chain->SetBranchAddress("HLT_OldMu100", &HLT_OldMu100);
        chain->SetBranchAddress("HLT_TkMu100", &HLT_TkMu100);
        chain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_met[10]);
        nFlag_met = 11;
    }
    else if(year==2016||year==2015){//2015 means 2016_pre
        chain->SetBranchAddress("HLT_Ele27_WPTight_Gsf", &HLT_Ele27_WPTight_Gsf);
        chain->SetBranchAddress("HLT_Ele115_CaloIdVT_GsfTrkIdT", &HLT_Ele115_CaloIdVT_GsfTrkIdT);
        chain->SetBranchAddress("HLT_Photon175", &HLT_Photon175);
        chain->SetBranchAddress("HLT_IsoMu24", &HLT_IsoMu24);
        chain->SetBranchAddress("HLT_IsoTkMu24", &HLT_IsoTkMu24);
        chain->SetBranchAddress("HLT_Mu50", &HLT_Mu50);
        chain->SetBranchAddress("HLT_TkMu50", &HLT_TkMu50);
        nFlag_met = 10;
    }
    else{
        chain->SetBranchAddress("HLT_Ele35_WPTight_Gsf", &HLT_Ele35_WPTight_Gsf);
        chain->SetBranchAddress("HLT_Ele115_CaloIdVT_GsfTrkIdT", &HLT_Ele115_CaloIdVT_GsfTrkIdT);
        chain->SetBranchAddress("HLT_Photon200", &HLT_Photon200);
        chain->SetBranchAddress("HLT_IsoMu27", &HLT_IsoMu27);
        chain->SetBranchAddress("HLT_Mu50", &HLT_Mu50);
        chain->SetBranchAddress("HLT_OldMu100", &HLT_OldMu100);
        chain->SetBranchAddress("HLT_TkMu100", &HLT_TkMu100);
        chain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_met[10]);
        nFlag_met = 11;
    }
}

Bool_t select_tree::select_jet(){
    nBtag = 0;
    jet_num = 0; 
    max_score = 0;
    int jet_index[nj];
    Bool_t jet_flag = false; 
    UInt_t btag_numup[]={100, 0, 100, 0};
    UInt_t btag_numdown[]={2, 0, 2, 0};
    for(int i = 0; i < nJet; i++) {   
        if(fabs(Jet_eta[i]) < 2.4 && Jet_pt[i] > 30 && Jet_jetId[i]==6){
            //mom_jets[i].SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i],Jet_mass[i]);
            jet_index[jet_num] = i;
            jet_num++;
            if (Jet_btagDeepFlavB[i] > btag_criteria)
                nBtag++;
            if(Jet_btagDeepFlavB[i]>max_score)
                max_score = Jet_btagDeepFlavB[i];
        }
    }      
    if(jet_num >= 3 &&  nBtag>=btag_numdown[category] && nBtag<=btag_numup[category]){
        jet_flag = true;
        for(int i=0;i<jet_num;i++){
            mom_jets[i].SetPtEtaPhiM(Jet_pt[jet_index[i]], Jet_eta[jet_index[i]], Jet_phi[jet_index[i]],Jet_mass[jet_index[i]]);  
            jet_eta[i]=Jet_eta[jet_index[i]];
            jet_pt[i]=Jet_pt[jet_index[i]];
            jet_phi[i]=Jet_phi[jet_index[i]];
            jet_mass[i]=Jet_mass[jet_index[i]];
            jet_btagDeepB[i]=Jet_btagDeepB[jet_index[i]];
            jet_btagDeepFlavB[i]=Jet_btagDeepFlavB[jet_index[i]];
            if(type == false){
                jet_partonFlavour[i]=Jet_partonFlavour[jet_index[i]];
                jet_hadronFlavour[i]=Jet_hadronFlavour[jet_index[i]];
            }
        }
    }
    return jet_flag;
}
Bool_t select_tree::select_lep(){
    Int_t nlepton = nMuon+nElectron;
    Bool_t lepton_flag = false; // if true pass the selction
    int num_select=0;
    bool is_from_jet;
    TLorentzVector p4_lepton;
    Int_t iso_id;
    Float_t veto_muon_isodown[]={0,0,0,0};
    Float_t veto_muon_isoup[]={0.25,0.25,100000,100000};
    Float_t sel_muon_isodown[]={0,0,0.15,0.15};
    Float_t sel_muon_isoup[]={0.15,0.15,100000,100000};
    for(int i = 0; i < nlepton; i++){
        if(i < nElectron){
            p4_lepton.SetPtEtaPhiM(Electron_pt[i],Electron_eta[i],Electron_phi[i],Electron_mass[i]);
            is_from_jet=is_lep_from_jet(p4_lepton);
            iso_id = iso_select(i);
            if(loose_noiso(i) && (iso_id>=1) && fabs(Electron_eta[i]) <2.4 && (fabs(Electron_eta[i])<1.4442
                ||fabs(Electron_eta[i])>1.5660)&&Electron_pt[i]>15){
                if((fabs(Electron_deltaEtaSC[i]+Electron_eta[i])<1.479&&fabs(Electron_dxy[i])<0.05&&fabs(Electron_dz[i])<0.1)
                    ||(fabs(Electron_deltaEtaSC[i]+Electron_eta[i])>=1.479&&fabs(Electron_dxy[i])<0.1&&fabs(Electron_dz[i])<0.2))
                {
                    num_select++;
                    if(tight_noiso(i) && (iso_id==2) && fabs(Electron_eta[i]) <2.4 && (fabs(Electron_eta[i])<1.4442
                        ||fabs(Electron_eta[i])>1.5660)&&Electron_pt[i]>30 && (!is_from_jet))
                    {  
                        lep_c = Electron_charge[i];
                        mom_lep = p4_lepton;
                        lep_flavour=false;
                        lepton_flag=true;
                        electron_deltaEtaSC = Electron_deltaEtaSC[i];
                    }
                }      
            }
        }       
        else{
            p4_lepton.SetPtEtaPhiM(Muon_pt[i-nElectron],Muon_eta[i-nElectron],Muon_phi[i-nElectron],Muon_mass[i-nElectron]);
            is_from_jet=is_lep_from_jet(p4_lepton);
            if(Muon_looseId[i-nElectron]==1 && Muon_pfRelIso04_all[i-nElectron]<=veto_muon_isoup[category] && Muon_pfRelIso04_all[i-nElectron]>=veto_muon_isodown[category]
                && Muon_pt[i-nElectron]>15 && fabs(Muon_eta[i-nElectron])<2.4)
            {    
                num_select++;
                if(Muon_tightId[i-nElectron]==1 && Muon_pfRelIso04_all[i-nElectron]<=sel_muon_isoup[category] && Muon_pfRelIso04_all[i-nElectron]>=sel_muon_isodown[category] 
                    && Muon_pt[i-nElectron]>30 && fabs(Muon_eta[i-nElectron])<2.4 && (!is_from_jet))
                {
                    lep_c = Muon_charge[i-nElectron];
                    mom_lep = p4_lepton;
                    lep_flavour=true;
                    lepton_flag=true;
                    electron_deltaEtaSC = 0;
                }
            }
        }
        if(num_select > 1) {
            lepton_flag=false;
            break;
        }
    }
    return lepton_flag;
}

void select_tree::read_LHE(){
    if(!input.Contains("TT"))
        return;
    LHE_nhad = 0;
    LHE_nlep = 0;
    for (int i = nLHEPart-6; i < nLHEPart; i++) {
        if(LHEPart_pdgId[i] == 2 || LHEPart_pdgId[i] == 4 || LHEPart_pdgId[i] == -2 || LHEPart_pdgId[i] == -4)
            LHE_nhad++;
        if(LHEPart_pdgId[i] == 1 || LHEPart_pdgId[i] == 3 || LHEPart_pdgId[i] == -1 || LHEPart_pdgId[i] == -3)
            LHE_nhad++;
        if(LHEPart_pdgId[i] == 11 || LHEPart_pdgId[i] == 13 || LHEPart_pdgId[i] == -11 || LHEPart_pdgId[i] == -13)
            LHE_nlep++;
        if(LHEPart_pdgId[i] == 15 || LHEPart_pdgId[i] == -15)
            LHE_nlep++;
    }
    if (LHE_nhad==2 && LHE_nlep==1) {
        // get information of ttbar process at parton level from LHEPart
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
            lep_charge = -1;
        } 
        else{
            p4_top = p4_b + p4_lep + p4_nu;
            p4_antitop = p4_antib + p4_up + p4_down;
            lep_charge = 1;
        }
    }
    if(LHE_nhad==0 && LHE_nlep==2){
        // get information of ttbar process at parton level from LHEPart
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
    if(LHE_nhad==4 && LHE_nlep==0){
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
    if(input.Contains("TT")){
        top_pt = p4_top.Pt();
        top_eta = p4_top.Eta();
        top_phi = p4_top.Phi();
        top_mass = p4_top.M();
        antitop_pt = p4_antitop.Pt();
        antitop_eta = p4_antitop.Eta();
        antitop_phi = p4_antitop.Phi();
        antitop_mass = p4_antitop.M();
        TLorentzVector p4_top_cms = p4_top;
        TLorentzVector p4_cms_lab = p4_antitop + p4_top;
        TVector3 p3_cms = p4_cms_lab.BoostVector();
        p4_top_cms.Boost(-p3_cms);
        TVector3 p3_top_cms = p4_top_cms.Vect();
        TVector3 p3_cms_lab = p4_cms_lab.Vect();
        ctstar = p3_top_cms.Dot(p3_cms_lab)/(p3_top_cms.Mag() * p3_cms_lab.Mag());
        M_tt_gen = (p4_top + p4_antitop).M();
        delta_rapidity_gen = p4_top.Rapidity() - p4_antitop.Rapidity();
    }
}
void select_tree::loop(TTree* mytree, TTree* rawtree){
    Bool_t jet_flag, lep_flag, trigger_flag;
    for(int entry=0; entry<chain->GetEntries(); entry++){
        nLHEPart = 0;
        chain->GetEntry(entry);
        if(type == false)
            rawtree->Fill();
        index = entry;
        if(year==2018){
            if(category < 2){
                ele_trigger=HLT_Ele32_WPTight_Gsf;
                mu_trigger=HLT_IsoMu24;
            }
            else{
                //ele_trigger=HLT_Ele115_CaloIdVT_GsfTrkIdT;
                //mu_trigger=HLT_Mu50||HLT_OldMu100||HLT_TkMu100;
                ele_trigger=HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
                mu_trigger=HLT_Mu27;
            }
        }
        else if(year==2016||year==2015){
            if(category < 2){
                ele_trigger=HLT_Ele27_WPTight_Gsf;
                mu_trigger=HLT_IsoMu24||HLT_IsoTkMu24;
            }
            else{
                //ele_trigger=HLT_Ele115_CaloIdVT_GsfTrkIdT||HLT_Photon175;
                //mu_trigger=HLT_Mu50||HLT_TkMu50;
                ele_trigger=HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
                mu_trigger=HLT_Mu27;
            }
        }
        else{
            if(category < 2){
                ele_trigger=HLT_Ele35_WPTight_Gsf;
                mu_trigger=HLT_IsoMu27;
            }
            else{
                //ele_trigger=HLT_Ele115_CaloIdVT_GsfTrkIdT||HLT_Photon200;
                //mu_trigger=HLT_Mu50||HLT_OldMu100||HLT_TkMu100;
                ele_trigger=HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
                mu_trigger=HLT_Mu27;
            }
        }
        met_match = true;
        for(int i=0; i<nFlag_met; i++){
            met_match *= Flag_met[i];
        }
        if((mu_trigger||ele_trigger) && met_match){
            jet_flag = select_jet();
            if(!jet_flag)
                continue;
            lep_flag = select_lep();
            if(!lep_flag)
                continue;
            trigger_flag=false;
            if(((!lep_flavour) && ele_trigger) || (lep_flavour && mu_trigger))
                trigger_flag=true;
            if(!trigger_flag || PV_npvsGood<1)
                continue;
            RECO* reco = new RECO(jet_num, mom_jets, mom_lep, lep_c, jet_btagDeepFlavB, MET_pt, MET_phi);
            Double_t nu_px = RECO::nu_px;
            Double_t nu_py = RECO::nu_py;
            neutrino_pz = RECO::nu_pz;
            mass_thad = reco->mass_thad;
            mass_tlep = reco->mass_tlep;
            mass_whad = reco->mass_whad;
            mass_wlep = reco->mass_wlep;
            mass_t = reco->mass_t;
            mass_at = reco->mass_at;   
            mass_bjj = reco->mass_bjj;
            mass_jj = reco->mass_jj;
            mass_lb = reco->mass_lb;
            rectop_pt = reco->rectop_pt;
            recantitop_pt = reco->recantitop_pt;
            mass_tt = reco->mass_tt;
            rapidity_tt = reco->rapidity_tt;
            like = reco->like;
            chi = reco->chi;
            MtW=sqrt(2*(mom_lep.Pt()*MET_pt-mom_lep.Px()*nu_px-mom_lep.Py()*nu_py));         
            //if(MtW<140){ 
            if(MtW<140 && like<10000){
                //cout<<entry<<" "<<like<<" "<<neutrino_pz<<endl;
                lepton_pt =mom_lep.Pt();
                lepton_eta = mom_lep.Eta();
                lepton_mass = mom_lep.M();
                lepton_phi = mom_lep.Phi();
                if(type == false)
                    read_LHE();
                mytree->Fill();
            } // end of cut of minimum
            delete reco;
        }
    }
}

void select_tree::write(){
    TTree *rawtree;
    TTree *mytree = new TTree("mytree", " tree with branches");
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
    mytree->Branch("max_score",&max_score,"max_score/F");
    mytree->Branch("jet_eta", jet_eta, "jet_eta[jet_num]/F");
    mytree->Branch("jet_pt", jet_pt, "jet_pt[jet_num]/F");
    mytree->Branch("jet_phi", jet_phi, "jet_phi[jet_num]/F");
    mytree->Branch("jet_mass", jet_mass, "jet_mass[jet_num]/F");

    mytree->Branch("rapidity_tt", &rapidity_tt, "rapidity_tt/F");
    mytree->Branch("mass_tt", &mass_tt, "mass_tt/F");
    mytree->Branch("likelihood", &like, "likelihood/D" );
    mytree->Branch("chi2", &chi, "chi2/D");
    mytree->Branch("MtW",&MtW,"MtW/F");
    mytree->Branch("neutrino_pz", &neutrino_pz, "neutrino_pz/F");
    mytree->Branch("mass_thad", &mass_thad, "mass_thad/F");
    mytree->Branch("mass_tlep", &mass_tlep, "mass_tlep/F");
    mytree->Branch("mass_whad", &mass_whad, "mass_whad/F");
    mytree->Branch("mass_wlep", &mass_wlep, "mass_wlep/F");
    mytree->Branch("mass_t", &mass_t, "mass_t/F");
    mytree->Branch("mass_at", &mass_at, "mass_at/F");
    mytree->Branch("rectop_pt", &rectop_pt, "rectop_pt/F");
    mytree->Branch("recantitop_pt", &recantitop_pt, "recantitop_pt/F");
    mytree->Branch("mass_bjj", &mass_bjj, "mass_bjj/F");
    mytree->Branch("mass_jj", &mass_jj, "mass_jj/F");
    mytree->Branch("mass_lb", &mass_lb, "mass_lb/F");
    mytree->Branch("index", &index, "index/I");
    
    mytree->Branch("ele_trigger", &ele_trigger, "ele_trigger/O");
    mytree->Branch("mu_trigger", &mu_trigger, "mu_trigger/O");
    
    if(type == true){
        mytree->Branch("run",&run,"run/i");
        mytree->Branch("luminosityBlock",&luminosityBlock,"luminosityBlock/i");
        mytree->Branch("event",&event,"event/l");
    }
    else{
        rawtree = new TTree("rawtree", "tree without selection");
        rawtree->Branch("nJet", &nJet, "nJet/i");
        rawtree->Branch("Generator_weight",&Generator_weight,"Generator_weight/F");
        if(input.Contains("TT")){
            mytree->Branch("top_pt", &top_pt, "top_pt/F");
            mytree->Branch("top_eta", &top_eta, "top_eta/F");
            mytree->Branch("top_phi", &top_phi, "top_phi/F");
            mytree->Branch("top_mass", &top_mass, "top_mass/F");
            mytree->Branch("antitop_pt", &antitop_pt, "antitop_pt/F");
            mytree->Branch("antitop_eta", &antitop_eta, "antitop_eta/F");
            mytree->Branch("antitop_phi", &antitop_phi, "antitop_phi/F");
            mytree->Branch("antitop_mass", &antitop_mass, "antitop_mass/F");
            mytree->Branch("ctstar",&ctstar,"ctstar/F");
            mytree->Branch("M_tt_gen", &M_tt_gen, "M_tt_gen/F");
            mytree->Branch("delta_rapidity_gen", &delta_rapidity_gen,"delta_rapidity_gen/F");
        }
        mytree->Branch("nGenJet", &nGenJet,"nGenJet/i");
        mytree->Branch("GenJet_eta", GenJet_eta, "GenJet_eta[nGenJet]/F");
        mytree->Branch("GenJet_pt", GenJet_pt, "GenJet_pt[nGenJet]/F");
        mytree->Branch("GenJet_phi", GenJet_phi, "GenJet_phi[nGenJet]/F");
        mytree->Branch("GenJet_mass", GenJet_mass, "GenJet_mass[nGenJet]/F");
        mytree->Branch("Generator_weight",&Generator_weight,"Generator_weight/F");
        mytree->Branch("jet_hadronFlavour", jet_hadronFlavour, "jet_hadronFlavour[jet_num]/I");
        mytree->Branch("electron_deltaEtaSC",&electron_deltaEtaSC, "electron_deltaEtaSC/F");      
        mytree->Branch("L1PreFiringWeight_Nom",&L1PreFiringWeight_Nom,"L1PreFiringWeight_Nom/F");
        mytree->Branch("L1PreFiringWeight_Up",&L1PreFiringWeight_Up,"L1PreFiringWeight_Up/F");
        mytree->Branch("L1PreFiringWeight_Dn",&L1PreFiringWeight_Dn,"L1PreFiringWeight_Dn/F");
        mytree->Branch("Pileup_nPU",&Pileup_nPU,"Pileup_nPU/I");
    }

    loop(mytree, rawtree);
    output->cd();
    if(type == false){
        rawtree->Write();
        cout<<"there are "<<rawtree->GetEntries()<<" events"<<endl;
        delete rawtree;
    }
    mytree->Write();
    cout<<mytree->GetEntries()<<" events selected"<<endl;
    cout<<"there are "<<mytree->GetEntries("jet_num>=4")<<" events of 4 jets"<<endl;
    delete mytree;
}
select_tree::~select_tree(){
    if(type == false){
        delete[] GenJet_pt ;
        delete[] GenJet_mass;
        delete[] GenJet_phi;
        delete[] GenJet_eta;
        delete[] jet_partonFlavour;
        delete[] Jet_partonFlavour;
        delete[] jet_hadronFlavour;
        delete[] Jet_hadronFlavour;
    }
    delete[] Electron_eta;
    delete[] Electron_mass;
    delete[] Electron_pt;
    delete[] Electron_phi;
    delete[] Electron_charge;
    delete[] Electron_cutBased;
    delete[] Electron_deltaEtaSC;
    delete[] Electron_dxy;
    delete[] Electron_dz;
    delete[] Electron_pfRelIso03_all;
    delete[] Electron_vidNestedWPBitmap;
    
    delete[] Muon_mass;
    delete[] Muon_phi;
    delete[] Muon_pt;
    delete[] Muon_eta;
    delete[] Muon_charge;
    delete[] Muon_pfRelIso04_all;
    delete[] Muon_tightId;
    delete[] Muon_looseId;
    delete[] Muon_highPtId;

    delete[] Jet_btagDeepB;
    delete[] Jet_btagDeepFlavB;
    delete[] Jet_mass;
    delete[] Jet_pt;
    delete[] Jet_phi;
    delete[] Jet_jetId;

    delete[] jet_btagDeepB;
    delete[] jet_btagDeepFlavB;
    delete[] jet_mass;
    delete[] jet_pt;
    delete[] jet_eta;
    delete[] jet_phi;
    delete[] mom_jets;
    output->Close();
}
