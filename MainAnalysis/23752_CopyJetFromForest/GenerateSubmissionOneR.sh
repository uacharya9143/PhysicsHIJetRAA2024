#!/bin/bash

File=Submit.condor
rm -f ${File}

ToRunList="$1"
ToRunR="$2"
#=/afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/23752_CopyJetFromForest
# Available arguments: 
# PPMC                     = nominal PP MC (UL)
# PPData                   = nominal PP data (UL)
# PPMCEOY                  = nominal PP MC (EOY)
# PPDataEOY                = nominal PP data (EOY)
# PbPbMC                   = PbPb MC with MC centrality table
# PbPbMCInclusive          = don't split centrality
# PbPbMCRho                = use rho to weight to data
# PbPbMCRhoCentralityUp    = use rho to weight to data.  Use the centrality-shift table
# PbPbMCRhoCentralityDown  = use rho to weight to data.  Shift centrality down
# PbPbMCRhoNoPhiResidual   = don't apply phi correction, but use rho to weight to data.
# PbPbMCJEC                = don't apply phi correction.  PT down to 0.  This is for phi JEC derivation
# PbPbData                 = nominal data
# PbPbDataNoJetID          = nominal data, don't apply jet ID
# PbPbDataNoResidual       = don't apply data residual
# PbPbDataNoPhiResidual    = don't apply phi residual
# PbPbDataCentralityUp     = centrality variation
# PbPbDataCentralityDown   = centrality variation
# PbPbDataStoredCentrality = nominal data with stored centrality

# ToRunList="PbPbData PbPbDataStoredCentrality PbPbDataNoResidual PbPbDataNoPhiResidual PbPbDataCentralityUp PbPbDataCentralityDown"
# ToRunList="PbPbData PPData PPDataEOY"
#ToRunList="PPDataEOY"
# ToRunList="PbPbDataStoredCentrality PbPbDataNoResidual PbPbDataNoPhiResidual PbPbDataCentralityUp PbPbDataCentralityDown PbPbDataNoJetID"
# ToRunList="PbPbMCRho PbPbMCRhoCentralityUp PbPbMCRhoCentralityDown PbPbMCRhoNoPhiResidual"
#ToRunList="PbPbData"
ToRunList="PPData"
#ToRunList="PbPbDataCentralityUp"
#ToRunList="PbPbDataCentralityDown"
#ToRunList="PbPbMC"
#ToRunList="PPMC"
#ToRunList="PbPbMCRho"
# ToRunList="PPMCEOY"
# ToRunList="PbPbDataNoResidual PbPbDataNoPhiResidual PbPbDataNoJetID"
#ToRunR="1 2 3 4 6 7 8 9"
ToRunR="4"
#ToRunR="6 7 8 9"
Group=20

echo "Setting up submission to run on the following list"
echo "$ToRunList"
echo "with the following jet radius tags"
echo "$ToRunR"

echo "Universe              = vanilla" >> ${File}
#echo "Executable            = /afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/23752_CopyJetFromForest/JobWrapperOneR.sh" >> ${File}
echo "Executable            = JobWrapperOneR.sh" >> ${File}
#echo "Executable            = /afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/23752_CopyJetFromForest/RunFileOneR.sh" >> ${File}
echo "should_transfer_files = NO" >> ${File}
#echo "+JobFlavour           = \"longlunch\"" >> ${File}
echo "+JobFlavour           = \"tomorrow\"" >> ${File}

echo "" >> ${File}
echo "" >> ${File}

mkdir -p Log/
rm -f Log/*

ToSubmitCount=0

for i in ${ToRunList}
do
   Count=0
   for RTag in ${ToRunR}
   do
      LocationKey=$i
      if [[ "$i" == "PbPbMCJEC"* ]]; then
         LocationKey="PbPbMCJEC"
      elif [[ "$i" == "PbPbMC"* ]]; then
         LocationKey="PbPbMC"
      elif [[ "$i" == "PbPbData"* ]]; then
         LocationKey="PbPbData"
      fi

      LocationBase=`DHQuery GlobalSetting.dh Sample ${LocationKey}${RTag} | tr -d '"'`

      for j in `find "${LocationBase}/" | grep root$ | Reformat $Group | tr ' ' ',' | sed "s/[ ,]*$//g"`
      #for j in $(cat PbPbMC_runList.txt | Reformat $Group | tr ' ' ',' | sed "s/[ ,]*$//g")
      do
         # echo $j
         Count=`echo $Count | AddConst 1`

         # Arguments = InputFile Tag Trigger IsMC IsPP Recluster RTag MinPT SkipReco
         #    DoPhiResidual DoDataResidual DoExclusion Centrality CentralityTable DoJetID
         # ExtraArguments start from the Trigger
         ExtraArguments="None 1 1 1 $RTag 40 0 0 0 0 default default 1"
         if [[ "$i" == "PPMC" ]]; then
             ExtraArguments="None 1 1 0 $RTag 40 0 0 0 1 default MC 1"   # NoUL
	     
         elif [[ "$i" == "PPData" ]]; then
             ExtraArguments="HLT_AK4PFJet100 0 1 0 $RTag 40 0 0 1 1 default Data 1" #for PPRef_2024 #NoUL
	     
         elif [[ "$i" == "PPMCEOY" ]]; then
             ExtraArguments="None 1 1 1 $RTag 40 0 0 0 0 default default 1"   # NonUL
	     
         elif [[ "$i" == "PPDataEOY" ]]; then
             # ExtraArguments="HLT_HIAK4PFJet80 0 1 1 $RTag 40 0 0 1 0 default default 1"   # recluster
             ExtraArguments="HLT_HIAK4PFJet80 0 1 0 $RTag 40 0 0 1 0 default default 1"   # don't recluster
	     
         elif [[ "$i" == "PbPbMC" ]]; then
             ExtraArguments="None 1 0 0 $RTag 40 0 0 0 1 default MC 1" #for the centrality bins PbPbMC
             #ExtraArguments="None 1 0 0 $RTag 60 0 1 0 1 Inclusive MC 1" #for inclusive all bins PbPbMC
	     
         elif [[ "$i" == "PbPbMCInclusive" ]]; then
             ExtraArguments="None 1 0 0 $RTag 60 0 1 0 1 Inclusive default 1" #for inclusive all bins PbPbMC
	     
         elif [[ "$i" == "PbPbMCRho" ]]; then
             ExtraArguments="None 1 0 0 $RTag 40 0 0 0 1 rho MC 1"
	     
         elif [[ "$i" == "PbPbMCRhoCentralityUp" ]]; then
             ExtraArguments="None 1 0 0 $RTag 60 0 1 0 1 rhoup default 1"
	     
         elif [[ "$i" == "PbPbMCRhoCentralityDown" ]]; then
             ExtraArguments="None 1 0 0 $RTag 60 0 1 0 1 rhodown default 1"
	     
         elif [[ "$i" == "PbPbMCRhoNoPhiResidual" ]]; then
             ExtraArguments="None 1 0 0 $RTag 60 0 0 0 1 rho default 1"
	     
         elif [[ "$i" == "PbPbMCJEC" ]]; then
             ExtraArguments="None 1 0 0 $RTag 0 1 0 0 0 Inclusive default 1"
	     
         elif [[ "$i" == "PbPbData" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 40 0 0 1 1 default Data 1" #for HIPbPb2024 data
	     
         elif [[ "$i" == "PbPbDataNoJetID" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 60 0 1 1 1 default Data 0"
	     
         elif [[ "$i" == "PbPbDataStoredCentrality" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 60 0 1 1 1 default default 1"
	     
         elif [[ "$i" == "PbPbDataNoResidual" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 60 0 1 0 1 default Data 1"
	     
         elif [[ "$i" == "PbPbDataNoPhiResidual" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 40 0 0 1 1 default Data 1"
	     
         elif [[ "$i" == "PbPbDataCentralityUp" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 40 0 0 1 1 default DataUp 1"
	     
         elif [[ "$i" == "PbPbDataCentralityDown" ]]; then
             ExtraArguments="HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 $RTag 40 0 0 1 1 default DataDown 1"
         fi

         echo "Arguments = $j Run_${i}_Part${Count} $ExtraArguments" >> ${File}
         echo "Output    = Log/${i}${RTag}Part${Count}.out" >> ${File}
         echo "Error     = Log/${i}${RTag}Part${Count}.err" >> ${File}
         echo "Log       = Log/${i}${RTag}Part${Count}.log" >> ${File}
         echo "Queue" >> ${File}
         echo "" >> ${File}

         ((ToSubmitCount=ToSubmitCount+1))
      done

      echo "" >> ${File}
      echo "" >> ${File}
   done
done

echo "Number of jobs to submit: $ToSubmitCount"

