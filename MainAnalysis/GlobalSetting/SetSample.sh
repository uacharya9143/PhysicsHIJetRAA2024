#!/bin/bash

EOSHI=/eos/cms/store/group/phys_heavyions/uacharya
VANDYHI=root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/uacharya
#EOSHIMC=/eos/cms/store/group/phys_heavyions/uacharya
#EOSHI2024pp=${EOSHI2017}/DYJetsToLL_MLL-50_TuneCP5_5020GeV-amcatnloFXFX-pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/
#EOSHI2024pbpb=${EOSHI2018}/DYJetsToLL_MLL-50_TuneCP5_HydjetDrumMB_5p02TeV-amcatnloFXFX-pythia8/HINPbPbSpring21MiniAOD-mva98_112X_upgrade2018_realistic_HI_v9-v1/

EOSHI2024=${EOSHI}/JetRAA_PbPb_2024/New_Data_PbPb/
EOSHI2024ppRef=${EOSHI}/JetRAA_ppRef_2024/
EOSHI2024ppMC=${EOSHI}/JetRAA_PbPb_2024/ppRef_2024PrivateMC_5p36TeV/
EOSHI2024PbPbMCNoE=${EOSHI}/JetRAA_PbPb_2024/PbPb_2024MC_5p36TeV/
EOSHI2024PbPbMCE=${EOSHI}/JetRAA_PbPb_2024/PbPb_2024MC_5p36TeV_Embed/

#for R in 015 2 3 4  6  8 10 12
##for R in  4
#do
   # PbPb Data, Jet100
   # DHSet GlobalSetting.dh SampleOverview PbPbData$R string "Jet 100, MINIAOD, 2021 Dec 04"
   # DHSet GlobalSetting.dh Sample PbPbData${R} string ${EOSHI2018}/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataHPForJetRAAHighThresholdJet100OnlyRetry2/211204_093116/
#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////
   
   # PbPb Data, Jet 80+100
   #DHSet GlobalSetting.dh SampleOverview PbPbData3 string "Jet120+100+80+60+40ETA5p1_jet03_R03, MINIAOD, 2024 Feb 18"
DHSet GlobalSetting.dh Sample PbPbData1 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_015_02_v1/250517_195605/

DHSet GlobalSetting.dh Sample PbPbData2 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_015_02_v1/250517_195605/

DHSet GlobalSetting.dh Sample PbPbData3 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_03_04_v1/250517_195649/

DHSet GlobalSetting.dh Sample PbPbData4 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_03_04_v1/250517_195649/

DHSet GlobalSetting.dh Sample PbPbData6 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_06_08_v1/250517_195718/

DHSet GlobalSetting.dh Sample PbPbData7 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_06_08_v1/250517_195718/

DHSet GlobalSetting.dh Sample PbPbData8 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_10_v1/250517_195741/

DHSet GlobalSetting.dh Sample PbPbData9 string ${EOSHI2024}/HIPhysicsRawPrime0/HIRun2024A-PromptReco-v1/MINIAOD/HIPhysicsRawPrime0/PbPb_Prime0_HIRun2024A_12_v1/250517_195759/

   

#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////


##########################//PPREF_2024_Data//#############

#DHSet GlobalSetting.dh SampleOverview PPData3 string "Jet120+100+80+60+40ETA5p1_jet015_R2, MINIAOD, 2025_Mar03"
DHSet GlobalSetting.dh Sample PPData1 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK015_AK02_probe0/250525_132856/

DHSet GlobalSetting.dh Sample PPData2 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK015_AK02_probe0/250525_132856/

DHSet GlobalSetting.dh Sample PPData3 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK03_AK04_probe0/250525_132912/

DHSet GlobalSetting.dh Sample PPData4 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK03_AK04_probe0/250525_132912/

DHSet GlobalSetting.dh Sample PPData6 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK06_AK08_probe0/250525_132927/

DHSet GlobalSetting.dh Sample PPData7 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK06_AK08_probe0/250525_132927/

DHSet GlobalSetting.dh Sample PPData8 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK10_AK12_probe0/250525_132940/

DHSet GlobalSetting.dh Sample PPData9 string ${EOSHI2024ppRef}/PPRefHardProbes0/Run2024J-PromptReco-v1/MINIAOD/PPRefHardProbes0/ppRef2024_jetRAA_AK10_AK12_probe0/250525_132940/


##########################//PbPb_2024_MC_noEmbed//#############

#DHSet GlobalSetting.dh Sample PbPbMC1 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC2 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC3 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC4 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC6 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC7 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC8 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/
#DHSet GlobalSetting.dh Sample PbPbMC9 string ${EOSHI2024PbPbMCNoE}/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_NoEmbed-68361c49e7cea33ff1f5e906f438597f/USER/QCD_pThat_15_TuneCP5_PbPb_5p36TeV_pythia8/MC_PbPb_2024MC_5p36TeV/250513_012146/

##########################//PbPb_2024_MC_Embed//#############


DHSet GlobalSetting.dh Sample PbPbMC1 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC2 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC3 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC4 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC6 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC7 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC8 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/
DHSet GlobalSetting.dh Sample PbPbMC9 string ${EOSHI2024PbPbMCE}/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/uacharya-PbPb_5p36TeV_2024_PrivateMC_Sample_MINIAOD_Embed-1f809eaabe951f48134ae3274f9f4c7d/USER/QCD_pThat_15_TuneCP5_PbPb_13p6TeV_pythia8/MC_PbPb_2024MC_5p36TeV_Embed/250515_164046/



##########################//PP_2024_MC//#############
DHSet GlobalSetting.dh Sample PPMC1 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC2 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC3 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC4 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC6 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC7 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC8 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/
DHSet GlobalSetting.dh Sample PPMC9 string ${EOSHI2024ppMC}/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/uacharya-ppRef_5p36TeV_2024_simu_pythia_MiniAOD_NoEmbed-e614c75cb0dc376772eecbc30cf16b2f/USER/QCD_pThat_15_TuneCP5_pp_5p36TeV_pythia8/MC_ppRef_2024PrivateMC_5p36TeV/250513_015951/


#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////#//////////////////////////////////////////////////////


##########################//pp_MC_Sample_from Luna//#############
#DHSet GlobalSetting.dh SampleOverview PPMC4 string "Jet120+100+80+60+40ETA5p1_jet06_R08, MINIAOD, 2024 Feb 18"
#DHSet GlobalSetting.dh Sample PPMC4 string ${EOSHI2024pp}/AODSIM/DYJetsToLL_MLL-50_TuneCP5_5020GeV-amcatnloFXFX-pythia8/20231020_ZHadronMLLWithMuTreePP/231020_152224/0000/

#DHSet GlobalSetting.dh SampleOverview PbPbMC4 string "Jet120+100+80+60+40ETA5p1_jet06_R08, MINIAOD, 2024 Feb 18"
#DHSet GlobalSetting.dh Sample PbPbMC4 string ${EOSHI2024pbpb}/MINIAODSIM/DYJetsToLL_MLL-50_TuneCP5_HydjetDrumMB_5p02TeV-amcatnloFXFX-pythia8/./20240605_ZHadronMCDY/240605_151412/0000/

 ##########################pp_MC_Sample_from Luna#############
  
   
   # PbPb MB Data
   #DHSet GlobalSetting.dh SampleOverview PbPbMBData$R string "MinimumBias2, MINIAOD, 2021 Dec 12"
   #DHSet GlobalSetting.dh Sample PbPbMBData${R} string ${EOSHI2018}/HIMinimumBias2/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIMinimumBias2/DefaultPbPbDataForJetRAAGatedMB${R}/

   # PbPb MC
   #DHSet GlobalSetting.dh SampleOverview PbPbMC$R string "Dijet with rho bug fix, MINIAOD, 2021 Nov 04"
   #DHSet GlobalSetting.dh Sample PbPbMC${R} string ${EOSHI2018}/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/DefaultPbPbMCForJetRAAWithRhoGenBugFix/211104_112116//

   # PbPb MC JEC
   #DHSet GlobalSetting.dh SampleOverview PbPbMCJEC$R string "Dijet with rho bug fix, no threshold on jets, MINIAOD, 2021 Dec 21"
   #DHSet GlobalSetting.dh Sample PbPbMCJEC${R} string ${EOSHI2018}/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/20211221DefaultPbPbMCForJetRAANoThreshold/211221_182709/

   # PP NonUL Data
   # DHSet GlobalSetting.dh SampleOverview PPDataEOY$R string "HighEGJet (EOY), PF candidates, AOD, 2021 Nov 15"
   # DHSet GlobalSetting.dh Sample PPDataEOY${R} string ${EOSHI2017}/HighEGJet/Run2017G-17Nov2017-v2/AOD/HighEGJet/20211115FullRunHighEGJetNoTrackTree/211115_214540/
   #DHSet GlobalSetting.dh SampleOverview PPDataEOY$R string "HighEGJet (EOY), jets, AOD, 2021 Dec 21"
   #DHSet GlobalSetting.dh Sample PPDataEOY${R} string ${EOSHI2017}/HighEGJet/Run2017G-17Nov2017-v2/AOD/HighEGJet/20211221ULFullRunClusterHighEGJet

   # PP Data UL
   # DHSet GlobalSetting.dh SampleOverview PPData$R string "HighEGJet (UL), Jets 40 GeV, MINIAOD, 2021 Dec 1"
   # DHSet GlobalSetting.dh Sample PPData${R} string ${EOSHI2017}/HighEGJet/Run2017G-UL2017_MiniAODv2-v1/MINIAOD/HighEGJet/20211201FullRunULHighEGJetHighTreshold
   
   # PP Data UL AOD
   #DHSet GlobalSetting.dh SampleOverview PPData$R string "HighEGJet (UL), Jets 40 GeV, AOD, 2021 Dec 22"
   #DHSet GlobalSetting.dh Sample PPData${R} string ${EOSHI2017}/HighEGJet/Run2017G-09Aug2019_UL2017-v1/AOD/HighEGJet/20211222ULFullRunClusterHighEGJet/211222_095621/

   # PP NonUL MC
  # DHSet GlobalSetting.dh SampleOverview PPMCEOY$R string "QCD (EOY), PF candidates, AOD, 2021 Aug 24"
  # DHSet GlobalSetting.dh Sample PPMCEOY${R} string ${EOSHI2017}/QCD_pThat-15_Dijet_TuneCP5_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV_v1-v1/AODSIM/ManualRun22651//

   # PP MC UL
  # DHSet GlobalSetting.dh SampleOverview PPMC$R string "Private QCD (UL), PF candidates + jets, MINIAOD, 2021 Nov 22"
  # DHSet GlobalSetting.dh Sample PPMC${R} string ${EOSHIOps2017}/RunIISummer20UL17pp5TeV/anstahll-QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8_MINIAOD_20211122-4230e9eff0736cf0d162fef5ca3cfbe4/USER/RunIISummer20UL17pp5TeV/2021122PrivateDijetJECForest
#done


# Old locations

# PbPb Data Split Nov 2021 version
# DHSet GlobalSetting.dh Sample PbPbData1 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit12WithRhoGenBugFix/211106_162257/
# DHSet GlobalSetting.dh Sample PbPbData2 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit12WithRhoGenBugFix/211106_162257/
# DHSet GlobalSetting.dh Sample PbPbData3 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit35WithRhoGenBugFix/211106_221601/
# DHSet GlobalSetting.dh Sample PbPbData4 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit35WithRhoGenBugFix/211106_221601/
# DHSet GlobalSetting.dh Sample PbPbData5 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit35WithRhoGenBugFix/211106_221601/
# DHSet GlobalSetting.dh Sample PbPbData6 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit67WithRhoGenBugFixRetry/211120_101659/
# DHSet GlobalSetting.dh Sample PbPbData7 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit67WithRhoGenBugFixRetry/211120_101659/
# DHSet GlobalSetting.dh Sample PbPbData8 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit8WithRhoGenBugFix/211106_162321/
# DHSet GlobalSetting.dh Sample PbPbData9 string $(EOSHI2018)/HIHardProbes/HIRun2018A-PbPb18_MiniAODv1-v1/MINIAOD/HIHardProbes/DefaultPbPbDataForJetRAASplit9WithRhoGenBugFix/211106_162329/
