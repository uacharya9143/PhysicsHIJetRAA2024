#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"

#include "CommandLine.h"
#include "DataHelper.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
vector<double> ParseList(string List);

int main(int argc, char *argv[])
{
   SetThesisStyle();
   vector<int> Colors = GetPrimaryColors();

   CommandLine CL(argc, argv);

   string FileName      = CL.Get("Input");
   string PassName      = CL.Get("Pass", "HTrigger");
   string AllName       = CL.Get("All", "HAll");
   string Output        = CL.Get("Output");
   string DHFileName    = CL.Get("DHFile", "GlobalSetting.dh");
   string State         = CL.Get("State", "TriggerTurnOn");
   string KeyPrefix     = CL.Get("KeyPrefix", "Fit");
   int FunctionChoice   = CL.GetInt("FunctionChoice", 0);
   double CutPercentage = CL.GetDouble("CutPercentage", 0.99);
   
   PdfFileHelper PdfFile(Output);
   PdfFile.AddTextPage("Trigger fit");

   TFile File(FileName.c_str());

   TH1D *HPass = (TH1D *)File.Get(PassName.c_str());
   TH1D *HAll = (TH1D *)File.Get(AllName.c_str());

   TGraphAsymmErrors G;
   G.Divide(HPass, HAll);

   string Function = "1+[0]*(tanh((x-[1])/[2])-1)";
   if(FunctionChoice == 1)
      Function = "1+[0]*(TMath::Erf((x-[1])/[2])-1)";

   //TF1 F("F", Function.c_str(), 0, 500); //disabled by uttam to check the fit range
   TF1 F("F", Function.c_str(), 0, 900);
   F.SetParameters(0.5, 80, 20);

   //G.Fit(&F, "w", "", 80, 500); //disabled by uttam to check the fit range
   //G.Fit(&F, "", "", 80, 500); //disabled by uttam to check the fit range
   G.Fit(&F, "w", "", 80, 900);
   G.Fit(&F, "", "", 80, 900);

   HAll->SetLineColor(Colors[0]);
   HPass->SetLineColor(Colors[1]);

   TCanvas Canvas;
   HAll->Draw();
   HPass->Draw("same");
   PdfFile.AddCanvas(Canvas);

   PdfFile.AddPlot(G, "ap");

   double P0 = F.GetParameter(0);
   double P1 = F.GetParameter(1);
   double P2 = F.GetParameter(2);

   DataHelper DHFile(DHFileName);
   
   double CutLocation = 0;

   DHFile[State][KeyPrefix+"_FunctionChoice"] = FunctionChoice;
   DHFile[State][KeyPrefix+"_NParameter"] = 3;
   DHFile[State][KeyPrefix+"_Formula"] = Function;
   DHFile[State][KeyPrefix+"_P0"] = F.GetParameter(0);
   DHFile[State][KeyPrefix+"_P1"] = F.GetParameter(1);
   DHFile[State][KeyPrefix+"_P2"] = F.GetParameter(2);
   DHFile[State][KeyPrefix+"_E0"] = F.GetParError(0);
   DHFile[State][KeyPrefix+"_E1"] = F.GetParError(1);
   DHFile[State][KeyPrefix+"_E2"] = F.GetParError(2);
   if(FunctionChoice == 1)
   {
      DHFile[State][KeyPrefix+"_X90"]   = TMath::ErfInverse(1 + (0.90 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X95"]   = TMath::ErfInverse(1 + (0.95 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X98"]   = TMath::ErfInverse(1 + (0.98 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X99"]   = TMath::ErfInverse(1 + (0.99 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X99.9"] = TMath::ErfInverse(1 + (0.999 - 1) / P0) * P2 + P1;
      CutLocation                       = TMath::ErfInverse(1 + (CutPercentage - 1) / P0) * P2 + P1;
   }
   else
   {
      DHFile[State][KeyPrefix+"_X90"]   = atanh(1 + (0.90 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X95"]   = atanh(1 + (0.95 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X98"]   = atanh(1 + (0.98 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X99"]   = atanh(1 + (0.99 - 1) / P0) * P2 + P1;
      DHFile[State][KeyPrefix+"_X99.9"] = atanh(1 + (0.999 - 1) / P0) * P2 + P1;
      CutLocation                       = atanh(1 + (CutPercentage - 1) / P0) * P2 + P1;
   }

   DHFile[State][KeyPrefix+"_Cut"] = CutLocation;

   vector<double> RecoBins = ParseList(DHFile["Binning"]["GenPT"].GetString());
   int NumberSmaller = 0;
   for(double Value : RecoBins)
      if(Value < CutLocation)
         NumberSmaller = NumberSmaller + 1;
   DHFile["Binning"]["PTUnderflow_"+KeyPrefix] = NumberSmaller;
   DHFile["Binning"]["PTOverflow_"+KeyPrefix] = 0;

   DHFile.SaveToFile();

   File.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

vector<double> ParseList(string List)
{
   vector<double> Result;

   for(char &c : List)
      if(c == ',')
         c = ' ';

   stringstream str(List);

   while(str)
   {
      double Temp = -1;
      str >> Temp;
      if(Temp > 0)
         Result.push_back(Temp);
   }
   
   return Result;
}



