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

/*! \file actual-genmatrix-ulam.h
\brief Headers for the actual algorithm that computes the matrix
\ingroup Ulam
*/
 
#include <list>
#include "petscmat.h"
#include <boost/math/special_functions/fpclassify.hpp>
#include "dynamic.h"


#ifndef ACTUAL_GENMATRIX
#define ACTUAL_GENMATRIX

/*In this function we compute the Ulam approximation of the Matrix,
	using the exaustion algorithm explained in the article.
	Please note that the algorithm is portable, with little effort to other
	rigorous numerical types.
	*/

PetscErrorCode genmatrix_ulam(PetscInt partsize,PetscScalar max_error, PetscInt intreport,bool debug,Mat * const Aaddr,  PetscScalar * const MatErrorAddr);
#endif
