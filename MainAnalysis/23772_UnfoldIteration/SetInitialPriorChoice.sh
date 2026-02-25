#!/bin/bash

#JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
JetR="1 2 3 4 6 7 8 9"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`

for R in $JetR
do
   for C in $Centrality
   do
      DHSet GlobalSetting.dh Prior PbPbData_R${R}_Centrality${C}_DefaultStep1 string Power50
      DHSet GlobalSetting.dh Prior PbPbMCRho_R${R}_Centrality${C}_DefaultStep1 string Power50
      DHSet GlobalSetting.dh Prior PbPbData_R${R}_Centrality${C}_AlternateStep1 string Power40
      DHSet GlobalSetting.dh Prior PbPbMCRho_R${R}_Centrality${C}_AlternateStep1 string Power40
   done
      
   DHSet GlobalSetting.dh Prior PPData_R${R}_CentralityInclusive_DefaultStep1 string Power50
   DHSet GlobalSetting.dh Prior PPMC_R${R}_CentralityInclusive_DefaultStep1 string Power50
   DHSet GlobalSetting.dh Prior PPData_R${R}_CentralityInclusive_AlternateStep1 string Power40
   DHSet GlobalSetting.dh Prior PPMC_R${R}_CentralityInclusive_AlternateStep1 string Power40
done
