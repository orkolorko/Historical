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

/*! \file dynamic.h
 * \brief contains the headers for the dynamic
	\ingroup Ulam
 */
/*!
 * When changing the dynamic, you have to modify the file dynamic.cxx
 * and the checkdiscontinuity function
 */

#include "rapfloat.h"


#ifndef DYNAMIC_H
#define DYNAMIC_H

class rinterval;

/*! \brief The function that computes the dynamic
 * This function  computes the image of an interval, please note that
 * since the functions are all rational, when dealing with intervals
 * with rational endpoints it outputs an interval with exact rational endpoints
 */
RigScalar dyn(const RigScalar &point);
/*! \brief checks if there is a discontinuity in the interval
 *  Please note that this function depends on the dynamic that we are studying
 * it is necessary to implement this function when changing the dynamics
 * \warning change this function when changing the dynamic
 */
bool checkdiscontinuity(rinterval &input);

#endif
