#!/bin/bash

MCFraction=1.0
DataFraction=1.0

#JetRs=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#@JetRs="6 7 9"; \
JetRs="4"; \

for R in $JetRs
do 
    JERSF_FILE="$ProjectBase/CommonCode/jer/Summer2024_PbPb5p36TeV_MC/Summer2024_PbPb5p36TeV_MC_SF_AK${R}PF.txt"; \
	for Suffix in NominalFine JECUp JECDown JERUp JERDown
	do
	    case $Suffix in
	      NominalFine) Shift=0.0; Smear=0.0;; 
	      JECUp) Shift=1.0; Smear=0.0;; 
	      JECDown) Shift=-1.0; Smear=0.0;;
	      JERUp) Shift=0.0; Smear=1.0;;
	      JERDown) Shift=0.0; Smear=-1.0;;
	    esac; \
	    echo "Running JetR=${R}, Suffix=$Suffix, Shift=${Shift}, Smear=${Smear}, JerFile=$JERSF_FILE"; \
	    eval `./ExecutePreset \
	      --MCTag Input/Output_PbPbMCRho_R4/PbPbMCRho \
	      --DataTag Input/Output_PbPbData_R4/PbPbData \
	      --Prefix PbPbData \
	      --Suffix $Suffix \
	      --GlobalSetting GlobalSetting.dh \
	      --Shift $Shift \
	      --Smear $Smear \
	      --Flooring false \
	      --MCFraction $MCFraction \
	      --DataFraction $DataFraction \
	      --JetR ${R} \
	      --EtaMin -2.0 \
	      --EtaMax 2.0 \
	      --UseJERSFFile true \
	      --JERSF $JERSF_FILE \
	      --UseJEU true | grep ^time`; \
	  done; \
done

