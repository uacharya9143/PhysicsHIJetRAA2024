#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLatex.h"

#include "CommandLine.h"
#include "CustomAssert.h"
#include "DataHelper.h"
#include "SetStyle.h"
//#include "RootUtilities.h"

#include "BinHelper.h"

int main(int argc, char *argv[]);
vector<TGraphAsymmErrors> Transcribe(TH1D *H, vector<double> Bins1, vector<double> Bins2, TH1D *H2 = nullptr, bool Normalize = true, double ExtraScale = 1);
TGraphAsymmErrors CalculateRatio(TGraphAsymmErrors &G1, TGraphAsymmErrors &G2, string DHFileName, string DHState);
TGraphAsymmErrors BuildSystematics(TGraphAsymmErrors &G, string FileName);
TGraph ToGraph(TH1D *H);
int GetBin(double Value, vector<double> &BinBoundary);

int main(int argc, char *argv[])
{
  //SilenceRoot();

   SetThesisStyle();
   vector<int> Colors = GetPrimaryColors();

   // Command line inputs
   CommandLine CL(argc, argv);

   string BaseFileName        = CL.Get("BaseFileName");
   string BaseRLabel          = CL.Get("BaseRLabel");
   vector<string> FileName    = CL.GetStringVector("FileName", vector<string>{});
   vector<string> RLabel      = CL.GetStringVector("RLabel", vector<string>{});
   vector<string> SysName     = CL.GetStringVector("Systematics", vector<string>{});
   vector<int> Color          = CL.GetIntVector("Color", vector<int>{1, 2, 3, 4, 5, 6, 7});

   string StatDHFileName      = CL.Get("StatDHFile");
   vector<string> StatDHState = CL.GetStringVector("StatDHState");

   double XMin                = CL.GetDouble("XMin", 100);
   double XMax                = CL.GetDouble("XMax", 1500);
   
   bool AddHIN18014           = CL.GetBool("AddHIN18014", false);

   string OutputFileName      = CL.Get("Output", "PPRatio.pdf");

   int FileCount = FileName.size();

   Assert(FileCount > 0, "Please specify at least one curve to plot");
   Assert(FileCount == RLabel.size(), "Please specify the radius labels for the input files");
   Assert(FileCount == SysName.size(), "We need systematics for all the files.  Put \"none\" to bypass for given curve");
   //Assert(FileCount == StatDHState.size(), "Stat reduction rho state count wrong.");

   // Get the global setting DH file
   string GlobalSettingFile  = CL.Get("GlobalSetting", "GlobalSetting.dh");
   DataHelper DHFile(GlobalSettingFile);

   TFile BaseFile(BaseFileName.c_str());

   // Find out the binnings
   vector<double> GenBins1
      = DetectBins((TH1D *)BaseFile.Get("HGenPrimaryBinMin"), (TH1D *)BaseFile.Get("HGenPrimaryBinMax"));
   vector<double> GenBins2
      = DetectBins((TH1D *)BaseFile.Get("HGenBinningBinMin"), (TH1D *)BaseFile.Get("HGenBinningBinMax"));

   // Get base spectrum
   string BasePRC            = "PPData_R" + BaseRLabel + "_CentralityInclusive";
   string BasePrior          = DHFile["PriorToUse"][BasePRC+"_Default"].GetString() + "Prior";
   int BaseIteration         = DHFile["Iterations"][BasePRC+"_Nominal_"+BasePrior].GetInteger();
   TH1D *HBase               = (TH1D *)BaseFile.Get(Form("HUnfoldedBayes%d", BaseIteration));
   double LumiBase           = stof(DHFile["Lumi"][BasePRC+"_BRIL"].GetString());
   double PUBugBase          = DHFile["PUBugCorrection"][BasePRC].GetDouble();
   double EventSelectionBase = DHFile["EventSelection"][BasePRC].GetDouble();
   double FactorBase         = PUBugBase * EventSelectionBase / LumiBase;
   vector<TGraphAsymmErrors> GBase = Transcribe(HBase, GenBins1, GenBins2, nullptr, true, FactorBase);

   // cout << BasePRC+"_Nominal_"+BasePrior << endl;
   // cout << BaseIteration << " " << HBase << endl;
   // cout << PUBugBase << " " << LumiBase << " " << DHFile["Lumi"]["PPData_R"+BaseRLabel+"_CentralityInclusive_BRIL"].GetString() << endl;

   // Get the spectra
   vector<vector<TGraphAsymmErrors>> GSpectra(FileCount);
   for(int i = 0; i < FileCount; i++)
   {
      TFile File(FileName[i].c_str());

      string PRC            = "PPData_R" + RLabel[i] + "_CentralityInclusive";
      string Prior          = DHFile["PriorToUse"][PRC+"_Default"].GetString() + "Prior";
      int Iteration         = DHFile["Iterations"][PRC+"_Nominal_"+Prior].GetInteger();
      TH1D *H               = (TH1D *)File.Get(Form("HUnfoldedBayes%d", Iteration));
      double PUBug          = DHFile["PUBugCorrection"][PRC].GetDouble();
      double EventSelection = DHFile["EventSelection"][PRC].GetDouble();
      double Lumi           = stof(DHFile["Lumi"][PRC+"_BRIL"].GetString());
      double Factor         = PUBug * EventSelection / Lumi;
      GSpectra[i] = Transcribe(H, GenBins1, GenBins2, nullptr, true, Factor);

      File.Close();
   }

   // Calculate all the ratio's
   vector<vector<TGraphAsymmErrors>> GRatio(FileCount);
   for(int i = 0; i < FileCount; i++)
      for(int j = 0; j < GSpectra[i].size(); j++)
         GRatio[i].push_back(CalculateRatio(GSpectra[i][j], GBase[j], StatDHFileName, StatDHState[i]));
   
   // Build systematics
   vector<vector<TGraphAsymmErrors>> GSys(FileCount);
   for(int i = 0; i < FileCount; i++)
      for(int j = 0; j < GSpectra[i].size(); j++)
         GSys[i].push_back(BuildSystematics(GRatio[i][j], SysName[i]));

   string LuminosityString
      = DHFile["Lumi"][Form("PPData_R%s_CentralityInclusive_BRIL",BaseRLabel.c_str())].GetString();
   double Luminosity = stof(LuminosityString) / 1000 / 1000;
   string LuminosityUnit = "pb^{-1}";

   // cout << LuminosityString << endl;

   // Start to assemble the plot
   TCanvas Canvas;
   Canvas.SetLogx();

   TH2D HWorld("HWorld", Form(";Jet p_{T} (GeV);#sigma(R = X) / #sigma(R = %.1f)", DHFile["JetR"][BaseRLabel].GetDouble()),
      100, XMin, XMax, 100, 0, 1.4);
   HWorld.SetStats(0);
   HWorld.GetXaxis()->SetMoreLogLabels();

   HWorld.Draw("axis");

   vector<TGraph> HIN18014(5);
   vector<TGraph> HIN18014P6(5);
   vector<TGraph> HIN18014P8(5);
   if(AddHIN18014 == true)
   {
      TFile File("HEPData/HIN18014PPRatio.root");

      HIN18014[0] = *((TGraph *)File.Get("R2"));
      HIN18014[1] = *((TGraph *)File.Get("R3"));
      HIN18014[2] = *((TGraph *)File.Get("R4"));
      HIN18014[3] = *((TGraph *)File.Get("R6"));
      HIN18014[4] = *((TGraph *)File.Get("R7"));

      File.Close();

      for(int i = 0; i < 5; i++)
      {
         HIN18014[i].SetLineColor(Colors[i]);
         HIN18014[i].SetLineWidth(2);
         HIN18014[i].Draw("l");
      }
      
      TFile FilePythia("SpectraRatio_Smooth_RedStat.root");

      HIN18014P6[0] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT6_1"));
      HIN18014P6[1] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT6_2"));
      HIN18014P6[2] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT6_3"));
      HIN18014P6[3] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT6_4"));
      HIN18014P6[4] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT6_5"));
      
      HIN18014P8[0] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT8_1"));
      HIN18014P8[1] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT8_2"));
      HIN18014P8[2] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT8_3"));
      HIN18014P8[3] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT8_4"));
      HIN18014P8[4] = ToGraph((TH1D *)FilePythia.Get("theoryCompPYT8_5"));

      FilePythia.Close();
      
      for(int i = 0; i < 5; i++)
      {
         HIN18014P6[i].SetLineColor(Colors[i]);
         HIN18014P6[i].SetLineWidth(2);
         HIN18014P6[i].SetLineStyle(kDashed);
         HIN18014P6[i].Draw("l");
         
         HIN18014P8[i].SetLineColor(Colors[i]);
         HIN18014P8[i].SetLineWidth(2);
         HIN18014P8[i].SetLineStyle(kDotted);
         HIN18014P8[i].Draw("l");
      }
   }

   for(int i = 0; i < FileCount; i++)
   {
      for(int j = 0; j < GRatio[i].size(); j++)
      {
         GSys[i][j].SetLineColor(Colors[Color[i]]);
         GSys[i][j].SetFillColorAlpha(Colors[Color[i]], 0.25);
         GSys[i][j].SetLineWidth(2);
         GSys[i][j].SetMarkerColor(Colors[Color[i]]);
         GSys[i][j].SetMarkerStyle(20);
         GSys[i][j].SetMarkerSize(2);
         GSys[i][j].Draw("2");
         
         GRatio[i][j].SetLineColor(Colors[Color[i]]);
         GRatio[i][j].SetLineWidth(2);
         GRatio[i][j].SetMarkerColor(Colors[Color[i]]);
         GRatio[i][j].SetMarkerStyle(20);
         GRatio[i][j].SetMarkerSize(2);
         GRatio[i][j].Draw("p");
      }
   }

   TLegend Legend(0.15, 0.70, 0.30, 0.70 + 0.035 * (FileCount > 5 ? 5 : FileCount));
   Legend.SetTextSize(0.035);
   Legend.SetFillStyle(0);
   Legend.SetBorderSize(0);
   
   TLegend Legend2(0.35, 0.70, 0.50, 0.70 + 0.035 * (FileCount > 5 ? FileCount - 5: 1));
   Legend2.SetTextSize(0.035);
   Legend2.SetFillStyle(0);
   Legend2.SetBorderSize(0);
   
   for(int i = 0; i < FileCount; i++)
   {
      if(i < 5)
         Legend.AddEntry(&GSys[i][0], Form("R = %.2f", DHFile["JetR"][RLabel[i]].GetDouble()), "lpf");
      else
         Legend2.AddEntry(&GSys[i][0], Form("R = %.2f", DHFile["JetR"][RLabel[i]].GetDouble()), "lpf");
   }
   Legend.Draw();
   if(FileCount > 5)
      Legend2.Draw();
   
   TGraph GExampleData, GExampleHIN18014, GExampleHIN18014P6, GExampleHIN18014P8;
   GExampleData.SetLineWidth(2);
   GExampleData.SetMarkerStyle(20);
   GExampleData.SetMarkerSize(2);
   GExampleHIN18014.SetLineWidth(2);
   GExampleHIN18014P6.SetLineWidth(2);
   GExampleHIN18014P6.SetLineStyle(kDashed);
   GExampleHIN18014P8.SetLineWidth(2);
   GExampleHIN18014P8.SetLineStyle(kDotted);

   TLegend LegendS(0.35, 0.12, 0.50, 0.12 + 0.035 * FileCount);
   LegendS.SetTextSize(0.035);
   LegendS.SetFillStyle(0);
   LegendS.SetBorderSize(0);
   LegendS.AddEntry("", "Styles", "");
   LegendS.AddEntry(&GExampleData, "Data", "pl");
   LegendS.AddEntry(&GExampleHIN18014, "HIN-18-014 Central Value", "l");
   LegendS.AddEntry(&GExampleHIN18014P6, "Pythia6", "l");
   LegendS.AddEntry(&GExampleHIN18014P8, "Pythia8", "l");
   if(AddHIN18014 == true)
      LegendS.Draw();

   TGraph GLine;
   GLine.SetPoint(0, 0, 1);
   GLine.SetPoint(1, 100000, 1);
   GLine.Draw("l");

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetNDC();

   Latex.SetTextAlign(12);
   Latex.DrawLatex(0.10, 0.92, "CMS #font[52]{Preliminary}");
   //Latex.DrawLatex(0.10, 0.92, "Work in Progress");
   Latex.SetTextAlign(32);
   Latex.DrawLatex(0.85, 0.92, Form("pp 5.36 TeV %.2f %s", Luminosity, LuminosityUnit.c_str()));
   
   // Latex.SetTextAlign(12);
   // Latex.DrawLatex(0.12, 0.87, "Statistical only");
   
   // if(AddHIN18014 == true)
   // {
   //    Latex.SetTextAlign(12);
   //    Latex.DrawLatex(0.12, 0.82, "Line = HIN-18-014 Central value");
   // }

   Canvas.SaveAs(OutputFileName.c_str());

   BaseFile.Close();

   return 0;
}

vector<TGraphAsymmErrors> Transcribe(TH1D *H, vector<double> Bins1, vector<double> Bins2, TH1D *H2, bool Normalize, double ExtraScale)
{
   // cout << ExtraScale << endl;
   // cout << H << endl;

   int BinningCount = Bins2.size() - 1;
   if(BinningCount <= 0)
      BinningCount = 1;

   vector<TGraphAsymmErrors> Result(BinningCount);
   if(H == nullptr)
      return Result;

   int PrimaryBinCount = Bins1.size() - 1;
   double PrimaryBins[1000] = {0};
   for(int i = 0; i <= PrimaryBinCount; i++)
      PrimaryBins[i] = Bins1[i];
   double Delta = PrimaryBins[PrimaryBinCount-1] - PrimaryBins[1];

   if(PrimaryBins[0] < -9998)
      PrimaryBins[0] = PrimaryBins[1] - Delta * 0.05;
   if(PrimaryBins[PrimaryBinCount] > 9998)
      PrimaryBins[PrimaryBinCount] = PrimaryBins[PrimaryBinCount-1] + Delta;
   if(PrimaryBins[0] < 0 && PrimaryBins[1] > 0)
      PrimaryBins[0] = 0;

   for(int iB = 0; iB < BinningCount; iB++)
   {
      for(int i = 0; i < PrimaryBinCount; i++)
      {
         double X = (PrimaryBins[i] + PrimaryBins[i+1]) / 2;
         double DX = (PrimaryBins[i+1] - PrimaryBins[i]) / 2;
         double Y = 0, DY = 0;

         if(H2 == nullptr)
         {
            Y = H->GetBinContent(i + iB * PrimaryBinCount + 1) * ExtraScale;
            DY = H->GetBinError(i + iB * PrimaryBinCount + 1) * ExtraScale;
         }
         else
         {
            double YUp = H->GetBinContent(i + iB * PrimaryBinCount + 1) * ExtraScale;
            double YDown = H2->GetBinContent(i + iB * PrimaryBinCount + 1) * ExtraScale;

            // cout << iB << " " << i << " " << YUp << " " << YDown << endl;

            Y = (YUp + YDown) / 2;
            DY = fabs(YUp - YDown) / 2;
         }

         double Width = PrimaryBins[i+1] - PrimaryBins[i];
         if(Normalize == false)
            Width = 1;

         Result[iB].SetPoint(i, X, Y / Width);
         Result[iB].SetPointError(i, DX, DX, DY / Width, DY / Width);
      }
   }

   return Result;
}

TGraphAsymmErrors CalculateRatio(TGraphAsymmErrors &G1, TGraphAsymmErrors &G2, string DHFileName, string DHState)
{
   TGraphAsymmErrors G;

   if(G1.GetN() != G2.GetN())
      return G;

   DataHelper DHFile(DHFileName);

   bool NoReduction;
   vector<double> BinBoundary;
   if(DHFile.Exist(DHState) == true)
   {
      NoReduction = false;
      BinBoundary = ParseList(DHFile[DHState]["PT"].GetString());
   }
   else
      NoReduction = true;

   // cout << G1.GetN() << " " << G2.GetN() << endl;

   int N = G2.GetN();
   for(int i = 0; i < N; i++)
   {
      double X1, X2, Y1, Y2, E1YH, E2YH, E1YL, E2YL, E1XH, E1XL;

      G1.GetPoint(i, X1, Y1);
      E1YH = G1.GetErrorYhigh(i);
      E1YL = G1.GetErrorYlow(i);
      E1XH = G1.GetErrorXhigh(i);
      E1XL = G1.GetErrorXlow(i);
      G2.GetPoint(i, X2, Y2);
      E2YH = G2.GetErrorYhigh(i);
      E2YL = G2.GetErrorYlow(i);

      double Rho = 0;
      if(NoReduction == false)
      {
         int Bin = GetBin(X1, BinBoundary);
         Rho = DHFile[DHState]["Rho"+to_string(Bin)].GetDouble();
      }

      double REL1 = E1YL / Y1;
      double REL2 = E2YL / Y2;
      double REL = sqrt(REL1 * REL1 + REL2 * REL2 - 2 * REL1 * REL2 * Rho);
      double EL = REL * (Y1 / Y2);
      double REH1 = E1YL / Y1;
      double REH2 = E2YL / Y2;
      double REH = sqrt(REH1 * REH1 + REH2 * REH2 - 2 * REH1 * REH2 * Rho);
      double EH = REH * (Y1 / Y2);

      if(Y2 == 0)
      {
         G.SetPoint(i, X1, 0);
         G.SetPointError(i, E1XL, E1XH, 0, 0);
      }
      else
      {
         G.SetPoint(i, X1, Y1 / Y2);
         G.SetPointError(i, E1XL, E1XH, EL, EH);
      }
   }

   return G;
}

TGraphAsymmErrors BuildSystematics(TGraphAsymmErrors &G, string FileName)
{
   int N = G.GetN();
   TGraphAsymmErrors Result(N);

   TFile File(FileName.c_str());

   TH1D *HMin = (TH1D *)File.Get("HGenPrimaryBinMin");
   TH1D *HMax = (TH1D *)File.Get("HGenPrimaryBinMax");

   TH1D *HPlus = (TH1D *)File.Get("HTotalPlus");
   TH1D *HMinus = (TH1D *)File.Get("HTotalMinus");

   bool FileOK = true;
   if(HMin == nullptr)     FileOK = false;
   if(HMax == nullptr)     FileOK = false;
   if(HPlus == nullptr)    FileOK = false;
   if(HMinus == nullptr)   FileOK = false;

   if(FileOK == false && FileName != "none")
   {
      cout << "Systematic file \"" << FileName << "\" not good!" << endl;
      cout << "Proceed without uncertainty..." << endl;
   }

   for(int i = 0; i < N; i++)
   {
      double X = G.GetPointX(i);
      double Y = G.GetPointY(i);
      double EXL = G.GetErrorXlow(i);
      double EXH = G.GetErrorXhigh(i);

      double EYL = 0;
      double EYH = 0;

      if(FileOK == true)
      {
         for(int j = 1; j <= HMin->GetNbinsX(); j++)
         {
            if(X >= HMin->GetBinContent(j) && X < HMax->GetBinContent(j))
            {
               EYL = HPlus->GetBinContent(j) * Y;
               EYH = -HMinus->GetBinContent(j) * Y;
               break;
            }
         }
      }

      Result.SetPoint(i, X, Y);
      Result.SetPointError(i, EXL, EXH, EYL, EYH);
   }

   File.Close();

   return Result;
}

TGraph ToGraph(TH1D *H)
{
   if(H == nullptr)
      return TGraph();

   TGraph Result;

   for(int i = 1; i <= H->GetNbinsX(); i++)
      Result.SetPoint(i - 1, H->GetBinCenter(i), H->GetBinContent(i));

   return Result;
}

int GetBin(double Value, vector<double> &BinBoundary)
{
   vector<double>::iterator iter = upper_bound(BinBoundary.begin(), BinBoundary.end(), Value);
   return (iter - BinBoundary.begin());
}

