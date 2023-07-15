#include "select_tree.cpp"
void process(TString outdir, TString outputFile, TString input, int year, int type){
    TString tree_name[] = {"mytree", "mytree", "jesUp", "jesDown", "jerUp", "jerDown", "unclusUp", "unclusDown"};
    TString jet_name[] = {"Jet_pt", "Jet_pt_nom", "Jet_pt_jesTotalUp", "Jet_pt_jesTotalDown", "Jet_pt_jerUp", "Jet_pt_jerDown", "Jet_pt_nom", "Jet_pt_nom"};
    TString MET_name[] = {"MET_pt", "MET_T1Smear_pt", "MET_T1Smear_pt_jesTotalUp", "MET_T1Smear_pt_jesTotalDown", "MET_T1Smear_pt_jerUp", "MET_T1Smear_pt_jerDown", "MET_T1Smear_pt_unclustEnUp", "MET_T1Smear_pt_unclustEnDown"};
    TString jes_source[] = {"Absolute", Form("Absolute_%d", year), "FlavorQCD", "BBEC1", "EC2", "HF", Form("BBEC1_%d", year), Form("EC2_%d", year), "RelativeBal", Form("RelativeSample_%d", year)};
    int num_j, num_e, num_m, num_g;
    read_object r(input, type);
    select_tree *s, *s1;
    num_j = r.nj;
    num_e = r.ne;
    num_m = r.nm;
    num_g = r.ng;
    /*num_j = 50;
    num_e = 20;
    num_m = 20;
    num_g = 50;*/
    if(type == 0){
        s = new select_tree(input, outdir+"/"+"new_"+outputFile, tree_name[0], jet_name[0], MET_name[0], year, 0, num_j, num_e, num_m);
        s->write();
        delete s;
    }
    else if(type == 1){
        s = new select_tree(input, outdir+"/"+"new_"+outputFile, tree_name[0], jet_name[0], MET_name[0], year, 1, num_j, num_e, num_m, num_g);
        s->write();
        delete s;
        for(int sys=2; sys<1; sys++){
            s1 = new select_tree(input, outdir+"/"+"new_"+outputFile, tree_name[sys], jet_name[sys], MET_name[sys], year, 2, num_j, num_e, num_m);
            s1->write();
            delete s1;
        }
    }
    else if(type == 2){
        s = new select_tree(input, outdir+"/"+"new_"+outputFile, tree_name[1], jet_name[1], MET_name[1], year, 3, num_j, num_e, num_m);
        s->write();
        delete s;
    }
    else if(type == 3){
        for(int sour=0; sour<10; sour++){
            s = new select_tree(input, outdir+"/"+"new_"+outputFile, tree_name[1], jet_name[1], MET_name[1], year, 3, num_j, num_e, num_m);
            s->write();
            delete s;
        }
    }
}

