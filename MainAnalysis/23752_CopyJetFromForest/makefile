
#FastJet = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet/3.3.4-ghbfee/bin/fastjet-config
#FJContrib = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet-contrib/1.044-ghbfee/
FastJet = /cvmfs/cms.cern.ch/el9_amd64_gcc12/external/fastjet/3.4.1-cc6ce46a28fb676c9080adb13b99f03c/bin/fastjet-config
FJContrib = /cvmfs/cms.cern.ch/el9_amd64_gcc12/external/fastjet-contrib/1.051-afd609f10b5f81984bb8b0a86a7935ee/

Fraction = 1.00

default: TestRun

Prepare:
	echo $(ProjectBase)
	ln -sf ../GlobalSetting/GlobalSetting.dh .
#/afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	# ln -sf $(ProjectBase)/Samples/Example SampleExample
	# mkdir -p Output/

#PPData4=/eos/cms/store/group/phys_heavyions/chenyi/pp2017/Forest//HighEGJet/Run2017G-17Nov2017-v2/AOD/HighEGJet/20211115FullRunHighEGJetNoTrackTree/211115_214540/0000/
#PbPbData4=/eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest//HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit35WithRhoGenBugFix/211106_221601/0000/
#PbPbMC4=/eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest//DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/DefaultPbPbMCForJetRAAWithRhoGenBugFix/211104_112116//0000//


#
#PbPbData4=/eos/cms/store/group/phys_heavyions/uacharya/JetRAA_PbPb_2024/HIPhysicsRawPrime1//HIRun2024B-PromptReco-v1/MINIAOD/HIPhysicsRawPrime1/JetRAA_pbpb2024B_v1_Prime1_jetRAA_015_02/250216_194226/0000/
PPData1=/eos/cms/store/group/phys_heavyions/uacharya/JetRAA_ppRef_2024/PPRefHardProbes2/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes2/ppRef2024_jetRAA_AK015_AK02_probe2/250519_160249/

#PPMC1=root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/uacharya/ppRef2024_MC/ppRef_2024PrivateMC_5p36TeV/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/

#PbPbMC4=/eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest//DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/DefaultPbPbMCForJetRAAWithRhoGenBugFix/211104_112116//0000//

TestRun: Execute
	# arguments = file tag trigger MC? PP? Recluster? R? LowMinPT? PhiResidual? Exclusion? Centrality? CentralityShift?
	# bash RunFile.sh `ls ExamplePbPbMC/Output*.root | head -n10 | tr '\n' ','` MeowTestRunMC None 1 0 0
	# bash RunFile.sh ExamplePbPbData/OutputAll.root MeowTestRunData HLT_HIPuAK4CaloJet100Eta5p1 0 0 0
	# bash RunFile.sh `ls PPMC/*.root | head -n3 | tr '\n' ','`   MeowTestRunPPMC   None             1 1 1
	# bash RunFile.sh `ls PPData/*.root | head -n1 | tr '\n' ','` MeowTestRunPPData HLT_HIAK4PFJet80 0 1 1
	# bash RunFileOneR.sh `ls ExamplePbPbMC/Output*.root | head -n10 | tr '\n' ','` MeowTestRunMC None 1 0 0 4 1 1 Inclusive
	 bash RunFileOneR.sh `ls $(PbPbData4)/*.root | head -n2 | tr '\n' ','` MeowTestRunMC2 HLT_HIPuAK4CaloJet100Eta5p1 0 0 0 4 1 1 0to10 1
	# bash RunFileOneR.sh `ls $(PbPbMC4)/*.root | head -n2 | tr '\n' ','` MeowTestRunMCRho None 1 0 0 4 0 1 1 rho default
	# bash RunFileOneR.sh `ls $(PbPbMC4)/*.root | head -n2 | tr '\n' ','` MeowTestRunMCRhoUp None 1 0 0 4 0 1 1 rhoup default
	# bash RunFileOneR.sh `ls $(PbPbMC4)/*.root | head -n2 | tr '\n' ','` MeowTestRunMCRhoDownNoSkip None 1 0 0 4 0 0 1 1 rhodown default
	# bash RunFileOneR.sh `ls $(PPData4)/*.root | head -n2 | tr '\n' ','` MeowTestRunPPData HLT_HIAK4PFJet80 0 1 1 4 0 0 default

ULTreeValidation: Execute
	# ./Execute --Input HiForestMiniAOD.root --Output ULFromJet_R4.root --Jet "ak4PFJetAnalyzer/t" \
	# 	--JetR 0.4 --CheckCentrality false --UseStoredGen false --UseStoredReco true \
	# 	--DoRecoSubtraction false
	# ./Execute --Input HiForestMiniAOD.root --Output ULFromPF_R4.root --Jet "ak4PFJetAnalyzer/t" \
	# 	--JetR 0.4 --CheckCentrality false --UseStoredGen false --UseStoredReco false \
	# 	--PF "particleFlowAnalyser/pftree"
	bash RunULValidation.sh

Execute: ProcessJets.cpp
	g++ `root-config --cflags --libs` ProcessJets.cpp -o Execute \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o \
		`$(FastJet) --cxxflags --libs` \
		-L$(FJContrib)/lib -I$(FJContrib)/include -lConstituentSubtractor

ExecuteVector: ProcessJetsVector.cpp
	g++ `root-config --cflags --libs` ProcessJetsVector.cpp -o ExecuteVector \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o \
		`$(FastJet) --cxxflags --libs` \
		-L$(FJContrib)/lib -I$(FJContrib)/include -lConstituentSubtractor

SubmitBeforeRho:
	#bash GenerateSubmissionOneR.sh "PbPbData PPMC PPData PbPbMCJEC PbPbMCInclusive" "1 2 3 4 5 6 7 8 9"
	#bash GenerateSubmissionOneR.sh "PbPbData " "4"
	#bash GenerateSubmissionOneR.sh "PbPbData " "1 2 3 4 6 7 8 9"
	#bash GenerateSubmissionOneR.sh "PPData " "1 2 3 4 6 7 8 9"
	bash GenerateSubmissionOneR.sh "PPData " "4"
	#bash GenerateSubmissionOneR.sh "PbPbMC " "1 2 3 4 6 7 8 9"
	#bash GenerateSubmissionOneR.sh "PbPbMC " "4"
	#bash GenerateSubmissionOneR.sh "PbPbMCInclusive" "4"
	#bash GenerateSubmissionOneR.sh "PPMC " "1 2 3 4 6 7 8 9"
	#bash GenerateSubmissionOneR.sh "PPMC " "4"
	#bash GenerateSubmissionOneR.sh "PbPbDataCentralityUp " "1 2 3 4 6 7 8 9"
	#bash GenerateSubmissionOneR.sh "PbPbDataCentralityDown " "1 2 3 4 6 7 8 9"
	# condor_submit Submit.condor

SubmitAfterRho:
	#bash GenerateSubmissionOneR.sh "PbPbMCRho" "1 2 3 4 6 7 8 9"
	bash GenerateSubmissionOneR.sh "PbPbMCRho" "4"
	# condor_submit Submit.condor

MergeFiles:
	bash MergeParts.sh

CleanAndMergeFiles:
	# Will clean out the temporary root files in the Parts/ directory!  Wait 10 seconds for safety...
	sleep 10
	rm Output/Parts/*root
	bash MergeParts.sh



