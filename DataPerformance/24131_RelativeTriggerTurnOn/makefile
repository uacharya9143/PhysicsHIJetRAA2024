
#FastJet   = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet/3.3.4-ghbfee/bin/fastjet-config
#FJContrib = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet-contrib/1.044-ghbfee/
FastJet = /cvmfs/cms.cern.ch/el9_amd64_gcc12/external/fastjet/3.4.1-cc6ce46a28fb676c9080adb13b99f03c/bin/fastjet-config
FJContrib = /cvmfs/cms.cern.ch/el9_amd64_gcc12/external/fastjet-contrib/1.051-afd609f10b5f81984bb8b0a86a7935ee/

#PbPbData1 = /eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit12/211020_104744/0000/
PPData4 =/eos/cms/store/group/phys_heavyions/uacharya/JetRAA_ppRef_2024//PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK03_AK04_probe0/250525_132912/0000/
#PbPbData4 =/eos/cms/store/group/phys_heavyions/uacharya/JetRAA_PbPb_2024/HIPhysicsRawPrime1//HIRun2024B-PromptReco-v1/MINIAOD/HIPhysicsRawPrime1/JetRAA_pbpb2024B_v1_Prime1_jetRAA_015_02/250216_194226/0000/
JECBase   = $(ProjectBase)/CommonCode/jec/
#JECTag    = Autumn18_HI_RAAV2_MC
JECTag    = Summer2024_ppRef5p36TeV_MC

default: Prepare TestRun

Prepare:
	#rm -f PbPbData1
	rm -f PPData4
	#ln -sf /eos/cms/store/group/phys_heavyions/chenyi/PbPb2018/Forest/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit12/211020_104744/0000/ PbPbData1
	ln -sf /eos/cms/store/group/phys_heavyions/uacharya/JetRAA_ppRef_2024/PPRefHardProbes2/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes2/ppRef2024_jetRAA_AK03_AK04_probe2/250519_160345/0000/ PPData4

	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .

Execute: MakeTurnOn.cpp
		g++  MakeTurnOn.cpp -o Execute `root-config --libs --cflags` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o \
		`$(FastJet) --cxxflags --libs` \
		-L$(FJContrib)/lib -I$(FJContrib)/include -lConstituentSubtractor

ExecuteFit: FitTurnOn.cpp
	g++ FitTurnOn.cpp -o ExecuteFit `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

TestRun: Execute
	time ./Execute --Input `ls $(PPData4)/*root | head -n10 | tr '\n' ','` \
		--Base HLT_AK4PFJet100 \
		--Trigger HLT_AK4PFJet100 \
		--Jet "ak4PFJetAnalyzer/t" \
		--CentralityMin 0.0 --CentralityMax 0.1 \
		--EtaMin -2.0 --EtaMax 2.0 \
		--DoBaselineCutAA true --DoBaselineCutPP false \
		--JEC "$(JECBase)/$(JECTag)/$(JECTag)_L2Relative_AK4PF.txt","$(JECBase)/$(JECTag)/$(JECTag)_L2L3Residual_AK4PF.txt"

ProcessAll: Execute
	bash Run_pbpb.sh
	bash Run_pp.sh

TestFit: ExecuteFit
	./ExecuteFit --Input Result_R1_Centrality0to10.root --Output Plots_R1_Centrality0to10.pdf \
		--DHFile GlobalSettingTest.dh --State PbPbTriggerTurnOn --KeyPrefix R1_Centrality0to10

ExecuteSummary: PlotPercentage.cpp
	g++  PlotPercentage.cpp -o ExecuteSummary `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

Harvest: ExecuteSummary ExecuteFit
	-bash RunFit.sh
	./ExecuteSummary --DHFile GlobalSetting.dh
	##########################################################################
	#                                                                        #
	#   MAKE SURE THAT YOU COPY GlobalSettingTemp.dh into GlobalSetting.dh   #
	#                                                                        #
	##########################################################################
#		--Trigger HLT_HIPuAK4CaloJet100Eta5p1 \
