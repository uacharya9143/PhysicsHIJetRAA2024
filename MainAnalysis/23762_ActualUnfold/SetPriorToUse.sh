#!/bin/bash

JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`

MCStep=$1
DataStep=$2
Method=$3

for R in $JetR
do
   #for C in $Centrality
  # do
  #    DHSet GlobalSetting.dh PriorToUse PbPbMCRho_R${R}_Centrality${C}_Default    string $MCStep
  #    DHSet GlobalSetting.dh PriorToUse PbPbData_R${R}_Centrality${C}_Default     string $DataStep
  #    DHSet GlobalSetting.dh PriorToUse PbPbMCRho_R${R}_Centrality${C}_Alternate  string $MCStep
  #    DHSet GlobalSetting.dh PriorToUse PbPbData_R${R}_Centrality${C}_Alternate   string $DataStep

   #   DHSet GlobalSetting.dh MethodToUse PbPbMCRho_R${R}_Centrality${C}_Default   string ${Method}
  #    DHSet GlobalSetting.dh MethodToUse PbPbData_R${R}_Centrality${C}_Default    string ${Method}
   #   DHSet GlobalSetting.dh MethodToUse PbPbMCRho_R${R}_Centrality${C}_Alternate string ${Method}
   #   DHSet GlobalSetting.dh MethodToUse PbPbData_R${R}_Centrality${C}_Alternate  string ${Method}
   #done
      
   DHSet GlobalSetting.dh PriorToUse PPMC_R${R}_CentralityInclusive_Default      string $MCStep
   DHSet GlobalSetting.dh PriorToUse PPData_R${R}_CentralityInclusive_Default    string $DataStep
   DHSet GlobalSetting.dh PriorToUse PPMC_R${R}_CentralityInclusive_Alternate    string $MCStep
   DHSet GlobalSetting.dh PriorToUse PPData_R${R}_CentralityInclusive_Alternate  string $DataStep
   
   DHSet GlobalSetting.dh MethodToUse PPMC_R${R}_CentralityInclusive_Default     string ${Method}
   DHSet GlobalSetting.dh MethodToUse PPData_R${R}_CentralityInclusive_Default   string ${Method}
   DHSet GlobalSetting.dh MethodToUse PPMC_R${R}_CentralityInclusive_Alternate   string ${Method}
   DHSet GlobalSetting.dh MethodToUse PPData_R${R}_CentralityInclusive_Alternate string ${Method}
done

