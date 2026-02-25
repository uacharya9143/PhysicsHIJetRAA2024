Group = --Group 1 --Row 1 --Column 1

default: RunTest

Prepare:
	rm -f Input Systematics
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output/ Input
	ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output/ Systematics
	ln -sf $(ProjectBase)/MainAnalysis/24165_SystematicsCombination/Output/ CombinedSystematics
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/Systematics.dh .
	mkdir -p Plots FinalPlots

Clean:
	rm -f Plots/*pdf
	rm -f FinalPlots/*pdf

RunTest: Execute
	./Execute --Input Systematics/PPData_R4_CentralityInclusive.root \
		--Output Plots/PPData_R4_CentralityInclusive.pdf \
		--FinalOutput FinalPlots/PPData_R4_CentralityInclusive.pdf \
		--GenPrimaryMin 0 --GenPrimaryMax 1600 \
		--WorldXMin 100 --WorldXMax 1500 --WorldYMin 0 --WorldYMax 1.0 --LogY false --LogX true \
		--XLabel "Jet p_{T}" --YLabel "Uncertainty" --Binning "none" \
		--LegendX 0.12 --LegendY 0.5 --LegendSize 0.05 \
		--XAxis 305 --YAxis 505 --RAxis 303 --MarkerModifier 1 \
		--Texts 0,0.12,0.88,"Anti-k_{T} jet R = 0.4",0,0.12,0.83,"|#eta| < 2.0" \
		$(Group) \
		--Variations HJECUp,HJECDown,HJERUp,HJERDown \
		--SystematicGroups 1,1,2,2 \
		--Labels "JEC","JER"

RunAll: Execute
	bash RunAll.sh

Execute: MakePaperPlot.cpp
	g++ MakePaperPlot.cpp -o Execute  `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

Deploy:
	mkdir -p $(DeployBase)/Systematics/Spectra
	cp FinalPlots/PbPbData_*pdf $(DeployBase)/Systematics/Spectra
	cp FinalPlots/PPData_*pdf $(DeployBase)/Systematics/Spectra
	mkdir -p $(DeployBase)/Systematics/PPRatio
	cp FinalPlots/PPDataRatio_*pdf $(DeployBase)/Systematics/PPRatio
	mkdir -p $(DeployBase)/Systematics/RAA
	cp FinalPlots/RAA_*pdf $(DeployBase)/Systematics/RAA
	mkdir -p $(DeployBase)/Systematics/RCP
	cp FinalPlots/RCP_*pdf $(DeployBase)/Systematics/RCP
	mkdir -p $(DeployBase)/Systematics/RRAA
	cp FinalPlots/RRAA_*pdf $(DeployBase)/Systematics/RRAA

DeployPaper:
	mkdir -p "$(DeployBasePaper)/Systematics/Spectra"
	cp FinalPlots/PbPbData_*pdf "$(DeployBasePaper)/Systematics/Spectra"
	cp FinalPlots/PPData_*pdf "$(DeployBasePaper)/Systematics/Spectra"
	mkdir -p "$(DeployBasePaper)/Systematics/PPRatio"
	cp FinalPlots/PPDataRatio_*pdf "$(DeployBasePaper)/Systematics/PPRatio"
	mkdir -p "$(DeployBasePaper)/Systematics/RAA"
	cp FinalPlots/RAA_*pdf "$(DeployBasePaper)/Systematics/RAA"
	mkdir -p "$(DeployBasePaper)/Systematics/RCP"
	cp FinalPlots/RCP_*pdf "$(DeployBasePaper)/Systematics/RCP"
	mkdir -p "$(DeployBasePaper)/Systematics/RRAA"
	cp FinalPlots/RRAA_*pdf "$(DeployBasePaper)/Systematics/RRAA"



