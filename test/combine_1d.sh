#!/bin/bash
now=$PWD
output=output_1d
cd /afs/cern.ch/user/m/mitang/CMSSW_11_3_4/src
eval `scramv1 runtime -sh`
cd /afs/cern.ch/user/m/mitang/EFT_ttbar/combine


cd datacard/original

rm -rf ttbar_4jets.txt
cp ttbar_M_4jets_2018.txt ttbar_4jets.txt
text2workspace.py ttbar_4jets.txt -o workspace_ttbar_4jets.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc3_sym:stagex_ttwc3_sym -m 125 --X-allow-no-background  -v 7 


rm -rf ../$output
mkdir ../$output
mv workspace* ../$output
mv higgsCombine* ../$outputvi 
cd ../$output
rename higgsCombine limit_ *.root
rename MultiDimFit.mH125. '' *.root
cd ../..
