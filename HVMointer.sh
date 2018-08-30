#!/bin/bash  
tmpfile=/home/argonshef/CAENHV/HVtmpfile.txt
/home/argonshef/CAENHV/sethv --monitorchannel 4 N > $tmpfile

if [ ! $? ];
    then
    python /home/argonshef/LArPurityPMT/WorkDir/email_HV.py
fi

