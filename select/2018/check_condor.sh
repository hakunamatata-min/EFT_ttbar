txt=condor_list_wrong.txt
cd condor_out_MC
rm -rf $txt   
for var in `cat ../condor_list_MC.txt`
do 
    jug=$(ls $var | grep yuekai.cc)
    if [[ "$jug" != "" ]]
    then 
	    disk=`cat $var/run.err|grep "Disk quota exceeded"`
        close=`cat $var/run.err|grep "trying to recover"`
        if [[ $disk != "" ]]
        then
            echo "$var Disk quota exceeded"
            echo ${var} >> $txt
        elif [[ $close != "" ]]
        then
            echo "$var not closed, trying to recover"
            echo ${var} >> $txt
        fi
    #else
		#echo "$var didn't finish"
		#echo $var >> $txt	
    fi
done
rm -rf ../$txt
mv $txt ../
echo "The failed tasks(if have) are summarized in $txt "
cd ..