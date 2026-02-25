#include <iostream>
#include <map>
using namespace std;

#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"

#include "CommandLine.h"
#include "DataHelper.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
pair<double, double> GetMinMax(TGraphAsymmErrors *G);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string InputFileName  = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   string DHFileName     = CL.Get("DHFile");
   string State          = CL.Get("State", "RhoWeight");
   string KeyBase        = CL.Get("Key");
   int Order1            = CL.GetInteger("Order1", 8);
   int Order2            = CL.GetInteger("Order2", 0);
   bool PinSmall         = CL.GetBool("PinSmall", false);
   bool PinLarge         = CL.GetBool("PinLarge", true);

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Fit");

   TFile File(InputFileName.c_str());

   TGraphAsymmErrors *GRatio = (TGraphAsymmErrors *)File.Get("GRatio");
   TGraphAsymmErrors *GRatioLog = (TGraphAsymmErrors *)File.Get("GRatioLog");

   if(GRatioLog == nullptr)
      return -1;

   if(PinSmall == true)
   {
      int N = GRatioLog->GetN();
      double X = GRatioLog->GetPointX(N - 1); // - GRatioLog->GetErrorXlow(N - 1);
      double Y = GRatioLog->GetPointY(N - 1);
      GRatioLog->SetPoint(N, X * 0.7, Y - 5);
   }
   if(PinLarge == true)
   {
      int N = GRatioLog->GetN();
      double X = GRatioLog->GetPointX(0); // + GRatioLog->GetErrorXhigh(0);
      double Y = GRatioLog->GetPointY(0);
      GRatioLog->SetPoint(N, X * 1.1, Y - 5);
   }

   string Formula = "([0]";
   for(int i = 1; i <= Order1; i++)
   {
      Formula = Formula + "+[" + i + "]";
      for(int j = 0; j < i; j++)
         Formula = Formula + "*x";
   }
   Formula = Formula + ")/(1";
   for(int i = 1; i <= Order2; i++)
   {
      Formula = Formula + "+[" + (i + Order1) + "]";
      for(int j = 0; j < i; j++)
         Formula = Formula + "*x";
   }
   Formula = Formula + ")";

   pair<double, double> MinMax = GetMinMax(GRatio);

   TF1 FLog("FLog", Formula.c_str(), 0, 1500);
   for(int i = 0; i < 10; i++)
      GRatioLog->Fit(&FLog, "W");

   TF1 F("F", Form("exp(%s)", Formula.c_str()), 0, 1500);
   for(int i = 0; i <= Order1 + Order2; i++)
   {
      F.SetParameter(i, FLog.GetParameter(i));
      F.SetParError(i, FLog.GetParError(i));
   }

   int Extra1 = Order1 + Order2 + 1;
   int Extra2 = Order1 + Order2 + 2;
   string ExtendedFormula =
      Form("(x<[%d])*(-999)+(x>[%d])*(-999)+(x>=[%d]&&x<=[%d])*(%s)", Extra1, Extra2, Extra1, Extra2,
         Formula.c_str());

   TF1 F2("F2", Form("exp(%s)", ExtendedFormula.c_str()), 0, 1500);
   for(int i = 0; i <= Order1 + Order2; i++)
   {
      F2.SetParameter(i, FLog.GetParameter(i));
      F2.SetParError(i, FLog.GetParError(i));
   }
   F2.SetParameter(Extra1, MinMax.first * 0.7);
   F2.SetParameter(Extra2, MinMax.second * 1.1);

   DataHelper DHFile(DHFileName);

   DHFile[State][KeyBase+"_RawFormula"] = "exp(" + Formula + ")";
   DHFile[State][KeyBase+"_Formula"] = "exp(" + ExtendedFormula + ")";
   DHFile[State][KeyBase+"_RawNPar"] = Order1 + Order2 + 1;
   DHFile[State][KeyBase+"_NPar"] = Order1 + Order2 + 1 + 2;
   for(int i = 0; i <= Order1 + Order2; i++)
   {
      DHFile[State][KeyBase+"_P"+to_string(i)] = F.GetParameter(i);
      DHFile[State][KeyBase+"_E"+to_string(i)] = F.GetParError(i);
   }
   DHFile[State][KeyBase+"_P"+to_string(Order1+Order2+1)] = MinMax.first * 0.7;
   DHFile[State][KeyBase+"_P"+to_string(Order1+Order2+2)] = MinMax.second * 1.1;
   DHFile[State][KeyBase+"_MinUE"] = MinMax.first;
   DHFile[State][KeyBase+"_MaxUE"] = MinMax.second;

   DHFile.SaveToFile();

   TCanvas Canvas;
   gStyle->SetOptFit(0);  // turn off stat box

   GRatioLog->Draw("ap");
   GRatioLog->GetXaxis()->SetTitle("#frac{#Sigma W_{i}*UE}{#Sigma W_{i}}");
   GRatioLog->GetXaxis()->SetTitleSize(0.025);
   GRatioLog->GetXaxis()->SetTitleOffset(1.15);
   GRatioLog->GetYaxis()->SetTitle("#frac{(#Sigma W_{i} / #Delta UE)_{Data}}{(#Sigma W_{i} / #Delta UE)_{MC}}");
   GRatioLog->GetYaxis()->SetTitleSize(0.03);
   GRatioLog->GetYaxis()->SetTitleOffset(1.10);
   //FLog.Draw("same");
   Canvas.SetLogy(false);
   PdfFile.AddCanvas(Canvas);
   
   GRatio->GetXaxis()->SetTitle("#frac{#Sigma W_{i}*UE}{#Sigma W_{i}}");
   GRatio->GetYaxis()->SetTitle("#frac{(#Sigma W_{i} / #Delta UE)_{Data}}{(#Sigma W_{i} / #Delta UE)_{MC}}");
   GRatio->GetXaxis()->SetTitleSize(0.025);
   GRatio->GetXaxis()->SetTitleOffset(1.15);
   GRatio->GetYaxis()->SetTitleSize(0.03);
   GRatio->GetYaxis()->SetTitleOffset(1.10);
   GRatio->Draw("ap");
   F.Draw("same");
   Canvas.SetLogy(false);
   PdfFile.AddCanvas(Canvas);
   
   GRatio->GetXaxis()->SetTitle("#frac{#Sigma W_{i}*UE}{#Sigma W_{i}}");
   GRatio->GetYaxis()->SetTitle("#frac{(#Sigma W_{i} / #Delta UE)_{Data}}{(#Sigma W_{i} / #Delta UE)_{MC}}");
   GRatio->GetXaxis()->SetTitleSize(0.025);
   GRatio->GetXaxis()->SetTitleOffset(1.15);
   GRatio->GetYaxis()->SetTitleSize(0.03);
   GRatio->GetYaxis()->SetTitleOffset(1.10);
   GRatio->Draw("ap");
   F.Draw("same");
   Canvas.SetLogy(true);
   PdfFile.AddCanvas(Canvas);

   TGraph GF;
   for(double X = 0; X < 1500; X = X + 0.1)
   {
      double Y = F.Eval(X);
      if(Y != Y || Y > 10000)
         Y = 10000;
      GF.SetPoint(GF.GetN(), X, Y);
   }
   GF.Draw("ap");
   Canvas.SetLogy(false);
   PdfFile.AddCanvas(Canvas);

   TGraph GF2;
   for(double X = 0; X < 1500; X = X + 0.1)
   {
      double Y = F2.Eval(X);
      if(Y != Y || Y > 10000)
         Y = 10000;
      GF2.SetPoint(GF2.GetN(), X, Y);
   }
   GF2.Draw("ap");
   Canvas.SetLogy(false);
   PdfFile.AddCanvas(Canvas);

   File.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

pair<double, double> GetMinMax(TGraphAsymmErrors *G)
{
   if(G == nullptr)
      return pair<double, double>{0, 0};

   pair<double, double> MinMax{9999, -9999};

   for(int i = 0; i < G->GetN(); i++)
   {
      double X, Y;
      G->GetPoint(i, X, Y);
      double EXL, EXH;
      EXL = G->GetErrorXlow(i);
      EXH = G->GetErrorXhigh(i);

      if(X - EXL < MinMax.first)    MinMax.first = X - EXL;
      if(X + EXH > MinMax.second)   MinMax.second = X + EXH;
   }

   if(MinMax.first < 2)
      MinMax.first = 0;

   return MinMax;
}


