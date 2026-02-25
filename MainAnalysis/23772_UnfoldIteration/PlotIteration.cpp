#include <iostream>
#include <cmath>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"

#include "DataHelper.h"
#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
//#include "RootUtilities.h"

#include "BinHelper.h"

int main(int argc, char *argv[]);
double CalculateChi2(TH1D *H1, TH1D *H2, int IgnoreBin = 0, bool UseError = true);
double CalculateE(TH1D *H, int IgnoreBin = 0, bool Relative = false, double Power = 1);
void SetMinMax(TGraph &G);

int main(int argc, char *argv[])
{
  //SilenceRoot();

   SetThumbStyle();

   CommandLine CL(argc, argv);

   string InputFileName   = CL.Get("Input");
   string OutputFileName  = CL.Get("Output");
   string DHFileName      = CL.Get("DHFile", "GlobalSetting.dh");
   int PointsToIgnore     = CL.GetInt("Ignore");
   int RecoPointsToIgnore = CL.GetInt("IgnoreReco", 10);
   string Reference       = CL.Get("Reference", "HMCTruth");
   string State           = CL.Get("State");
   string Key             = CL.Get("Key");
   double DPower          = CL.GetDouble("DPower", 1.7);

   vector<int> Iteration = ListIterations(InputFileName);
   // {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 68, 70, 80, 90, 100, 125, 150, 175, 200, 250, 300, 350, 400, 450, 500};

   TGraph GraphFChi2;
   TGraph GraphFD2;
   TGraph GraphChi2;
   TGraph GraphD2;
   TGraph GraphWD;
   TGraph GraphE;
   TGraph GraphE2;
   TGraph GraphRE;
   TGraph GraphRE2;
   TGraph GraphM;

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Iteration scan");

   TFile InputFile(InputFileName.c_str());

   TH1D *HMCMeasured = (TH1D *)InputFile.Get("HMCTruth");
   TH1D *HInput = (TH1D *)InputFile.Get("HInput");
   double ScalingFactor = HInput->Integral() / HMCMeasured->Integral();

   TH1D *HMCTruth = (TH1D *)InputFile.Get(Reference.c_str());
   HMCTruth->Scale(ScalingFactor);
   HMCTruth->SetStats(0);
   HMCTruth->SetLineColor(kBlack);
   HMCTruth->SetLineWidth(2);

   TH2D *HResponse = (TH2D *)InputFile.Get("HMCResponse");

   double BestMetric = -1;
   int BestIteration = -1;

   for(int i : Iteration)
   {
      TH1D *HUnfolded = (TH1D *)InputFile.Get(Form("HUnfoldedBayes%d", i));
      if(HUnfolded == nullptr)
         continue;

      // double Scale = HInput->Integral(RecoPointsToIgnore, -1) / HFolded->Integral(RecoPointsToIgnore, -1);
      double Scale = HMCTruth->Integral(PointsToIgnore, -1) / HUnfolded->Integral(PointsToIgnore, -1);
      // HUnfolded->Scale(Scale);
      
      TH1D *HFolded = ForwardFold(HUnfolded, HResponse);

      double FChi2 = CalculateChi2(HInput, HFolded, RecoPointsToIgnore, true);
      double FD2   = CalculateChi2(HInput, HFolded, RecoPointsToIgnore, false);
      double Chi2  = CalculateChi2(HMCTruth, HUnfolded, PointsToIgnore, true);
      double D2    = CalculateChi2(HMCTruth, HUnfolded, PointsToIgnore, false);

      if(FChi2 > 0 && isinf(FChi2) == false)
         GraphFChi2.SetPoint(GraphFChi2.GetN(), i, FChi2);
      if(FD2 > 0 && isinf(FD2) == false)
         GraphFD2.SetPoint(GraphFD2.GetN(), i, FD2);
      if(Chi2 > 0 && isinf(Chi2) == false)
         GraphChi2.SetPoint(GraphChi2.GetN(), i, Chi2);
      if(D2 > 0 && isinf(D2) == false)
         GraphD2.SetPoint(GraphD2.GetN(), i, D2);
      
      double E   = CalculateE(HUnfolded, PointsToIgnore, false, 1);
      double E2  = CalculateE(HUnfolded, PointsToIgnore, false, 2);
      double RE  = CalculateE(HUnfolded, PointsToIgnore, true, 1);
      double RE2 = CalculateE(HUnfolded, PointsToIgnore, true, 2);
      GraphE.SetPoint(GraphE.GetN(), i, E);
      GraphE2.SetPoint(GraphE2.GetN(), i, E2);
      GraphRE.SetPoint(GraphRE.GetN(), i, RE);
      GraphRE2.SetPoint(GraphRE2.GetN(), i, RE2);

      TCanvas Canvas;
      HMCTruth->SetTitle(Form("Iteration %d [%.2f]", i, Scale));
      HMCTruth->Draw("hist");
      HUnfolded->SetMarkerStyle(20);
      HUnfolded->Draw("same");
      PdfFile.AddCanvas(Canvas);
      Canvas.SetLogy();
      PdfFile.AddCanvas(Canvas);
      
      HInput->SetTitle(Form("Folded iteration %d", i));
      HInput->Draw("hist");
      HFolded->SetMarkerStyle(20);
      HFolded->Draw("same");
      Canvas.SetLogy(false);
      PdfFile.AddCanvas(Canvas);
      Canvas.SetLogy();
      PdfFile.AddCanvas(Canvas);

      TH1D *HTruthPower = (TH1D *)HMCTruth->Clone("TruthPower");
      TH1D *HUnfoldedPower = (TH1D *)HUnfolded->Clone("UnfoldedPower");

      double Power = 1;
      for(int j = 1; j <= HTruthPower->GetNbinsX(); j++)
      {
         HTruthPower->SetBinContent(j, HTruthPower->GetBinContent(j) * Power);
         HTruthPower->SetBinError(j, HTruthPower->GetBinError(j) * Power);
         HUnfoldedPower->SetBinContent(j, HUnfoldedPower->GetBinContent(j) * Power);
         HUnfoldedPower->SetBinError(j, HUnfoldedPower->GetBinError(j) * Power);
         Power = Power * DPower;
      }

      HTruthPower->SetTitle(Form("Iteration %d power", i));
      HTruthPower->Draw("hist");
      HUnfoldedPower->SetMarkerStyle(20);
      HUnfoldedPower->Draw("same");
      // PdfFile.AddCanvas(Canvas);
      // Canvas.SetLogy();
      // PdfFile.AddCanvas(Canvas);
      
      double WD2 = CalculateChi2(HTruthPower, HUnfoldedPower, PointsToIgnore, false);
      if(WD2 > 0 && isinf(WD2) == false)
         GraphWD.SetPoint(GraphWD.GetN(), i, WD2);

      double Metric = FChi2 + Chi2 + E;
      if(Metric > 0 && isinf(Metric) == false)
      {
         if(i > 1 && (BestMetric < 0 || BestMetric > Metric))
         {
            BestMetric = Metric;
            BestIteration = i;
         }
         GraphM.SetPoint(GraphM.GetN(), i, Metric);
      }
   }

   SetMinMax(GraphFChi2);
   SetMinMax(GraphFD2);
   SetMinMax(GraphChi2);
   SetMinMax(GraphD2);
   SetMinMax(GraphWD);

   InputFile.Close();

   // cout << "!" << endl;

   DataHelper DHFile(DHFileName);
   DHFile[State][Key] = BestIteration;
   DHFile.SaveToFile();

   PdfFile.AddTextPage("Summary plots");

   GraphFChi2.SetNameTitle("GFChi2", "");
   GraphFD2.SetNameTitle("GFD2", "");
   GraphChi2.SetNameTitle("GChi2", "");
   GraphD2.SetNameTitle("GD2", "");
   GraphE.SetNameTitle("GE", "");
   GraphE2.SetNameTitle("GE2", "");
   GraphRE.SetNameTitle("GRE", "");
   GraphRE2.SetNameTitle("GRE2", "");
   GraphWD.SetNameTitle("GWD2", "");
   GraphM.SetNameTitle("GM", "");

   GraphFChi2.GetXaxis()->SetTitle("Number of iterations");
   GraphFChi2.GetYaxis()->SetTitle("#sum #chi^{2} (folded)");
   GraphFD2.GetXaxis()->SetTitle("Number of iterations");
   GraphFD2.GetYaxis()->SetTitle("#sum Distance^{2} (folded)");
   GraphChi2.GetXaxis()->SetTitle("Number of iterations");
   GraphChi2.GetYaxis()->SetTitle("#sum #chi^{2}");
   GraphD2.GetXaxis()->SetTitle("Number of iterations");
   GraphD2.GetYaxis()->SetTitle("#sum Distance^{2}");
   GraphE.GetXaxis()->SetTitle("Number of iterations");
   GraphE.GetYaxis()->SetTitle("#sum Error");
   GraphE2.GetXaxis()->SetTitle("Number of iterations");
   GraphE2.GetYaxis()->SetTitle("#sum Error^{2}");
   GraphRE.GetXaxis()->SetTitle("Number of iterations");
   GraphRE.GetYaxis()->SetTitle("#sum Relative error");
   GraphRE2.GetXaxis()->SetTitle("Number of iterations");
   GraphRE2.GetYaxis()->SetTitle("#sum Relative error^{2}");
   GraphWD.GetXaxis()->SetTitle("Number of iterations");
   GraphWD.GetYaxis()->SetTitle("#sum Weighted distance^{2}");
   GraphM.GetXaxis()->SetTitle("Number of iterations");
   GraphM.GetYaxis()->SetTitle("Chosen metric");

   PdfFile.AddPlot(GraphFChi2, "apl");
   PdfFile.AddPlot(GraphFChi2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphFChi2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphFD2, "apl");
   PdfFile.AddPlot(GraphFD2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphFD2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphChi2, "apl");
   PdfFile.AddPlot(GraphChi2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphChi2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphD2, "apl");
   PdfFile.AddPlot(GraphD2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphD2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphE, "apl");
   PdfFile.AddPlot(GraphE, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphE, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphE2, "apl");
   PdfFile.AddPlot(GraphE2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphE2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphRE, "apl");
   PdfFile.AddPlot(GraphRE, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphRE, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphRE2, "apl");
   PdfFile.AddPlot(GraphRE2, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphRE2, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphWD, "apl");
   PdfFile.AddPlot(GraphWD, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphWD, "apl", true, false, true, true);
   PdfFile.AddPlot(GraphM, "apl");
   PdfFile.AddPlot(GraphM, "apl", false, false, true, true);
   PdfFile.AddPlot(GraphM, "apl", true, false, true, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

double CalculateChi2(TH1D *H1, TH1D *H2, int IgnoreBin, bool UseError)
{
   if(H1 == nullptr)
      return 0;
   if(H2 == nullptr)
      return 0;

   int N = H1->GetNbinsX();

   double Chi2 = 0;

   for(int i = IgnoreBin + 1; i <= N; i++)
   {
      double V1 = H1->GetBinContent(i);
      double V2 = H2->GetBinContent(i);
      double E1 = H1->GetBinError(i);
      double E2 = H2->GetBinError(i);

      double D = V1 - V2;
      double Chi = (V1 - V2) / sqrt(E1 * E1 + E2 * E2);
      if(V1 == 0 && V2 == 0)
         continue;

      if(UseError == true)
         Chi2 = Chi2 + Chi * Chi;
      else
         Chi2 = Chi2 + D * D;
   }

   if(Chi2 != Chi2)
      return 0;

   return Chi2;
}

double CalculateE(TH1D *H, int IgnoreBin, bool Relative, double Power)
{
   if(H == nullptr)
      return 0;

   int N = H->GetNbinsX();

   double SumE = 0;

   for(int i = IgnoreBin + 1; i <= N; i++)
   {
      double V = H->GetBinContent(i);
      double E = H->GetBinError(i);

      if(Relative == true)
      {
         if(V == 0)
            E = 0;
         else
            E = E / V;
      }

      E = pow(E, Power);
      SumE = SumE + E;
   }

   return SumE;

}

void SetMinMax(TGraph &G)
{
   double Min, Max;

   int N = G.GetN();
   for(int i = 0; i < N; i++)
   {
      double Y = G.GetPointY(i);
    
      if(i == 0)
      {
         Min = Y;
         Max = Y;
         continue;
      }

      if(Min > Y)   Min = Y;
      if(Max < Y)   Max = Y;
   }

   if(Min != Min)   Min = 1;
   if(Max != Max)   Max = 9999;
   if(Min == Max)   Max = Min + 1;

   G.GetYaxis()->SetRangeUser(Min, Max);
}


