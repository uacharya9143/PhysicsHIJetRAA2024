#include <iostream>
#include <vector>
#include <map>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "DataHelper.h"
#include "TauHelperFunctions3.h"
#include "DrawRandom.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "CustomAssert.h"
//#include "RootUtilities.h"

#include "BinHelper.h"
#include "Messenger.h"
#include "JetCorrector.h"

int main(int argc, char *argv[]);
int GetBin(double Value, vector<double> &BinBoundary);
void VisualizeVector(PdfFileHelper &PdfFile, vector<double> &V, string Title);
void VisualizeMatrix(PdfFileHelper &PdfFile, vector<vector<double>> &M, string Title);
vector<double> DoSampling(PdfFileHelper &PdfFile, int NSample,
   vector<double> &M01, vector<double> &M10, vector<vector<double>> &M11,
   vector<double> &M01E, vector<double> &M10E, vector<vector<double>> &M11E);

int main(int argc, char *argv[])
{
  //SilenceRoot();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   string Tree1Name              = CL.Get("Tree1");
   string Tree2Name              = CL.Get("Tree2");
   string DHFileName             = CL.Get("DHFile");
   double MatchAngle             = CL.GetDouble("MatchAngle");
   double EtaMin                 = CL.GetDouble("EtaMin");
   double EtaMax                 = CL.GetDouble("EtaMax");
   string OutputFileName         = CL.Get("Output");
   int NSample                   = CL.GetInt("NSample", 100000);
   bool UseGen                   = CL.GetBool("UseGen", true);
   
   bool DoJEC                    = CL.GetBool("DoJEC", !UseGen);
   vector<string> JEC1FileNames  = ((DoJEC == true) ? CL.GetStringVector("JEC1") : vector<string>{});
   vector<string> JEC2FileNames  = ((DoJEC == true) ? CL.GetStringVector("JEC2") : vector<string>{});

   JetCorrector JEC1(JEC1FileNames);
   JetCorrector JEC2(JEC2FileNames);

   string OutputDHFileName       = CL.Get("DHOutput");
   string OutputDHState          = CL.Get("DHState");

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Uncertainty Cancellation");
   PdfFile.AddTextPage({
      "Tree 1 = " + Tree1Name,
      "Tree 2 = " + Tree2Name,
      "Matching angle = " + to_string(MatchAngle),
      "Eta = [" + to_string(EtaMin) + ", " + to_string(EtaMax) + "]"
   });

   bool IdenticalMode = false;
   if(Tree1Name == Tree2Name)
      IdenticalMode = true;

   DataHelper DHFile(DHFileName);

   vector<double> BinBoundary = ParseList(DHFile["Binning"]["GenPT"].GetString());
   int BinCount = BinBoundary.size() + 1;
   
   double PTMin = DHFile["Binning"]["GenMin"].GetDouble();

   vector<double> M10(BinCount), M10E(BinCount);
   vector<double> M01(BinCount), M01E(BinCount);
   vector<vector<double>> M11(BinCount), M11E(BinCount);
   for(vector<double> &V : M11)
      V.resize(BinCount, 0);
   for(vector<double> &V : M11E)
      V.resize(BinCount, 0);

   for(string FileName : InputFileNames)
   {
      TFile File(FileName.c_str());

      HiEventTreeMessenger MEvent(File);
      JetTreeMessenger MJet1(File, Tree1Name);
      JetTreeMessenger MJet2(IdenticalMode == true ? nullptr : (TTree *)File.Get(Tree2Name.c_str()));

      if(MEvent.Tree == nullptr)   continue;
      if(MJet1.Tree == nullptr)    continue;
      if(IdenticalMode == false && MJet2.Tree == nullptr)    continue;

      int EntryCount = MEvent.Tree->GetEntries();
      for(int iE = 0; iE < EntryCount; iE++)
      {
         MEvent.GetEntry(iE);
         MJet1.GetEntry(iE);
         if(IdenticalMode == false)
            MJet2.GetEntry(iE);

         vector<FourVector> Jet1;
         vector<FourVector> Jet2;
         vector<pair<FourVector, FourVector>> JetM;

         double Weight = 1;

         int NJet1 = (UseGen == true) ? MJet1.GenCount : MJet1.JetCount;
         for(int iJ1 = 0; iJ1 < NJet1; iJ1++)
         {
            double PT  = (UseGen == true) ? MJet1.GenPT[iJ1]  : MJet1.JetRawPT[iJ1];
            double Eta = (UseGen == true) ? MJet1.GenEta[iJ1] : MJet1.JetEta[iJ1];
            double Phi = (UseGen == true) ? MJet1.GenPhi[iJ1] : MJet1.JetPhi[iJ1];
            double M   = (UseGen == true) ? MJet1.GenM[iJ1]   : MJet1.JetM[iJ1];

            if(DoJEC == true)
            {
               JEC1.SetJetPT(PT);
               JEC1.SetJetEta(Eta);
               JEC1.SetJetPhi(Phi);
               JEC1.SetJetArea(1);
               JEC1.SetRho(0);
               PT = JEC1.GetCorrectedPT();
            }

            if(PT < PTMin)
               continue;
            if(Eta < EtaMin || Eta > EtaMax)
               continue;

            FourVector J;
            J.SetPtEtaPhiMass(PT, Eta, Phi, M);
            Jet1.push_back(J);
         }

         if(IdenticalMode == true)
            Jet2 = Jet1;
         else
         {
            int NJet2 = (UseGen == true) ? MJet2.GenCount : MJet2.JetCount;
            for(int iJ2 = 0; iJ2 < NJet2; iJ2++)
            {
               double PT  = (UseGen == true) ? MJet2.GenPT[iJ2]  : MJet2.JetRawPT[iJ2];
               double Eta = (UseGen == true) ? MJet2.GenEta[iJ2] : MJet2.JetEta[iJ2];
               double Phi = (UseGen == true) ? MJet2.GenPhi[iJ2] : MJet2.JetPhi[iJ2];
               double M   = (UseGen == true) ? MJet2.GenM[iJ2]   : MJet2.JetM[iJ2];
               
               if(DoJEC == true)
               {
                  JEC2.SetJetPT(PT);
                  JEC2.SetJetEta(Eta);
                  JEC2.SetJetPhi(Phi);
                  JEC2.SetJetArea(1);
                  JEC2.SetRho(0);
                  PT = JEC2.GetCorrectedPT();
               }
               
               if(PT < PTMin)
                  continue;
               if(Eta < EtaMin || Eta > EtaMax)
                  continue;

               FourVector J;
               J.SetPtEtaPhiMass(PT, Eta, Phi, M);
               Jet2.push_back(J);
            }
         }

         for(int i1 = 0; i1 < Jet1.size(); i1++)
         {
            int BestIndex = -1;
            double BestDR = -1;

            for(int i2 = 0; i2 < Jet2.size(); i2++)
            {
               double DR = GetDR(Jet1[i1], Jet2[i2]);

               if(BestDR < 0 || DR < BestDR)
               {
                  BestIndex = i2;
                  BestDR = DR;
               }
            }

            if(BestIndex >= 0 && BestDR < MatchAngle)   // we have a match!
            {
               JetM.push_back(pair<FourVector, FourVector>(Jet1[i1], Jet2[BestIndex]));
               Jet1.erase(Jet1.begin() + i1);
               Jet2.erase(Jet2.begin() + BestIndex);
               i1 = i1 - 1;
            }
         }

         // Now fill in the bins
         for(FourVector &P : Jet1)
         {
            int Bin = GetBin(P.GetPT(), BinBoundary);
            M10[Bin] = M10[Bin] + Weight;
            M10E[Bin] = M10E[Bin] + Weight * Weight;
         }
         for(FourVector &P : Jet2)
         {
            int Bin = GetBin(P.GetPT(), BinBoundary);
            M01[Bin] = M01[Bin] + Weight;
            M01E[Bin] = M01E[Bin] + Weight * Weight;
         }
         for(pair<FourVector, FourVector> &P : JetM)
         {
            int Bin1 = GetBin(P.first.GetPT(), BinBoundary);
            int Bin2 = GetBin(P.second.GetPT(), BinBoundary);
            M11[Bin1][Bin2] = M11[Bin1][Bin2] + Weight;
            M11E[Bin1][Bin2] = M11E[Bin1][Bin2] + Weight;
         }
      }

      File.Close();
   }

   for(double &V : M10E)   V = sqrt(V);
   for(double &V : M01E)   V = sqrt(V);
   for(vector<double> &VV : M11E)   for(double &V : VV)   V = sqrt(V);

   VisualizeVector(PdfFile, M10, "In Jet 1 not in Jet 2");
   VisualizeVector(PdfFile, M01, "In Jet 2 not in Jet 1");
   VisualizeMatrix(PdfFile, M11, "In both");

   vector<double> Rho = DoSampling(PdfFile, NSample, M10, M01, M11, M10E, M01E, M11E);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   DataHelper DHFile2(OutputDHFileName);

   DHFile2[OutputDHState]["PT"] = DHFile["Binning"]["GenPT"];
   DHFile2[OutputDHState]["PTMin"] = PTMin;
   DHFile2[OutputDHState]["NSample"] = NSample;
   DHFile2[OutputDHState]["NFile"] = (int)InputFileNames.size();
   DHFile2[OutputDHState]["N"] = (int)Rho.size();
   for(int i = 0; i < (int)Rho.size(); i++)
      DHFile2[OutputDHState]["Rho"+to_string(i)] = Rho[i];

   DHFile2.SaveToFile();

   return 0;
}

int GetBin(double Value, vector<double> &BinBoundary)
{
   vector<double>::iterator iter = upper_bound(BinBoundary.begin(), BinBoundary.end(), Value);
   return (iter - BinBoundary.begin());
}

void VisualizeVector(PdfFileHelper &PdfFile, vector<double> &V, string Title)
{
   static int Index = 0;
   Index = Index + 1;

   int N = V.size();

   TH1D H(Form("H1%d", Index), ";Bin Index;", N, 0, N);
   H.SetTitle(Title.c_str());
   
   for(int i = 0; i < N; i++)
      H.SetBinContent(i + 1, V[i]);
   
   H.SetStats(0);
   PdfFile.AddPlot(H, "", true);
}

void VisualizeMatrix(PdfFileHelper &PdfFile, vector<vector<double>> &M, string Title)
{
   static int Index = 0;
   Index = Index + 1;

   int N1 = M.size();
   int N2 = M[0].size();

   TH2D H(Form("H2%d", Index), ";Bin Index 1;Bin Index 2", N1, 0, N1, N2, 0, N2);
   H.SetTitle(Title.c_str());

   for(int i = 0; i < N1; i++)
      for(int j = 0; j < N2; j++)
         H.SetBinContent(i + 1, j + 1, M[i][j]);
   
   H.SetStats(0);
   PdfFile.AddPlot(H, "colz", false, true);
}

vector<double> DoSampling(PdfFileHelper &PdfFile, int NSample,
   vector<double> &M01, vector<double> &M10, vector<vector<double>> &M11,
   vector<double> &M01E, vector<double> &M10E, vector<vector<double>> &M11E)
{
   static vector<int> Colors = GetPrimaryColors();

   int NBin1 = M10.size();
   int NBin2 = M01.size();

   Assert(NBin1 == NBin2, "Sampling only support mirroring bins right now");

   vector<double> S01(NBin1);
   vector<double> S10(NBin2);
   vector<vector<double>> S11(NBin1);
   for(vector<double> &V : S11)
      V.resize(NBin2);

   vector<double> S1(NBin1);
   vector<double> S2(NBin2);
   
   vector<double> SumS1(NBin1, 0);
   vector<double> SumS1S1(NBin1, 0);
   vector<double> SumS2(NBin2, 0);
   vector<double> SumS2S2(NBin2, 0);
   vector<double> SumR0(NBin1, 0);
   vector<double> SumR(NBin1, 0);
   vector<double> SumRR(NBin1,0 );
   
   for(int iS = 0; iS < NSample; iS++)
   {
      // reset to 0.  Maybe not needed?
      fill(S10.begin(), S10.end(), 0);
      fill(S01.begin(), S01.end(), 0);
      for(vector<double> &V : S11)
         fill(V.begin(), V.end(), 0);

      // Sample all the entries
      for(int i = 0; i < NBin1; i++)
      {
         S10[i] = DrawGaussian(M10[i], M10E[i]);
         if(S10[i] < 0)
            S10[i] = 0;
         S1[i] = S10[i];
      }
      for(int i = 0; i < NBin2; i++)
      {
         S01[i] = DrawGaussian(M01[i], M01E[i]);
         if(S01[i] < 0)
            S01[i] = 0;
         S2[i] = S01[i];
      }
      for(int i = 0; i < NBin1; i++)
      {
         for(int j = 0; j < NBin2; j++)
         {
            S11[i][j] = DrawGaussian(M11[i][j], M11E[i][j]);
            if(S11[i][j] < 0)
               S11[i][j] = 0;
         
            S1[i] = S1[i] + S11[i][j];
            S2[j] = S2[j] + S11[i][j];
         }
      }

      // Now we increment the global totals
      for(int i = 0; i < NBin1; i++)
      {
         SumS1[i]   = SumS1[i] + S1[i];
         SumS1S1[i] = SumS1S1[i] + S1[i] * S1[i];
      }
      for(int i = 0; i < NBin2; i++)
      {
         SumS2[i]   = SumS2[i] + S2[i];
         SumS2S2[i] = SumS2S2[i] + S2[i] * S2[i];
      }
      for(int i = 0; i < NBin1; i++)
      {
         double R = 0;
         if(S2[i] == 0)   // for now
            continue;
         R = S1[i] / S2[i];

         SumR0[i] = SumR0[i] + 1;
         SumR[i]  = SumR[i] + R;
         SumRR[i] = SumRR[i] + R * R;
      }
   }

   // Finally we can calculate the final quantities
   TGraphErrors G1, G2, GR;
   TGraph E1, E2, ER, ENaive;
   TGraph GRho;
   vector<double> VRho(NBin1);

   for(int i = 0; i < NBin1; i++)
   {
      double V1 = SumS1[i] / NSample;
      double RMS1 = sqrt(SumS1S1[i] / NSample - SumS1[i] / NSample * SumS1[i] / NSample);
      G1.SetPoint(i, i, V1);
      G1.SetPointError(i, 0, RMS1);
      
      double V2 = SumS2[i] / NSample;
      double RMS2 = sqrt(SumS2S2[i] / NSample - SumS2[i] / NSample * SumS2[i] / NSample);
      G2.SetPoint(i, i, V2);
      G2.SetPointError(i, 0, RMS2);
      
      double VR = SumR[i] / SumR0[i];
      double RMSR = sqrt(SumRR[i] / SumR0[i] - SumR[i] / SumR0[i] * SumR[i] / SumR0[i]);
      GR.SetPoint(i, i, VR);
      GR.SetPointError(i, 0, RMSR);

      double F1 = RMS1 / V1;
      double F2 = RMS2 / V2;
      double FR = RMSR / VR;

      E1.SetPoint(i, i, F1);
      E2.SetPoint(i, i, F2);
      ER.SetPoint(i, i, FR);
      ENaive.SetPoint(i, i, sqrt(F1 * F1 + F2 * F2));

      double Rho = (F1 * F1 + F2 * F2 - FR * FR) / (2 * F1 * F2);
      GRho.SetPoint(i, i, Rho);
      VRho[i] = Rho;
   }

   PdfFile.AddPlot(G1, "ap", true);
   PdfFile.AddPlot(G2, "ap", true);
   PdfFile.AddPlot(GR, "ap", true);

   ENaive.SetMarkerColor(Colors[0]);
   ENaive.SetLineColor(Colors[0]);
   E1.SetMarkerColor(Colors[1]);
   E1.SetLineColor(Colors[1]);
   E2.SetMarkerColor(Colors[2]);
   E2.SetLineColor(Colors[2]);
   ER.SetMarkerColor(Colors[3]);
   ER.SetLineColor(Colors[3]);

   TCanvas Canvas;

   ENaive.GetXaxis()->SetTitle("Gen bin index (full)");
   ENaive.GetYaxis()->SetTitle("Relative uncertainty");

   ENaive.Draw("apl");
   E1.Draw("pl");
   E2.Draw("pl");
   ER.Draw("pl");

   TLegend Legend(0.2, 0.5, 0.4, 0.8);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);
   Legend.AddEntry(&E1, "Jet 1", "pl");
   Legend.AddEntry(&E2, "Jet 2", "pl");
   Legend.AddEntry(&ENaive, "Independent", "pl");
   Legend.AddEntry(&ER, "Ratio", "pl");
   Legend.Draw();

   PdfFile.AddCanvas(Canvas);

   GRho.GetXaxis()->SetTitle("Gen bin index (full)");
   GRho.GetYaxis()->SetTitle("#rho");

   PdfFile.AddPlot(GRho, "apl");

   return VRho;
}


