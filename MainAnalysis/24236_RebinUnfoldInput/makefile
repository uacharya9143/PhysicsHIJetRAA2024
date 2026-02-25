
default: TestRun

Prepare:
	rm -f Input
	#ln -sf $(ProjectBase)/MainAnalysis/23762_UnfoldPreparation/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_UnfoldPreparation/Output_R4 Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	mkdir -p Output 

RunAll: Execute
	bash RunAll.sh

TestRun: Execute
	./Execute --Input Input/PbPbData_R4_Centrality0to10_NominalFine.root \
		--Output PbPbData_R4_Centrality0to10_Nominal.root \
		--DHFile GlobalSetting.dh

Execute: Rebin.cpp
	g++ Rebin.cpp -o Execute -I../../CommonCode/include `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
