default: Prepare RunPP

Prepare:
	rm -f Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_ActualUnfold/Output Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh
	mkdir -p Plots

Clean:
	rm -f Plots/*

RunPP: Execute
	bash RunFile.sh PPData "" 1

RunAA: Execute
	bash RunFile.sh PbPbData "" 0

TestRunOld: Execute
	./Execute --Input Input/TestRunData.root \
		--Output Plots/TestRunMC.pdf --FinalOutput Plots/MatrixTestRunMC.pdf \
		--WorldXMin 1 --WorldXMax 1000 --WorldYMin 1 --WorldYMax 1000 --WorldZMin 0.0001 --WorldZMax 0.1 --LogZ true \
		--UseActualZMax true \
		--GenPrimaryMin 1 --GenPrimaryMax 1000 --RecoPrimaryMin 1 --RecoPrimaryMax 1000 \
		--Primary "Jet p_{T} (GeV)" --Binning "None" --RowNormalize true \
		--XAxis 303 --YAxis 303 \
		--GroupX 1 --GroupY 1 --Row 1 --Column 1
	./Execute --Input Input/TestRunData0to10.root \
		--Output Plots/TestRunMC0to10.pdf --FinalOutput Plots/MatrixTestRunMC0to10.pdf \
		--WorldXMin 0 --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1000 --WorldZMin 0.0001 --WorldZMax 0.1 --LogZ true \
		--UseActualZMax true \
		--GenPrimaryMin 0 --GenPrimaryMax 1000 --RecoPrimaryMin 0 --RecoPrimaryMax 1000 \
		--Primary "Jet p_{T} (GeV)" --Binning "None" --RowNormalize true \
		--XAxis 303 --YAxis 303 \
		--GroupX 1 --GroupY 1 --Row 1 --Column 1
	./Execute --Input Input/TestRunData50to90.root \
		--Output Plots/TestRunMC50to90.pdf --FinalOutput Plots/MatrixTestRunMC50to90.pdf \
		--WorldXMin 0 --WorldXMax 1000 --WorldYMin 0 --WorldYMax 1000 --WorldZMin 0.0001 --WorldZMax 0.1 --LogZ true \
		--UseActualZMax true \
		--GenPrimaryMin 0 --GenPrimaryMax 1000 --RecoPrimaryMin 0 --RecoPrimaryMax 1000 \
		--Primary "Jet p_{T} (GeV)" --Binning "None" --RowNormalize true \
		--XAxis 303 --YAxis 303 \
		--GroupX 1 --GroupY 1 --Row 1 --Column 1

Execute: MakePaperPlot.cpp
	#g++ MakePaperPlot.cpp -o Execute `root-config --libs --cflags` \

	#g++ MakePaperPlot.cpp -o Execute `root-config --libs` -lGpad -lGraf \
	#-I$(ProjectBase)/CommonCode/include \
	#$(ProjectBase)/CommonCode/library/*.o \
	#`root-config --cflags` 

	g++ `root-config --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	MakePaperPlot.cpp \
	$(ProjectBase)/CommonCode/library/*.o \
	-o Execute \
	`root-config --libs` -lGpad -lGraf

Deploy:
	mkdir -p $(DeployBase)/Matrix/
	cp Plots/PbPbData*pdf $(DeployBase)/Matrix/
	cp Plots/PPData*pdf $(DeployBase)/Matrix/

DeployPaper:
	mkdir -p "$(DeployBasePaper)/Matrix/"
	cp Plots/PbPbData*pdf "$(DeployBasePaper)/Matrix/"
	cp Plots/PPData*pdf   "$(DeployBasePaper)/Matrix/"


