#!/bin/bash
#Code to run the DAQ for the purity measurement. Edit params.txt to change the configuration - Dominic Barker 01/08/2018

timestamp=$(date +%s)
echo $timestamp
while [ $timestamp -le 1535994000 ]
do
    ../AgMD2_GPM/runGPM
    python email_DAQ.py
done 
