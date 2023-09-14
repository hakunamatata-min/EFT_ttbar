import math
import numpy as np
import statsmodels.api as sm
import ROOT

def copy(h0: ROOT.TH1D, h1: ROOT.TH1D, s: int) -> None:
    for i in range(0, h0.GetNbinsX()):
        h0.SetBinContent(i+1, h1.GetBinContent(s+i+1))
def set_val(h1: ROOT.TH1D, val: float)-> None:
    for i in range(0, h1.GetNbinsX()):
        h1.SetBinContent(i+1, val)
def sym(hup: ROOT.TH1D, hdn: ROOT.TH1D) -> None:
    for i in range(0, hup.GetNbinsX()):
        val = max(math.fabs(hup.GetBinContent(i+1)-1), math.fabs(hdn.GetBinContent(i+1)-1))
        hup.SetBinContent(i+1, val)
        hdn.SetBinContent(i+1, 2-val)
def lowess(hist: ROOT.TH1D, frac: float) -> None:
    x_values = list()
    y_values = list()
    for i in range(0, hist.GetNbinsX()):
        x_values.append(i)
        y_values.append(hist.GetBinContent(i+1))
    x = np.array(x_values)
    y = np.array(y_values)
    yest = sm.nonparametric.lowess(y, x, frac)[:,1]
    for i in range(0, hist.GetNbinsX()):
        hist.SetBinContent(i+1, yest[i])
def smooth_sys(hist_up: ROOT.TH1D, hist_dn: ROOT.TH1D, hist_nom: ROOT.TH1D, start:list, option:list) -> None:
    norm_up = hist_up.GetSumOfWeights()
    norm_dn = hist_dn.GetSumOfWeights()
    norm_nom = hist_nom.GetSumOfWeights()
    hd_up = list()
    hd_dn = list()
    hd_nom = list()
    for f in range(0, len(start) - 1):
        hd_up.append(ROOT.TH1D("hd_up_{0}".format(f), "", start[f+1]-start[f], 0, start[f+1]-start[f]))
        hd_dn.append(ROOT.TH1D("hd_dn_{0}".format(f), "", start[f+1]-start[f], 0, start[f+1]-start[f]))
        hd_nom.append(ROOT.TH1D("hd_nom_{0}".format(f), "", start[f+1]-start[f], 0, start[f+1]-start[f]))
        copy(hd_up[f], hist_up, start[f])
        copy(hd_dn[f], hist_dn, start[f])
        copy(hd_nom[f], hist_nom, start[f])
        hd_up[f].Divide(hd_nom[f])
        hd_dn[f].Divide(hd_nom[f])
    if(option[0] == 1):  
        if(len(option[2]) == len(start) - 1):
            for f in range(0, len(start) - 1):
                lowess(hd_up[f], option[2][f])
                lowess(hd_dn[f], option[2][f])
        else:
            for f in range(0, len(start) - 1):
                lowess(hd_up[f], option[2][0])
                lowess(hd_dn[f], option[2][0])
    elif(option[0] == 2):
        for f in range(0, len(start) - 1):
            set_val(hd_up[f], norm_up/norm_nom)
            set_val(hd_dn[f], norm_dn/norm_nom)
    if(option[1] == 1):
        for f in range(0, len(start) - 1):
            sym(hd_up[f], hd_dn[f])
    for f in range(0, len(start) - 1):
        for i in range(0, start[f+1]-start[f]):
            #print(i+1+start[f], hd_up[f].GetBinContent(i+1), hd_nom[f].GetBinContent(i+1)*hd_up[f].GetBinContent(i+1))
            hist_up.SetBinContent(i+1+start[f], hd_nom[f].GetBinContent(i+1)*hd_up[f].GetBinContent(i+1))
            hist_dn.SetBinContent(i+1+start[f], hd_nom[f].GetBinContent(i+1)*hd_dn[f].GetBinContent(i+1))
    hist_up.ResetStats()
    hist_dn.ResetStats()

def get_sys_name(h1_sys_name: str) -> None:
    pos = 0
    while(h1_sys_name[pos] != '_'):
        pos = pos + 1
    if("ttbar" in h1_sys_name):
        pos = pos + 1
        while(h1_sys_name[pos] != '_'):
            pos = pos + 1
    return h1_sys_name[pos+1: ]

def smooth(file_name: str, sys_type: dict) -> None:
    file = ROOT.TFile(file_name, "update")
    hist_map = dict()
    sys_contained = set()
    if("ttx" in file_name):
        start = [0, 16, 23]
    else:
        start = [0, 9, 20, 30, 41]
    for key in file.GetListOfKeys():
        hist = key.ReadObj()
        hist_name = hist.GetName()
        hist_map[hist_name] = hist.Clone()
        hist_map[hist_name].SetName(hist_name.replace("Ori", ""))
        if("OriUp" in hist_name or "OriDown" in hist_name):
            hist_name = hist_name.replace("OriUp", "")
            hist_name = hist_name.replace("OriDown", "")
            sys_contained.add(str(hist_name))

    for sys in sys_contained:
        sys_name = get_sys_name(sys)
        nom_name = sys.replace("_"+sys_name, "")
        smooth_sys(hist_map[sys+"OriUp"], hist_map[sys+"OriDown"], hist_map[nom_name], start, sys_type[sys_name])
        file.cd()
        hist_map[sys+"OriUp"].Write()
        hist_map[sys+"OriDown"].Write()
    file.Close()

cut_names = ["_E_3jets", "_M_3jets", "_E_4jets", "_M_4jets"]
years = [2015, 2016, 2017, 2018]
datacard_names = ["./datacard/", "./datacard_ttx/"]
for year in years:
    sys_type = {"jes_Absolute": [0, 0], "jes_Absolute_{0}".format(year): [0, 0], "jes_FlavorQCD": [0, 0], "jes_BBEC1": [0, 0], 
                "jes_EC2": [0, 0], "jes_HF": [0, 0], "jes_BBEC1_{0}".format(year): [0, 0], "jes_EC2_{0}".format(year): [0, 0], 
                "jes_RelativeBal": [0, 0], "jes_RelativeSample_{0}".format(year): [0, 0], "jer": [0, 0], "unclus": [0, 0], 
                "SF_lepton": [0, 0], "SF_btag": [0, 0], "SF_btag{0}".format(year): [0, 0], "SF_ltag": [0, 0], "SF_ltag{0}".format(year): [0, 0], 
                "pdf": [0, 0], "alphas": [0, 0], "L1PF": [0, 0], "PU": [0, 0], "muR1": [0, 0],
                "muF1": [0, 0], "muR2": [0, 0], "muF2": [0, 0], "muR3": [0, 0], "muF3": [0, 0],
                "ISR": [0, 0], "FSR": [0, 0], "mtop": [0, 0], "hdamp": [0, 0], "TuneCP5": [2, 0],
                "nnlo_wt": [0, 0], "EW_un": [0, 0], "qcds": [2, 0]}
    for cut_name in cut_names:
        for datacard_name in datacard_names:
            file_name = datacard_name+"ttbar"+cut_name+"_{0}.root".format(year)
            smooth(file_name, sys_type)