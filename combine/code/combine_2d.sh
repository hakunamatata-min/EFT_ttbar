#!/bin/bash
now=$PWD
cd /afs/cern.ch/user/m/mitang/CMSSW_11_3_4/src
eval `scramv1 runtime -sh`
cd /afs/cern.ch/user/m/mitang/EFT_ttbar/combine

cd $1/original
echo $1

rm -rf ttbar_semi.txt
combineCards.py ttbar_*_*_*.txt > ttbar_semi.txt
text2workspace.py ttbar_semi.txt -o workspace_ttbar_semi.root --PO doStage0 --PO doacttbar -P HiggsAnalysis.CombinedLimit.stagex_ttwc:stagex_ttwc -m 125 --X-allow-no-background  -v 7 
#combine -M GenerateOnly --saveToys workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10  -v 3  -m 125
#text2workspace.py ttbar_4jets.txt -o workspace_ttbar_semi_fcp.root --PO doStage0 --PO doacttbar --PO dofcp -P HiggsAnalysis.CombinedLimit.stagex_ttwc3:stagex_ttwc3 -m 125 --X-allow-no-background  -v 7 
#combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P k --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_K -v 3 -m 125 --setParameterRanges x=-3.0,3.0 --saveInactivePOI=1
#combine -M MultiDimFit workspace_ttbar_semi.root -S 1  -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P y --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_y -v 3 -m 125 --setParameterRanges y=-2.0,2.0 --saveInactivePOI=1 
#combine -M MultiDimFit workspace_ttbar_semi.root -S 1  -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P z --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_z -v 3 -m 125 --setParameterRanges z=-1.0,3.0 --saveInactivePOI=1 
#combine -M MultiDimFit workspace_ttbar_semi.root -S 1  -t -1 --expectSignal=1 --algo=grid --points=120 --alignEdges=1 -P k --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_k -v 3 -m 125 --setParameterRanges k=-3.0,3.0 --saveInactivePOI=1 

combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=6400 --alignEdges=1  -P k -P y --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_2Dyk_fix -v 3 -m 125 --setParameterRanges y=-3.0,1.0:k=-3.0,3.0 --saveInactivePOI=1 &> out1.txt
combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=6400 --alignEdges=1  -P z -P k --floatOtherPOIs=1 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_2Dzk -v 3 -m 125 --setParameterRanges z=-1.0,3.0:k=-3.5,3.5:y=-3.0,3.0 --saveInactivePOI=1 &>out2.txt
combine -M MultiDimFit workspace_ttbar_semi.root -S 1 -t -1 --expectSignal=1 --algo=grid --points=6400 --alignEdges=1  -P y -P z --floatOtherPOIs=0 --X-rtd TMCSO_AdaptivePseudoAsimov=10 -n ttbar_semi_2Dyz_fix -v 3 -m 125 --setParameterRanges y=-3.0,1.0 --saveInactivePOI=1 &>out4.txt


rm -rf ../output_2d
mkdir ../output_2d
mv higgsCombine*2D* ../output_2d
mv out*.txt ../output_2d
cd ../output_2d
rename higgsCombine limit_ *.root
rename MultiDimFit.mH125. '' *.root
cd ../

#  -S [ --systematics ] arg (=1) Include constrained systematic uncertainties, 
#-S 0 will ignore systematics constraint terms in the datacard.
#   -t [ --toys ] arg (=0)  Number of Toy MC extractions
# --algo arg (=none)   Algorithm to compute uncertainties
#--grid arg  Use the specified file containing a grid of SamplingDistributions for the limit
# --X-rtd arg Define some constants to be used at runtime (for debugging purposes). 
# -v [ --verbose ] arg (=0) Verbosity level (-1 = very quiet; 0 =quiet, 1 = verbose, 2+ = debug) 
# --alignEdges arg (=0) Align the grid points such that the endpoints of the ranges are included
