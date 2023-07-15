#include "reco.h"
Double_t RECO::likelihood(Double_t *pz, double* pars){
    Double_t nu_pz=pz[0];
    int bl = pars[0];
    int bh = pars[1];
    int j1 = pars[2];
    int j2 = pars[3];
    double nu_E=sqrt(nu_px*nu_px+nu_py*nu_py+nu_pz*nu_pz);
    TLorentzVector mom_nu= TLorentzVector(nu_px,nu_py,nu_pz,nu_E);
    Double_t mass_wlep=(mom_nu+mom_lep).M();
    Double_t mass_whad=(mom_jets[j1]+mom_jets[j2]).M();
    Double_t mass_tlep=(mom_nu+mom_lep+mom_jets[bl]).M();
    Double_t mass_thad=(mom_jets[j1]+mom_jets[j2]+mom_jets[bh]).M();
    Double_t pro_wlep=ROOT::Math::gaussian_pdf(mass_wlep,9.5,82.9);
    Double_t pro_tlep=ROOT::Math::gaussian_pdf(mass_tlep,16.3,172.5);
    Double_t pro_whad=ROOT::Math::gaussian_pdf(mass_whad,9.5,82.9);
    Double_t pro_thad=ROOT::Math::gaussian_pdf(mass_thad,16.3,172.5);
    Double_t log_nupz;
    log_nupz=-TMath::Log(pro_tlep)-TMath::Log(pro_wlep)-TMath::Log(pro_thad)-TMath::Log(pro_whad);      
    return log_nupz;
}
/////////////////////////////////////////////////////////////
//define likelihood function with two b-jets
Double_t RECO::likelihood_3jet(Double_t *pz, Double_t* pars){
    Double_t nu_pz=pz[0];
    int bl = pars[0];
    int bh = pars[1];
    int j1 = pars[2];
    double nu_E=sqrt(nu_px*nu_px+nu_py*nu_py+nu_pz*nu_pz);
    TLorentzVector mom_nu= TLorentzVector(nu_px,nu_py,nu_pz,nu_E);
    Double_t mass_wlep=(mom_nu+mom_lep).M();
    Double_t mass_tlep=(mom_nu+mom_lep+mom_jets[bl]).M();
    Double_t mass_thad=(mom_jets[j1]+mom_jets[bh]).M();
    Double_t pro_wlep=ROOT::Math::gaussian_pdf(mass_wlep,9.5,82.9);
    Double_t pro_tlep=ROOT::Math::gaussian_pdf(mass_tlep,16.3,172.5);
    Double_t pro_thad=ROOT::Math::gaussian_pdf(mass_thad,16.3,172.5);
    Double_t log_nupz;
    log_nupz=-TMath::Log(pro_tlep)-TMath::Log(pro_wlep)-TMath::Log(pro_thad);      
    return log_nupz;
}
int* RECO::btag_sort(Float_t* btag_score, int* index){
    for (int i = 0; i < num_jets; i++)
        index[i] = i;
    for (int kk = 0; kk < 2; kk++) {
        int max = kk;
        for (int tt = kk + 1; tt < num_jets; tt++) {
            if (btag_score[index[tt]] > btag_score[index[max]])
                max = tt;
        }
        int tmp = index[max];
        index[max] = index[kk];
        index[kk] = tmp;
    }
    return index;
}
void RECO::chi2_sort(int* index){
    int bjet_lep, bjet_had, min_j1, min_j2;
    bjet_lep = 0, bjet_had = 1, min_j1 = 2, min_j2 = 3;
    Double_t min_chi2, chi2_v;
    Double_t m_bjj, m_jj;
    if(num_jets >= 4){
        for (int bindex = 0; bindex < 2; bindex++) {
            for (int j1 = 2; j1 < num_jets; j1++) {
                for (int j2 = j1 + 1; j2 < num_jets; j2++) {     
                    m_bjj = (mom_jets[index[1-bindex]]+mom_jets[index[j1]]+mom_jets[index[j2]]).M();
                    m_jj = (mom_jets[index[j1]]+mom_jets[index[j2]]).M();
                    chi2_v = ((m_bjj - 172.5)/16.3)*((m_bjj - 172.5)/16.3)+((m_jj-82.9)/9.5)*((m_jj-82.9)/9.5) ;           
                    if (bindex == 0 && j1 == 2 && j2 == 3){
                        min_chi2 = chi2_v;
                    } 
                    else if (chi2_v < min_chi2){
                        min_chi2 = chi2_v;
                        bjet_lep = bindex;
                        bjet_had = 1 - bindex;
                        min_j1 = j1;
                        min_j2 = j2;
                    }          
                }
            }
        }
        chi2_index[0] = index[bjet_lep];
        chi2_index[1] = index[bjet_had];
        chi2_index[2] = index[min_j1];
        chi2_index[3] = index[min_j2];
        chi = min_chi2;   
    }
    else{
        chi = -1;
    }
}
void RECO::like_sort(int* index){
    int bjet_lep, bjet_had, min_j1, min_j2;
    bjet_lep = 0, bjet_had = 1, min_j1 = 2, min_j2 = 3;
    Double_t minimum_likelihood, nupz, minimum;
    Double_t neutrino_pz;
    //for at least 4 jets  
    if(num_jets >= 4){
        for (int bindex = 0; bindex < 2; bindex++) {
            for (int j1 = 2; j1 < num_jets; j1++) {
                for (int j2 = j1 + 1; j2 < num_jets; j2++) {
                    TF1 *likelihood_fun =new TF1("likelihood_fun", RECO::likelihood, -1000.0, 1000.0, 4);
                    likelihood_fun->SetParameters(index[bindex], index[1-bindex], index[j1], index[j2]); // pass the index of j1 and j2 as parameters to a
                    minimum_likelihood = likelihood_fun->GetMinimum(-1000.0, 1000.0);
                    nupz = likelihood_fun->GetMinimumX(-1000.0, 1000.0);
                    if (bindex == 0 && j1 == 2 && j2 == 3){
                        minimum = minimum_likelihood;
                        neutrino_pz = nupz;
                    } 
                    else if (minimum_likelihood < minimum){
                        minimum = minimum_likelihood;
                        neutrino_pz = nupz;
                        bjet_lep = bindex;
                        bjet_had = 1 - bindex;
                        min_j1 = j1;
                        min_j2 = j2;
                    }          
                }
            }
        }
        jets_index[0]=index[bjet_lep];
        jets_index[1]=index[bjet_had];
        jets_index[2]=index[min_j1];
        jets_index[3]=index[min_j2];
        nu_pz = neutrino_pz;
        like = minimum;
    }
    //for exactly 3 jets
    else if (num_jets==3){
        for (int bindex = 0; bindex < 2; bindex++) {
            TF1 *likelihood_fun=new TF1("likelihood_fun", RECO::likelihood_3jet, -1000.0, 1000.0, 3);
            likelihood_fun->SetParameters(index[bindex], index[1-bindex], index[2]);
            minimum_likelihood = likelihood_fun->GetMinimum(-1000.0, 1000.0);
            nupz = likelihood_fun->GetMinimumX(-1000.0, 1000.0);
            if (bindex == 0 ) {
                minimum = minimum_likelihood;
                neutrino_pz = nupz;
            } 
            else if (minimum_likelihood < minimum) {
                minimum = minimum_likelihood;
                neutrino_pz = nupz;
                bjet_lep = bindex;
                bjet_had = 1 - bindex;
            }          
        }
        jets_index[0]=index[bjet_lep];
        jets_index[1]=index[bjet_had];
        jets_index[2]=index[2];
        nu_pz = neutrino_pz;
        like = minimum + 4.0;
    }
}
void RECO::reco_top(){
    Float_t nu_E =sqrt(nu_px * nu_px + nu_py * nu_py + nu_pz * nu_pz);
    TLorentzVector mom_nu = TLorentzVector(nu_px, nu_py, nu_pz, nu_E);
    TLorentzVector mom_top, mom_antitop;
    if(num_jets>=4){
        mass_tlep = (mom_nu+mom_lep+mom_jets[jets_index[0]]).M();
        mass_wlep = (mom_nu+mom_lep).M();
        mass_thad =(mom_jets[jets_index[2]] + mom_jets[jets_index[3]]+mom_jets[jets_index[1]]).M();
        mass_whad = (mom_jets[jets_index[2]] + mom_jets[jets_index[3]]).M();
        if(lep_charge > 0) {
            mom_top = mom_nu + mom_lep + mom_jets[jets_index[0]];
            mom_antitop = mom_jets[jets_index[2]]+mom_jets[jets_index[3]]+mom_jets[jets_index[1]];
        } 
        else {
            mom_top = mom_jets[jets_index[2]]+mom_jets[jets_index[3]]+mom_jets[jets_index[1]];
            mom_antitop = mom_nu + mom_lep + mom_jets[jets_index[0]];
        }
        //cout<<mom_jets[jets_index[0]].Pt()<<" "<<mom_jets[jets_index[1]].Pt()<<" "<<mom_jets[jets_index[2]].Pt()<<" "<<mom_jets[jets_index[3]].Pt()<<endl;
    }
    else{
        mass_tlep =(mom_nu+mom_lep+mom_jets[jets_index[0]]).M();
        mass_wlep = (mom_nu+mom_lep).M();
        mass_thad =(mom_jets[jets_index[2]] + mom_jets[jets_index[1]]).M();
        mass_whad = (mom_jets[jets_index[2]]).M();
        if(lep_charge > 0) {
            mom_top = mom_nu + mom_lep + mom_jets[jets_index[0]];
            mom_antitop = mom_jets[jets_index[2]] + mom_jets[jets_index[1]];
        } 
        else {
            mom_top = mom_jets[jets_index[2]] + mom_jets[jets_index[1]];
            mom_antitop = mom_nu + mom_lep + mom_jets[jets_index[0]];
        }
    }
    rectop_pt = mom_top.Pt();
    recantitop_pt = mom_antitop.Pt();
    rapidity_tt = mom_top.Rapidity() - mom_antitop.Rapidity();
    mass_tt = (mom_antitop + mom_top).M();
    mass_t = mom_top.M();
    mass_at = mom_antitop.M();
    //cout<<lep_charge<<" "<<mom_top.Pt()<<" "<<mom_antitop.Pt()<<endl;
}
void RECO::reco_chi(){
    if(num_jets>=4){
        mass_bjj = (mom_jets[chi2_index[1]]+mom_jets[chi2_index[2]]+mom_jets[chi2_index[3]]).M();
        mass_jj = (mom_jets[chi2_index[2]]+mom_jets[chi2_index[3]]).M();
        mass_lb = (mom_jets[chi2_index[0]]+mom_lep).M();
    }
    else{
        mass_bjj = -1;
        mass_jj = -1;
        mass_lb = -1;
    }
}
bool RECO::diff(){
    if(num_jets==3)
        return true;
    else{
        /*for(int i=0; i<4; i++)
            cout<<jets_index[i]<<" "<<chi2_index[i]<<" ";
        cout<<endl;*/
        for(int i=0; i<4; i++){
            if (jets_index[i] != chi2_index[i])
                return false;
        }
        return true;
    }
}

RECO::RECO(int jet_num, TLorentzVector* mom_alljets, TLorentzVector mom_lepton, int lep_c, Float_t* btag_score, Float_t MET_pt, Float_t MET_phi){
    num_jets = jet_num;
    lep_charge = lep_c;
    mom_lep = mom_lepton;
    mom_jets = new TLorentzVector[jet_num];
    for(int i = 0; i < jet_num; i++)
        mom_jets[i] = mom_alljets[i];
    nu_px = MET_pt*cos(MET_phi);
    nu_py = MET_pt*sin(MET_phi);
    int index[jet_num];
    btag_sort(btag_score, index);
    chi2_sort(index);
    reco_chi();
    like_sort(index);
    reco_top();
}
RECO::~RECO(){
    delete[] mom_jets;
}