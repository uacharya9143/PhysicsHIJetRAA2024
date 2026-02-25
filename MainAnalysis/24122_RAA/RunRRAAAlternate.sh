#!/bin/bash

PPState=$1
PPSuffix=$2
AAState=$3
AASuffix=$4

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`

for BaseR in 1 2
do
   for R in $JetR
   do
      if [[ $R == $BaseR ]]; then
         continue
      fi

      PPFile=Root/DataRatioR${R}R${BaseR}_PP${PPSuffix}_AA${AASuffix}.root
      PPCurve0=RAA_R${R}R${BaseR}_CentralityInclusive
      PPCurve1=RAA_R${R}R${BaseR}_CentralityInclusive
      PPCurve2=RAA_R${R}R${BaseR}_CentralityInclusive
      PPCurve3=RAA_R${R}R${BaseR}_CentralityInclusive
      PPCurve4=RAA_R${R}R${BaseR}_CentralityInclusive
      AAFile=Root/DataRatioR${R}R${BaseR}_PP${PPSuffix}_AA${AASuffix}.root
      AACurve0=RAA_R${R}R${BaseR}_Centrality0to10
      AACurve1=RAA_R${R}R${BaseR}_Centrality10to30
      AACurve2=RAA_R${R}R${BaseR}_Centrality30to50
      AACurve3=RAA_R${R}R${BaseR}_Centrality50to70
      AACurve4=RAA_R${R}R${BaseR}_Centrality50to90
      SysFile0=CombinedSystematics/RRAA_R${R}R${BaseR}_Centrality0to10.root
      SysFile1=CombinedSystematics/RRAA_R${R}R${BaseR}_Centrality10to30.root
      SysFile2=CombinedSystematics/RRAA_R${R}R${BaseR}_Centrality30to50.root
      SysFile3=CombinedSystematics/RRAA_R${R}R${BaseR}_Centrality50to70.root
      SysFile4=CombinedSystematics/RRAA_R${R}R${BaseR}_Centrality50to90.root
      CLabel0=R${R}R${BaseR}_Centrality0to10
      CLabel1=R${R}R${BaseR}_Centrality10to30
      CLabel2=R${R}R${BaseR}_Centrality30to50
      CLabel3=R${R}R${BaseR}_Centrality50to70
      CLabel4=R${R}R${BaseR}_Centrality50to90

      #PPLumi=`DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"' | DivideConst 1000000`
      PPLumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"') / 1000000" |bc)
      PPLumiUnit="pb^{-1}"
      #AALumi=`DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"' | DivideConst 1000`
      AALumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"') / 1000" |bc)
      AALumiUnit="nb^{-1}"

      RValue1=`DHQuery GlobalSetting.dh JetR $BaseR`
      RValue2=`DHQuery GlobalSetting.dh JetR $R`
      YLabel="R_{AA} Ratio: (R = $RValue2) / (R = $RValue1)"

      ./Execute \
         --PP ${PPFile},${PPFile},${PPFile},${PPFile},${PPFile} \
         --PPName $PPCurve0,$PPCurve1,$PPCurve2,$PPCurve3,$PPCurve3 \
         --AA ${AAFile},${AAFile},${AAFile},${AAFile},${AAFile} \
         --AAName $AACurve0,$AACurve1,$AACurve2,$AACurve3,$AACurve4 \
         --Systematics ${SysFile0},${SysFile1},${SysFile2},${SysFile3},${SysFile4} \
         --Labels "0-10%","10-30%","30-50%","50-70%","50-90%" \
         --FinalOutput Plots/RRAAR${R}R${BaseR}_PP${PPSuffix}_AA${AASuffix}.pdf \
         --RootOutput Root/RRAAR${R}R${BaseR}_PP${PPSuffix}_AA${AASuffix}.root \
         --CurveLabel $CLabel0,$CLabel1,$CLabel2,$CLabel3,$CLabel4 \
         --WorldXMin 158 --WorldXMax 1500 --WorldYMin 0.0 --WorldYMax 2.0 --LogX true --LogY false \
         --XLabel "Jet p_{T} (GeV)" --YLabel "$YLabel" \
         --XAxisSpacing 505 --YAxisSpacing 505 \
         --LegendX 0.7 --LegendY 0.45 --LegendSize 0.045 \
         --PPLumi ${PPLumi} --AALumi ${AALumi} --PPLumiUnit ${PPLumiUnit} --AALumiUnit ${AALumiUnit} \
         --StatDHFile none --StatDHState none
   done
done

