#!/bin/bash

JetR=`DHQuery GlobalSetting.dh Global JetR`

PbPbMCBase=`DHQuery GlobalSetting.dh Sample PbPbMC1`
PPMCBase=`DHQuery GlobalSetting.dh Sample PPMC1`
PbPbDataBase=`DHQuery GlobalSetting.dh Sample PbPbData1`
PPDataBase=`DHQuery GlobalSetting.dh Sample PPData1`

NFile=500
NSample=250000

for R1 in $JetR
do
   for R2 in 1 2 8 9
   do
      R1Value=`DHQuery GlobalSetting.dh JetR $R1`
      R2Value=`DHQuery GlobalSetting.dh JetR $R2`
      echo "Running (R = $R1Value) / (R = $R2Value)"

      Match=`DHQuery GlobalSetting.dh JetRMatch $((R1 < R2 ? R1 : R2))`
      
      # Run PbPb Gen
      # ./Execute --Input `find $PbPbMCBase | grep root | head -n $NFile | tr '\n' ','` \
      #    --Tree1 akCs${R1}PFJetAnalyzer/t \
      #    --Tree2 akCs${R2}PFJetAnalyzer/t \
      #    --DHFile GlobalSetting.dh \
      #    --MatchAngle $Match \
      #    --EtaMin -2 --EtaMax 2 \
      #    --Output pdf/Rho_PbPbR${R1}R${R2}.pdf \
      #    --NSample $NSample --UseGen true \
      #    --DHOutput StatisticsRho.dh --DHState PbPbR${R1}R${R2}
      
      # Run PbPb Reco
      # JECBase=$ProjectBase/CommonCode/jec/Autumn18_HI_RAAV2_MC/Autumn18_HI_RAAV2_MC
      # ./Execute --Input `find $PbPbMCBase | grep root | head -n $NFile | tr '\n' ','` \
      #    --Tree1 akCs${R1}PFJetAnalyzer/t \
      #    --Tree2 akCs${R2}PFJetAnalyzer/t \
      #    --DHFile GlobalSetting.dh \
      #    --MatchAngle $Match \
      #    --EtaMin -2 --EtaMax 2 \
      #    --Output pdf/Rho_RecoPbPbR${R1}R${R2}.pdf \
      #    --NSample $NSample --UseGen false \
      #    --DoJEC true \
      #    --JEC1 ${JECBase}_L2Relative_AK${R1}PF.txt \
      #    --JEC2 ${JECBase}_L2Relative_AK${R2}PF.txt \
      #    --DHOutput StatisticsRho.dh --DHState RecoPbPbR${R1}R${R2}

      # Run PbPb Data
      JECBase=$ProjectBase/CommonCode/jec/Summer2024_PbPb5p36TeV_MC/Summer2024_PbPb5p36TeV_MC
      ./Execute --Input `find $PbPbDataBase | grep root | head -n $NFile | tr '\n' ','` \
         --Tree1 "akCs${R1}PFJetAnalyzer/t" \
         --Tree2 "akCs${R2}PFJetAnalyzer/t" \
         --DHFile GlobalSetting.dh \
         --MatchAngle $Match \
         --EtaMin -2 --EtaMax 2 \
         --Output pdf/Rho_DataPbPbR${R1}R${R2}.pdf \
         --NSample $NSample --UseGen false \
         --DoJEC true \
         --JEC1 ${JECBase}_L2Relative_AK${R1}PF.txt \
         --JEC2 ${JECBase}_L2Relative_AK${R2}PF.txt \
         --DHOutput StatisticsRho.dh --DHState DataPbPbR${R1}R${R2}

      # Run pp
      # ./Execute --Input `find $PPBase | grep root | head -n $NFile | tr '\n' ','` \
      #    --Tree1 ak${R1}PFJetAnalyzer/t \
      #    --Tree2 ak${R2}PFJetAnalyzer/t \
      #    --DHFile GlobalSetting.dh \
      #    --MatchAngle $Match \
      #    --EtaMin -2 --EtaMax 2 \
      #    --Output pdf/Rho_PPR${R1}R${R2}.pdf \
      #    --NSample $NSample \
      #    --DHOutput Rho.dh --DHState PPR${R1}R${R2}
   done
done
