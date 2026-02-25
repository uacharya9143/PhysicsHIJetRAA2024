#!/bin/sh

Prefix=$1
Suffix=$2
StateSuffix=$3
PriorChoice=$4
IsPP=$5

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
      Ignore=`DHQuery GlobalSetting.dh Binning PTUnderflow_R${R}_Centrality${C}`

      #Ignore=10
      #IgnoreReco=25
      IgnoreReco=`DHQuery GlobalSetting.dh Binning PTUnderflow_R${R}_Centrality${C}`

      Prior=${PriorChoice}Prior

	   ./Execute --Input Input/${Prefix}_R${R}_Centrality${C}_${Suffix}_${Prior}.root \
         --Output Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_${Prior}.pdf \
         --Ignore ${Ignore} --IgnoreReco ${IgnoreReco} --Reference HMCTruth \
         --State Iterations --Key ${Prefix}_R${R}_Centrality${C}_${StateSuffix}_${Prior}
   done
done


