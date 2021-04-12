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

//! \file rapfloat.cxx implements the methods of rapfloat.h
//! \ingroup Ulam
#include "rapfloat.h"
#include <fenv.h>

rapfloat::rapfloat()
	{
		value=APFloat(0);
		error=APFloat(0);
	}

rapfloat::rapfloat(const APFloat &a, const APFloat &b)
	{
		value=a;
		error=b;
	}

bool rapfloat::operator>=(const APFloat &test) const // This ordering operator takes into account the error 
{													// please note that we are checking the most restrictive condition, i.e.,			
		if (this->value >= test)					// a measure is bigger of a given number only if the interval [value-error,value+error]
			{										// is bigger than the given number
				if ((this->value)-(this->error)>=test)
				{
					if ((this->value)+(this->error)>=test)
						{
							return true;
						}
				}
			}
		return false;
};

rapfloat operator+(const rapfloat &a, const rapfloat &b)
{
    rapfloat temp;
    temp.value = a.value + b.value;
    
    //please note that since the second entry is the error, when we sum the second members, we round upward
    int old_rounding=fegetround();
	fesetround(FE_UPWARD);					
    temp.error = a.error + b.error;
	fesetround(old_rounding);
	
	return temp;
}

rapfloat& rapfloat::operator=(const rapfloat& a)
{
	if (this == &a)  return *this;    // Same object?      
	value=a.value;
	error=a.error;
	return *this;
}

