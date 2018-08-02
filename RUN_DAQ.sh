#!/bin/bash
#Code to run the DAQ for the purity measurement. Edit params.txt to change the configuration - Dominic Barker 01/08/2018

if [ $PWD = "/home/argonshef/LArPurityPMT/WorkDir"]; 
then
    echo "Please Run this not fromt the Work Direcory. Create a runx directory in LArPurityPMT"
    return 0
done 

timestamp=$(date +%s)

while [ $timestamp -le 1535994000 ]
do
    $HOME/AgMD2_GPM/runGPM
    python email_DAQ.py
done 
