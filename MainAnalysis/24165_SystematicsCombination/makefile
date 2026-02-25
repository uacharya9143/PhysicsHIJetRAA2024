
DH = Systematics.dh

default: Prepare TestRun

Prepare:
	rm -f Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/$(DH) .
	#ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/23763_Systematics/Output_R4 Input

	mkdir -p Output_R4/

Clean:
	rm -f Output/*root

TestRun: Execute
	./Execute --File1 Input/PPData_R1_CentralityInclusive.root \
		--File2 Input/PPData_R9_CentralityInclusive.root \
		--Output PPDataRatio_R1R9_CentralityInclusive.root \
		--DHFile $(DH) --State PPDataRatio_R1R9_CentralityInclusive \
		--Include HJECUp,HJECDown,HJERUp,HJERDown,HPrior,HIteration \
		--Global GlobalSystematics.dh

SetDHFile:
	bash SetDefault.sh

RunAll: Execute
	bash RunAll.sh

Execute: SystematicsCombiner.cpp
	g++ SystematicsCombiner.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
