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
//! \file actualgen-lipschitz.cxx implements actualgen-lipschitz.h 

#include "actualgen-lipschitz.h"
#include "dynamic-lipschitz.h"
#include <fstream>
#include <utility>
#include "iRRAM/lib.h"
#include "iRRAM/core.h"

using namespace std;

// The entries of this function are the size of the partition (size), the allowed error on each coefficient (step)
// the pointer to the matrix Aaddr, the pointer to the error MatErrorAddr, and an integer that governs the output
// of the program, a boolean that governs the debugging output

PetscInt genmatrix_lipschitz(PetscInt size, Mat * const Aaddr, PetscScalar * const MatErrorAddr, PetscInt intreport, bool debug)
{
	// The following code is used to limit the number of coefficients of the matrix that are computed;
	// since we are computing $A_ij=m(I_i \cap T^-1(I_j))$, it makes sens to compute the coefficients
	// of the matrix corresponding to those I_j which are near (the sense of this near is going to be made precise below)
	// to the images of the endpoints of I_i
	
	//These below are accessory variables used to compute using the iRRAM
	PetscScalar * out;
	out=new PetscScalar[2];
	PetscInt *input;
	input=new PetscInt[2];
	
	ofstream output("intoutput.txt");
	
	// the following is the usual ErrorCode variable of Petsc
	PetscErrorCode  ierr;
	
	// the entry input[1] is used to pass to the iRRAM the size of the partition
	input[1]=size;
	
	// we start the outer loop, i.e., we start to iterate along the I_i
	//#pragma omp parallel for
	for (PetscInt i=0;i<size;i++)
	{
		
		input[0]=i;
		std::pair<PetscInt*,PetscScalar*> inputs_estimate(input,out);
		//computes an estimate of the image of the interval
		iRRAM_exec(dynamicestimate,inputs_estimate);
		PetscInt target=PetscInt(out[0]*size)%size;
		PetscInt der=PetscInt(out[1]);
		//computes the target indexes
		PetscInt jmin=target-4*der;
		PetscInt jmax=target+4*der;
		
		double rowmeasure=0;
		
		
		if (jmin<0)
		{
			//if jmin is smaller than 0, we have crossed a discontinuity
			//therefore we split the computation in two parts
			for (PetscInt j=0;j<jmax;j++)
					{
						double locerror=0;
						double value=0;
						
						std::pair<inputvalue,outputvalue> inputs_compute(inputvalue(i,j,size),outputvalue(&value,&locerror));
						ierr=iRRAM_exec(ComputeValue,inputs_compute);CHKERRQ(ierr);
					
						// As a control, we check that the measure along a row is 1
					
						// Please note that we are filling up the transpose of the Markov matrix associated to the
						// Ulam approximation; this simplifies (and probably fastens up) some of the code in eigenerr
						if (value!=0)
						{	 
							ierr=MatSetValue(*Aaddr,j,i,value,ADD_VALUES);CHKERRQ(ierr);
							rowmeasure+=value;
						}
						// if the error on the computed coefficient is bigger than the errors computed up to it,
						// we keep it as the new maximum error
						if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;		
						//std::cout << j << std::endl;
					}
			//jmin+size<size since jmin<0
			for (PetscInt j=size+jmin;j<size;j++)
					{
						double locerror=0;
						double value=0;
						std::pair<inputvalue,outputvalue> inputs_compute(inputvalue(i,j,size),outputvalue(&value,&locerror));
						ierr=iRRAM_exec(ComputeValue,inputs_compute);CHKERRQ(ierr);
						
						// As a control, we check that the measure along a row is 1
					
						// Please note that we are filling up the transpose of the Markov matrix associated to the
						// Ulam approximation; this simplifies (and probably fastens up) some of the code in eigenerr
						if (value!=0)
						{	 
							ierr=MatSetValue(*Aaddr,j,i,value,ADD_VALUES);CHKERRQ(ierr);
							rowmeasure+=value;
						}
						// if the error on the computed coefficient is bigger than the errors computed up to it,
						// we keep it as the new maximum error
						if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;		
						//std::cout << j << std::endl;
					}
		}
		if (jmax>=size)
		{
			//if jmax is bigger than 0, we have crossed a discontinuity
			//therefore we split the computation in two parts
			
			for (PetscInt j=0;j<jmax%size;j++)
					{
						double locerror=0;
						double value=0;
						std::pair<inputvalue,outputvalue> inputs_compute(inputvalue(i,j,size),outputvalue(&value,&locerror));
						ierr=iRRAM_exec(ComputeValue,inputs_compute);CHKERRQ(ierr);
						
						// As a control, we check that the measure along a row is 1
					
						// Please note that we are filling up the transpose of the Markov matrix associated to the
						// Ulam approximation; this simplifies (and probably fastens up) some of the code in eigenerr
						if (value!=0)
						{	 
							ierr=MatSetValue(*Aaddr,j,i,value,ADD_VALUES);CHKERRQ(ierr);
							rowmeasure+=value;
						}
						// if the error on the computed coefficient is bigger than the errors computed up to it,
						// we keep it as the new maximum error
						if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;		
						//std::cout << j << std::endl;
					}
			for (PetscInt j=jmin;j<size;j++)
					{
						double locerror=0;
						double value=0;
						std::pair<inputvalue,outputvalue> inputs_compute(inputvalue(i,j,size),outputvalue(&value,&locerror));
						ierr=iRRAM_exec(ComputeValue,inputs_compute);CHKERRQ(ierr);
						
						// As a control, we check that the measure along a row is 1
					
						// Please note that we are filling up the transpose of the Markov matrix associated to the
						// Ulam approximation; this simplifies (and probably fastens up) some of the code in eigenerr
						if (value!=0)
						{	 
							ierr=MatSetValue(*Aaddr,j,i,value,ADD_VALUES);CHKERRQ(ierr);
							rowmeasure+=value;
						}
						// if the error on the computed coefficient is bigger than the errors computed up to it,
						// we keep it as the new maximum error
						if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;		
						//std::cout << j << std::endl;
					}
		}
		//general case
		if ((jmin>=0) && (jmax<size))
			{
				for (PetscInt j=jmin;j<jmax;j++)
					{
						double locerror=0;
						double value=0;
						std::pair<inputvalue,outputvalue> inputs_compute(inputvalue(i,j,size),outputvalue(&value,&locerror));
						ierr=iRRAM_exec(ComputeValue,inputs_compute);CHKERRQ(ierr);
					
						// As a control, we check that the measure along a row is 1
					
						// Please note that we are filling up the transpose of the Markov matrix associated to the
						// Ulam approximation; this simplifies (and probably fastens up) some of the code in eigenerr
						if (value!=0)
						{	 
							ierr=MatSetValue(*Aaddr,j,i,value,ADD_VALUES);CHKERRQ(ierr);
							rowmeasure+=value;
						}
						// if the error on the computed coefficient is bigger than the errors computed up to it,
						// we keep it as the new maximum error
						if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;		
						//std::cout << j << std::endl;
					}
			}
		//test that the sum along the row is near 1
		if (abs(1-rowmeasure)>0.0000000000001) 
		{
			std::cout << i << " Problema "<< rowmeasure << std::endl;
			std::cin.get();
		}
		if (i%100==0) std::cout << i << " rowmeasure "<< rowmeasure << std::endl;
	}
	delete input;
	delete out;
	return 0;
	
}

//please note that all the following primitives suppose the function T is monotone increasing (preserves orientation)

// phi_j is in the first branch, Lphi_i is in the first

REAL primitive11(REAL T1, REAL Td,int j,int l, int k, REAL x)
{
	/*
	RATIONAL b1(i,k);
	REAL T1=dynamic(REAL(b1));
	REAL Td=dynamic_der(REAL(b1));
	*/
	//iRRAM::cout << " Td " << Td << std::endl;
	
	REAL  a0(RATIONAL(j-1,k));
		
	REAL temp=REAL(-1)*(REAL(k)*x*(REAL(3)*REAL(k)*x*T1-REAL(6)*REAL(k)*REAL(l)*T1-REAL(6)*a0*REAL(k)*T1-REAL(2)*REAL(k)*power(x,2)-REAL(3)*Td*x+REAL(3)*REAL(k)*REAL(l)*x+REAL(3)*a0*REAL(k)*x+REAL(6)*REAL(l)*Td+REAL(6)*a0*Td))/(REAL(6)*power(Td,2));
	
	return temp;
}

// phi_j is in the second branch, Lphi_i is in the first

REAL primitive12(REAL T1, REAL Td, int j, int l, int k, REAL x)
{
	/*
	RATIONAL b1(i,k);
	REAL T1=dynamic(REAL(b1));
	REAL Td=dynamic_der(REAL(b1));
	*/
	//iRRAM::cout << " Td " << Td << std::endl;
	
	REAL  a2(RATIONAL(j+1,k));
	
	REAL temp=(REAL(k)*x*(REAL(3)*REAL(k)*x*T1-REAL(6)*REAL(k)*REAL(l)*T1-REAL(6)*a2*REAL(k)*T1-REAL(2)*REAL(k)*power(x,2)-REAL(3)*Td*x+REAL(3)*REAL(k)*REAL(l)*x+REAL(3)*a2*REAL(k)*x+REAL(6)*REAL(l)*Td+REAL(6)*a2*Td))/(REAL(6)*power(Td,2));
	
	
	return temp;
}

// phi_j is in the first branch, Lphi_i is in the second

REAL primitive21(REAL T1, REAL Td, int j, int l, int k, REAL x)
{
	/*
	RATIONAL b1(i,k);
	REAL T1=dynamic(REAL(b1));
	REAL Td=dynamic_der(REAL(b1));
	*/
	//iRRAM::cout << " Td " << Td << std::endl;
	
	REAL  a0(RATIONAL(j-1,k));
  
	REAL temp=(REAL(k)*x*(REAL(3)*REAL(k)*x*T1-REAL(6)*REAL(k)*REAL(l)*T1-REAL(6)*a0*REAL(k)*T1-REAL(2)*REAL(k)*power(x,2)+REAL(3)*Td*x+REAL(3)*REAL(k)*REAL(l)*x+REAL(3)*a0*REAL(k)*x-REAL(6)*REAL(l)*Td-REAL(6)*a0*Td))/(6*power(Td,2));
	
	return temp;
}

// phi_j is in the second branch, Lphi_i is in the second

REAL primitive22(REAL T1, REAL Td, int j, int l,int k, REAL x)
{
	/*
	RATIONAL b1(i,k);
	REAL T1=dynamic(REAL(b1));
	REAL Td=dynamic_der(REAL(b1));
	*/
	//iRRAM::cout << " Td " << Td << std::endl;
	
	REAL  a2(RATIONAL(j+1,k));
	
	REAL temp=REAL(-1)*(REAL(k)*x*(REAL(3)*REAL(k)*x*T1-REAL(6)*REAL(k)*REAL(l)*T1-REAL(6)*a2*REAL(k)*T1-REAL(2)*REAL(k)*power(x,2)+REAL(3)*Td*x+REAL(3)*REAL(k)*REAL(l)*x+REAL(3)*a2*REAL(k)*x-REAL(6)*REAL(l)*Td-REAL(6)*a2*Td))/(6*power(Td,2));

	return temp;
}


PetscInt ComputeValue(const std::pair<inputvalue,outputvalue> & input_compute)
{
	inputvalue a(input_compute.first);
	outputvalue b(input_compute.second);
	
	stiff_begin();
	int i=a.i;
	int j=a.j;
	int size=a.size;
	
	int k=-200;
	
	REAL integral(0);
	
	RATIONAL b1(i,size);
	
	REAL T1=dynamic(REAL(b1));
	REAL Td=dynamic_der(REAL(b1));
	
	double targetdouble=double(T1);
	double derdouble=double(Td);
	
	int intpar=int(targetdouble);
	int intder=int(derdouble);
	
	int lmin=intpar-1;
	int lmax=intpar+1;
	
	//std::cout << "lmin " << lmin << " lmax " << lmax << std::endl;
	
	REAL T0=T1-Td*REAL(RATIONAL(1,size));
	REAL T2=T1+Td*REAL(RATIONAL(1,size));
	
	for (int l=lmin;l<=lmax;l++)
	{
	
	REAL a0(RATIONAL(j-1+l*size,size));
	REAL a1(RATIONAL(j+l*size,size));
	REAL a2(RATIONAL(j+1+l*size,size));
	
	/*
	iRRAM::cout << "j "<<j << std::endl;
	iRRAM::cout << "l "<<l << std::endl;
	iRRAM::cout << "a0 " << a0 << std::endl;	
	iRRAM::cout << "a1 " << a1 << std::endl;
	iRRAM::cout << "a2 " << a2 << std::endl;
	
	iRRAM::cout << "T1 " << T1 << std::endl;
	iRRAM::cout << "Td " <<Td << std::endl;	
	
	iRRAM::cout << "T0 " <<T0 << std::endl;	
	iRRAM::cout << "T2 " <<T2 << std::endl;
	std::cin.get();
	*/
	
	//this really big case if is substituted in more recent versions
	//of the program by an algorithm that sort the points and choses
	// the right branch of the primitives
	
	if (positive(T0-a0,k))
		{
			if (bound(T0-a1,k))
			{
				if (positive(a2-T1,k))
				{
					if (positive(T2-a2,k))
					{
						integral+=primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,T1);
					}
					if (positive(a2-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T1)-primitive21(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
					}
					if (bound(a2-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T1)-primitive21(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
					}
				}
				if (positive(T1-a2,k))
				{
					integral+=primitive12(T1,Td,j,l,size,a2)-primitive12(T1,Td,j,l,size,T0);
				}
				if (bound(T1-a2,k))
				{
					integral+=primitive12(T1,Td,j,l,size,a2)-primitive12(T1,Td,j,l,size,T0);
				}
			}
			
			
			if (positive(T0-a1,k))
			{
				if (positive(T0-a2,k))
				{
					integral+=REAL(0);
				}
				if (positive(a2-T0,k))
				{
					if (positive(T1-a2,k))
					{
						integral+=primitive12(T1,Td,j,l,size,a2)-primitive12(T1,Td,j,l,size,T0);
					}
					if (bound(T1-a2,k))
					{
						integral+=primitive12(T1,Td,j,l,size,a2)-primitive12(T1,Td,j,l,size,T0);
					}
					if (positive(a2-T1,k))
					{
						if (positive(T2-a2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,T1);
						}
						if (positive(a2-T2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
						}
					}
				}
			}
			if (positive(a1-T0,k))
			{
				if (positive(T1-a1,k))
				{
					integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(T0));
					if (positive(T1-a2,k))
					{
						integral+=primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
					if (positive(a2-T1,k))
					{
						if (positive(T2-a2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,REAL(T1))-primitive12(T1,Td,j,l,size,REAL(a1))+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (positive(a2-T2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,REAL(T1))-primitive12(T1,Td,j,l,size,REAL(a1))+primitive22(T1,Td,j,l,size,REAL(T2))-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (bound(T2-a2,k))
						{
							integral+=primitive12(T1,Td,j,l,size,REAL(T1))-primitive12(T1,Td,j,l,size,REAL(a1))+primitive22(T1,Td,j,l,size,REAL(T2))-primitive22(T1,Td,j,l,size,REAL(T1));
						}
					}
					if (bound(T1-a2,k))
					{
						integral+=primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
				}
				if (positive(a1-T1,k))
				{
					if (positive(a1-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,T1);
					}
					if (positive(T2-a1,k))
					{
						if (positive(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (positive(T2-a2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(a1));
						}
					}
					if (bound(a1-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,T1);
					}
				}
				if (bound(a1-T1,k))
				{
					if (positive(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
					}
					if (positive(T2-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,T1);
					}
					if (bound(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,T1)-primitive11(T1,Td,j,l,size,T0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,T1);
					}
				}
			}
		}
		
	// Check from here! Second possibility...
	if (positive(a0-T0,k))
		{
			if (bound(a0-T1,k))
			{
				if (positive(T2-a1,k))
				{
					if (positive(a2-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,a1);
					}
					if (positive(T2-a2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,a1);
					}
					if (bound(T2-a2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,a1);
					}
				}
				if (positive(a1-T2,k))
				{
					integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,a0);
				}
				if (bound(a1-T2,k))
				{
					integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,a0);
				}
			}
			
			
			
			if (positive(a0-T1,k))
			{
				if (positive(a0-T2,k))
				{
					integral+=REAL(0);
				}
				if (positive(T2-a0,k))
				{
					if (positive(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,a0);
					}
					if (bound(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,a0);
					}
					if (positive(T2-a1,k))
					{
						if (positive(T2-a2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,a1);
						}
						if (positive(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,a1);
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,a1)-primitive21(T1,Td,j,l,size,a0)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,a1);
						}
					}
				}
			}
			if (positive(T1-a0,k))
			{
				if (positive(a1-T1,k))
				{
					integral+=primitive11(T1,Td,j,l,size,REAL(T1))-primitive11(T1,Td,j,l,size,REAL(a0));
					if (positive(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,REAL(T1));
					}
					if (positive(T2-a1,k))
					{
						if (positive(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (positive(T2-a2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,REAL(T2))-primitive22(T1,Td,j,l,size,REAL(a1));
						}
					}
					if (bound(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1);
					}
				}
				if (positive(T1-a1,k))
				{
					if (positive(T1-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
					if (positive(a2-T1,k))
					{
						if (positive(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (positive(T2-a2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
					}
					if (bound(T1-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
				}
				if (bound(T1-a1,k))
				{
					if (positive(T2-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
					}
					if (positive(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(a1));
					}
					if (bound(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
					}
				}
			}
		}
//Third possibility
	if (bound(a0-T0,k))
		{
			if (positive(a1-T1,k))
				{
					integral+=primitive11(T1,Td,j,l,size,REAL(T1))-primitive11(T1,Td,j,l,size,T0);
					if (positive(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,T2)-primitive21(T1,Td,j,l,size,REAL(T1));
					}
					if (positive(T2-a1,k))
					{
						if (positive(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (positive(T2-a2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1)+primitive22(T1,Td,j,l,size,REAL(T2))-primitive22(T1,Td,j,l,size,REAL(a1));
						}
					}
					if (bound(a1-T2,k))
					{
						integral+=primitive21(T1,Td,j,l,size,REAL(a1))-primitive21(T1,Td,j,l,size,T1);
					}
				}
				if (positive(T1-a1,k))
				{
					if (positive(T1-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
					if (positive(a2-T1,k))
					{
						if (positive(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (positive(T2-a2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
						if (bound(a2-T2,k))
						{
							integral+=primitive11(T1,Td,j,l,size,a1)-primitive11(T1,Td,j,l,size,a0)+primitive12(T1,Td,j,l,size,T1)-primitive12(T1,Td,j,l,size,a1)+primitive22(T1,Td,j,l,size,a2)-primitive22(T1,Td,j,l,size,REAL(T1));
						}
					}
					if (bound(T1-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive12(T1,Td,j,l,size,REAL(a2))-primitive12(T1,Td,j,l,size,REAL(a1));
					}
				}
				if (bound(T1-a1,k))
				{
					if (positive(T2-a2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
					}
					if (positive(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,T2)-primitive22(T1,Td,j,l,size,REAL(a1));
					}
					if (bound(a2-T2,k))
					{
						integral+=primitive11(T1,Td,j,l,size,REAL(a1))-primitive11(T1,Td,j,l,size,REAL(a0))+primitive22(T1,Td,j,l,size,REAL(a2))-primitive22(T1,Td,j,l,size,REAL(a1));
					}
				}
		}

	/*
	for (int l=0;l<1000;l++)
	{
		REAL point(a0+RATIONAL(2*l,1000*size));
		REAL inv1=inverse1(point);
		REAL inv2=inverse1(point);
		
		integral+=REAL(RATIONAL(2,1000*size))*phi(j,size,point)*lphi(i,size,point);
	}
	integral=integral*REAL(size);
	*/
	}
	REAL value=REAL(size)*integral;
	//iRRAM::cout << i <<" "<< j << " integral " <<integral << " value " << value << std::endl;
	//std::cin.get();
	*b.valueAddr=double(value);

	stiff_end();
	return 0;
}
