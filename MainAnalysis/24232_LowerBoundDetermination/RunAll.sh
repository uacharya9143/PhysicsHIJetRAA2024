#!/bin/bash

JetR=`DHQuery GlobalSetting.dh Global JetR`
Centrality=`DHQuery GlobalSetting.dh Global Centrality`
#Centrality="Inclusive"
#JERFile=${ProjectBase}/CommonCode/jer/Summer2024_ppRef5p36TeV_MC/Summer2024_ppRef5p36TeV_MC_SF_AK${R}PF.txt

for R in $JetR
do
#   for C in $Centrality Inclusive
   for C in $Centrality
#   for C in $Centrality
   do
      Prefix=
      if [[ "$C" == "Inclusive" ]]; then
         Prefix=PPMC
      else
         Prefix=PbPbMCRho
      fi
      #JERFile=${ProjectBase}/CommonCode/jer/24215_JER/JER_${Prefix}_R${R}_Centrality${C}.txt
      JERFile=${ProjectBase}/CommonCode/jer/Summer2024_5p36TeV_JER/JER_${Prefix}_R${R}_Centrality${C}.txt
      #echo "$JERFile"

      TriggerBase=R${R}_Centrality${C}
      ./Execute --DHFile GlobalSetting.dh --Base R${R}_Centrality${C} \
         --MinPT 100 \
         --DoTrigger true --TriggerPercentage 0.99 --TriggerBase $TriggerBase \
         --DoTriggerResolution true --TriggerResolutionShift 2 --ResolutionFile $JERFile
   done
done
#sqrt([0]*abs([0])/(x*x)+[1]*[1]*pow(x,[3])+[2]*[2]) #LunaFile
#sqrt(pow(p0,2)+pow(p1,2)/x+pow((p2/x),2)+pow((p3/x),3)) #UttamFile
