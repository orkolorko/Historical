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

//! \file dynamic-lipschitz.cxx implementation of dynamic-lipschitz.h
//! \ingroup Lipschitz
#include "dynamic-lipschitz.h"
#include <utility>

PetscInt dynamicestimate(const std::pair<PetscInt*,PetscScalar*> &inputs)
{
		PetscInt *inputAddr=inputs.first;
		PetscScalar *output=inputs.second;
		
		REAL num(inputAddr[0]);
		REAL den(inputAddr[1]);
		REAL point(num/den);
		REAL temp=dynamic(point);
		REAL der=dynamic_der(point);
		output[0]=double(temp);
		output[1]=double(der);
		return 0;
}

//4x+0.01*sin(8\pi*x)
REAL aux_dynamic(const REAL &x)
{
	REAL temp;
	temp=REAL(4)*x+REAL(0.01)*sin(REAL(8)*pi()*x);
	return temp;
}

REAL aux_dynamic_der (const REAL &x)
{
	REAL temp;
	temp=REAL(4)+REAL(0.01)*8*pi()*cos(REAL(8)*pi()*x);
	return temp;
}

REAL aux_dynamic_der_der (const REAL &x)
{
	REAL temp;
	temp=REAL(-0.01)*64*power(pi(),2)*sin(REAL(8)*pi()*x);
	return temp;
}

REAL dynamic(const REAL &x)
{
	return aux_dynamic(x);
}

REAL dynamic_der (const REAL &x)
{
	return aux_dynamic_der(x);
}

PetscScalar DoubleDynamic(PetscInt i, PetscInt j)
{
	REAL num(i);
	REAL den(j);
	REAL point=num/den;
	REAL temp=dynamic(point);
	return double(temp);
}
