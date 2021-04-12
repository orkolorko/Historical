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

//! \file dynamic.cxx implements dynamic.h
//! \ingroup Ulam

#include "dynamic.h"
#include "rinterval.h"

//The other examples, in the paper are in the file, commented


bool checkdiscontinuity(rinterval &input)
{
	RigScalar a1=RigScalar(5,17);
	RigScalar a2=RigScalar(10,17);
	RigScalar a3=RigScalar(15,17);
	if (input.iscontained(a1)) 
	{
		cout << "Raggiunto a1" << endl;
		return true;
	}
	if (input.iscontained(a2))
	{
		cout << "Raggiunto a2" << endl;
		return true;
	}
	if (input.iscontained(a3))
	{
		cout << "Raggiunto a3" << endl;
		return true;
	}
	return false;
}


//Mappa NonMarkov2

RigScalar dyn(const RigScalar &point)
{
	RigScalar temp(0);
		if ((point>=0) && (point<=RigScalar(5,17)))
		{
			temp=RigScalar(17,5)*point;
		}
		if ((point>RigScalar(5,17)) && (point<=RigScalar(10,17)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(5,17);
			temp=RigScalar(17,5)*temp1;
		}
		if ((point>RigScalar(10,17)) && (point<=RigScalar(15,17)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(10,17);
			temp=RigScalar(17,5)*temp1;
		}
	if ((point>RigScalar(15,17)) && (point<=RigScalar(1)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(15,17);
			temp=RigScalar(17,5)*temp1;
		}
	return temp;
};



//Mappa NonMarkov1
/*
RigScalar dyn(const RigScalar &point)
{
	RigScalar temp(0);
		if ((point>=0) && (point<=RigScalar(5,17)))
		{
			temp=RigScalar(17,5)*point;
		}
		if ((point>RigScalar(5,17)) && (point<=RigScalar(10,17)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(5,17);
			temp=RigScalar(34,25)*temp1*temp1+3*temp1;
		}
		if ((point>RigScalar(10,17)) && (point<=RigScalar(15,17)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(10,17);
			temp=RigScalar(34,25)*temp1*temp1+3*temp1;
		}
	if ((point>RigScalar(15,17)) && (point<=RigScalar(1)))
		{
			RigScalar temp1(0);
			temp1=point-RigScalar(15,17);
			temp=RigScalar(17,5)*temp1;
		}
	return temp;
};
*/

 
//Mappa quaddefault
/*

bool checkdiscontinuity(rinterval &input)
{
	RigScalar a1=RigScalar(1,4);
	RigScalar a2=RigScalar(1,2);
	RigScalar a3=RigScalar(3,4);
	if (input.iscontained(a1)) 
	{
		cout << "Raggiunto a1" << endl;
		return true;
	}
	if (input.iscontained(a2))
	{
		cout << "Raggiunto a2" << endl;
		return true;
	}
	if (input.iscontained(a3))
	{
		cout << "Raggiunto a3" << endl;
		return true;
	}
	return false;
}


RigScalar dyn(const RigScalar &point)
{
	RigScalar temp(0);
		if ((point>=0) && (point<=RigScalar(1,4)))
		{
			temp=RigScalar(1,8)+RigScalar(3)*point+RigScalar(2)*point*point;
		}
		if ((point>RigScalar(1,4)) && (point<=RigScalar(1,2)))
		{
			temp=4*(point-RigScalar(1,4));
		}
		if ((point>RigScalar(1,2)) && (point<=RigScalar(3,4)))
		{
			temp=4*(point-RigScalar(1,2));
		}
	if ((point>RigScalar(3,4)) && (point<=RigScalar(1)))
		{
			temp=RigScalar(7,8)+RigScalar(3)*(point-1)-1*RigScalar(2)*(point-1)*(point-1);
		}
	return temp;
};


*/

// Lanford
/*
RigScalar non_quot(const RigScalar &point)
{
	RigScalar temp;
	temp=RigScalar(2,1)*point+RigScalar(1,2)*point*(RigScalar(1,1)-point);
	return temp;
}

RigScalar non_quot_2_it(const RigScalar &point)
{
	RigScalar temp;
	temp=non_quot(point);
	temp=non_quot(temp);
	return temp;
}

bool checkdiscontinuity(rinterval &input)
{
	rinterval non_quot_image(non_quot_2_it(input.leftend()),non_quot_2_it(input.rightend()));
	
	RigScalar a1=RigScalar(1,1);
	RigScalar a2=RigScalar(2,1);
	RigScalar a3=RigScalar(3,1);
	if (non_quot_image.iscontained(a1)) 
	{
		cout << "Raggiunto a1" << endl;
		return true;
	}
	if (non_quot_image.iscontained(a2))
	{
		cout << "Raggiunto a2" << endl;
		return true;
	}
	if (non_quot_image.iscontained(a3))
	{
		cout << "Raggiunto a3" << endl;
		return true;
	}
	return false;
}
 
RigScalar dyn(const RigScalar &point)
{
	RigScalar temp;
	temp=non_quot_2_it(point);
	temp=temp-(temp.numerator()/temp.denominator());
	return temp;
}
*/
