#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>
#include <iostream>
#include <utility>
#include "iRRAM_lib.h"

//#include "FADBAD++/fadiff.h"

using namespace iRRAM;

typedef boost::numeric::interval<double> Interval;

Interval f(Interval x)
{
	
	return (x-Interval(1))*(x-Interval(1));
}

void fbisect(Interval x)
{
	if (zero_in(f(x)))
	{
		if (width(x)<0.0001)
		{
			std::cout << "x" << std::endl;
			std::cout << x << std::endl;
			std::cout << "f(x)" << std::endl;
			std::cout<<f(x)<<std::endl;
	
		}
		else
		{
			std::pair <Interval, Interval> divided;
			divided=bisect(x);
			fbisect(divided.first);
			fbisect(divided.second);
		}
	}
}

int main(void)
{
	Interval a;
	a=Interval(0,3);
	Interval b(0,20);
	std::cout.precision(30);
	fbisect(a);
}
