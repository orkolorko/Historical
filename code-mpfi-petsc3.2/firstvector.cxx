#include <iostream>
#include "eigenerr.h"
#include <string>
#include <fstream>
#include <stdlib.h> 

using namespace std;


int main (int argc, char *argv[])
{
		char nomefile[PETSC_MAX_PATH_LEN];
		char labile[PETSC_MAX_PATH_LEN];
		PetscViewer           fd;
		PetscErrorCode        ierr;
		PetscBool            flg;
		
		Mat sA;
		Vec Norm;
		/*Iter Norm;*/
		PetscScalar error=0.00000001;
		PetscScalar errorM=0.000000000000001;
		
		PetscInitialize(&argc,&argv,(char*)0,"-input mandatory for the input matrix file\n-B input of the B constat\n-lambda input of the lambda constant");
		MPI_Comm comm=PETSC_COMM_WORLD;
		
		PetscInt SizeRow,SizeCol;
		PetscInt rigerr=0;
		PetscInt start=0;
		PetscInt end;
		
		PetscOptionsGetString(PETSC_NULL,"-input",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -input option");
		/*
		PetscOptionsGetString(PETSC_NULL,"-error",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate error with the -error option");
		error=atof(labile);
		
		PetscOptionsGetString(PETSC_NULL,"-errorM",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate error with the -errorM option");
		errorM=atof(labile);
		
		PetscOptionsGetString(PETSC_NULL,"-input",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -input option");
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-rigerr",&rigerr,&flg);CHKERRQ(ierr);
		if (!flg) rigerr=0;
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-start",&start,&flg);CHKERRQ(ierr);
		*/
		
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,nomefile,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		ierr=MatCreate(comm,&sA);CHKERRQ(ierr);
		ierr=MatLoad(sA,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		ierr=MatGetSize(sA,&SizeRow,&SizeCol);CHKERRQ(ierr);
		
		if (SizeRow!=SizeCol)
		{
		ierr=PetscPrintf(comm,"La matrice non è quadrata");CHKERRQ(ierr);
		PetscFinalize();
		return 0;
		}
		PetscInt size=SizeRow;
		
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-end",&end,&flg);CHKERRQ(ierr);
		if (!flg) end=size;
			
		Vec yk,e0,e1,temp;
		PetscScalar DiffIterates;
		PetscScalar norm=1;
		
		ierr=VecCreate(comm,&yk);CHKERRQ(ierr);
		ierr=VecSetSizes(yk,PETSC_DECIDE,size);CHKERRQ(ierr);
		ierr=VecSetFromOptions(yk);CHKERRQ(ierr);
		ierr=VecDuplicate(yk,&e0);CHKERRQ(ierr);
		ierr=VecDuplicate(yk,&e1);CHKERRQ(ierr);
		ierr=VecSetValue(e0,0,0.5,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecSetValue(e1,10,0.5,INSERT_VALUES);CHKERRQ(ierr);
		
		ierr=VecAssemblyBegin(e0);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(e1);CHKERRQ(ierr);
		
		ierr=VecAssemblyEnd(e0);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(e1);CHKERRQ(ierr);
		
		ierr=VecDuplicate(yk,&temp);CHKERRQ(ierr);
		int i=0;
		int Iterates=100;
		
		while (norm+i*size*(0.00000000000000001)>=1/double(SizeRow))
		{
			if (norm+i*size*(0.00000000000000001)>2.5)
				{
					ierr=PetscPrintf(PETSC_COMM_WORLD,"Non sta contraendo!");CHKERRQ(ierr);
					return 1;
				}
			
			for (int k=0;k<Iterates;k++)
			{
				i++;
				ierr=MatMult(sA,e0,temp);CHKERRQ(ierr);
				ierr=VecCopy(temp,e0);CHKERRQ(ierr);
				ierr=MatMult(sA,e1,temp);CHKERRQ(ierr);
				ierr=VecCopy(temp,e1);CHKERRQ(ierr);
				
			}
			VecWAXPY(temp,-1,e0,e1);
			ierr=VecNorm(temp,NORM_1,&norm);CHKERRQ(ierr);
			ierr=PetscPrintf(PETSC_COMM_WORLD,"Al passo %d normdec vale %e\n",i,norm);
		}
		std::cout << i << std::endl;
		
		ierr=VecDestroy(&e0);CHKERRQ(ierr);
		ierr=VecDestroy(&e1);CHKERRQ(ierr);
		
		PetscFinalize();
		return 0;
}
