
default: Prepare

Prepare:
	mkdir -p Output
	rm -f Input UnfoldedInput
	#ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output Input
	#ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output UnfoldedInput
	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output_R4 UnfoldedInput
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh

Clean:
	rm -f Output/*root

# RunFile.sh Parameters
#    Prefix Suffix IsPP IterationPrefix PriorChoice IterationPriorChoice

RunMC: Execute
	bash RunFile.sh PbPbMCRho Nominal 0
	bash RunFile.sh PPMC      Nominal 1

RunDataStep1: Execute
	bash RunFile.sh PbPbData Nominal 0 PbPbMCRho DefaultStep1
	bash RunFile.sh PPData   Nominal 1 PPMC      DefaultStep1

RunDataStep2: Execute
	bash RunFile.sh PbPbData Nominal 0 PbPbData  DefaultStep2 DefaultStep1
	bash RunFile.sh PPData   Nominal 1 PPData    DefaultStep2 DefaultStep1

RunData: Execute
	bash RunFile.sh PbPbData Nominal 0 PbPbMCRho Nominal
	bash RunFile.sh PPData   Nominal 1 PPMC      Nominal

Execute: GeneratePerfectData.cpp
	g++ GeneratePerfectData.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
