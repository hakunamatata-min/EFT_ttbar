txt=condor_list_QCDem.txt
cd condor_out_QCDem
rm -rf $txt
for var in `ls`
do 
    jug=$(ls $var | grep yuekai.cc)
    if [[ "$jug" == "" ]]
    then 
        echo 1 
	    echo $var >> $txt 
	fi
#   rm -rf $var
done
echo "The failed tasks(if have) are summarized in $txt "
mv $txt ../
cd ..
