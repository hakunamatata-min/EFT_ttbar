#!bin/bash
#bash EW_un.sh
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
dir="./output"
for ((i=0;i<12;i++))
do
    rm -f $dir/${files[i]}.root
    hadd $dir/${files[i]}.root $dir/${files[i]}_*.root
done
root -l -q -b Add.cpp
root -l -q -b derive_ratio.cpp