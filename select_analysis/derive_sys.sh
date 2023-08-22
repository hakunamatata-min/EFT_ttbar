cut=(0 1 2 3)
year=(2015 2016 2017 2018)
rm -rf 
for((i=0;i<13;i++))
do
    for b in ${cut[*]}
    do
        for c in ${year[*]}
        do
            nohup root -l -q -b derive_sys.cpp"($b, $c, $i)" > run_log/prepare_$b$c$1.txt 2>&1 &
        done
    done
done
