/*
 *	Copyright 2011-2013 Stefano Galatolo (galatolo@mail.dm.unipi.it), 
 * 						Isaia Nisoli (nisoli@im.ufrj.br) 
 *	
 *	This file is part of CompInvMeas-Lip.
 *
 *  CompInvMeas-Lip is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CompInvMeas-Lip is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CompInvMeas-Lip.  If not, see <http://www.gnu.org/licenses/>.
*/
//! \file actualgen-lipschitz.h general headers for generating the matrix 
//! \ingroup Lipschitz

#include "iRRAM/lib.h"
#include "petscmat.h"

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 


using namespace std;
using namespace iRRAM;

/*! \brief ausiliary input class for iRRAM
*/
/*!  This auxiliary class is used to pass inputs to the iRRAM, since the iRRAM
 * environment can only be called through the templated  
 * iRRAM_exec() function.
 */
//! \ingroup Lipschitz
class inputvalue
{
	public:
	//! the index of I_i
	PetscInt i;
	//! the index of I_j
	PetscInt j;
	//! the size of the partition
	PetscInt size; 
	//! constructor
	inputvalue(PetscInt i1,PetscInt j1, PetscInt size1)
	{
		i=i1;
		j=j1;
		size=size1;
	}	
};

/*! \brief ausiliary output class for iRRAM
*/
/*! This auxiliary class is used to pass output addresses to the iRRAM, since the iRRAM
 * environment can only be called through the templated  
 * iRRAM_exec() function.
 */
 //! \ingroup Lipschitz
class outputvalue
{
	public:
	//! pointer to the value
	PetscScalar * valueAddr;
	//! pointer to the error
	PetscScalar * errorAddr;
	//!constructor
	outputvalue(PetscScalar * valueAddr1, PetscScalar * errorAddr1)
	{
		valueAddr=valueAddr1;
		errorAddr=errorAddr1;
	}
};
/*! \brief computes the entries*/
/*! This function computes the entries of the matrix, it is used inside the 
 * iRRAM_exec environment. Since the iRRAM_exec environment
 * is defined, in the 2013_01 version, as a template
 * template <class ARGUMENT, class RESULT> RESULT iRRAM_exec (RESULT iRRAM_compute(const ARGUMENT&), const ARGUMENT& x)
 * it is possible to pass only one argument
 */
 //! \ingroup Lipschitz
PetscInt ComputeValue(const std::pair<inputvalue,outputvalue> &);

/*!\brief generates the matrix*/
/*! \param size the size of the partition
 * \param step the size of the intervals we throw away
 * \param Aaddr a pointer to the matrix A
 * \param MatErrorAddr a pointer to a variable that stores the global error of the matrix
 * \param intreport controls frequency of output
 * \param debug controls whether to output debug information
 * */
 //! \ingroup Lipschitz
PetscInt genmatrix_lipschitz(PetscInt size, Mat * const Aaddr, PetscScalar * const MatErrorAddr,int intreport,bool debug);

/*These are the function that implement the primitive, the numbers denote which branch are we in of the tent maps
 the number 1 means that the tent is increasing, the number 2 that it is decreasing.
 The first number refers to L\phi_j and the second number to \phi_i
 */
//! Primitive, product of increasing,increasing branches
//! \ingroup Lipschitz
REAL primitive11(REAL T1,REAL Td, int j,int l, int size, REAL x);
//! Primitive, product of increasing,decreasing branches
//! \ingroup Lipschitz
REAL primitive12(REAL T1,REAL Td, int j,int l, int size, REAL x);
//! Primitive, product of decreasing,increasing branches
//! \ingroup Lipschitz
REAL primitive21(REAL T1,REAL Td, int j,int l, int size, REAL x);
//! Primitive, product of decreasing,decreasing branches
//! \ingroup Lipschitz
REAL primitive22(REAL T1,REAL Td, int j,int l, int size, REAL x);

