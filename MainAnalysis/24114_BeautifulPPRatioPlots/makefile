default: RunJetPT

Prepare:
	rm -f OriginalInput Input Systematics HEPData CombinedSystematics
	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output OriginalInput
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output Systematics
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	ln -sf $(ProjectBase)/MainAnalysis/24114_PPRatioHEPData/ HEPData
	ln -sf $(ProjectBase)/MainAnalysis/24165_SystematicsCombination/Output/ CombinedSystematics
	ln -sf $(ProjectBase)/MainAnalysis/24263_StatisticalUncertaintyCancellation/StatisticsRho.dh . #disabled by uttam
	mkdir -p Plots

Clean:
	rm -f Plots/*

RunJetPT: Execute
	bash RunJetPT.sh

Execute: MakePlot.cpp
	g++ MakePlot.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
Deploy:
	mkdir -p $(DeployBase)/Result/PPRatio/
	cp Plots/* $(DeployBase)/Result/PPRatio/

DeployPaper:
	mkdir -p "$(DeployBasePaper)/Result/PPRatio/"
	cp Plots/* "$(DeployBasePaper)/Result/PPRatio/"


