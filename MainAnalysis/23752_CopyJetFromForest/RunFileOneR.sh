#!/bin/sh

echo
echo 'Make sure you have project base properly set!!!'
echo

InputFile=$1
Tag=$2
Trigger=$3
IsMC=$4
IsPP=$5
Recluster=$6
RTag=$7
MinPT=$8
SkipReco=$9
DoPhiResidual=${10}
DoDataResidual=${11}
DoExclusion=${12}
Centrality=${13}
CentralityTable=${14}
DoJetID=${15}

echo "Runing with the following"
echo "  Input = $InputFile"
echo "  Tag = $Tag"
echo "  Trigger = $Trigger"
echo "  IsMC = $IsMC (1 = yes, 0 = no)"
echo "  IsPP = $IsPP (0 = PbPb, 1 = NonUL pp, 2 = UL pp)"
echo "  Recluster = $Recluster (0 = no, 1 = yes)"
echo "  RTag = $RTag (what radius?  Goes from 1-9)"
echo "  MinPT = $MinPT (in GeV)"
echo "  SkipReco = $SkipReco (0 = no, 1 = yes)"
echo "  DoPhiResidual = $DoPhiResidual (0 = no, 1 = yes)"
echo "  DoDataResidual = $DoDataResidual (0 = no, 1 = yes)"
echo "  DoExclusion = $DoExclusion (0 = no, 1 = yes)"
echo "  Centrality = $Centrality"
echo "  CentralityTable = $CentralityTable"
echo "  JetID = $DoJetID (0 = no, 1 = yes)"
echo

Fraction=1

JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`

DoRhoWeight=0
RhoSuffix=
if [[ "$Centrality" == "default" ]]; then
   if [[ "$IsPP" != "0" ]]; then
      Centrality="Inclusive"
   else
      Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
   fi
elif [[ "$Centrality" == "rho"* ]]; then
   DoRhoWeight=1
   if [[ "$Centrality" == "rhoup" ]]; then
      RhoSuffix="_CentralityUp"
   elif [[ "$Centrality" == "rhodown" ]]; then
      RhoSuffix="_CentralityDown"
   else
      RhoSuffix=
   fi
   if [[ "$IsPP" != "0" ]]; then
      Centrality="Inclusive"
   else
      Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
   fi
fi

RValue=`DHQuery GlobalSetting.dh JetR $RTag`

JECBase="$ProjectBase/CommonCode/jec/"
#JECTag="Autumn18_HI_RAAV2_MC"
JECTag="Summer2024_ppRef5p36TeV_MC"
# PhiTag="Phi_24151"
PhiTag="Phi_24252"

if [[ "$IsMC" == "1" ]] && [[ "$IsPP" == "0" ]]; then
   #JECTag="Autumn18_HI_RAAV3_MC"
   JECTag="Summer2024_PbPb5p36TeV_MC"
elif [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "0" ]]; then
   JECTag="Summer2024_PbPb5p36TeV_MC"
elif [[ "$IsMC" == "1" ]] && [[ "$IsPP" == "1" ]]; then
   JECTag="Summer2024_ppRef5p36TeV_MC"
   #JECTag="Spring18_ppRef5TeV_RAAV3_MC"
elif [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "1" ]]; then
   JECTag="Summer2024_ppRef5p36TeV_MC"
elif [[ "$IsMC" == "1" ]] && [[ "$IsPP" == "2" ]]; then
    JECTag="Summer2024_ppRef5p36TeV_MC"
  #JECTag="Summer20UL17_ppRef5TeV_RAAV2_MC"
elif [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "2" ]]; then
   JECTag="Summer20UL17_ppRef5TeV_RAAV2_DATA"
fi

#JEC=""

JEC="$JECBase/$JECTag/${JECTag}_L2Relative_AK${RTag}PF.txt"

#if [[ "$DoPhiResidual" == 1 ]]; then
#   JEC="$JEC,$JECBase/$PhiTag/PhiCorrectionGenLowNoThreshold_AK${RTag}PF.txt"
#fi
if [[ "$DoDataResidual" == 1 ]]; then
   JEC="$JEC,$JECBase/$JECTag/${JECTag}_L2L3Residual_AK${RTag}PF.txt"
fi


Exclusion=`DHQuery GlobalSetting.dh Binning JetExclusion | tr -d '"'`
if [[ "$DoExclusion" == 0 ]]; then
   Exclusion="99,100,99,100"
fi

#JEU=""

JEU="$JECBase/$JECTag/${JECTag}_Uncertainty_AK${RTag}PF.txt"

echo $JEC
echo $JEU
#echo "JEC (DISABLED): $JEC"
#echo "JEU: $JEU"
#echo "NOTE: Running without JEC corrections to study uncorrected jets"

BaselineCutAA=false
if [[ "$IsMC" == "0" ]] && [[ "$IsPP" == "0" ]]; then
   BaselineCutAA=true
fi
BaselineCutPP=false
if [[ "$IsMC" == "0" ]] && [[ "$IsPP" != "0" ]]; then
   BaselineCutPP=true
fi

for CTag in $Centrality
do
   CMin=`DHQuery GlobalSetting.dh CentralityMin $CTag`
   CMax=`DHQuery GlobalSetting.dh CentralityMax $CTag`

   CheckCentrality=true
   if [[ "$CTag" == "Inclusive" ]]; then
      CheckCentrality=false
   fi
   
   ReEvaluateCentrality=true
   CentralityShift=0
   if [[ $CentralityTable == "default" ]]; then
      ReEvaluateCentrality=false
      CentralityShift=0
   elif [[ $CentralityTable == "MC" ]]; then
      ReEvaluateCentrality=true
      CentralityShift=100
   elif [[ $CentralityTable == "Data" ]]; then
      ReEvaluateCentrality=true
      CentralityShift=0
   elif [[ $CentralityTable == "DataUp" ]]; then
      ReEvaluateCentrality=true
      CentralityShift=1
   elif [[ $CentralityTable == "DataDown" ]]; then
      ReEvaluateCentrality=true
      CentralityShift=-1
   fi

   RhoKey="none"
   if [[ "$DoRhoWeight" == 1 ]]; then
      RhoKey="R${RTag}_Centrality${CTag}$RhoSuffix"
      CheckCentrality=false
      ReEvaluateCentrality=false
      CentralityShift=0
   fi

   echo "Running R = $RValue, Centrality = $CTag"

   Stored=true
   if [[ "$Recluster" == "1" ]]; then
      Stored=false
   fi

   PTMin=$MinPT
   GenPTMin=$MinPT

   Jet="akCs4PFJetAnalyzer/t"
   if [[ "$IsPP" == 0 ]]; then
      Jet="akCs${RTag}PFJetAnalyzer/t"
   else
      Jet="ak${RTag}PFJetAnalyzer/t"
   fi

   JetIDTag=
   if [[ "$IsPP" == 0 ]]; then
      JetIDTag=PbPb
   else
      JetIDTag=PP
   fi
   if [[ "$IsMC" == 0 ]]; then
      JetIDTag=${JetIDTag}Data
   else
      JetIDTag=${JetIDTag}MC
   fi

   KeepSkipped=true
   if [[ "$IsMC" == 1 ]]; then
      KeepSkipped=false
   else
      KeepSkipped=true
   fi

   if [[ "$IsMC" == "0" ]]; then
      SkipGen=true
      SkipMatch=true
   else
      SkipGen=false
      SkipMatch=false
   fi

   mkdir -p /tmp/uacharya/
   echo ./Execute --Input $InputFile --Output /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root \
       --JetR $RValue --Jet "${Jet}" --JEC ${JEC} --JEU ${JEU} \
       --Fraction $Fraction --Exclusion "$Exclusion" --KeepSkippedEvent $KeepSkipped \
       --UseStoredGen $Stored --UseStoredReco $Stored --DoRecoSubtraction false --Trigger $Trigger \
       --CheckCentrality $CheckCentrality --CentralityMin $CMin --CentralityMax $CMax \
       --PTMin $PTMin --GenPTMin $GenPTMin --DontStoreReco $SkipReco --DontStoreGen $SkipGen --DontStoreMatch $SkipMatch\
       --DoBaselineCutPP $BaselineCutPP --DoBaselineCutAA $BaselineCutAA \
       --DHFile GlobalSetting.dh --RhoKeyBase $RhoKey --CutUE true \
       --ReEvaluateCentrality $ReEvaluateCentrality --CentralityShift $CentralityShift \
       --DoJetID $DoJetID --JetIDKeyBase ${JetIDTag}_R${RTag}_Centrality${CTag}
   ./Execute --Input $InputFile --Output /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root \
      --JetR $RValue --Jet "${Jet}" --JEC ${JEC} --JEU ${JEU} \
      --Fraction $Fraction --Exclusion "$Exclusion" --KeepSkippedEvent $KeepSkipped \
      --UseStoredGen $Stored --UseStoredReco $Stored --DoRecoSubtraction false --Trigger $Trigger \
      --CheckCentrality $CheckCentrality --CentralityMin $CMin --CentralityMax $CMax \
      --PTMin $PTMin --GenPTMin $GenPTMin --DontStoreReco $SkipReco --DontStoreGen $SkipGen --DontStoreMatch $SkipMatch \
      --DoBaselineCutPP $BaselineCutPP --DoBaselineCutAA $BaselineCutAA \
      --DHFile GlobalSetting.dh --RhoKeyBase $RhoKey --CutUE true \
      --ReEvaluateCentrality $ReEvaluateCentrality --CentralityShift $CentralityShift \
      --DoJetID $DoJetID --JetIDKeyBase ${JetIDTag}_R${RTag}_Centrality${CTag}

   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PPMC_R4/${Tag}_R${RTag}_Centrality${CTag}.root
   mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PPData_R4/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbData_PPData/${Tag}_R${RTag}_Centrality${CTag}.root
  #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbData_CentralityUp/${Tag}_R${RTag}_Centrality${CTag}.root
  # mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbData_CentralityDown/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbMC/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PPMC/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbMCRho/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbData_JEU/${Tag}_R${RTag}_Centrality${CTag}.root
  #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PPRef_Data/${Tag}_R${RTag}_Centrality${CTag}.root
   #mv /tmp/uacharya/${Tag}_R${RTag}_Centrality${CTag}.root Output_PbPbData_R4/${Tag}_R${RTag}_Centrality${CTag}.root

done


