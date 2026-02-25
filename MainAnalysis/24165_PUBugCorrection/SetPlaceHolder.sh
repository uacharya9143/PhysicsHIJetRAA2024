#!/bin/bash

#JetR=`DHQuery GlobalSetting.dh Global JetR`
JetR="1 2 3 4 6 7 8 9"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality`

DHRemove GlobalSetting.dh PUBugCorrection
for R in $JetR
do
   for C in $Centrality
   do
      DHSet GlobalSetting.dh PUBugCorrection PbPbData_R${R}_Centrality${C}   float 0.95
   done
done

for R in $JetR
do
   DHSet GlobalSetting.dh PUBugCorrection PPData_R${R}_CentralityInclusive float 0.95
done




