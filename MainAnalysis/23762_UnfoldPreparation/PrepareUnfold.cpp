#include <vector>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"

#include "CustomAssert.h"
#include "CommandLine.h"
#include "ProgressBar.h"

#include "BinHelper.h"
#include "JetCorrector.h"

#define MAX 10240

enum ObservableType {ObservableNone, ObservableJetPT};
enum ObservableStep {Gen, Reco, Matched};

class Messenger;
int main(int argc, char *argv[]);
void FillMinMax(TH1D &HMin1, TH1D &HMax1, TH1D &HMin2, TH1D &HMax2, vector<double> &Bin1, vector<double> &Bin2);
vector<double> GetFineBins(double Min, double Max = 1500);

class Messenger
{
 private:
  TTree *Tree;
  int                    Run;
  int                    Lumi;
  long long              Event;
  double                 PTHat;
  double                 EventWeight;
  int                    NRecoJets;
  float                  RecoJetPT[MAX];
  float                  RecoJetEta[MAX];
  float                  RecoJetPhi[MAX];
  float                  RecoJetMass[MAX];
  float                  RecoJetJEC[MAX];
  float                  RecoJetJEU[MAX];
  float                  RecoJetWeight[MAX];
  int                    NGenJets;
  float                  GenJetPT[MAX];
  float                  GenJetEta[MAX];
  float                  GenJetPhi[MAX];
  float                  GenJetMass[MAX];
  float                  GenJetWeight[MAX];
  float                  MatchedJetPT[MAX];
  float                  MatchedJetEta[MAX];
  float                  MatchedJetPhi[MAX];
  float                  MatchedJetMass[MAX];
  float                  MatchedJetJEC[MAX];
  float                  MatchedJetJEU[MAX];
  float                  MatchedJetAngle[MAX];
  float                  MatchedJetWeight[MAX];
  double                 MaxMatchedJetAngle;
  bool                   UseJEU;
  double                 JEUOffset;
  bool                   UseJERSF;
  SingleJetCorrector     JERSF;
  double                 EtaMin, EtaMax;
 public:
  Messenger()              { Initialize(nullptr); }
  Messenger(TTree *Tree)   { Initialize(Tree); }
  Messenger(TFile &File, string TreeName = "UnfoldingTreeFlat")
    {
      TTree *Tree = (TTree *)File.Get(TreeName.c_str());
      Initialize(Tree);
    }
  ~Messenger() {}
  void Initialize(TTree *InputTree = nullptr)
  {
    EventWeight = 1;
    Run = 1;
    Lumi = 1;
    Event = 1;
    NRecoJets = 0;
    NGenJets = 0;
    MaxMatchedJetAngle = -1;
    UseJEU = true;
    JEUOffset = 0;
    UseJERSF = false;
    EtaMin = -1000;
    EtaMax = 1000;

    Tree = InputTree;
    if(Tree == nullptr)
      return;

    Tree->SetBranchAddress("PTHat", &PTHat);
    Tree->SetBranchAddress("EventWeight", &EventWeight);
    Tree->SetBranchAddress("Run", &Run);
    Tree->SetBranchAddress("Event", &Event);
    Tree->SetBranchAddress("Lumi", &Lumi);
    Tree->SetBranchAddress("NRecoJets", &NRecoJets);
    Tree->SetBranchAddress("RecoJetPT", &RecoJetPT);
    Tree->SetBranchAddress("RecoJetEta", &RecoJetEta);
    Tree->SetBranchAddress("RecoJetPhi", &RecoJetPhi);
    Tree->SetBranchAddress("RecoJetMass", &RecoJetMass);
    Tree->SetBranchAddress("RecoJetJEC", &RecoJetJEC);
    Tree->SetBranchAddress("RecoJetJEU", &RecoJetJEU);
    Tree->SetBranchAddress("RecoJetWeight", &RecoJetWeight);
    Tree->SetBranchAddress("NGenJets", &NGenJets);
    Tree->SetBranchAddress("GenJetPT", &GenJetPT);
    Tree->SetBranchAddress("GenJetEta", &GenJetEta);
    Tree->SetBranchAddress("GenJetPhi", &GenJetPhi);
    Tree->SetBranchAddress("GenJetMass", &GenJetMass);
    Tree->SetBranchAddress("GenJetWeight", &GenJetWeight);
    Tree->SetBranchAddress("MatchedJetPT", &MatchedJetPT);
    Tree->SetBranchAddress("MatchedJetEta", &MatchedJetEta);
    Tree->SetBranchAddress("MatchedJetPhi", &MatchedJetPhi);
    Tree->SetBranchAddress("MatchedJetMass", &MatchedJetMass);
    Tree->SetBranchAddress("MatchedJetJEC", &MatchedJetJEC);
    Tree->SetBranchAddress("MatchedJetJEU", &MatchedJetJEU);
    Tree->SetBranchAddress("MatchedJetAngle", &MatchedJetAngle);
    Tree->SetBranchAddress("MatchedJetWeight", &MatchedJetWeight);
  }
  void SetUseJEU(bool Value = true)
  {
    UseJEU = Value;
  }
  void SetJEUOffset(double Value = 0)
  {
    JEUOffset = Value;
  }
  void InitializeJERSF(string FileName)
  {
    UseJERSF = true;
    JERSF.Initialize(FileName);
  }
  double GetPTHat()
  {
    return PTHat;
  }

  void SetMaxMatchedJetAngle(double Value = -1) { MaxMatchedJetAngle = Value; }
  void SetEtaMin(double Value = -1000)          { EtaMin = Value; }
  void SetEtaMax(double Value = 1000)           { EtaMax = Value; }
  void GetEntry(int Entry)
  {
    if(Tree == nullptr)
      return;

    Tree->GetEntry(Entry);

    if(EventWeight < 0)
      EventWeight = 1;

    if(MaxMatchedJetAngle > 0)
      {
	for(int iJ = 0; iJ < NGenJets; iJ++)
	  {
      if(MatchedJetAngle[iJ] < 0 || MatchedJetAngle[iJ] > MaxMatchedJetAngle)
	    {
		      MatchedJetPT[iJ] = 0;
		      MatchedJetMass[iJ] = 0;
	      }
	  }
      }

    for(int iJ = 0; iJ < NGenJets; iJ++)
      {
	if(GenJetEta[iJ] < EtaMin)       GenJetPT[iJ] = 0;
	if(GenJetEta[iJ] > EtaMax)       GenJetPT[iJ] = 0;
	if(MatchedJetEta[iJ] < EtaMin)   MatchedJetPT[iJ] = 0;
	if(MatchedJetEta[iJ] > EtaMax)   MatchedJetPT[iJ] = 0;
      }
    for(int iJ = 0; iJ < NRecoJets; iJ++)
      {
	if(RecoJetEta[iJ] < EtaMin)       RecoJetPT[iJ] = 0;
	if(RecoJetEta[iJ] > EtaMax)       RecoJetPT[iJ] = 0;
	//if(RecoJetPT[iJ]>(2.5*PTHat)) continue;
	
      }
  }
  int GetEntries()           { if(Tree != nullptr) return Tree->GetEntries(); return 0;}
  int GetRun()               { return Run; }
  int GetLumi()              { return Lumi; }
  long long GetEvent()       { return Event; }
  double GetItemCount(ObservableStep Step, ObservableType &Type)
  {
    if(Type == ObservableJetPT && Step == Gen)
      return NGenJets;
    if(Type == ObservableJetPT && Step == Reco)
      return NRecoJets;
    if(Type == ObservableJetPT && Step == Matched)
      return NGenJets;
      
    return 0;
  }
  double GetValue(ObservableStep Step, ObservableType &Type, int Index, int Item,
		  double Shift = 0, double Smear = 0, double ExtraScale = 1)
  {
    double Value = GetValueNoScale(Step, Type, Index, Item, Shift, Smear);
    if(Value > 0)
      Value = Value * ExtraScale;
    return Value;
  }
  double GetValueNoScale(ObservableStep Step, ObservableType &Type, int Index, int Item,
			 double Shift = 0, double Smear = 0)
  {
    if(Type == ObservableJetPT && Step == Gen && Item < NGenJets)
      {
	      return GenJetPT[Item];
      }
    if(Type == ObservableJetPT && Step == Reco && Item < NRecoJets)
      {
	      double Value = RecoJetPT[Item];
        if(UseJEU == true)
          {
            double JEU = RecoJetJEU[Item];
            if(JEUOffset != 0)
            {
              int Sign = (JEUOffset > 0) ? 1 : -1;
              JEU = (JEU * JEU + JEUOffset * JEUOffset * Sign);
              JEU = (JEU > 0) ? JEU : 0;
              JEU = sqrt(JEU);
            }
            Value = Value * (1 + Shift * JEU / RecoJetJEC[Item]);
          }
        else
          Value = Value * (1 + Shift);
        return Value;
      }
    if(Type == ObservableJetPT && Step == Matched && Item < NGenJets)
      {
        double Value = MatchedJetPT[Item];
        if(UseJEU == true)
          {
            double JEU = MatchedJetJEU[Item];
            if(JEUOffset != 0)
            {
                int Sign = (JEUOffset > 0) ? 1 : -1;
                JEU = (JEU * JEU + JEUOffset * JEUOffset * Sign);
                JEU = (JEU > 0) ? JEU : 0;
                JEU = sqrt(JEU);
            }
            Value = Value * (1 + Shift * JEU / MatchedJetJEC[Item]);
          }
        else
          Value = Value * (1 + Shift);
          double SmearAmount = GetSmear(Value, MatchedJetEta[Item], Smear);
          if(Value > 0)   // if Value <= 0 it means bad matching and so we shouldn't unsmear it
          Value = (Value - GenJetPT[Item]) * (SmearAmount + 1) + GenJetPT[Item];
        return Value;
      }

    return -1;
  }
  double GetSmear(double PT, double Eta, double Smear)
  {
    if(UseJERSF == false)
      return Smear;
    if(Eta < -2.0 || Eta > 2.0)
      return 0;   
    JERSF.SetJetPT(PT);
    JERSF.SetJetEta(Eta);
    JERSF.SetJetPhi(0);
    JERSF.SetRho(0);
    JERSF.SetJetArea(0.5);

    vector<double> Parameters = JERSF.GetParameters();
    //cout << "Parameters: ";
    //for(auto v : Parameters)
    //  cout << v << " ";
    // cout << endl;
    
    if(Parameters.size() < 3)   // Error!
      {
	  
	      cerr << "Error! JER SF parameter problem for PT = " << PT << ", eta = " << Eta << endl;
	      return Smear;
      }
    //cout<<Parameters[0]<<" "<<Parameters[0] - 1<<endl;
    double Mean = Parameters[0] - 1;
    double Low = min(Parameters[1], Parameters[2]) - 1;
    double High = max(Parameters[1], Parameters[2]) - 1;

    if(Smear == 0)   return Mean;
    if(Smear < 0)    return Mean + Smear * (Mean - Low);   // Note Smear < 0, Mean - Low > 0, so it works out
    if(Smear > 0)    return Mean + Smear * (High - Mean);

    // it should never reach here!
    return 0;
  }
  int GetSimpleBin(ObservableStep Step, ObservableType &Type, int Index, int Item, vector<double> &Bins,
		   double Shift = 0, double Smear = 0, double ExtraScale = 1, double Min = -99999, double Max = 99999)
  {
    if(Bins.size() == 0) return 0;

    double Value = GetValue(Step, Type, Index, Item, Shift, Smear, ExtraScale);
    if(Value < Min || Value > Max) return -1;
    for(int i = 0; i < (int)Bins.size(); i++)
    {
      if(Value < Bins[i])
	    return i;
    } 
    return Bins.size(); //possible flaw in here,
    //return 1;
  }
  int GetCompositeBin(ObservableStep Step,
		      ObservableType &Type1, int Index1, int Item1, vector<double> &Bins1, double Shift1, double Smear1,
		      double ExtraScale1, double Min1, double Max1,
		      ObservableType &Type2, int Index2, int Item2, vector<double> &Bins2, double Shift2, double Smear2,
		      double ExtraScale2, double Min2, double Max2)
  {
    // #1 is the primary binning, and #2 is the additional binning

    int SimpleBin1 = GetSimpleBin(Step, Type1, Index1, Item1, Bins1, Shift1, Smear1, ExtraScale1, Min1, Max1);
    int SimpleBin2 = GetSimpleBin(Step, Type2, Index2, Item2, Bins2, Shift2, Smear2, ExtraScale2, Min2, Max2);

    if(Type2 == ObservableNone)
      return SimpleBin1;

    return SimpleBin2 * (Bins1.size() + 1) + SimpleBin1;
  }
  double GetMatchedAngle(ObservableType Type, int Item)
  {
    if(Item < 0)           return 99999;
    if(Item >= NGenJets)   return 99999;
    return MatchedJetAngle[Item];
  }
  double GetEventWeight()
  {
    return EventWeight;
  }
  
  double GetJetWeight(ObservableStep Step, int Index)
  {
    if(Step == Gen)
      return (GenJetWeight != nullptr && NGenJets > Index) ? GenJetWeight[Index] : 1;
    if(Step == Reco)
      return (RecoJetWeight != nullptr && NRecoJets > Index) ? RecoJetWeight[Index] : 1;
    if(Step == Matched)
      return (MatchedJetWeight != nullptr && NGenJets > Index) ? MatchedJetWeight[Index] : 1;
    return 1;
  }
};

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string MCFileName     = CL.Get("MC");
  string DataFileName   = CL.Get("Data");
  string OutputFileName = CL.Get("Output");
  bool ExportJSON       = CL.GetBool("ExportJSON", true);
  string JSONFileName   = CL.Get("JSONOutput", "JSON.txt");
  double MCFraction     = CL.GetDouble("MCFraction", 1.00);
  double DataFraction   = CL.GetDouble("DataFraction", 1.00);

  // vector<double> Default{0.0, 1.0};
  string Default = "0.0,1.0";

  string Primary                 = CL.Get("Observable", "JetP");
  int PrimaryIndex               = CL.GetInt("ObservableIndex", -1);
  string PrimaryGenBinString     = CL.Get("ObservableGenBins", Default);
  string PrimaryRecoBinString    = CL.Get("ObservableRecoBins", Default);
  double PrimaryShift            = CL.GetDouble("ObservableShift", 0);
  double PrimarySmear            = CL.GetDouble("ObservableSmear", 0);
  string Binning                 = CL.Get("Binning", "None");
  int BinningIndex               = CL.GetInt("BinningIndex", -1);
  string BinningGenBinString     = CL.Get("BinningGenBins", Default);
  string BinningRecoBinString    = CL.Get("BinningRecoBins", Default);
  double BinningShift            = CL.GetDouble("BinningShift", 0);
  double BinningSmear            = CL.GetDouble("BinningSmear", 0);
  bool CheckMatchAngle           = CL.GetBool("CheckMatchAngle", true);
  double MaxMatchAngle           = CL.GetDouble("MaxMatchAngle", 0.2);

  double EtaMin                  = CL.GetDouble("EtaMin", -2.60);
  double EtaMax                  = CL.GetDouble("EtaMax", +2.60);

  double PrimaryGenMin           = CL.GetDouble("ObservableGenMin", -99999);
  double PrimaryGenMax           = CL.GetDouble("ObservableGenMax", +99999);
  double PrimaryRecoMin          = CL.GetDouble("ObservableRecoMin", -99999);
  double PrimaryRecoMax          = CL.GetDouble("ObservableRecoMax", +99999);
  double BinningGenMin           = CL.GetDouble("BinningGenMin", -99999);
  double BinningGenMax           = CL.GetDouble("BinningGenMax", +99999);
  double BinningRecoMin          = CL.GetDouble("BinningRecoMin", -99999);
  double BinningRecoMax          = CL.GetDouble("BinningRecoMax", +99999);

  vector<double> PrimaryGenBins, PrimaryRecoBins, BinningGenBins, BinningRecoBins;
  if(PrimaryGenBinString == "fine")   PrimaryGenBins = GetFineBins(PrimaryGenMin);
  else                                PrimaryGenBins  = ParseList(PrimaryGenBinString);
  if(PrimaryRecoBinString == "fine")  PrimaryRecoBins = GetFineBins(PrimaryRecoMin);
  else                                PrimaryRecoBins  = ParseList(PrimaryRecoBinString);
  if(BinningGenBinString == "fine")   BinningGenBins = GetFineBins(BinningGenMin);
  else                                BinningGenBins  = ParseList(BinningGenBinString);
  if(BinningRecoBinString == "fine")  BinningRecoBins = GetFineBins(BinningRecoMin);
  else                                BinningRecoBins  = ParseList(BinningRecoBinString);

  bool UseJEU                    = CL.GetBool("UseJEU", true);
  double JEUOffset               = CL.GetDouble("JEUOffset", 0);
  bool UseJERSFFile              = CL.GetBool("UseJERSFFile", false);
   
  bool DoFlooring                = CL.GetBool("Flooring", false);

  sort(PrimaryGenBins.begin(), PrimaryGenBins.end());
  sort(PrimaryRecoBins.begin(), PrimaryRecoBins.end());
  sort(BinningGenBins.begin(), BinningGenBins.end());
  sort(BinningRecoBins.begin(), BinningRecoBins.end());

  ObservableType PrimaryType = ObservableNone;
  if(Primary == "JetPT")              PrimaryType = ObservableJetPT;

  ObservableType BinningType = ObservableNone;
  if(Binning == "JetPT")              BinningType = ObservableJetPT;

  if(BinningType == ObservableNone)
    {
      BinningGenBins.clear();
      BinningRecoBins.clear();
    }

  Assert(PrimaryType != ObservableNone, "Primary observable type is none");

  TFile FMC(MCFileName.c_str());
  TFile FData(DataFileName.c_str());
  TFile FOutput(OutputFileName.c_str(), "RECREATE");

  vector<string> ToCopy{"EventCount", "BaselineEventCount", "AllEventCount"};
  for(string S : ToCopy)
    {
      if(FData.Get(S.c_str()) != nullptr)
	FData.Get(S.c_str())->Clone(("Data" + S).c_str())->Write();
      if(FMC.Get(S.c_str()) != nullptr)
	FMC.Get(S.c_str())->Clone(("MC" + S).c_str())->Write();
    }

  int GenBinCount = PrimaryGenBins.size() + 1;
  if(BinningType != ObservableNone)
    GenBinCount = GenBinCount * (BinningGenBins.size() + 1);
  int RecoBinCount = PrimaryRecoBins.size() + 1;
  if(BinningType != ObservableNone)
    RecoBinCount = RecoBinCount * (BinningRecoBins.size() + 1);
  int MatchedBinCount = PrimaryRecoBins.size() + 1;
  if(BinningType != ObservableNone)
    MatchedBinCount = MatchedBinCount * (BinningRecoBins.size() + 1);

  TH1D HMCGen("HMCGen", ";Gen", GenBinCount, 0, GenBinCount);
  TH1D HMCMatched("HMCMatched", ";Matched", MatchedBinCount, 0, MatchedBinCount);
  TH1D HMCMatchedGenBin("HMCMatchedGenBin", ";Matched (Gen)", GenBinCount, 0, GenBinCount);
  TH1D HMCReco("HMCReco", ";Reco", RecoBinCount, 0, RecoBinCount);
  TH1D HMCRecoGenBin("HMCRecoGenBin", ";Reco (Gen)", GenBinCount, 0, GenBinCount);
  TH2D HResponseNoWeight("HResponseNoWeight", ";Matched;Gen", MatchedBinCount, 0, MatchedBinCount, GenBinCount, 0, GenBinCount);
  TH2D HResponse("HResponse", ";Matched;Gen", MatchedBinCount, 0, MatchedBinCount, GenBinCount, 0, GenBinCount);
  TH1D HDataReco("HDataReco", ";Reco", RecoBinCount, 0, RecoBinCount);
  TH1D HDataRecoGenBin("HDataRecoGenBin", ";Gen", GenBinCount, 0, GenBinCount);

  TH1D HGenPrimaryBinMin("HGenPrimaryBinMin", ";Gen;Min", GenBinCount, 0, GenBinCount);
  TH1D HGenPrimaryBinMax("HGenPrimaryBinMax", ";Gen;Max", GenBinCount, 0, GenBinCount);
  TH1D HGenBinningBinMin("HGenBinningBinMin", ";Gen;Min", GenBinCount, 0, GenBinCount);
  TH1D HGenBinningBinMax("HGenBinningBinMax", ";Gen;Max", GenBinCount, 0, GenBinCount);
  TH1D HRecoPrimaryBinMin("HRecoPrimaryBinMin", ";Reco;Min", RecoBinCount, 0, RecoBinCount);
  TH1D HRecoPrimaryBinMax("HRecoPrimaryBinMax", ";Reco;Max", RecoBinCount, 0, RecoBinCount);
  TH1D HRecoBinningBinMin("HRecoBinningBinMin", ";Reco;Min", RecoBinCount, 0, RecoBinCount);
  TH1D HRecoBinningBinMax("HRecoBinningBinMax", ";Reco;Max", RecoBinCount, 0, RecoBinCount);
  TH1D HMatchedPrimaryBinMin("HMatchedPrimaryBinMin", ";Matched;Min", MatchedBinCount, 0, MatchedBinCount);
  TH1D HMatchedPrimaryBinMax("HMatchedPrimaryBinMax", ";Matched;Max", MatchedBinCount, 0, MatchedBinCount);
  TH1D HMatchedBinningBinMin("HMatchedBinningBinMin", ";Matched;Min", MatchedBinCount, 0, MatchedBinCount);
  TH1D HMatchedBinningBinMax("HMatchedBinningBinMax", ";Matched;Max", MatchedBinCount, 0, MatchedBinCount);

  FillMinMax(HGenPrimaryBinMin, HGenPrimaryBinMax, HGenBinningBinMin,
	     HGenBinningBinMax, PrimaryGenBins, BinningGenBins);
  FillMinMax(HRecoPrimaryBinMin, HRecoPrimaryBinMax, HRecoBinningBinMin,
	     HRecoBinningBinMax, PrimaryRecoBins, BinningRecoBins);
  FillMinMax(HMatchedPrimaryBinMin, HMatchedPrimaryBinMax, HMatchedBinningBinMin,
	     HMatchedBinningBinMax, PrimaryRecoBins, BinningRecoBins);

  Messenger MMC(FMC);
  if(CheckMatchAngle == true)
    MMC.SetMaxMatchedJetAngle(MaxMatchAngle);
  MMC.SetUseJEU(UseJEU);
  MMC.SetJEUOffset(JEUOffset);
  if(UseJERSFFile == true)
    MMC.InitializeJERSF(CL.Get("JERSF"));

  MMC.SetEtaMin(EtaMin);
  MMC.SetEtaMax(EtaMax);

  int EntryCount = MMC.GetEntries() * MCFraction;
  ProgressBar BarMC(cout, EntryCount);
  BarMC.SetStyle(2);

  for(int iE = 0; iE < EntryCount; iE++)
    {
      MMC.GetEntry(iE);

      BarMC.Update(iE);
      if(EntryCount < 300 || (iE % (EntryCount / 200)) == 0)
	  BarMC.Print();

      int NJet = MMC.GetItemCount(Gen, PrimaryType);
      for(int iJ = 0; iJ < NJet; iJ++)
	      {
          double jtPt=MMC.GetValue(Gen,PrimaryType,PrimaryIndex,iJ,0,0, 1);
          //cout<<"Gen: "<<jtPt<<'\t'<<2.5*MMC.GetPTHat()<<endl;
          if(jtPt>2.5*MMC.GetPTHat()) continue; //Should this be applied to Gen as well?
              

	        int GenBin = MMC.GetCompositeBin(Gen,
					   PrimaryType, PrimaryIndex, iJ, PrimaryGenBins, 0, 1, 1, PrimaryGenMin, PrimaryGenMax,
					   BinningType, BinningIndex, iJ, BinningGenBins, 0, 1, 1, BinningGenMin, BinningGenMax);
	        HMCGen.Fill(GenBin, MMC.GetEventWeight() * MMC.GetJetWeight(Gen, iJ));// IS the weight used here in this loop exactly matches eith eht base weight in Matched loop.
	      }

      ObservableType PrimaryMatrixType = PrimaryType;
      ObservableType BinningMatrixType = BinningType;

      NJet = MMC.GetItemCount(Gen, PrimaryMatrixType); //Iterating over the NGenJets  not the number of matched jets.but filling histogram using MatchedJetweight[ij]. If NGenJets is used as loop limits then the checking of the validmatch actually exist for that index is not varified, chances of filling histogram with garbage.
      for(int iJ = 0; iJ < NJet; iJ++)
	      {
        //double jtPt=MMC.GetValue(Matched,PrimaryType,PrimaryIndex,iJ,PrimaryShift, PrimarySmear, 1);
        double jtPt=MMC.GetValue(Matched,PrimaryType,PrimaryIndex,iJ,0,0, 1);
        //cout<<"Gen: "<<jtPt<<'\t'<<2.5*MMC.GetPTHat()<<endl;
        if(jtPt>2.5*MMC.GetPTHat()) continue; //Should this be applied to Gen as well?
            

        double Angle = MMC.GetMatchedAngle(PrimaryMatrixType, iJ);
        if(CheckMatchAngle == true && (Angle > MaxMatchAngle || Angle < 0)) //
                continue;

        int GenBin = MMC.GetCompositeBin(Gen,
                PrimaryMatrixType, PrimaryIndex, iJ, PrimaryGenBins, 0, 1, 1, PrimaryGenMin, PrimaryGenMax,
                BinningMatrixType, BinningIndex, iJ, BinningGenBins, 0, 1, 1, BinningGenMin, BinningGenMax);
        int MatchedBin = MMC.GetCompositeBin(Matched,
                    PrimaryMatrixType, PrimaryIndex, iJ, PrimaryRecoBins, PrimaryShift, PrimarySmear, 1,
                    PrimaryRecoMin, PrimaryRecoMax,
                    BinningMatrixType, BinningIndex, iJ, BinningRecoBins, BinningShift, BinningSmear, 1,
                    BinningRecoMin, BinningRecoMax);
            
        int MatchedGenBin = MMC.GetCompositeBin(Matched,
                  PrimaryMatrixType, PrimaryIndex, iJ, PrimaryGenBins, PrimaryShift, PrimarySmear, 1,
                  PrimaryGenMin, PrimaryGenMax,
                  BinningMatrixType, BinningIndex, iJ, BinningGenBins, BinningShift, BinningSmear, 1,
                  BinningGenMin, BinningGenMax);

        HMCMatched.Fill(MatchedBin, MMC.GetEventWeight() * MMC.GetJetWeight(Matched, iJ));// Does the Eventweight includes the jetlevel weights if so then it is squared???
        HResponseNoWeight.Fill(MatchedBin, GenBin, MMC.GetEventWeight());//Fill2D HISTOGRAM for response matrix no Jetweight
        //HResponse.Fill(MatchedBin, GenBin, MMC.GetJetWeight(Matched, iJ));
        HResponse.Fill(MatchedBin, GenBin, MMC.GetEventWeight()*MMC.GetJetWeight(Matched, iJ));//Used event weight for consistency. Added by Uttam.
        HMCMatchedGenBin.Fill(MatchedGenBin, MMC.GetEventWeight() * MMC.GetJetWeight(Matched, iJ));
        }

      //     NJet = MMC.GetItemCount(Reco, PrimaryType);
      //     for(int iJ = 0; iJ < NJet; iJ++)
      // {
      //   double jtPt=MMC.GetValue(Reco,PrimaryType,PrimaryIndex,iJ,0,0, 1);
      //   //double jtPt=MMC.GetValue(Reco,PrimaryType,PrimaryIndex,iJ,0, 0, 1);
      //   	  //cout<<"Reco: "<<jtPt<<'\t'<<2.5*MMC.GetPTHat()<<endl;
      //   if(jtPt>2.5*MMC.GetPTHat()) continue;
      //   int RecoBin = MMC.GetCompositeBin(Reco,
      // 				    PrimaryType, PrimaryIndex, iJ, PrimaryRecoBins, 0, 1, 1, PrimaryRecoMin, PrimaryRecoMax,
      // 				    BinningType, BinningIndex, iJ, BinningRecoBins, 0, 1, 1, BinningRecoMin, BinningRecoMax);
      //   HMCReco.Fill(RecoBin, MMC.GetEventWeight() * MMC.GetJetWeight(Reco, iJ));

      //   int RecoGenBin = MMC.GetCompositeBin(Reco,
      // 				       PrimaryType, PrimaryIndex, iJ, PrimaryGenBins, 0, 1, 1, PrimaryGenMin, PrimaryGenMax,
      // 				       BinningType, BinningIndex, iJ, BinningGenBins, 0, 1, 1, BinningGenMin, BinningGenMax);
      //   HMCRecoGenBin.Fill(RecoGenBin, MMC.GetEventWeight() * MMC.GetJetWeight(Reco, iJ));
      // }

      //  FIXED CODE (uses Matched array - excludes fake jets):
      NJet = MMC.GetItemCount(Gen, PrimaryType);  // NGenJets (loop over gen)
      for(int iJ = 0; iJ < NJet; iJ++)
      {
        // Check if this gen jet has a valid match
        double Angle = MMC.GetMatchedAngle(PrimaryMatrixType, iJ);
        if(CheckMatchAngle == true && (Angle > MaxMatchAngle || Angle < 0))
          continue;  // Skip missing jets
        
        // Use Matched array (contains the reco jet that matched this gen jet)
        double jtPt = MMC.GetValue(Matched, PrimaryType, PrimaryIndex, iJ, 0, 0, 1);
        if(jtPt > 2.5 * MMC.GetPTHat()) continue;
        
        int RecoBin = MMC.GetCompositeBin(Matched,  // ← Changed from Reco to Matched
                                          PrimaryType, PrimaryIndex, iJ, PrimaryRecoBins, 0, 1, 1,
                                          PrimaryRecoMin, PrimaryRecoMax,
                                          BinningType, BinningIndex, iJ, BinningRecoBins, 0, 1, 1,
                                          BinningRecoMin, BinningRecoMax);
        HMCReco.Fill(RecoBin, MMC.GetEventWeight() * MMC.GetJetWeight(Matched, iJ));
        
        int RecoGenBin = MMC.GetCompositeBin(Matched,  // ← Changed from Reco to Matched
                                            PrimaryType, PrimaryIndex, iJ, PrimaryGenBins, 0, 1, 1,
                                            PrimaryGenMin, PrimaryGenMax,
                                            BinningType, BinningIndex, iJ, BinningGenBins, 0, 1, 1,
                                            BinningGenMin, BinningGenMax);
        HMCRecoGenBin.Fill(RecoGenBin, MMC.GetEventWeight() * MMC.GetJetWeight(Matched, iJ));
      }
 
    }

  BarMC.Update(EntryCount);
  BarMC.Print();
  BarMC.PrintLine();

  if(DoFlooring == true)
    {
      for(int i = 1; i <= HResponseNoWeight.GetNbinsX(); i++)
	for(int j = 1; j <= HResponseNoWeight.GetNbinsY(); j++)
	  if(HResponseNoWeight.GetBinContent(i, j) == 0)
	    HResponseNoWeight.SetBinContent(i, j, 0.5);
      for(int i = 1; i <= HResponse.GetNbinsX(); i++)
	for(int j = 1; j <= HResponse.GetNbinsY(); j++)
	  if(HResponse.GetBinContent(i, j) == 0)
	    HResponse.SetBinContent(i, j, 0.5);
    }

  map<int, vector<int>> Lumis;

  Messenger MData(FData);
  EntryCount = MData.GetEntries() * DataFraction;
  ProgressBar BarData(cout, EntryCount);
  BarData.SetStyle(2);

  MData.SetEtaMin(EtaMin);
  MData.SetEtaMax(EtaMax);
   
  for(int iE = 0; iE < EntryCount; iE++)
    {
      MData.GetEntry(iE);
      
      BarData.Update(iE);
      if(EntryCount < 300 || (iE % (EntryCount / 200)) == 0)
	BarData.Print();

      if(ExportJSON == true)
	{
	  if(Lumis.find(MData.GetRun()) == Lumis.end())
            Lumis.insert(pair<int, vector<int>>(MData.GetRun(), vector<int>{}));
	  Lumis[MData.GetRun()].push_back(MData.GetLumi());
	  if(MData.GetRun() > 1 && Lumis[MData.GetRun()].size() > 5000)
	    {
	      vector<int> V = Lumis[MData.GetRun()];
	      sort(V.begin(), V.end());
	      V.erase(unique(V.begin(), V.end()), V.end());
	      Lumis[MData.GetRun()] = V;
	    }
	}

      int NJet = MData.GetItemCount(Reco, PrimaryType);
      for(int iJ = 0; iJ < NJet; iJ++)
	{
	  double jtPt=MData.GetValue(Reco,PrimaryType,PrimaryIndex,iJ,0,0, 1);
	  //double jtPt=MMC.GetValue(Reco,PrimaryType,PrimaryIndex,iJ,PrimaryShift, PrimarySmear, 1);
	  //if(jtPt>2.5*MData.GetPTHat()) continue;
	  int RecoBin = MData.GetCompositeBin(Reco,
					      PrimaryType, PrimaryIndex, iJ, PrimaryRecoBins, 0, 1, 1, PrimaryRecoMin, PrimaryRecoMax,
					      BinningType, BinningIndex, iJ, BinningRecoBins, 0, 1, 1, BinningRecoMin, BinningRecoMax);
	     
	  HDataReco.Fill(RecoBin, MData.GetEventWeight() * MData.GetJetWeight(Reco, iJ));
         
	  int RecoGenBin = MData.GetCompositeBin(Reco,
						 PrimaryType, PrimaryIndex, iJ, PrimaryGenBins, 0, 1, 1, PrimaryGenMin, PrimaryGenMax,
						 BinningType, BinningIndex, iJ, BinningGenBins, 0, 1, 1, BinningGenMin, BinningGenMax);
	  HDataRecoGenBin.Fill(RecoGenBin, MData.GetEventWeight() * MData.GetJetWeight(Reco, iJ));
	}
    }
   
  BarData.Update(EntryCount);
  BarData.Print();
  BarData.PrintLine();

  if(ExportJSON == true)
    {
      bool First = true;

      ofstream JSON(JSONFileName);

      JSON << "{";
      for(pair<int, vector<int>> P : Lumis)
	{
	  if(First == true)
            First = false;
	  else
            JSON << ",";
         
	  vector<int> &V = P.second;
	  sort(V.begin(), V.end());
	  V.erase(unique(V.begin(), V.end()), V.end());

	  bool FirstLumi = true;

	  JSON << "\"" << P.first << "\": [";
	  for(int L : V)
	    {
	      if(FirstLumi == true)
		FirstLumi = false;
	      else
		JSON << ",";
	      JSON << "[" << L << "," << L << "]";
	    }
	  JSON << "]";
	}
      JSON << "}";

      JSON.close();
    }

  FOutput.cd();
  HMCGen.Write();
  HMCMatched.Write();
  HMCMatchedGenBin.Write();
  HMCReco.Write();
  HMCRecoGenBin.Write();
  HResponseNoWeight.Write();
  HResponse.Write();
  HDataReco.Write();
  HDataRecoGenBin.Write();
  HGenPrimaryBinMin.Write();
  HGenPrimaryBinMax.Write();
  HGenBinningBinMin.Write();
  HGenBinningBinMax.Write();
  HRecoPrimaryBinMin.Write();
  HRecoPrimaryBinMax.Write();
  HRecoBinningBinMin.Write();
  HRecoBinningBinMax.Write();
  HMatchedPrimaryBinMin.Write();
  HMatchedPrimaryBinMax.Write();
  HMatchedBinningBinMin.Write();
  HMatchedBinningBinMax.Write();

  FOutput.Close();
  FData.Close();
  FMC.Close();

  return 0;
}

void FillMinMax(TH1D &HMin1, TH1D &HMax1, TH1D &HMin2, TH1D &HMax2, vector<double> &Bin1, vector<double> &Bin2)
{
  for(int iP = 0; iP <= Bin1.size(); iP++)
    {
      for(int iB = 0; iB <= Bin2.size(); iB++)
	{
	  int Bin = iB * (Bin1.size() + 1) + iP + 1;

	  if(iP == 0)
            HMin1.SetBinContent(Bin, -99999);
	  else
            HMin1.SetBinContent(Bin, Bin1[iP-1]);
         
	  if(iP >= Bin1.size())
            HMax1.SetBinContent(Bin, 99999);
	  else
            HMax1.SetBinContent(Bin, Bin1[iP]);

	  if(iB == 0)
            HMin2.SetBinContent(Bin, -99999);
	  else
            HMin2.SetBinContent(Bin, Bin2[iB-1]);
         
	  if(iB >= Bin2.size())
            HMax2.SetBinContent(Bin, 99999);
	  else
            HMax2.SetBinContent(Bin, Bin2[iB]);
	}
    }
}

vector<double> GetFineBins(double Min, double Max)
{
  vector<double> Result;
  for(int i = (int)Min + 1; i < (int)Max; i++)
    Result.push_back(i);
  return Result;
}



