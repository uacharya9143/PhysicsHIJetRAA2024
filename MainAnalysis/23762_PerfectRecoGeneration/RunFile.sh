#!/bin/bash

Prefix=$1
Suffix=$2
IsPP=$3
IterationPrefix=$4
PriorChoice=$5
IterationPriorChoice=$6

if [[ "$IterationPriorChoice" == "" ]]; then
   IterationPriorChoice=$PriorChoice
fi

#JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR="1 2 3 4 6 7 8 9"; \
JetR="4"; \

Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Centrality="Inclusive"
fi

for R in $JetR
do
   echo Running R $R
      
   for C in $Centrality
   do
      Prior=${PriorChoice}Prior
      IterationPrior=${IterationPriorChoice}Prior

      GenHistogram=HMCGen
      GenFile=
      OutputFile=
      if [[ "$IterationPrefix" == "" ]]; then
         GenHistogram=HMCGen
         GenFile=Input/${Prefix}_R${R}_Centrality${C}_${Suffix}.root
         OutputFile=Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_PerfectReco.root
      else
         Iteration=`DHQuery GlobalSetting.dh Iterations ${IterationPrefix}_R${R}_Centrality${C}_${Suffix}_${IterationPrior}`
         GenHistogram=HUnfoldedBayes${Iteration}
         GenFile=UnfoldedInput/${Prefix}_R${R}_Centrality${C}_${Suffix}_${Prior}.root
         OutputFile=Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}_${Prior}_PerfectReco.root
      fi

      ./Execute \
         --MC           Input/${Prefix}_R${R}_Centrality${C}_${Suffix}.root \
         --Yield        Input/${Prefix}_R${R}_Centrality${C}_${Suffix}.root \
         --Gen          $GenFile \
         --GenHistogram $GenHistogram \
         --Output       $OutputFile
   done
done

