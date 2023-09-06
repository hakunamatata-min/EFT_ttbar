cutname=(M_4jets M_3jets E_4jets E_3jets)
years=(2015 2016 2017 2018)
rm -rf ew_pdf/
mkdir ew_pdf/
for c in ${cutname[*]}
do
    for b in ${years[*]}
    do
        mkdir ew_pdf/${c}_${b}
    done
done
cd ./draw
for((s=0;s<5;s++));do
    for((t=0;t<3;t++));do
        root -l -q -b draw_ew.cpp"(0, 3, $s, $t, 0)"
    done
done
cd ../