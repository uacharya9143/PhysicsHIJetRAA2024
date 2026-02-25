#!/bin/bash

IsPP=1   # 0 = PbPb, 1 = pp NonUL, 2 = pp UL
FromMB=0   # if 1 use minimum bias
IsMC=0
FileCount=5

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`

if [[ "$IsPP" != "0" ]]; then
   #TriggerBase=HLT_HIAK4PFJet60
    #TriggerBase=HLT_HIPuAK4CaloJet100Eta5p1
    TriggerBase=HLT_AK4PFJet80
   #Trigger=HLT_HIAK4PFJet80
   #Trigger=HLT_HIPuAK4CaloJet100Eta5p1
   Trigger=HLT_AK4PFJet100
   Centrality="Inclusive"
   CutString="--DoBaselineCutAA false --DoBaselineCutPP true"
else
   #if [[ "$FromMB" != 0 ]]; then
      #TriggerBase=HLT_HIMinimumBias
      #Centrality="Inclusive"
   #else
   TriggerBase=HLT_HIPuAK4CaloJet80Eta5p1
   #fi
   Trigger=HLT_HIPuAK4CaloJet100Eta5p1
   #Trigger=HLT_AK4PFJet100
   CutString="--DoBaselineCutAA true --DoBaselineCutPP false"
fi

for R in $JetR
do
   JetRValue=`DHQuery GlobalSetting.dh JetR ${R}`

   JECBase=$ProjectBase/CommonCode/jec/
   
   JetTree=
   JECTag=
   Recluster=false
   ReEvaluate=false
   DoJetID=false
   JetIDKeyBase="none"
  
   if [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "0" ]]; then
      #if [[ "$FromMB" == "0" ]]; then
      Prefix=PbPbData
      #else
        # Prefix=PbPbMBData
      #fi
      JECTag="Summer2024_ppRef5p36TeV_MC"
	   JECList="${JECBase}/${JECTag}/${JECTag}_L2Relative_AK${R}PF.txt","${JECBase}/${JECTag}/${JECTag}_L2L3Residual_AK${R}PF.txt"
      JetTree="akCs${R}PFJetAnalyzer/t"
      Recluster=false
      ReEvaluate=true
      DoJetID=true
   fi
   if [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "1" ]]; then
      Prefix=PPData
      JECTag="Summer2024_ppRef5p36TeV_MC"
	   JECList="${JECBase}/${JECTag}/${JECTag}_L2Relative_AK${R}PF.txt","${JECBase}/${JECTag}/${JECTag}_L2L3Residual_AK${R}PF.txt"
      JetTree="ak${R}PFJetAnalyzer/t"
      Recluster=false
      ReEvaluate=false
      DoJetID=true
      JetIDKeyBase=PPData_R${R}_CentralityInclusive
      
      #elif [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "2" ]]; then
      #Prefix=PPULData
      #JECTag="Summer20UL17_ppRef5TeV_RAAV1_DATA"
      #JECTag=" Spring24_pplowPU_136TeV_V1_MC"
      #JECList="${JECBase}/${JECTag}/${JECTag}_L2Relative_AK${R}PF.txt","${JECBase}/${JECTag}/${JECTag}_L2L3Residual_AK${R}PF.txt"
      #JECList="${JECBase}/${JECTag}/${JECTag}_L2Relative_AK4PF.txt","${JECBase}/${JECTag}/${JECTag}_L2L3Residual_AK4PF.txt"
      #JetTree="ak${R}PFJetAnalyzer/t"
      #JetTree="ak4PFJetAnalyzer/t"
      #Recluster=false
      #ReEvaluate=false
      #DoJetID=true
      #JetIDKeyBase=PPData_R${R}_CentralityInclusive

   fi

   LocationBase=`DHQuery GlobalSetting.dh Sample $Prefix$R | tr -d '"'`

   for C in $Centrality
   do
      CMin=`DHQuery GlobalSetting.dh CentralityMin $C`
      CMax=`DHQuery GlobalSetting.dh CentralityMax $C`
      
      time ./Execute --Input `find $LocationBase | grep root$ | head -n${FileCount} | tr '\n' ','` \
         --OutputBase Result_${Prefix}_R${R}_Centrality${C} \
         --Base ${TriggerBase} \
         --Trigger ${Trigger} \
         --Jet "${JetTree}" --JetR ${JetRValue} \
         --Recluster ${Recluster} \
         --ReEvaluateCentrality $ReEvaluate --CentralityMin ${CMin} --CentralityMax ${CMax} \
         --EtaMin -2.0 --EtaMax 2.0 \
         ${CutString} \
         --DoJetID $DoJetID --JetIDKeyBase $JetIDKeyBase \
         --JEC $JECList --WriteTree true
   done
done


