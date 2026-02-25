
default: TestRun

Prepare:
	rm -f Input
	ln -fs $(ProjectBase)/MainAnalysis/24236_RebinUnfoldInput/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .

Execute: EvaluateConditionNumber.cpp
	g++ EvaluateConditionNumber.cpp -o Execute `root-config --libs --cflags` \
	-I../../CommonCode/include/ \
	../../CommonCode/library/*.o

ExecutePlot: MakeSummaryPlot.cpp
	g++ MakeSummaryPlot.cpp -o ExecutePlot `root-config --libs --cflags` \
	-I../../CommonCode/include/ \
	../../CommonCode/library/*.o

TestRun: Execute
	./Execute --Input Input/PbPbData_R4_Centrality50to90_Nominal.root --Response HResponse --DHFile Condition.dh

RunAll: RunNominal RunNominalPlot RunOther

RunNominal: Execute
	bash RunNominal.sh

RunNominalPlot: ExecutePlot
	./ExecutePlot --DHFile Condition.dh --Output ConditionMC.pdf --PPBase PPMC --AABase PbPbMCRho
	./ExecutePlot --DHFile Condition.dh --Output ConditionData.pdf --PPBase PPData --AABase PbPbData

RunOther: Execute
	bash RunOther.sh

Deploy:
	mkdir -p $(DeployBase)/Check/ConditionNumber/
	cp Condition*pdf $(DeployBase)/Check/ConditionNumber/


