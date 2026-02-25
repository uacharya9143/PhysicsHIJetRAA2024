#!/bin/bash
set -euo pipefail
Prefix=$1
Type=$2
Suffix=$3
IsPP=$4
Tier=$5

num_or_default() {
  # Usage: num_or_default "<cmd>" <default>
  local cmd="$1" def="$2" out
  # shellcheck disable=SC2086
  out=$(eval "$cmd" | tr -d '"' | xargs)
  if [[ -z "$out" ]]; then
    echo "$def"
  else
    echo "$out"
  fi
}

bc_calc() {
  # Safe bc with scale, guarding empty inputs
  # Usage: bc_calc "<expr>"
  echo "$1" | bc
}

#JetR=$(DHQuery GlobalSetting.dh Global JetR | sed 's/"//g')
#JetR="1 2 3 4 6 7 8 9"
JetR="4"
Centrality=$(DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g')
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
      RValue=$(DHQuery GlobalSetting.dh JetR "$R")

      State=PbPbData
      if [[ "$IsPP" == "1" ]]; then
         State=PPData
      fi

      Luminosity=1
      LuminosityUnit="#mub^{-1}"
      ExtraScale=0.25 # coming from |eta| going from -2. to +2. // ==1/d(eta)
      
      if [[ "$IsPP" == "1" ]]; then
          Luminosity=$(num_or_default "DHQuery GlobalSetting.dh Lumi ${State}_R${R}_Centrality${C}_BRIL" "0")
          
          # Validate Luminosity before printf
          if [[ -z "$Luminosity" ]] || [[ ! "$Luminosity" =~ ^[0-9.]+$ ]]; then
              Luminosity="0"
          fi
          Luminosity=$(printf "%.6f" "$Luminosity")

          LuminosityUnit="#mub^{-1}"
          echo "PP Case, ExtraScale before potential division by Luminosity: $Luminosity, $ExtraScale"
          
          if [[ "$Luminosity" == "0" ]]; then
              ExtraScale=$(echo "scale=15; ${ExtraScale}" | bc)
          else
              ExtraScale=$(echo "scale=15; ${ExtraScale} / ${Luminosity}" | bc)
          fi
      else
          echo "PbPb Case, Initial ExtraScale: $ExtraScale"
          Luminosity=$(num_or_default "DHQuery GlobalSetting.dh Lumi ${State}_R${R}_Centrality${C}_BRIL" "0")
          MBCount=$(num_or_default "DHQuery GlobalSetting.dh MBCount ${State}_R${R}_Centrality${C}_OfficialCount" "0")
          
          # Validate MBCount before printf
          if [[ -z "$MBCount" ]] || [[ ! "$MBCount" =~ ^[0-9.]+$ ]]; then
              MBCount="0"
          fi
          MBCount=$(printf "%.6f" "$MBCount")
          
          TAA=$(num_or_default "DHQuery GlobalSetting.dh TAA ${C}" "0")
          
          # Validate TAA before printf
          if [[ -z "$TAA" ]] || [[ ! "$TAA" =~ ^[0-9.]+$ ]]; then
              TAA="0"
          fi
          TAA=$(printf "%.6f" "$TAA")
          TAA=$(echo "scale=6; ${TAA} / 1000" | bc)

          echo "Before Division_afterConversion = Lumi=$Luminosity, MB=$MBCount, TAA=$TAA, ExtraScale=$ExtraScale"
          
          # Guard potential zeros to avoid bc errors
          if [[ -z "$MBCount" || -z "$TAA" || "$MBCount" == "0" || "$TAA" == "0" ]]; then
              echo "Error: Division by zero detected for MBCount or TAA."
              continue  # Skip to next iteration
          else
              ExtraScale=$(echo "scale=15; ${ExtraScale} / ${MBCount} / ${TAA}" | bc)
          fi
          
          echo "After Division = Lumi=$Luminosity, MB=$MBCount, TAA=$TAA, ExtraScale=$ExtraScale"
      fi
      
      PUBugCorrection=$(num_or_default "DHQuery GlobalSetting.dh PUBugCorrection ${State}_R${R}_Centrality${C}" "1")
      ExtraScale=$(echo "scale=15; ${ExtraScale} * ${PUBugCorrection}" | bc)

      EfficiencyCorrection=$(num_or_default "DHQuery GlobalSetting.dh EventSelection ${State}_R${R}_Centrality${C}" "1")
      ExtraScale=$(echo "scale=15; ${ExtraScale} * ${EfficiencyCorrection}" | bc)
      echo "After Division Efficiency and PUBugcorrection = PUBugCorrection=$PUBugCorrection, EfficiencyCorrection=$EfficiencyCorrection, ExtraScale=$ExtraScale"

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
          CMin=$(num_or_default "DHQuery GlobalSetting.dh CentralityMin ${C}" "0")
          CMax=$(num_or_default "DHQuery GlobalSetting.dh CentralityMax ${C}" "0")

          CentralityMin=$(bc_calc "${CMin} * 100")
          CentralityMax=$(bc_calc "${CMax} * 100")

          CentralityString="Centrality ${CentralityMin}%-${CentralityMax}%"
      fi

      YLabel="default"
      if [[ "$IsPP" == "1" ]]; then
         YLabel="d^{2}#sigma / dp_{T}d#eta"
      else
         YLabel="#frac{1}{<T_{AA}>}#frac{1}{N_{evt}}#frac{d^{2}N_{jet}}{dp_{T}d#eta}"
      fi

      NP="$(DHQuery GlobalSetting.dh PriorToUse "${Prefix}_R${R}_Centrality${C}_Default" | tr -d '"')Prior"
      PRC="${Prefix}_R${R}_Centrality${C}"
      PRCTS="${PRC}_${Type}${Suffix}"
      PRCN="${PRCTS}_${NP}.root"
      
      PRCTST=${PRCTS}
      if [[ "$Tier" != "Gen" ]]; then
         PRCTST=${PRCTS}_${Tier}
      fi

      Method=$(DHQuery GlobalSetting.dh MethodToUse "${State}_R${R}_Centrality${C}_Default" | tr -d '"')
      if [[ "$Tier" == "Gen" ]]; then
         if [[ "${Method}" == "Bayes" ]]; then
            HPrimary=HUnfoldedBayes$(DHQuery GlobalSetting.dh Iterations "${State}_R${R}_Centrality${C}_Nominal_${NP}")
         elif [[ "$Method" == "TUnfold" ]]; then
            HPrimary=HUnfoldedTUnfold
         fi
         Underflow=$(DHQuery GlobalSetting.dh Binning "PTUnderflow_R${R}_Centrality${C}")
         Overflow=$(DHQuery GlobalSetting.dh Binning "PTOverflow_R${R}_Centrality${C}")
      elif [[ "$Tier" == "Reco" ]]; then 
         if [[ "${Method}" == "Bayes" ]]; then
            HPrimary=HRefoldedBayes$(DHQuery GlobalSetting.dh Iterations "${State}_R${R}_Centrality${C}_Nominal_${NP}")
         elif [[ "$Method" == "TUnfold" ]]; then
            HPrimary=HRefoldedTUnfold
         fi
         Underflow=0
         Overflow=0
      elif [[ "$Tier" == "InputReco" ]]; then
         HPrimary=HInput
         Underflow=0
         Overflow=0
      elif [[ "$Tier" == "InputRecoGenBin" ]]; then
         HPrimary=HInputGenBin
         Underflow=0
         Overflow=0
      fi

      MCFile="Input/${PRCN}"
      MCHist="HMCTruth"
      MCLabel="Input"
      NormalizeMCToData="true"
      MCExtraScale="1"
      MCAbsoluteScale="true"

      if [[ "$Tier" == "Gen" ]]; then
         if [[ "$Suffix" == "_Toy" ]]; then
            MCFile="Input/${PRCN}"
            MCHist="HMCTruth"
            MCLabel="Input"
            NormalizeMCToData="true"
            MCExtraScale="1"
            MCAbsoluteScale="true"
         fi
         if [[ "$Type" != "Nominal" ]]; then
            MCFile="DUMMY"
            MCHist="DUMMY"
            MCLabel="DUMMY"
            NormalizeMCToData="false"
            MCExtraScale="1"
            MCAbsoluteScale="true"
         fi
      elif [[ "$Tier" == "Reco" ]]; then
         MCFile="Input/${PRCN}"
         MCHist="HInput"
         MCLabel="Input"
         NormalizeMCToData="false"
         MCExtraScale="$ExtraScale"
         MCAbsoluteScale="false"
      elif [[ "$Tier" == "InputReco" ]]; then
         MCFile="Input/${PRCN}"
         MCHist="HMCMeasured"
         MCLabel="MC"
         NormalizeMCToData="true"
         MCExtraScale="$ExtraScale"
         MCAbsoluteScale="false"
      elif [[ "$Tier" == "InputRecoGenBin" ]]; then
         MCFile="Input/${PRCN}"
         MCHist="HMCMeasuredGenBin"
         MCLabel="MC"
         NormalizeMCToData="true"
         MCExtraScale="$ExtraScale"
         MCAbsoluteScale="false"
      fi

      SystematicsFile=Systematics/${PRC}.root
      if [[ "$Tier" != "Gen" ]]; then
         SystematicsFile=NULL
      fi

      DataLabel=Data
      if [[ "$Tier" == "Gen" ]]; then
         DataLabel="Data"
      elif [[ "$Tier" == "Reco" ]]; then
         DataLabel="MCReco"
      elif [[ "$Tier" == "InputReco" ]]; then
         DataLabel="Reco data"
      elif [[ "$Tier" == "InputRecoGenBin" ]]; then
         DataLabel="Reco data"
      fi

      BinningTier=Gen
      if [[ "$Tier" == "Gen" ]]; then
         BinningTier=Gen
      elif [[ "$Tier" == "Reco" ]]; then
         BinningTier=Reco
      elif [[ "$Tier" == "InputReco" ]]; then
         BinningTier=Reco
      elif [[ "$Tier" == "InputRecoGenBin" ]]; then
         BinningTier=Gen
      fi
      
      ./Execute \
         --Input "Input/${PRCN}" \
         --Systematic "$SystematicsFile" \
         --Output "Plots_R4/QualityPlots_${PRCTST}.pdf" --FinalOutput "Plots_R4/${PRCTST}.pdf" \
         --RootOutput "Root_R4/${PRCTST}.root" \
         --DataLabel "$DataLabel" \
         --MCFile "${MCFile}" --MCHistogram "${MCHist}" --MCLabel "${MCLabel}" \
         --MCExtraScale "$MCExtraScale" --MCAbsoluteScale "$MCAbsoluteScale" \
         --Tier "$BinningTier" \
         --NormalizeMCToData "${NormalizeMCToData}" \
         --PrimaryName "$HPrimary" \
         --Underflow "$Underflow" \
         --Overflow "$Overflow" \
         --DoSelfNormalize false \
         --ExtraScale "$ExtraScale" \
         --WorldXMin 80 --WorldXMax 1000 --WorldYMin 0.0000000001 --WorldYMax 0.01 --WorldRMin 0.51 --WorldRMax 1.49 \
         --LogX true --LogY true \
         --XLabel "Jet p_{T} (GeV)" --YLabel "${YLabel}" --Binning None \
         --LegendX 0.10 --LegendY 0.10 --LegendSize 0.04 \
         --XAxis 505 --YAxis 505 --RAxis 505 --MarkerModifier 0.5 \
         --Texts 0,0.65,0.9,"Anti-k_{T} jet R = $RValue",0,0.65,0.85,"|#eta_{jet}| < 2.0",0,0.65,0.8,"$CentralityString" \
         --Luminosity "$Luminosity" --LuminosityUnit "$LuminosityUnit" --System "$System" \
         --IgnoreGroup 0 --Row 1 --Column 1
   done
done