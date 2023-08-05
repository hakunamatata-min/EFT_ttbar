files=( TT \
        DY \
        ST \
        W \
        QCD )
year=( 2015 2016 2017 2018 )
dir_year=( "UL2016_pre" "UL2016_post" "UL2017" "UL2018" )
for ((y=0;y<4;y++))
do
    sample_year=${year[y]}
    dir=./$sample_year/output
    cd $dir
    for ((i=0;i<5;i++))
    do
        rm -f btageff_${files[i]}.root
        hadd btageff_${files[i]}.root btageff_*${files[i]}*.root
        root -l -q -b ../../derive_eff.cpp"(\"btageff_${files[i]}.root\")"
        mv btageff_${files[i]}.root ../../../${dir_year[y]}/btag
    done
    cd ../../
done