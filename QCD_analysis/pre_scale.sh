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
golden_json=( "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt" 
"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Legacy_2017/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt" 
"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Legacy_2016/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt" )
parallel recorded_lumi ::: "${golden_json[@]}" ::: "${triggers[@]}"