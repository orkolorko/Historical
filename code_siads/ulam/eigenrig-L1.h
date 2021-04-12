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
//! \file eigenrig-L1.h headers for the estimate of the contraction time 
//! \ingroup Ulam

//Standard libraries
#include <iostream>
#include <fstream>
//PETSc
#include "petscmat.h" 
#include "petscksp.h"
using namespace std;
//! if petsc supports higher precision types, it can be substituted here
typedef PetscScalar APFloat;

//! computes the time needed to half the norm
int DecayTime(Mat const * const,APFloat,APFloat,PetscInt,PetscInt,PetscInt,int * const,int * const,int * const); 
