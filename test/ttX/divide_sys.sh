cutname=(M_4jets M_3jets E_4jets E_3jets)
years=(2015 2016 2017 2018);
sys=("jes" "jer" "unclus" "sl" "sj_unco" "sj_co" "L1PF" "muR" "muF" "ISR" "FSR" "mtop" "hdamp" "TuneCP5" "nnlo" "EW_un" "pdf" "qcds" "PU")
type=("no" "flat" "smooth")
rm -rf sys_pdf/
mkdir sys_pdf/
for d in ${type[*]}
do
    mkdir sys_pdf/$d
    for c in ${sys[*]}
    do
        mkdir sys_pdf/$d/$c
        for b in ${years[*]}
        do
            mkdir sys_pdf/$d/$c/$b
            for a in ${cutname[*]}
            do
                mkdir sys_pdf/$d/$c/$b/$a
            done
        done
    done
done
#cd ./draw
#root -l -q -b draw_sys.cpp
#cd ../

        