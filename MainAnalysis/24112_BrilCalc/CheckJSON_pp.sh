#!/bin/bash
#source /cvmfs/cms-bril.cern.ch/cms-lumi-pog/brilws-docker/brilws-env
#source /cvmfs/cms.cern.ch/cmsset_default.sh  # Load CMSSW environment
#export PATH=$HOME/.local/bin:/cvmfs/cms.cern.ch/cms-lumi-pog/brilconda/bin:$PATH
#source /cvmfs/cms.cern.ch/cmsset_default.sh

brilcalc='singularity -s exec  --env PYTHONPATH=/home/bril/.local/lib/python3.10/site-packages /cvmfs/unpacked.cern.ch/gitlab-registry.cern.ch/cms-cloud/brilws-docker:latest brilcalc'
Input=$1
DH=$2

if [[ "$DH" == "" ]]; then
   DH=GlobalSetting.dh
fi

$brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json -i ${Input} -o temp.csv 

BrilValue=$(grep -v "^#" temp.csv | cut -d ',' -f 6 | awk '{sum += $1} END {print sum}') 
USED_PARTS=1
TOTAL_PARTS=1

Fraction=$(awk -v u="$USED_PARTS" -v t="$TOTAL_PARTS" 'BEGIN{if(t==0){print 0}else{print u/t}}')
BrilValueSet=$(awk -v v="$BrilValue" -v f="$Fraction" 'BEGIN{printf "%.6f", v*f}')

Tag=`basename $Input | sed "s/_Nominal//" | sed "s/_JSON.txt//"`

echo "$Tag total=$BrilValue scaled($USED_PARTS/$TOTAL_PARTS)=$BrilValueSet"
DHSet "$DH" Lumi "${Tag/Fine}_BRIL" double "$BrilValueSet"



#DHSet $DH Lumi ${Tag/Fine}_PHYSICS double $PhysicsValue



#BrilValue=1000
#$brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i ${Input} -o temp.csv #disabled by uttam for now
#$brilcalc lumi  -i ${Input} -o temp.csv #disabled by uttam for now

####PhysicsValue=`grep -v "^#" temp.csv | cut -d ',' -f 6 | Sum`
#PhysicsValue=$(grep -v "^#" temp.csv | cut -d ',' -f 6 | awk '{sum += $1} END {print sum}') #disabled by uttam for now
#PhysicsValue=1000
#rm temp.csv #disabled by uttam for now
