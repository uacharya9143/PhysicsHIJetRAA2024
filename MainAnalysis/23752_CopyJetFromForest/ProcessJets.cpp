#include <vector>
#include <iostream>
#include <map>
using namespace std;

#include "fastjet/ClusterSequence.hh"
using namespace fastjet;

#include "TFile.h"
#include "TTree.h"
#include "TF1.h"

#include "ProgressBar.h"
#include "CommandLine.h"
#include "CustomAssert.h"
#include "TauHelperFunctions3.h"
#include "DataHelper.h"

#include "Messenger.h"
#include "JetCorrector.h"
#include "JetUncertainty.h"

#define MAX 10240

struct JetPF;
int main(int argc, char *argv[]);
bool IsExcluded(double Eta, double Phi, vector<double> &Exclusion);
double GetRhoAtCenter(RhoTreeMessenger &M, double Eta);
double GetUE(RhoTreeMessenger &M, double Eta, double R);
double JetPhiWeight(double Eta, vector<double> &JetExclusion);
double GetRhoWeight(DataHelper &DHFile, string &Key, double UE);
bool PassJetID(JetPF &ID, JetPF &Min, JetPF &Max);
double GetCentrality(double hiHF, int Variation);
void ZeroArray(float *X, int N);
void ZeroArray(float *X, int N, float InitialValue);  // Forward declaration


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
   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input", vector<string>{"SampleExample/HiForestAOD.root"});
   string OutputFileName         = CL.Get("Output", "Output/JetTreeAOD.root");

   bool KeepSkippedEvent         = CL.GetBool("KeepSkippedEvent", true);
   bool DontStoreReco            = CL.GetBool("DontStoreReco", false);
   bool DontStoreGen             = CL.GetBool("DontStoreGen", false);
   bool DontStoreMatch           = CL.GetBool("DontStoreMatch", false);

   string DHFileName             = CL.Get("DHFile", "GlobalSetting.dh");
   string DHKeyBase              = CL.Get("RhoKeyBase", "none");
   bool CutUE                    = CL.GetBool("CutUE", false);

   double Fraction               = CL.GetDouble("Fraction", 1.00);

   bool DoBaselineCutPP          = CL.GetBool("DoBaselineCutPP", false);
   bool DoBaselineCutAA          = CL.GetBool("DoBaselineCutAA", false);

   string JetName                = CL.Get("Jet", "akFlowPuCs4PFJetAnalyzer/t");
   double JetR                   = CL.GetDouble("JetR", 0.4);
//////////////////////////////////////////////////////////////////////// Added by Uttam for diferent jetRadius
   // Smart ΔR: Use R/2 for small jets, cap for large jets
   double MaxMatchDRCap          = CL.GetDouble("MaxMatchDRCap", 0.3);
   double DefaultMatchDR         = min(JetR / 2.0, MaxMatchDRCap);
   double MaxMatchDR             = CL.GetDouble("MaxMatchDR", DefaultMatchDR);
////////////////////////////////////////////////////////////////////////Added by Uttam for diferent jetRadius

   cout << "=== Matching Configuration ===" << endl;
   cout << "Jet Radius (R): " << JetR << endl;
   cout << "Matching ΔR: " << MaxMatchDR << endl;
   cout << "==============================" << endl;

   vector<string> JECFile        = CL.GetStringVector("JEC", vector<string>{"None"});
   string JEUFile                = CL.Get("JEU", "None");
   string Trigger                = CL.Get("Trigger", "NONE");

   bool DoJetID                  = CL.GetBool("DoJetID", false);
   string DHJetIDKeyBase         = (DoJetID ? CL.Get("JetIDKeyBase") : "none");

   string PFName                 = CL.Get("PF", "pfcandAnalyzer/pfTree");
   string PFName2                = CL.Get("PF2", "particleFlowAnalyser/pftree");

   double EtaMin                 = CL.GetDouble("EtaMin", -2.0);
   double EtaMax                 = CL.GetDouble("EtaMax", +2.0);

   double PTMin                  = CL.GetDouble("PTMin", 0);
   double GenPTMin               = CL.GetDouble("GenPTMin", 0);

   bool CheckCentrality          = CL.GetBool("CheckCentrality", true);
   bool ReEvaluateCentrality     = CL.GetBool("ReEvaluateCentrality", false);
   int CentralityShift           = ReEvaluateCentrality ? CL.GetInt("CentralityShift") : 0;
   double CentralityMin          = CL.GetDouble("CentralityMin", 0.00);
   double CentralityMax          = CL.GetDouble("CentralityMax", 1.00);

   bool UseStoredGen             = CL.GetBool("UseStoredGen", false);
   bool UseStoredReco            = CL.GetBool("UseStoredReco", true);
   bool DoRecoSubtraction        = UseStoredReco ? CL.GetBool("DoRecoSubtraction") : false;

   Assert(DoRecoSubtraction == false, "reco reclustering in PbPb case not implemented yet");
   
   vector<double> JetExclusion   = CL.GetDoubleVector("Exclusion", vector<double>{});

   Assert(JetExclusion.size() % 4 == 0, "Exclusion zone format: (eta min, eta max, phi min, phi max)");

   DataHelper DHFile(DHFileName);
   GetRhoWeight(DHFile, DHKeyBase, 0);   // Initialize rho weight function

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
   JetUncertainty JEU(JEUFile);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TTree OutputTree("UnfoldingTreeFlat", "Trees for unfolding studies");

   double EventWeight = 1;
   double PTHat = 1;
   int Run;
   int Lumi;
   long long Event;
   double Centrality = 0;
   int NRecoJets = 0;
   float RecoJetPT[MAX];
   float RecoJetEta[MAX];
   float RecoJetPhi[MAX];
   float RecoJetMass[MAX];
   float RecoJetJEC[MAX];
   float RecoJetJEU[MAX];
   float RecoJetRho[MAX];
   float RecoJetUE[MAX];
   float RecoJetWeight[MAX];
   float RecoJetRhoWeight[MAX];
   float RecoJetPhiWeight[MAX];
   int NGenJets = 0;
   float GenJetPT[MAX];
   float GenJetEta[MAX];
   float GenJetPhi[MAX];
   float GenJetMass[MAX];
   float GenJetWeight[MAX];
   float GenJetPhiWeight[MAX];
   float MatchedJetPT[MAX];
   float MatchedJetEta[MAX];
   float MatchedJetPhi[MAX];
   float MatchedJetMass[MAX];
   float MatchedJetAngle[MAX];
   float MatchedJetJEC[MAX];
   float MatchedJetJEU[MAX];
   float MatchedJetRho[MAX];
   float MatchedJetUE[MAX];
   float MatchedJetWeight[MAX];
   float MatchedJetRhoWeight[MAX];
   float MatchedJetPhiWeight[MAX];

   OutputTree.Branch("EventWeight",         &EventWeight,    "EventWeight/D");
   OutputTree.Branch("PTHat",               &PTHat,          "PTHat/D");
   OutputTree.Branch("Run",                 &Run,            "Run/I");
   OutputTree.Branch("Lumi",                &Lumi,           "Lumi/I");
   OutputTree.Branch("Event",               &Event,          "Event/L");
   OutputTree.Branch("Centrality",          &Centrality,     "Centrality/D");
   OutputTree.Branch("NRecoJets",           &NRecoJets,      "NRecoJets/I");
   if(DontStoreReco == false)
   {
      OutputTree.Branch("RecoJetPT",           &RecoJetPT,        "RecoJetPT[NRecoJets]/F");
      OutputTree.Branch("RecoJetEta",          &RecoJetEta,       "RecoJetEta[NRecoJets]/F");
      OutputTree.Branch("RecoJetPhi",          &RecoJetPhi,       "RecoJetPhi[NRecoJets]/F");
      OutputTree.Branch("RecoJetMass",         &RecoJetMass,      "RecoJetMass[NRecoJets]/F");
      OutputTree.Branch("RecoJetJEC",          &RecoJetJEC,       "RecoJetJEC[NRecoJets]/F");
      OutputTree.Branch("RecoJetJEU",          &RecoJetJEU,       "RecoJetJEU[NRecoJets]/F");
      OutputTree.Branch("RecoJetRho",          &RecoJetRho,       "RecoJetRho[NRecoJets]/F");
      OutputTree.Branch("RecoJetUE",           &RecoJetUE,        "RecoJetUE[NRecoJets]/F");
      OutputTree.Branch("RecoJetWeight",       &RecoJetWeight,    "RecoJetWeight[NRecoJets]/F");
      OutputTree.Branch("RecoJetPhiWeight",    &RecoJetPhiWeight, "RecoJetPhiWeight[NRecoJets]/F");
      OutputTree.Branch("RecoJetRhoWeight",    &RecoJetRhoWeight, "RecoJetRhoWeight[NRecoJets]/F");
   }
   OutputTree.Branch("NGenJets",            &NGenJets,       "NGenJets/I");
   if(DontStoreGen == false)
   {
      OutputTree.Branch("GenJetPT",            &GenJetPT,        "GenJetPT[NGenJets]/F");
      OutputTree.Branch("GenJetEta",           &GenJetEta,       "GenJetEta[NGenJets]/F");
      OutputTree.Branch("GenJetPhi",           &GenJetPhi,       "GenJetPhi[NGenJets]/F");
      OutputTree.Branch("GenJetMass",          &GenJetMass,      "GenJetMass[NGenJets]/F");
      OutputTree.Branch("GenJetWeight",        &GenJetWeight,    "GenJetWeight[NGenJets]/F");
      OutputTree.Branch("GenJetPhiWeight",     &GenJetPhiWeight, "GenJetPhiWeight[NGenJets]/F");
      if(DontStoreMatch == false)
      {
         OutputTree.Branch("MatchedJetPT",        &MatchedJetPT,        "MatchedJetPT[NGenJets]/F");
         OutputTree.Branch("MatchedJetEta",       &MatchedJetEta,       "MatchedJetEta[NGenJets]/F");
         OutputTree.Branch("MatchedJetPhi",       &MatchedJetPhi,       "MatchedJetPhi[NGenJets]/F");
         OutputTree.Branch("MatchedJetMass",      &MatchedJetMass,      "MatchedJetMass[NGenJets]/F");
         OutputTree.Branch("MatchedJetAngle",     &MatchedJetAngle,     "MatchedJetAngle[NGenJets]/F");
         OutputTree.Branch("MatchedJetJEC",       &MatchedJetJEC,       "MatchedJetJEC[NGenJets]/F");
         OutputTree.Branch("MatchedJetJEU",       &MatchedJetJEU,       "MatchedJetJEU[NGenJets]/F");
         OutputTree.Branch("MatchedJetRho",       &MatchedJetRho,       "MatchedJetRho[NGenJets]/F");
         OutputTree.Branch("MatchedJetUE",        &MatchedJetUE,        "MatchedJetUE[NGenJets]/F");
         OutputTree.Branch("MatchedJetWeight",    &MatchedJetWeight,    "MatchedJetWeight[NGenJets]/F");
         OutputTree.Branch("MatchedJetPhiWeight", &MatchedJetPhiWeight, "MatchedJetPhiWeight[NGenJets]/F");
         OutputTree.Branch("MatchedJetRhoWeight", &MatchedJetRhoWeight, "MatchedJetRhoWeight[NGenJets]/F");
      }
   }

   for(string InputFileName : InputFileNames)
   {
      TFile InputFile(InputFileName.c_str());

      HiEventTreeMessenger MEvent(InputFile);
      GenParticleTreeMessenger MGen(InputFile);
      JetTreeMessenger MJet(InputFile, JetName);
      TriggerTreeMessenger MTrigger(InputFile, "hltanalysis/HltTree");
      SkimTreeMessenger MSkim(InputFile);
      RhoTreeMessenger MRho(InputFile, "hiPuRhoAnalyzer/t");
      
      PFTreeMessenger MPF(InputFile, PFName);
      if(MPF.Tree == nullptr)
         MPF.Initialize(InputFile, PFName2);

      int EntryCount = MEvent.Tree->GetEntries() * Fraction;
      ProgressBar Bar(cout, EntryCount);
      Bar.SetStyle(-1);

      for(int iE = 0; iE < EntryCount; iE++)
      {
         Bar.Update(iE);
         if(EntryCount < 100 || (iE % (EntryCount / 80)) == 0)
            Bar.Print();

         MEvent.GetEntry(iE);
         MGen.GetEntry(iE);
         MJet.GetEntry(iE);
         MTrigger.GetEntry(iE);
         MPF.GetEntry(iE);
         MSkim.GetEntry(iE);
         MRho.GetEntry(iE);

         Run = MEvent.Run;
         Lumi = MEvent.Lumi;
         Event = MEvent.Event;

         EventWeight = MEvent.weight;
         PTHat = MJet.PTHat;

         bool SkipEvent = false;

         if(Trigger != "NONE" && Trigger != "None")
         {
            int Decision = MTrigger.CheckTriggerStartWith(Trigger);

            if(Decision < 0)
               cerr << "Warning! Trigger not found!" << endl;
            if(Decision == 0)   // trigger found and not fired
               SkipEvent = true;
         }

         if(ReEvaluateCentrality == false)
            Centrality = MEvent.hiBin * 0.005;
         else
            Centrality = GetCentrality(MEvent.hiHF, CentralityShift);

         // cout << Centrality << " " << MEvent.hiBin * 0.005 << endl;

         if(CheckCentrality == true)
         {
            if(Centrality < CentralityMin)
               SkipEvent = true;
            if(Centrality >= CentralityMax)
               SkipEvent = true;
         }

         if(DoBaselineCutAA == true)
         {
            if(MSkim.HBHENoiseRun2Loose != 1)
               SkipEvent = true;
            if(MSkim.PVFilter != 1)
               SkipEvent = true;
            if(MSkim.ClusterCompatibilityFilter != 1)
               SkipEvent = true;
            if(MSkim.HFCoincidenceFilter2Th4 != 1)
               SkipEvent = true;
         }
         if(DoBaselineCutPP == true)
         {
            if(MSkim.HBHENoiseRun2Loose != 1)
               SkipEvent = true;
            if(MSkim.PVFilter != 1)
               SkipEvent = true;
            if(MSkim.BeamScrapingFilter != 1)
               SkipEvent = true;
         }

         vector<pair<FourVector, PseudoJet>> GenJets;
         vector<pair<FourVector, PseudoJet>> RecoJets;
         vector<double> RecoJetJECVector;

         if(SkipEvent == false)
         {
            if(UseStoredGen == true)
            {
               for(int iG = 0; iG < MJet.GenCount; iG++)
               {
                  if(MJet.GenEta[iG] < EtaMin || MJet.GenEta[iG] > EtaMax)
                     continue;
                  if(MJet.GenPT[iG] < GenPTMin)
                     continue;

                  FourVector P;
                  P.SetPtEtaPhi(MJet.GenPT[iG], MJet.GenEta[iG], MJet.GenPhi[iG]);
                  PseudoJet J(P[1], P[2], P[3], P[0]);
                  GenJets.push_back(pair<FourVector, PseudoJet>(P, J));
               }
            }
            else
            {
               if(MGen.Tree != nullptr)
               {
                  // Cluster gen jets
                  vector<PseudoJet> VisibleParticles;
                  for(int iG = 0; iG < MGen.ID->size(); iG++)
                  {
                     FourVector P;
                     P.SetPtEtaPhi(MGen.PT->at(iG), MGen.Eta->at(iG), MGen.Phi->at(iG));

                     if(MGen.ID->at(iG) == 12 || MGen.ID->at(iG) == -12)
                        continue;
                     if(MGen.ID->at(iG) == 14 || MGen.ID->at(iG) == -14)
                        continue;
                     if(MGen.ID->at(iG) == 16 || MGen.ID->at(iG) == -16)
                        continue;
                     if(MGen.DaughterCount->at(iG) > 0)
                        continue;

                     VisibleParticles.emplace_back(PseudoJet(P[1], P[2], P[3], P[0]));
                  }

                  JetDefinition Definition(antikt_algorithm, JetR);
                  ClusterSequence Sequence(VisibleParticles, Definition);
                  vector<PseudoJet> GenFastJets = Sequence.inclusive_jets(0.5);   // anti-kt, R = 0.4

                  for(int iG = 0; iG < GenFastJets.size(); iG++)
                  {
                     if(GenFastJets[iG].eta() < EtaMin || GenFastJets[iG].eta() > EtaMax)
                        continue;
                     if(GenFastJets[iG].perp() < GenPTMin)
                        continue;

                     PseudoJet &J = GenFastJets[iG];
                     FourVector P(J.e(), J.px(), J.py(), J.pz());
                     GenJets.push_back(pair<FourVector, PseudoJet>(P, J));
                  }
               }
            }
         }

         if(SkipEvent == false)
         {
            if(UseStoredReco == true)
            {
               for(int iR = 0; iR < MJet.JetCount; iR++)
               {
                  if(MJet.JetEta[iR] < EtaMin || MJet.JetEta[iR] > EtaMax)
                     continue;

                  double AverageRho = GetUE(MRho, MJet.JetEta[iR], JetR) / (JetR * JetR * M_PI);

                  JEC.SetJetPT(MJet.JetRawPT[iR]);
                  JEC.SetJetEta(MJet.JetEta[iR]);
                  JEC.SetJetPhi(MJet.JetPhi[iR]);
                  JEC.SetRho(AverageRho);
                  if(JEC.GetCorrection() > 0)
                     MJet.JetPT[iR] = JEC.GetCorrectedPT();

                  if(MJet.JetPT[iR] < PTMin && MJet.JetRawPT[iR] < PTMin)
                     continue;
                  if(IsExcluded(MJet.JetEta[iR], MJet.JetPhi[iR], JetExclusion) == true)
                     continue;

                  if(DoJetID == true)
                  {
                     JetPF JetID;
                     JetID.CHF = MJet.JetPFCHF[iR];
                     JetID.CEF = MJet.JetPFCEF[iR];
                     JetID.NHF = MJet.JetPFNHF[iR];
                     JetID.NEF = MJet.JetPFNEF[iR];
                     JetID.MUF = MJet.JetPFMUF[iR];
                     JetID.CM  = MJet.JetPFCHM[iR] + MJet.JetPFCEM[iR];
                     JetID.M   = JetID.CM + MJet.JetPFNHM[iR] + MJet.JetPFNEM[iR];

                     if(PassJetID(JetID, JetIDMin, JetIDMax) == false)
                        continue;
                  }

                  FourVector P;
                  P.SetPtEtaPhi(MJet.JetPT[iR], MJet.JetEta[iR], MJet.JetPhi[iR]);
                  PseudoJet J(P[1], P[2], P[3], P[0]);
                  RecoJets.push_back(pair<FourVector, PseudoJet>(P, J));
                  RecoJetJECVector.push_back(JEC.GetCorrection());
               }
            }
            else
            {
               // Cluster reco jets from PF candidates.  No subtraction at the moment
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

                  double AverageRho = GetUE(MRho, P.GetEta(), JetR) / (JetR * JetR * M_PI);

                  JEC.SetJetPT(P.GetPT());
                  JEC.SetJetEta(P.GetEta());
                  JEC.SetJetPhi(P.GetPhi());
                  JEC.SetRho(AverageRho);

                  if(P.GetPT() < PTMin && P.GetPT() * JEC.GetCorrection() < PTMin)
                     continue;
                  if(IsExcluded(P.GetEta(), P.GetPhi(), JetExclusion) == true)
                     continue;

                  P = P * JEC.GetCorrection();

                  RecoJets.push_back(pair<FourVector, PseudoJet>(P, J));
                  RecoJetJECVector.push_back(JEC.GetCorrection());
               }
            }
         }

         // Skip reco jets with 0 weight
         if(CutUE == true)
         {
            for(int i = 0; i < (int)RecoJets.size(); i++)
            {
               double Eta = RecoJets[i].first.GetEta();
               double UE = GetUE(MRho, Eta, JetR);
               double Weight = GetRhoWeight(DHFile, DHKeyBase, UE);
               if(Weight == 0)
               {
                  RecoJets.erase(RecoJets.begin() + i);
                  RecoJetJECVector.erase(RecoJetJECVector.begin() + i);
                  i = i - 1;
               }
            }
         }

         // Export gen jet
         NGenJets = GenJets.size();
         ZeroArray(GenJetPT,        MAX);
         ZeroArray(GenJetEta,       MAX);
         ZeroArray(GenJetPhi,       MAX);
         ZeroArray(GenJetMass,      MAX);
         ZeroArray(GenJetPhiWeight, MAX);
         ZeroArray(GenJetWeight,    MAX);
         for(int iG = 0; iG < NGenJets; iG++)
         {
            GenJetPT[iG]        = GenJets[iG].first.GetPT();
            GenJetEta[iG]       = GenJets[iG].first.GetEta();
            GenJetPhi[iG]       = GenJets[iG].first.GetPhi();
            GenJetMass[iG]      = GenJets[iG].first.GetMass();
            
            GenJetPhiWeight[iG] = JetPhiWeight(GenJetEta[iG], JetExclusion);
            GenJetWeight[iG]    = GenJetPhiWeight[iG];
         }

         // Export reco jets
         NRecoJets = RecoJets.size();
         ZeroArray(RecoJetPT,        MAX);
         ZeroArray(RecoJetEta,       MAX);
         ZeroArray(RecoJetPhi,       MAX);
         ZeroArray(RecoJetMass,      MAX);
         ZeroArray(RecoJetJEC,       MAX);
         ZeroArray(RecoJetJEU,       MAX);
         ZeroArray(RecoJetRho,       MAX);
         ZeroArray(RecoJetUE,        MAX);
         ZeroArray(RecoJetPhiWeight, MAX);
         ZeroArray(RecoJetRhoWeight, MAX);
         ZeroArray(RecoJetWeight,    MAX);
         for(int iR = 0; iR < NRecoJets; iR++)
         {
            RecoJetPT[iR]        = RecoJets[iR].first.GetPT();
            RecoJetEta[iR]       = RecoJets[iR].first.GetEta();
            RecoJetPhi[iR]       = RecoJets[iR].first.GetPhi();
            RecoJetMass[iR]      = RecoJets[iR].first.GetMass();
            
            RecoJetJEC[iR]       = RecoJetJECVector[iR];

            JEU.SetJetPT(RecoJetPT[iR]);
            JEU.SetJetEta(RecoJetEta[iR]);
            JEU.SetJetPhi(RecoJetPhi[iR]);
            RecoJetJEU[iR]       = JEU.GetUncertainty().first;

            RecoJetRho[iR]       = GetRhoAtCenter(MRho, RecoJetEta[iR]);
            RecoJetUE[iR]        = GetUE(MRho, RecoJetEta[iR], JetR);

            RecoJetPhiWeight[iR] = JetPhiWeight(RecoJetEta[iR], JetExclusion);
            RecoJetRhoWeight[iR] = GetRhoWeight(DHFile, DHKeyBase, RecoJetUE[iR]);
            RecoJetWeight[iR]    = RecoJetPhiWeight[iR] * RecoJetRhoWeight[iR];
         }
         // For each gen jet, find the best reco jet
         ZeroArray(MatchedJetPT,        MAX);
         ZeroArray(MatchedJetEta,       MAX);
         ZeroArray(MatchedJetPhi,       MAX);
         ZeroArray(MatchedJetMass,      MAX);
         ZeroArray(MatchedJetJEC,       MAX);
         ZeroArray(MatchedJetJEU,       MAX);
         ZeroArray(MatchedJetRho,       MAX);
         ZeroArray(MatchedJetUE,        MAX);
         //ZeroArray(MatchedJetAngle,     MAX);
         ZeroArray(MatchedJetAngle,     MAX, -1); //Added by Uttam for check Feb2026
         ZeroArray(MatchedJetPhiWeight, MAX);
         ZeroArray(MatchedJetRhoWeight, MAX);
         ZeroArray(MatchedJetWeight,    MAX);

         //const double MaxMatchDR = JetR / 2.0;//Added by Uttam for check Feb2026
         vector<bool> RecoJetUsed(NRecoJets, false);//Added by Uttam for check Feb2026

         for(int iG = 0; iG < NGenJets; iG++)
         {
            MatchedJetAngle[iG] = -1;

            int BestIndex = -1;
            double BestAngle = -1;
            for(int iR = 0; iR < NRecoJets; iR++)
            {
               if(RecoJetUsed[iR]) continue;  //Added by Uttam for check Feb2026
               double Angle = GetDR(GenJets[iG].first, RecoJets[iR].first);
               //if(BestAngle < 0 || Angle < BestAngle)
               if(Angle < MaxMatchDR && (BestAngle < 0 || Angle < BestAngle)) //Added by Uttam for check Feb2026
                  {
                  BestIndex = iR;
                  BestAngle = Angle;
                  }
            }

            if(BestIndex < 0)   // not found for some reason
               continue;
            RecoJetUsed[BestIndex] = true; //Added by Uttam for check Feb2026

            MatchedJetPT[iG]        = RecoJets[BestIndex].first.GetPT();
            MatchedJetEta[iG]       = RecoJets[BestIndex].first.GetEta();
            MatchedJetPhi[iG]       = RecoJets[BestIndex].first.GetPhi();
            MatchedJetMass[iG]      = RecoJets[BestIndex].first.GetMass();
            MatchedJetJEC[iG]       = RecoJetJEC[BestIndex];
            MatchedJetJEU[iG]       = RecoJetJEU[BestIndex];
            MatchedJetRho[iG]       = RecoJetRho[BestIndex];
            MatchedJetUE[iG]        = RecoJetUE[BestIndex];
            MatchedJetAngle[iG]     = BestAngle;
            MatchedJetPhiWeight[iG] = RecoJetPhiWeight[BestIndex];
            MatchedJetRhoWeight[iG] = RecoJetRhoWeight[BestIndex];
            MatchedJetWeight[iG]    = RecoJetWeight[BestIndex];
         }

         if(DontStoreReco == true)
         {
            NRecoJets = 0;
            ZeroArray(RecoJetPT,        MAX);
            ZeroArray(RecoJetEta,       MAX);
            ZeroArray(RecoJetPhi,       MAX);
            ZeroArray(RecoJetMass,      MAX);
            ZeroArray(RecoJetJEC,       MAX);
            ZeroArray(RecoJetJEU,       MAX);
            ZeroArray(RecoJetRho,       MAX);
            ZeroArray(RecoJetUE,        MAX);
            ZeroArray(RecoJetPhiWeight, MAX);
            ZeroArray(RecoJetRhoWeight, MAX);
            ZeroArray(RecoJetWeight,    MAX);
         }

         if(SkipEvent == false || KeepSkippedEvent == true)
            OutputTree.Fill();
      }

      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();

      InputFile.Close();
   }

   OutputFile.cd();
   OutputTree.Write();
   OutputFile.Close();

   return 0;
}

bool IsExcluded(double Eta, double Phi, vector<double> &Exclusion)
{
   // true = excluded
   if(Exclusion.size() == 0)
      return false;

   for(int i = 0; i + 4 <= (int)Exclusion.size(); i = i + 4)
   {
      if(Eta > Exclusion[i+0] && Eta < Exclusion[i+1])   // eta in range, check phi
      {
         if(Phi > Exclusion[i+2] && Phi < Exclusion[i+3])   // phi also in range, kill
            return true;
         if(Phi + 2 * M_PI > Exclusion[i+2] && Phi + 2 * M_PI < Exclusion[i+3])   // shift phi for wrapping
            return true;
         if(Phi - 2 * M_PI > Exclusion[i+2] && Phi - 2 * M_PI < Exclusion[i+3])   // shift phi for wrapping
            return true;
      }
   }

   return false;
}

double GetRhoAtCenter(RhoTreeMessenger &M, double Eta)
{
   double Result = 0;

   if(M.EtaMin == nullptr)
      return -1;

   int NBin = M.EtaMin->size();
   if(NBin == 0)
      return -1;

   for(int i = 0; i < NBin; i++)
   {
      if(M.EtaMin->at(i) <= Eta && M.EtaMax->at(i) >= Eta)
      {
         Result = M.Rho->at(i);
         break;
      }
   }

   return Result;
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

   return Result;
}

double JetPhiWeight(double Eta, vector<double> &JetExclusion)
{
   vector<pair<double, double>> PhiRanges;

   for(int i = 0; i + 4 <= (int)JetExclusion.size(); i = i + 4)
   {
      if(Eta < JetExclusion[i] || Eta > JetExclusion[i+1])
         continue;

      double PhiMin = JetExclusion[i+2];
      double PhiMax = JetExclusion[i+3];
      if(PhiMax < PhiMin)   // goes over the +-pi boundary
         PhiMax = PhiMax + 2 * M_PI;

      PhiRanges.push_back(pair<double, double>(PhiMin, PhiMax));
   }

   bool Change = true;
   while(Change == true)
   {
      Change = false;

      int N = PhiRanges.size();
      for(int i = 0; i < N && Change == false; i++)
      {
         for(int j = i + 1; j < N && Change == false; j++)
         {
            double Min1 = PhiRanges[i].first;
            double Max1 = PhiRanges[i].second;
            double Min2 = PhiRanges[j].first;
            double Max2 = PhiRanges[j].second;
            // The criteria for intersection is one of the end points is enclosed by the other range
            // This checks the case where shifting by 2pi is not needed
            if((Min1 > Min2 && Min1 < Max2) || (Max1 > Min2 && Max1 < Max2)
                  || (Min2 > Min1 && Min2 < Max1) || (Max2 > Min1 && Max2 < Max1))
            {
               Change = true;
               PhiRanges[i].first = min(Min1, Min2);
               PhiRanges[i].second = min(Max1, Max2);
               PhiRanges.erase(PhiRanges.begin() + j);
               break;
            }

            // Shift the lower one up by 2 * pi, since the lower bound is always within [-pi, +pi]
            // So this is sufficient and we don't need further cases
            if(Min1 < Min2) { Min1 = Min1 + 2 * M_PI; Max1 = Max1 + 2 * M_PI; }
            else            { Min2 = Min2 + 2 * M_PI; Max2 = Max2 + 2 * M_PI; }

            // Again checking if any endpoints are inside other ranges
            if((Min1 > Min2 && Min1 < Max2) || (Max1 > Min2 && Max1 < Max2)
                  || (Min2 > Min1 && Min2 < Max1) || (Max2 > Min1 && Max2 < Max1))
            {
               Change = true;
               PhiRanges[i].first = min(Min1, Min2);
               PhiRanges[i].second = min(Max1, Max2);
               PhiRanges.erase(PhiRanges.begin() + j);
               break;
            }
         }
      }
   }

   double ExcludedPhi = 0;
   for(pair<double, double> R : PhiRanges)
      ExcludedPhi = ExcludedPhi + (R.second - R.first);
   if(ExcludedPhi > 2 * M_PI)   // all excluded
      return 0;

   return (2 * M_PI) / (2 * M_PI - ExcludedPhi);
}

double GetRhoWeight(DataHelper &DHFile, string &Key, double UE)
{
   static bool First = true;
   static bool NoWeight = false;

   if(First == true)
   {
      if(DHFile["RhoWeight"].Exist(Key+"_Formula") == false)
      {
         First = false;
         NoWeight = true;
      }
   }
   if(NoWeight == true)
      return 1;

   static TF1 Function("Function", DHFile["RhoWeight"][Key+"_Formula"].GetString().c_str(), 0, 1500);

   if(First == true)
   {
      // cout << "Rho weighting formula = " << DHFile["RhoWeight"][Key+"_Formula"].GetString() << endl;

      int N = DHFile["RhoWeight"][Key+"_NPar"].GetInteger();

      First = false;
      for(int i = 0; i < N; i++)
         Function.SetParameter(i, DHFile["RhoWeight"][Key+"_P"+to_string(i)].GetDouble());

      // Function.Print("v");
   }
   
   if(UE > DHFile["RhoWeight"][Key+"_MaxUE"].GetDouble())
      return 0;

   return Function.Eval(UE);
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
   //Centrality table from 2023 Run//
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


   
   static double MCBinEdge[201] = {0, 12.0923, 12.9511, 13.78, 14.612, 15.4534, 16.3322, 17.2009, 18.0867, 19.0249,19.9645,
				   20.9242, 21.9086, 22.9248, 23.9894, 25.1201, 26.2783, 27.4691, 28.6973, 29.9524, 31.2796,
				   32.6688, 34.1073, 35.5829, 37.1416, 38.7405, 40.4475, 42.2128, 44.0999, 46.104, 48.1587,
				   50.2701, 52.4407, 54.7864, 57.1857, 59.7292, 62.416, 65.256, 68.1128, 71.1673, 74.2676,
				   77.5537, 80.9378, 84.4461, 88.0186, 91.7726, 95.7439, 99.7508, 103.977, 108.385,113.01,
				   117.772, 122.662, 127.767, 133.001, 138.452, 144.199, 150.001, 156.233, 162.519, 168.973,
				   175.589, 182.625, 189.917, 197.262, 204.964, 212.904, 221.076, 229.627, 238.513, 247.438,
				   256.741, 266.008, 275.749, 285.951, 296.49, 307.156, 318.197, 329.471, 340.946, 352.995,
				   365.154, 377.63, 390.645, 403.796, 417.3, 431.166, 445.786, 460.379, 475.394, 490.836,
				   506.385, 522.587, 539.274, 556.561, 573.887, 591.713, 609.922, 628.302, 647.414, 666.922,
				   686.271, 706.505, 727.099, 748.436, 769.791, 791.39, 814.272, 836.924, 860.546, 884.249,
				   908.426, 932.993, 958.157, 984.205, 1010.73, 1037.36, 1064.78, 1092.31, 1120.19, 1148.79,
				   1177.14, 1207.38, 1237.37, 1267.84, 1299.7, 1331.57, 1364.06, 1397.12, 1431.16, 1464.49,
				   1498.79, 1534.36, 1571.29, 1607.4, 1643.85, 1681.44, 1719.81, 1759.37, 1799.31, 1839.9,
				   1882.56, 1925.24, 1967.81, 2011.86, 2055.06, 2099.45, 2145.73, 2191.73, 2239.71, 2286.42,
				   2335.47, 2386.02, 2436.77, 2487.91, 2540.2, 2592, 2645.75, 2700.28, 2755.9, 2812.66, 2870.6,
				   2929.42, 2988.99, 3048.85, 3110.39, 3172.71, 3235.12, 3300.11, 3367.31, 3433.1, 3501.38,
				   3571.03, 3644.55, 3716.54, 3790.77, 3867.41, 3945.16, 4024.39, 4103.58, 4184.03, 4266.59, 4352.8,
				   4438.54, 4526.96, 4616.15, 4708.98, 4803.25, 4902.16, 4999.26, 5099.69, 5203.09, 5309.67,
				   5419.98, 5528.29, 5643.13, 5762.85, 5892.3, 6043.3, 6239.48, 7387.56};

   /*
   static double BinEdge[201] = {0,
      10.5072, 11.2099, 11.8364, 12.478, 13.1194, 13.7623, 14.4081, 15.0709, 15.7532, 16.4673,
      17.1881, 17.923, 18.673, 19.4865, 20.3033, 21.1536, 22.0086, 22.9046, 23.8196, 24.7924,
      25.8082, 26.8714, 27.9481, 29.0828, 30.2757, 31.5043, 32.8044, 34.1572, 35.6142, 37.1211,
      38.6798, 40.3116, 42.0398, 43.8572, 45.6977, 47.6312, 49.6899, 51.815, 54.028, 56.3037,
      58.7091, 61.2024, 63.8353, 66.5926, 69.3617, 72.2068, 75.2459, 78.3873, 81.5916, 84.9419,
      88.498, 92.1789, 95.9582, 99.8431, 103.739, 107.78, 111.97, 116.312, 120.806, 125.46,
      130.269, 135.247, 140.389, 145.713, 151.212, 156.871, 162.729, 168.762, 174.998, 181.424,
      188.063, 194.907, 201.942, 209.19, 216.683, 224.37, 232.291, 240.43, 248.807, 257.416,
      266.256, 275.348, 284.668, 294.216, 304.053, 314.142, 324.488, 335.101, 345.974, 357.116,
      368.547, 380.283, 392.29, 404.564, 417.122, 429.968, 443.116, 456.577, 470.357, 484.422,
      498.78, 513.473, 528.479, 543.813, 559.445, 575.411, 591.724, 608.352, 625.344, 642.686,
      660.361, 678.371, 696.749, 715.485, 734.608, 754.068, 773.846, 794.046, 814.649, 835.608,
      856.972, 878.719, 900.887, 923.409, 946.374, 969.674, 993.435, 1017.62, 1042.21, 1067.28,
      1092.72, 1118.64, 1144.96, 1171.71, 1198.98, 1226.67, 1254.82, 1283.46, 1312.65, 1342.21,
      1372.27, 1402.85, 1433.93, 1465.49, 1497.62, 1530.29, 1563.49, 1597.22, 1631.49, 1666.37,
      1701.8, 1737.75, 1774.35, 1811.51, 1849.29, 1887.75, 1926.79, 1966.6, 2006.97, 2047.99,
      2089.71, 2132.1, 2175.23, 2219.17, 2263.72, 2309.2, 2355.43, 2402.47, 2450.33, 2499.05,
      2548.66, 2599.16, 2650.59, 2703.03, 2756.32, 2810.75, 2866.27, 2922.91, 2980.54, 3039.47,
      3099.53, 3160.98, 3223.66, 3287.71, 3353.18, 3420.34, 3489.13, 3559.72, 3632.06, 3706.18,
      3782.42, 3860.78, 3941.42, 4024.52, 4110.27, 4199.4, 4292.8, 4394.49, 4519.52, 5199.95};
   static double BinEdgeDown[201] = {0,
      10.5071, 11.2094, 11.8357, 12.4763, 13.117, 13.7597, 14.4049, 15.0671, 15.7491, 16.4622,
      17.1812, 17.9144, 18.6674, 19.4797, 20.2963, 21.1435, 21.9974, 22.8928, 23.8068, 24.7805,
      25.7931, 26.8556, 27.9308, 29.0638, 30.2582, 31.4795, 32.7816, 34.1349, 35.5834, 37.0941,
      38.6474, 40.2782, 42.0035, 43.8112, 45.6576, 47.5758, 49.6381, 51.6667, 53.7353, 55.8903,
      58.1259, 60.4528, 62.8712, 65.3859, 67.9968, 70.7065, 73.5231, 76.4519, 79.4922, 82.6461,
      85.9264, 89.3269, 92.8562, 96.5212, 100.322, 104.262, 108.344, 112.585, 116.971, 121.521,
      126.225, 131.09, 136.127, 141.328, 146.721, 152.284, 158.014, 163.935, 170.054, 176.372,
      182.878, 189.602, 196.532, 203.653, 211.017, 218.599, 226.387, 234.418, 242.667, 251.16,
      259.886, 268.852, 278.071, 287.498, 297.2, 307.184, 317.409, 327.894, 338.66, 349.686,
      360.996, 372.607, 384.508, 396.669, 409.133, 421.86, 434.906, 448.258, 461.916, 475.906,
      490.16, 504.74, 519.663, 534.911, 550.453, 566.322, 582.525, 599.08, 615.968, 633.211,
      650.805, 668.76, 687.048, 705.707, 724.774, 744.163, 763.9, 783.999, 804.528, 825.432,
      846.746, 868.429, 890.523, 913.007, 935.952, 959.211, 982.919, 1007.08, 1031.63, 1056.62,
      1082.08, 1107.96, 1134.24, 1160.99, 1188.22, 1215.91, 1244.06, 1272.69, 1301.85, 1331.45,
      1361.51, 1392.07, 1423.18, 1454.77, 1486.93, 1519.57, 1552.81, 1586.55, 1620.87, 1655.79,
      1691.26, 1727.27, 1763.93, 1801.12, 1838.97, 1877.47, 1916.61, 1956.45, 1996.89, 2038.04,
      2079.84, 2122.35, 2165.52, 2209.53, 2254.24, 2299.83, 2346.19, 2393.31, 2441.28, 2490.16,
      2539.86, 2590.57, 2642.16, 2694.74, 2748.23, 2802.81, 2858.47, 2915.33, 2973.2, 3032.28,
      3092.56, 3154.24, 3217.19, 3281.45, 3347.18, 3414.6, 3483.65, 3554.56, 3627.2, 3701.66,
      3778.25, 3856.97, 3937.98, 4021.48, 4107.62, 4197.21, 4291.05, 4393.19, 4518.6, 5199.95};
   static double BinEdgeUp[201] = {0,
      10.5075, 11.2107, 11.838, 12.4797, 13.1213, 13.7641, 14.4124, 15.0745, 15.7577, 16.473,
      17.1939, 17.9297, 18.6812, 19.4958, 20.3143, 21.1648, 22.0218, 22.9159, 23.8328, 24.8059,
      25.8204, 26.89, 27.9702, 29.1042, 30.3022, 31.528, 32.8347, 34.1896, 35.6439, 37.1542,
      38.7172, 40.3518, 42.091, 43.9053, 45.7415, 47.6853, 49.7457, 51.8755, 54.0983, 56.3594,
      58.7848, 61.2861, 63.9228, 66.6825, 69.4421, 72.297, 75.3547, 78.4967, 81.6977, 85.0755,
      88.6211, 92.3058, 96.1071, 99.9975, 104.065, 108.272, 112.512, 116.906, 121.601, 126.465,
      131.482, 136.866, 142.229, 147.786, 153.546, 159.571, 165.586, 171.902, 178.419, 185.063,
      191.856, 199.055, 206.261, 213.999, 221.719, 229.671, 237.84, 246.088, 254.828, 263.883,
      272.907, 282.236, 291.925, 301.519, 311.477, 321.691, 332.153, 342.892, 353.878, 365.161,
      376.742, 388.577, 400.684, 413.075, 425.746, 438.711, 451.989, 465.556, 479.45, 493.608,
      508.077, 522.891, 538.003, 553.415, 569.151, 585.216, 601.601, 618.354, 635.422, 652.84,
      670.599, 688.699, 707.161, 726.014, 745.185, 764.687, 784.557, 804.838, 825.489, 846.537,
      867.951, 889.752, 911.955, 934.588, 957.52, 980.912, 1004.73, 1028.94, 1053.57, 1078.67, 
      1104.17, 1130.07, 1156.39, 1183.2, 1210.47, 1238.17, 1266.38, 1295.02, 1324.16, 1353.71,
      1383.77, 1414.35, 1445.41, 1477, 1509.09, 1541.74, 1574.88, 1608.59, 1642.83, 1677.66,
      1713.07, 1748.98, 1785.47, 1822.63, 1860.33, 1898.72, 1937.73, 1977.42, 2017.71, 2058.62,
      2100.25, 2142.57, 2185.56, 2229.38, 2273.91, 2319.2, 2365.33, 2412.22, 2459.94, 2508.52,
      2557.98, 2608.35, 2659.61, 2711.86, 2765, 2819.23, 2874.58, 2930.97, 2988.46, 3047.12,
      3106.95, 3168.15, 3230.6, 3294.37, 3359.58, 3426.47, 3494.95, 3565.21, 3637.21, 3711.03,
      3786.91, 3864.85, 3945.11, 4027.8, 4113.06, 4201.73, 4294.72, 4395.9, 4520.5, 5199.95};
   static double MCBinEdge[201] = {0.000000000e+00,
      1.221866608e+01,1.303708744e+01,1.376741886e+01,1.451291370e+01,1.526025295e+01,
      1.600864601e+01,1.676231384e+01,1.753350639e+01,1.832830048e+01,1.915963364e+01,
      1.999889565e+01,2.085316849e+01,2.172974205e+01,2.267727470e+01,2.363133430e+01,
      2.462081909e+01,2.561552811e+01,2.665853500e+01,2.772228050e+01,2.886320305e+01,
      3.004099464e+01,3.128650284e+01,3.254307938e+01,3.386550522e+01,3.525390244e+01,
      3.669124985e+01,3.820637894e+01,3.978758240e+01,4.148179626e+01,4.324157333e+01,
      4.506051636e+01,4.696524811e+01,4.899179459e+01,5.109997177e+01,5.324172974e+01,
      5.550939560e+01,5.792085266e+01,6.038174057e+01,6.297782898e+01,6.560985565e+01,
      6.843524933e+01,7.135434723e+01,7.441539764e+01,7.762516785e+01,8.084247589e+01,
      8.416105652e+01,8.773950195e+01,9.139731598e+01,9.512862396e+01,9.905710602e+01,
      1.031847458e+02,1.074815903e+02,1.119287720e+02,1.164504089e+02,1.211777344e+02,
      1.260805893e+02,1.309948730e+02,1.361708069e+02,1.416123962e+02,1.472980804e+02,
      1.531386566e+02,1.594185638e+02,1.656330414e+02,1.721137543e+02,1.788811798e+02,
      1.858440247e+02,1.928451691e+02,2.002444916e+02,2.078301239e+02,2.155285797e+02,
      2.234885254e+02,2.318784180e+02,2.402543335e+02,2.493193817e+02,2.583033752e+02,
      2.675081482e+02,2.770369873e+02,2.867287598e+02,2.968448181e+02,3.074582214e+02,
      3.178819885e+02,3.287866211e+02,3.400741882e+02,3.512948608e+02,3.629790039e+02,
      3.751250610e+02,3.871965332e+02,3.996041565e+02,4.125161438e+02,4.256829224e+02,
      4.390009766e+02,4.526673584e+02,4.668159485e+02,4.810065918e+02,4.956786194e+02,
      5.105879822e+02,5.261378784e+02,5.417820435e+02,5.576412964e+02,5.741410522e+02,
      5.910710449e+02,6.083785400e+02,6.260684814e+02,6.436163330e+02,6.618848267e+02,
      6.802882690e+02,6.994486084e+02,7.189249878e+02,7.389677734e+02,7.589831543e+02,
      7.794594727e+02,8.003760986e+02,8.216380005e+02,8.435552368e+02,8.657709351e+02,
      8.883392334e+02,9.110311279e+02,9.349785156e+02,9.585595093e+02,9.825820312e+02,
      1.007016602e+03,1.031899536e+03,1.057812256e+03,1.084011597e+03,1.111714233e+03,
      1.138206909e+03,1.165716187e+03,1.193733154e+03,1.221653320e+03,1.251507690e+03,
      1.281227417e+03,1.311011963e+03,1.341104126e+03,1.372396362e+03,1.404285767e+03,
      1.436516602e+03,1.468652344e+03,1.501910034e+03,1.535562012e+03,1.569694702e+03,
      1.604690063e+03,1.640646484e+03,1.676045776e+03,1.712617920e+03,1.749282837e+03,
      1.787432129e+03,1.825893921e+03,1.866071289e+03,1.906581055e+03,1.947844116e+03,
      1.989663940e+03,2.031399536e+03,2.072800781e+03,2.115318115e+03,2.159495117e+03,
      2.205225830e+03,2.252677734e+03,2.298583740e+03,2.345652588e+03,2.393358154e+03,
      2.442872314e+03,2.491453369e+03,2.541042969e+03,2.592810303e+03,2.645517822e+03,
      2.699104248e+03,2.753293457e+03,2.807929688e+03,2.864374023e+03,2.922601807e+03,
      2.979420898e+03,3.038675049e+03,3.098716309e+03,3.159294434e+03,3.221664062e+03,
      3.285901611e+03,3.350948486e+03,3.415805664e+03,3.482691406e+03,3.552617432e+03,
      3.623614502e+03,3.694631104e+03,3.767252930e+03,3.840279297e+03,3.917036621e+03,
      3.993660400e+03,4.073357666e+03,4.154333008e+03,4.238127930e+03,4.322208496e+03,
      4.409825195e+03,4.498888672e+03,4.589717773e+03,4.681564941e+03,4.777086914e+03,
      4.877952637e+03,4.987053711e+03,5.113039062e+03,5.279584473e+03,9.999999999e+03};
   */
   

   for(int i = 0; i < BinCount; i++)
   {
      if(Variation == 100 && hiHF >= MCBinEdge[i] && hiHF < MCBinEdge[i+1])
         return (double)(BinCount - 1 - i) / BinCount;
      if(Variation == 0 && hiHF >= BinEdge[i] && hiHF < BinEdge[i+1])
         return (double)(BinCount - 1 - i) / BinCount;
      if(Variation == 1 && hiHF >= BinEdgeUp[i] && hiHF < BinEdgeUp[i+1])
         return (double)(BinCount - 1 - i) / BinCount;
      if(Variation == -1 && hiHF >= BinEdgeDown[i] && hiHF < BinEdgeDown[i+1])
         return (double)(BinCount - 1 - i) / BinCount;
   }

   return -1;
}

void ZeroArray(float *X, int N)
{
   //return;
   for(int i = 0; i < N; i++)
      X[i] = 0;
}
void ZeroArray(float *X, int N, float InitialValue)
{
   for(int i = 0; i < N; i++)
      X[i] = InitialValue;
}
