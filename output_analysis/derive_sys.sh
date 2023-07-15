lep=(1 2)
cate=(1 2)
width=("widthx0p55" "widthx1p45")
for a in ${lep[*]}
do
    for b in ${cate[*]}
    do
        for c in ${width[*]}
        do
            nohup root -l -q -b derive_sys.cpp"($b, $a, \"$c\")" >run_log/sys_$a$b$c.txt 2>&1 &
        done
    done
done
