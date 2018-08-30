#!/bin/bash
#Code to run the DAQ for the purity measurement. Edit params.txt to change the configuration - Dominic Barker 01/08/2018
WorkDir="/home/argonshef/LArPurityPMT/WorkDir/"
Email="email_DAQ.py"
 
if [ $PWD = "/home/argonshef/LArPurityPMT/WorkDir" ]; 
then
    echo "Please Run this not fromt the Work Direcory. Create a runx directory in LArPurityPMT"
    exit 1
fi


timestamp=$(date +%s)
fails=0

while [ $timestamp -le 1535994000 ]
do
    tmpfile=tmpfile.txt
    $HOME/AgMD2_GPM/runGPM > $tmpfile

    #Check the exist code 
    if [ ! $? ];
    then
	fails++
    fi
    
    if [ $fails -gt 2 ];
    then
	python $WorkDir$Email 1
	exit 1 
    fi

    python $WorkDir$Email 0
done 

