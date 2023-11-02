#!/bin/bash
output=output_fcp
now=$PWD
cd /afs/cern.ch/user/y/yuekai/tth_cms/CMSSW_11_3_4/src
eval `scramv1 runtime -sh`
cd $now
cd datacard/

rm -rf ttbar_semi.txt
combineCards.py ttbar_*_*_*.txt > ttbar_semi.txt
text2workspace.py ttbar_semi.txt -o workspace_ttbar_semi.root --PO doStage0 --PO doacttbar --PO dofcp -P HiggsAnalysis.CombinedLimit.stagex_ttwc:stagex_ttwc -m 125 --X-allow-no-background  -v 7 
combine -M MultiDimFit workspace_ttbar_semi.root  -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P fcp --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_fcp -v 3 -m 125 --setParameterRanges fcp=-1.0,1.0:y=0.0,0.0 --saveInactivePOI=1

#rename y.root y_S0.root *y.root
#rename z.root z_S0.root *z.root
#rename k.root k_S0.root *k.root

rm -rf ../$output
mkdir ../$output
mv workspace* ../$output
mv higgsCombine* ../$output
cd ../$output
rename higgsCombine limit_ *.root
rename MultiDimFit.mH125. '' *.root
cd ../
