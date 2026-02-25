#!/bin/bash

Location=$1
Prefix=$2
Suffix=$3
IsPP=$4
PriorChoice=$5

#JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR="1 2 3 4 6 7 8 9"; \
JetR="4"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Centrality="Inclusive"
fi

for R in $JetR
do
   for C in $Centrality
   do
      Prior=`DHQuery GlobalSetting.dh Prior ${Prefix}_R${R}_Centrality${C}_${PriorChoice} | tr -d '"'`
      # If not found in DHFile, set directly
      if [[ "$Prior" == "" ]]; then
         Prior=$PriorChoice
      fi
      # If no input, resort to Power50
      if [[ "$PriorChoice" == "" ]]; then
         PriorChoice=Power50
         Prior=Power50
      fi

      PriorString=$Prior
      PriorExtra=""
      if [[ "$Prior" == "Power40" ]]; then
         PriorString="Power"
         PriorExtra="--PriorK -4.0"
      elif [[ "$Prior" == "Power50" ]]; then
         PriorString="Power"
         PriorExtra="--PriorK -5.0"
      elif [[ "$Prior" == "Power60" ]]; then
         PriorString="Power"
         PriorExtra="--PriorK -6.0"
      elif [[ "$Prior" == "Power70" ]]; then
         PriorString="Power"
         PriorExtra="--PriorK -7.0"
      elif [[ "$Prior" == "ExternalMC" ]]; then
         PriorString="ExternalMC"
         PriorExtra="--ExternalPriorFile Input/${Prefix}_R${R}_Centrality${C}_Nominal.root"
      elif [[ "$Prior" == "External" ]]; then
         PriorString="External"
         # Use nominal for now.  Can upgrade later if needed
         PriorFile=Output_R4/`DHQuery GlobalSetting.dh Prior ${Prefix}_R${R}_Centrality${C}_${PriorChoice}_File | tr -d '"'`
         PriorHistogram=`DHQuery GlobalSetting.dh Prior ${Prefix}_R${R}_Centrality${C}_${PriorChoice}_Histogram | tr -d '"'`
         PriorExtra="--ExternalPriorFile $PriorFile --ExternalPriorHistogram $PriorHistogram"
      fi

      DoToyError=false
      # if [[ "$Suffix" == "Nominal" ]] && [[ "$PriorChoice" == "Nominal" ]]; then
      #    DoToyError=true
      # fi
      DoFakeRate=false
      echo "Unfolding now with R$R, Centrality $C, Prefix $Prefix, Suffix $Suffix and prior $Prior ($PriorChoice)"
      # echo Input file = $Location/${Prefix}_R${R}_Centrality${C}_${Suffix}.root

      Ignore=`DHQuery GlobalSetting.dh Binning PTUnderflow_R${R}_Centrality${C}`

      OutputSuffix="${PriorChoice}Prior"

      ./Execute --Input $Location/${Prefix}_R${R}_Centrality${C}_${Suffix}.root \
         --Output Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_${OutputSuffix}.root \
         --Prior $PriorString $PriorExtra --DoToyError $DoToyError \
         --DoBayes true --DoSVD false --DoInvert true --DoTUnfold true --DoFit true --DoFakeRate false
         # --FoldNormalize true --Ignore $Ignore --DoToyError $DoToyError
      ./ExecutePlot --Input Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_${OutputSuffix}.root \
         --Output Plots_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_${OutputSuffix}.pdf \
         --Ignore $Ignore
   done

done



