#include<TROOT.h>
#include<Math/PdfFuncMathCore.h>
#include<TFile.h>
#include<TChain.h>
#include<TLorentzVector.h>
#include<TMath.h>
#include<TF1.h>
#include<TH1F.h>
#include<TH2F.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include"json/json.h"
#include"lib_json/json_reader.cpp"
#include"lib_json/json_value.cpp"
#include"lib_json/json_writer.cpp"
using namespace std;
map<UInt_t, std::vector<UInt_t>> read_gJSON(int year){
    Json::Reader reader;
    Json::Value valroot;
    Json::Value::Members members;
    ifstream file;
    file.open(Form("./GoldenJSON_Files/%d_goldenJSON.txt", year));
    map<UInt_t, std::vector<UInt_t>> event_map;
    if(!file.is_open()){
        cout<<"open failed!"<<endl;
        return event_map;
    }
    if(!(reader.parse(file, valroot, false))){
        cout<<"parse failed!"<<endl;
        return event_map;
    }
    members = valroot.getMemberNames();
    for(Json::Value::Members::iterator it=members.begin(); it!=members.end(); it++){
        std::string run = *it;
        for(unsigned i=0; i<valroot[run.c_str()].size(); i++){
            //cout<<valroot[run.c_str()][i][0u].asUInt()<<" "<<valroot[run.c_str()][i][1u].asUInt()<<endl;
            event_map[UInt_t(std::stoi(run))].push_back(valroot[run.c_str()][i][0u].asUInt());
            event_map[UInt_t(std::stoi(run))].push_back(valroot[run.c_str()][i][1u].asUInt());
        }
    }
    return event_map;
}