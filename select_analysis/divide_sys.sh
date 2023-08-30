cutname=(M_4jets M_3jets E_4jets E_3jets)
years=(2015 2016 2017 2018);
sys=("jes_Absolute" "jes_Absolute_un" "jes_FlavorQCD" "jes_BBEC1" "jes_EC2" "jes_HF" "jes_BBEC1_un" "jes_EC2_un" "jes_RelativeBal" "jes_RelativeSample_un" "jer" "unclus" "sl" "sb_un" "sb_co" "sl_un" "sl_co" "L1PF" "muR" "muF" "ISR" "FSR" "mtop" "hdamp" "TuneCP5" "nnlo" "EW_un" "alphas" "pdf" "qcds" "PU")
rm -rf sys_pdf/
mkdir sys_pdf/
for c in ${sys[*]}
do
    mkdir sys_pdf/$c
    for b in ${years[*]}
    do
        mkdir sys_pdf/$c/$b
        for a in ${cutname[*]}
        do
            mkdir sys_pdf/$c/$b/$a
        done
    done
done
cd ./draw
root -l -q -b draw_sys.cpp
cd ../

        