#!/bin/bash

recorded_lumi() {
    content=$(brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i $1 -u /fb --hltpath ${2}_v* | tail -n 4)
    while read line; do
        recorded=-1
        if [[ $1 =~ "2018" ]]; then
            year=2018
        elif [[ $1 =~ "2017" ]]; then
            year=2017
        elif [[ $1 =~ "2016" ]]; then
            year=2016
        elif [[ $1 =~ "2015" ]]; then
            year=2015
        fi
        if [[ $line == \#Sum\ recorded* ]]; then
            # recorded=$(echo $line | cut -d ' ' -f 4)
            # https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html
            recorded=${line:16} # parameter substitution
            break
        fi
    done <<< "$content"
    echo $year: $2 $recorded
}

export -f recorded_lumi
echo "calculate lumis for json file:" $1
triggers=( HLT_Ele23_CaloIdM_TrackIdM_PFJet30 HLT_Mu27 )
golden_json=( "../proceed_data/GoldenJSON_Files/2015_goldenJSON.txt" 
"../proceed_data/GoldenJSON_Files/2016_goldenJSON.txt" 
"../proceed_data/GoldenJSON_Files/2017_goldenJSON.txt" 
"../proceed_data/GoldenJSON_Files/2018_goldenJSON.txt" )
parallel recorded_lumi ::: "${golden_json[@]}" ::: "${triggers[@]}"