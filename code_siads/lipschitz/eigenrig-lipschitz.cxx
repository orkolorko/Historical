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

//! \file eigenrig-lipschitz.cxx implements eigenrig-lipschitz.h
//! \ingroup Lipschitz


#include "eigenrig-lipschitz.h"
#include <fenv.h>
#include <limits>


// Computes the decay time bound for the matrix Pk and the decay time of the matrix \Pi
int DecayTime(Mat const * const Aaddr,APFloat MatError,APFloat error,PetscInt intreport, int * const TimeEstAddr,int * const TimeApproxAddr,int * const IteratesAddr) 
{
	const int originalRounding = fegetround( );
	
	int TimeEst=0;
	int TimeApprox=0;
	int Iterates=0;
	
	
	PetscErrorCode ierr;
	PetscInt sizerow;
	PetscInt sizecol;
	PetscInt size;
	
	MPI_Comm comm=PETSC_COMM_WORLD;
	
	
	double AbsError=std::numeric_limits<double>::epsilon();								 	
	
	Vec basnullspace;
	Vec temp;
	
	// the maximum number of non-zeros can be computed by the markovization algorithm or while generating the matrix...
	// 16 works for all the examples in our paper
	int nnz=16;
	
	
	// This array is going to store the C_i of the 
	PetscScalar Norms[256];
	for (int i=0;i<256;i++)
	{
		Norms[i]=0;
	}
	
	//upper estimate for the multiplication error, following numerical remark 1
	PetscScalar gamma_k=nnz*std::numeric_limits<double>::epsilon()/(1-nnz*std::numeric_limits<double>::epsilon());
	
	ierr=PetscPrintf(PETSC_COMM_WORLD,"gammma_k %e\n",gamma_k);CHKERRQ(ierr);
	
	
	// we check the matrix is square
	ierr=MatGetSize(*Aaddr,&sizerow,&sizecol);CHKERRQ(ierr);
	if (sizerow==sizecol)
	{
		size=sizerow;
	}
	
	// Create the memory structures that are going to hold the 
	// vectors of type e_0-e_i
	ierr=VecCreate(comm,&basnullspace);CHKERRQ(ierr);
	ierr=VecSetSizes(basnullspace,PETSC_DECIDE,size);CHKERRQ(ierr);
	ierr=VecSetFromOptions(basnullspace);CHKERRQ(ierr);
	ierr=VecDuplicate(basnullspace,&temp);CHKERRQ(ierr);
	
	for (int j=1; j<sizecol;j++)
	{
		// Sets the entries of the vector
		// First of all we set all the entries to 0
		ierr=VecZeroEntries(basnullspace);CHKERRQ(ierr);
		ierr=VecZeroEntries(temp);CHKERRQ(ierr);
		// We set the vector basnullspace to e_0-e_j
		ierr=VecSetValue(basnullspace,0,1,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecSetValue(basnullspace,j,-1,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(basnullspace);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(basnullspace);CHKERRQ(ierr);
		// temp=basnullspace
		ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
		
		// we set the norm to 1
		PetscReal norm=1;
		// for each vector, i computes the number of iterates
		int i=0;
		
		// for each j, we use these entries to be sure 
		// that we get the minimum time needed to half the norm of the vector 
		bool TimeEstBool=true;
		bool TimeApproxBool=true;
		
		// this is an upper estimate of the norm of the matrix P_k 
		double normdec=norm+i*size*AbsError+i*2*gamma_k;
		
		while (normdec>=error)
		{
			// we add one to the iteration counter
			i++;
			
			// this is an output, in case something is going really wrong
			if (normdec>2.5)
				{
					ierr=PetscPrintf(PETSC_COMM_WORLD,"Non sta contraendo!",j);CHKERRQ(ierr);
					return 1;
				}
			// basnullspace=A*temp
			ierr=MatMult(*Aaddr,temp,basnullspace);CHKERRQ(ierr);
			// temp=basnullspace
			ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
			// we compute the infinity norm of temp
			ierr=VecNorm(temp,NORM_INFINITY,&norm);CHKERRQ(ierr);
			// since ||e_i-e_j||<= 2*max_i ||e_1-e_i||
			// we take her two times of norm
			norm*=2;
			
			// if norm is smaller than 0.5 and it is the first time
			// such that the norm is smaller than 0.5 we store the time
			if ((norm<=0.5) && (TimeEstBool==true))
				{
					if (i>TimeEst) TimeEst=i;
					TimeEstBool=false;
				}
			
			// normdec is the upper estimate of the norm of the abstract
			// discretized operator
			normdec=norm+i*size*AbsError+i*2*gamma_k;
			
			// if normdec is smaller than 0.5 and it is the first time
			// such that the normdec is smaller than 0.5 we store the time
			
			if ((normdec<=0.5) && (TimeApproxBool==true))
				{
					if (i>TimeApprox) TimeApprox=i;
					TimeApproxBool=false;
				}
			
			// we want to store the biggest of the normdec as j varies
			if (normdec>=Norms[i])
			{
				Norms[i]=normdec;
			}
			// we store the maximum of the i needed to contract the simplex
			if (i>Iterates) Iterates=i;
		}
		//output
		if (j%intreport==0) 
		{
			ierr=PetscPrintf(PETSC_COMM_WORLD,"Vector %d, TimeEst %d, TimeApprox %d,Iterates %d\n",j,TimeEst,TimeApprox,Iterates);
		}
	}
	//Updates the output data
	*TimeApproxAddr=TimeApprox;
	*TimeEstAddr=TimeEst;
	*IteratesAddr=Iterates;
	
	double normsum=0;
	Norms[0]=1;
	for (int i=0;i<TimeApprox+1;i++)
	{
		normsum+=Norms[i];
		ierr=PetscPrintf(PETSC_COMM_WORLD,"C%d=%f\n",i,Norms[i]);CHKERRQ(ierr);
	}
	ierr=PetscPrintf(PETSC_COMM_WORLD,"NormSum=%f\n",normsum);CHKERRQ(ierr);
	//Frees the memory
	ierr=VecDestroy(&basnullspace);CHKERRQ(ierr);
	ierr=VecDestroy(&temp);CHKERRQ(ierr);
	
	//Outputs
	ierr=PetscPrintf(PETSC_COMM_WORLD,"Timeapprox %d\n",*TimeApproxAddr);CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"TimeEst %d\n",*TimeEstAddr);CHKERRQ(ierr);
	fesetround(originalRounding);
	
	return 0;
}
