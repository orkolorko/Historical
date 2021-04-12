#include "iRRAM_core.h"
#include "iRRAM_lib.h"
#include "petscmat.h"
#include "realautder.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 

PetscScalar DoubleDynamic(PetscInt i, PetscInt j);
REALAUTDER dynamicautder(REALAUTDER x);
REAL dynamic(REAL x);
PetscInt IntervalDynamic(RATIONAL lend, RATIONAL rend, REAL *outlend, REAL *outrend);
PetscInt dynamicestimate(PetscInt *input, PetscScalar * output);
PetscInt dynamicestimate2(PetscInt *input, PetscScalar * output);
REAL lorenz(REAL x);

template <int k> Taylor<k> DynamicTaylor(Taylor<k> x)
{
	//iRRAM::cout << "Fino a qua ci sono arrivato" << std::endl;
	REAL alpha(REAL(95)/REAL(100));
	Taylor<k> temp(111,0);
	Taylor<k> theta(REAL(17)/REAL(10),0);
	Taylor<k> y=x-Taylor<k>(RATIONAL(1,2),0);
	int l=-100; //this is an hack, found a way to connect this to the required error on the coefficients of the matrix
	//
	//output(y);
	if (positive(y.viewcoeff(0),l))
	{
	temp=theta*power(y,alpha);
	}
	if (positive(-1*y.viewcoeff(0),l))
	{
	y=abs(y);
	//output(y);
	temp=Taylor<k>(REAL(-1),0)*theta*power(y,alpha)+Taylor<k>(REAL(1),0);//;
	//output(temp);
	//temp=power(y,alpha);
	//temp=y;
	}
	return temp;
}

template <int k> Taylor<k> intdistor(const Taylor<k> &x)
{
	Taylor<k> temp;
	temp=abs(x-Taylor<k>(REAL(1),0));
	REAL alpha=abs(REAL(1)/x.viewcoeff(0));
	Taylor<k> y=temp/abs(x);
	temp=power(y,alpha);
	return y;
}





/*
template <int k> Taylor<k> DynamicTaylor2(Taylor<k> x)
{
	Taylor<k> temp(111,0);
	REAL alpha(REAL(8)/REAL(10));
	Taylor<k> theta(REAL(17)/REAL(10),0);
	
	Taylor<k> y=x-Taylor<k>(RATIONAL(1,2),0);
	int l=-100; //this is an hack, found a way to connect this to the required error on the coefficients of the matrix
	//iRRAM::cout << "Fino a qua ci sono arrivato" << endl;
	Taylor<k> test=DynamicTaylor(y);
	
	if (positive(y.viewcoeff(0),l) && positive(-1*(test.viewcoeff(0)-RATIONAL(1,2)),l))
	{
	temp=Taylor<k>(REAL(-1),0)*theta*power(abs(theta*power(y,alpha)-Taylor<k>(RATIONAL(1,2),0)),alpha)+1;
	}
	if (positive(y.viewcoeff(0),l) && positive(test.viewcoeff(0)-RATIONAL(1,2),l))
	{
	temp=theta*power(theta*power(y,alpha)-Taylor<k>(RATIONAL(1,2),0),alpha);
	}
	if (positive(-1*y.viewcoeff(0),l) && positive(-1*(test.viewcoeff(0)-RATIONAL(1,2)),l))
	{
	temp=Taylor<k>(REAL(-1),0)*theta*power(abs(Taylor<k>(REAL(-1),0)*theta*power(abs(y),alpha)+Taylor<k>(RATIONAL(1,2),0)),alpha)+1;
	}
	if (positive(-1*y.viewcoeff(0),l) && positive(test.viewcoeff(0)-RATIONAL(1,2),l))
	{
	temp=theta*power(Taylor<k>(REAL(-1),0)*theta*power(abs(y),alpha)+Taylor<k>(RATIONAL(1,2),0),alpha);
	}
	return temp;
}
*/

