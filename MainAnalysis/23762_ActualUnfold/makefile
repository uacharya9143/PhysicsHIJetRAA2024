
default: RunTest

Prepare:
	rm -f Input ToyInput
	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output_R4/ Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_FakeData/Output_R4/ ToyInput
	ln -sf $(ProjectBase)/MainAnalysis/23762_UnfoldPreparation/Output_R4/ InputUncert

# 	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output/ Input
# 	ln -sf $(ProjectBase)/MainAnalysis/23762_FakeData/Output/ ToyInput
# 	ln -sf $(ProjectBase)/MainAnalysis/23762_UnfoldPreparation/Output/ InputUncert

	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	mkdir -p Output Plots

Clean:
	rm -f Output/*root
	rm -f Plots/*pdf

Execute: Unfold1D.cpp
	 g++ Unfold1D.cpp -o Execute -lRooUnfold  `root-config --libs --cflags` \
	-L$(ProjectBase)/RooUnfold \
	-I$(ProjectBase)/CommonCode/include \
	-I$(ProjectBase)/RooUnfold/src \
	$(ProjectBase)/CommonCode/library/*.o 
	#$(ProjectBase)/RooUnfold/build/libRooUnfold.so 

ExecutePlot: Visualize.cpp
	 g++ Visualize.cpp -o ExecutePlot  `root-config --libs --cflags` \
	-L$(ProjectBase)/RooUnfold \
	-I$(ProjectBase)/CommonCode/include \
	-I$(ProjectBase)/RooUnfold/src \
	$(ProjectBase)/CommonCode/library/*.o 
	#$(ProjectBase)/RooUnfold/build/libRooUnfold.so 

RunMCToy: Execute ExecutePlot
	bash RunFile.sh ToyInput PPMC      Nominal_Toy                   1 DefaultStep1
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy                   0 DefaultStep1

RunDataStep1: Execute ExecutePlot
	bash RunFile.sh Input    PbPbData  Nominal                       0 DefaultStep1
	bash RunFile.sh Input    PPData    Nominal                       1 DefaultStep1

RunDataStep1Toy: Execute ExecutePlot
	bash RunFile.sh ToyInput PbPbData  Nominal_DefaultStep1Prior_Toy 0 DefaultStep1
	bash RunFile.sh ToyInput PPData    Nominal_DefaultStep1Prior_Toy 1 DefaultStep1

RunDataStep2: Execute ExecutePlot
	bash RunFile.sh Input    PbPbData  Nominal                       0 DefaultStep2
	bash RunFile.sh Input    PPData    Nominal                       1 DefaultStep2

RunDataStep2Toy: Execute ExecutePlot
	bash RunFile.sh ToyInput PbPbData  Nominal_DefaultStep2Prior_Toy 0 DefaultStep2
	bash RunFile.sh ToyInput PPData    Nominal_DefaultStep2Prior_Toy 1 DefaultStep2

RunAAStep1: Execute ExecutePlot
	bash RunFile.sh Input    PbPbMCRho Nominal                       0 DefaultStep1
	bash RunFile.sh InputUncert    PbPbData  JECUp                         0 DefaultStep1
	bash RunFile.sh InputUncert    PbPbData  JECDown                       0 DefaultStep1
	bash RunFile.sh InputUncert    PbPbData  JERUp                         0 DefaultStep1
	bash RunFile.sh InputUncert    PbPbData  JERDown                       0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  JECAggressiveUp               0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  JECAggressiveDown             0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  JECQuench                     0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  FlatJER                       0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NoJER                         0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  CentralityUp                  0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  CentralityDown                0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  MCCentrality                  0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  StoredCentrality              0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  JECNoResidual                 0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  JECNoPhi                      0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NominalEarthquake             0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NominalCleaned                0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NominalCleanedStrong          0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NominalBinBiasUp              0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NominalBinBiasDown            0 DefaultStep1
	#bash RunFile.sh Input    PbPbData  NoJetID                       0 DefaultStep1#disabled for now no Input//Uttam

RunAAStep2: Execute ExecutePlot
	bash RunFile.sh Input    PbPbMCRho Nominal                       0 DefaultStep1
	bash RunFile.sh Input    PbPbData  JECUp                         0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECDown                       0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECAggressiveUp               0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECAggressiveDown             0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECQuench                     0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JERUp                         0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JERDown                       0 DefaultStep2
	bash RunFile.sh Input    PbPbData  FlatJER                       0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NoJER                         0 DefaultStep2
	bash RunFile.sh Input    PbPbData  CentralityUp                  0 DefaultStep2
	bash RunFile.sh Input    PbPbData  CentralityDown                0 DefaultStep2
	bash RunFile.sh Input    PbPbData  MCCentrality                  0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECNoResidual                 0 DefaultStep2
	bash RunFile.sh Input    PbPbData  JECNoPhi                      0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NominalEarthquake             0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NominalCleaned                0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NominalBinBiasUp              0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NominalBinBiasDown            0 DefaultStep2
	bash RunFile.sh Input    PbPbData  NoJetID                       0 DefaultStep2

RunPPStep1: Execute ExecutePlot
	bash RunFile.sh Input    PPMC      Nominal                       1 DefaultStep1
	bash RunFile.sh InputUncert    PPData    JECUp                         1 DefaultStep1
	bash RunFile.sh InputUncert    PPData    JECDown                       1 DefaultStep1
	bash RunFile.sh InputUncert    PPData    JERUp                         1 DefaultStep1
	bash RunFile.sh InputUncert    PPData    JERDown                       1 DefaultStep1
	#bash RunFile.sh Input    PPData    JECAggressiveUp               1 DefaultStep1
	#bash RunFile.sh Input    PPData    JECAggressiveDown             1 DefaultStep1#disabled for now no Input//input

RunPPStep2: Execute ExecutePlot
	bash RunFile.sh Input    PPMC      Nominal                       1 DefaultStep1
	bash RunFile.sh Input    PPData    JECUp                         1 DefaultStep2
	bash RunFile.sh Input    PPData    JECDown                       1 DefaultStep2
	bash RunFile.sh Input    PPData    JERUp                         1 DefaultStep2
	bash RunFile.sh Input    PPData    JERDown                       1 DefaultStep2
	bash RunFile.sh Input    PPData    JECAggressiveUp               1 DefaultStep2
	bash RunFile.sh Input    PPData    JECAggressiveDown             1 DefaultStep2

RunSetDHFileStep1TUnfold:
	bash SetPriorToUse.sh DefaultStep1 DefaultStep1 TUnfold

RunSetDHFileStep1:
	bash SetPriorToUse.sh DefaultStep1 DefaultStep1 Bayes

RunSetDHFileStep2:
	bash SetPriorToUse.sh DefaultStep1 DefaultStep2 Bayes

RunDataToy: Execute ExecutePlot
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Nominal
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Alternate
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Nominal
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Alternate

RunAAMCToyChoice: Execute ExecutePlot
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 ExternalMC
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 Power40
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 Power50
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 Power60
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 Flat
	bash RunFile.sh ToyInput PbPbMCRho Nominal_Toy 0 Original

RunPPMCToyChoice: Execute ExecutePlot
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 ExternalMC
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 Power40
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 Power50
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 Power60
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 Flat
	bash RunFile.sh ToyInput PPMC    Nominal_Toy 1 Original

RunAADataToyChoice: Execute ExecutePlot
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 ExternalMC
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Power40
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Power50
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Power60
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Flat
	bash RunFile.sh ToyInput PbPbData  Nominal_Power50Prior_Toy 0 Original

RunPPDataToyChoice: Execute ExecutePlot
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 ExternalMC
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Power40
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Power50
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Power60
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Flat
	bash RunFile.sh ToyInput PPData    Nominal_Power50Prior_Toy 1 Original

RunTest: Execute ExecutePlot
	# ./Execute --Input ToyInput/PPMC_R4_CentralityInclusive_Nominal_Toy.root --Output TestMCPrior.root \
	#    --Prior ExternalMC --ExternalPriorFile Input/PPMC_R4_CentralityInclusive_Nominal.root
	# ./ExecutePlot --Input TestMCPrior.root --Output TestMCPrior.pdf --Ignore 14
	# ./Execute --Input ToyInput/PPMC_R4_CentralityInclusive_Nominal_Toy.root --Output TestOriginalPrior.root \
	#    --Prior Original
	# ./ExecutePlot --Input TestOriginalPrior.root --Output TestOriginalPrior.pdf --Ignore 14
	./Execute --Input ToyInput/PPMC_R4_CentralityInclusive_Nominal_Toy.root --Output TestPower5Prior.root \
		--Prior Power --PriorK -5 --DoBayes true --DoSVD false --DoInvert true --DoTUnfold true --DoFit true
	# ./ExecutePlot --Input TestPower5Prior.root --Output TestPower5Prior.pdf --Ignore 14
	# ./Execute --Input ToyInput/PPMC_R4_CentralityInclusive_Nominal_Toy.root --Output TestPower6Prior.root \
	#    --Prior Power --PriorK -6
	# ./ExecutePlot --Input TestPower6Prior.root --Output TestPower6Prior.pdf --Ignore 14
	# ./Execute --Input ToyInput/PPMC_R4_CentralityInclusive_Nominal_Toy.root --Output TestPower7Prior.root \
	#    --Prior Power --PriorK -7 --DoFoldNormalize true --Ignore 14
	# ./ExecutePlot --Input TestPower7Prior.root --Output TestPower7Prior.pdf --Ignore 14
	# ./Execute --Input Input/PPData_R4_CentralityInclusive_Nominal.root --Output Meow.root \
	#    --Prior Flat
	# ./ExecutePlot --Input Meow.root --Output Meow.pdf --Ignore 10


