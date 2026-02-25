#!/bin/bash

MCFraction=1.0
DataFraction=1.0

#JetRs=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetRs="6 7 8 9"; \
#JetRs="1"; \
JetRs="4"; \
#JetRs=1 2 3 4

for R in $JetRs
do 
    JERSF_FILE="$ProjectBase/CommonCode/jer/Summer2024_PbPb5p36TeV_MC/Summer2024_PbPb5p36TeV_MC_SF_AK${R}PF.txt"; \
	echo "Running JetRs=${R}, JerFile=$JERSF_FILE"; \
	eval `./ExecutePreset \
			--MCTag Input/Output_PbPbMCRho_R4/PbPbMCRho \
			--DataTag Input/Output_PbPbMCRho_R4/PbPbMCRho \
			--Prefix PbPbMCRho \
			--Suffix NominalFine \
			--GlobalSetting GlobalSetting.dh \
			--Shift 0.0 --Smear 0.0 --Flooring false \
			--MCFraction $MCFraction --DataFraction $MCFraction \
			--JetR ${R} \
			--EtaMin -2.0 --EtaMax 2.0 \
			--UseJERSFFile true \
			--JERSF $JERSF_FILE \
	      		--UseJEU true \
	    		| grep ^time`;
done

