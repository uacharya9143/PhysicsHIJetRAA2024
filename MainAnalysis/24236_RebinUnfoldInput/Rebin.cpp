#include <iostream>
#include <vector>
using namespace std;

#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"

#include "CommandLine.h"
#include "DataHelper.h"
#include "CustomAssert.h"
#include "DrawRandom.h"

#include "BinHelper.h"

int main(int argc, char *argv[]);
bool CheckLineUp(const vector<double> &InputBins, const vector<double> &OutputBins);
void ZeroOffdiagonal(TH2D *H, double ZeroMin, double ZeroMax,
   const vector<double> &XBins, const vector<double> &YBins);
void Earthquake(TH2D *H, double Magnitude);
void ApplyBias(TH2D *H, double XPower, double YPower,
   const vector<double> &XBins, const vector<double> &YBins);
TH1D *RebinHistogram1D(TH1D *HIn, const vector<double> &InputBins, const vector<double> &OutputBins);
TH2D *RebinHistogram2D(TH2D *HIn,
   const vector<double> &InputXBins, const vector<double> &OutputXBins,
   const vector<double> &InputYBins, const vector<double> &OutputYBins);
void ConstructBinningHistograms(string Tag, vector<double> &OutputBins);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName       = CL.Get("Input");
   string OutputFileName      = CL.Get("Output");
   string DHFileName          = CL.Get("DHFile");

   bool DoZeroing             = CL.GetBool("DoZeroing", false);
   double ZeroMin             = DoZeroing ? CL.GetDouble("ZeroMin") : -1;
   double ZeroMax             = DoZeroing ? CL.GetDouble("ZeroMax") : -1;
   bool DoEarthquake          = CL.GetBool("DoEarthquake", false);
   double EarthquakeMagnitude = DoEarthquake ? CL.GetDouble("EarthquakeMagnitude") : -1;
   bool DoBias                = CL.GetBool("DoBias", false);
   double GenBiasPower        = DoBias ? CL.GetDouble("GenBiasPower") : 0;
   double RecoBiasPower       = DoBias ? CL.GetDouble("RecoBiasPower") : 0;

   DataHelper DHFile(DHFileName);

   TFile InputFile(InputFileName.c_str());
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   vector<double> InputGenBins      = DetectBins((TH1D *)InputFile.Get("HGenPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HGenPrimaryBinMax"));
   vector<double> InputRecoBins     = DetectBins((TH1D *)InputFile.Get("HRecoPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HRecoPrimaryBinMax"));
   vector<double> InputMatchedBins  = DetectBins((TH1D *)InputFile.Get("HMatchedPrimaryBinMin"),
                                                 (TH1D *)InputFile.Get("HMatchedPrimaryBinMax"));

   vector<double> OutputGenBins     = ParseList(DHFile["Binning"]["GenPT"].GetString());
   vector<double> OutputRecoBins    = ParseList(DHFile["Binning"]["RecoPT"].GetString());
   vector<double> OutputMatchedBins = ParseList(DHFile["Binning"]["RecoPT"].GetString());

   Assert(CheckLineUp(InputGenBins, OutputGenBins),         "Gen bins do not line up!");
   Assert(CheckLineUp(InputRecoBins, OutputRecoBins),       "Reco bins do not line up!");
   Assert(CheckLineUp(InputMatchedBins, OutputMatchedBins), "Matched bins do not line up!");

   OutputGenBins.insert(OutputGenBins.begin(), InputGenBins[0]);
   OutputRecoBins.insert(OutputRecoBins.begin(), InputRecoBins[0]);
   OutputMatchedBins.insert(OutputMatchedBins.begin(), InputMatchedBins[0]);
   
   OutputGenBins.insert(OutputGenBins.end(), InputGenBins[InputGenBins.size()-1]);
   OutputRecoBins.insert(OutputRecoBins.end(), InputRecoBins[InputRecoBins.size()-1]);
   OutputMatchedBins.insert(OutputMatchedBins.end(), InputMatchedBins[InputMatchedBins.size()-1]);

   vector<pair<string, string>> Histogram1D
   {
      {"Gen",     "HMCGen"},
      {"Matched", "HMCMatched"},
      {"Gen",     "HMCMatchedGenBin"},
      {"Reco",    "HMCReco"},
      {"Gen",     "HMCRecoGenBin"},
      {"Reco",    "HDataReco"},
      {"Gen",     "HDataRecoGenBin"}
   };
   vector<string> Histogram2D
   {
      "HResponseNoWeight",
      "HResponse"
   };

   for(pair<string, string> HName : Histogram1D)
   {
      vector<double> *InputBins, *OutputBins;
      if(HName.first == "Gen")
         InputBins = &InputGenBins, OutputBins = &OutputGenBins;
      if(HName.first == "Reco")
         InputBins = &InputRecoBins, OutputBins = &OutputRecoBins;
      if(HName.first == "Matched")
         InputBins = &InputMatchedBins, OutputBins = &OutputMatchedBins;

      string InputName = HName.second;

      // Small hack for now before we have proper histograms from the fine version
      if(InputName == "HDataRecoGenBin")
         InputName = "HDataReco";

      TH1D *H = RebinHistogram1D((TH1D *)InputFile.Get(InputName.c_str()), *InputBins, *OutputBins);
      OutputFile.cd();
      H->Clone(HName.second.c_str())->Write();
   }

   for(string HName : Histogram2D)
   {
      TH2D *HIn = (TH2D *)InputFile.Get(HName.c_str());
      if(DoZeroing == true)
         ZeroOffdiagonal(HIn, ZeroMin, ZeroMax, InputMatchedBins, InputGenBins);
      if(DoBias == true)
         ApplyBias(HIn, RecoBiasPower, GenBiasPower, InputMatchedBins, InputGenBins);
      if(DoEarthquake == true)
         Earthquake(HIn, EarthquakeMagnitude);
      TH2D *H = RebinHistogram2D(HIn, InputMatchedBins, OutputMatchedBins, InputGenBins, OutputGenBins);
      OutputFile.cd();
      H->Clone(HName.c_str())->Write();
   }

   OutputFile.cd();

   ConstructBinningHistograms("Gen", OutputGenBins);
   ConstructBinningHistograms("Reco", OutputRecoBins);
   ConstructBinningHistograms("Matched", OutputMatchedBins);

   OutputFile.Close();
   InputFile.Close();

   return 0;
}

bool CheckLineUp(const vector<double> &InputBins, const vector<double> &OutputBins)
{
   for(double V : OutputBins)
   {
      bool Found = false;
      for(double C : InputBins)
         if(fabs(C - V) < 1e-7)
            Found = true;
      if(Found == false)
         return false;
   }
   return true;
}

void ZeroOffdiagonal(TH2D *H, double ZeroMin, double ZeroMax,
   const vector<double> &XBins, const vector<double> &YBins)
{
   if(H == nullptr)
      return;

   int NX = XBins.size() - 1;
   int NY = YBins.size() - 1;

   for(int iX = 0; iX < NX; iX++)
   {
      for(int iY = 0; iY < NY; iY++)
      {
         double XMin = XBins[iX];
         double XMax = XBins[iX+1];
         if(XMin < -9999)   XMin = XMax - 1;
         if(XMax > 9999)    XMax = XMin + 1;
         
         double YMin = YBins[iY];
         double YMax = YBins[iY+1];
         if(YMin < -9999)   YMin = YMax - 1;
         if(YMax > 9999)    YMax = YMin + 1;
         
         double X = (XMin + XMax) / 2;
         double Y = (YMin + YMax) / 2;

         double R = Y / X;

         if((ZeroMin > 0 && R < ZeroMin)
            || (ZeroMax > 0 && R > ZeroMax))
         {
            H->SetBinContent(iX + 1, iY + 1, 0);
            H->SetBinError(iX + 1, iY + 1, 0);
         }
      }
   }
}

void Earthquake(TH2D *H, double Magnitude)
{
   if(H == nullptr)
      return;

   int NX = H->GetNbinsX();
   int NY = H->GetNbinsY();

   for(int iX = 1; iX <= NX; iX++)
   {
      for(int iY = 1; iY <= NY; iY++)
      {
         double V = H->GetBinContent(iX, iY);
         double E = H->GetBinError(iX, iY);

         V = DrawGaussian(V, E * Magnitude);
         if(V < 0)
            V = 0;

         H->SetBinContent(iX, iY, V);
      }
   }
}

void ApplyBias(TH2D *H, double XPower, double YPower,
   const vector<double> &XBins, const vector<double> &YBins)
{
   if(H == nullptr)
      return;

   int NX = XBins.size() - 1;
   int NY = YBins.size() - 1;

   for(int iX = 0; iX < NX; iX++)
   {
      for(int iY = 0; iY < NY; iY++)
      {
         double XMin = XBins[iX];
         double XMax = XBins[iX+1];
         if(XMin < -9999)   XMin = XMax - 1;
         if(XMax > 9999)    XMax = XMin + 1;
         
         double YMin = YBins[iY];
         double YMax = YBins[iY+1];
         if(YMin < -9999)   YMin = YMax - 1;
         if(YMax > 9999)    YMax = YMin + 1;
         
         double X = (XMin + XMax) / 2;
         double Y = (YMin + YMax) / 2;

         double Factor = pow(X, XPower) * pow(Y, YPower);

         H->SetBinContent(iX + 1, iY + 1, H->GetBinContent(iX + 1, iY + 1) * Factor);
         H->SetBinError(iX + 1, iY + 1, H->GetBinError(iX + 1, iY + 1) * Factor);
      }
   }
}

TH1D *RebinHistogram1D(TH1D *HIn, const vector<double> &InputBins, const vector<double> &OutputBins)
{
   if(HIn == nullptr)
      return nullptr;

   int NIn = InputBins.size() - 1;
   int NOut = OutputBins.size() - 1;

   static int Index = 0;
   Index = Index + 1;

   TH1D *HOut = new TH1D(Form("H1Out%d", Index), "", NOut, 0, NOut);
   HOut->SetTitle(HIn->GetTitle());
   HOut->GetXaxis()->SetTitle(HIn->GetXaxis()->GetTitle());
   HOut->GetYaxis()->SetTitle(HIn->GetYaxis()->GetTitle());

   int PreviousEnd = 0;
   for(int iOut = 0; iOut < NOut; iOut++)
   {
      double Min = OutputBins[iOut];
      double Max = OutputBins[iOut+1];

      for(int iIn = PreviousEnd; iIn < NIn; iIn++)
      {
         double In = (InputBins[iIn] + InputBins[iIn+1]) / 2;
         if(In < Min)
            continue;
         if(In > Max)
         {
            PreviousEnd = iIn;
            break;
         }

         double VIn  = HIn->GetBinContent(iIn + 1);
         double EIn  = HIn->GetBinError(iIn + 1);
         double VOut = HOut->GetBinContent(iOut + 1);
         double EOut = HOut->GetBinError(iOut + 1);

         VOut = VOut + VIn;
         EOut = sqrt(EOut * EOut + EIn * EIn);

         HOut->SetBinContent(iOut + 1, VOut);
         HOut->SetBinError(iOut + 1, EOut);
      }
   }

   return HOut;
}

TH2D *RebinHistogram2D(TH2D *HIn,
   const vector<double> &InputXBins, const vector<double> &OutputXBins,
   const vector<double> &InputYBins, const vector<double> &OutputYBins)
{
   if(HIn == nullptr)
      return nullptr;

   int NXIn = InputXBins.size() - 1;
   int NXOut = OutputXBins.size() - 1;
   int NYIn = InputYBins.size() - 1;
   int NYOut = OutputYBins.size() - 1;

   static int Index = 0;
   Index = Index + 1;

   TH2D *HOut = new TH2D(Form("H2Out%d", Index), "", NXOut, 0, NXOut, NYOut, 0, NYOut);
   HOut->SetTitle(HIn->GetTitle());
   HOut->GetXaxis()->SetTitle(HIn->GetXaxis()->GetTitle());
   HOut->GetYaxis()->SetTitle(HIn->GetYaxis()->GetTitle());

   int PreviousXEnd = 0;
   for(int iXOut = 0; iXOut < NXOut; iXOut++)
   {
      double XMin = OutputXBins[iXOut];
      double XMax = OutputXBins[iXOut+1];

      for(int iXIn = PreviousXEnd; iXIn < NXIn; iXIn++)
      {
         double XIn = (InputXBins[iXIn] + InputXBins[iXIn+1]) / 2;
         if(XIn < XMin)
            continue;
         if(XIn > XMax)
         {
            PreviousXEnd = iXIn;
            break;
         }

         int PreviousYEnd = 0;
         for(int iYOut = 0; iYOut < NYOut; iYOut++)
         {
            double YMin = OutputYBins[iYOut];
            double YMax = OutputYBins[iYOut+1];

            for(int iYIn = PreviousYEnd; iYIn < NYIn; iYIn++)
            {
               double YIn = (InputYBins[iYIn] + InputYBins[iYIn+1]) / 2;
               if(YIn < YMin)
                  continue;
               if(YIn > YMax)
               {
                  PreviousYEnd = iYIn;
                  break;
               }

               double VIn  = HIn->GetBinContent(iXIn + 1, iYIn + 1);
               double EIn  = HIn->GetBinError(iXIn + 1, iYIn + 1);
               double VOut = HOut->GetBinContent(iXOut + 1, iYOut + 1);
               double EOut = HOut->GetBinError(iXOut + 1, iYOut + 1);

               VOut = VOut + VIn;
               EOut = sqrt(EOut * EOut + EIn * EIn);

               HOut->SetBinContent(iXOut + 1, iYOut + 1, VOut);
               HOut->SetBinError(iXOut + 1, iYOut + 1, EOut);
            }
         }
      }
   }

   return HOut;
}

void ConstructBinningHistograms(string Tag, vector<double> &OutputBins)
{
   int N = OutputBins.size() - 1;

   TH1D HPrimaryBinMin(Form("H%sPrimaryBinMin", Tag.c_str()), "", N, 0, N);
   TH1D HPrimaryBinMax(Form("H%sPrimaryBinMax", Tag.c_str()), "", N, 0, N);
   TH1D HBinningBinMin(Form("H%sBinningBinMin", Tag.c_str()), "", N, 0, N);
   TH1D HBinningBinMax(Form("H%sBinningBinMax", Tag.c_str()), "", N, 0, N);

   for(int i = 0; i < N; i++)
   {
      HPrimaryBinMin.SetBinContent(i + 1, OutputBins[i]);
      HPrimaryBinMax.SetBinContent(i + 1, OutputBins[i+1]);
      HBinningBinMin.SetBinContent(i + 1, -99999);
      HBinningBinMax.SetBinContent(i + 1, +99999);
   }

   HPrimaryBinMin.Write();
   HPrimaryBinMax.Write();
   HBinningBinMin.Write();
   HBinningBinMax.Write();
}

