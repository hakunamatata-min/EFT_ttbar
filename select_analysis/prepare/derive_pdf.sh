cut=(0 1 2 3)
div=(0 1 2 3)
pro=(5 6 7)
year=(2015 2016 2017 2018)
for a in ${cut[*]}
do
    for b in ${div[*]}
    do
        for c in ${year[*]}
        do
            for d in ${pro[*]}
            do
                nohup root -l -q -b derive_pdf.cpp"($a, $b, $c, $d)"  >run_log/pdf_$a$b$c$d.txt 2>&1 &
            done
        done
    done
done