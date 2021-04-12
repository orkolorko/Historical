//      markovize.cxx
//      
//      Copyright 2012 Isaia Nisoli <isaia@aiace2>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include "petscmat.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 

typedef unsigned long int unlongint;
using namespace std;

PetscInt markovize(Mat const inputMatrix, Mat * const outputMatrixAddr)
{
	
	PetscErrorCode        ierr;
	PetscInt sizerow,sizecol;
	Vec Column;

	unlongint NormalizingInteger=1125899906842624;
	ierr=VecCreate(PETSC_COMM_WORLD,&Column);CHKERRQ(ierr);
	//ierr=VecSetFromOptions(Column);CHKERRQ(ierr);
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
				IntDiff-= unlongint(value*NormalizingInteger);	
				nnz++;
			}
		}
		//std::cout << nnz << std::endl;
		PetscInt spread=IntDiff/nnz;
		PetscInt rest=IntDiff%nnz;
		//std::cout << spread << std::endl;
		//std::cout << rest << std::endl;
		for (int i=0;i<sizerow;i++)
		{
			PetscScalar value;
			ierr=VecGetValues(Column,1,&i,&value);CHKERRQ(ierr);
			if (value!=0)
			{
				//std::cout << value << std::endl;		
				unlongint IntValue=unlongint(value*NormalizingInteger);
				//std::cout << IntValue << std::endl;		
				
				if (nnz!=1) IntValue+=spread;
				if (nnz==1) IntValue+=spread+rest;
				//std::cout << IntValue << std::endl;		
				
				PetscScalar newvalue=double(IntValue)/double(NormalizingInteger);
				//std::cout << newvalue << std::endl;
		
				MatSetValue(*outputMatrixAddr,i,j,newvalue,INSERT_VALUES);
				nnz--;
			}
		}
		//std::cin.get();
	}
	
	return 0;
}

PetscInt main (PetscInt argc, char *argv[])
{
		string nomefile;
		
		PetscReal step=0.00000000000000000000001;
		PetscInt sizerow,sizecol;
		PetscViewer           fd;
		
		char                  fileinput[PETSC_MAX_PATH_LEN];     /* input file name */
		char                  fileoutput[PETSC_MAX_PATH_LEN];     /* input file name */
		
		char                  labile[PETSC_MAX_PATH_LEN]; /* input for the PetscPetscInt */ 
		PetscErrorCode        ierr;
		PetscTruth            flg;
		

		PetscInitialize(&argc,&argv,(char*)0,"-size mandatory for the size of the partition default=16384\
												\n -output mandatory for the output file\
												\n -error optional for the error default=1e-23");
		
		MPI_Comm comm=PETSC_COMM_WORLD;
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Ho inizializzato PETSC\n");CHKERRQ(ierr);
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-input",fileinput,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(1,"Must indicate text file with the -input option");
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-output",fileoutput,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(1,"Must indicate text file with the -output option");
		
		
		Mat A;
		PetscScalar MatError(0);
		
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinput,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		ierr=MatLoad(fd,MATSEQAIJ,&A);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(fd);CHKERRQ(ierr);		
		
		Mat B;
		ierr=MatDuplicate(A,MAT_SHARE_NONZERO_PATTERN,&B);CHKERRQ(ierr);
		
		ierr=markovize(A,&B);CHKERRQ(ierr);
		ierr=MatAssemblyBegin(B,MAT_FINAL_ASSEMBLY);
		ierr=MatAssemblyEnd(B,MAT_FINAL_ASSEMBLY);
		
		ierr=PetscViewerBinaryOpen(comm,fileoutput,FILE_MODE_WRITE,&fd);CHKERRQ(ierr);
		ierr=MatView(B,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(fd);CHKERRQ(ierr);		
		ierr=MatView(B,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);
		ierr=MatGetSize(B,&sizerow,&sizecol);CHKERRQ(ierr);
		
		std::cin.get();
		ierr=MatDestroy(A);CHKERRQ(ierr);
		ierr=MatDestroy(B);CHKERRQ(ierr);
		
		std::cout << sizerow << " " << sizecol << endl;
		
		return 0;
}
