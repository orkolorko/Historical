#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include <fstream>
#include <stdlib.h> 

#include "linalg.h"

using namespace std;

int main (int argc, char *argv[])
{
		string nomefilein,nomefileout;
		srmatrix sA;
		
		// This section checks the command line inputs of the program 
		for (int i=0; i<argc; i++)
			{
				string str1("-input");
				if (str1.compare(argv[i])==0)
					{
						string labile(argv[i+1]);
						nomefilein=labile;
					}
				str1=string("-output");
				if (str1.compare(argv[i])==0)
					{
						string labile(argv[i+1]);
						nomefileout=labile;
					}
				str1=string("-help");
				if (str1.compare(argv[i])==0)
					{
						cout << "-input mandatory for the input file" << endl;
						cout << "-output mandatory for the output file" << endl;
						return 0;
					}
			}
		std::ifstream ifile(nomefilein.c_str());
		boost::archive::text_iarchive iarchive(ifile);
		std::ofstream ofile(nomefileout.c_str());
		boost::archive::text_oarchive oarchive(ofile);
		
		// Gets the matrix from the input file
		iarchive >> sA;
		sA.Markovize();
		
		//Outputs the matrix
		oarchive << sA;
		
		return 0;
}
