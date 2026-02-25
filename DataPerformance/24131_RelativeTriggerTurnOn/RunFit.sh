#!/bin/bash

IsPP=1 # 0 = PbPb, 1 = pp
Prefix=PPData #PbPbData
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Prefix=PPData
   Centrality="Inclusive"
elif [[ "$IsPP" == "0" ]]; then
   Prefix=PbPbData
   #Centrality= 0to10 10to30 30to50 50to90 50to70 70to90
   Centrality=$(echo $Centrality | tr ' ' '\n' | grep -v Inclusive | tr '\n' ' ')
fi

for R in 1 2 3 4 6 7 8 9
#for R in 1
do
   #for C in 0to10 10to30 30to50 50to90 50to70 70to90
   for C in $Centrality
   #./ExecuteFit --Input Output/Result_PbPbData_R${R}_Centrality${C}.root --Output Plots/Plots_PbPbData_R${R}_Centrality${C}.pdf \
   #./ExecuteFit --Input Output/Result_PPData_R${R}_Centrality${C}.root --Output Plots/Plots_PPData_R${R}_Centrality${C}.pdf \

   #for C in Inclusive
   do
        ./ExecuteFit --Input Output/Result_${Prefix}_R${R}_Centrality${C}.root --Output Plots/Plots_${Prefix}_R${R}_Centrality${C}.pdf \
         --DHFile GlobalSetting.dh --State TriggerTurnOn --KeyPrefix R${R}_Centrality${C} \
         --FunctionChoice 1 --CutPercentage 0.99
   done
done
