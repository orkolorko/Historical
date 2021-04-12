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
 
/*! \file actual-genmatrix-ulam.cxx
\brief The file were resides the actual algorithm that computes the matrix
\ingroup Ulam
*/

 
#include <list>
#include "petscmat.h"
#include <boost/math/special_functions/fpclassify.hpp>
#include "rapfloat.h"
#include "rinterval.h"
#include "rpartition.h"
#include "actual-genmatrix-ulam.h"

/*In this function we compute the Ulam approximation of the Matrix,
	using the exaustion algorithm explained in the article.
	Please note that the algorithm is portable, with little effort to other
	rigorous numerical types.	
	**/
/*!\brief generates the matrix*/
/*! \param size the size of the partition
 * \param step the size of the intervals we throw away
 * \param Aaddr a pointer to the matrix A
 * \param MatErrorAddr a pointer to a variable that stores the global error of the matrix
 * \param intreport controls frequency of output
 * \param debug controls whether to output debug information
 * */
//! \ingroup Ulam

PetscErrorCode genmatrix_ulam(PetscInt partsize,PetscScalar max_error,PetscInt intreport,bool debug,Mat * const Aaddr,PetscScalar * const MatErrorAddr)
{
	PetscErrorCode ierr(0);
	ierr=PetscPrintf(MPI_COMM_WORLD,"Generate the matrix\n");CHKERRQ(ierr);
	
	// number of the intervals for the subpartitions, when using the mixing algorithm 
	int supder=8;
	int size=partsize;
	RigScalar len(1,partsize);
	
	rapfloat mix(0,0);
	bool discontinuity;
	APFloat LocError(0);
	APFloat PrescrError=max_error;
	
	//sets the domain
	rinterval dom(RigScalar(0),RigScalar(1));
	
	//builds the partition
	rpartition locpartition(dom,partsize);
	
	rinterval Dom;
	rinterval CoDom;
	rinterval temp;
	
	for (int i=0;i<partsize;i++)
		{
				mix=rapfloat(0,0);
				discontinuity=false;
				Dom=locpartition.read_value(i);
				temp=rinterval(dynamic(Dom));
				
				//check if the discontinuity is contained in the interval
				discontinuity=checkdiscontinuity(Dom);
				
				if (discontinuity==false)
				{
				//gets the index of the interval which contains the central point of the interval
				int index=int(size*(boost::rational_cast<double>(temp.intpoint())));
				
				//computes the indexes that we are going to compute
				int minind,maxind;
				//we check that we do not escape from the admissible indexes
				if ((index-supder)>=0) minind=index-supder;
					else minind=0;
				//we check that we do not escape from the admissible indexes
				if ((index+supder)<=size) maxind=index+supder;
					else maxind=size;
				
					for (int j=minind;j<maxind;j++)	
						{
							//please note that here, when the dynamic is given
							//as a quotient, there are alternatives ways to implement
							//this part
							CoDom=locpartition.read_value(j);
							// Calls the function that computes m(T^{-1}I_i\cap I_j)
							mix=mixing(CoDom,Dom, supder,PrescrError);
							// Divides the computed measure by m(I_i) to get the conditional probability
							// fills up the matrix, future development is going to use a vector of vector matrix, to build up
							// each row in parallel
							if (mix.getvalue()!=0)
								{
									APFloat normmix=mix.getvalue()*partsize;
									// this is the only step that needs modification if we want to change the scalars
									// cout << rowindex << " " << colindex<< " "<< normmix << endl;
									ierr=MatSetValue(*Aaddr,j,i,normmix,ADD_VALUES);CHKERRQ(ierr);
								}
							// keeps track of the biggest error in the computation of the coefficients
							LocError=(mix.geterror())*partsize;
							if (LocError>*MatErrorAddr){*MatErrorAddr=LocError;};
						}
					// A small output to keep track of the program
					if (i%intreport==0)
					{
						cout << "I'm computing row " << i << endl;
					}
				
				}
				
				//if the interval contains the discontinuity
				if (discontinuity==true)
				{
				
				//since there is a discontinuity, we compute the indexes near the images of the two endpoints
				int index1=int(size*(boost::rational_cast<double>(temp.leftend())));
				int index2=int(size*(boost::rational_cast<double>(temp.rightend())));
				
				//compute the indexes that we are going to compute
				int minind1,maxind1;
				//take only admissible indexes
				if ((index1-supder)>=0) minind1=index1-supder;
					else minind1=0;
				//take only admissible indexes
				if ((index1+supder)<=size) maxind1=index1+supder;
					else maxind1=size;
					
				//compute the indexes that we are going to compute
					
				int minind2,maxind2;
				//take only admissible indexes
				if ((index2-supder)>=0) minind2=index2-supder;
					else minind2=0;
				//take only admissible indexes
				if ((index2+supder)<=size) maxind2=index2+supder;
					else maxind2=size;
				
					//computes for the indexes near one end
					for (int j=minind1;j<maxind1;j++)	
						{
							CoDom=locpartition.read_value(j);
							// Calls the function that computes m(T^{-1}I_i\cap I_j)
							mix=mixing(CoDom, Dom, supder, PrescrError);
							// Divides the computed measure by m(I_i) to get the conditional probability
							// fills up the matrix, future development is going to use a vector of vector matrix, to build up
							// each row in parallel
							if (mix.getvalue()!=0)
								{
									APFloat normmix=mix.getvalue()*partsize;
									// this is the only step that needs modification if we want to change the scalars
									// cout << rowindex << " " << colindex<< " "<< normmix << endl;
									ierr=MatSetValue(*Aaddr,j,i,normmix,ADD_VALUES);CHKERRQ(ierr);
								}
							// keeps track of the biggest error in the computation of the coefficients
							LocError=(mix.geterror())*partsize;
							if (LocError>*MatErrorAddr){*MatErrorAddr=LocError;};
						}
					//computes for the indexes near the other end
					for (int j=minind2;j<maxind2;j++)	
						{
							CoDom=locpartition.read_value(j);
							// Calls the function that computes m(T^{-1}I_i\cap I_j)
							mix=mixing(CoDom, Dom, supder, PrescrError);
							// Divides the computed measure by m(I_i) to get the conditional probability
							// fills up the matrix, future development is going to use a vector of vector matrix, to build up
							// each row in parallel
							if (mix.getvalue()!=0)
								{
									APFloat normmix=mix.getvalue()*partsize;
									// this is the only step that needs modification if we want to change the scalars
									// cout << rowindex << " " << colindex<< " "<< normmix << endl;
									ierr=MatSetValue(*Aaddr,j,i,normmix,ADD_VALUES);CHKERRQ(ierr);
								}
							// keeps track of the biggest error in the computation of the coefficients
							LocError=(mix.geterror())*partsize;
							if (LocError>*MatErrorAddr){*MatErrorAddr=LocError;};
						}
					// A small output to keep track of the program
					
					if (i%intreport==0)
					{
						cout << "I'm computing row " << i << endl;
					}
				}
			// modifies the output
			if (LocError>=*MatErrorAddr) *MatErrorAddr=LocError;
		}
		
	return ierr;
}
