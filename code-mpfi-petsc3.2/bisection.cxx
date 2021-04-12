#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>
#include <iostream>
#include <stdexcept>
#include <utility>
#include "iRRAM_core.h"
#include "iRRAM_lib.h"
#include "petscmat.h"
#include "realautder.h"
#include "dynamic.h"

using namespace iRRAM;

typedef boost::numeric::interval<double> Interval;

double absvaluedistortion(REAL x, int l)
{
	Taylor<3> point(x,1);
	Taylor<3> temp=DynamicTaylor(DynamicTaylor(DynamicTaylor(point)));
	REAL out=abs(temp.viewcoeff(2)/(temp.viewcoeff(1)*temp.viewcoeff(1)))-REAL(l);
	return double(out);
}

double primitive(const REAL & x)
{
	Taylor<3> point(x,1);
	Taylor<3> temp=DynamicTaylor(DynamicTaylor(DynamicTaylor(point)));
	REAL sgn=abs(temp.viewcoeff(2)/(temp.viewcoeff(1)*temp.viewcoeff(1)))/abs(temp.viewcoeff(2)/(temp.viewcoeff(1)*temp.viewcoeff(1)));
	REAL out=sgn/temp.viewcoeff(1);
	return double(out);
}

Interval integral(Interval x)
{
	double lower=x.lower();
	double upper=x.upper();
	Interval out(0);
	double imlower=iRRAM_exec(primitive,REAL(lower));
	double imupper=iRRAM_exec(primitive,REAL(upper));
	if (imupper>imlower) out=Interval(imlower,imupper);
	else out=Interval(imupper,imlower);
	return out;
}

Interval f(Interval x,int l)
{
	double lower=x.lower();
	double upper=x.upper();
	Interval out(0);
	double imlower=iRRAM_exec(absvaluedistortion,REAL(lower),l);
	double imupper=iRRAM_exec(absvaluedistortion,REAL(upper),l);
	if (imupper>imlower) out=Interval(imlower,imupper);
	else out=Interval(imupper,imlower);
	return out;
	//return Interval(lower,upper);
	//return (x-Interval(1))*(x-Interval(1));
}

Interval betterf(Interval x,int l)
{
	std::pair <Interval, Interval> divided;
	divided=bisect(x);
	Interval a=f(divided.first,l);
	Interval b=f(divided.second,l);
	return hull(a,b);
}



Interval fbisect(Interval x,int l,double error)
{
	if (zero_in(betterf(x,l)))
	{
		if (width(x)<error)
		{
			//std::cout << "x" << std::endl;
			//std::cout << x << std::endl;
			//std::cout << "f(x)" << std::endl;
			//std::cout<<betterf(x,l)<<std::endl;
			return x;
		}
		else
		{
			std::pair <Interval, Interval> divided;
			divided=bisect(x);
			try
			{
				Interval x1=fbisect(divided.first,l,error);
				if (!empty(x1)) return x1;
			}
			catch (std::runtime_error e)
			{
				
			}
			try
			{
				Interval x2=fbisect(divided.second,l,error);
				if (!empty(x2)) return x2;
			}
			catch
			(std::runtime_error e)
			{
				
			}
		}
	}
	Interval a;
	return a.empty();
}

int main(PetscInt argc, char *argv[])
{
	iRRAM_initialize(argc,argv);
	int l=1200;
	int size=32768;
	for (int i=0; i<argc; i++)
			{
				string str1("-l");
				if (str1.compare(argv[i])==0)
					{
						l=atoi(argv[i+1]);
					}
				str1=string("-size");
				if (str1.compare(argv[i])==0)
					{
						size=atoi(argv[i+1]);
					}
			}
	Interval a;
	//a.empty();
	Interval distortion(0);
	for (int i=0;i<size-1;i++)
	{
		if (i%1000==0) std::cout << i << std::endl;
		Interval a;
		a=Interval(double(i)/double(2*size),double(i+1)/double(2*size));
		
		std::cout.precision(30);
		if (zero_in(betterf(a,l))) 
		{
			//std::cout<< a <<std::endl;
			//std::cout<< betterf(a,l)<<std::endl;
			Interval b=fbisect(a,l,0.000000000000001);
			std::cout<< b << std::endl;
			distortion+=integral(b);
			//std::cin.get();
		}
	}
	Interval minderinv(iRRAM_exec(primitive,REAL(0)));
	std::cout << "minderinv " << minderinv << std::endl; 
	std::cout << "Distortion " << Interval(2)*(distortion+minderinv) <<std::endl;
}
