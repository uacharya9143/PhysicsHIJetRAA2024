#include <iostream>
#include <vector>
using namespace std;

#include "TCanvas.h"
#include "TH1D.h"
#include "TFile.h"
#include "TKey.h"
#include "TGraph.h"
#include "TError.h"
#include "TLegend.h"

#include "SetStyle.h"
#include "PlotHelper4.h"
#include "CommandLine.h"
//#include "RootUtilities.h"

#include "BinHelper.h"

int main(int argc, char *argv[]);
void AddResponsePlot(PdfFileHelper &PdfFile, TH2D *HResponse);

int main(int argc, char *argv[])
{
  //SilenceRoot();

   vector<int> Colors = GetPrimaryColors();
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   int Ignore = CL.GetInt("Ignore", 20);

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage(OutputFileName);

   TFile File(InputFileName.c_str());

   TH1D *HMCTruth = (TH1D *)File.Get("HMCTruth");
   HMCTruth->SetMarkerStyle(20);
   HMCTruth->SetMarkerColor(Colors[0]);
   HMCTruth->SetLineWidth(2);
   HMCTruth->SetLineColor(Colors[0]);
   HMCTruth->SetStats(0);

   TH1D *HInput = (TH1D *)File.Get("HInput");
   HInput->SetMarkerStyle(20);
   HInput->SetMarkerColor(Colors[0]);
   HInput->SetLineWidth(2);
   HInput->SetLineColor(Colors[0]);
   HInput->SetStats(0);

   TH2D *HResponse = (TH2D *)File.Get("HMCResponse");
   AddResponsePlot(PdfFile, HResponse);

   TIter Iter(File.GetListOfKeys());
   for(TKey *Key = (TKey *)Iter(); Key != nullptr; Key = (TKey *)Iter())
   {
      string Name = Key->ReadObj()->GetName();

      if(Key->ReadObj()->IsA()->InheritsFrom("TH1") == false)
         continue;
      if(Name.rfind("HUnfoldedBayes", 0) != 0)
         continue;

      TGraph G1;
      G1.SetPoint(0, Ignore, 0);
      G1.SetPoint(1, Ignore, 1000000000);
      
      TGraph G2;
      G2.SetPoint(0, 0, 1);
      G2.SetPoint(1, 1000000000, 1);

      TH1D *H = (TH1D *)File.Get(Name.c_str());
      H->SetMarkerStyle(20);
      H->SetMarkerColor(Colors[1]);
      H->SetLineWidth(2);
      H->SetLineColor(Colors[1]);

      HMCTruth->Scale(H->Integral(Ignore, -1) / HMCTruth->Integral(Ignore, -1));
      HMCTruth->SetTitle(Form("Bayes unfold with iteration = %s", Name.substr(14).c_str()));

      TH1D *HFold = ForwardFold(H, HResponse);
      HFold->SetMarkerStyle(20);
      HFold->SetMarkerColor(Colors[1]);
      HFold->SetLineWidth(2);
      HFold->SetLineColor(Colors[1]);
      
      TH1D *HGenFold = ForwardFold(HMCTruth, HResponse);
      HGenFold->SetMarkerStyle(20);
      HGenFold->SetMarkerColor(Colors[2]);
      HGenFold->SetLineWidth(2);
      HGenFold->SetLineColor(Colors[2]);
      
      TH1D *HInputFold = ForwardFold(HInput, HResponse);
      HInputFold->SetMarkerStyle(20);
      HInputFold->SetMarkerColor(Colors[3]);
      HInputFold->SetLineWidth(2);
      HInputFold->SetLineColor(Colors[3]);
      
      // double Scale = HInput->Integral(Ignore, -1) / HFold->Integral(Ignore, -1);
      // cout << "ScaleFold = " << HInput->Integral(Ignore, -1) / HFold->Integral(Ignore, -1) << endl;
      // cout << "ScaleUnfold = " << H->Integral(Ignore, -1) / HMCTruth->Integral(Ignore, -1) << endl;

 
      TCanvas Canvas;
      HMCTruth->Draw("hist");
      H->Draw("hist same");
      H->Draw("same");
      G1.Draw("l");

      TLegend GenLegend(0.2, 0.2, 0.4, 0.4);
      GenLegend.SetTextFont(42);
      GenLegend.SetTextSize(0.035);
      GenLegend.SetBorderSize(0);
      GenLegend.SetFillStyle(0);
      GenLegend.AddEntry(HMCTruth, "MC Truth", "l");
      GenLegend.AddEntry(H, "Unfolded", "l");
      GenLegend.Draw();

      Canvas.SetLogy();
      PdfFile.AddCanvas(Canvas);

      TH1D *HRatio = (TH1D *)H->Clone((Name + "Ratio").c_str());
      HRatio->Divide(HMCTruth);

      HRatio->SetMaximum(2);
      HRatio->SetMinimum(0);
      HRatio->Draw();
      G1.Draw("l");
      G2.Draw("l");
      Canvas.SetLogy(false);
      PdfFile.AddCanvas(Canvas);


      // HFold->Scale(HInput->Integral(Ignore, -1) / HFold->Integral(Ignore, -1));
      // HGenFold->Scale(HInput->Integral(Ignore, -1) / HGenFold->Integral(Ignore, -1));
      // HInputFold->Scale(HInput->Integral(Ignore, -1) / HInputFold->Integral(Ignore, -1));

      HInput->Draw("hist");
      HFold->Draw("hist same");
      HGenFold->Draw("hist same");
      HInputFold->Draw("hist same");
      
      TLegend RecoLegend(0.2, 0.2, 0.4, 0.5);
      RecoLegend.SetTextFont(42);
      RecoLegend.SetTextSize(0.035);
      RecoLegend.SetBorderSize(0);
      RecoLegend.SetFillStyle(0);
      RecoLegend.AddEntry(HInput, "Input", "l");
      RecoLegend.AddEntry(HFold, "Folded Unfold", "l");
      RecoLegend.AddEntry(HGenFold, "Folded MCTruth", "l");
      RecoLegend.AddEntry(HInputFold, "Folded Reco (ie., nonsense)", "l");
      RecoLegend.Draw();
      
      Canvas.SetLogy();
      PdfFile.AddCanvas(Canvas);


      TH1D *HRatio_Input_FolUnFold=(TH1D *)HInput->Clone((Name+"Ratio").c_str());
      HRatio_Input_FolUnFold->Divide(HFold);
      HRatio_Input_FolUnFold->SetMaximum(5);
      HRatio_Input_FolUnFold->SetMinimum(0);
      HRatio_Input_FolUnFold->SetMarkerColor(Colors[4]);
      HRatio_Input_FolUnFold->SetMarkerStyle(24);
      HRatio_Input_FolUnFold->SetLineColor(Colors[4]);
      
      TH1D *HRatio_Input_FolMCTruth=(TH1D *)HInput->Clone((Name+"Ratio").c_str());
      HRatio_Input_FolMCTruth->Divide(HGenFold);
      HRatio_Input_FolMCTruth->SetMaximum(5);
      HRatio_Input_FolMCTruth->SetMinimum(0);
      HRatio_Input_FolMCTruth->SetMarkerColor(Colors[2]);
      HRatio_Input_FolMCTruth->SetLineColor(Colors[2]);
      HRatio_Input_FolMCTruth->Draw();
      HRatio_Input_FolUnFold->Draw("same");
      TLegend RatLegend(0.3, 0.65, 0.4, 0.8);
      RatLegend.SetTextFont(42);
      RatLegend.SetTextSize(0.035);
      RatLegend.SetBorderSize(0);
      RatLegend.SetFillStyle(0);
      RatLegend.AddEntry(HRatio_Input_FolUnFold, "Input/Folded_Unfolded", "l");
      RatLegend.AddEntry(HRatio_Input_FolMCTruth, "Input/Folded_MCTruth", "l");
      RatLegend.Draw();
  
      //G1.Draw("l");
      //G2.Draw("l");
      Canvas.SetLogy(false);
      PdfFile.AddCanvas(Canvas);
   }

   File.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void AddResponsePlot(PdfFileHelper &PdfFile, TH2D *HResponse)
{
   TH2D *H = (TH2D *)HResponse->Clone();
   H->Reset();

   int NGen = HResponse->GetNbinsY();
   int NReco = HResponse->GetNbinsX();

   for(int iG = 1; iG <= NGen; iG++)
   {
      double N = 0;
      for(int iR = 1; iR <= NReco; iR++)
         N = N + HResponse->GetBinContent(iR, iG);

      if(N == 0)
         continue;

      for(int iR = 1; iR <= NReco; iR++)
         H->SetBinContent(iR, iG, HResponse->GetBinContent(iR, iG) / N);
   }

   H->SetStats(0);
   PdfFile.AddPlot(H, "colz", false, true);
   H->SetMinimum(1e-6);
   PdfFile.AddPlot(H, "colz", false, true);
}


