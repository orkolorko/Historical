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

//!\file eigenerr-lipschitz.cxx main for the program that computes the decay time (lipschitz)
//! \ingroup Lipschitz

#include <iostream>
#include "lipschitz/eigenrig-lipschitz.h"
#include <string>
#include <fstream>
#include <stdlib.h> 

using namespace std;

//! main function
//! \ingroup Lipschitz
int main (int argc, char *argv[])
{
		char nomefile[PETSC_MAX_PATH_LEN];
		PetscViewer           fd;
		PetscErrorCode        ierr;
		PetscBool            flg;
		
		Mat sA;
		Vec Norm;
		PetscScalar error=0.00000001;
		PetscScalar errorM=0.000000000000001;
		PetscInt intreport,rigerr;
		
		// Input of the arguments
		PetscInitialize(&argc,&argv,(char*)0,"-matrix mandatory for the matrix file\n\
												-error final diameter of the simplex\n\
												-rigerr set as 1 if rigorous estimate of the error\n\
												-intreport frequency of output\n\
												-output file for the output of the density\
												\n");
		MPI_Comm comm=PETSC_COMM_WORLD;
		
		PetscInt SizeRow,SizeCol;
		
		PetscOptionsGetString(PETSC_NULL,"-matrix",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -matrix option");
		
		PetscOptionsGetScalar(PETSC_NULL,"-error",&error,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate error with the -error option");
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-rigerr",&rigerr,&flg);CHKERRQ(ierr);
		if (!flg) rigerr=0;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-intreport",&intreport,&flg);CHKERRQ(ierr);
		if (!flg) intreport=1024;
		
		// Loading the matrix
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,nomefile,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		ierr=MatCreate(comm,&sA);CHKERRQ(ierr);
		ierr=MatLoad(sA,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		ierr=MatGetSize(sA,&SizeRow,&SizeCol);CHKERRQ(ierr);
		
		// Simple output, as a simple test
		if (SizeRow!=SizeCol)
		{
		ierr=PetscPrintf(comm,"The matrix is not square");CHKERRQ(ierr);
		PetscFinalize();
		return 0;
		}
		PetscInt size=SizeRow;
		
		ierr=PetscPrintf(comm,"Size:%d",size);CHKERRQ(ierr);
		
		// We set to some default values 
		int TimeEst=13;
		int TimeApprox=14;
		int Iterates=60;
		
		// If the flag rigerr is set  
		if (rigerr==1)
		{
			ierr=DecayTime(&sA,errorM,error,intreport,&TimeEst,&TimeApprox,&Iterates);CHKERRQ(ierr);
			ierr=PetscPrintf(comm,"TimeEst is %d, TimeApprox is %d\n",TimeEst,TimeApprox);CHKERRQ(ierr);
		}
		
		// This block computes the eigenvector of the matrix using the power
		// method 
		Vec yk,e,temp;
		PetscScalar DiffIterates;
		{
			// Allocate yk
			ierr=VecCreate(comm,&yk);CHKERRQ(ierr);
			// Sets the vector in the cluster
			ierr=VecSetSizes(yk,PETSC_DECIDE,size);CHKERRQ(ierr);
			ierr=VecSetFromOptions(yk);CHKERRQ(ierr);
			// Allocates e
			ierr=VecDuplicate(yk,&e);CHKERRQ(ierr);
			// e=(1,...,1)^T
			ierr=VecSet(e,1);CHKERRQ(ierr);
			// Assembly e
			ierr=VecAssemblyBegin(e);CHKERRQ(ierr);
			ierr=VecAssemblyEnd(e);CHKERRQ(ierr);
			// yk=sA*e
			ierr=MatMult(sA,e,yk);CHKERRQ(ierr);
			// Destroys e
			ierr=VecDestroy(&e);CHKERRQ(ierr);
			// allocates temp
			ierr=VecDuplicate(yk,&temp);CHKERRQ(ierr);
	
			for (int i=0; i<Iterates; i++)
			{
				//temp=yk
				ierr=VecCopy(yk,temp);CHKERRQ(ierr);
				//yk=sA*temp
				ierr=MatMult(sA,temp,yk);CHKERRQ(ierr);
				
				/* Every five iterations*/
				if (i%5==0)
				{
					/* Normalize the vector yk*/
					PetscScalar NormYk;
					ierr=VecNorm(yk,NORM_1,&NormYk);CHKERRQ(ierr);
					ierr=VecScale(yk,(1/NormYk));CHKERRQ(ierr);
					/* We check that nothing strange is happening*/
					ierr=VecAXPY(temp,-1,yk);CHKERRQ(ierr);
					ierr=VecNorm(temp,NORM_1,&DiffIterates);CHKERRQ(ierr);
					ierr=PetscPrintf(PETSC_COMM_WORLD,"We are at iteration %d and the difference is %e\n",i,DiffIterates);CHKERRQ(ierr);
				}
			}
			// Allocate Norm
			ierr=VecDuplicate(yk,&Norm);CHKERRQ(ierr);
			// Norm=yk
			ierr=VecCopy(yk,Norm);CHKERRQ(ierr);
			// Rescales to get the measure
			ierr=VecScale(Norm,PetscScalar(size));CHKERRQ(ierr);
		}
		
		// Outputs the computed eigenvector in text format
		PetscOptionsGetString(PETSC_NULL,"-output",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate output text file with the -output option");
		ierr=PetscViewerASCIIOpen(PETSC_COMM_WORLD,nomefile,&fd);CHKERRQ(ierr);
		ierr=PetscViewerSetFormat(fd,PETSC_VIEWER_ASCII_SYMMODU);CHKERRQ(ierr);
		ierr=VecView(Norm,fd);CHKERRQ(ierr);
		
		// Destroys the viewer
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		
		// Frees memory
		ierr=VecDestroy(&yk);CHKERRQ(ierr);
		ierr=VecDestroy(&Norm);CHKERRQ(ierr);
		
		PetscFinalize();
		return 0;
}
