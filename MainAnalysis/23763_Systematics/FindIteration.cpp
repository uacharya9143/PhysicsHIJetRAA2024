#include <vector>
#include <iostream>
using namespace std;

#include "TFile.h"
#include "TKey.h"

#include "CommandLine.h"

int main(int argc, char *argv[]);
vector<int> ListIterations(string FileName);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   string Mode          = CL.Get("Mode");
   int Reference        = CL.GetInt("Iteration");

   vector<int> Iterations = ListIterations(InputFileName);

   if(Mode == "Next")
   {
      int Best = -1;
      for(int i = 0; i < (int)Iterations.size(); i++)
      {
         if(Iterations[i] > Reference)
         {
            if(Iterations[i] < Best || Best < 0)
               Best = Iterations[i];
         }
      }
      if(Best > 0)
         cout << Best << endl;
      else
         cout << Reference << endl;
      return 0;
   }
   if(Mode == "Previous")
   {
      int Best = -1;
      for(int i = 0; i < (int)Iterations.size(); i++)
      {
         if(Iterations[i] < Reference )
         {
            if(Iterations[i] > Best|| Best < 0)
               Best = Iterations[i];
         }
      }
      if(Best > 0)
         cout << Best << endl;
      else
         cout << Reference << endl;
      return 0;
   }

   cout << Reference << endl;

   return 0;
}

vector<int> ListIterations(string FileName)
{
   vector<int> Result;

   TFile File(FileName.c_str());

   TIter Iter(File.GetListOfKeys());
   for(TKey *Key = (TKey *)Iter(); Key != nullptr; Key = (TKey *)Iter())
   {
      string Name = Key->ReadObj()->GetName();
      if(Name.rfind("HUnfoldedBayes", 0) != 0)
         continue;

      Result.push_back(stoi(Name.substr(14)));
   }

   File.Close();

   return Result;
}




