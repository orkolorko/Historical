//      genmatrix.cxx
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

using namespace std;



PetscInt genmatrix(PetscInt, PetscScalar, Mat * const,PetscScalar * const, PetscInt, bool);

PetscInt main (PetscInt argc, char *argv[])
{
		string nomefile;
		PetscInt partsize=65536;
		PetscInt intreport=64;
		PetscInt inputdebug;
		bool debug;
		
		PetscReal step=0.00000000000000000000001;
		PetscInt sizerow,sizecol;
		PetscViewer           fd;
		
		char                  file[PETSC_MAX_PATH_LEN];     /* input file name */
		char                  labile[PETSC_MAX_PATH_LEN]; /* input for the PetscPetscInt */ 
		PetscErrorCode        ierr;
		PetscBool            flg;
		
		
		PetscInitialize(&argc,&argv,(char*)0,"-size mandatory for the size of the partition default=16384\
												\n -output mandatory for the output file\
												\n -error optional for the error default=1e-23");
		
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
		
		ierr=PetscOptionsGetScalar(PETSC_NULL,"-error",&step,&flg);CHKERRQ(ierr);
		if (!flg) step=0.00000000000000000000001;
		
		ierr=PetscPrintf(PETSC_COMM_WORLD,"The dimension of the partition is %d\n",partsize,step*PetscScalar(partsize));CHKERRQ(ierr);
		
		
		Mat A;
		PetscScalar MatError(0);
		
		ierr=MatCreateSeqAIJ(PETSC_COMM_WORLD,partsize,partsize,20,PETSC_NULL,&A);CHKERRQ(ierr);
		//ierr=MatCreateMPIAIJ(PETSC_COMM_WORLD,PETSC_DECIDE,PETSC_DECIDE,partsize,partsize,15,PETSC_NULL,15,PETSC_NULL,&A);CHKERRQ(ierr);
		
		ierr=MatSetFromOptions(A);CHKERRQ(ierr);
		ierr=genmatrix(partsize,step,&A,&MatError,intreport,debug);CHKERRQ(ierr);
		ierr=MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
		ierr=MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
		//ierr=MatView(A,PETSC_VIEWER_DRAW_WORLD);CHKERRQ(ierr);
		//std::cin.get();
		
		ierr=PetscViewerBinaryOpen(comm,file,FILE_MODE_WRITE,&fd);CHKERRQ(ierr);
		ierr=MatView(A,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);		
		
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,file,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		ierr=MatLoad(A,fd);CHKERRQ(ierr);
		ierr=MatGetSize(A,&sizerow,&sizecol);CHKERRQ(ierr);
		std::cout << sizerow << " " << sizecol << endl;
		std::cout << "The error is " << MatError << endl;
		
		
		/*
		
		
		ierr=MatDestroy(&A);CHKERRQ(ierr);
		*/
		PetscFinalize();
		return 0;
}





/* Pw-linear map non markov
REALAUTDER dynamicautder(REALAUTDER &x)
{
	REALAUTDER temp;
	
	temp=REALAUTDER(sqrt(REAL(2)),REAL(0))*x+REALAUTDER(sqrt(REAL(0.104857)),0);
	
	while (temp.x>1) temp=temp- REALAUTDER(REAL(1),REAL(0));
	return temp;
}

REAL dynamic(REAL x)
{
	REAL temp;
	temp=sqrt(REAL(2))*x+sqrt(REAL(0.104857));
	
	while (temp>REAL(1)) temp=temp-REAL(1);
	
	return temp;
}
*/
