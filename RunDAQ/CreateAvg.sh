#!/bin/bash  

if [ $PWD = "/home/argonshef/LArPurityPMT/WorkDir" ];
then
    echo "Please Run this not fromt the Work Direcory. Create a runx directory in LArPurityPMT"
    exit 1
fi


for file in *.dat; do
    
    echo $file

    #Make Sure the file exists and is not empt 
    [ -s "$file" ] || continue

    #Check That .root file does not already exist 
    rootfile="${file%.dat}.root"
    echo $rootfile

    if [ ! -s "$rootfile" ];
    then 
	#Create the root file 
	./waveform  "$file"  0 0 0 0 0 6 0 0
    fi

    #Check that the Averging file does not exist 
    avgfile="${file%.dat}_AVG.root"
    echo $avgfile 

    if [ -s "$rootfile" ] && [ ! -s "$avgfile" ];
    then 
    	#Create the Averaging file.
    	/home/argonshef/Lohan_files/LAr\ runs/dataAveraging $rootfile $avgfile
    fi

done 
