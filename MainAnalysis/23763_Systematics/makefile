default: Prepare TestIteration

Prepare:
	rm -f Input BinMapping OriginalInput
	#ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output Input
	#ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output BinMapping
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput//Output_R4 BinMapping
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/Systematics.dh
	mkdir -p Output_R4 Plot_R4 Config

Clean:
	rm -f Output/*root
	rm -f Plot/*pdf
	rm -f Config/*config

RunData: Execute ExecutePlot ExecuteIteration
	bash RunFile.sh PbPbData 0
	bash RunFile.sh PPData 1

Execute: Harvest.cpp
	g++ Harvest.cpp -o  Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

ExecutePlot: MakeSystematicSummaryPlot.cpp
	g++ MakeSystematicSummaryPlot.cpp -o  ExecutePlot `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 


ExecuteIteration: FindIteration.cpp
	g++ FindIteration.cpp  -o ExecuteIteration `root-config --cflags --glibs` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
#	-I${RootMacrosBase}

TestIteration: ExecuteIteration
	./ExecuteIteration --Input Input/PPData_R4_CentralityInclusive_Nominal_Power50Prior.root \
		--Mode Next --Iteration 11
	./ExecuteIteration --Input Input/PPData_R4_CentralityInclusive_Nominal_Power50Prior.root \
		--Mode Previous --Iteration 11


