#include "prepare.h"


void prepare::Floor(TH1D* hist){
    for (int i=0;i<hist->GetNbinsX();i++){
        if(!(hist->GetBinContent(i+1)>1.E-6)){
            hist->SetBinContent(i+1, 1.E-6);
        }
    }
}
/*void sum_h1(TH1D* hs, TH1D* h1, TH1D *h2){
    for(int i=0; i<h1->GetNbinsX(); i++){
        hs->SetBinContent(i+1, h1->GetBinContent(i+1));
    }
    for(int i=0; i<h2->GetNbinsX(); i++){
        hs->SetBinContent(h1->GetNbinsX()+i+1, h2->GetBinContent(i+1));
    }
}*/
void prepare::give_sys_name(TString file, TString weight, int s, int c){
    
    if(s<3){
        tree_up = sys_up[s];
        tree_dn = sys_down[s];
        file_up = file;
        file_dn = file;
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<8 || s==15 ){
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_dn = file;
        weight_up = weight;
        weight_up.ReplaceAll(sys[s],sys_up[s]);
        weight_dn = weight;
        weight_dn.ReplaceAll(sys[s],sys_down[s]);
    }
    else if(s<12){
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_dn = file;
        weight_up = weight + "*" + sys_up[s];
        weight_dn = weight + "*" + sys_down[s];
    }
    else if(s<14){
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_up.ReplaceAll("TuneCP5",sys_up[s]+"_TuneCP5");
        file_dn = file;
        file_dn.ReplaceAll("TuneCP5",sys_down[s]+"_TuneCP5");
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<16){
        tree_up = "mytree";
        tree_dn = "mytree";
        file_up = file;
        file_up.ReplaceAll("TuneCP5",sys_up[s]);
        file_dn = file;
        file_dn.ReplaceAll("TuneCP5",sys_down[s]);
        weight_up = weight;
        weight_dn = weight;
    }
    else if(s<17){
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

void prepare::renew_weight(TString* weight, TString file, int f){
    double lumi_s[4]={19.5, 16.8, 41.48, 59.83};
    double lumi = lumi_s[year-2015];
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TChain* tree1=new TChain("rawtree"); 
    tree1->Add(dir+"/MC/"+file);
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
void prepare::draw(TH1D* h1, TString file, TString tree, TString weight){
    auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TChain chain(tree);
    chain.Add(dir+"/MC/"+file);
    TH1D *hm1;
    hm1 = new TH1D("hm1", "", nbins, xbins);
    hm1->Sumw2();
    c0->cd();
    chain.Draw("mass_tt>>hm1", weight+"*"+cut);
    for(int i=0; i<nbins; i++){
        h1->SetBinContent(i+1, hm1->GetBinContent(i+1));
        h1->SetBinError(i+1, hm1->GetBinError(i+1));
    }
    c0->cd();
    Floor(h1);
    delete hm1;
    delete c0;
}
void prepare::draw(int c){
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";
    TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom" + other_con1 + other_con2;
    TH1D* hist;
    if(c < 5)
        weight = weight + EW[c] + "*nnlo_wt";
    for(int i=edge_i[c]; i<edge_f[c]; i++){
        weight_nom = weight;
        renew_weight(&weight_nom, fileNames[i], i);
        TH1D* h1 = new TH1D("CG", "", nbins, 0, nbins);
        //h1->Sumw2();
        draw(h1, fileNames[i], "mytree", weight_nom);
        if(i == edge_i[c]){
            hist = (TH1D*)h1->Clone();
            //hist->Sumw2();
            hist->SetName(process[c]+"_sub");
        }
        else{
            hist->Add(h1);
        }
        delete h1;
    }
    file->cd();
    hist->Write();
    delete hist;
}

void prepare::draw(int c, int s){
    TString other_con1 = "*((jet_num>=4)||(jet_num==3 && jet_pt[0]>50))";
    TString other_con2;
    if(year==2018)
        other_con2 = "*(lep_flavour||((!lep_flavour) && lepton_pt>34))";
    else
        other_con2 = "*1";
    TString weight = "Generator_weight*SF_btag*SF_lepton*pu_wt*L1PreFiringWeight_Nom" + other_con1 + other_con2;
    TH1D *hist_up, *hist_dn;
    if(c < 5)
        weight = weight + EW[c] + "*nnlo_wt";
    for(int i=edge_i[c]; i<edge_f[c]; i++){
        give_sys_name(fileNames[i], weight, s, c);
        renew_weight(&weight_up, file_up, i);
        renew_weight(&weight_dn, file_dn, i);
        TH1D* h1_up = new TH1D("CG_up", "", nbins, 0, nbins);
        TH1D* h1_dn = new TH1D("CG_dn", "", nbins, 0, nbins);
        //h1_up->Sumw2();
        //h1_dn->Sumw2();
        draw(h1_up, file_up, tree_up, weight_up);
        draw(h1_dn, file_dn, tree_dn, weight_dn);
        if(i == edge_i[c]){
            hist_up = (TH1D*)h1_up->Clone();
            hist_dn = (TH1D*)h1_dn->Clone();
            if(sys_n[s].Contains("SF_btag_unco")){
                hist_up->SetName(process[c]+Form("_SF_btag%dUp_sub",year));
                hist_dn->SetName(process[c]+Form("_SF_btag%dDown_sub",year));
            }
            else if(c>4 && (sys_n[s].Contains("muR")||sys_n[s].Contains("muF"))){
                hist_up->SetName(process[c]+"_"+sys_n[s]+Form("%dUp_sub",c-4));
                hist_dn->SetName(process[c]+"_"+sys_n[s]+Form("%dDown_sub",c-4));
            }
            else{
                hist_up->SetName(process[c]+"_"+sys_n[s]+"Up_sub");
                hist_dn->SetName(process[c]+"_"+sys_n[s]+"Down_sub");
            }
        }
        else{
            hist_up->Add(h1_up);
            hist_dn->Add(h1_dn);
        }
        delete h1_up;
        delete h1_dn;
    }
    file->cd();
    hist_up->Write();
    hist_dn->Write();
    delete hist_up;
    delete hist_dn;
}
/*void prepare::add_pdf_weight(){
    TFile* pdf_file = TFile::Open(pdf_dir+"/pdfw_"+cut_name+".root");
    TH1D *hist_up, *hist_dn;
    for(int c=0; c<7; c++){
        hist_up = (TH1D*)pdf_file->Get(process[c]+"_pdfUp");
        hist_dn = (TH1D*)pdf_file->Get(process[c]+"_pdfDown");
        file->cd();
        hist_up->Write();
        hist_dn->Write();
        delete hist_up;
        delete hist_dn;
    }
    std::vector<TString> sys_num;
    sysNames.push_back("pdf");
    for(int c=0; c<7; c++)
        sys_num.push_back("1");
    sys_num.push_back("-");
    sysnum.push_back(sys_num);
}*/
void prepare::set_dir(){
    dir = Form("/home/yksong/code/EFT-ttbar/select_analysis/output/%d", year);
    outputDir = "/home/yksong/code/EFT-ttbar/test/ttX/datacard/divided";
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
    TString sys_s[] = {"jes","jer","unclus","SF_lepton","SF_btag","SF_btag","L1PreFiringWeight_Nom","pu_wt","muR","muF","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un"};
    TString sys_ns[] = {"jes","jer","unclus","SF_lepton","SF_btag_unco","SF_btag_co","L1PF","PU","muR","muF","ISR","FSR","mtop","hdamp","TuneCP5","nnlo_wt","EW_un"};
    TString sys_up_s[] = {"jesUp","jerUp","unclusUp","SF_lepton_up","SF_btag_up","SF_btag_up_co","L1PreFiringWeight_Up","pu_wt_up","muR_up","muF_up","ISR_up","FSR_up","mtop173p5","hdampUP","TuneCP5up","nnlo_wt_up","EW_un_up"};
    TString sys_down_s[] = {"jesDown","jerDown","unclusDown","SF_lepton_down","SF_btag_down","SF_btag_down_co","L1PreFiringWeight_Dn","pu_wt_dn","muR_down","muF_down","ISR_down","FSR_down","mtop171p5","hdampDOWN","TuneCP5down","nnlo_wt_down","EW_un_down"};
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
    for(int i=0; i<17; i++){
        sys[i] = sys_s[i];
        sys_n[i] = sys_ns[i];
        sys_up[i] = sys_up_s[i];
        sys_down[i] = sys_down_s[i];
    }
    for(int i=0; i<9; i++){
        process[i] = process_s[i];
    }
    for(int i=0; i<5; i++){
        EW[i] = EWs[i];
    }
}

void prepare::draw_data(){
    //TChain chain_data("mytree");
    //chain_data.Add(dir+"/data/new_data*.root"); //fake data now
    TString data_name="data_obs_sub";
    //auto c0 = new TCanvas("c0", "c0", 8, 30, 600, 600); // temporary canvas
    TH1D* h1 = new TH1D(data_name, "", nbins, 0 ,nbins);
    for(int i=0; i<h1->GetNbinsX(); i++){
        h1->SetBinContent(i+1, 1);
    }
    //c0->cd();
    //chain_data.Draw("mass_tt>>data_name", cut);
    file->cd();
    h1->Write();
    delete h1;
    //delete c0;
}
/*void prepare::add_qcd(){
    TFile* qcd_file = TFile::Open(qcd_dir + "/QCD_"+cut_name+".root");
    TH1D* hist = (TH1D*)qcd_file->Get("QCD_sub");
    TH1D* hist_up = (TH1D*)qcd_file->Get("QCD_qcdsUp_sub");
    TH1D* hist_dn = (TH1D*)qcd_file->Get("QCD_qcdsDown_sub");
    file->cd();
    hist->Write();
    hist_up->Write();
    hist_dn->Write();
    delete hist;
}*/


prepare::prepare(TString cut_s, TString cut_name_s, int year_s, double *xbin, int nbin){
    year = year_s;
    set_dir();
    cut = cut_s;
    cut_name = cut_name_s;
    TString category="ttbar_"+cut_name;
    file=new TFile(outputDir+"/"+category+Form("_%d.root", year), "recreate");
    nbins = nbin;
    for(int i=0; i<nbin+1; i++)
        xbins[i] = xbin[i];
    for(int c=0; c<8; c++){
        draw(c);
        cout<<"finished nom of "<<process[c]<<endl;
    }
    //add_qcd();
    for(int s=0; s<17; s++){
        for(int c=0; c<8; c++){
            if(c>4 && s>11 )
                break;
            draw(c, s);
            cout<<"finished sys of "<<sys_n[s]<<" of "<<process[c]<<endl;
        }
    }
    draw_data();
}
prepare::~prepare(){
    file->Close();
}