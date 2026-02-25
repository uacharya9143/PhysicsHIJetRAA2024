default: RunJetPT RunJetPTClosure

Prepare:
	rm -f OriginalInput Input Systematics HEPData
# 	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output OriginalInput
# 	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output Input
# 	ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output Systematics

	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output_R4 OriginalInput
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output_R4 Systematics

	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	ln -sf $(ProjectBase)/MainAnalysis/24113_HEPData/ HEPData
	mkdir -p Plots Root
	mkdir -p Plots_R4 Root_R4

Clean:
	rm -f Plots/*pdf
	rm -f Root/*root

RunJetPT: Execute
	bash RunFile.sh PbPbData Nominal "" 0 Gen
	bash RunFile.sh PPData   Nominal "" 1 Gen

	bash RunFile.sh PPData   Nominal "" 1 InputReco
	bash RunFile.sh PPData   Nominal "" 1 Reco
	bash RunFile.sh PbPbData Nominal "" 0 InputReco
	bash RunFile.sh PbPbData Nominal "" 0 Reco

RunJetPTReco: Execute
	bash RunFile.sh PbPbData  Nominal ""  0 Reco
	bash RunFile.sh PPData    Nominal ""  1 Reco
	bash RunFile.sh PbPbMCRho Nominal Toy 0 Reco
	bash RunFile.sh PPMC      Nominal Toy 1 Reco
	bash RunFile.sh PbPbData  Nominal ""  0 InputReco
	bash RunFile.sh PPData    Nominal ""  1 InputReco
	bash RunFile.sh PbPbMCRho Nominal Toy 0 InputReco
	bash RunFile.sh PPMC      Nominal Toy 1 InputReco
	bash RunFile.sh PbPbData  Nominal ""  0 InputRecoGenBin
	bash RunFile.sh PPData    Nominal ""  1 InputRecoGenBin
	bash RunFile.sh PbPbMCRho Nominal Toy 0 InputRecoGenBin
	bash RunFile.sh PPMC      Nominal Toy 1 InputRecoGenBin

RunCheck: Execute
	bash RunFile.sh PbPbData JERUp              "" 0 Gen
	bash RunFile.sh PbPbData JERDown            "" 0 Gen
	bash RunFile.sh PbPbData FlatJER            "" 0 Gen
	bash RunFile.sh PbPbData NoJER              "" 0 Gen
	bash RunFile.sh PbPbData JECUp              "" 0 Gen
	bash RunFile.sh PbPbData JECDown            "" 0 Gen
	bash RunFile.sh PbPbData JECQuench          "" 0 Gen
	bash RunFile.sh PbPbData JECAggressiveUp    "" 0 Gen
	bash RunFile.sh PbPbData JECAggressiveDown  "" 0 Gen
	bash RunFile.sh PbPbData JECNoResidual      "" 0 Gen
	bash RunFile.sh PbPbData JECNoPhi           "" 0 Gen
	bash RunFile.sh PbPbData CentralityUp       "" 0 Gen
	bash RunFile.sh PbPbData CentralityDown     "" 0 Gen
	bash RunFile.sh PbPbData MCCentrality       "" 0 Gen
	bash RunFile.sh PbPbData StoredCentrality   "" 0 Gen
	bash RunFile.sh PbPbData NominalEarthquake  "" 0 Gen
	bash RunFile.sh PbPbData NominalBinBiasUp   "" 0 Gen
	bash RunFile.sh PbPbData NominalBinBiasDown "" 0 Gen
	bash RunFile.sh PbPbData NominalCleaned     "" 0 Gen
	bash RunFile.sh PbPbData NominalCleanedStrong "" 0 Gen
	bash RunFile.sh PbPbData NoJetID            "" 0 Gen
	bash RunFile.sh PbPbData JECNoResidual      "" 0 InputReco
	bash RunFile.sh PbPbData JECNoPhi           "" 0 InputReco
	bash RunFile.sh PbPbData JECNoResidual      "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData JECNoPhi           "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData MCCentrality       "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData StoredCentrality   "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData NoJetID            "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData JECUp              "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData JECDown            "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData JECAggressiveUp    "" 0 InputRecoGenBin
	bash RunFile.sh PbPbData JECAggressiveDown  "" 0 InputRecoGenBin

RunJetPTClosure: Execute
	bash RunFile.sh PbPbMCRho Nominal Toy 0 Gen
	bash RunFile.sh PPMC      Nominal Toy 1 Gen
	
	bash RunFile.sh PPMC      Nominal Toy 1 InputReco
	bash RunFile.sh PbPbMCRho Nominal Toy 0 InputReco
	bash RunFile.sh PbPbMCRho Nominal Toy 0 Reco
	bash RunFile.sh PPMC      Nominal Toy 1 Reco

Execute: MakePaperPlot.cpp
	g++ MakePaperPlot.cpp -o Execute  `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

Deploy:
	mkdir -p $(DeployBase)/Result/Spectra/
	cp Plots/PbPbData*Nominal.pdf $(DeployBase)/Result/Spectra/
	cp Plots/PPData*Nominal.pdf $(DeployBase)/Result/Spectra/
	mkdir -p $(DeployBase)/Check/RefoldCheck/
	cp Plots/PbPbData*Nominal_Reco.pdf $(DeployBase)/Check/RefoldCheck/
	cp Plots/PPData*Nominal_Reco.pdf $(DeployBase)/Check/RefoldCheck/
	mkdir -p $(DeployBase)/Check/ToyRefoldCheck/
	cp Plots/PbPbMCRho*Toy_Reco.pdf $(DeployBase)/Check/ToyRefoldCheck/
	cp Plots/PPMC*Toy_Reco.pdf $(DeployBase)/Check/ToyRefoldCheck/
	mkdir -p $(DeployBase)/Check/Spectra/MatrixCheck
	cp Plots/PbPbData*NominalEarthquake.pdf  $(DeployBase)/Check/Spectra/MatrixCheck/
	cp Plots/PbPbData*NominalBinBiasUp.pdf   $(DeployBase)/Check/Spectra/MatrixCheck/
	cp Plots/PbPbData*NominalBinBiasDown.pdf $(DeployBase)/Check/Spectra/MatrixCheck/
	cp Plots/PbPbData*NominalCleaned.pdf     $(DeployBase)/Check/Spectra/MatrixCheck/

DeployPaper:
	mkdir -p "$(DeployBasePaper)/Result/Spectra/"
	cp Plots/PbPbData*Nominal.pdf "$(DeployBasePaper)/Result/Spectra/"
	cp Plots/PPData*Nominal.pdf "$(DeployBasePaper)/Result/Spectra/"

DeployRoot:
	mkdir -p $(DeployBase)/Result/Graphs
	cp Root/*_Nominal.root $(DeployBase)/Result/Graphs/

