#include <iostream>
#include <vector>
using namespace std;

#include "TH1D.h"
#include "TFile.h"

#include "CustomAssert.h"
#include "CommandLine.h"
//#include "RootUtilities.h"
#include "DataHelper.h"

int main(int argc, char *argv[]);
vector<string> DetectVariations(DataHelper &DHFile, string State);
vector<double> DetectBins(TH1D *HMin, TH1D *HMax);
void SelfNormalize(TH1D *H, int NormalizationGroupSize = -1);
void AddQuadrature(TH1D &HTotalPlus, TH1D &HTotalMinus, TH1D &HNominal, TH1D &HVariation);
void DoBridging(TH1D *HB, TH1D *H, int Pattern = 0);
void DoBridgingPattern1(TH1D *HB, TH1D *H);
void DoBridgingPattern2(TH1D *HB, TH1D *H);
void DoBridgingPattern3(TH1D *HB, TH1D *H);

int main(int argc, char *argv[])
{
  // SilenceRoot();

   CommandLine CL(argc, argv);

   bool ReadFromDHFile = CL.GetBool("ReadFromDHFile");
   string BinMappingFileName         = CL.Get("BinMapping");
   string OutputFileName             = CL.Get("Output");
   vector<bool> DoSelfNormalize      = CL.GetBoolVector("DoSelfNormalize", vector<bool>{false});

   vector<string> BaseFileNames;
   vector<string> FileNames;
   vector<string> BaseHistogramNames;
   vector<string> HistogramNames;
   vector<string> Labels;
   vector<int> Groupings;
   vector<int> Bridging;
   vector<double> ExtraScaling;

   if(ReadFromDHFile == true)
   {
      string DHFileName = CL.Get("DHFile");
      string State      = CL.Get("State");

      DataHelper DHFile(DHFileName);

      vector<string> Sources = DetectVariations(DHFile, State);

      for(string S : Sources)
      {
         BaseFileNames.push_back(DHFile[State][S+"_BaseFile"].GetString());
         BaseHistogramNames.push_back(DHFile[State][S+"_BaseHistogram"].GetString());
         FileNames.push_back(DHFile[State][S+"_VariantFile"].GetString());
         HistogramNames.push_back(DHFile[State][S+"_VariantHistogram"].GetString());
         Labels.push_back(DHFile[State][S+"_Label"].GetString());
         Groupings.push_back(DHFile[State][S+"_Include"].GetInteger());
         Bridging.push_back(DHFile[State][S+"_Bridging"].GetInteger());
         ExtraScaling.push_back(DHFile[State][S+"_ExtraScaling"].GetDouble());
      }
   }
   else
   {
      BaseFileNames      = CL.GetStringVector("BaseInput");
      FileNames          = CL.GetStringVector("Input");
      BaseHistogramNames = CL.GetStringVector("BaseHistogram");
      HistogramNames     = CL.GetStringVector("Histogram");
      Labels             = CL.GetStringVector("Label");
      Groupings          = CL.GetIntVector("Group");
      Bridging           = CL.GetIntVector("Bridging");
      ExtraScaling       = CL.GetDoubleVector("ExtraScaling");
   }

   Assert(FileNames.size() > 0, "No file names specified");
   Assert(FileNames.size() == HistogramNames.size(), "Inconsistent file name and histogram name");
   Assert(FileNames.size() == Labels.size(), "Inconsistent file name and label name");
   Assert(FileNames.size() == Groupings.size(), "Unknown uncertainty grouping");
   Assert(FileNames.size() == BaseFileNames.size(), "Inconsistent file name and base file name");
   Assert(FileNames.size() == BaseHistogramNames.size(), "Inconsistent file name and base histogram name");

   if(DoSelfNormalize.size() == 1)
      DoSelfNormalize.resize(FileNames.size(), DoSelfNormalize[0]);
   
   Assert(FileNames.size() == DoSelfNormalize.size(), "Inconsistent file name and normalization prescription");

   if(ExtraScaling.size() < FileNames.size())
      ExtraScaling.insert(ExtraScaling.end(), FileNames.size() - ExtraScaling.size(), 1);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TFile BinMappingFile(BinMappingFileName.c_str());

   OutputFile.cd();
   BinMappingFile.Get("HGenPrimaryBinMin")->Clone()->Write();
   BinMappingFile.Get("HGenPrimaryBinMax")->Clone()->Write();
   BinMappingFile.Get("HGenBinningBinMin")->Clone()->Write();
   BinMappingFile.Get("HGenBinningBinMax")->Clone()->Write();
   BinMappingFile.Get("HRecoPrimaryBinMin")->Clone()->Write();
   BinMappingFile.Get("HRecoPrimaryBinMax")->Clone()->Write();
   BinMappingFile.Get("HRecoBinningBinMin")->Clone()->Write();
   BinMappingFile.Get("HRecoBinningBinMax")->Clone()->Write();
   BinMappingFile.Get("HMatchedPrimaryBinMin")->Clone()->Write();
   BinMappingFile.Get("HMatchedPrimaryBinMax")->Clone()->Write();
   BinMappingFile.Get("HMatchedBinningBinMin")->Clone()->Write();
   BinMappingFile.Get("HMatchedBinningBinMax")->Clone()->Write();
   
   int NormalizationGroupSize = DetectBins((TH1D *)BinMappingFile.Get("HGenPrimaryBinMin"), (TH1D *)BinMappingFile.Get("HGenPrimaryBinMax")).size() - 1;
   // cout << "Detected group size = " << NormalizationGroupSize << endl;

   BinMappingFile.Close();

   TH1D *HTotalPlus = nullptr;
   TH1D *HTotalMinus = nullptr;

   int N = FileNames.size();

   for(int i = 0; i < N; i++)
   {
      TFile File(FileNames[i].c_str());
      TFile BaseFile(BaseFileNames[i].c_str());

      TH1D *H = (TH1D *)File.Get(HistogramNames[i].c_str());
      TH1D *HB = (TH1D *)BaseFile.Get(BaseHistogramNames[i].c_str());

      if(H == nullptr || HB == nullptr)   // problem!  no systematics
      {
         OutputFile.cd();
         TH1D *HCloned = (TH1D *)BaseFile.Get("HMCTruth")->Clone(Form("H%s", Labels[i].c_str()));
         TH1D *HBCloned = (TH1D *)BaseFile.Get("HMCTruth")->Clone(Form("H%sBase", Labels[i].c_str()));
         HCloned->Write();
         HBCloned->Write();
      }
      else
      {
         OutputFile.cd();
         TH1D *HCloned = (TH1D *)H->Clone(Form("H%s", Labels[i].c_str()));
         TH1D *HBCloned = (TH1D *)HB->Clone(Form("H%sBase", Labels[i].c_str()));

         HCloned->Scale(ExtraScaling[i]);

         if(HTotalPlus == nullptr && HTotalMinus == nullptr)
         {
            HTotalPlus = (TH1D *)HCloned->Clone("HTotalPlus");
            HTotalMinus = (TH1D *)HCloned->Clone("HTotalMinus");
            HTotalPlus->Reset();
            HTotalMinus->Reset();
         }
         if(DoSelfNormalize[i] == true)
         {
            SelfNormalize(HCloned, NormalizationGroupSize);
            SelfNormalize(HBCloned, NormalizationGroupSize);
         }
         if(Bridging[i] > 0)
            DoBridging(HBCloned, HCloned, Bridging[i]);
         HCloned->Write();
         HBCloned->Write();

         if(Groupings[i] == 1)
            AddQuadrature(*HTotalPlus, *HTotalMinus, *HBCloned, *HCloned);
      }

      File.Close();
   }

   HTotalPlus->Write();
   HTotalMinus->Write();

   OutputFile.Close();

   return 0;
}

vector<string> DetectVariations(DataHelper &DHFile, string State)
{
   vector<string> Result;

   Assert(DHFile.Exist(State), "State does not exist in the DHFile");

   vector<string> Keys = DHFile[State].GetListOfKeys();
   for(string &Key : Keys)
   {
      int Location = Key.rfind("_");
      if(Location == string::npos)   // unrelated
         continue;
      Result.push_back(Key.substr(0, Location));
   }

   sort(Result.begin(), Result.end());
   Result.erase(unique(Result.begin(), Result.end()), Result.end());

   for(int i = 0; i < (int)Result.size(); i++)
   {
      string Source = Result[i];

      int ExistCount = 0;
      if(DHFile[State].Exist(Source + "_BaseFile") == true)           ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_BaseHistogram") == true)      ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_VariantFile") == true)        ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_VariantHistogram") == true)   ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_Label") == true)              ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_Include") == true)            ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_Bridging") == true)           ExistCount = ExistCount + 1;
      if(DHFile[State].Exist(Source + "_ExtraScaling") == true)       ExistCount = ExistCount + 1;

      if(ExistCount == 0)   // unrelated stuff
      {
         Result.erase(Result.begin() + i);
         i = i - 1;
         continue;
      }

      if(ExistCount < 8)
         cerr << "Warning: source " << Source << " incomplete!" << endl;
   }

   return Result;
}

vector<double> DetectBins(TH1D *HMin, TH1D *HMax)
{
   if(HMin == nullptr || HMax == nullptr)
      return vector<double>{};

   vector<pair<double, double>> Bins;

   for(int i = 1; i <= HMin->GetNbinsX(); i++)
      Bins.push_back(pair<double, double>(HMin->GetBinContent(i), HMax->GetBinContent(i)));

   sort(Bins.begin(), Bins.end());
   auto iterator = unique(Bins.begin(), Bins.end());
   Bins.erase(iterator, Bins.end());

   vector<double> Result;
   for(auto iterator : Bins)
   {
      Result.push_back(iterator.first);
      Result.push_back(iterator.second);
   }
   
   sort(Result.begin(), Result.end());
   auto iterator2 = unique(Result.begin(), Result.end());
   Result.erase(iterator2, Result.end());

   return Result;
}

void SelfNormalize(TH1D *H, int NormalizationGroupSize)
{
   if(H == nullptr)
      return;
   
   int BinningCount = H->GetNbinsX() / NormalizationGroupSize;
   if(BinningCount <= 0)
      BinningCount = 1;

   cout << H->GetNbinsX() << " " << NormalizationGroupSize << " " << BinningCount << endl;

   for(int iB = 0; iB < BinningCount; iB++)
   {
      double Total = 0;
      for(int i = 0; i < NormalizationGroupSize; i++)
         Total = Total + H->GetBinContent(i + 1 + iB * NormalizationGroupSize);
      
      for(int i = 0; i < NormalizationGroupSize; i++)
      {
         H->SetBinContent(i + 1 + iB * NormalizationGroupSize, H->GetBinContent(i + 1 + iB * NormalizationGroupSize) / Total);
         H->SetBinError(i + 1 + iB * NormalizationGroupSize, H->GetBinError(i + 1 + iB * NormalizationGroupSize) / Total);
      }
   }
}

void AddQuadrature(TH1D &HTotalPlus, TH1D &HTotalMinus, TH1D &HNominal, TH1D &HVariation)
{
   for(int i = 1; i <= HNominal.GetNbinsX(); i++)
   {
      double VN = HNominal.GetBinContent(i);
      double VV = HVariation.GetBinContent(i);
      
      if(VN == 0)
         continue;

      double Ratio = VV / VN - 1;

      double TotalPlus = HTotalPlus.GetBinContent(i);
      double TotalMinus = HTotalMinus.GetBinContent(i);

      TotalPlus = sqrt(TotalPlus * TotalPlus + Ratio * Ratio);
      TotalMinus = sqrt(TotalMinus * TotalMinus + Ratio * Ratio);

      HTotalPlus.SetBinContent(i, TotalPlus);
      HTotalMinus.SetBinContent(i, -TotalMinus);
   }
}

void DoBridging(TH1D *HB, TH1D *H, int Pattern)
{
   if(Pattern == 0)
      return;

   if(Pattern == 1)
      DoBridgingPattern1(HB, H);
   if(Pattern == 2)
      DoBridgingPattern2(HB, H);
   if(Pattern == 3)
      DoBridgingPattern3(HB, H);
}

void DoBridgingPattern1(TH1D *HB, TH1D *H)
{
   if(HB == nullptr || H == nullptr)
      return;

   int N = HB->GetNbinsX();
   vector<double> V(N);
   for(int i = 1; i <= N; i++)
      V[i-1] = fabs(H->GetBinContent(i) / HB->GetBinContent(i) - 1);

   double StartFraction = 0.5;
   int DecreaseBin = -1;
   for(int i = StartFraction * N; i < N - 1; i++)
   {
      if(V[i] > V[i-1])
      {
         DecreaseBin = i;
         break;
      }
   }

   if(DecreaseBin < 0)
      return;

   for(int i = DecreaseBin + 1; i < N - 2; i++)
      V[i] = V[DecreaseBin] + (V[N-1] - V[DecreaseBin]) / (N - 1 - DecreaseBin) * (i - DecreaseBin);

   for(int i = 1; i <= N; i++)
      H->SetBinContent(i, HB->GetBinContent(i) * (1 + V[i-1]));
}

void DoBridgingPattern2(TH1D *HB, TH1D *H)
{
   int Ignore = 1;

   if(HB == nullptr || H == nullptr)
      return;

   int N = HB->GetNbinsX();
   vector<double> V(N);
   for(int i = 1; i <= N; i++)
      V[i-1] = H->GetBinContent(i) / HB->GetBinContent(i) - 1;

   vector<int> Maximum(N);
   for(int i = 1; i < N - 1 - Ignore; i++)
   {
      if(V[i] > V[i-1] && V[i] > V[i+1] && V[i] > 0)
         Maximum[i] = 1;
      if(V[i] < V[i-1] && V[i] < V[i+1] && V[i] < 0)
         Maximum[i] = -1;
   }
   if(V[N-1-Ignore] > V[N-2-Ignore] && V[N-1-Ignore] > 0)
      Maximum[N-1-Ignore] = 1;
   if(V[N-1-Ignore] < V[N-2-Ignore] && V[N-1-Ignore] < 0)
      Maximum[N-1-Ignore] = -1;

   for(int i = 0; i < N; i++)
      V[i] = fabs(V[i]);

   int Start = -1;
   for(int i = 0; i < N; i++)
   {
      if(Maximum[i] == 0)
         continue;

      if(Start < 0)
      {
         Start = i;
         continue;
      }

      if(Maximum[Start] == Maximum[i])
      {
         Start = i;
         continue;
      }

      for(int j = Start + 1; j < i; j++)
         V[j] = V[Start] + (V[i] - V[Start]) / (i - Start) * (j - Start);
      Start = i;
   }   
   
   for(int i = 1; i <= N; i++)
      H->SetBinContent(i, HB->GetBinContent(i) * (1 + V[i-1]));
}

void DoBridgingPattern3(TH1D *HB, TH1D *H)
{
   int Ignore = 1;

   if(HB == nullptr || H == nullptr)
      return;

   int N = HB->GetNbinsX();
   vector<double> V(N);
   for(int i = 1; i <= N; i++)
      V[i-1] = H->GetBinContent(i) / HB->GetBinContent(i) - 1;

   vector<int> Maximum(N);
   for(int i = 1; i < N - 1 - Ignore; i++)
   {
      if(V[i] > V[i-1] && V[i] > V[i+1] && V[i] > 0)
         Maximum[i] = 1;
      if(V[i] < V[i-1] && V[i] < V[i+1] && V[i] < 0)
         Maximum[i] = -1;
   }
   if(V[N-1-Ignore] > V[N-2-Ignore] && V[N-1-Ignore] > 0)
      Maximum[N-1-Ignore] = 1;
   if(V[N-1-Ignore] < V[N-2-Ignore] && V[N-1-Ignore] < 0)
      Maximum[N-1-Ignore] = -1;

   for(int i = 0; i < N; i++)
      V[i] = fabs(V[i]);

   int Start = -1;
   for(int i = 0; i < N; i++)
   {
      if(Maximum[i] == 0)
         continue;

      if(Start < 0)
      {
         Start = i;
         continue;
      }

      if(Maximum[Start] == Maximum[i])
      {
         Start = i;
         continue;
      }

      for(int j = Start + 1; j < i; j++)
         V[j] = V[Start] + (V[i] - V[Start]) / (i - Start) * (j - Start);
      Start = i;
   }

   for(int i = N - 1; i >= 0; i--)
   {
      if(Maximum[i] == 0)
         continue;

      for(int j = i; j < N; j++)
         V[j] = V[i];
      break;
   }
   
   for(int i = 1; i <= N; i++)
   {
      if(V[i-1] != V[i-1])
         continue;
      H->SetBinContent(i, HB->GetBinContent(i) * (1 + V[i-1]));
   }
}

