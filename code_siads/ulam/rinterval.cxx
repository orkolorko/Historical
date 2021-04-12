/*
 *	Copyright 2011-2013 Stefano Galatolo (galatolo@mail.dm.unipi.it), 
 * 						Isaia Nisoli (nisoli@im.ufrj.br) 
 *	
 *	This file is part of CompInvMeas-Ulam.
 *
 *  CompInvMeas-Ulam is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CompInvMeas-Ulam is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CompInvMeas-Ulam.  If not, see <http://www.gnu.org/licenses/>.
*/

//!\file rinterval.cxx implements the methods of rinterval.h
//!\ingroup Ulam

#include "rinterval.h"
#include "rpartition.h"
#include <iostream>
#include "dynamic.h"
#include <fenv.h>
#include <limits>

//Rigorous interval

int Debug2=0;

//! Constructor for the 0 interval
//!\ingroup Ulam
rinterval::rinterval()
{
	min=RigScalar(0);
	max=RigScalar(0);
	in=RigScalar(0);
}

/*! Constructor for an interval of type [a,b], with error
 please note that the intervals constructed by using input1 and input2, if input2<input1
 are of the type [input2,input1], this was used in case of dynamics on the circle...
	\ingroup Ulam
*/
rinterval::rinterval(const RigScalar &input1, const RigScalar &input2)
{
	if (input1>input2)
		{
			min=input2;
			max=input1;
		}
	else
		{
			min=input1;
			max=input2;
		}
	in=(input1+input2)/2;
}

/*! Constructor for an interval of type [a,b], with an explicit interior point, with error
 please note that the intervals constructed by using input1 and input2, if input2<input1
 are of the type [input2,input1]
 * \ingroup Ulam
*/
rinterval::rinterval(const RigScalar &input1, const RigScalar &input2, const RigScalar &input3)
{

	if (input1>input2)
		{
			min=input2;
			max=input1;
		}
	else
		{
			min=input1;
			max=input2;
		}
	in=input3;
}

// Please note, that at the current state of the project this is the only 
// function that depends on the implementation of the arbitrary precision floats
//! length of the interval
//!\ingroup Ulam
const APFloat rinterval::APlength() const
{
	RigScalar value=abs(max-min);
	return boost::rational_cast<double>(value);
}

// This function, given two intervals tests if the test interval is in one of the three cases, 
// if the interval is contained returns 2, 
// if it is not contained but intersects returns 1, 
// if no intersection returns 0

//! containment function, with a "tribool"-type output
//!\ingroup Ulam
const int rinterval::contained(const rinterval &test) const 
{
		int i=0;
				// checks if the left end is contained in the interval
				if ((test.min>=min) && (test.min<=max)) 
				{
					i++;
				}
				// checks if the right end is contained in the interval
				if ((test.max>=min) && (test.max<=max))
				{
					i++;
				}
				// checks if the interior point of test is contained in the interval
				if ((i==2) && (test.in>=min) && (test.in<=max))
				{
					i++;
				}
		return i;
}

//! computes the image of an interval through the dynamic
//!\ingroup Ulam
const rinterval dynamic(const rinterval &domain)
{
	RigScalar temp1;
	RigScalar temp2;
	RigScalar temp3;
	temp1=dyn(domain.min);
	temp2=dyn(domain.max);
	temp3=dyn(domain.in);
	rinterval temp(temp1,temp2,temp3); //Please note that since we are using rational functions, the images are rationals
	return temp;
}

//! computes the coefficients
//!\ingroup Ulam
const rapfloat mixing(const rinterval &p,const rinterval &q, const int &supder, const APFloat &error) //computes the measure of m(T^{-1}p\cap q)
{
	rapfloat measure(0,0); // initializes the measure at 0 with 0 error
	rinterval temp; 
	rpartition qpart(q,supder); //partitions the q interval in hundred parts
	Container::iterator i; 
	for (i=(qpart.value).begin();i!=(qpart.value).end();i++) //starts evaluating the interval
		{
			
			int test=0;
			temp=dynamic(*i); //sends one of the interval of the partition of q in the future
			test=p.contained(temp);
			int old_rounding=fegetround();
			fesetround(FE_UPWARD);			
			APFloat len=(*i).APlength(); // since the estimate depends on upper value of the length we round upward
				//If the interval is contained
			fesetround(old_rounding);
			
			if (test==3)
				{
				// we take into account the 
				measure=measure+rapfloat((*i).APlength(),(*i).APlength()*std::numeric_limits<APFloat>::epsilon());
				} 
				//sums its measure to the measure
			
			if  ((test>0) && (test<3)) //if it is not contained, but the intersection is not empty 
				{
					
					if (len>error)    //if the length of the partition is bigger than the allowed error
					{
						measure=measure+mixing(p,*i,supder,error); //iterates the function, partitioning *i
					}
					
					if (len<=error) //in case the length is smaller than the allowed error
					{
						test=p.contained(temp);
						measure=measure+rapfloat(0,len); //sums the length of the interval *i to the error rounding upwards
					}
					
				}
			
		}
	return measure;
}

//! assignment operator
//!\ingroup Ulam
rinterval& rinterval::operator=(const rinterval &rhs)
{
		if (this == &rhs)  return *this;    // Same object?      
		else
			{
			try
				{
				this->min=rhs.min;
				this->max=rhs.max;
				this->in=rhs.in;
				}
			catch(std::exception & exc)
				{	
				std::cerr << "exception" << exc.what() << endl;
				}
			return *this;
			}
}

//! ostream operator
//!\ingroup Ulam
std::ostream &operator<< (std::ostream &os, const rinterval &inter)
{
		return os << "[" << inter.min << "," << inter.max <<"], Internal point " << inter.in << endl;
};

