#include "SF_add_it_tree.cpp"
void SF_add_it(TString inputFile, bool is_sys, int year){
    TString trees[]={"jesUp", "jesDown", "jerUp", "jerDown", "unclusUp", "unclusDown"};
    SF_add_it_tree *s;
    if(!is_sys){
        s = new SF_add_it_tree(inputFile, "mytree", true, year);
        delete s;
        for(int i=0; i<6; i++){
            s = new SF_add_it_tree(inputFile, trees[i], false, year);
            delete s;
        }
    }
    else{
        s = new SF_add_it_tree(inputFile, "mytree", false, year);
        delete s;
    }
}