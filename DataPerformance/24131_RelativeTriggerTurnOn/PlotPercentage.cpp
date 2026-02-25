#include <iostream>
using namespace std;

#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TMath.h"

#include "SetStyle.h"
#include "CommandLine.h"
#include "DataHelper.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[]);
void AppendPlot(PdfFileHelper &PdfFile, DataHelper &DHFile, string State, double Percentage);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string DHFileName = CL.Get("DHFile", "GlobalSettingTest.dh");
   string State      = CL.Get("State",  "TriggerTurnOn");
   string Output     = CL.Get("Output", "Summary.pdf");

   DataHelper DHFile(DHFileName);

   PdfFileHelper PdfFile(Output);
   PdfFile.AddTextPage("Percentage location");

   AppendPlot(PdfFile, DHFile, State, 0.90);
   AppendPlot(PdfFile, DHFile, State, 0.95);
   AppendPlot(PdfFile, DHFile, State, 0.97);
   AppendPlot(PdfFile, DHFile, State, 0.975);
   AppendPlot(PdfFile, DHFile, State, 0.98);
   AppendPlot(PdfFile, DHFile, State, 0.99);
   AppendPlot(PdfFile, DHFile, State, 0.995);
   AppendPlot(PdfFile, DHFile, State, 0.999);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void AppendPlot(PdfFileHelper &PdfFile, DataHelper &DHFile, string State, double Percentage)
{
  // vector<string> R{"1", "2", "3", "4", "5", "6", "7", "8", "9"};
   vector<string> R{"1", "2", "3", "4", "6", "7", "8", "9"};
   vector<string> C{"0to10", "10to30", "30to50", "50to90", "Inclusive"};
   //vector<string> RLabel{"0.15", "0.20", "0.30", "0.40", "0.50", "0.60", "0.80", "1.00", "1.20"};
   vector<string> RLabel{"0.15", "0.20", "0.30", "0.40", "0.60", "0.80", "1.00", "1.20"};
   vector<string> CLabel{"0-10", "10-30", "30-50", "50-90", "pp"};

   TH2D H(Form("H%f", Percentage), Form("P = %.1f%%", Percentage * 100), 9, 0, 9, 5, 0, 5);
   H.SetStats(0);
   //for(int iR = 0; iR < 9; iR++)
   for(int iR = 0; iR < 8; iR++)
      H.GetXaxis()->SetBinLabel(iR + 1, RLabel[iR].c_str());
   for(int iC = 0; iC < 5; iC++)
      H.GetYaxis()->SetBinLabel(iC + 1, CLabel[iC].c_str());

   //for(int iR = 0; iR < 9; iR++)
   for(int iR = 0; iR < 8; iR++)
   {
      for(int iC = 0; iC < 5; iC++)
      {
         string KeyPrefix = "R" + R[iR] + "_Centrality" + C[iC];

         double P0 = DHFile[State][KeyPrefix+"_P0"].GetDouble();
         double P1 = DHFile[State][KeyPrefix+"_P1"].GetDouble();
         double P2 = DHFile[State][KeyPrefix+"_P2"].GetDouble();

         int FunctionChoice = DHFile[State][KeyPrefix+"_FunctionChoice"].GetInteger();

         double Value = atanh(1 + (Percentage - 1) / P0) * P2 + P1;
         if(FunctionChoice == 1)
            Value = TMath::ErfInverse(1 + (Percentage - 1) / P0) * P2 + P1;

         H.SetBinContent(iR + 1, iC + 1, Value);
      }
   }

   H.SetMinimum(100);

   PdfFile.AddPlot(H, "colz text30");
}




