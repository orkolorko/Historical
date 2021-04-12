#include "eigenerr.h"
#include <fenv.h>

// Computes the decay time bound for the matrix Pk and the decay time of the matrix \Pi
int DecayTime(Mat const * const Aaddr,APFloat MatError,APFloat error, PetscInt start,PetscInt end, int * const TimeEstAddr,int * const TimeApproxAddr,int * const IteratesAddr) 
{
	const int originalRounding = fegetround( );
	fesetround(FE_UPWARD);
	int TimeEst=0;
	int TimeApprox=0;
	int Iterates=0;
	double AbsError=MatError;								 	
	Vec basnullspace;
	Vec temp;
	
	PetscScalar Norms[256];
	
	for (int i=0;i<256;i++)
	{
		Norms[i]=0;
	}
	
	PetscErrorCode ierr;
	PetscInt sizerow;
	PetscInt sizecol;
	PetscInt size;
	
	MPI_Comm comm=PETSC_COMM_WORLD;
	ierr=MatGetSize(*Aaddr,&sizerow,&sizecol);CHKERRQ(ierr);
	if (sizerow==sizecol)
	{
		size=sizerow;
	}
	
	ierr=VecCreate(comm,&basnullspace);CHKERRQ(ierr);
	ierr=VecSetSizes(basnullspace,PETSC_DECIDE,size);CHKERRQ(ierr);
	ierr=VecSetFromOptions(basnullspace);CHKERRQ(ierr);
	ierr=VecDuplicate(basnullspace,&temp);CHKERRQ(ierr);
			
	
	for (int j=0; j<size-2;j++)
	{
		/* Declares the main variables */
		
		/* Creates the vectors */
		ierr=VecSetValue(basnullspace,size-1,1,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecSetValue(basnullspace,j,-1,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(basnullspace);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(basnullspace);CHKERRQ(ierr);
		ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
		if (j%1024==0) ierr=PetscPrintf(PETSC_COMM_WORLD,"Vettore %d della base\n",j);CHKERRQ(ierr);
		PetscReal norm=1;
		int i=0;
		bool TimeEstBool=true;
		bool TimeApproxBool=true;
		double normdec=norm+i*size*AbsError;
		while (norm+i*size*(0.00000000000000001)>=0.5*double(size)/double(size-1-j))
		{
			if (norm+i*size*(0.00000000000000001)>2.5)
				{
					ierr=PetscPrintf(PETSC_COMM_WORLD,"Non sta contraendo!",j);CHKERRQ(ierr);
					return 1;
				}
			ierr=VecNorm(temp,NORM_1,&norm);CHKERRQ(ierr);
			ierr=MatMult(*Aaddr,temp,basnullspace);CHKERRQ(ierr);
			ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
			
			if ((norm<=0.5) && (TimeEstBool==true))
				{
					if (i>TimeEst) TimeEst=i;
					TimeEstBool=false;
				}
			if ((normdec<=0.5) && (TimeApproxBool==true))
				{
					if (i>TimeApprox) TimeApprox=i;
					TimeApproxBool=false;
				}
			normdec=norm+i*size*AbsError;
			if (normdec>=Norms[i])
			{
				Norms[i]=normdec;
			}
			i++;
			//ierr=PetscPrintf(PETSC_COMM_WORLD,"Al passo %d normdec vale %e\n",i,normdec);
			//ierr=PetscPrintf(PETSC_COMM_WORLD,"Al vettore %d, iterata %d, norm vale %f\n",j,i,norm+i*size*(0.00000000000000001));CHKERRQ(ierr);
		}
		
		if (i>Iterates) Iterates=i;
		if (j%128==0) ierr=PetscPrintf(PETSC_COMM_WORLD,"Al vettore %d TimeEst vale %d, TimeApprox %d, iterates %d\n",j,TimeEst,TimeApprox,Iterates);
		ierr=VecZeroEntries(basnullspace);CHKERRQ(ierr);
		ierr=VecZeroEntries(temp);CHKERRQ(ierr);
	}	
	*TimeApproxAddr=TimeApprox;
	*TimeEstAddr=TimeEst;
	*IteratesAddr=Iterates;
	
	double normsum=0;
	for (int i=0;i<TimeEst;i++)
	{
		normsum+=Norms[i];
		ierr=PetscPrintf(PETSC_COMM_WORLD,"C%d=%f\n",i,Norms[i]);CHKERRQ(ierr);
	}
	ierr=PetscPrintf(PETSC_COMM_WORLD,"NormSum=%f\n",normsum);CHKERRQ(ierr);
	
	ierr=VecDestroy(&basnullspace);CHKERRQ(ierr);
	ierr=VecDestroy(&temp);CHKERRQ(ierr);
	
	ierr=PetscPrintf(PETSC_COMM_WORLD,"Timeapprox %d\n",*TimeApproxAddr);CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"TimeEst %d\n",*TimeEstAddr);CHKERRQ(ierr);
	fesetround(originalRounding);
	
	return 0;
}
