#!/bin/bash

MCFraction=1.0
DataFraction=1.0

#JetRs=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#@JetRs="6 7 9"; \
#JetRs="1 2 3 4 6 7 8 9"; \
JetRs="4"; \

for R in $JetRs
do 
    #JERSF_FILE=""; \
	JERSF_FILE="$ProjectBase/CommonCode/jer/Summer2024_ppRef5p36TeV_MC/Summer2024_ppRef5p36TeV_MC_SF_AK${R}PF.txt"; \
	 #when you enable this enable useJERSF to "true"
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
	      --MCTag Input/Output_PPMC_R4/PPMC \
	      --DataTag Input/Output_PPData_R4/PPData \
	      --Prefix PPData \
	      --Suffix $Suffix \
	      --Centrality "Inclusive" \
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

#FOr the MC as sample in for both data and simulation, enable the 
			##double jtPt=MData.GetValue(Reco,PrimaryType,PrimaryIndex,iJ,0,0, 1);
			##if(jtPt>2.5*MData.GetPTHat()) continue;
#for the data section in PrepareUnfold.cpp code.## BY uttam. This is for the same cut applied to the bot data set when they are both from the MC levels. 
###FOr the dataTag from the data part, disable it because the data do not have this ptHat.