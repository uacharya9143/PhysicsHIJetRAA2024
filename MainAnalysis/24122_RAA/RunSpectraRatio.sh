#!/bin/bash

PPState=$1
PPSuffix=$2
AAState=$3
AASuffix=$4
OutputTag=$5

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`

for BaseR in 1 2
do
   for R in $JetR
   do
      PPFile0=SpectraRoot/${PPState}_R${BaseR}_CentralityInclusive_${PPSuffix}.root
      PPFile1=SpectraRoot/${AAState}_R${BaseR}_Centrality0to10_${AASuffix}.root
      PPFile2=SpectraRoot/${AAState}_R${BaseR}_Centrality10to30_${AASuffix}.root
      PPFile3=SpectraRoot/${AAState}_R${BaseR}_Centrality30to50_${AASuffix}.root
      PPFile4=SpectraRoot/${AAState}_R${BaseR}_Centrality50to70_${AASuffix}.root
      PPFile5=SpectraRoot/${AAState}_R${BaseR}_Centrality70to90_${AASuffix}.root
      PPFile6=SpectraRoot/${AAState}_R${BaseR}_Centrality50to90_${AASuffix}.root
      PPCurve0=Result0
      PPCurve1=Result0
      PPCurve2=Result0
      PPCurve3=Result0
      PPCurve4=Result0
      PPCurve5=Result0
      PPCurve6=Result0
      AAFile0=SpectraRoot/${PPState}_R${R}_CentralityInclusive_${PPSuffix}.root
      AAFile1=SpectraRoot/${AAState}_R${R}_Centrality0to10_${AASuffix}.root
      AAFile2=SpectraRoot/${AAState}_R${R}_Centrality10to30_${AASuffix}.root
      AAFile3=SpectraRoot/${AAState}_R${R}_Centrality30to50_${AASuffix}.root
      AAFile4=SpectraRoot/${AAState}_R${R}_Centrality50to70_${AASuffix}.root
      AAFile5=SpectraRoot/${AAState}_R${R}_Centrality70to90_${AASuffix}.root
      AAFile6=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
      AACurve0=Result0
      AACurve1=Result0
      AACurve2=Result0
      AACurve3=Result0
      AACurve4=Result0
      AACurve5=Result0
      AACurve6=Result0
      SysFile0=CombinedSystematics/PPDataRatio_R${R}R${BaseR}_CentralityInclusive.root
      SysFile1=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality0to10.root
      SysFile2=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality10to30.root
      SysFile3=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality30to50.root
      SysFile4=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality50to70.root
      SysFile5=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality70to90.root
      SysFile6=CombinedSystematics/PbPbDataRatio_R${R}R${BaseR}_Centrality50to90.root
      CLabel0=R${R}R${BaseR}_CentralityInclusive
      CLabel1=R${R}R${BaseR}_Centrality0to10
      CLabel2=R${R}R${BaseR}_Centrality10to30
      CLabel3=R${R}R${BaseR}_Centrality30to50
      CLabel4=R${R}R${BaseR}_Centrality50to70
      CLabel5=R${R}R${BaseR}_Centrality70to90
      CLabel6=R${R}R${BaseR}_Centrality50to90

      #PPLumi=`DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"' | DivideConst 1000000`
      PPLumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"') / 1000000" |bc)
      PPLumiUnit="pb^{-1}"
      #AALumi=`DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"' | DivideConst 1000`
      AALumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"') / 1000" |bc)
      AALumiUnit="nb^{-1}"

      if [[ "$PPLumi" == "" ]]; then
         PPLumi=0
      fi
      if [[ "$PbPbLumi" == "" ]]; then
         PbPbLumi=0
      fi

      RValue=`DHQuery GlobalSetting.dh JetR $R`
      BaseRValue=`DHQuery GlobalSetting.dh JetR $BaseR`
      YLabel="Spectra ratio (R = $RValue / R = $BaseRValue)"

      if [[ "$OutputTag" == "" ]]; then
         OutputTag=PP${PPSuffix}_AA${AASuffix}
      fi

      ./Execute \
         --PP ${PPFile0},${PPFile1},${PPFile2},${PPFile3},${PPFile4},${PPFile5},${PPFile6} \
         --PPName $PPCurve0,$PPCurve1,$PPCurve2,$PPCurve3,$PPCurve4,$PPCurve5,$PPCurve6 \
         --AA ${AAFile0},${AAFile1},${AAFile2},${AAFile3},${AAFile4},${AAFile5},${AAFile6} \
         --AAName $AACurve0,$AACurve1,$AACurve2,$AACurve3,$AACurve4,$AACurve5,$AACurve6 \
         --Systematics ${SysFile0},${SysFile1},${SysFile2},${SysFile3},${SysFile4},${SysFile5},${SysFile6} \
         --Labels "pp","0-10%","10-30%","30-50%","50-70%","70-90%","50-90%" \
         --FinalOutput Plots/DataRatioR${R}R${BaseR}_${OutputTag}.pdf \
         --RootOutput Root/DataRatioR${R}R${BaseR}_${OutputTag}.root \
         --CurveLabel $CLabel0,$CLabel1,$CLabel2,$CLabel3,$CLabel4,$CLabel5,$CLabel6 \
         --WorldXMin 158 --WorldXMax 1500 --WorldYMin 0.0 --WorldYMax 4.0 --LogX true --LogY false \
         --XLabel "Jet p_{T} (GeV)" --YLabel "$YLabel" \
         --XAxisSpacing 505 --YAxisSpacing 505 \
         --LegendX 0.75 --LegendY 0.35 --LegendSize 0.040 \
         --PPLumi ${PPLumi} --AALumi ${AALumi} --PPLumiUnit ${PPLumiUnit} --AALumiUnit ${AALumiUnit} \
         --StatDHFile StatisticsRho.dh --StatDHState DataPbPbR${R}R${BaseR}
         # --StatDHFile none --StatDHState none
   done
done

