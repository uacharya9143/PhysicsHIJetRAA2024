#!/bin/bash

echo Yay....?

#cd /afs/cern.ch/work/c/chenyi/CMSSW/CMSSW_11_2_1_patch2/src/
cd /afs/cern.ch/user/u/uacharya/JetRAA_2024/CMSSW_14_1_6/src/
eval `scramv1 runtime -sh`
cd -

#CodeDir=/afs/cern.ch/work/c/chenyi//PhysicsWorkspace/HIJetRAA2018/MainAnalysis/23762_UnfoldPreparation
CodeDir=/afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/23762_UnfoldPreparation/

#export ProjectBase=/afs/cern.ch/work/c/chenyi//PhysicsWorkspace/HIJetRAA2018/
export ProjectBase=/afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/
#export PATH=$PATH:/afs/cern.ch/user/c/chenyi/RootMacros/

echo Running preset with argument = "$@"

ln -sf $CodeDir/Input Input
ln -sf $CodeDir/Output Output
cp $CodeDir/GlobalSetting.dh .

cp $CodeDir/Execute .
eval `$CodeDir/ExecutePreset $@ | grep ^time`

rm -f Input Output GlobalSetting.dh Execute

