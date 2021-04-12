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

//! \defgroup Ulam Ulam approximation
//!\file genmatrix-ulam.cxx main for the generation of the ulam matrix
//!\ingroup Ulam

#include "petscmat.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 
#include <ctime>
#include <boost/rational.hpp>
#include "ulam/actual-genmatrix-ulam.h"

//! rational type, easily changed to other types 
typedef boost::rational<long int> Rational;

using namespace std;

//! main function
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
		PetscBool            	flg;
		PetscInt				step_exp;
		
		//Input of the arguments
		PetscInitialize(&argc,&argv,(char*)0,"-size mandatory for the size of the partition default=16384\
												\n -output mandatory for the output file\
												\n -error_exp optional for the exponent of the error, base 2 default=-50\
												\n -intreport frequency of output\
												\n -outdebug trigger of debug output\
												");
		
		MPI_Comm comm=PETSC_COMM_WORLD;
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Ho inizializzato PETSC\n");CHKERRQ(ierr);
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-output",file,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -output option");
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-size",&partsize,&flg);CHKERRQ(ierr);
		if (!flg) partsize=65536;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-intreport",&intreport,&flg);CHKERRQ(ierr);
		if (!flg) intreport=1024;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-outdebug",&inputdebug,&flg);CHKERRQ(ierr);
		if (!flg) inputdebug=0;
		
		if (inputdebug==0) debug=false;
		if (inputdebug==1) debug=true;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-error_exp",&step_exp,&flg);CHKERRQ(ierr);
		if (!flg) step_exp=-50;
		step=pow(2,step_exp);
		
		// Simple output
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The dimension of the partition is %d, and the relative error on the coefficients is of the order of %e\n",partsize,step);CHKERRQ(ierr);
		
		// Declaration of the objects which we are going to compute 
		Mat A;
		PetscScalar MatError(0);
		
		// Allocate the matrix 
		ierr=MatCreateSeqAIJ(PETSC_COMM_WORLD,partsize,partsize,20,PETSC_NULL,&A);CHKERRQ(ierr);
		ierr=MatSetFromOptions(A);CHKERRQ(ierr);
		
		// Generate the matrix
		ierr=genmatrix_ulam(partsize,step,intreport,debug,&A,&MatError);CHKERRQ(ierr);
		
		// Assembly the matrix
		ierr=MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
		ierr=MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
		
		// As a test for the matrix we compute the norm
		PetscScalar norm;
		ierr=MatNorm(A,NORM_1,&norm);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The 1-norm of the matrix is %e\n",norm);CHKERRQ(ierr);
		
		// Output the matrix to the file
		ierr=PetscViewerBinaryOpen(comm,file,FILE_MODE_WRITE,&fd);CHKERRQ(ierr);
		ierr=MatView(A,fd);CHKERRQ(ierr);
		
		// Output the error of the matrix
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The maximum error of the matrix is %e\n",MatError);CHKERRQ(ierr);
		
		// Destroys the viewer
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);		
		clock_t ends = clock();
		
		// Outputs the running time
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Running time: %e\n",(double) (ends - start) / CLOCKS_PER_SEC);CHKERRQ(ierr);
		
		// Deallocs the memory
		ierr=MatDestroy(&A);CHKERRQ(ierr);
		
		return 0;
}
