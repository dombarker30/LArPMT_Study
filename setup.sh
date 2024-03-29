#!/usr/bin/env bash

#Setup the directories
export LArANASRCDir=$(dirname "$(realpath $BASH_SOURCE)")
export LArANAOBJDir=$LArANASRCDir/../build/obj
export LArANAEXEDir=$LArANASRCDir/../build/exe
export LArANABUILDDIR=$LArANASRCDir/../build


 #Setup the installer
larinstall(){
    current_dir=$PWD;
    cd $LArANASRCDir; 
    make;
    cd $current_dir;
    lartillclean
}

#Setup the uninstaller
laruninstall(){
echo "You Fool. You cannot uninstall me"
#    rm -rf $LArANASRCDir;
#    rm -rf $LArANABUILDDIR;
}

lartillclean(){
    find -name '*~' | args rm  
}


#Setup the Quick DAQ Commands
export startdaq=/home/argonshef/CAENDAQ/PixelDAQ/bin/x86_64/Release/DAQDriver.bin 
alias  daqdir="cd /home/argonshef/CAENDAQ/PixelDAQ/"
alias  startkeysightdaq="/home/argonshef/KeysightDAQ/runGPM"

export crt_calibration=$LArANAEXEDir/crt_calibration
export crt_coincidence=$LArANAEXEDir/crt_coincidence
#export KeySight_DAQDecoder=$LArANAEXEDir/KeySight_DAQDecoder
export KeySight_DAQDecoder=/home/argonshef/LArAnalysis/build/exe/KeySight_DAQDecoder
export PixelTPC_DAQDecoder=$LArANAEXEDir/PixelTPC_DAQDecoder

#print Commands and locations 
larprint(){
echo "##############################################################"
echo "# Below is a list of Exectuables:                             #"
echo "# crt_calibration                                             #" 
echo "# crt_coincidence                                             #"
echo "# KeySight_DAQDecoder                                         #"
echo "# PixelTPC_DAQDecoder                                         #"
echo "#                                                             #" 
echo "# Below is a list of quick links. Add a doller to the start   #"
echo "# LArAnASRCDir    source directory                            #"
echo "# LArANAOBJDir    object directory                            #"
echo "# LArANAEXEDir    exe directory                               #"
echo "# LArANABUILDDIR  build directory                             #"
echo "#                                                             #"
echo "# startdaq will run the DAQ exectuable with the config file   #"
echo "# found in the CAENDAQ/PixelDAQ/srcs. If the DAQ is running   #"
echo "# you can quit anytime by pressing q.                         #" 
echo "#                                                             #"
echo "# 'startkeysightdaq' will run the DAQ for the PMT and/or CRTs #"
echo "# using the config file params.txt found in                   #"
echo "# 'LArAnalysis/srcs/SubSystems/DAQConfig'.                    #"
echo "###############################################################"
}

#Further assitance
larhelp(){
echo" Hello welcome to the laranalysis page first sourcce the setup script source.sh. Then larprint will show you want exetubales exist some may need arguments such as files which have a '-i' signifies as the input file. For further info on how to run each exctuable type 'exectuable help' for guidance."
echo " " 
larprint
} 
  
echo "LAr Analysis setup complete" 
