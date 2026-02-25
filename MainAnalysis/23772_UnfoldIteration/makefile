
default: TestRun

Prepare:
	rm -f Input
	#ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	mkdir -p Output

Clean:
	rm -f Output/*pdf

SetDefault:
	bash SetInitialPriorChoice.sh

# RunFile.sh parameters
#    Prefix Suffix StateSuffix PriorChoice IsPP

RunMC: Execute
	bash RunFile.sh PbPbMCRho Nominal_Toy Nominal DefaultStep1 0
	bash RunFile.sh PPMC      Nominal_Toy Nominal DefaultStep1 1

RunDataStep1: Execute
	bash RunFile.sh PbPbData  Nominal_DefaultStep1Prior_Toy Nominal DefaultStep1 0
	bash RunFile.sh PPData    Nominal_DefaultStep1Prior_Toy Nominal DefaultStep1 1
	bash SetDataPriorExtra.sh

RunDataStep2: Execute
	bash RunFile.sh PbPbData  Nominal_DefaultStep2Prior_Toy Nominal DefaultStep2 0
	bash RunFile.sh PPData    Nominal_DefaultStep2Prior_Toy Nominal DefaultStep2 1

RunData: Execute
	bash RunFile.sh PbPbData Nominal_Power50Prior_Toy Nominal Nominal 0
	bash RunFile.sh PbPbData Nominal_Power50Prior_Toy Nominal Alternate 0
	bash RunFile.sh PPData Nominal_Power50Prior_Toy Nominal Nominal 1
	bash RunFile.sh PPData Nominal_Power50Prior_Toy Nominal Alternate 1

TestRun: Execute
	./Execute --Input Input/PPMC_R4_CentralityInclusive_Nominal_Toy_DefaultStep1Prior.root \
		--Output PPMC_R4_CentralityInclusive_Nominal_Toy_DefaultStep1Prior.pdf \
		--Ignore 12 --IgnoreReco 10 --DHFile Meow.dh \
		--State Iterations --Key PPMC_R4_CentralityInclusive_Nominal_DefaultStep1Prior
	./Execute --Input Input/PPData_R4_CentralityInclusive_Nominal_DefaultStep1Prior_Toy_DefaultStep1Prior.root \
		--Output PPData_R4_CentralityInclusive_Nominal_DefaultStep1Prior_Toy_DefaultStep1Prior.pdf \
		--Ignore 12 --IgnoreReco 10 --DHFile Meow.dh \
		--State Iterations --Key PPData_R4_CentralityInclusive_Nominal_DefaultStep1Prior

Execute: PlotIteration.cpp
	g++ PlotIteration.cpp -o Execute `root-config --libs --cflags` \
	-I../../CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

ExecuteLCurve: PlotLCurve.cpp
	g++ PlotLCurve.cpp -o ExecuteLCurve `root-config --libs --cflags` \
	-I../../CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

ExecutePlot: MakeSummaryPlot.cpp
	g++ MakeSummaryPlot.cpp -o ExecutePlot `root-config --libs --cflags` \
	-I../../CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

RunPlot: ExecutePlot
	./ExecutePlot --DHFile GlobalSetting.dh --Output IterationMC.pdf --PPBase PPMC --AABase PbPbMCRho
	./ExecutePlot --DHFile GlobalSetting.dh --Output IterationData.pdf --PPBase PPData --AABase PbPbData

Deploy:
	mkdir -p $(DeployBase)/Check/Iterations/
	cp Iteration*pdf $(DeployBase)/Check/Iterations/



