#!bin/bash
#bash EW_un.sh
#writen by Yuekai Song in Jul 21,2023
# to write dataset files into .txt and divide
input=MC.txt
i=0
for dataset in `cat $input`
do 
    echo $dataset
    var="Chunk$i"
    rm -rf $var
    mkdir $var
    cd $var
    if [[ $dataset =~ "-pythia" ]]
    then
        temp=${dataset%%-pythia8*}
    fi
    process=${temp:1}
    if [[ $dataset =~ "UL18" ]]
    then
        process=18_${process}
    fi
    if [[ $dataset =~ "UL17" ]]
    then
        process=17_${process}
    fi
    if [[ $dataset =~ "preVFP" ]]
    then
        process=15_${process}
    elif [[ $dataset =~ "UL16" ]]
    then
        process=16_${process}
    fi
    #dasgoclient --query "file dataset=$dataset" > ${process}.txt
    dasgoclient -query="file dataset=$dataset" > ${process}.txt
    cd ../
    let i=i+1
done

#source adjust_entry.sh

echo "divide each dataset into several pieces "
max=0
for var in `ls | grep Chunk`
do 
    cd $var
    echo $var
    process=$( ls *.txt )
    process=${process%%.txt*}
    total=$(cat *.txt | wc -l)
    if [[ $max -le $total ]]
    then 
        max=$total
    fi
    #divide=$total  #edit this line  
    #lines=$(($total / $divide ))
    #remind=$(($total % $divide ))
    #echo "total=$total line=$lines divide=$divide remind=$remind"
    echo "total=$total"
    num_txt=1
    #touch ${process}_{1..8}.txt
    for line in $(cat *.txt)
    do
        mkdir ../${process}_${num_txt}
        cd ../${process}_${num_txt}
        txt_name=${process}_${num_txt}.txt
        echo $line >> $txt_name
        let num_txt=num_txt+1
        cd ../$var
    done
    cd ../
done
out=condor_out_MC
rm -rf $out
mkdir $out
exp="mv *_{1.."$max"} "$out
eval $exp 2> /dev/null
rm -rf Chunk*
ls $out  > condor_list_MC.txt
echo "directories are written into condor_list.txt"