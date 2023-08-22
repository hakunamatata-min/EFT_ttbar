cut=(0 1 2 3)
year=(2015 2016 2017 2018)
var=(0 1 7 8 9)
rm -rf run_log/
mkdir run_log/
for a in ${var[*]}
do
    for b in ${cut[*]}
    do
        for c in ${year[*]}
        do
            nohup root -l -q -b derive_sys.cpp"($b, $c, $a)" > run_log/derive_sys_${b}_${c}_${a}.txt 2>&1 &
        done
    done
done
