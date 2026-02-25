
#MCFraction = 1.0
#DataFraction = 1.0

default: RunAA

Prepare:
	#ln -sf ../23752_CopyJetFromForest/Output Input
	ln -sf ../23752_CopyJetFromForest/ Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh .
	#ln -sf ../GlobalSetting/GlobalSetting.dh

Execute: PrepareUnfold.cpp
	 g++ PrepareUnfold.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 

ExecutePreset: RunPreset.cpp
	g++ RunPreset.cpp -o ExecutePreset `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 


Debug:
	GenBins=`DHQuery GlobalSetting.dh Binning GenPT`; \
	RecoBins=`DHQuery GlobalSetting.dh Binning RecoPT`; \
	echo $$GenBins; \
	DHQuery GlobalSetting.dh Binning GenPT

RunAA: Execute ExecutePreset
	bash RunFileAA_MC.sh
	bash RunFileAA_Data_Sys.sh

RunAAMCCentrality: Execute ExecutePreset
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbMC \
		--Prefix PbPbMC --Suffix Nominal --GlobalSetting GlobalSetting.dh \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(MCFraction) \
		| grep ^time`
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix Nominal --GlobalSetting GlobalSetting.dh \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix JECUp --GlobalSetting GlobalSetting.dh \
		--Shift 1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix JECDown --GlobalSetting GlobalSetting.dh \
		--Shift -1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`

RunAAExternal: Execute ExecutePreset
	GenBins=`DHQuery GlobalSetting.dh Binning GenPT`; \
	RecoBins=`DHQuery GlobalSetting.dh Binning RecoPT`; \
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbMC \
		--Prefix PbPbMC --Suffix Nominal --GenBins $$GenBins --RecoBins $$RecoBins \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(MCFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix Nominal --GenBins $$GenBins --RecoBins $$RecoBins \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix JECUp --GenBins $$GenBins --RecoBins $$RecoBins \
		--Shift 1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PbPbMC --DataTag Input/PbPbData \
		--Prefix PbPbData --Suffix JECDown --GenBins $$GenBins --RecoBins $$RecoBins \
		--Shift -1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`;

RunPP: Execute ExecutePreset
	bash RunFilePP_MC.sh
	bash RunFilePP_Data_Sys.sh




DebugRun: Execute ExecutePreset
	# eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPMC \
	# 	--Prefix PPMC --Suffix Nominal --Centrality "Inclusive" --GlobalSetting GlobalSetting.dh \
	# 	--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(MCFraction) \
	# 	--JetR 1,2 \
	# 	| grep ^time`
	eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPData \
		--Prefix PPData --Suffix Nominal --Centrality "Inclusive" --GlobalSetting GlobalSetting.dh \
		--Shift 0.0 --Smear -1 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		--JetR 1,2 \
		--UseJERSFFile true \
		--JERSF $(ProjectBase)/CommonCode/jer/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_SF_AK4PF.txt \
		| grep ^time`

RunPPExternal: Execute ExecutePreset
	GenBins=`DHQuery GlobalSetting.dh Binning GenPT`; \
	RecoBins=`DHQuery GlobalSetting.dh Binning RecoPT`; \
	eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPMC \
		--Prefix PPMC --Suffix Nominal --Centrality "Inclusive" --GenBins $$GenBins --RecoBins $$RecoBins \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(MCFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPData \
		--Prefix PPData --Suffix Nominal --Centrality "Inclusive" --GenBins $$GenBins --RecoBins $$RecoBins  \
		--Shift 0.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPData \
		--Prefix PPData --Suffix JECUp --Centrality "Inclusive" --GenBins $$GenBins --RecoBins $$RecoBins  \
		--Shift 1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`; \
	eval `./ExecutePreset --MCTag Input/PPMC --DataTag Input/PPData \
		--Prefix PPData --Suffix JECDown --Centrality "Inclusive" --GenBins $$GenBins --RecoBins $$RecoBins  \
		--Shift -1.0 --Smear 0.0 --Flooring false --MCFraction $(MCFraction) --DataFraction $(DataFraction) \
		| grep ^time`;



