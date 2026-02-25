#include <iostream>
#include <vector>
using namespace std;

#include "TF1.h"

#include "CommandLine.h"
#include "DataHelper.h"

#include "JetCorrector.h"

int main(int argc, char *argv[]);
vector<double> ParseList(string List);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string DHFileName             = CL.Get("DHFile", "GlobalSetting.dh");
   string Base                   = CL.Get("Base", "R1_Centrality0to10");

   double MinPT                  = CL.GetDouble("MinPT");

   bool DoTrigger                = CL.GetBool("DoTrigger", false);
   double TriggerPercentage      = CL.GetDouble("TriggerPercentage", 0.99);
   double TriggerTolerance       = CL.GetDouble("TriggerTolerance", 0.001);
   string TriggerBase            = CL.Get("TriggerBase", Base);
   bool DoTriggerResolution      = DoTrigger ? CL.GetBool("DoTriggerResolution") : false;
   double TriggerResolutionShift = DoTriggerResolution ? CL.GetDouble("TriggerResolutionShift") : 0;
   string ResolutionFile         = DoTriggerResolution ? CL.Get("ResolutionFile") : "NONE";

   double PTBound = MinPT;

   DataHelper DHFile(DHFileName);

   vector<double> GenBins   = ParseList(DHFile["Binning"]["GenPT"].GetString());
   vector<double> RecoBins  = ParseList(DHFile["Binning"]["RecoPT"].GetString());
   GenBins.insert(GenBins.begin(), 0);
   GenBins.insert(GenBins.end(), 99999);
   RecoBins.insert(RecoBins.begin(), 0);
   RecoBins.insert(RecoBins.end(), 99999);

   // If we decide to look at trigger, evaluate the turn on point
   if(DoTrigger == true)
   {
      string Formula = DHFile["TriggerTurnOn"][TriggerBase+"_Formula"].GetString();
      cout<<"FOrmula" << Formula <<" " << "Bin" <<Base<<endl;
      TF1 Function("Function", Formula.c_str(), 0, 1500);
      int N = DHFile["TriggerTurnOn"][TriggerBase+"_NParameter"].GetInteger();
      for(int i = 0; i < N; i++)
      {
         Function.SetParameter(i, DHFile["TriggerTurnOn"][TriggerBase+"_P"+to_string(i)].GetDouble());
         Function.SetParError(i, DHFile["TriggerTurnOn"][TriggerBase+"_E"+to_string(i)].GetDouble());
      }

      double Min = 0;
      double Max = 1;
      while(Function.Eval(Max) < TriggerPercentage)
         Max = Max * 2;
      while(Max - Min > TriggerTolerance)
      {
         if(Function.Eval((Min + Max) / 2) < TriggerPercentage)
            Min = (Min + Max) / 2;
         else
            Max = (Min + Max) / 2;
      }

      double PTBoundFromTrigger = (Min + Max) / 2;

      if(DoTriggerResolution == true)
      {
         JetCorrector JER(ResolutionFile);

         JER.SetJetPT(PTBoundFromTrigger);
         JER.SetJetEta(0);
         JER.SetJetPhi(0);
         JER.SetRho(0);
         JER.SetJetArea(0.5);

         double Value = JER.GetCorrection();
         PTBoundFromTrigger = PTBoundFromTrigger * (1 + Value * TriggerResolutionShift);
      }

      if(PTBound < PTBoundFromTrigger)
         PTBound = PTBoundFromTrigger;
   }

   // Set final output
   int NumberSmaller = 0;
   for(double Value : GenBins)
      if(Value < PTBound)
         NumberSmaller = NumberSmaller + 1;
   DHFile["Binning"]["PTUnderflow_"+Base] = NumberSmaller;
   DHFile["Binning"]["PTUnderflowValue_"+Base] = PTBound;

   DHFile.SaveToFile();

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

