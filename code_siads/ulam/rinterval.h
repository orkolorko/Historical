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
/*! \file rinterval.h
 * \brief contains the class rinterval
	\ingroup Ulam
 */
 
//Stl libraries
#include <vector>
#include <list>
//Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/rational.hpp>
#include "rapfloat.h"
//#include "dynamic.h"

using namespace std;

typedef double APFloat;


#ifndef RINTERVAL_H
#define RINTERVAL_H

/*! \class rinterval
	\brief This class represents an interval with rational ends
	\ingroup Ulam
 */
class rinterval
{
		//! left endpoint
		RigScalar min;
		//! right endpoint
		RigScalar max;
		//! interior point
		RigScalar in;
		
		public:
		
		//Constructor
		//! empty constructor
		rinterval();
		//! constructor, given endpoints
		rinterval(const RigScalar &, const RigScalar &);
		//! constructor given endpoints and interior point
		rinterval(const RigScalar &, const RigScalar &, const RigScalar &);
		
		//Member Functions
		//! returns left endpoint
		const RigScalar leftend(){return min;};
		//! returns right endpoint
		const RigScalar rightend(){return max;};
		//! returns interior point
		const RigScalar intpoint(){return in;};
		
		//! checks containment of intervals
		const int contained(const rinterval&) const;
		//! checks overlapping of intervals
		const bool overlap(const rinterval&) const;
		//! assignment operator
		rinterval& operator=(const rinterval &rhs);
		
		//! checks containment of a point
		const bool iscontained(const RigScalar & point) const 
			{
				if ((point>=min)&& (point<=max)){return true;}
				else return false;
			};
		//! checks containment of a point
		const bool iscontained(const float & point) const 
			{
				if ((point>=min)&& (point<=max)){return true;}
				else return false;
			};
		//! checks containment of a point
		const bool iscontained(const double & point) const 
			{
				if ((point>=min)&& (point<=max)){return true;}
				else return false;
			};
			
		const APFloat APlength() const;			
		//Friends
		friend const rinterval dynamic(const rinterval &);
		friend const rapfloat mixing(const rinterval&, const rinterval &, const int &, const APFloat &);		
		friend std::ostream &operator<< (std::ostream &, const rinterval &);
		
		//Friend class		
		friend class rpartition;
};
#endif
