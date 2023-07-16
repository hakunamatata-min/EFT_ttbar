#!/bin/bash
now=$PWD
cd /afs/cern.ch/user/y/yuekai/tth_cms/CMSSW_8_1_0/src
eval `scramv1 runtime -sh`
cd $now
cd datacard/no

rm -rf ttbar_3jets.txt
combineCards.py ttbar_*_3jets_*.txt > ttbar_3jets.txt
text2workspace.py ttbar_3jets.txt -o workspace_ttbar_3jets.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc3_sym:stagex_ttwc3_sym -m 125 --X-allow-no-background  -v 7 
combine -M MultiDimFit workspace_ttbar_3jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P y --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_3jets_y -v 3 -m 125 --setParameterRanges y=-2.0,2.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_3jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P z --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_3jets_z -v 3 -m 125 --setParameterRanges z=-1.0,3.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_3jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P k --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_3jets_k -v 3 -m 125 --setParameterRanges k=-3.0,3.0 --saveInactivePOI=1

rm -rf ttbar_4jets.txt
combineCards.py ttbar_*_4jets_*.txt > ttbar_4jets.txt
text2workspace.py ttbar_4jets.txt -o workspace_ttbar_4jets.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc3_sym:stagex_ttwc3_sym -m 125 --X-allow-no-background  -v 7 
combine -M MultiDimFit workspace_ttbar_4jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P y --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_4jets_y -v 3 -m 125 --setParameterRanges y=-2.0,2.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_4jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P z --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_4jets_z -v 3 -m 125 --setParameterRanges z=-1.0,3.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_4jets.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P k --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_4jets_k -v 3 -m 125 --setParameterRanges k=-3.0,3.0 --saveInactivePOI=1

rm -rf ttbar_semi.txt
combineCards.py ttbar_3jets.txt ttbar_4jets.txt > ttbar_semi.txt
text2workspace.py ttbar_semi.txt -o workspace_ttbar_semi.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc3_sym:stagex_ttwc3_sym -m 125 --X-allow-no-background  -v 7 
combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P y --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_y -v 3 -m 125 --setParameterRanges y=-2.0,2.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P z --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_z -v 3 -m 125 --setParameterRanges z=-1.0,3.0 --saveInactivePOI=1
combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P k --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_k -v 3 -m 125 --setParameterRanges k=-3.0,3.0 --saveInactivePOI=1

rename higgsCombine limit_ *.root
rename MultiDimFit.mH125. '' *.root
#rename y.root y_S0.root *y.root
#rename z.root z_S0.root *z.root
#rename k.root k_S0.root *k.root

rm -rf ../../output_1d
mkdir ../../output_1d
mv higgsCombine* ../../output_1d
cd ../../output_1d
rename higgsCombine limit_ *.root
rename MultiDimFit.mH125. '' *.root
cd ../