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
//! \defgroup Lipschitz Lipschitz approximation

//!\file genmatrix-lipschitz.cxx main for the generation of the lipschitz matrix
//! \ingroup Lipschitz

#include "iRRAM/lib.h"
#include "petscmat.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 
#include <ctime>
#include "lipschitz/actualgen-lipschitz.h"

using namespace std;
using namespace iRRAM;

//! main function
//! \ingroup Lipschitz
PetscInt main (PetscInt argc, char *argv[])
{
		clock_t start = clock();

		string nomefile;
		PetscInt partsize=65536;
		PetscInt intreport=64;
		PetscInt inputdebug;
		bool debug;
		
		PetscReal step=0.00000000000000000000001;
		PetscViewer           fd;
		
		char                  file[PETSC_MAX_PATH_LEN];     /* input file name */
		
		PetscErrorCode        ierr;
		PetscBool            flg;
		
		iRRAM_initialize(argc,argv);
		
		// Input of the arguments
		PetscInitialize(&argc,&argv,(char*)0,"-size mandatory for the size of the partition default=16384\
												\n -output mandatory for the output file\
												\n -intreport frequency of output\
												\n -outdebug trigger of debug output");
		
		MPI_Comm comm=PETSC_COMM_WORLD;
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Ho inizializzato PETSC\n");CHKERRQ(ierr);
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-output",file,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -output option");
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-size",&partsize,&flg);CHKERRQ(ierr);
		if (!flg) partsize=65536;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-intreport",&intreport,&flg);CHKERRQ(ierr);
		if (!flg) intreport=64;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-outdebug",&inputdebug,&flg);CHKERRQ(ierr);
		if (!flg) inputdebug=0;
		
		if (inputdebug==0) debug=false;
		if (inputdebug==1) debug=true;
		
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The dimension of the partition is %d, and the error on the coefficients is of the order of %e\n",partsize,step*PetscScalar(partsize));CHKERRQ(ierr);
		
		// Declaration of the objects which we are going to compute 
		Mat A;
		PetscScalar MatError(0);
		
		// Allocation of the matrix
		ierr=MatCreateSeqAIJ(PETSC_COMM_WORLD,partsize,partsize,10,PETSC_NULL,&A);CHKERRQ(ierr);
		ierr=MatSetFromOptions(A);CHKERRQ(ierr);
		
		// Generates the matrix
		ierr=genmatrix_lipschitz(partsize,&A,&MatError,intreport,debug);CHKERRQ(ierr);
		
		// Assembles the matrix
		ierr=MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
		ierr=MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
		
		// Simple test, we check the norm of the matrix
		PetscScalar norm;
		ierr=MatNorm(A,NORM_1,&norm);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The 1-norm of the matrix is %e\n",norm);CHKERRQ(ierr);
		
		// Outputs the matrix
		ierr=PetscViewerBinaryOpen(comm,file,FILE_MODE_WRITE,&fd);CHKERRQ(ierr);
		ierr=MatView(A,fd);CHKERRQ(ierr);
		
		//Destroys the viewer
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);		
		
		// Outputs the running time
		clock_t ends = clock();
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Running time: %e\n",(double) (ends - start) / CLOCKS_PER_SEC);CHKERRQ(ierr);
		
		// Destroys the matrix 
		ierr=MatDestroy(&A);CHKERRQ(ierr);
		
		return 0;
}
