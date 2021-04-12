#include "rigerror.h"

// Computes the decay time bound for the matrix Pk and the decay time of the matrix \Pi
int DecayTime(Mat const * const Aaddr,APFloat MatError, int * const TimeEstAddr,int * const TimeApproxAddr) 
{
	int TimeEst=0;
	int TimeApprox=0;
	double AbsError=MatError;								 	
	Vec basnullspace;
	Vec temp;
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
	
	for (int j=1; j<sizecol-1;j++)
	{
		/* Declares the main variables */
		
		/* Creates the vectors */
		ierr=VecCreate(comm,&basnullspace);CHKERRQ(ierr);
		ierr=VecSetSizes(basnullspace,PETSC_DECIDE,size);CHKERRQ(ierr);
		ierr=VecSetFromOptions(basnullspace);CHKERRQ(ierr);
		ierr=VecSetValue(basnullspace,0,0.5,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecSetValue(basnullspace,j,-0.5,INSERT_VALUES);CHKERRQ(ierr);
		ierr=VecAssemblyBegin(basnullspace);CHKERRQ(ierr);
		ierr=VecAssemblyEnd(basnullspace);CHKERRQ(ierr);
		
		ierr=VecDuplicate(basnullspace,&temp);CHKERRQ(ierr);
		ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
		if (j%1000==0) ierr=PetscPrintf(PETSC_COMM_WORLD,"Vettore %d della base\n",j);CHKERRQ(ierr);
		PetscReal norm=1;
		int i=0;
		double normdec=norm+i*size*AbsError;
		while (normdec>=0.5)
		{
			ierr=VecNorm(temp,NORM_1,&norm);CHKERRQ(ierr);
			ierr=MatMultTranspose(*Aaddr,temp,basnullspace);CHKERRQ(ierr);
			ierr=VecCopy(basnullspace,temp);CHKERRQ(ierr);
			i++;
			if (norm<=0.5)
				{
					if (i>TimeEst) TimeEst=i;
				}
			normdec=norm+i*size*AbsError;
		}
		if (j%1000==0) ierr=PetscPrintf(PETSC_COMM_WORLD,"Per il vettore %d i vale %d\n",j,i);
		if (i>TimeApprox) TimeApprox=i;
		ierr=VecDestroy(basnullspace);CHKERRQ(ierr);
		ierr=VecDestroy(temp);CHKERRQ(ierr);
	}	
	*TimeApproxAddr=TimeApprox;
	*TimeEstAddr=TimeEst;
	
	ierr=PetscPrintf(PETSC_COMM_WORLD,"Timeapprox %d\n",*TimeApproxAddr);CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"TimeEst %d\n",*TimeEstAddr);CHKERRQ(ierr);
	
	return 0;
}


int rigerror(APFloat &B, APFloat &lambda, Vec const * const NormAddr, const int TimeApprox, const int TimeEst, APFloat * const rigerrAddr)
{
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD,&rank);
	VecScatter ctx;
	PetscErrorCode ierr;
	Vec NormLoc;
	PetscScalar * NormVal;
	
	ierr=VecScatterCreateToZero(*NormAddr,&ctx,&NormLoc);CHKERRQ(ierr);
    ierr=VecScatterBegin(ctx,*NormAddr,NormLoc,INSERT_VALUES,SCATTER_FORWARD);CHKERRQ(ierr);
	ierr=VecScatterEnd(ctx,*NormAddr,NormLoc,INSERT_VALUES,SCATTER_FORWARD);CHKERRQ(ierr);
	ierr=VecScatterDestroy(ctx);CHKERRQ(ierr);
    
    ierr=VecGetArray(NormLoc,&NormVal);CHKERRQ(ierr);
	
	if (rank==0)
		{
		int partsize;
		APFloat errorNorm;
		PetscErrorCode ierr;
	
		ierr=VecGetSize(*NormAddr,&partsize);CHKERRQ(ierr);
	
		partsize--;
	
		APFloat epsilon1;
		errorNorm=NormVal[partsize];
		
		ierr=PetscPrintf(PETSC_COMM_SELF,"errorNorm %e\n",errorNorm);CHKERRQ(ierr);
	
		epsilon1=2*partsize*(TimeApprox)*(errorNorm);
		ierr=PetscPrintf(PETSC_COMM_SELF,"epsilon1 %e\n",epsilon1);CHKERRQ(ierr);
	
		APFloat normest;
		APFloat gamma;
		APFloat temp1=2*lambda+1;
		APFloat temp2=B;
		APFloat labile=(1/(1-2*lambda));
		APFloat err;
		APFloat length=APFloat(1)/partsize;
	
		if (temp1<temp2)
			{
				gamma=temp2;
			}
		else
			{
				gamma=temp1;
			}
	
		ierr=PetscPrintf(PETSC_COMM_WORLD,"Timeapprox %d\n",TimeApprox);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_WORLD,"TimeEst %d\n",TimeEst);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_SELF,"Gamma %e\n",gamma);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_SELF,"B' %e\n",temp2);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_SELF,"labile %e\n",labile);CHKERRQ(ierr);
		ierr=PetscPrintf(PETSC_COMM_SELF,"B %e\n",labile*temp2);CHKERRQ(ierr);
	
		normest=gamma*length;
		ierr=PetscPrintf(PETSC_COMM_SELF,"Gamma times length %e\n",normest);CHKERRQ(ierr);
	
		err=epsilon1+2*(TimeEst)*normest*labile*temp2;
	
		*rigerrAddr=err;
		}
	ierr=VecRestoreArray(NormLoc,&NormVal);CHKERRQ(ierr);
	VecDestroy(NormLoc);
	
	return 0;
}

