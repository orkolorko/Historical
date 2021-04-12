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
			
		int TimeEst=9;
		int TimeApprox=10;
		int Iterates=30;
		
		if (rigerr==1)
		{
			ierr=DecayTime(&sA,errorM,error,start,end,&TimeEst,&TimeApprox,&Iterates);CHKERRQ(ierr);
			ierr=PetscPrintf(comm,"TimeEst is %d, TimeApprox is %d\n",TimeEst,TimeApprox);CHKERRQ(ierr);
		}
		
		
		Vec yk,e,temp;
		PetscScalar DiffIterates;
		
		ierr=VecCreate(comm,&yk);CHKERRQ(ierr);
		ierr=VecSetSizes(yk,PETSC_DECIDE,size);CHKERRQ(ierr);
		ierr=VecSetFromOptions(yk);CHKERRQ(ierr);
		ierr=VecDuplicate(yk,&e);CHKERRQ(ierr);
		ierr=VecSet(e,0.5);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(e);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(e);CHKERRQ(ierr);
		ierr=MatMult(sA,e,yk);CHKERRQ(ierr);
		ierr=VecDestroy(&e);CHKERRQ(ierr);
		ierr=VecDuplicate(yk,&temp);CHKERRQ(ierr);
	
		for (int i=0; i<Iterates; i++)
			{
				ierr=VecCopy(yk,temp);CHKERRQ(ierr);
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
		ierr=VecDuplicate(yk,&Norm);CHKERRQ(ierr);
		ierr=VecCopy(yk,Norm);CHKERRQ(ierr);
		
		PetscOptionsGetString(PETSC_NULL,"-output",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate output text file with the -output option");
		ierr=PetscViewerASCIIOpen(PETSC_COMM_WORLD,nomefile,&fd);CHKERRQ(ierr);
		ierr=PetscViewerSetFormat(fd,PETSC_VIEWER_ASCII_SYMMODU);CHKERRQ(ierr);
		ierr=VecView(Norm,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		
		ierr=VecDestroy(&yk);CHKERRQ(ierr);
		ierr=VecDestroy(&Norm);CHKERRQ(ierr);
		
		PetscFinalize();
		return 0;
}
