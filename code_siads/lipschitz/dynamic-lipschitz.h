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

//! \file dynamic-lipschitz.h headers for the dynamic (lipschitz)
//! \ingroup Lipschitz

#include "iRRAM/lib.h"
#include "petscmat.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 
#include <utility>

using namespace iRRAM;

//! given a rational point, computes value and derivative 
//! \ingroup Lipschitz
PetscInt dynamicestimate(const std::pair<PetscInt*,PetscScalar*> &inputs);

//! dynamic
//! \ingroup Lipschitz
REAL dynamic (const REAL &x);
//! derivative
//! \ingroup Lipschitz
REAL dynamic_der (const REAL &x);
//! second derivative
//! \ingroup Lipschitz
REAL dynamic_der_der (const REAL &x);

//! auxiliary, used sometimes with quotients
//! \ingroup Lipschitz
REAL aux_dynamic (const REAL &x);
//! auxiliary, used sometimes with quotients
//! \ingroup Lipschitz
REAL aux_dynamic_der (const REAL &x);
//! auxiliary, used sometimes with quotients
//! \ingroup Lipschitz
REAL aux_dynamic_der_der (const REAL &x);

//! given a rational point outputs the image as a double
//! \ingroup Lipschitz
PetscScalar DoubleDynamic(PetscInt i, PetscInt j);
