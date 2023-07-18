#include"prepare.cpp"
void prepare_datacard(int i, int j, int year){
    TString cuts[] = {"(jet_num == 3 && likelihood<20.0 && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3 && likelihood<20.0 && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString div_name[2] = {"_y0", "_y1"};
    TString div_con[4] = {"*(fabs(rapidity_tt)<1.4)", "*(fabs(rapidity_tt)>1.4)",};
    int nbins[] = {16, 7};
    double xbins[][20] = {{300,340,360,380,400,420,440,460,480,500,520,540,570,600,640,700,3000}, 
                          {300,450,500,570,600,700,820,3000}};

    cout<<"begin "<<cutsName[i]<<":"<<endl;
    prepare* p = new prepare(cuts[i]+div_con[j], cutsName[i]+div_name[j], year, xbins[j], nbins[j]);
    delete p;
}