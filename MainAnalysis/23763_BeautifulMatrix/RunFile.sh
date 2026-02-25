
Prefix=$1
Suffix=$2
IsPP=$3

#JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
JetR="1 2 3 4 6 7 8 9"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Centrality="Inclusive"
fi

if [[ "$Suffix" != "" ]]; then
   Suffix="_$Suffix"
fi

for R in $JetR
do
   for C in $Centrality
   do
      RValue=`DHQuery GlobalSetting.dh JetR $R`

      State=PbPbData
      if [[ "$IsPP" == "1" ]]; then
         State=PPData
      fi

      System="PbPb"
      if [[ "$IsPP" == "1" ]]; then
         System="pp"
      else
         System="PbPb"
      fi

      CentralityString=" "
      if [[ "$IsPP" == "1" ]]; then
         CentralityString=" "
      else
          #CentralityMin=`DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"' | MultiplyConst 100`
         #CentralityMax=`DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"' | MultiplyConst 100`
	  CentralityMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
          CentralityMin=$(echo "$(DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"')" \* 100 | bc)
	  
#CentralityMin=`DHQuery GlobalSetting.dh CentralityMin ${C} | tr -d '"'* 100`
         #CentralityMax=`DHQuery GlobalSetting.dh CentralityMax ${C} | tr -d '"' * 100`

	  
         CentralityString="Centrality ${CentralityMin}%-${CentralityMax}%"
      fi

      PRC="${Prefix}_R${R}_Centrality${C}"
      NP="`DHQuery GlobalSetting.dh PriorToUse ${PRC}_Default | tr -d '"'`Prior"
      PRCN="${PRC}_Nominal${Suffix}_${NP}.root"

      ./Execute \
         --Input Input/${PRCN} \
         --Output Plots/QualityPlots_${PRC}${Suffix}.pdf --FinalOutput Plots/${PRC}${Suffix}.pdf \
         --UseActualZMax true \
         --WorldXMin 80 --WorldXMax 1500 --LogX true \
         --WorldYMin 80 --WorldYMax 1500 --LogY true \
         --WorldZMin 0.0001 --WorldZMax 0.1 --LogZ true \
         --GenPrimaryMin 80 --GenPrimaryMax 1500 --RecoPrimaryMin 80 --RecoPrimaryMax 1500 \
         --Primary "Jet p_{T} (GeV)" --Binning "None" --RowNormalize true \
         --XAxis 303 --YAxis 303 \
         --GroupX 1 --GroupY 1 --Row 1 --Column 1
   done
done
