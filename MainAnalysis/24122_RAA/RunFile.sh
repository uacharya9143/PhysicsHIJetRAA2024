#!/bin/bash
#set -euo pipefail
PPState=$1
PPSuffix=$2
AAState=$3
AASuffix=$4
OutputTag=$5 

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
  # Safe bc -l with scale, guarding empty inputs
  # Usage: bc_calc "<expr>"
  echo "$1" | bc -l
}

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR="1 2 3 4 6 7 8 9"; \
JetR="4"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`

for R in $JetR
do
   PPFile=SpectraRoot/${PPState}_R${R}_CentralityInclusive_${PPSuffix}.root
   PPCurve=Result0
   AAFile0=SpectraRoot/${AAState}_R${R}_Centrality0to10_${AASuffix}.root
   AAFile1=SpectraRoot/${AAState}_R${R}_Centrality10to30_${AASuffix}.root
   AAFile2=SpectraRoot/${AAState}_R${R}_Centrality30to50_${AASuffix}.root
   AAFile3=SpectraRoot/${AAState}_R${R}_Centrality50to70_${AASuffix}.root
   AAFile4=SpectraRoot/${AAState}_R${R}_Centrality70to90_${AASuffix}.root
   AAFile5=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
   AACurve0=Result0
   AACurve1=Result0
   AACurve2=Result0
   AACurve3=Result0
   AACurve4=Result0
   AACurve5=Result0
   SysFile0=CombinedSystematics/RAA_R${R}_Centrality0to10.root
   SysFile1=CombinedSystematics/RAA_R${R}_Centrality10to30.root
   SysFile2=CombinedSystematics/RAA_R${R}_Centrality30to50.root
   SysFile3=CombinedSystematics/RAA_R${R}_Centrality50to70.root
   SysFile4=CombinedSystematics/RAA_R${R}_Centrality70to90.root
   SysFile5=CombinedSystematics/RAA_R${R}_Centrality50to90.root
   CLabel0=R${R}_Centrality0to10
   CLabel1=R${R}_Centrality10to30
   CLabel2=R${R}_Centrality30to50
   CLabel3=R${R}_Centrality50to70
   CLabel4=R${R}_Centrality70to90
   CLabel5=R${R}_Centrality50to90

   #PPLumi=`DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"' | DivideConst 1000000` #disabled by uttam
   #PPLumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"') / 1000000" | bc)
   #PPLumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"') / 1" | bc)
   PPLumi=$(num_or_default "DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL" "0")
   PPLumi=$(printf "%.6f" "$PPLumi")
   PPLumi=$(echo "scale=6; $PPLumi / 1000000" | bc)
   PPLumiUnit="pb^{-1}"
   echo "PPLumi: $PPLumi"

   #AALumi=`DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"' | DivideConst 1000` #disabled by uttam
   #AALumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"') / 1000" | bc)
   #AALumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"') / 1" | bc)
   AALumi=$(num_or_default "DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL" "0")
   AALumi=$(printf "%.6f" "$AALumi")
   AALumi=$(echo "scale=6; $AALumi / 1000" | bc)
   AALumiUnit="nb^{-1}"
   echo "AALumi: $AALumi"

   if [[ "$PPLumi" == "" ]]; then
      PPLumi=0
   fi
   if [[ "$PbPbLumi" == "" ]]; then
      PbPbLumi=0
   fi

   RValue=`DHQuery GlobalSetting.dh JetR $R`
   YLabel="R_{AA} (R = $RValue)"

   if [[ "$OutputTag" == "" ]]; then
      OutputTag=PP${PPSuffix}_AA${AASuffix}
   fi

   ./Execute \
      --PP ${PPFile},${PPFile},${PPFile},${PPFile},${PPFile},${PPFile} \
      --PPName $PPCurve,$PPCurve,$PPCurve,$PPCurve,$PPCurve,$PPCurve \
      --AA ${AAFile0},${AAFile1},${AAFile2},${AAFile3},${AAFile4},${AAFile5} \
      --AAName $AACurve0,$AACurve1,$AACurve2,$AACurve3,$AACurve4,$AACurve5 \
      --Systematics ${SysFile0},${SysFile1},${SysFile2},${SysFile3},${SysFile4},${SysFile5} \
      --Labels "0-10%","10-30%","30-50%","50-70%","70-90%","50-90%" \
      --FinalOutput Plots_R4/RAAR${R}_${OutputTag}.pdf \
      --RootOutput Root_R4/RAAR${R}_${OutputTag}.root \
      --CurveLabel $CLabel0,$CLabel1,$CLabel2,$CLabel3,$CLabel4,$CLabel5 \
      --WorldXMin 158 --WorldXMax 1000 --WorldYMin 0.0 --WorldYMax 1.2 --LogX true --LogY false \
      --XLabel "Jet p_{T} (GeV)" --YLabel "$YLabel" \
      --XAxisSpacing 505 --YAxisSpacing 505 \
      --LegendX 0.75 --LegendY 0.35 --LegendSize 0.040 \
      --PPLumi ${PPLumi} --AALumi ${AALumi} --PPLumiUnit ${PPLumiUnit} --AALumiUnit ${AALumiUnit} \
      --StatDHFile none --StatDHState none
done


