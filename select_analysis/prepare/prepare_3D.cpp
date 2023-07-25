#include "prepare_3D.h"



void prepare_3D::give_sys_name(TString file, TString weight, int s, int c){    
    if(s<12){//JES*10, JER, MET_uncluster
        tree_up = sys_up[s];
        tree_dn = sys_down[s];
        file_up = file;
        file_dn = file;
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<19 || s==26 ){//SF_btag*4, SF_lepton, L1_PreFiring, Pileup, and NNLO_QCD
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_dn = file;
        weight_up = weight;
        weight_up.ReplaceAll(sys[s],sys_up[s]);
        weight_dn = weight;
        weight_dn.ReplaceAll(sys[s],sys_down[s]);
    }
    else if(s<23){//muR, muF, ISR, FSR
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_dn = file;
        weight_up = weight + "*" + sys_up[s];
        weight_dn = weight + "*" + sys_down[s];
    }
    else if(s<25){//mtop, hdamp
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_up.ReplaceAll("TuneCP5",sys_up[s]+"_TuneCP5");
        file_dn = file;
        file_dn.ReplaceAll("TuneCP5",sys_down[s]+"_TuneCP5");
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<27){//TuneCP5
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_up.ReplaceAll("TuneCP5",sys_up[s]);
        file_dn = file;
        file_dn.ReplaceAll("TuneCP5",sys_down[s]);
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<28){//EW_un
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_dn = file;
        weight_up = weight;
        weight_up = weight_up + EW[c]+ "_EWUp";
        weight_dn = weight;
        weight_dn = weight_dn + EW[c]+ "_EWDown";
    }
    //cout<<weight_up<<" "<<weight_dn<<" "<<weight<<" "<<endl;
}

void prepare_3D::renew_weight(TString* weight, TString file, int f){ //global weight
    double lumi_s[4]={19.5, 16.8, 41.48, 59.83};
    double lumi = lumi_s[year-2015];
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600);
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(dir+"/MC_new/"+file);
    TH1D *nmc=new TH1D("nmc","",50,0,100);
    //nmc->Sumw2();
    c0->cd();
    tree1->Draw("nJet>>nmc","Generator_weight");
    //cout<<nmc->GetSumOfWeights()<<endl;
    float weight1=cross_sections[f]*lumi/(nmc->GetSumOfWeights())*K_Factors[f]*1000;
    //cout<<weight1<<endl;
    *weight = Form("%f*", weight1) + (*weight);
    delete tree1;
    delete nmc;
    delete c0;
}
void prepare_3D::draw(TH3D* h1, TString file, TString tree, TString weight){
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600);
    TChain chain(tree);
    chain.Add(dir+"/MC_new/"+file);
    c0->cd();
    TString h1Name = h1->GetName();
    //h1->Sumw2();
    chain.Draw("likelihood:fabs(rapidity_tt):mass_tt>>"+h1Name, weight+"*"+cut);
    c0->cd();
    delete c0;
}
void prepare_3D::draw(int c){
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";
    TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom" + other_con1 + other_con2;
    TH3D* hist = new TH3D(process[c]+"_sub", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    hist->Sumw2();
    if(c < 5)
        weight = weight + EW[c] + "*nnlo_wt";
    for(int i=edge_i[c]; i<edge_f[c]; i++){
        weight_nom = weight;
        renew_weight(&weight_nom, fileNames[i], i);
        TH3D* h1 = new TH3D("hm1", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
        h1->Sumw2();
        draw(h1, fileNames[i], "mytree", weight_nom);
        hist->Add(h1);
        delete h1;
    }
    file->cd();
    hist->Write();
    delete hist;
}

void prepare_3D::draw(int c, int s){
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";
    TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom" + other_con1 + other_con2;
    TH3D* hist_up = new TH3D("hist_up", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    TH3D* hist_dn = new TH3D("hist_dn", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    if(c>4 && (sys_n[s].Contains("muR")||sys_n[s].Contains("muF"))){
        hist_up->SetName(process[c]+"_"+sys_n[s]+Form("%dUp_sub",c-4));
        hist_dn->SetName(process[c]+"_"+sys_n[s]+Form("%dDown_sub",c-4));
    }
    else if(sys_n[s].Contains("SF_lepton") && cut_name.Contains("E_")){
        hist_up->SetName(process[c]+"_SF_ElecUp_sub");
        hist_dn->SetName(process[c]+"_SF_ElecDown_sub");
    }
    else if(sys_n[s].Contains("SF_lepton") && cut_name.Contains("M_")){
        hist_up->SetName(process[c]+"_SF_MuonUp_sub");
        hist_dn->SetName(process[c]+"_SF_MuonDown_sub");
    }
    else{
        hist_up->SetName(process[c]+"_"+sys_n[s]+"Up_sub");
        hist_dn->SetName(process[c]+"_"+sys_n[s]+"Down_sub");
    }
    if(c < 5)
        weight = weight + EW[c] + "*nnlo_wt";
    for(int i=edge_i[c]; i<edge_f[c]; i++){
        give_sys_name(fileNames[i], weight, s, c);
        renew_weight(&weight_up, file_up, i);
        renew_weight(&weight_dn, file_dn, i);
        TH3D* h1_up = new TH3D("CG_up", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
        TH3D* h1_dn = new TH3D("CG_dn", "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
        h1_up->Sumw2();
        h1_dn->Sumw2();
        draw(h1_up, file_up, tree_up, weight_up);
        draw(h1_dn, file_dn, tree_dn, weight_dn);
        hist_up->Add(h1_up);
        hist_dn->Add(h1_dn);
        delete h1_up;
        delete h1_dn;
    }
    file->cd();
    hist_up->Write();
    hist_dn->Write();
    delete hist_up;
    delete hist_dn;
}

void prepare_3D::set_dir(){
    dir = Form("../output/%d", year);
    outputDir = Form("../output/%d/datacard", year);
    const int nsample = 20;
    TString fileName[nsample] = {  
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
                            "new_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM.root",
                            };
    Float_t cross_section[nsample]={366.91, 89.05, 377.96,
                                    169.9, 147.4, 41.0, 5.7, 1.4, 0.63, 0.15, 0.0036,
                                    3.36, 136.02, 80.95, 35.6, 35.6,
                                    8927.0, 2809.0, 826.3, 544.3};
    Float_t K_Factor[nsample] = {1.0, 1.0, 1.0,
                                1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,
                                1.0,1.0,1.0,1.0,1.0,
                                1.21,1.21,1.21,1.21};
    TString sys_s[] = {"jer", "unclus", "SF_btag", "SF_btag", "SF_btag", "SF_btag", "SF_lepton", "L1PreFiringWeight_Nom", "pu_wt", "muR", "muF", "ISR", "FSR", "mtop", "hdamp", "TuneCP5", "nnlo_wt", "EW_un"};
    TString sys_ns[] = {"jer", "unclus", "SF_btag", Form("SF_btag%d", year), "SF_ltag", Form("SF_ltag%d", year), "SF_lepton", "L1PF", "PU", "muR", "muF", "ISR", "FSR", "mtop", "hdamp", "TuneCP5", "nnlo_wt", "EW_un"};
    TString sys_up_s[] = {"jerUp", "unclusUp", "SF_btag_up_co", "SF_btag_up", "SF_ltag_up_co", "SF_ltag_up", "SF_lepton_up", "L1PreFiringWeight_Up", "pu_wt_up", "muR_up", "muF_up", "ISR_up", "FSR_up", "mtop173p5", "hdampUP", "TuneCP5up", "nnlo_wt_up", "EW_un_up"};
    TString sys_down_s[] = {"jerDown", "unclusDown", "SF_btag_down_co", "SF_btag_down", "SF_ltag_down_co", "SF_ltag_down", "SF_lepton_down", "L1PreFiringWeight_Dn", "pu_wt_dn", "muR_down", "muF_down", "ISR_down", "FSR_down", "mtop171p5", "hdampDOWN", "TuneCP5down", "nnlo_wt_down", "EW_un_down"};
    
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    TString sf_bl[] = {"SF_btag_co", "SF_ltag_co", "SF_btag_un", "SF_btag_un"};
    for(int i=0; i<nsample; i++){
        fileNames[i] = fileName[i].ReplaceAll(".root","_*.root");
        cross_sections[i] = cross_section[i];
        K_Factors[i] = K_Factor[i];
    }
    //Double_t mtt_edge[9]={0,370,420,500,600,700,800,950,2000};
    //Double_t ytt_edge[10]={-4.0,-1.4,-0.9,-0.5,-0.15,0.15,0.5,0.9,1.4,4.0};
    //Double_t mtt_edge[5]={0, 400, 500, 700, 2000};
    //Double_t ytt_edge[6]={-3.0, -1.2, -0.3, 0.3, 1.2, 3.0};
    //Double_t xbin[2][20] = {{0,340,360,380,400,420,440,460,480,500,520,540,570,600,640,700,3000}, {0,450,500,570,630,700,820,3000}};
    //int nbin[2] = {16, 7};
    TString process_s[]={"ttbar_ci0000","ttbar_ci0100", "ttbar_ci0010", "ttbar_ci0001", "ttbar_ci0200", "DYJets","STop", "WJets", "QCD"};
    TString EWs[5] = {"*weight_ci0000", "*weight_ci0100", "*weight_ci0010", "*weight_ci0001", "*weight_ci0200"};

    int edge_is[] = {0, 0, 0, 0, 0, 3, 11, 16};//23,31}; 
    int edge_fs[] = {3, 3, 3, 3, 3, 11, 16, 20,}; 
    for(int i=0; i<8; i++){
        edge_i[i] = edge_is[i];
        edge_f[i] = edge_fs[i];
    }
    for(int i=0; i<10; i++){
        sys[i] = "jes";
        sys_n[i] = "jes_"+jes_source[i];
        sys_up[i] = "jes_"+jes_source[i]+"Up";
        sys_down[i] = "jes_"+jes_source[i]+"Down";
    }
    for(int i=0; i<18; i++){
        sys[i+10] = sys_s[i];
        sys_n[i+10] = sys_ns[i];
        sys_up[i+10] = sys_up_s[i];
        sys_down[i+10] = sys_down_s[i];
    }
    for(int i=0; i<9; i++){
        process[i] = process_s[i];
    }
    for(int i=0; i<5; i++){
        EW[i] = EWs[i];
    }
}

void prepare_3D::draw_data(){
    //TChain chain_data("mytree");
    //chain_data.Add(dir+"/data/new_data*.root"); //fake data now
    TString data_name="data_obs_sub";
    //auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TH3D* h1 = new TH3D(data_name, "", xbins, xlow, xup, ybins, ylow, yup, zbins, zlow, zup);
    for(int i=0; i<h1->GetNbinsX(); i++){
        for(int j=0; j<h1->GetNbinsY(); j++){
            for(int k=0; k<h1->GetNbinsZ(); k++)
                h1->SetBinContent(i+1, j+1, k+1, 1.0);
        }
    }
    //c0->cd();
    //chain_data.Draw("mass_tt>>data_name", cut);
    file->cd();
    h1->Write();
    delete h1;
    //delete c0;
}

prepare_3D::prepare_3D(TString cut_s, TString cut_name_s, int year_s, int* xyz_bins, double* xyz_range){
    year = year_s;
    set_dir();
    cut = cut_s;
    cut_name = cut_name_s;
    TString category="ttbar_"+cut_name;
    file=new TFile(outputDir+"/"+category+".root","recreate");
    xbins = xyz_bins[0];
    ybins = xyz_bins[1];
    zbins = xyz_bins[2];
    xlow = xyz_range[0];
    xup = xyz_range[1];
    ylow = xyz_range[2];
    yup = xyz_range[3];
    zlow = xyz_range[4];
    zup = xyz_range[5];
    for(int c=0; c<8; c++){
        draw(c);
        cout<<"finished nom of "<<process[c]<<endl;
    }
    //add_qcd();
    for(int s=0; s<28; s++){
        for(int c=0; c<8; c++){
            if(c>4 && s>22)//sys only for signal
                break;
            draw(c, s);
            cout<<"finished sys of "<<sys_n[s]<<" of "<<process[c]<<endl;
        }
    }
    draw_data();
}
prepare_3D::~prepare_3D(){
    file->Close();
}