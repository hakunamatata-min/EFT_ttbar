#include"prepare.cpp"
void prepare_datacard(int i, int j, int year){
    TString cuts[] = {"(jet_num == 3 && likelihood<20.0 && (!lep_flavour))","(jet_num >= 4  && (!lep_flavour))",
        "(jet_num == 3 && likelihood<20.0 && lep_flavour)",  "(jet_num >= 4 && lep_flavour)"};
    TString cutsName[] = {"E_3jets", "E_4jets", "M_3jets", "M_4jets"};
    TString div_name[4] = {"_y0", "_y1", "_y2", "_y3"};
    TString div_con[4] = {"*(fabs(rapidity_tt)<0.4)", "*(fabs(rapidity_tt)<1.0 && fabs(rapidity_tt)>0.4)",
                            "*(fabs(rapidity_tt)<2.0 && fabs(rapidity_tt)>1.0)", "*(fabs(rapidity_tt)>2.0)"};
    int nbins[] = {9, 11, 10, 11};
    double xbins[][20] = {{0,300,340,380,420,460,500,600,800,3000}, {0,300,350,400,450,500,550,600,700,800,1000,3000}, 
                        {0,400,450,500,550,600,650,700,800,1000,3000}, {0,450,550,650,700,750,800,900,1000,1200,1400,3000}};

    cout<<"begin "<<cutsName[i]<<":"<<endl;
    prepare* p = new prepare(cuts[i]+div_con[j], cutsName[i]+div_name[j], year, xbins[j], nbins[j]);
    delete p;

}