
default: Prepare

Prepare:
	rm -f PerfectInput Input
	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_PerfectRecoGeneration/Output_R4 PerfectInput
	#ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output Input
	#ln -sf $(ProjectBase)/MainAnalysis/23762_PerfectRecoGeneration/Output PerfectInput

	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	mkdir -p Output

Clean:
	rm -f Output/*root

# RunFile.sh arguments:
#    Prefix Suffix YieldPrefix YieldSuffix PriorChoice Fraction IsPP

RunMC: Execute
	bash RunFile.sh PbPbMCRho Nominal PbPbData Nominal None 1.00 0
	bash RunFile.sh PPMC      Nominal PPData   Nominal None 1.00 1

RunDataStep1: Execute
	bash RunFile.sh PbPbData Nominal PbPbData Nominal DefaultStep1   1.00 0
	bash RunFile.sh PPData   Nominal PPData   Nominal DefaultStep1   1.00 1

RunDataStep2: Execute
	bash RunFile.sh PbPbData Nominal PbPbData Nominal DefaultStep2   1.00 0
	bash RunFile.sh PPData   Nominal PPData   Nominal DefaultStep2   1.00 1

RunData: Execute
	bash RunFile.sh PbPbData Nominal PbPbData Nominal Nominal   1.00 0
	bash RunFile.sh PPData   Nominal PPData   Nominal Nominal   1.00 1

Execute: GenerateFakeData.cpp
	#CXXFLAGS="-mmacosx-version-min=11.2" g++ GenerateFakeData.cpp -o Execute `root-config --libs --cflags` 
	g++ GenerateFakeData.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
