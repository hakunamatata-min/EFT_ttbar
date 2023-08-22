cut=(0 1 2 3)
cg=(1 2 3)
year=(2015 2016 2017 2018)
root -l -q -b convert_3Dto1D.cpp
cd ../QCD_analysis/
rm -rf run_log
mkdir run_log
for a in ${cut[*]}
do
    for b in ${cg[*]}
    do
        for c in ${year[*]}
        do
            nohup root -l -q -b derive_qcd_3D.cpp"($a, $b, $c, 1)"  >run_log/qcd_$a$b$c.txt 2>&1 &
        done
    done
done

fq="true"
while [ $fq = "true" ]
do
    ps -aux |grep "derive_qcd_3D" |grep -v "grep"
    if [ $? -eq 0 ]; then
        echo "the derive_qcd is runing."
        sleep 1m
    else 
        echo "the derive_qcd is finished."
        fq="false"
    fi
done
root -l -q -b convert_3Dto1D.cpp
root -l -q -b add_qcd.cpp
cd ../combine
root -l -q -b write_datacard.cpp