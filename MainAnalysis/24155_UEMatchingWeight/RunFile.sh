#!/bin/sh

mkdir -p pdf
mkdir -p root

JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`
#Centrality="Inclusive"
# Centrality="50to70 70to90"

JetR='1 2 3 4 6 7 8 9'
#JetR='4'

for R in $JetR
do
   for C in $Centrality
   do
      echo Running R $R C $C

     TriggerPTMin=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_Cut`
      if [[ $R == "8" ]] || [[ $R == "9" ]]; then
         if [[ $C == "0to10" ]]; then
            TriggerPTMin=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_X95`
         fi
      fi
      if [[ $C == "50to70" ]] || [[ $C == "70to90" ]]; then
         TriggerPTMin=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality50to90_Cut`
      fi
      DHSet GlobalSettingTemp.dh RhoWeight R${R}_Centrality${C}_StartingPT float $TriggerPTMin

      ./Execute --Data Input/Output_PbPbData_PPData/PbPbData_R${R}_Centrality${C}.root \
         --MC Input/Output_PbPbMC/PbPbMC_R${R}_CentralityInclusive.root \
         --JetMinPT ${TriggerPTMin} --Output root/Output_R${R}_Centrality${C}.root
      
#      ./Execute --Data Input/Output_PbPbData_CentralityUp/PbPbDataCentralityUp_R${R}_Centrality${C}.root \
#         --MC Input/Output_PbPbMC/PbPbMC_R${R}_CentralityInclusive.root \
#         --JetMinPT ${TriggerPTMin} --Output root/OutputCentralityUp_R${R}_Centrality${C}.root
      
#      ./Execute --Data Input/Output_PbPbData_CentralityDown/PbPbDataCentralityDown_R${R}_Centrality${C}.root \
#         --MC Input/Output_PbPbMC/PbPbMC_R${R}_CentralityInclusive.root \
#         --JetMinPT ${TriggerPTMin} --Output root/OutputCentralityDown_R${R}_Centrality${C}.root
      ###//////////////////////////////////////////////////////////////////
      
      # Skip=false
      # if [[ "$C" == "0to10" ]]; then Skip=true; fi

      # ./Execute --Data Input/PbPbData_R${R}_Centrality${C}.root \
      #    --MC Input/PbPbMC_R${R}_CentralityInclusive.root \
      #    --JetMinPT ${TriggerPTMin} --Output pdf/Output_R${R}_Centrality${C}.pdf \
      #    --RootOutput root/Output_R${R}_Centrality${C}.root \
		#    --BinCount 100 --BinMagnification 10 --SkipFinalBin $Skip
   done
done
