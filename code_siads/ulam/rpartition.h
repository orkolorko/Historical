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
//!\file  rpartition.h contains the rpartition header
//!\ingroup Ulam

#include "rapfloat.h"
#include "rinterval.h"

#ifndef RPARTITION_H
#define RPARTITION_H

typedef std::vector<rinterval> Container;
//! \brief this class represents a partition of the interval
/*! This class is nothing else than a list/vector of intervals, which represent
 *  a partition of the interval [0,1]
 */
//!\ingroup Ulam
class rpartition
{
		
		Container value;
		
		public:	
		//! returns the interval with index i
		const rinterval & read_value(int i){return value[i];};
		//! returns the size of the partition
		const int get_size(){return value.size();};
		//Constructor
		//! empty constructor
		rpartition();
		//! copy constructor
		rpartition(const rpartition &); 
		//! makes a homogeneous partition of an interval in "size" pieces
		rpartition(const rinterval &, int size); 
		//! joins two partitions
		rpartition(const rpartition &, const rpartition &);
		
		//Member functions
		//! assignment operator
		rpartition& operator=(const rpartition& );
		
		//Destructor
		//Friend
		//! outputs the partition
		void printpartition(ofstream &) const;
		//! outputs the dynamic
		void printdynamic(ofstream &) const;
		//! \brief the function that computes the values
		/*! The function that computes the values uses the rpartition object in
		 * its recursive part
		 * */
		friend const rapfloat mixing(const rinterval&, const rinterval &, const int&, const APFloat &);
};
#endif
