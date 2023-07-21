cd condor_out_MC
for i in `ls .`
do 
    cd $i
    result=`cat *txt|grep 4542DA7B-739F-5D45-BA3B-68E76301A7EA.root`
    if [[ $result != "" ]]
    then
        echo $i
    fi
    cd ..
done
