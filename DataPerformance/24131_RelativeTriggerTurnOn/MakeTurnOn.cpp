#include <iostream>
using namespace std;

#include "fastjet/ClusterSequence.hh"
using namespace fastjet;

#include "TFile.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "TauHelperFunctions3.h"
#include "ProgressBar.h"
#include "SetStyle.h"
#include "DataHelper.h"
#include "CustomAssert.h"

#include "Messenger.h"
#include "JetCorrector.h"

struct JetPF;
int main(int argc, char *argv[]);
double GetUE(RhoTreeMessenger &M, double Eta, double R);
bool PassJetID(JetPF &ID, JetPF &Min, JetPF &Max);
double GetCentrality(double hiHF, int Variation);

struct JetPF
{
  float CHF;
  float CEF;
  float NHF;
  float NEF;
  float MUF;
  float M;
  float CM;
};

int main(int argc, char *argv[])
{
  SetThesisStyle();

  CommandLine CL(argc, argv);

  vector<string> InputFiles = CL.GetStringVector("Input");
  string OutputBase         = CL.Get("OutputBase", "Result");

  bool WriteTree            = CL.GetBool("WriteTree", false);

  string BaseTrigger        = CL.Get("Base");
  string Trigger            = CL.Get("Trigger");
  string JetName            = CL.Get("Jet");
  vector<string> JECFile    = CL.GetStringVector("JEC");
  double CentralityMin      = CL.GetDouble("CentralityMin", 0.00);
  double CentralityMax      = CL.GetDouble("CentralityMax", 0.90);
  bool ReEvaluateCentrality = CL.GetBool("ReEvaluateCentrality", false);
  string DHFileName         = CL.Get("DHFile", "GlobalSetting.dh");
  bool DoJetID              = CL.GetBool("JetID", false);
  string DHJetIDKeyBase     = (DoJetID ? CL.Get("JetIDKeyBase") : "none");
  double EtaMin             = CL.GetDouble("EtaMin", -2.0);
  double EtaMax             = CL.GetDouble("EtaMax", +2.0);
  bool DoBaselineCutPP      = CL.GetBool("DoBaselineCutPP", false);
  bool DoBaselineCutAA      = CL.GetBool("DoBaselineCutAA", false);
   
  double JetR               = CL.GetDouble("JetR", 0.4);
  bool Recluster            = CL.GetBool("Recluster", false);
   
  DataHelper DHFile(DHFileName);
  cout<<"Trigger_test_code"<<endl;

  JetPF JetIDMin, JetIDMax;
  if(DoJetID == true)
    {
      Assert(DHFile.Exist("JetID"), "Jet ID state not exist");

      JetIDMin.CHF = DHFile["JetID"][DHJetIDKeyBase+"_CHFMinCut"].GetDouble();
      JetIDMax.CHF = DHFile["JetID"][DHJetIDKeyBase+"_CHFMaxCut"].GetDouble();
      JetIDMin.CEF = DHFile["JetID"][DHJetIDKeyBase+"_CEFMinCut"].GetDouble();
      JetIDMax.CEF = DHFile["JetID"][DHJetIDKeyBase+"_CEFMaxCut"].GetDouble();
      JetIDMin.NHF = DHFile["JetID"][DHJetIDKeyBase+"_NHFMinCut"].GetDouble();
      JetIDMax.NHF = DHFile["JetID"][DHJetIDKeyBase+"_NHFMaxCut"].GetDouble();
      JetIDMin.NEF = DHFile["JetID"][DHJetIDKeyBase+"_NEFMinCut"].GetDouble();
      JetIDMax.NEF = DHFile["JetID"][DHJetIDKeyBase+"_NEFMaxCut"].GetDouble();
      JetIDMin.MUF = DHFile["JetID"][DHJetIDKeyBase+"_MUFMinCut"].GetDouble();
      JetIDMax.MUF = DHFile["JetID"][DHJetIDKeyBase+"_MUFMaxCut"].GetDouble();
      JetIDMin.M   = DHFile["JetID"][DHJetIDKeyBase+"_MultiplicityMinCut"].GetInteger();
      JetIDMax.M   = DHFile["JetID"][DHJetIDKeyBase+"_MultiplicityMaxCut"].GetInteger();
      JetIDMin.CM  = DHFile["JetID"][DHJetIDKeyBase+"_ChargedMultiplicityMinCut"].GetInteger();
      JetIDMax.CM  = DHFile["JetID"][DHJetIDKeyBase+"_ChargedMultiplicityMaxCut"].GetInteger();

      if(JetIDMin.CHF == JetIDMax.CHF)   JetIDMin.CHF = -999999, JetIDMax.CHF = 999999;
      if(JetIDMin.CEF == JetIDMax.CEF)   JetIDMin.CEF = -999999, JetIDMax.CEF = 999999;
      if(JetIDMin.NHF == JetIDMax.NHF)   JetIDMin.NHF = -999999, JetIDMax.NHF = 999999;
      if(JetIDMin.NEF == JetIDMax.NEF)   JetIDMin.NEF = -999999, JetIDMax.NEF = 999999;
      if(JetIDMin.MUF == JetIDMax.MUF)   JetIDMin.MUF = -999999, JetIDMax.MUF = 999999;
      if(JetIDMin.CM == JetIDMax.CM)     JetIDMin.CM = -999999, JetIDMax.CM = 999999;
      if(JetIDMin.M == JetIDMax.M)       JetIDMin.M = -999999, JetIDMax.M = 999999;
    }

  JetCorrector JEC(JECFile);

  // PdfFileHelper PdfFile(OutputBase + ".pdf");
  // PdfFile.AddTextPage("Trigger plot");

  TFile OutputFile((OutputBase + ".root").c_str(), "RECREATE");

  TH1D HAll("HAll", ";Jet p_{T};", 250, 0, 500);
  TH1D HTrigger("HTrigger", ";Jet p_{T};", 250, 0, 500);

  TTree TriggerTree("TriggerTree", "Trigger tree");
  int TreeRun, TreeLumi;
  float TreeCentrality;
  float TreeJetCHF, TreeJetCEF, TreeJetNHF, TreeJetNEF, TreeJetMUF;
  int TreeJetM, TreeJetCM;
  float TreeJetPT, TreeJetEta, TreeJetPhi, TreeJetUE;
  bool TreePassBase, TreePassTrigger;
  TriggerTree.Branch("Run", &TreeRun, "Run/I");
  TriggerTree.Branch("Lumi", &TreeLumi, "Lumi/I");
  TriggerTree.Branch("Centrality", &TreeCentrality, "Centrality/F");
  TriggerTree.Branch("JetPT", &TreeJetPT, "JetPT/F");
  TriggerTree.Branch("JetEta", &TreeJetEta, "JetEta/F");
  TriggerTree.Branch("JetPhi", &TreeJetPhi, "JetPhi/F");
  TriggerTree.Branch("JetCHF", &TreeJetCHF, "JetCHF/F");
  TriggerTree.Branch("JetCEF", &TreeJetCEF, "JetCEF/F");
  TriggerTree.Branch("JetNHF", &TreeJetNHF, "JetNHF/F");
  TriggerTree.Branch("JetNEF", &TreeJetNEF, "JetNEF/F");
  TriggerTree.Branch("JetMUF", &TreeJetMUF, "JetMUF/F");
  TriggerTree.Branch("JetM", &TreeJetM, "JetM/I");
  TriggerTree.Branch("JetCM", &TreeJetCM, "JetCM/I");
  TriggerTree.Branch("PassBase", &TreePassBase, "PassBase/O");
  TriggerTree.Branch("PassTrigger", &TreePassTrigger, "PassTrigger/O");
   
  //cout<<"Trigger_test_code0"<<endl;

   
  for(string FileName : InputFiles)
    {
      TFile File(FileName.c_str());

      HiEventTreeMessenger MEvent(File);
      JetTreeMessenger MJet(File, JetName);
      TriggerTreeMessenger MTrigger(File, "hltanalysis/HltTree");
      SkimTreeMessenger MSkim(File);
      PFTreeMessenger MPF(File, "pfcandAnalyzer/pfTree");
      RhoTreeMessenger MRho(File, "hiPuRhoAnalyzer/t");

      int EntryCount = MEvent.Tree->GetEntries();
      ProgressBar Bar(cout, EntryCount);
      Bar.SetStyle(2);

      for(int iE = 0; iE < EntryCount; iE++)
	{
	  if(EntryCount < 500 || (iE % (EntryCount / 250)) == 0)
	    {
	      Bar.Update(iE);
	      Bar.Print();
	    }

	  MEvent.GetEntry(iE);
	  MJet.GetEntry(iE);
	  MTrigger.GetEntry(iE);
	  MSkim.GetEntry(iE);
	  if(Recluster == true)
            MPF.GetEntry(iE);
	  MRho.GetEntry(iE);

	  double Centrality = (MEvent.hiBin + 0.5) * 0.005;
	  //cout<<"Centrality: "<<Centrality<<'\t'<<"CentralityMin: "<<CentralityMin<<'\t'<<"CentralityMax: "<<CentralityMax<<endl;
	  if(ReEvaluateCentrality == true)
            {	
				Centrality = GetCentrality(MEvent.hiHF, 0);
				//cout<<"Centrality: "<<Centrality<<'\t'<<"CentralityMin: "<<CentralityMin<<'\t'<<"CentralityMax: "<<CentralityMax<<endl;
			}
	  //cout<<"Centrality: "<<Centrality<<'\t'<<"CentralityMin: "<<CentralityMin<<'\t'<<"CentralityMax: "<<CentralityMax<<endl;

	//Disaballed to check if pp runs or not////
	  if(Centrality < CentralityMin)
           continue;
	  if(Centrality > CentralityMax)
           continue;

//Disaballed to check if pp runs or not////


	  if(MTrigger.CheckTriggerStartWith(BaseTrigger) <= 0)
	    continue;
	  //////////////////////////////////////////
	  ///////////////////////////////
	  int Decision = MTrigger.CheckTriggerStartWith(Trigger);


	  if(DoBaselineCutAA == true)
	    {
	      // if(MSkim.HBHENoiseRun2Loose != 1)
	      //   continue;
	      if(MSkim.PVFilter != 1)
			continue;
	      if(MSkim.ClusterCompatibilityFilter != 1)
			continue;
	      if(MSkim.HFCoincidenceFilter2Th4 != 1)
			continue;
	    }
	  if(DoBaselineCutPP == true)
	    {
	      //if(MSkim.HBHENoiseRun2Loose != 1)
	      //   continue;
	      if(MSkim.PVFilter != 1)
			continue;
	      if(MSkim.ClusterCompatibilityFilter != 1)
			continue;
	      //if(MSkim.BeamScrapingFilter != 1)
	      //  continue;
	    }

	  FourVector LeadingJet(0, 0, 0, 0);
	  double LeadingJetUE = 0;
	  JetPF LeadingJetID;

	  if(Recluster == false)
	    {
	      int LeadingJetIndex = 0;
	      for(int iJ = 0; iJ < MJet.JetCount; iJ++)
		{
		  double UE = GetUE(MRho, MJet.JetEta[iJ], JetR);
               
		  JEC.SetJetPT(MJet.JetRawPT[iJ]);
		  JEC.SetJetEta(MJet.JetEta[iJ]);
		  JEC.SetJetPhi(MJet.JetPhi[iJ]);
		  JEC.SetRho(UE / (JetR * JetR * M_PI));
		  MJet.JetPT[iJ] = JEC.GetCorrectedPT();

		  JetPF JetID;
		  JetID.CHF = MJet.JetPFCHF[iJ];
		  JetID.CEF = MJet.JetPFCEF[iJ];
		  JetID.NHF = MJet.JetPFNHF[iJ];
		  JetID.NEF = MJet.JetPFNEF[iJ];
		  JetID.MUF = MJet.JetPFMUF[iJ];
		  JetID.CM  = MJet.JetPFCHM[iJ] + MJet.JetPFCEM[iJ];
		  JetID.M   = JetID.CM + MJet.JetPFNHM[iJ] + MJet.JetPFNEM[iJ];

		  if(DoJetID == true)
		    if(PassJetID(JetID, JetIDMin, JetIDMax) == false)
		      continue;
               
		  if(MJet.JetPT[iJ] > MJet.JetPT[LeadingJetIndex])
		    {
		      LeadingJetIndex = iJ;
		      LeadingJetUE = UE;
		      LeadingJetID = JetID;
		    }
		}

	      LeadingJet.SetPtEtaPhi(MJet.JetPT[LeadingJetIndex], MJet.JetEta[LeadingJetIndex], MJet.JetPhi[LeadingJetIndex]);
	    }
	  else
	    {
	      vector<PseudoJet> Particles;
	      for(int iPF = 0; iPF < MPF.ID->size(); iPF++)
		{
		  FourVector P;
		  P.SetPtEtaPhi(MPF.PT->at(iPF), MPF.Eta->at(iPF), MPF.Phi->at(iPF));
		  P[0] = MPF.E->at(iPF);

		  Particles.emplace_back(PseudoJet(P[1], P[2], P[3], P[0]));
		}

	      JetDefinition Definition(antikt_algorithm, JetR);
	      ClusterSequence Sequence(Particles, Definition);
	      vector<PseudoJet> FastJets = Sequence.inclusive_jets(0.5);   // anti-kt, R = 0.4

	      for(int iR = 0; iR < FastJets.size(); iR++)
		{
		  if(FastJets[iR].eta() < EtaMin || FastJets[iR].eta() > EtaMax)
		    continue;

		  PseudoJet &J = FastJets[iR];
		  FourVector P(J.e(), J.px(), J.py(), J.pz());

		  double UE = GetUE(MRho, P.GetEta(), JetR);

		  JEC.SetJetPT(P.GetPT());
		  JEC.SetJetEta(P.GetEta());
		  JEC.SetJetPhi(P.GetPhi());
		  JEC.SetRho(UE / (JetR * JetR * M_PI));
		  P = P * JEC.GetCorrection();

		  if(P.GetPT() > LeadingJet.GetPT())
		    {
		      LeadingJet = P;
		      LeadingJetUE = UE;
		    }
		}
	    }

	  if(LeadingJet.GetEta() < EtaMin || LeadingJet.GetEta() > EtaMax)
        continue;

	  HAll.Fill(LeadingJet.GetPT());
	  //cout<<"Hall: "<<LeadingJet.GetPT()<<endl;
	  if(Decision > 0)
        {
			HTrigger.Fill(LeadingJet.GetPT());
			//cout<<"HTrigger: "<<LeadingJet.GetPT()<<endl;
			//cout<<"Centrality: "<<Centrality<<'\t'<<"CentralityMin: "<<CentralityMin<<'\t'<<"CentralityMax: "<<CentralityMax<<'\t'<<"HTrigger: "<<LeadingJet.GetPT()<<endl;

		}

	  if(WriteTree == true)
	    {
	      TreeRun         = MEvent.Run;
	      TreeLumi        = MEvent.Lumi;
	      TreeCentrality  = Centrality;
	      TreeJetPT       = LeadingJet.GetPT();
	      TreeJetEta      = LeadingJet.GetEta();
	      TreeJetPhi      = LeadingJet.GetPhi();
	      TreeJetUE       = LeadingJetUE;
	      TreeJetCHF      = LeadingJetID.CHF;
	      TreeJetCEF      = LeadingJetID.CEF;
	      TreeJetNHF      = LeadingJetID.NHF;
	      TreeJetNEF      = LeadingJetID.NEF;
	      TreeJetMUF      = LeadingJetID.MUF;
	      TreeJetM        = LeadingJetID.M;
	      TreeJetCM       = LeadingJetID.CM;
	      TreePassBase    = true;
	      TreePassTrigger = Decision;

	      TriggerTree.Fill();
	    }
	}
     // cout<<"Trigger_test_code1"<<endl;

      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();

      File.Close();
      	 
      //////////////////////////////////
      //cout<<"Trigger_test_code2"<<endl;
      ///////////////////////////////

    }

  // PdfFile.AddPlot(HAll);
  // PdfFile.AddPlot(HTrigger);

  OutputFile.cd();

  // TGraphAsymmErrors G;
  // G.Divide(&HTrigger, &HAll);
  // PdfFile.AddPlot(G, "ap");

  HTrigger.Write();
  HAll.Write();
  // G.Write();

  if(WriteTree == true)
    TriggerTree.Write();

  OutputFile.Close();

  // PdfFile.AddTimeStampPage();
  // PdfFile.Close();

  return 0;
}

double GetUE(RhoTreeMessenger &M, double Eta, double R)
{
  double Result = 0;

  if(M.EtaMin == nullptr)
    return -1;

  int NBin = M.EtaMin->size();
  if(NBin == 0)
    return -1;

  for(int i = 0; i < NBin; i++)
    {
      if(M.EtaMax->at(i) < Eta - R)
	continue;
      if(M.EtaMin->at(i) > Eta + R)
	continue;

      double XMin = (max(M.EtaMin->at(i), Eta - R) - Eta) / R;
      double XMax = (min(M.EtaMax->at(i), Eta + R) - Eta) / R;

      if(XMin <= -1)
	XMin = -0.99999;
      if(XMax >= +1)
	XMax = +0.99999;

      double High = XMax * sqrt(1 - XMax * XMax) + asin(XMax);
      double Low = XMin * sqrt(1 - XMin * XMin) + asin(XMin);

      Result = Result + R * R * (High - Low) * M.Rho->at(i);
    }
  //cout<<"Trigger_test_code4"<<endl;

  return Result;
}

bool PassJetID(JetPF &ID, JetPF &Min, JetPF &Max)
{
  if(ID.CHF <= Min.CHF)   return false;
  if(ID.CHF >= Max.CHF)   return false;
  if(ID.CEF <= Min.CEF)   return false;
  if(ID.CEF >= Max.CEF)   return false;
  if(ID.NHF <= Min.NHF)   return false;
  if(ID.NHF >= Max.NHF)   return false;
  if(ID.NEF <= Min.NEF)   return false;
  if(ID.NEF >= Max.NEF)   return false;
  if(ID.MUF <= Min.MUF)   return false;
  if(ID.MUF >= Max.MUF)   return false;
  if(ID.CM <= Min.CM)     return false;
  if(ID.CM >= Max.CM)     return false;
  if(ID.M <= Min.M)       return false;
  if(ID.M >= Max.M)       return false;
  return true;
}

double GetCentrality(double hiHF, int Variation)
{
  static int BinCount = 200;


  static double BinEdge[201] = {0, 10.3417, 11.0768, 11.7884, 12.5014, 13.223, 13.979, 14.7243, 15.4805, 16.2881,
				17.0953, 17.9161, 18.7604, 19.6304, 20.551, 21.5208, 22.5138, 23.5369, 24.5918,
				25.6714, 26.8101, 28.006, 29.2436, 30.5117, 31.8466, 33.228, 34.6998, 36.2222,
				37.8447, 39.5591, 41.3376, 43.1468, 45.0235, 47.0435, 49.0904, 51.2926, 53.6121,
				56.0542, 58.5133, 61.1585, 63.8113, 66.639, 69.5748, 72.568, 75.676, 78.9026,
				82.3018, 85.7734, 89.444, 93.2175, 97.2133, 101.253, 105.319, 109.546, 113.867,
				118.346, 123.019, 127.884, 132.907, 138.133, 143.553, 149.142, 154.94, 160.848,
				166.99, 173.359, 179.998, 186.842, 193.842, 201.117, 208.532, 216.211, 224.158,
				232.3, 240.736, 249.435, 258.302, 267.463, 277.014, 286.845, 296.83, 307.189,
				317.783, 328.756, 339.992, 351.395, 363.16, 375.455, 387.794, 400.501, 413.561,
				426.97, 440.651, 454.496, 468.886, 483.371, 498.4, 513.754, 529.457, 545.441,
				561.988, 578.779, 595.858, 613.597, 631.446, 649.849, 668.467, 687.351, 706.767,
				726.71, 747.018, 767.66, 788.701, 810.28, 832.266, 854.725, 877.463, 900.464,
				924.078, 948.465, 973.01, 998.014, 1023.62, 1049.62, 1075.96, 1102.82, 1129.95,
				1157.78, 1186.03, 1215.15, 1244.29, 1274.23, 1304.67, 1335.36, 1366.46, 1398.45,
				1431.19, 1464.02, 1497.65, 1531.96, 1566.77, 1602.1, 1637.92, 1674.27, 1711.1,
				1749.07, 1787.21, 1826.02, 1865.25, 1905.66, 1946.56, 1987.95, 2030.81, 2073.58,
				2117.19, 2161.79, 2206.96, 2252.79, 2300.01, 2347.25, 2395.35, 2444.79, 2494.51,
				2544.84, 2596.27, 2649.26, 2703.27, 2758.12, 2813.87, 2870.23, 2927.2, 2985.36,
				3045.09, 3105.31, 3166.48, 3229.14, 3293.26, 3359.1, 3425.63, 3493.44, 3562.41,
				3633.68, 3706.37, 3780.38, 3856.7, 3934.4, 4013.53, 4095.4, 4178.3, 4263.29,
				4350.7, 4440.56, 4532.84, 4628.66, 4727.32, 4827.83, 4933.06, 5042.94, 5161.8,
				5305.56, 8171.19};
   
  static double BinEdgeDown[201] = {0, 10.3463, 11.0861, 11.7998, 12.5172, 13.2458, 14.0069, 14.7568, 15.519, 16.3334,
				    17.1454, 17.9715, 18.8204, 19.7036, 20.6264, 21.605, 22.6104, 23.6387, 24.7063,
				    25.796, 26.9534, 28.1605, 29.4073, 30.6918, 32.0377, 33.4416, 34.9364, 36.4788,
				    38.1179, 39.8435, 41.6583, 43.4757, 45.4068, 47.4261, 49.5297, 51.6006, 53.7062,
				    55.9019, 58.199, 60.5762, 63.0623, 65.6426, 68.3145, 71.1183, 74.0371, 77.0526,
				    80.1847, 83.4274, 86.8108, 90.3357, 93.9759, 97.7734, 101.745, 105.854, 110.126,
				    114.482, 119.024, 123.747, 128.668, 133.745, 139.036, 144.527, 150.19, 156.04,
				    162.051, 168.268, 174.704, 181.437, 188.373, 195.443, 202.785, 210.302, 218.137,
				    226.175, 234.398, 242.965, 251.767, 260.763, 270.085, 279.822, 289.708, 299.86,
				    310.338, 321.122, 332.245, 343.563, 355.174, 367.154, 379.524, 392.109, 405.031,
				    418.299, 431.845, 445.699, 459.798, 474.234, 489.117, 504.386, 519.952, 535.84,
				    552.219, 569.001, 585.895, 603.3, 621.38, 639.438, 658.099, 676.89, 696.136,
				    715.903, 736.115, 756.791, 777.69, 799.191, 821.022, 843.312, 866.052, 889.133,
				    912.586, 936.843, 961.302, 986.171, 1011.74, 1037.65, 1064.03, 1090.8, 1118.01,
				    1145.77, 1173.82, 1202.86, 1232.18, 1262.05, 1292.32, 1323.16, 1354.33, 1386.08,
				    1418.62, 1451.56, 1485.4, 1519.47, 1554.19, 1589.76, 1625.49, 1661.89, 1698.83,
				    1736.63, 1775, 1813.88, 1853.12, 1893.44, 1934.42, 1975.76, 2018.46, 2061.74,
				    2105.37, 2149.7, 2194.95, 2240.94, 2288.16, 2335.73, 2383.92, 2433.41, 2483.29,
				    2533.65, 2585.27, 2638.35, 2692.45, 2747.33, 2803.27, 2859.75, 2916.98, 2975.45,
				    3035.05, 3095.57, 3156.99, 3219.67, 3284.06, 3349.98, 3417.04, 3485.29, 3554.01,
				    3625.58, 3698.49, 3772.71, 3849.45, 3927.34, 4006.82, 4089.11, 4172.24, 4257.81,
				    4345.44, 4435.8, 4528.43, 4624.6, 4723.91, 4824.73, 4930.41, 5040.92, 5160.37, 5304.54, 8171.19};

   
  static double BinEdgeUp[201] = {0, 10.3379, 11.0725, 11.7802, 12.4919, 13.21, 13.9601, 14.7025, 15.4608, 16.2614,
				  17.0642, 17.8839, 18.7243, 19.5915, 20.501, 21.4676, 22.4592, 23.4754, 24.5231,
				  25.597, 26.7278, 27.914, 29.1421, 30.4015, 31.7342, 33.0996, 34.549, 36.0668,
				  37.6735, 39.3801, 41.1438, 42.9432, 44.8014, 46.7956, 48.853, 51.0064, 53.3073,
				  55.7264, 58.1748, 60.78, 63.436, 66.2411, 69.112, 72.1054, 75.1783, 78.3803, 81.7626,
				  85.1756, 88.7616, 92.5521, 96.492, 100.55, 104.72, 109.084, 113.514, 118.197, 123.071,
				  128.061, 133.341, 138.724, 144.257, 149.905, 155.906, 162.144, 168.39, 174.967, 181.708,
				  188.672, 196.01, 203.584, 211.181, 219.091, 227.066, 235.409,244.073, 253.075, 262.124,
				  271.539, 281.203, 290.991, 301.253, 311.645, 322.338, 333.437, 344.657, 356.169, 368.062,
				  380.364, 392.808, 405.651, 418.805, 432.246, 445.973, 459.933, 474.24, 488.99, 504.095,
				  519.521, 535.239, 551.417, 568.002, 584.782, 601.983, 619.827, 637.684, 656.132, 674.811,
				  693.728, 713.198, 733.254,753.661, 774.269, 795.462, 817.018, 839.074, 861.556, 884.288,
				  907.465, 931.318, 955.542, 980.139, 1005.18, 1030.77, 1056.79, 1083.18, 1110.08, 1137.3,
				  1165.1, 1193.47, 1222.35, 1251.81, 1281.66, 1311.96, 1342.79, 1373.97, 1406.01, 1438.58,
				  1471.65, 1505.24, 1539.46, 1574.28, 1609.71, 1645.39, 1681.8, 1718.61, 1756.42, 1794.69,
				  1833.55, 1872.86, 1913.08, 1953.8, 1995.29, 2038.11, 2080.97, 2124.46, 2168.93, 2214.04,
				  2260.09, 2307.06, 2354.41, 2402.35, 2451.68, 2501.29, 2551.71, 2603.17, 2655.91, 2709.96,
				  2764.55, 2820.31, 2876.54, 2933.35, 2991.78, 3051.22, 3111.18, 3172.27, 3234.87, 3298.85,
				  3364.68, 3430.89, 3498.44, 3567.44, 3638.38, 3711.01, 3784.97, 3861.04, 3938.56, 4017.48,
				  4099.05, 4181.81, 4266.65, 4353.98, 4443.42, 4535.56, 4630.97, 4729.41, 4829.54, 4934.49,
				  5044.11, 5162.64, 5306.21, 8171.19};



  /*
   
    static double BinEdge[201] = {0, 10.5072, 11.2099, 11.8364, 12.478, 13.1194, 13.7623, 14.4081, 15.0709, 15.7532, 16.4673, 17.1881, 17.923, 18.673, 19.4865, 20.3033, 21.1536, 22.0086, 22.9046, 23.8196, 24.7924, 25.8082, 26.8714, 27.9481, 29.0828, 30.2757, 31.5043, 32.8044, 34.1572, 35.6142, 37.1211, 38.6798, 40.3116, 42.0398, 43.8572, 45.6977, 47.6312, 49.6899, 51.815, 54.028, 56.3037, 58.7091, 61.2024, 63.8353, 66.5926, 69.3617, 72.2068, 75.2459, 78.3873, 81.5916, 84.9419, 88.498, 92.1789, 95.9582, 99.8431, 103.739, 107.78, 111.97, 116.312, 120.806, 125.46, 130.269, 135.247, 140.389, 145.713, 151.212, 156.871, 162.729, 168.762, 174.998, 181.424, 188.063, 194.907, 201.942, 209.19, 216.683, 224.37, 232.291, 240.43, 248.807, 257.416, 266.256, 275.348, 284.668, 294.216, 304.053, 314.142, 324.488, 335.101, 345.974, 357.116, 368.547, 380.283, 392.29, 404.564, 417.122, 429.968, 443.116, 456.577, 470.357, 484.422, 498.78, 513.473, 528.479, 543.813, 559.445, 575.411, 591.724, 608.352, 625.344, 642.686, 660.361, 678.371, 696.749, 715.485, 734.608, 754.068, 773.846, 794.046, 814.649, 835.608, 856.972, 878.719, 900.887, 923.409, 946.374, 969.674, 993.435, 1017.62, 1042.21, 1067.28, 1092.72, 1118.64, 1144.96, 1171.71, 1198.98, 1226.67, 1254.82, 1283.46, 1312.65, 1342.21, 1372.27, 1402.85, 1433.93, 1465.49, 1497.62, 1530.29, 1563.49, 1597.22, 1631.49, 1666.37, 1701.8, 1737.75, 1774.35, 1811.51, 1849.29, 1887.75, 1926.79, 1966.6, 2006.97, 2047.99, 2089.71, 2132.1, 2175.23, 2219.17, 2263.72, 2309.2, 2355.43, 2402.47, 2450.33, 2499.05, 2548.66, 2599.16, 2650.59, 2703.03, 2756.32, 2810.75, 2866.27, 2922.91, 2980.54, 3039.47, 3099.53, 3160.98, 3223.66, 3287.71, 3353.18, 3420.34, 3489.13, 3559.72, 3632.06, 3706.18, 3782.42, 3860.78, 3941.42, 4024.52, 4110.27, 4199.4, 4292.8, 4394.49, 4519.52, 5199.95};
    static double BinEdgeDown[201] = {0, 10.5071, 11.2094, 11.8357, 12.4763, 13.117, 13.7597, 14.4049, 15.0671, 15.7491, 16.4622, 17.1812, 17.9144, 18.6674, 19.4797, 20.2963, 21.1435, 21.9974, 22.8928, 23.8068, 24.7805, 25.7931, 26.8556, 27.9308, 29.0638, 30.2582, 31.4795, 32.7816, 34.1349, 35.5834, 37.0941, 38.6474, 40.2782, 42.0035, 43.8112, 45.6576, 47.5758, 49.6381, 51.6667, 53.7353, 55.8903, 58.1259, 60.4528, 62.8712, 65.3859, 67.9968, 70.7065, 73.5231, 76.4519, 79.4922, 82.6461, 85.9264, 89.3269, 92.8562, 96.5212, 100.322, 104.262, 108.344, 112.585, 116.971, 121.521, 126.225, 131.09, 136.127, 141.328, 146.721, 152.284, 158.014, 163.935, 170.054, 176.372, 182.878, 189.602, 196.532, 203.653, 211.017, 218.599, 226.387, 234.418, 242.667, 251.16, 259.886, 268.852, 278.071, 287.498, 297.2, 307.184, 317.409, 327.894, 338.66, 349.686, 360.996, 372.607, 384.508, 396.669, 409.133, 421.86, 434.906, 448.258, 461.916, 475.906, 490.16, 504.74, 519.663, 534.911, 550.453, 566.322, 582.525, 599.08, 615.968, 633.211, 650.805, 668.76, 687.048, 705.707, 724.774, 744.163, 763.9, 783.999, 804.528, 825.432, 846.746, 868.429, 890.523, 913.007, 935.952, 959.211, 982.919, 1007.08, 1031.63, 1056.62, 1082.08, 1107.96, 1134.24, 1160.99, 1188.22, 1215.91, 1244.06, 1272.69, 1301.85, 1331.45, 1361.51, 1392.07, 1423.18, 1454.77, 1486.93, 1519.57, 1552.81, 1586.55, 1620.87, 1655.79, 1691.26, 1727.27, 1763.93, 1801.12, 1838.97, 1877.47, 1916.61, 1956.45, 1996.89, 2038.04, 2079.84, 2122.35, 2165.52, 2209.53, 2254.24, 2299.83, 2346.19, 2393.31, 2441.28, 2490.16, 2539.86, 2590.57, 2642.16, 2694.74, 2748.23, 2802.81, 2858.47, 2915.33, 2973.2, 3032.28, 3092.56, 3154.24, 3217.19, 3281.45, 3347.18, 3414.6, 3483.65, 3554.56, 3627.2, 3701.66, 3778.25, 3856.97, 3937.98, 4021.48, 4107.62, 4197.21, 4291.05, 4393.19, 4518.6, 5199.95};
    static double BinEdgeUp[201] = {0, 10.5075, 11.2107, 11.838, 12.4797, 13.1213, 13.7641, 14.4124, 15.0745, 15.7577, 16.473, 17.1939, 17.9297, 18.6812, 19.4958, 20.3143, 21.1648, 22.0218, 22.9159, 23.8328, 24.8059, 25.8204, 26.89, 27.9702, 29.1042, 30.3022, 31.528, 32.8347, 34.1896, 35.6439, 37.1542, 38.7172, 40.3518, 42.091, 43.9053, 45.7415, 47.6853, 49.7457, 51.8755, 54.0983, 56.3594, 58.7848, 61.2861, 63.9228, 66.6825, 69.4421, 72.297, 75.3547, 78.4967, 81.6977, 85.0755, 88.6211, 92.3058, 96.1071, 99.9975, 104.065, 108.272, 112.512, 116.906, 121.601, 126.465, 131.482, 136.866, 142.229, 147.786, 153.546, 159.571, 165.586, 171.902, 178.419, 185.063, 191.856, 199.055, 206.261, 213.999, 221.719, 229.671, 237.84, 246.088, 254.828, 263.883, 272.907, 282.236, 291.925, 301.519, 311.477, 321.691, 332.153, 342.892, 353.878, 365.161, 376.742, 388.577, 400.684, 413.075, 425.746, 438.711, 451.989, 465.556, 479.45, 493.608, 508.077, 522.891, 538.003, 553.415, 569.151, 585.216, 601.601, 618.354, 635.422, 652.84, 670.599, 688.699, 707.161, 726.014, 745.185, 764.687, 784.557, 804.838, 825.489, 846.537, 867.951, 889.752, 911.955, 934.588, 957.52, 980.912, 1004.73, 1028.94, 1053.57, 1078.67, 1104.17, 1130.07, 1156.39, 1183.2, 1210.47, 1238.17, 1266.38, 1295.02, 1324.16, 1353.71, 1383.77, 1414.35, 1445.41, 1477, 1509.09, 1541.74, 1574.88, 1608.59, 1642.83, 1677.66, 1713.07, 1748.98, 1785.47, 1822.63, 1860.33, 1898.72, 1937.73, 1977.42, 2017.71, 2058.62, 2100.25, 2142.57, 2185.56, 2229.38, 2273.91, 2319.2, 2365.33, 2412.22, 2459.94, 2508.52, 2557.98, 2608.35, 2659.61, 2711.86, 2765, 2819.23, 2874.58, 2930.97, 2988.46, 3047.12, 3106.95, 3168.15, 3230.6, 3294.37, 3359.58, 3426.47, 3494.95, 3565.21, 3637.21, 3711.03, 3786.91, 3864.85, 3945.11, 4027.8, 4113.06, 4201.73, 4294.72, 4395.9, 4520.5, 5199.95};

  */

  for(int i = 0; i < BinCount; i++)
    {
      if(Variation == 0 && hiHF >= BinEdge[i] && hiHF < BinEdge[i+1])
	return (double)(BinCount - 1 - i) / BinCount;
      if(Variation > 0 && hiHF >= BinEdgeUp[i] && hiHF < BinEdgeUp[i+1])
	return (double)(BinCount - 1 - i) / BinCount;
      if(Variation < 0 && hiHF >= BinEdgeDown[i] && hiHF < BinEdgeDown[i+1])
	return (double)(BinCount - 1 - i) / BinCount;
    }

  return -1;
}


