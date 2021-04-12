#include <iostream>
#include "petscmat.h"
#include <string>
#include <fstream>
#include <stdlib.h> 
#include <fenv.h>

using namespace std;

PetscErrorCode ConeContraction(Mat sA, Vec x, PetscInt ChartIndex,PetscInt size, PetscScalar *M, PetscInt *MaxIter)
{
	/*
	fesetround (FE_UPWARD);
	*/
	
	PetscErrorCode ierr;
	Mat Projection;
	
	ierr=MatCreateMPIAIJ(PETSC_COMM_WORLD,PETSC_DECIDE,PETSC_DECIDE,size-1,size,2,PETSC_NULL,2,PETSC_NULL,&Projection);CHKERRQ(ierr);
	
	std::cout << "projection" << std::endl;
	for (int i=0;i<size-1;i++)
	{
		
		if (i<ChartIndex)
		{
			ierr=MatSetValue(Projection,i,i,1,INSERT_VALUES);CHKERRQ(ierr);
		}
		if (i>=ChartIndex)
		{
			ierr=MatSetValue(Projection,i,i+1,1,INSERT_VALUES);CHKERRQ(ierr);
		}
	}
	Mat Injection;
	
	ierr=MatCreateMPIAIJ(PETSC_COMM_WORLD,PETSC_DECIDE,PETSC_DECIDE,size,size-1,2,PETSC_NULL,2,PETSC_NULL,&Injection);CHKERRQ(ierr);
		
	for (int i=0;i<size;i++)
	{
		if (i<ChartIndex)
		{
			ierr=MatSetValue(Injection,i,i,1,INSERT_VALUES);CHKERRQ(ierr);
		}
		if (i>ChartIndex)
		{
			ierr=MatSetValue(Injection,i,i-1,1,INSERT_VALUES);CHKERRQ(ierr);
		}
	}
	
	
	ierr=MatAssemblyBegin(Injection,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr=MatAssemblyBegin(Projection,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr=MatAssemblyEnd(Projection,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr=MatAssemblyEnd(Injection,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);	
		
	Vec CandidateProj;
	
	ierr=VecCreate(PETSC_COMM_WORLD,&CandidateProj);CHKERRQ(ierr);
	ierr=VecSetSizes(CandidateProj,PETSC_DECIDE,size-1);CHKERRQ(ierr);
	ierr=VecSetFromOptions(CandidateProj);CHKERRQ(ierr);
	
	PetscScalar Norm;	
	PetscReal MinNorm=1;
	PetscReal MaxNorm=1;
	
	ierr=VecNorm(x,NORM_1,&Norm);
	
	Vec MinEncl,MaxEncl;
	ierr=VecDuplicate(x,&MinEncl);CHKERRQ(ierr);
	ierr=VecDuplicate(x,&MaxEncl);CHKERRQ(ierr);
	
	ierr=MatMult(Projection,x,CandidateProj);CHKERRQ(ierr);
	ierr=MatMult(Injection,CandidateProj,MinEncl);CHKERRQ(ierr);
	
	ierr=VecNorm(CandidateProj,NORM_1,&Norm);
	std::cout << Norm << std::endl;
	
	ierr=VecSetValue(MinEncl,ChartIndex,1-Norm,INSERT_VALUES);
	ierr=VecWAXPY(MaxEncl,-1,x,MinEncl);	
	ierr=VecNorm(MaxEncl,NORM_INFINITY,&Norm);
	
	std::cout << "test " << Norm << std::endl;
	
	Vec MinEnclProj,MaxEnclProj;
	
	ierr=VecDuplicate(CandidateProj,&MinEnclProj);CHKERRQ(ierr);
	ierr=VecDuplicate(CandidateProj,&MaxEnclProj);CHKERRQ(ierr);
	
	ierr=VecCopy(CandidateProj,MinEnclProj);CHKERRQ(ierr);
	ierr=VecCopy(CandidateProj,MaxEnclProj);CHKERRQ(ierr);
	
	ierr=VecNorm(MaxEnclProj,NORM_1,&MaxNorm);
	ierr=VecNorm(MinEnclProj,NORM_1,&MinNorm);
		
	PetscScalar Dilation=0.00000000001;
	
	ierr=VecShift(MaxEnclProj,Dilation);CHKERRQ(ierr);
	ierr=VecShift(MinEnclProj,-1*Dilation);CHKERRQ(ierr);
	
	ierr=VecNorm(MaxEnclProj,NORM_1,&MaxNorm);
	ierr=VecNorm(MinEnclProj,NORM_1,&MinNorm);
		
	std::cout << "MinEnclProj " << MinNorm << std::endl;
	std::cout << "MaxEnclProj " << MaxNorm << std::endl;
		
	std::cin.get();
	
	Vec TempMin,TempMax;
	
	ierr=VecDuplicate(x,&TempMin);CHKERRQ(ierr);
	ierr=VecDuplicate(x,&TempMax);CHKERRQ(ierr);
	
	Vec DiffMin,DiffMax;
	
	ierr=VecDuplicate(CandidateProj,&DiffMin);CHKERRQ(ierr);
	ierr=VecDuplicate(CandidateProj,&DiffMax);CHKERRQ(ierr);
	
	int l=0;
	
	while ((MaxNorm>Dilation) || (MinNorm>Dilation))
	{
		PetscScalar OldMaxNorm=MaxNorm;
		PetscScalar OldMinNorm=MinNorm;
		
		l++;

		std::cout << l << std::endl;

		std::cout.precision(15);
		
		ierr=MatMult(Injection,MinEnclProj,MinEncl);CHKERRQ(ierr);
		ierr=MatMult(Injection,MaxEnclProj,MaxEncl);CHKERRQ(ierr);
		
		ierr=VecNorm(MaxEncl,NORM_1,&MaxNorm);
		ierr=VecNorm(MinEncl,NORM_1,&MinNorm);
		
		std::cout << MinNorm << std::endl;
		std::cout << MaxNorm << std::endl;
		
		std::cin.get();
		
		ierr=VecSetValue(MinEncl,ChartIndex,1-MaxNorm,INSERT_VALUES);
		ierr=VecSetValue(MaxEncl,ChartIndex,1-MinNorm,INSERT_VALUES);
		
		ierr=VecAssemblyBegin(MinEncl);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(MaxEncl);CHKERRQ(ierr);
		
		ierr=VecAssemblyEnd(MinEncl);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(MaxEncl);CHKERRQ(ierr);
		
		ierr=VecNorm(MaxEncl,NORM_1,&MaxNorm);
		ierr=VecNorm(MinEncl,NORM_1,&MinNorm);
		
		std::cout << "MinNorm in R^{n+1} " <<MinNorm << std::endl;
		std::cout << "MaxNorm in R^{n+1} " <<MaxNorm << std::endl;
		
		std::cin.get();
		
		ierr=MatMult(sA,MinEncl,TempMin);CHKERRQ(ierr);
		ierr=MatMult(sA,MaxEncl,TempMax);CHKERRQ(ierr);
		
		ierr=VecNorm(TempMin,NORM_1,&MinNorm);
		ierr=VecNorm(TempMax,NORM_1,&MaxNorm);
		
		std::cout << "TempMinNorm in R^{n+1} " <<MinNorm << std::endl;
		std::cout << "TempMaxNorm in R^{n+1} " <<MaxNorm << std::endl;
		
		ierr=VecScale(TempMax,1/MinNorm);
		ierr=VecScale(TempMin,1/MaxNorm);	
		
		ierr=VecNorm(TempMin,NORM_1,&MinNorm);
		ierr=VecNorm(TempMax,NORM_1,&MaxNorm);
		
		std::cout << "TempMinNorm in R^{n+1} " <<MinNorm << std::endl;
		std::cout << "TempMaxNorm in R^{n+1} " <<MaxNorm << std::endl;
		
		std::cin.get();
		
		ierr=MatMult(Projection,TempMin,MinEnclProj);CHKERRQ(ierr);
		ierr=MatMult(Projection,TempMax,MaxEnclProj);CHKERRQ(ierr);
			
		ierr=VecWAXPY(DiffMax,-1,CandidateProj,MaxEnclProj);	
		ierr=VecWAXPY(DiffMin,-1,CandidateProj,MinEnclProj);	
	
		ierr=VecNorm(DiffMax,NORM_INFINITY,&MaxNorm);
		ierr=VecNorm(DiffMin,NORM_INFINITY,&MinNorm);	
		
		std::cout << "MinDiameter " << MinNorm << std::endl;
		std::cout << "MaxDiameter " << MaxNorm << std::endl;
		
		std::cout << "ratio " << MinNorm/OldMinNorm << std::endl;
		std::cout << "ratio " << MaxNorm/OldMaxNorm << std::endl;
		
		
		std::cin.get();
		
		
	}
	
	
	/*
	fesetround (FE_TONEAREST);
	*/
	
	return 0;
	
}

int main (int argc, char *argv[])
{
		
		char nomefile[PETSC_MAX_PATH_LEN];
		char labile[PETSC_MAX_PATH_LEN];
		PetscViewer           fd;
		PetscErrorCode        ierr;
		PetscBool	          flg;
		
		Mat sA;
		
		
		PetscInitialize(&argc,&argv,(char*)0,"-input mandatory for the input matrix file\n-B input of the B constat\n-lambda input of the lambda constant");
		MPI_Comm comm=PETSC_COMM_WORLD;
		
		ierr=MatCreate(comm,&sA);CHKERRQ(ierr);
		
		
		PetscInt SizeRow,SizeCol;
		PetscInt rigerr=0;
		
		PetscOptionsGetString(PETSC_NULL,"-input",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate text file with the -input option");
		
		ierr=PetscViewerBinaryOpen(PETSC_COMM_WORLD,nomefile,FILE_MODE_READ,&fd);CHKERRQ(ierr);
		
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
		PetscInt Iterates=12;
		PetscScalar DiffIterates;
		
		Vec x,temp;
		ierr=VecCreate(comm,&x);CHKERRQ(ierr);
		ierr=VecSetSizes(x,PETSC_DECIDE,size);CHKERRQ(ierr);
		ierr=VecSetFromOptions(x);CHKERRQ(ierr);
		
		ierr=VecSet(x,double(1));;CHKERRQ(ierr);
		ierr=VecAssemblyBegin(x);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(x);CHKERRQ(ierr);
		
		ierr=VecDuplicate(x,&temp);CHKERRQ(ierr);
		
		for (int i=0;i<15;i++)
		{
			std::cout << i << std::endl;
			ierr=MatMult(sA,x,temp);CHKERRQ(ierr);
			ierr=VecCopy(temp,x);
		}
		
		
		PetscInt where;
		PetscReal max;
		PetscReal norm;
		ierr=VecMax(x,&where,&max);CHKERRQ(ierr);
		ierr=VecNorm(x,NORM_1,&norm);
		ierr=VecScale(x,1/norm);
		
		std::cout << "where " << where << std::endl;
		std::cout << "max " << max << std::endl;
		
		PetscScalar M;
		PetscInt MaxIter;
		
		ierr=ConeContraction(sA,x,where,size,&M,&MaxIter);CHKERRQ(ierr);
		
		/*
		std::cout << "M " << M << std::endl;
		std::cout << "MaxIter " << MaxIter << std::endl;
		
		ierr=MainChart(sA,x,where,size,M,&MaxIter);CHKERRQ(ierr);
		
		std::cout << "M " << M << std::endl;
		std::cout << "MaxIter " << MaxIter << std::endl;
		
		
		//ierr=VecRescaleInt(temp,tempRad,Norm,NormRad);CHKERRQ(ierr);
		//ierr=VecNormInt(temp,tempRad,&Norm,&NormRad);CHKERRQ(ierr);
		//std::cout << Norm << std::endl;
		//std::cout << NormRad << std::endl;
		
		PetscOptionsGetString(PETSC_NULL,"-output",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(comm,1,"Must indicate output text file with the -output option");
		ierr=PetscViewerASCIIOpen(PETSC_COMM_WORLD,nomefile,&fd);CHKERRQ(ierr);
		ierr=PetscViewerSetFormat(fd,PETSC_VIEWER_ASCII_SYMMODU);CHKERRQ(ierr);
		ierr=VecView(temp,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(&fd);CHKERRQ(ierr);
		/*
		PetscOptionsGetString(PETSC_NULL,"-outputRad",nomefile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(1,"Must indicate outputRad text file with the -outputRad option");
		ierr=PetscViewerASCIIOpen(PETSC_COMM_WORLD,nomefile,&fd);CHKERRQ(ierr);
		ierr=PetscViewerSetFormat(fd,PETSC_VIEWER_ASCII_SYMMODU);CHKERRQ(ierr);
		ierr=VecView(tempRad,fd);CHKERRQ(ierr);
		ierr=PetscViewerDestroy(fd);CHKERRQ(ierr);
		
		ierr=VecDestroy(&x);CHKERRQ(ierr);
		//ierr=VecDestroy(xRad);CHKERRQ(ierr);
		ierr=VecDestroy(&temp);CHKERRQ(ierr);
		//ierr=VecDestroy(tempRad);CHKERRQ(ierr);
		*/
		PetscFinalize();
		
		return 0;
}
/*
		PetscOptionsGetString(PETSC_NULL,"-error",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(1,"Must indicate error with the -error option");
		error=atof(labile);
		
		PetscOptionsGetString(PETSC_NULL,"-errorM",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(1,"Must indicate error with the -errorM option");
		errorM=atof(labile);
		
		PetscOptionsGetString(PETSC_NULL,"-B",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(1,"Must indicate B' with the -B option");
		B=atof(labile);
		
		PetscOptionsGetString(PETSC_NULL,"-lambda",labile,PETSC_MAX_PATH_LEN-1,&flg);
		if (!flg) SETERRQ(1,"Must indicate lambda with the -lambda option");
		lambda=atof(labile);
		
		ierr=PetscOptionsGetInt(PETSC_NULL,"-rigerr",&rigerr,&flg);CHKERRQ(ierr);
		if (!flg) rigerr=0;
*/
