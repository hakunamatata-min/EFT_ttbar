#include "prepare/prepare.cpp"
void derive_sys(int i, int year, int var){
    TString cuts[] = {"(jet_num == 3  && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3  && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString title[] = {"likelihood","mass_t","mass_wlep","mass_whad","lepton_pt","leading_pt","jet_num","top_pt","Mtt","deltay"};
    TString xvars[] = {"likelihood","mass_t","mass_wlep","mass_whad","lepton_pt","jet_pt[0]","jet_num","rectop_pt","mass_tt","rapidity_tt"};
    Double_t xup[] = {50, 450, 140, 140, 250, 400, 7, 500, 1500, 3};
    Double_t xdown[] = {13, 50, 50, 50, 30, 30, 3, 50, 300, -3};
    Int_t bins[]={37, 40, 36, 36, 22, 20, 4, 20, 24, 24};
    
    TString dir = Form("./output/%d", year);
    TString outputDir = Form("./sys_root/%d", year);
    prepare p(dir, outputDir);
    p.set_bins(xvars[var], title[var], bins[var], xdown[var], xup[var]);
    p.run(cuts[i], cutsName[i], year, 1);
}