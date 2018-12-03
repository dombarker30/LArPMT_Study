#!/usr/bin/env bash

#Setup the directories
export LArAnASRCDir=$(dirname "$(realpath $_)")
export LArANAOBJDir=$LArAnASRCDir/../build/obj
export LArANAEXEDir=$LArAnASRCDir/../build/exe
export LArANABUILDDIR=$LArAnASRCDir/../build

#Setup the installer
larinstall(){
    current_dir=$PWD;
    cd $LArAnASRCDir; 
    make;
    cd $current_dir;
}

#Setup the uninstaller
laruninstall(){
    rm -rf $LArAnASRCDir;
    rm -rf $LArANABUILDDIR;
}


#Setup the Quick DAQ Commands


#Setup the Quick exuctable commands.
export crt_calibration=$LArANAEXEDir/crt_calibration
export crt_coincidence=$LArANAEXEDir/crt_coincidence
export keysight_deocoder=$LArANAEXEDir/KeySight_DAQDecoder
