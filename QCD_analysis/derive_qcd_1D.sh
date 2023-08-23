cut=(0 1 2 3)
year=(2015 2016 2017 2018)
cg=(1 2 3)
rm -rf run_log/
mkdir run_log/
for a in ${cut[*]}
do
    for b in ${cg[*]}
    do
        for c in ${year[*]}
        do
            nohup root -l -q -b derive_qcd_1D.cpp"($a, $b, $c, 0)"  >run_log/qcd_1D_$a$b$c.txt 2>&1 &
        done
    done
done