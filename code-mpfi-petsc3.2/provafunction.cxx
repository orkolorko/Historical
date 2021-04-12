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

#include "iRRAM_core.h"
#include "iRRAM_lib.h"
#include "petscmat.h"
#include "realautder.h"
#include "dynamic.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 

using namespace std;
using namespace iRRAM;


REAL dynamic(REAL x);

int evaluate(PetscInt *inputAddr, double *outputAddr)
{
	RATIONAL labile(inputAddr[0],inputAddr[1]);
	RATIONAL labileinv(inputAddr[1],inputAddr[0]);
	
	Taylor<3> point(REAL(labile),1);
	Taylor<3> temp=DynamicTaylor(DynamicTaylor(point));
	//Taylor<2> temp=intdistor(point);
	
	
	for (int i=0;i<2;i++)
	{
		outputAddr[i]=double(temp.viewcoeff(i));
	}
	
	//REAL temp=power(abs(REAL(1)-REAL(labileinv)),REAL(labileinv));
	//outputAddr[0]=double(temp);
	//outputAddr[1]=1;
	return 0;
}

PetscInt main (PetscInt argc, char *argv[])
{
		PetscInitialize(&argc,&argv,(char*)0,"-size mandatory for the size of the partition default=16384\
												\n -output mandatory for the output file");
		string nomefile;
		PetscInt partsize=65536;
		PetscReal step=0.00000000000000000000001;
		PetscInt sizerow,sizecol;
		PetscViewer           fd;
		
		char                  filename[PETSC_MAX_PATH_LEN];     /* input file name */
		char                  labile[PETSC_MAX_PATH_LEN]; /* input for the PetscPetscInt */ 
		FILE                  *fp;
		PetscErrorCode        ierr;
		PetscTruth            flg;
		
		ierr=PetscOptionsGetString(PETSC_NULL,"-output",filename,PETSC_MAX_PATH_LEN-1,&flg);CHKERRQ(ierr);
		if (!flg) SETERRQ(1,"Must indicate text file with the -output option");
		
		int size=65537;
		double y;
		
		iRRAM_initialize(argc,argv);
		
		PetscFOpen(MPI_COMM_WORLD,filename,"w",&fp);
		double outputAddr[5];
		PetscInt inputAddr[2];
		
		double DistValuesAddr[size];
		/*
		inputAddr[0]=-1;
		inputAddr[1]=100;
		
		iRRAM_exec(evaluate,inputAddr,outputAddr);
		for (int i=0;i<5;i++)	
		{
			std::cout << outputAddr[i] << std::endl;
		}
		*/
		
		for (int i=32000;i<size;i++)
		{
			inputAddr[0]=i;
			inputAddr[1]=size;
			iRRAM_exec(evaluate,inputAddr,outputAddr);
			std::cout << i << endl;
			DistValuesAddr[i]=outputAddr[2];
			PetscFPrintf(MPI_COMM_WORLD,fp,"%f\t%f\n",double(i)/double(size),outputAddr[0],outputAddr[1]);
		}

		PetscFClose(MPI_COMM_WORLD,fp);
		
		return 0;
}



