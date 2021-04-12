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

//! \file markovize.cxx implements the markovization

#include "petscmat.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 

typedef unsigned long int unlongint;
using namespace std;
//! markovizes the matrices
PetscInt markovize(Mat const inputMatrix, Mat * const outputMatrixAddr)
{
	
	PetscErrorCode        ierr;
	PetscInt sizerow,sizecol;
	Vec Column;
	
	// this integer is used to implement the markovization: the idea
	// is to multiply the values for this normalizing integer
	// and then cast them to integer, since this is a big power of two
	// we are only changing the exponent, leaving the mantissa untouched
	// We are truncating the floating point numbers 
	unlongint NormalizingInteger=1125899906842624;
	
	ierr=VecCreate(PETSC_COMM_WORLD,&Column);CHKERRQ(ierr);
	ierr=VecSetType(Column,VECSEQ);CHKERRQ(ierr);
	
	ierr=MatGetSize(inputMatrix,&sizerow,&sizecol);CHKERRQ(ierr);
	ierr=VecSetSizes(Column,PETSC_DECIDE,sizerow);
	
	for (PetscInt j=0;j<sizecol;j++)
	{
		if (j%16==0) std::cout << j << std::endl;
		PetscInt nnz=0;
		
		unlongint IntDiff=NormalizingInteger;
		ierr=MatGetColumnVector(inputMatrix,Column,j);CHKERRQ(ierr);
		for (int i=0;i<sizerow;i++)
		{
			PetscScalar value;
			ierr=VecGetValues(Column,1,&i,&value);CHKERRQ(ierr);
			if (value!=0)
			{
				// since we want to assure that the sum is exactly one
				// we subtract the values in the column from the 
				// normalizing integer
				IntDiff-= unlongint(value*NormalizingInteger);	
				nnz++;
			}
		}
		// we spread the difference between the normalizing integer
		// and the values in the column evenly between the numbers 
		// in the column
		PetscInt spread=IntDiff/nnz;
		PetscInt rest=IntDiff%nnz;
		for (int i=0;i<sizerow;i++)
		{
			PetscScalar value;
			ierr=VecGetValues(Column,1,&i,&value);CHKERRQ(ierr);
			if (value!=0)
			{
				unlongint IntValue=unlongint(value*NormalizingInteger);
				
				if (nnz!=1) IntValue+=spread;
				if (nnz==1) IntValue+=spread+rest;
				
				PetscScalar newvalue=double(IntValue)/double(NormalizingInteger);
				
				MatSetValue(*outputMatrixAddr,i,j,newvalue,INSERT_VALUES);
				nnz--;
			}
		}
	}
	
	return 0;
}

//! main for markovize
PetscInt main (PetscInt argc, char *argv[])
{
		string nomefile;
		
		PetscInt sizerow,sizecol;
		PetscViewer           fd;
		
		char                  fileinput[PETSC_MAX_PATH_LEN];     /* input file name */
		char                  fileoutput[PETSC_MAX_PATH_LEN];     /* input file name */
		
		PetscErrorCode        ierr;
		PetscBool          flg;
		
		//
		PetscInitialize(&argc,&argv,(char*)0,"-input mandatory for the input file\
												\n -output mandatory for the output file\n");
		
		MPI_Comm comm=PETSC_COMM_WORLD;
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Ho inizializzato PETSC\n");CHKERRQ(ierr);
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-input",fileinput,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(comm,1,"Must indicate matrix file with the -input option");
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-output",fileoutput,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(comm,1,"Must indicate matrix file with the -output option");
		
		//Loads the input matrix
		Mat A;
		
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinput,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		
		ierr=MatCreate(comm,&A);CHKERRQ(ierr);
		ierr=MatLoad(A,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);		
		
		Mat B;
		//Allocates memory for B
		ierr=MatDuplicate(A,MAT_SHARE_NONZERO_PATTERN,&B);CHKERRQ(ierr);
		
		//Markovize B
		ierr=markovize(A,&B);CHKERRQ(ierr);
		//Assembly B
		ierr=MatAssemblyBegin(B,MAT_FINAL_ASSEMBLY);
		ierr=MatAssemblyEnd(B,MAT_FINAL_ASSEMBLY);
		//output B
		ierr=PetscViewerBinaryOpen(comm,fileoutput,FILE_MODE_WRITE,&fd);CHKERRQ(ierr);
		ierr=MatView(B,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);		
		//show the nonzero structure of B on screen
		ierr=MatView(B,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);
		ierr=MatGetSize(B,&sizerow,&sizecol);CHKERRQ(ierr);
		
		std::cin.get();
		//frees memory
		ierr=MatDestroy(&A);CHKERRQ(ierr);
		ierr=MatDestroy(&B);CHKERRQ(ierr);
		
		//output of the dimensions, just to have a check
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Dimensions of B, %d x %d \n",sizerow,sizecol);CHKERRQ(ierr);
			
		return 0;
}
