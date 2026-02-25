
default: TestRunSummary

Prepare:
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	mkdir -p pdf

#ExampleFile = /eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest//DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/DefaultPbPbMCForJetRAAWithRhoGenBugFix/211104_112116/

ExampleFile = /eos/cms/store/group/phys_heavyions/uacharya/JetRAA_PbPb_2024/HIPhysicsRawPrime1//HIRun2024B-PromptReco-v1/MINIAOD/HIPhysicsRawPrime1/JetRAA_pbpb2024B_v1_Prime1_jetRAA_03_04/250216_195227/0000/

TestRun: Execute
	./Execute --Input `find $(ExampleFile) | grep root | head -n5 | tr '\n' ','` \
		--Tree1 akCs4PFJetAnalyzer/t \
		--Tree2 akCs4PFJetAnalyzer/t \
		--DHFile GlobalSetting.dh \
		--MatchAngle 0.15 \
		--EtaMin -2 --EtaMax 2 \
		--Output TestOutput.pdf \
		--NSample 100000 \
		--DHOutput Rho.dh --DHState PbPbR4R4

Execute: Uncertainty.cpp
	g++ Uncertainty.cpp -o Execute `root-config --libs --cflags` \
	-I../../CommonCode/include/ \
	../../CommonCode/library/*.o

RunAll: Execute
	bash RunAll.sh


ExecuteSummary: SummaryPlots.cpp
	g++ SummaryPlots.cpp -o ExecuteSummary `root-config --libs --cflags`

TestRunSummary: ExecuteSummary
	./ExecuteSummary


