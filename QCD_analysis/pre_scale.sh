#!/bin/bash

recorded_lumi() {
  content=$(brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i $1 --hltpath ${2}_v* | tail -n 4)
  while read line; do
    recorded=-1
    if [[ $line == \#Sum\ recorded* ]]; then
        # recorded=$(echo $line | cut -d ' ' -f 4)
        # https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html
        recorded=${line:16} # parameter substitution
        break
    fi
    done <<< "$content"
    echo $2 $recorded
  }

export -f recorded_lumi
echo "calculate lumis for json file:" $1
triggers=( HLT_Ele23_CaloIdM_TrackIdM_PFJet30 HLT_Mu27 )
parallel recorded_lumi ::: $1 ::: "${triggers[@]}"
