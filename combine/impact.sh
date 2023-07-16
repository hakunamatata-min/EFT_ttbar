now=$PWD
cd /afs/cern.ch/user/y/yuekai/tth_cms/CMSSW_11_3_4/src/
eval `scramv1 runtime -sh`
cd $now



cd datacard/no
rm -f ttbar_semi.txt
combineCards.py ttbar_*_*_*.txt > ttbar_semi.txt

text2workspace.py ttbar_semi.txt -o workspace_ttbar.root --PO doStage0 --PO doacttbar --PO domu -P HiggsAnalysis.CombinedLimit.stagex_ttwc:stagex_ttwc -m 125 --X-allow-no-background  -v 7 > out 

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --doInitialFit -v 7  --redefineSignalPOIs z --freezeParameters y,k &> initial.txt

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --doFits -v 7  --redefineSignalPOIs z --freezeParameters y,k  &> dofit.txt

combineTool.py -M Impacts -d ./workspace_ttbar.root -m 125 -t -1 --robustFit 1 --redefineSignalPOIs z --exclude y,k --output impacts.json &> pjson.txt

plotImpacts.py -i impacts.json -o impacts #--P z

rm -rf ../../impact_z
mkdir ../../impact_z
mv *Fit* ../../impact_z
mv initial.txt ../../impact_z
mv dofit.txt ../../impact_z
mv pjson.txt ../../impact_z
mv impacts* ../../impact_z
mv out ../../impact_z
cd ../../
