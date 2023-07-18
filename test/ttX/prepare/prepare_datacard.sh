cut=(0 1 2 3)
div=(0 1)
year=(2015 2016 2017 2018)

for a in ${cut[*]}
do
    for b in ${div[*]}
    do
        for c in ${year[*]}
        do
            nohup root -l -q -b prepare_datacard.cpp"($a, $b, $c)" > run_log/prepare_$a$b$c.txt 2>&1 &
        done
    done
done