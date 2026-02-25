#!/bin/bash

PPState=$1
PPSuffix=$2
AAState=$3
AASuffix=$4

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`

for R in $JetR
do
   PPFile0=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
   PPFile1=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
   PPFile2=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
   PPFile3=SpectraRoot/${AAState}_R${R}_Centrality50to90_${AASuffix}.root
   PPCurve0=Result0
   PPCurve1=Result0
   PPCurve2=Result0
   PPCurve3=Result0
   AAFile0=SpectraRoot/${AAState}_R${R}_Centrality0to10_${AASuffix}.root
   AAFile1=SpectraRoot/${AAState}_R${R}_Centrality10to30_${AASuffix}.root
   AAFile2=SpectraRoot/${AAState}_R${R}_Centrality30to50_${AASuffix}.root
   AAFile3=SpectraRoot/${AAState}_R${R}_Centrality50to70_${AASuffix}.root
   AACurve0=Result0
   AACurve1=Result0
   AACurve2=Result0
   AACurve3=Result0
   SysFile0=CombinedSystematics/RCP_R${R}_Centrality0to10.root
   SysFile1=CombinedSystematics/RCP_R${R}_Centrality10to30.root
   SysFile2=CombinedSystematics/RCP_R${R}_Centrality30to50.root
   SysFile3=CombinedSystematics/RCP_R${R}_Centrality50to70.root
   CLabel0=R${R}_Centrality0to10
   CLabel1=R${R}_Centrality10to30
   CLabel2=R${R}_Centrality30to50
   CLabel3=R${R}_Centrality50to70

   #PPLumi=`DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"' | DivideConst 1000000` #disabled by uttam
   PPLumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${PPState}_R${R}_CentralityInclusive_BRIL | tr -d '"') / 1000000" | bc)
   PPLumiUnit="pb^{-1}"

   #AALumi=`DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"' | DivideConst 1000` #disabled by uttam

   AALumi=$(echo "scale=6; $(DHQuery GlobalSetting.dh Lumi ${AAState}_R${R}_Centrality0to10_BRIL | tr -d '"' ) / 1000" | bc)
AALumiUnit="nb^{-1}"

   RValue=`DHQuery GlobalSetting.dh JetR $R`
   YLabel="R_{CP} (R = $RValue, Base = 50-90%)"

   ./Execute \
      --PP ${PPFile0},${PPFile1},${PPFile2},${PPFile3} \
      --PPName $PPCurve0,$PPCurve1,$PPCurve2,$PPCurve3 \
      --AA ${AAFile0},${AAFile1},${AAFile2},${AAFile3} \
      --AAName $AACurve0,$AACurve1,$AACurve2,$AACurve3 \
      --Systematics ${SysFile0},${SysFile1},${SysFile2},${SysFile3} \
      --Labels "0-10%","10-30%","30-50%","50-70%" \
      --FinalOutput Plots/RCPR${R}_AA${AASuffix}.pdf \
      --RootOutput Root/RCPR${R}_AA${AASuffix}.root \
      --CurveLabel $CLabel0,$CLabel1,$CLabel2,$CLabel3 \
      --WorldXMin 158 --WorldXMax 1500 --WorldYMin 0.0 --WorldYMax 1.2 --LogX true --LogY false \
      --XLabel "Jet p_{T} (GeV)" --YLabel "$YLabel" \
      --XAxisSpacing 505 --YAxisSpacing 505 \
      --LegendX 0.7 --LegendY 0.45 --LegendSize 0.045 \
      --PPLumi ${PPLumi} --AALumi ${AALumi} --PPLumiUnit ${PPLumiUnit} --AALumiUnit ${AALumiUnit} \
      --StatDHFile none --StatDHState none
done


