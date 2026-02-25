#!/bin/bash

IsPP=1  # 0 = PbPb, 1 = pp NonUL, 2 = pp UL
FromMB=0   # if 1 use minimum bias
IsMC=0
FileCount=5

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR=1
#Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`


for R in $JetR
do
    JetRValue=`DHQuery GlobalSetting.dh JetR ${R}`
    echo "$JetRValue" "$JetR"
	#echo "$JetR"
    JECBase=$ProjectBase/CommonCode/jec/
   	JetTree=
    JECTag=
    Recluster=false
    ReEvaluate=false
    DoJetID=true
    JetIDKeyBase="none"

    # Process PbPb Data if IsMC==0 and IsPP==0
    if [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "1" ]]; then
	Prefix=PPData
	#JECTag="Summer2024_PbPb5p36TeV_MC"
	JECTag="Summer2024_ppRef5p36TeV_MC"
	JECList="${JECBase}/${JECTag}/${JECTag}_L2Relative_AK${R}PF.txt"
	#,"${JECBase}/${JECTag}/${JECTag}_L2L3Residual_AK${R}PF.txt"
	JetTree="ak${R}PFJetAnalyzer/t"
		
	Recluster=false
	ReEvaluate=false
	DoJetID=true

	# Set PbPb-specific trigger settings
	TriggerBase=HLT_AK4PFJet80
	Trigger=HLT_AK4PFJet100
	#TriggerBase='none'
	#Trigger='none'
	CutString="--DoBaselineCutAA false --DoBaselineCutPP true"
	Centrality="Inclusive"  # No loop over centrality
	#Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
	
	LocationBase=`DHQuery GlobalSetting.dh Sample $Prefix$R | tr -d '"'`
	# Add after the LocationBase line in PP section:	
	echo "DEBUG: Looking for Sample entry: $Prefix$R"
	echo "DEBUG: LocationBase = $LocationBase"
	echo "DEBUG: Files found:"
	find $LocationBase | grep root$ | head -n5
	echo "---"
	#for C in $Centrality
	#do
        #CMin=`DHQuery GlobalSetting.dh CentralityMin $C`
        #CMax=`DHQuery GlobalSetting.dh CentralityMax $C`
            
    time ./Execute --Input `find $LocationBase | grep root$ | head -n${FileCount} | tr '\n' ','` \
	--OutputBase Output/Result_${Prefix}_R${R}_Centrality${Centrality} \
	--Base ${TriggerBase} \
	--Trigger ${Trigger} \
	--Jet "${JetTree}" --JetR ${JetRValue} \
	--Recluster ${Recluster} \
	--ReEvaluateCentrality $ReEvaluate \
	--EtaMin -2.0 --EtaMax 2.0 \
	--DoJetID $DoJetID --JetIDKeyBase $JetIDKeyBase \
	--JEC $JECList --WriteTree true
	#done
    fi
done



