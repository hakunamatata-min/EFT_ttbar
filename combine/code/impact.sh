#!/bin/bash
now=$PWD
output=impact_z_3jets
cd /afs/cern.ch/user/y/yuekai/tth_cms/CMSSW_11_3_4/src/
eval `scramv1 runtime -sh`
cd /afs/cern.ch/user/m/mitang/EFT_ttbar/combine

#output=$now/impact_z/$1

cd $1/original

rm -f ttbar_semi.txt
combineCards.py ttbar_M_3jets_2018.txt > ttbar_semi.txt

text2workspace.py ttbar_semi.txt -o workspace_ttbar.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc:stagex_ttwc -m 125 --X-allow-no-background  -v 7 > out 

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --doInitialFit -v 7  --redefineSignalPOIs z --freezeParameters y,k &> initial.txt

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --doFits -v 7  --redefineSignalPOIs z --freezeParameters y,k  &> dofit.txt

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --redefineSignalPOIs z --exclude y,k --output impacts.json &> pjson.txt

plotImpacts.py -i impacts.json -o impacts #--P z

rm -rf ../$output
mkdir ../$output
mv ttbar_semi.txt ../$output
mv combine* ../$output
mv workspace* ../$output
mv *Fit* ../$output
mv initial.txt ../$output
mv dofit.txt ../$output
mv pjson.txt ../$output
mv impacts* ../$output
mv out ../$output
cd $now

