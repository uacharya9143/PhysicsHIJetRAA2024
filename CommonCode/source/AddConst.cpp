#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main(int argc, char *argv[])
{
   double constant = 0;

   if(argc == 1)
      constant = 0;
   else
   {
      stringstream str(argv[1]);
      str >> constant;
   }

   cout.precision(10);

   while(cin)
   {
      double temp = -135798642;

      cin >> temp;

      if(temp + 135798642 < 1e-8)
         break;

      cout << temp + constant << endl;
   }

   return 0;
}

