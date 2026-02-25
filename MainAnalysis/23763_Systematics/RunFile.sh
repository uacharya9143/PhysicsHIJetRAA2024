#!/bin/bash

Prefix=$1
IsPP=$2

#JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR="1 2 3 4 6 7 8 9"; \
JetR="4"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Centrality="Inclusive"
fi

MCPrefix=$1
if [[ "$Prefix" == "PbPbData" ]]; then
   MCPrefix="PbPbMCRho"
else
   MCPrefix="PPMC"
fi

for R in $JetR
do
   for C in $Centrality
   do
      MCNP="`DHQuery GlobalSetting.dh PriorToUse ${MCPrefix}_R${R}_Centrality${C}_Default | tr -d '"'`Prior"
      NP="`DHQuery GlobalSetting.dh PriorToUse ${Prefix}_R${R}_Centrality${C}_Default | tr -d '"'`Prior"
      #Prior PPData_R7_CentralityInclusive_Default =DefaulrStep1"+Prior
      AP="`DHQuery GlobalSetting.dh PriorToUse ${Prefix}_R${R}_Centrality${C}_Alternate | tr -d '"'`Prior"
      #echo MCNP "$MCNP" " " NP  "$NP" " AP  " "$AP"
      RC="R${R}_Centrality${C}"
      PRC="${Prefix}_${RC}"
      PRCN="Input/${PRC}_Nominal_${NP}.root"
      #echo PRC "$PRC"
      echo PRCN "$PRCN"
      MCPRC="${MCPrefix}_${RC}"
      MCPRCN="Input/${MCPRC}_Nominal_${MCNP}.root"
      MCPRCToy="Input/${MCPRC}_Nominal_Toy_${MCNP}.root"
      echo PRCN "$MCPRCToy"
      Method=`DHQuery GlobalSetting.dh MethodToUse ${Prefix}_R${R}_Centrality${C}_Default | tr -d '"'`
      #echo Method "$Method"
      if [[ "$Method" == "Bayes" ]]; then
         NominalIteration=`DHQuery GlobalSetting.dh Iterations ${Prefix}_R${R}_Centrality${C}_Nominal_${NP}`
         HNominal=HUnfoldedBayes${NominalIteration}
	      #echo Method "$Method" " "  NominalIteration .....${NominalIteration}...."$HNominal"
	      echo "NominalIteration: $NominalIteration"
	      echo "DHQuery GlobalSetting.dh Iterations ${Prefix}_R${R}_Centrality${C}_Nominal_${NP}"
         MCIteration=`DHQuery GlobalSetting.dh Iterations ${MCPrefix}_R${R}_Centrality${C}_Nominal_${MCNP}`
         HMCNominal=HUnfoldedBayes${MCIteration}

         PriorIteration=`DHQuery GlobalSetting.dh Iterations ${Prefix}_R${R}_Centrality${C}_Nominal_${AP}`
         HPrior=HUnfoldedBayes${PriorIteration}
	      echo " PriorIteration: $PriorIteration"
         IterationUp=`./ExecuteIteration --Input ${PRCN} --Mode Next --Iteration $NominalIteration`
         IterationDown=`./ExecuteIteration --Input ${PRCN} --Mode Previous --Iteration $NominalIteration`
         HIterationUp=HUnfoldedBayes${IterationUp}
         HIterationDown=HUnfoldedBayes${IterationDown}
      elif [[ "$Method" == "TUnfold" ]]; then
         HNominal=HUnfoldedTUnfold
         HMCNominal=HUnfoldedTUnfold
         HPrior=HUnfoldedTUnfold
         HIterationUp=HUnfoldedTUnfold
         HIterationDown=HUnfoldedTUnfold
	 #echo Method "$Method" " "  NominalIteration ....."$HNominal" NominalIteration....

      fi

      State=$PRC

      Config=Config/$PRC.config
      rm -f $Config

      echo "$PRC JECUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC JECUp_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC JECUp_VariantFile      string  Input/${PRC}_JECUp_${NP}.root" >> $Config
      echo "$PRC JECUp_VariantHistogram string  ${HNominal}" >> $Config
      echo "$PRC JECUp_Label            string  JECUp" >> $Config
      echo "$PRC JECUp_Include          int     1" >> $Config
      echo "$PRC JECUp_Bridging         int     0" >> $Config
      echo "$PRC JECUp_ExtraScaling     float   1" >> $Config

      echo "$PRC JECDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC JECDown_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC JECDown_VariantFile      string  Input/${PRC}_JECDown_${NP}.root" >> $Config
      echo "$PRC JECDown_VariantHistogram string  ${HNominal}" >> $Config
      echo "$PRC JECDown_Label            string  JECDown" >> $Config
      echo "$PRC JECDown_Include          int     1" >> $Config
      echo "$PRC JECDown_Bridging         int     0" >> $Config
      echo "$PRC JECDown_ExtraScaling     float   1" >> $Config

#      echo "$PRC JECAggressiveUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC JECAggressiveUp_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC JECAggressiveUp_VariantFile      string  Input/${PRC}_JECAggressiveUp_${NP}.root" >> $Config
#      echo "$PRC JECAggressiveUp_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC JECAggressiveUp_Label            string  JECAggressiveUp" >> $Config
#      echo "$PRC JECAggressiveUp_Include          int     0" >> $Config
#      echo "$PRC JECAggressiveUp_Bridging         int     0" >> $Config
#      echo "$PRC JECAggressiveUp_ExtraScaling     float   1" >> $Config

#      echo "$PRC JECAggressiveDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC JECAggressiveDown_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC JECAggressiveDown_VariantFile      string  Input/${PRC}_JECAggressiveDown_${NP}.root" >> $Config
#      echo "$PRC JECAggressiveDown_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC JECAggressiveDown_Label            string  JECAggressiveDown" >> $Config
#      echo "$PRC JECAggressiveDown_Include          int     0" >> $Config
#      echo "$PRC JECAggressiveDown_Bridging         int     0" >> $Config
#      echo "$PRC JECAggressiveDown_ExtraScaling     float   1" >> $Config
      
      # If we want to use full pp uncertainty but aggressive PbPb
      # if [[ "$IsPP" == 1 ]]; then
      #    echo "$PRC JECAggressiveUp_VariantFile    string  Input/${PRC}_JECUp_${NP}.root" >> $Config
      #    echo "$PRC JECAggressiveDown_VariantFile  string  Input/${PRC}_JECDown_${NP}.root" >> $Config
      # fi
      
 #     echo "$PRC JECQuench_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
 #     echo "$PRC JECQuench_BaseHistogram    string  ${HNominal}" >> $Config
 #     echo "$PRC JECQuench_VariantFile      string  Input/${PRC}_JECQuench_${NP}.root" >> $Config
 #     echo "$PRC JECQuench_VariantHistogram string  ${HNominal}" >> $Config
 #     echo "$PRC JECQuench_Label            string  JECQuench" >> $Config
 #     echo "$PRC JECQuench_Include          int     0" >> $Config
 #     echo "$PRC JECQuench_Bridging         int     0" >> $Config
 #     echo "$PRC JECQuench_ExtraScaling     float   1" >> $Config

      echo "$PRC JERUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC JERUp_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC JERUp_VariantFile      string  Input/${PRC}_JERUp_${NP}.root" >> $Config
      echo "$PRC JERUp_VariantHistogram string  ${HNominal}" >> $Config
      echo "$PRC JERUp_Label            string  JERUp" >> $Config
      echo "$PRC JERUp_Include          int     1" >> $Config
      echo "$PRC JERUp_Bridging         int     0" >> $Config
      echo "$PRC JERUp_ExtraScaling     float   1" >> $Config

      echo "$PRC JERDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC JERDown_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC JERDown_VariantFile      string  Input/${PRC}_JERDown_${NP}.root" >> $Config
      echo "$PRC JERDown_VariantHistogram string  ${HNominal}" >> $Config
      echo "$PRC JERDown_Label            string  JERDown" >> $Config
      echo "$PRC JERDown_Include          int     1" >> $Config
      echo "$PRC JERDown_Bridging         int     0" >> $Config
      echo "$PRC JERDown_ExtraScaling     float   1" >> $Config

      echo "$PRC IterationUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC IterationUp_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC IterationUp_VariantFile      string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC IterationUp_VariantHistogram string  ${HIterationUp}" >> $Config
      echo "$PRC IterationUp_Label            string  IterationUp" >> $Config
      echo "$PRC IterationUp_Include          int     0" >> $Config
      echo "$PRC IterationUp_Bridging         int     0" >> $Config
      echo "$PRC IterationUp_ExtraScaling     float   1" >> $Config

      echo "$PRC IterationDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC IterationDown_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC IterationDown_VariantFile      string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC IterationDown_VariantHistogram string  ${HIterationDown}" >> $Config
      echo "$PRC IterationDown_Label            string  IterationDown" >> $Config
      echo "$PRC IterationDown_Include          int     0" >> $Config
      echo "$PRC IterationDown_Bridging         int     0" >> $Config
      echo "$PRC IterationDown_ExtraScaling     float   1" >> $Config

      echo "$PRC Prior_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
      echo "$PRC Prior_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC Prior_VariantFile      string  Input/${PRC}_Nominal_${AP}.root" >> $Config
      echo "$PRC Prior_VariantHistogram string  ${HPrior}" >> $Config
      echo "$PRC Prior_Label            string  Prior" >> $Config
      echo "$PRC Prior_Include          int     0" >> $Config
      echo "$PRC Prior_Bridging         int     0" >> $Config
      echo "$PRC Prior_ExtraScaling     float   1" >> $Config

      echo "$PRC Iteration_BaseFile         string  Input/${MCPRC}_Nominal_Toy_${MCNP}.root" >> $Config
      echo "$PRC Iteration_BaseHistogram    string  ${HNominal}" >> $Config
      echo "$PRC Iteration_VariantFile      string  Input/${MCPRC}_Nominal_Toy_${MCNP}.root" >> $Config
      echo "$PRC Iteration_VariantHistogram string  ${HMCNominal}" >> $Config
      echo "$PRC Iteration_Label            string  Iteration" >> $Config
      echo "$PRC Iteration_Include          int     1" >> $Config
      echo "$PRC Iteration_Bridging         int     0" >> $Config
      echo "$PRC Iteration_ExtraScaling     float   1" >> $Config

#      echo "$PRC CentralityUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC CentralityUp_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC CentralityUp_VariantFile      string  Input/${PRC}_CentralityUp_${NP}.root" >> $Config
#      echo "$PRC CentralityUp_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC CentralityUp_Label            string  CentralityUp" >> $Config
#      echo "$PRC CentralityUp_Include          int     1" >> $Config
#      echo "$PRC CentralityUp_Bridging         int     0" >> $Config
#      echo "$PRC CentralityUp_ExtraScaling     float   1" >> $Config

#      echo "$PRC CentralityDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC CentralityDown_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC CentralityDown_VariantFile      string  Input/${PRC}_CentralityDown_${NP}.root" >> $Config
#      echo "$PRC CentralityDown_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC CentralityDown_Label            string  CentralityDown" >> $Config
#      echo "$PRC CentralityDown_Include          int     1" >> $Config
#      echo "$PRC CentralityDown_Bridging         int     0" >> $Config
#      echo "$PRC CentralityDown_ExtraScaling     float   1" >> $Config

#      echo "$PRC BinBiasUp_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC BinBiasUp_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC BinBiasUp_VariantFile      string  Input/${PRC}_NominalBinBiasUp_${NP}.root" >> $Config
#      echo "$PRC BinBiasUp_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC BinBiasUp_Label            string  BinBiasUp" >> $Config
#      echo "$PRC BinBiasUp_Include          int     1" >> $Config
#      echo "$PRC BinBiasUp_Bridging         int     0" >> $Config
#      echo "$PRC BinBiasUp_ExtraScaling     float   1" >> $Config

#      echo "$PRC BinBiasDown_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC BinBiasDown_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC BinBiasDown_VariantFile      string  Input/${PRC}_NominalBinBiasDown_${NP}.root" >> $Config
#      echo "$PRC BinBiasDown_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC BinBiasDown_Label            string  BinBiasDown" >> $Config
#      echo "$PRC BinBiasDown_Include          int     1" >> $Config
#      echo "$PRC BinBiasDown_Bridging         int     0" >> $Config
#      echo "$PRC BinBiasDown_ExtraScaling     float   1" >> $Config

#      echo "$PRC Earthquake_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC Earthquake_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC Earthquake_VariantFile      string  Input/${PRC}_NominalEarthquake_${NP}.root" >> $Config
#      echo "$PRC Earthquake_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC Earthquake_Label            string  Earthquake" >> $Config
#      echo "$PRC Earthquake_Include          int     1" >> $Config
#      echo "$PRC Earthquake_Bridging         int     0" >> $Config
#      echo "$PRC Earthquake_ExtraScaling     float   1" >> $Config

#      echo "$PRC Cleaned_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC Cleaned_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC Cleaned_VariantFile      string  Input/${PRC}_NominalCleaned_${NP}.root" >> $Config
#      echo "$PRC Cleaned_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC Cleaned_Label            string  Cleaned" >> $Config
#      echo "$PRC Cleaned_Include          int     0" >> $Config
#      echo "$PRC Cleaned_Bridging         int     0" >> $Config
#      echo "$PRC Cleaned_ExtraScaling     float   1" >> $Config

#      echo "$PRC CleanedStrong_BaseFile         string  Input/${PRC}_Nominal_${NP}.root" >> $Config
#      echo "$PRC CleanedStrong_BaseHistogram    string  ${HNominal}" >> $Config
#      echo "$PRC CleanedStrong_VariantFile      string  Input/${PRC}_NominalCleanedStrong_${NP}.root" >> $Config
#      echo "$PRC CleanedStrong_VariantHistogram string  ${HNominal}" >> $Config
#      echo "$PRC CleanedStrong_Label            string  CleanedStrong" >> $Config
#      echo "$PRC CleanedStrong_Include          int     0" >> $Config
#      echo "$PRC CleanedStrong_Bridging         int     0" >> $Config
#      echo "$PRC CleanedStrong_ExtraScaling     float   1" >> $Config

      if [[ $IsPP == 0 ]]; then
         TAA=`DHQuery GlobalSetting.dh TAA $C`
         TAAError=`DHQuery GlobalSetting.dh TAAErrorUp $C`
         #TAARelativeError=`echo $TAAError | DivideConst $TAA`
	 TAARelativeError=$(echo "scale=4; $TAAError / $TAA" | bc -l)
         echo "$PRC Global_TAA float $TAARelativeError" >> $Config
         echo "$PRC Global_NMB float 0.0126" >> $Config
         echo "$PRC Global_PbPbSelection float 0.01" >> $Config
         echo "$PRC Global_PUBug float 0.04" >> $Config
      else
         echo "$PRC Global_Luminosity float 0.019" >> $Config
         echo "$PRC Global_PPSelection float 0.01" >> $Config
      fi
      echo Method "$Method"
      echo "NominalIteration: $NominalIteration"
      echo "$PRC"
      DHRemove Systematics.dh $PRC
      echo Congiffff "$Config"
      DHSet Systematics.dh $Config

      ./Execute \
         --ReadFromDHFile true \
         --DHFile Systematics.dh --State $PRC \
         --BinMapping BinMapping/${PRC}_Nominal.root \
         --DoSelfNormalize false \
         --Output Output_R4/${PRC}.root
      
      ./ExecutePlot \
         --Input Output_R4/${PRC}.root --Output Plot_R4/${PRC}.pdf \
         --Variations HJECUp,HJECDown,HJERUp,HJERDown,HIterationUp,HIterationDown \
         --Labels "JEC Up","JEC Down","JER Up","JER Down","Iteration Up","Iteration Down"
#         --Variations HJECUp,HJECDown,HJERUp,HJERDown,HIterationUp,HIterationDown,HPrior,HIteration,HCentralityDown,HCentralityDown,HJECQuench \
#         --Labels "JEC Up","JEC Down","JER Up","JER Down","Iteration Up","Iteration Down","Prior","Iteration","Centrality Up","Centrality Down","Quench"
   done
done


