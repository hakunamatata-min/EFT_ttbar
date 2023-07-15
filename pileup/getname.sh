#writen by Renqi Pan in Oct 22,2021
# to write dataset files into .txt and divide
input="../MC_2017.txt"
for dataset in `cat $input`
do 
	echo $dataset
    dasgoclient --query="file dataset=$dataset" >> MC.txt
done
