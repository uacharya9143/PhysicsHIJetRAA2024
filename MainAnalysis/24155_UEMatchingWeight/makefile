
# default: FitAll #TestRunMatch TestRunFit
default: RunVisualize

Prepare:
	rm -f GlobalSetting.dh Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
#	ln -sf /eos/user/c/chenyi//BatchOutput/PhysicsWorkspace/HIJetRAA2018/MainAnalysis/23752_CopyJetFromForest// Input
	ln -sf /afs/cern.ch/user/u/uacharya/eos/cms_analaysis/JetRAA_Run3/PhysicsHIJetRAA2024/MainAnalysis/23752_CopyJetFromForest// Input

TestRun: Execute
	./Execute --Data Input/Output_PbPbData_PPData/PbPbData_R4_Centrality50to90.root \
		--MC Input/Output_PbPbMC/PbPbMC_R4_CentralityInclusive.root \
		--JetMinPT 131.387 --Output root/Output_R4_Centrality50to90.root

TestRunMatch: ExecuteMatch
	./ExecuteMatch --Input root/Output_R1_Centrality50to90.root \
		--Output pdf/Output_R1_Centrality50to90.pdf \
		--RootOutput graph/Output_R1_Centrality50to90.root \
		--BinCount 100 --BinMagnification 25 --SkipFinalBin true

Execute: StripTree.cpp
	g++ StripTree.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

ExecuteMatch: MatchUE.cpp
	g++ MatchUE.cpp -o ExecuteMatch `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

RunAll: Execute
	bash RunFile.sh
	#-I$(ProjectBase)/CommonCode/include \
	#$(ProjectBase)/CommonCode/library/*.o 

ExecuteFit: FitRatio.cpp
	g++ FitRatio.cpp -o ExecuteFit `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

TestRunFit: ExecuteFit
	./ExecuteFit --Input graph/Output_R1_Centrality50to90.root \
		--Output pdf/Fit_R1_Centrality50to90.pdf \
		--DHFile GlobalSetting.dh --Key R1_Centrality50to90 --Order1 8 --Order2 0

FitAll: ExecuteMatch ExecuteFit
	bash BinFile.sh
	bash FitFile.sh

ExecuteVisualize: VisualizeDifference.cpp
	g++ VisualizeDifference.cpp -o  ExecuteVisualize `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

RunVisualize: ExecuteVisualize
	./ExecuteVisualize --DHFile GlobalSetting.dh --DHFileNew GlobalSettingTemp.dh --Output RhoWeightComparison.pdf

