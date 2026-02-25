#!/bin/bash

#JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
JetR="1 2 3 4 6 7 8 9"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`

#PPVariations="HJECUp,HJECDown,HJECAggressiveUp,HJECAggressiveDown,HJERUp,HJERDown,HIteration,HPrior,HEarthquake,HBinBiasUp,HBinBiasDown"
#PPSystematicGroups="1,1,1,1,2,2,3,3,4,4,4"
#PPLabels="JES,JER,Unfolding,Matrix"
#AAVariations="HJECUp,HJECDown,HJECAggressiveUp,HJECAggressiveDown,HJERUp,HJERDown,HIteration,HPrior,HCentralityUp,HCentralityDown,HJECQuench,HEarthquake,HBinBiasUp,HBinBiasDown"
#AASystematicGroups="1,1,1,1,2,2,3,3,5,5,4,6,6,6"
#AALabels="JES,JER,Unfolding,Quench,Centrality,Matrix"

PPVariations="HJECUp,HJECDown,HJERUp,HJERDown,HIteration"
PPSystematicGroups="1,1,2,2,3"
PPLabels="JES,JER,Unfolding"
AAVariations="HJECUp,HJECDown,HJERUp,HJERDown,HIteration"
AASystematicGroups="1,1,2,2,3"
AALabels="JES,JER,Unfolding"

# Run pp spectrum
echo Running pp spectrum plots
for R in $JetR
do
   RValue=`DHQuery GlobalSetting.dh JetR $R`
   LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_CentralityInclusive_Cut`

   ./Execute --Input Systematics/PPData_R${R}_CentralityInclusive.root \
      --Output Plots/PPData_R${R}_CentralityInclusive.pdf \
      --FinalOutput FinalPlots/PPData_R${R}_CentralityInclusive.pdf \
      --Global Systematics.dh \
      --GenPrimaryMin 0 --GenPrimaryMax 1600 \
      --WorldXMin $LowerBound \
      --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
      --LogY false --LogX true \
      --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
      --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
      --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
      --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0" \
      --Group 1 --Row 1 --Column 1 \
      --Variations ${PPVariations} \
      --SystematicGroups ${PPSystematicGroups} \
      --Labels ${PPLabels}
done
#        --WorldXMin $LowerBound \

# Run PbPb spectrum
echo Running PbPb spectrum plots
for R in $JetR
do
   for C in $Centrality
   do
      RValue=`DHQuery GlobalSetting.dh JetR $R`
      LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_Cut`
      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`

      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`
      CMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
      CMax=$(echo "$(DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"')" \* 100 | bc)
      CLabel="${CMin}-${CMax}%"
      
      ./Execute --Input Systematics/PbPbData_R${R}_Centrality${C}.root \
         --Output Plots/PbPbData_R${R}_Centrality${C}.pdf \
         --FinalOutput FinalPlots/PbPbData_R${R}_Centrality${C}.pdf \
         --Global Systematics.dh \
         --GenPrimaryMin 0 --GenPrimaryMax 1600 \
         --WorldXMin $LowerBound \
	      --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
	      --LogY false --LogX true \
         --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
         --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
         --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
         --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0",0,0.12,0.78,"$CLabel" \
         --Group 1 --Row 1 --Column 1 \
         --Variations ${AAVariations} \
         --SystematicGroups ${AASystematicGroups} \
         --Labels ${AALabels}
   done
done

# Run pp spectra ratio
echo Running pp spectra ratio plots
for R in $JetR
do
   RValue=`DHQuery GlobalSetting.dh JetR $R`
   LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R9_CentralityInclusive_Cut`
   
   ./Execute --Input CombinedSystematics/PPDataRatio_R${R}R9_CentralityInclusive.root \
      --Output Plots/PPDataRatio_R${R}R9_CentralityInclusive.pdf \
      --FinalOutput FinalPlots/PPDataRatio_R${R}R9_CentralityInclusive.pdf \
      --Global Systematics.dh \
      --GenPrimaryMin 0 --GenPrimaryMax 1600 \
      --WorldXMin $LowerBound \
      --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
      --LogY false --LogX true \
      --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
      --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
      --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
      --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0" \
      --Group 1 --Row 1 --Column 1 \
      --Variations ${PPVariations} \
      --SystematicGroups ${PPSystematicGroups} \
      --Labels ${PPLabels}
done

# Run RAA
echo Running RAA plots
for R in $JetR
do
   for C in $Centrality
   do
      RValue=`DHQuery GlobalSetting.dh JetR $R`
      LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_Cut`
      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`

      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`
      
      CMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
      CMax=$(echo "$(DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"')" \* 100 | bc)
      CLabel="${CMin}-${CMax}%"
   
      ./Execute --Input CombinedSystematics/RAA_R${R}_Centrality${C}.root \
         --Output Plots/RAA_R${R}_Centrality${C}.pdf \
         --FinalOutput FinalPlots/RAA_R${R}_Centrality${C}.pdf \
         --Global Systematics.dh \
         --GenPrimaryMin 0 --GenPrimaryMax 1600 \
         --WorldXMin $LowerBound \
	 --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
	 --LogY false --LogX true \
         --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
         --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
         --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
         --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0",0,0.12,0.78,"$CLabel" \
         --Group 1 --Row 1 --Column 1 \
         --Variations ${AAVariations} \
         --SystematicGroups ${AASystematicGroups} \
         --Labels ${AALabels}
   done
done

# Run RCP
echo Running RCP plots
for R in $JetR
do
   for C in $Centrality
   do
      RValue=`DHQuery GlobalSetting.dh JetR $R`
      LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_Cut`
      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`

      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`
      CMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
      CMax=$(echo "$(DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"')" \* 100 | bc)
      CLabel="${CMin}-${CMax}%"
   
      ./Execute --Input CombinedSystematics/RCP_R${R}_Centrality${C}.root \
         --Output Plots/RCP_R${R}_Centrality${C}.pdf \
         --FinalOutput FinalPlots/RCP_R${R}_Centrality${C}.pdf \
         --Global Systematics.dh \
         --GenPrimaryMin 0 --GenPrimaryMax 1600 \
         --WorldXMin $LowerBound \
	 --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
	 --LogY false --LogX true \
         --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
         --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
         --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
         --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0",0,0.12,0.78,"$CLabel" \
         --Group 1 --Row 1 --Column 1 \
         --Variations ${AAVariations} \
         --SystematicGroups ${AASystematicGroups} \
         --Labels ${AALabels}
   done
done

# Run RRAA
echo Running RRAA plots
for R in $JetR
do
   for C in $Centrality
   do
      RValue=`DHQuery GlobalSetting.dh JetR $R`
      LowerBound=`DHQuery GlobalSetting.dh TriggerTurnOn R${R}_Centrality${C}_Cut`
      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`

      #CMax=`DHQuery GlobalSetting.dh CentralityMax $C | MultiplyConst 100`
      CMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
      CMax=$(echo "$(DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"')" \* 100 | bc)
      CLabel="${CMin}-${CMax}%"
      
      ./Execute --Input CombinedSystematics/RRAA_R${R}R1_Centrality${C}.root \
         --Output Plots/RRAA_R${R}R1_Centrality${C}.pdf \
         --FinalOutput FinalPlots/RRAA_R${R}R1_Centrality${C}.pdf \
         --Global Systematics.dh \
         --GenPrimaryMin 0 --GenPrimaryMax 1600 \
         --WorldXMin $LowerBound \
	 --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1.0 \
	 --LogY false --LogX true \
         --XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
         --LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
         --XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
         --Texts 0,0.12,0.88,"Anti-k_{T} jet R = $RValue",0,0.12,0.83,"|#eta| < 2.0",0,0.12,0.78,"$CLabel" \
         --Group 1 --Row 1 --Column 1 \
         --Variations ${AAVariations} \
         --SystematicGroups ${AASystematicGroups} \
         --Labels ${AALabels}
   done
done





