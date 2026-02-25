#include <iostream>
#include <cstdlib>
using namespace std;

#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TKey.h"

#include "CommandLine.h"
#include "DrawRandom.h"

int main(int argc, char *argv[]);
double GetRandom(TH1D *H);
void GenerateSample(TH1D *HShape, TH1D *HTarget, int Underflow, int Overflow, long long ActualEvent);
void GenerateSampleFast(TH1D *HShape, TH1D *HTarget, int Underflow, int Overflow, long long ActualEvent);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string MCFileName = CL.Get("MC");
   string ShapeFileName = CL.Get("Shape");
   string OutputFileName = CL.Get("Output");
   string YieldFileName = CL.Get("Yield", ShapeFileName);
   double TargetFraction = CL.GetDouble("Fraction", 1.00);
   double RandomSeed = CL.GetDouble("Seed", 31426);
   int Underflow = CL.GetInt("Underflow", 0);
   int Overflow = CL.GetInt("Overflow", 0);

   srand(RandomSeed);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TFile MCFile(MCFileName.c_str());

   TIter Iter(MCFile.GetListOfKeys());
   for(TKey *Key = (TKey *)Iter(); Key != nullptr; Key = (TKey *)Iter())
   {
      if(Key->ReadObj()->GetName() == TString("HDataReco"))
         continue;
      if(Key->ReadObj()->GetName() == TString("HMCGen"))
         continue;

      OutputFile.cd();
      Key->ReadObj()->Clone()->Write();
   }

   MCFile.Close();

   TFile YieldFile(YieldFileName.c_str());
   TH1D *HYield = (TH1D *)YieldFile.Get("HDataReco");
   double TargetEventCount = HYield->Integral(1 + Underflow, HYield->GetNbinsX() - Overflow) * TargetFraction;
   long long ActualEvent = DrawPoisson(TargetEventCount);
   YieldFile.Close();
   
   TFile ShapeFile(ShapeFileName.c_str());

   OutputFile.cd();
   ShapeFile.Get("HMCGen")->Clone()->Write();

   TH1D *HShape = (TH1D *)ShapeFile.Get("HDataReco");
   // cout << HShape->Integral(1 + Underflow, HShape->GetNbinsX() - Overflow) << " " << HShape->Integral() << endl;
   // cout << ActualEvent << endl;

   TH1D *HNewData = (TH1D *)HShape->Clone();
   HNewData->Reset();
   if(HShape->Integral() == HShape->Integral())
      GenerateSampleFast(HShape, HNewData, Underflow, Overflow, ActualEvent);
   HNewData->SetName("HDataReco");
   HNewData->Write();

   HShape->SetName("HShapeReco");
   HShape->Write();

   ShapeFile.Close();

   OutputFile.Close();

   return 0;
}

double GetRandom(TH1D *H)
{
   if(H == nullptr)
      return 0;

   double Value = DrawRandom(0, H->Integral());

   double SoFar = 0;
   for(int i = 1; i <= H->GetNbinsX(); i++)
   {
      SoFar = SoFar + H->GetBinContent(i);
      if(SoFar >= Value)
         return H->GetBinCenter(i);
   }

   return 0;
}

void GenerateSample(TH1D *HShape, TH1D *HTarget, int Underflow, int Overflow, long long ActualEvent)
{
   // Simply fill histogram until we get the target count in the relevant bins

   if(HShape == nullptr || HTarget == nullptr)
      return;
   if(HTarget == HShape)   // WTF
      return;
   
   while(HTarget->Integral(1 + Underflow, HTarget->GetNbinsX() - Overflow) < ActualEvent)
      HTarget->Fill(GetRandom(HShape));
}

void GenerateSampleFast(TH1D *HShape, TH1D *HTarget, int Underflow, int Overflow, long long ActualEvent)
{
   // In this function we under-generate using poisson in each bin.  Then we call the old generate sample to
   //    fill in the gap.

   if(HShape == nullptr || HTarget == nullptr)
      return;
   if(HTarget == HShape)   // WTF
      return;
   
   int N = HShape->GetNbinsX();

   vector<long long> Events(N);
   double Factor = ActualEvent / HShape->Integral(1 + Underflow, N - Overflow);
   double Total = 0;

   do
   {
      for(int i = 0; i < N; i++)
         Events[i] = DrawPoisson(HShape->GetBinContent(i + 1) * Factor);

      Total = 0;
      for(int i = Underflow; i < N - Overflow; i++)
         Total = Total + Events[i];

   } while(Total > ActualEvent);

   for(int i = 0; i < N; i++)
   {
      // cout << i << " " << Events[i] << endl;

      HTarget->SetBinContent(i + 1, Events[i]);
      HTarget->SetBinError(i + 1, sqrt(Events[i]));
   }

   GenerateSample(HShape, HTarget, Underflow, Overflow, ActualEvent);
}


