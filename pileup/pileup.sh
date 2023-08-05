#!bin/bash
#bash pileup.sh
files=( TTToSemiLeptonic_TuneCP5_13TeV-powheg \
        TTTo2L2Nu_TuneCP5_13TeV-powheg \
        TTToHadronic_TuneCP5_13TeV-powheg \
        DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM \
        DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM \
        ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo \
        ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin \
        ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin \
        ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg \
        ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg \
        W1JetsToLNu_TuneCP5_13TeV-madgraphMLM \
        W2JetsToLNu_TuneCP5_13TeV-madgraphMLM \
        W3JetsToLNu_TuneCP5_13TeV-madgraphMLM \
        W4JetsToLNu_TuneCP5_13TeV-madgraphMLM )
year=( 2015 2016 2017 2018 )
for ((y=0;y<4;y++))
do
    sample_year=${year[y]}
    dir=./$sample_year/output
    for ((i=0;i<20;i++))
    do
        rm -f $dir/Mu_${files[i]}.root
        hadd $dir/Mu_${files[i]}.root $dir/Mu_${files[i]}_*.root
    done
    root -l -q -b Add.cpp"($sample_year)"
    root -l -q -b derive_pu_w.cpp"($sample_year)"
done