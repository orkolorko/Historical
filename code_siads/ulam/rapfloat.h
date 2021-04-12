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

//! \file rapfloat.h this file holds the code for the non-rigorous scalar types
//!\ingroup Ulam

//Stl libraries
#include <vector>
#include <list>
//Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
//#include <boost/multiprecision/gmp.hpp>
#include <boost/rational.hpp>

using namespace std;
//using namespace boost::multiprecision;

/*! \brief the scalar that hold the entries of the matrix
 */
 //!\ingroup Ulam
typedef double APFloat;
/*!\brief Exact precision rational type
 *
 * This is the type that stores the exact precision rational types
 * can be changed to other rational types without much work
 * in particular the new multiprecisions type from boost
 */
 //!\ingroup Ulam
typedef boost::rational<long int> RigScalar; //boost::numeric::interval<APFloat>

#ifndef RAPFLOAT_H
#define RAPFLOAT_H
//Classes

//! A scalar that takes care of the error
/*!
 * This class is used to pass the computed value of the matrix entries
 * together with the error made in computing them
 */
 //!\ingroup Ulam

class rapfloat
{
	APFloat value;
    APFloat error;
	
	public:
	//Constructors
	//! The empty constructor
	/*!
	 * Sets both values to 0
	 */
	rapfloat();
	//! Constructor with two values
	/*!
	 * \param value
	 * \param error the error range
	 */
	rapfloat(const APFloat &value, const APFloat &error);
	
	//Useful functions
	//! return the value
	APFloat getvalue(){return value;};
	//! returns the error
	APFloat geterror(){return error;};
	//! certain comparison operator
	bool operator>=(const APFloat &test) const;
	
	//Operator overloading
	//! + operator that takes care of the error
	friend rapfloat operator+ (const rapfloat &a, const rapfloat &b);
    //! assignement operator
    rapfloat& operator=(const rapfloat& );
    
	//Friend functions
	//! overloaded ostream operator
	friend std::ostream &operator<< (std::ostream &os, const rapfloat &a)
	{
		return os << "value = " << a.value << ", error = " << a.error;
	}
};
#endif
