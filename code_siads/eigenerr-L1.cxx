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

//!\file eigenerr-L1.cxx main for the program that computes the decay time (ulam)

#include <iostream>
#include "ulam/eigenrig-L1.h"
#include <string>
#include <fstream>
#include <stdlib.h> 

using namespace std;

//! main function
//!\ingroup Ulam
int main (int argc, char *argv[])
{
		char nomefile[PETSC_MAX_PATH_LEN];
		char labile[PETSC_MAX_PATH_LEN];
		PetscViewer           fd;
		PetscErrorCode        ierr;
		PetscBool            flg;
		
		Mat sA;
		Vec Normalized;
		/*Iter Norm;*/
		PetscScalar error=0.00000001;
		PetscScalar errorM=0.000000000000001;
		
		int Iterates=30;
		
		PetscInitialize(&argc,&argv,(char*)0,"-matrix mandatory for the matrix file\n\
												-error final diameter of the simplex\n\
												-errorM error on the coefficients of the matrix\n\
												-rigerr set as 1 if rigorous estimate of the error\n\
												-intreport frequency of output\n\
												-output file for the output of the density\
												\n");
		MPI_Comm comm=PETSC_COMM_WORLD;
		
		PetscInt SizeRow,SizeCol;
		PetscInt rigerr=0;
		PetscInt start=0;
		PetscInt end;
		PetscInt intreport;
		
		// Input of the arguments
		PetscOptionsGetScalar(PETSC_NULL,"-error",&error,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate error with the -error option");
		
		PetscOptionsGetScalar(PETSC_NULL,"-errorM",&errorM,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate error with the -errorM option");
		
		PetscOptionsGetString(PETSC_NULL,"-matrix",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate matrix file with the -matrix option");
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-rigerr",&rigerr,&flg);CHKERRQ(ierr);
		if (!flg) rigerr=0;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-intreport",&intreport,&flg);CHKERRQ(ierr);
		if (!flg) intreport=1024;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-start",&start,&flg);CHKERRQ(ierr);
		ierr=PetscOptionsGetInt(PETSC_NULL,"-iter",&Iterates,&flg);CHKERRQ(ierr);
		
		//Loading the matrix
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,nomefile,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		ierr=MatCreate(comm,&sA);CHKERRQ(ierr);
		ierr=MatLoad(sA,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		ierr=MatGetSize(sA,&SizeRow,&SizeCol);CHKERRQ(ierr);
		
		// simple test on the matrix
		if (SizeRow!=SizeCol)
		{
		ierr=PetscPrintf(comm,"La matrice non è quadrata");CHKERRQ(ierr);
		PetscFinalize();
		return 0;
		}
		PetscInt size=SizeRow;
		
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-end",&end,&flg);CHKERRQ(ierr);
		if (!flg) end=size;
			
		int TimeEst=9;
		int TimeApprox=10;
		
		// if the rigerr flag is set, compute N, N_e and l
		if (rigerr==1)
		{
			ierr=DecayTime(&sA,errorM,error,intreport,start,end,&TimeEst,&TimeApprox,&Iterates);CHKERRQ(ierr);
			ierr=PetscPrintf(comm,"TimeEst is %d, TimeApprox is %d\n",TimeEst,TimeApprox);CHKERRQ(ierr);
			ierr=PetscPrintf(comm,"Iterates is %d\n",Iterates);CHKERRQ(ierr);
		}
		
		// Compute the eigenvector using the power method
		Vec yk,e,temp;
		PetscScalar DiffIterates;
		PetscScalar NormYk;
		
		//Allocate yk
		ierr=VecCreate(comm,&yk);CHKERRQ(ierr);
		//dim(yk)=size
		ierr=VecSetSizes(yk,PETSC_DECIDE,size);CHKERRQ(ierr);
		ierr=VecSetFromOptions(yk);CHKERRQ(ierr);
		//Allocate e
		ierr=VecDuplicate(yk,&e);CHKERRQ(ierr);
		//e=(1,...1)^T
		ierr=VecSet(e,1);CHKERRQ(ierr);
		//Assembly
		ierr=VecAssemblyBegin(e);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(e);CHKERRQ(ierr);
		//yk=sA*e
		ierr=MatMult(sA,e,yk);CHKERRQ(ierr);
		//Destroy
		ierr=VecDestroy(&e);CHKERRQ(ierr);
		//Allocate temp
		ierr=VecDuplicate(yk,&temp);CHKERRQ(ierr);
		
		//Normalize yk
		ierr=VecNorm(yk,NORM_1,&NormYk);CHKERRQ(ierr);
		ierr=VecScale(yk,(1/NormYk));CHKERRQ(ierr);
		
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
					/* Check if we have reached the stopping condition*/
					ierr=VecAXPY(temp,-1,yk);CHKERRQ(ierr);
					ierr=VecNorm(temp,NORM_1,&DiffIterates);CHKERRQ(ierr);
					ierr=PetscPrintf(PETSC_COMM_WORLD,"We are at iteration %d and the difference is %e\n",i,DiffIterates);CHKERRQ(ierr);
				}
			}
		//Allocate normalized
		ierr=VecDuplicate(yk,&Normalized);CHKERRQ(ierr);
		ierr=VecCopy(yk,Normalized);CHKERRQ(ierr);
		
		//Outputs the computed density
		PetscOptionsGetString(PETSC_NULL,"-output",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate output text file with the -output option");
		ierr=PetscViewerASCIIOpen(PETSC_COMM_WORLD,nomefile,&fd);CHKERRQ(ierr);
		ierr=PetscViewerSetFormat(fd,PETSC_VIEWER_ASCII_SYMMODU);CHKERRQ(ierr);
		ierr=VecView(Normalized,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		
		//Frees memory
		ierr=VecDestroy(&yk);CHKERRQ(ierr);
		ierr=VecDestroy(&Normalized);CHKERRQ(ierr);
		
		PetscFinalize();
		return 0;
}
