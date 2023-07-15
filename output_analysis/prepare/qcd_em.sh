cut=(0 1 2 3)
div=(0 1 2 3)
cg=(1 2 3)
for a in ${cut[*]}
do
    for b in ${div[*]}
    do
        for c in ${cg[*]}
        do
            nohup root -l -q -b derive_qcd_em.cpp"($a, $b, $c)"  >run_log/qcd_$a$b$c.txt 2>&1 &
        done
    done
done
fq="true"
while [ $fq = "true" ]
do
    ps -aux |grep "derive_qcd" |grep -v "grep"
    if [ $? -eq 0 ]; then
        echo "the derive_qcd is runing."
        sleep 1m
    else 
        echo "the derive_qcd is finished."
        fq="false"
    fi
done
for a in ${cut[*]}
do
    for b in ${div[*]}
    do
        nohup root -l -q -b add_qcd.cpp"($a, $b)" > run_log/qcds_$a$b.txt 2>&1 &
    done
done