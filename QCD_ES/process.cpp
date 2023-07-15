#include "select_tree.cpp"
void process(TString outdir, TString outputFile, TString input, int year, int cate, Bool_t is_MC){
    int num_j, num_e, num_m, num_g;
    int em;
    //read_object r(input, is_MC);
    select_tree *s, *s1, *s2;
    /*num_j = r.nj;
    num_e = r.ne;
    num_m = r.nm;
    num_g = r.ng;*/
    num_j = 50;
    num_e = 20;
    num_m = 20;
    num_g = 50;
    em = 0;
    if(outputFile.Contains("EMEnriched") || outputFile.Contains("EGamma"))
        em = 1;
    else if(outputFile.Contains("MuEnrichedPt5") || outputFile.Contains("SingleMuon"))
        em = 2;
    if(!is_MC){
        s = new select_tree(input, outdir+"/"+"new_"+outputFile, year, cate, true, em, num_j, num_e, num_m);
        s->write();
        delete s;
    }
    else{
        s = new select_tree(input, outdir+"/"+"new_"+outputFile, year, cate, false, em, num_j, num_e, num_m, num_g);
        s->write();
        delete s;
    }
}

