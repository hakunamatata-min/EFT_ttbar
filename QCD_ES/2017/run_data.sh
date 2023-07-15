#!/bin/bash
#voms-proxy-init --voms cms -valid 192:00 -out ~/temp/x509up
# sumbit asssinment: condor_submit condor.sub
#source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.18.04/x86_64-centos7-gcc48-opt/bin/thisroot.sh
mkdir -p myout
output=$PWD/myout
echo "output: $output"
wrong="f"
cd /afs/cern.ch/user/y/yuekai/ttbar/condor/QCD_ES/2017/condor_out_data/$1
file=$(ls ${1}.txt)
dir_f=$(cat $file)
#dir="root://cms-xrd-global.cern.ch/"$dir
dir="root://xrootd-cms.infn.it/"$dir_f
eos="/eos/user/y/yuekai/ttbar/QCD_ES/2017/data"
inputFile=${file%.txt*}
inputFile=${inputFile}.root
echo $dir >$output/out1.txt
xrdcp $dir $output 2>>$output/out1.txt
#python crab_script.py $dir $output 2>>$output/out1.txt
jug=`cat $output/out1.txt|grep "ERROR"`
n=$(ls $output|grep root|wc -l)
if [[ $n -ne 1 || $jug != "" ]]
then
    rm -f $output/*.root
    dir="root://cms-xrd-global.cern.ch/"$dir_f
    echo $dir >$output/out2.txt
    xrdcp $dir $output 2>>$output/out2.txt
    #python crab_script.py $dir $output 2>>$output/out2.txt
    jug=`cat $output/out2.txt|grep "ERROR"`
    n=$(ls $output|grep root|wc -l)
    if [[ $n -ne 1 || $jug != "" ]]
    then
        rm -f $output/*.root
        dir="root://cmsxrootd.fnal.gov/"$dir_f
        echo $dir >$output/out3.txt
        xrdcp $dir $output 2>>$output/out3.txt
        #python crab_script.py $dir $output 2>>$output/out3.txt
        jug=`cat $output/out3.txt|grep "ERROR"`
        n=$(ls $output|grep root|wc -l)
        if [[ $n -ne 1 || $jug != "" ]]
        then
            wrong="t"
        fi
    fi
fi
mv $output/out*.txt /afs/cern.ch/user/y/yuekai/ttbar/condor/QCD_ES/2017/condor_out_data/$1
if [[ $wrong == "f" ]]
then
    echo "input file: $dir"
    cd /afs/cern.ch/user/y/yuekai/ttbar/condor/QCD_ES
    input=$(ls $output|grep root)
    root -l -q -b ./process.cpp"(\"$output\",\"$inputFile\",\"$output/$input\",2017,0,0)"
    root -l -q -b ./process.cpp"(\"$output\",\"$inputFile\",\"$output/$input\",2017,1,0)"
    root -l -q -b ./process.cpp"(\"$output\",\"$inputFile\",\"$output/$input\",2017,2,0)"
    root -l -q -b ./process.cpp"(\"$output\",\"$inputFile\",\"$output/$input\",2017,3,0)"
    num=$(ls $output|grep new|wc -l)
    if [ $num -eq 4 ]
    then
        mv $(ls $output/new*_A.root)  ${eos}/A
        mv $(ls $output/new*_B.root)  ${eos}/B
        mv $(ls $output/new*_C.root)  ${eos}/C
        mv $(ls $output/new*_D.root)  ${eos}/D
    else
	    echo "Failed. task unfinished"
    fi
fi
rm -rf $output
echo "root files are storied in $eos"
#var=$1
#if [[ $var =~ "_pythia" ]]
#then
#	temp=${var%%_pythia8*}
#else
#    temp=${var%%-pythia8*}
#fi
#process=${temp:1}
#dasgoclient --query "file dataset=$1" > ${process}.txt
#dasgoclient -query="file dataset=$1" > ${process}.txt
# root -l -q -b ../get_info.cpp"(\"$1\")"
