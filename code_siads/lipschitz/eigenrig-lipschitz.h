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
//! \file eigenrig-lipschitz.h computes "N" in the lipschitz case
//! \ingroup Lipschitz

//Standard libraries
#include <iostream>
#include <fstream>
//PETSc
#include "petscmat.h" 
#include "petscksp.h"
using namespace std;
//! non rigorous scalars, if petsc supports higher precision, easily changed
//! \ingroup Lipschitz
typedef PetscScalar APFloat;

//! computes the decay time for the lipschitz approximation
//! \ingroup Lipschitz
int DecayTime(Mat const * const,APFloat,APFloat,PetscInt,int * const,int * const,int * const); 
