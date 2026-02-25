
default: RunRAA RunRRAA RunRCP Deploy

Prepare:
	rm -f SpectraRoot CombinedSystematics
	#ln -sf $(ProjectBase)/MainAnalysis/23763_BeautifulPlots/Root SpectraRoot
	ln -sf $(ProjectBase)/MainAnalysis/23763_BeautifulPlots/Root_R4 SpectraRoot
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	#ln -sf $(ProjectBase)/MainAnalysis/24165_SystematicsCombination/Output/ CombinedSystematics
	ln -sf $(ProjectBase)/MainAnalysis/24165_SystematicsCombination/Output_R4/ CombinedSystematics
	ln -sf $(ProjectBase)/MainAnalysis/24263_StatisticalUncertaintyCancellation/StatisticsRho.dh . #disabled by uttam
	mkdir -p Plots Root
	mkdir -p Plots_R4 Root_R4

Clean:
	rm -f Plots/*pdf Root/*root

RunRAA: Execute
	bash RunFile.sh PPData Nominal PbPbData Nominal
	bash RunFile.sh PPData Nominal PbPbData Nominal_Reco
	bash RunFile.sh PPData Nominal PbPbData Nominal_InputReco

RunRAACheck: RunRAAVariationCheck RunRAARecoCheck

RunRAAVariationCheck: Execute
	bash RunFile.sh PPData Nominal PbPbData JERUp
	bash RunFile.sh PPData Nominal PbPbData JERDown
	bash RunFile.sh PPData Nominal PbPbData FlatJER
	bash RunFile.sh PPData Nominal PbPbData NoJER
	bash RunFile.sh PPData Nominal PbPbData JECUp
	bash RunFile.sh PPData Nominal PbPbData JECDown
	bash RunFile.sh PPData Nominal PbPbData JECAggressiveUp
	bash RunFile.sh PPData Nominal PbPbData JECAggressiveDown
	bash RunFile.sh PPData Nominal PbPbData JECQuench
	bash RunFile.sh PPData Nominal PbPbData JECNoResidual
	bash RunFile.sh PPData Nominal PbPbData JECNoPhi
	bash RunFile.sh PPData Nominal PbPbData CentralityUp
	bash RunFile.sh PPData Nominal PbPbData CentralityDown
	bash RunFile.sh PPData Nominal PbPbData MCCentrality
	bash RunFile.sh PPData Nominal PbPbData StoredCentrality
	bash RunFile.sh PPData Nominal PbPbData NominalEarthquake 
	bash RunFile.sh PPData Nominal PbPbData NominalCleaned
	bash RunFile.sh PPData Nominal PbPbData NominalCleanedStrong
	bash RunFile.sh PPData Nominal PbPbData NominalBinBiasUp
	bash RunFile.sh PPData Nominal PbPbData NominalBinBiasDown
	bash RunFile.sh PPData Nominal PbPbData NoJetID

RunRAARecoCheck: Execute
	bash RunFile.sh PPData Nominal_Reco PbPbData Nominal_Reco
	bash RunFile.sh PPData Nominal_InputReco PbPbData Nominal_InputReco
	bash RunFile.sh PPData Nominal_InputReco PbPbData JECNoResidual_InputReco
	bash RunFile.sh PPData Nominal_InputReco PbPbData JECNoPhi_InputReco
	bash RunFile.sh PPData Nominal PbPbData Nominal_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData Nominal_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData StoredCentrality_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECNoResidual_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECNoPhi_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData NoJetID_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECUp_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECDown_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECAggressiveUp_InputRecoGenBin
	bash RunFile.sh PPData Nominal_InputRecoGenBin PbPbData JECAggressiveDown_InputRecoGenBin

RunRCP: Execute
	bash RunRCP.sh PPData Nominal PbPbData Nominal

RunRRAAOld: Execute
	bash RunRRAA.sh PPData Nominal PbPbData Nominal

RunRRAA: Execute
	bash RunSpectraRatio.sh PPData Nominal PbPbData Nominal
	bash RunRRAAAlternate.sh PPData Nominal PbPbData Nominal

Execute: PlotRAA.cpp
	g++ PlotRAA.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

Deploy:
	mkdir -p $(DeployBase)/Result/RAA
	cp Plots/RAA*_PPNominal_AANominal.pdf $(DeployBase)/Result/RAA/
	mkdir -p $(DeployBase)/Result/RCP
	cp Plots/RCP*_AANominal.pdf $(DeployBase)/Result/RCP/
	mkdir -p $(DeployBase)/Result/RRAA
	cp Plots/RRAA*_PPNominal_AANominal.pdf $(DeployBase)/Result/RRAA/
	mkdir -p $(DeployBase)/Check/RAA/JERVariation
	cp Plots/RAA*_PPNominal_AAJERUp.pdf $(DeployBase)/Check/RAA/JERVariation/
	cp Plots/RAA*_PPNominal_AAJERDown.pdf $(DeployBase)/Check/RAA/JERVariation/
	mkdir -p $(DeployBase)/Check/RAA/JERExtremeCheck
	cp Plots/RAA*_PPNominal_AAFlatJER.pdf $(DeployBase)/Check/RAA/JERExtremeCheck/
	cp Plots/RAA*_PPNominal_AANoJER.pdf $(DeployBase)/Check/RAA/JERExtremeCheck/
	mkdir -p $(DeployBase)/Check/RAA/JECVariation
	cp Plots/RAA*_PPNominal_AAJECUp.pdf $(DeployBase)/Check/RAA/JECVariation/
	cp Plots/RAA*_PPNominal_AAJECDown.pdf $(DeployBase)/Check/RAA/JECVariation/
	cp Plots/RAA*_PPNominal_AAJECQuench.pdf $(DeployBase)/Check/RAA/JECVariation/
	mkdir -p $(DeployBase)/Check/RAA/JECExtremeCheck
	cp Plots/RAA*_PPNominal_AAJECAggressiveUp.pdf $(DeployBase)/Check/RAA/JECExtremeCheck/
	cp Plots/RAA*_PPNominal_AAJECAggressiveDown.pdf $(DeployBase)/Check/RAA/JECExtremeCheck/
	cp Plots/RAA*_PPNominal_AAJECNoPhi.pdf $(DeployBase)/Check/RAA/JECExtremeCheck/
	cp Plots/RAA*_PPNominal_AAJECNoResidual.pdf $(DeployBase)/Check/RAA/JECExtremeCheck/
	mkdir -p $(DeployBase)/Check/RAA/CentralityVariation
	cp Plots/RAA*_PPNominal_AACentralityUp.pdf $(DeployBase)/Check/RAA/CentralityVariation/
	cp Plots/RAA*_PPNominal_AACentralityDown.pdf $(DeployBase)/Check/RAA/CentralityVariation/
	mkdir -p $(DeployBase)/Check/RAA/CentralityFromMC
	cp Plots/RAA*_PPNominal_AAStoredCentrality.pdf $(DeployBase)/Check/RAA/CentralityFromMC/
	cp Plots/RAA*_PPNominal_AAMCCentrality.pdf $(DeployBase)/Check/RAA/CentralityFromMC/
	mkdir -p $(DeployBase)/Check/RAA/Cleaning
	cp Plots/RAA*_PPNominal_AANominalCleaned.pdf $(DeployBase)/Check/RAA/Cleaning/
	cp Plots/RAA*_PPNominal_AANominalCleanedStrong.pdf $(DeployBase)/Check/RAA/Cleaning/
	mkdir -p $(DeployBase)/Check/RAA/JetSelection
	cp Plots/RAA*_PPNominal_AANoJetID.pdf $(DeployBase)/Check/RAA/JetSelection/
	mkdir -p $(DeployBase)/Check/RAA/BinBias
	cp Plots/RAA*_PPNominal_AANominalBinBiasUp.pdf $(DeployBase)/Check/RAA/BinBias/
	cp Plots/RAA*_PPNominal_AANominalBinBiasDown.pdf $(DeployBase)/Check/RAA/BinBias/
	mkdir -p $(DeployBase)/Check/RAA/Earthquake
	cp Plots/RAA*_PPNominal_AANominalEarthquake.pdf $(DeployBase)/Check/RAA/Earthquake/
	mkdir -p $(DeployBase)/Check/RAA/Refolded
	cp Plots/RAA*_PPNominal_Reco_AANominal_Reco.pdf $(DeployBase)/Check/RAA/Refolded/
	mkdir -p $(DeployBase)/Check/RAA/RecoLevel
	# cp Plots/RAA*_PPNominal_InputReco_AANominal_InputReco.pdf $(DeployBase)/Check/RAA/RecoLevel/
	# cp Plots/RAA*_PPNominal_InputReco_AAJECNoPhi_InputReco.pdf $(DeployBase)/Check/RAA/RecoLevel/
	# cp Plots/RAA*_PPNominal_InputReco_AAJECNoResidual_InputReco.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AANominal_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAStoredCentrality_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECNoPhi_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECNoResidual_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AANoJetID_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECUp_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECDown_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECAggressiveUp_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	cp Plots/RAA*_PPNominal_InputRecoGenBin_AAJECAggressiveDown_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/
	mkdir -p $(DeployBase)/Check/RAA/RecoGenMix
	cp Plots/RAA*_PPNominal_AANominal_InputRecoGenBin.pdf $(DeployBase)/Check/RAA/RecoLevel/

DeployRoot:
	mkdir -p $(DeployBase)/Result/Graphs/
	cp Root/*_AANominal.root $(DeployBase)/Result/Graphs/

DeployPaper:
	mkdir -p "$(DeployBasePaper)/Result/RAA"
	cp Plots/RAA*_PPNominal_AANominal.pdf "$(DeployBasePaper)/Result/RAA/"
	mkdir -p "$(DeployBasePaper)/Result/RCP"
	cp Plots/RCP*_AANominal.pdf "$(DeployBasePaper)/Result/RCP/"
	mkdir -p "$(DeployBasePaper)/Result/RRAA"
	cp Plots/RRAA*_PPNominal_AANominal.pdf "$(DeployBasePaper)/Result/RRAA/"

