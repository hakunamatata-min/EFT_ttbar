txt=unfinish.txt
cd condor_out_MC
for var in `cat ../condor_list_MC.txt`
do 
    jug=$(ls $var | grep yuekai.cc)
    if [[ "$jug" != "" ]]
    then 
	    o=`cat $var/run.err|grep "Disk quota exceeded"`
        if [[ $o != "" ]]
        then
        #echo "$var finished"
            echo ${var} >> $txt
        fi
    else
		echo "$var didn't finish"
		echo $var >> $txt	
    fi
#   rm -rf $var
done
rm -rf ../$txt
mv $txt ../
echo "The failed tasks(if have) are summarized in $txt "
cd ..
