cut=(0 1 2 3)
year=(2015 2016 2017 2018)
rm -rf run_log/
mkdir run_log/
for a in ${cut[*]}
do
    for c in ${year[*]}
    do
        nohup root -l -q -b prepare_datacard_3D.cpp"($a, $c)" > run_log/prepare_$a$c.txt 2>&1 &
    done
done