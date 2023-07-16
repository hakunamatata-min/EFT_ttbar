#!bin/bash
#bash Add.sh
files=( NNLO_18_TTToSemiLeptonic_TuneCP5_13TeV-powheg \
        NNLO_18_TTTo2L2Nu_TuneCP5_13TeV-powheg \
        NNLO_18_TTToHadronic_TuneCP5_13TeV-powheg \
        NNLO_17_TTToSemiLeptonic_TuneCP5_13TeV-powheg \
        NNLO_17_TTTo2L2Nu_TuneCP5_13TeV-powheg \
        NNLO_17_TTToHadronic_TuneCP5_13TeV-powheg \
        NNLO_16_TTToSemiLeptonic_TuneCP5_13TeV-powheg \
        NNLO_16_TTTo2L2Nu_TuneCP5_13TeV-powheg \
        NNLO_16_TTToHadronic_TuneCP5_13TeV-powheg \
        NNLO_15_TTToSemiLeptonic_TuneCP5_13TeV-powheg \
        NNLO_15_TTTo2L2Nu_TuneCP5_13TeV-powheg \
        NNLO_15_TTToHadronic_TuneCP5_13TeV-powheg )
dir="/home/yksong/code/output/EW_un"
for ((i=0;i<12;i++))
do
    #echo $dir/${files[i]}
    hadd $dir/${files[i]}.root $dir/${files[i]}_*.root
done