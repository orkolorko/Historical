//#include <boost/numeric/interval.hpp>
//#include <boost/numeric/interval/io.hpp>
#include <utility>
#include <iostream>

#include "iRRAM.h"

using namespace iRRAM;
using namespace std;


void printinterval(const INTERVAL& x){
 cout << setRwidth(20)<<"( "<<inf(x)<<"  ,  "<<sup(x)<<" )\n";
}



typedef iRRAM::INTERVAL Interval;

typedef Interval (*pfcn) (Interval,int);

Interval function (Interval X, int n)
{
	if (n==0) return X*X+Interval(-3)*X+Interval(6); 
	else return Interval(2)*X+Interval(-3);
}

Interval F(Interval X){return function(X,0);}
Interval DF(Interval X){return function(X,1);}

Interval K(pfcn f,Interval X)
{
	//std::cout << "X "<< X << std::endl;
	double med=median(X);
	//std::cout << "med "<< med << std::endl;
	
	Interval m(med);
	//std::cout << "m " << m << std::endl;
	
	Interval DFm(DF(m));
	//std::cout << "DFm " << DFm << std::endl;
	
	Interval out=m-F(m)/DFm+(Interval(1)-DF(X)/DFm)*(X-m);
	//std::cout << "out " << out << std::endl;
	
	return out;
}

void krawczyk(pfcn f, Interval X, double tol)
{
	Interval KX=K(f,X);
	try
	{
	KX=intersect(X,KX); 
	}
	catch (std::runtime_error a)
	{
	//	std::cout << a.what() << endl;
	//	std::cout << "No root in " <<KX << endl;
	}
	if (!(empty(KX)))
	{
		if (width(KX)<tol)
		{
			if (proper_subset(KX,X))
			{
				std::cout << "unique root in the interval " << KX << std::endl;
			}
			else
			{
				std::cout << "maybe a root in the interval " << KX << std::endl;
			}
		}
		else
		{
			std::pair <Interval, Interval> divided;
			divided=bisect(KX);
			krawczyk(f,divided.first,tol);
			krawczyk(f,divided.second,tol);
		}	
	}
}

int main(int argc, char*argv[])
{
	Interval X(-3,2);
	double tol(0.000000000000001);
	krawczyk(function,X,tol);
	return 0;
}
*/
