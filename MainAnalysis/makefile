
default: Prepare RunPart2C

Prepare:
	(cd GlobalSetting && make)

Sleep:
	SleepUntilDone.sh

RunPart1: RunPart1A RunPart1B RunPart1C

RunPart1A:
	(cd 23752_CopyJetFromForest && make Prepare SubmitBeforeRho)
	SleepUntilDone.sh

RunPart1B:
	(cd 23752_CopyJetFromForest && make Prepare DeleteAndMerge)
	(cd $(ProjectBase)/DataPerformance/24131_RelativeTriggerTurnOn && make Prepare ProcessAll)
	(cd $(ProjectBase)/DataPerformance/24131_RelativeTriggerTurnOn && make Harvest)
	# (cd 23775_CentralityWeights && make Prepare TestRun)
	(cd 24155_UEMatchingWeight  && make Prepare RunAll FitAll)
	(cd 23752_CopyJetFromForest && make Prepare SubmitAfterRho)
	(cd 24263_StatisticalUncertaintyCancellation && make Prepare RunAll) #added by Uttam later for StatisticalRho.dh
	SleepUntilDone.sh

RunPart1C:
	#(cd 23752_CopyJetFromForest && make Prepare DeleteAndMerge)
	(cd 23762_UnfoldPreparation && make Prepare RunPP RunAA)
	(cd 24112_BrilCalc          && make Prepare RunPP RunAA)
	(cd 24121_MBCounting        && make Prepare MergeMB RunAA)

CleanPart2:
	#############################################
	# Clean all local output files?             #
	# Waiting 10 seconds in case it's a mistake #
	#############################################
	sleep 10
	(cd 23762_PerfectRecoGeneration    && make Clean)
	(cd 23762_FakeData                 && make Clean)
	(cd 23762_ActualUnfold             && make Clean)
	(cd 23772_UnfoldIteration          && make Clean)
	(cd 23763_Systematics              && make Clean)
	(cd 24165_SystematicsCombination   && make Clean)
	(cd 24113_HEPData                  && make Clean)
	(cd 23763_BeautifulPlots           && make Clean)
	(cd 23763_BeautifulMatrix          && make Clean)
	(cd 24114_PPRatioHEPData           && make Clean)
	(cd 24114_BeautifulPPRatioPlots    && make Clean)
	(cd 24171_BeautifulSystematicsPlot && make Clean)
	(cd 24122_RAA                      && make Clean)

RunPart2: RunPart2A RunPart2B RunPart2C RunPart2D RunPart2E

RunPart2A:
	######################################################################
	# Make sure to copy over the following from lxplus before continuing #
	#    1. unfold preparation output files                              #
	#    2. GlobalSetting.dh                                             #
	#    3. StatisticsRho.dh                                             #
	# Also make sure that the ProjectBase is set correctly!              #
	######################################################################
	sleep 10
	(cd 24236_RebinUnfoldInput         && make Prepare RunAll)
	(cd 24232_LowerBoundDetermination  && make Prepare RunAll)
	(cd 24233_UpperBoundDetermination  && make Prepare RunAll)
	(cd 24165_PUBugCorrection          && make Prepare PlaceHolder) #not needed for now
	(cd 24231_EventSelectionEfficiency && make Prepare SetEfficiency)
	(cd 24233_OfficialMBCount          && make Prepare RunAll)
	(cd 23772_UnfoldIteration          && make Prepare SetDefault)
	(cd 23762_PerfectRecoGeneration    && make Prepare RunMC)
	(cd 23762_FakeData                 && make Prepare RunMC)
	(cd 23762_ActualUnfold             && make Prepare RunMCToy)
	(cd 23772_UnfoldIteration          && make Prepare RunMC)
	(cd 23762_ActualUnfold             && make Prepare RunDataStep1)
	(cd 23762_PerfectRecoGeneration    && make Prepare RunDataStep1)
	(cd 23762_FakeData                 && make Prepare RunDataStep1)
	(cd 23762_ActualUnfold             && make Prepare RunDataStep1Toy)
	(cd 23772_UnfoldIteration          && make Prepare RunDataStep1)
	(cd 23762_ActualUnfold             && make Prepare RunDataStep2)
	(cd 23762_PerfectRecoGeneration    && make Prepare RunDataStep2)
	(cd 23762_FakeData                 && make Prepare RunDataStep2)
	(cd 23762_ActualUnfold             && make Prepare RunDataStep2Toy)
	(cd 23772_UnfoldIteration          && make Prepare RunDataStep2)
	# (cd 23762_ActualUnfold             && make Prepare RunPPStep1 RunAAStep1)
	# (cd 23762_ActualUnfold             && make Prepare RunSetDHFileStep1TUnfold)
	(cd 23762_ActualUnfold             && make Prepare RunPPStep1 RunAAStep1)
	(cd 23762_ActualUnfold             && make Prepare RunSetDHFileStep1)
	# (cd 23762_ActualUnfold             && make Prepare RunPPStep2 RunAAStep2)
	# (cd 23762_ActualUnfold             && make Prepare RunSetDHFileStep2)


RunPart2B:
	(cd 24113_HEPData                  && make default)
	(cd 24114_PPRatioHEPData           && make default)

RunPart2C:
	(cd 23763_Systematics              && make Prepare RunData)
	(cd 24165_SystematicsCombination   && make Prepare SetDHFile RunAll)
	(cd 23763_BeautifulPlots           && make Prepare RunJetPT RunJetPTClosure)
	(cd 24122_RAA                      && make Prepare RunRAA RunRCP RunRRAA)
	(cd 23763_BeautifulMatrix          && make Prepare RunPP RunAA)
	(cd 24114_BeautifulPPRatioPlots    && make Prepare RunJetPT)
	(cd 24171_BeautifulSystematicsPlot && make Prepare RunAll)

RunPart2D:
	(cd 23763_BeautifulPlots           && make Prepare RunCheck RunJetPTReco)
	(cd 24122_RAA                      && make Prepare RunRAACheck)
	(cd 24231_ConditionNumber          && make Prepare RunNominal RunNominalPlot)
	(cd 23772_UnfoldIteration          && make Prepare RunPlot)

RunPart2E:
	(cd 23763_BeautifulPlots           && make Deploy DeployRoot DeployPaper)
	(cd 23763_BeautifulMatrix          && make Deploy DeployPaper)
	(cd 24114_BeautifulPPRatioPlots    && make Deploy DeployPaper)
	(cd 24171_BeautifulSystematicsPlot && make Deploy DeployPaper)
	(cd 24122_RAA                      && make Deploy DeployRoot DeployPaper)
	(cd 24231_ConditionNumber          && make Deploy)
	(cd 23772_UnfoldIteration          && make Deploy)

RunPart2F:   # Things that are good to have but not essential in any way
	(cd 24231_ConditionNumber          && make Prepare RunOther)
###################################################################################
###################################################################################


RunPart2AOld:
	#########################################################################
	# Make sure to copy over the following from lxplus before continuing... #
	#    1. unfold preparation output files                                 #
	#    2. GlobalSetting.dh                                                #
	# Also make sure that the ProjectBase is set correctly!                 #
	#########################################################################
	sleep 10
	(cd 24232_LowerBoundDetermination  && make Prepare RunAll)
	(cd 24233_UpperBoundDetermination  && make Prepare RunAll)
	(cd 24165_PUBugCorrection          && make Prepare PlaceHolder)
	(cd 24231_EventSelectionEfficiency && make Prepare SetEfficiency)
	(cd 24233_OfficialMBCount          && make Prepare RunAll)
	(cd 23772_UnfoldIteration          && make Prepare SetDefault)
	(cd 23762_PerfectRecoGeneration    && make Prepare RunMC)
	(cd 23762_FakeData                 && make Prepare RunMC)
	(cd 23762_ActualUnfold             && make Prepare RunAA RunPP)
	(cd 23772_UnfoldIteration          && make Prepare RunMC)
	(cd 23762_PerfectRecoGeneration    && make Prepare RunData)
	(cd 23762_FakeData                 && make Prepare RunData)
	(cd 23762_ActualUnfold             && make Prepare RunDataToy)
	(cd 23772_UnfoldIteration          && make Prepare RunData)
###################################################################################
###################################################################################
