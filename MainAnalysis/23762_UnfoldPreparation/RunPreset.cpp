#include <iostream>
#include <vector>
#include <map>
using namespace std;

#include "DataHelper.h"
#include "CommandLine.h"
#include "BinHelper.h"

int main(int argc, char *argv[]);
map<string, string> GetBinnings();

int main(int argc, char *argv[])
{
   map<string, string> Binnings = GetBinnings();

   CommandLine CL(argc, argv);
   
   string GlobalSettingFile  = CL.Get("GlobalSetting", "GlobalSetting.dh");
   DataHelper DHFile(GlobalSettingFile);

   string DataTag            = CL.Get("DataTag");
   string MCTag              = CL.Get("MCTag");
   string Prefix             = CL.Get("Prefix");
   string Suffix             = CL.Get("Suffix");

   bool UseJEU               = CL.GetBool("UseJEU", true);
   double JEUOffset          = CL.GetDouble("JEUOffset", 0);
   bool UseJERSFFile         = CL.GetBool("UseJERSFFile", false);
   string JERSFFile          = CL.Get("JERSF", "none");

   string JetShift           = CL.Get("Shift", "0.0");
   string JetSmear           = CL.Get("Smear", "0.0");//"0.025");
   string Flooring           = CL.Get("Flooring", "false");

   double EtaMin             = CL.GetDouble("EtaMin", -2.0);
   double EtaMax             = CL.GetDouble("EtaMax", +2.0);

   bool DoCopy               = CL.GetBool("DoCopy", false);

   vector<string> JetR       = CL.GetStringVector("JetR", ParseStringList(DHFile["Global"]["JetR"].GetString()));
   vector<string> Centrality = CL.GetStringVector("Centrality", ParseStringList(DHFile["Global"]["Centrality"].GetString()));

   double MCFraction         = CL.GetDouble("MCFraction", 1.0);
   double DataFraction       = CL.GetDouble("DataFraction", 1.0);

   Binnings["GenBins"]       = CL.Get("GenBins", DHFile["Binning"]["GenPT"].GetString());
   Binnings["RecoBins"]      = CL.Get("RecoBins", DHFile["Binning"]["RecoPT"].GetString());

   double GenMin             = CL.GetDouble("GenMin", DHFile["Binning"]["GenMin"].GetDouble());
   double RecoMin            = CL.GetDouble("RecoMin", DHFile["Binning"]["RecoMin"].GetDouble());

   for(string R : JetR)
   {
      double RValue = DHFile["JetR"][R].GetDouble();

      for(string C : Centrality)
      {
         string RC = "R" + R + "_Centrality" + C;
         string MCFile = MCTag + "_" + RC + ".root";
         string DataFile = DataTag + "_" + RC + ".root";

         if(DoCopy == true)
         {
            cout << "time cp " << MCFile << " MC.root;" << endl;
            cout << "time cp " << DataFile << " Data.root;" << endl;
            cout << "time ./Execute --MC MC.root --Data Data.root";
         }
         else
            cout << "time ./Execute --MC " << MCFile << " --Data " << DataFile;
         cout << " --MCFraction " << MCFraction << " --DataFraction " << DataFraction
            << " --Output Output_R4/" << Prefix << "_" << RC << "_" << Suffix << ".root"
            << " --JSONOutput Output_R4/" << Prefix << "_" << RC << "_" << Suffix << "_JSON.txt"
            << " --ExportJSON true"
            << " --EtaMin " << EtaMin << " --EtaMax " << EtaMax
            << " --Observable JetPT --ObservableIndex -1 --Binning None --BinningIndex -1"
            << " --ObservableGenBins " << Binnings["GenBins"]
            << " --ObservableRecoBins " << Binnings["RecoBins"]
            << " --ObservableGenMin " << GenMin
            << " --ObservableRecoMin " << RecoMin
	   //            << " --BinningGenBins 0,1 --BinningRecoBins 0,1"
            << " --BinningGenBins 0,1 --BinningRecoBins 0,1"
            << " --ObservableShift " << JetShift
            << " --ObservableSmear " << JetSmear
            << " --Flooring " << Flooring
            << " --UseJEU " << UseJEU
            << " --JEUOffset " << JEUOffset
            << " --UseJERSFFile " << UseJERSFFile << " --JERSF " << JERSFFile
            << " --CheckMatchAngle true --MaxMatchAngle " << DHFile["JetRMatch"][R].GetDouble()
            << ";" << endl;
         if(DoCopy == true)
            cout << "time rm MC.root Data.root;" << endl;
      }
   }

   return 0;
}

map<string, string> GetBinnings()
{
   map<string, string> Binnings;

   string List = "";
   for(int i = 81; i<= 1499; i++)
      List = List + to_string(i) + ",";

   Binnings["GenBins"] = List;
   Binnings["RecoBins"] = List;

   return Binnings;
}

