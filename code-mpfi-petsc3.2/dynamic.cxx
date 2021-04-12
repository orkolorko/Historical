#include "dynamic.h"
#include "realautder.h"

PetscInt dynamicestimate2(PetscInt *inputAddr, PetscScalar * output)
{
	REAL num(inputAddr[0]);
	REAL den(inputAddr[1]);
	Taylor<2> point(num/den,REAL(1));
	Taylor<2> temp=DynamicTaylor(DynamicTaylor(point));
	output[0]=double(temp.viewcoeff(0));
	output[1]=double(temp.viewcoeff(1));
	return 0;
}

PetscInt dynamicestimate(PetscInt *inputAddr, PetscScalar * output)
{
	REAL num(inputAddr[0]);
	REAL den(inputAddr[1]);
	Taylor<2> point(num/den,REAL(1));
	Taylor<2> temp=DynamicTaylor(point);
	output[0]=double(temp.viewcoeff(0));
	output[1]=double(temp.viewcoeff(1));
	return 0;
}

PetscScalar DoubleDynamic(PetscInt i, PetscInt j)
{
	REAL num(i);
	REAL den(j);
	REAL point=num/den;
	REAL temp=dynamic(dynamic(point));
	return double(temp);
}

REAL dynamic(REAL x)
{
	REAL temp=lorenz(lorenz(x));
	return temp;
}

REAL lorenz(REAL x)
{
	REAL temp=111;
	REAL alpha=REAL(95)/REAL(100);
	REAL theta=REAL(17)/REAL(10);
	x=x-REAL(RATIONAL(1,2));
	int k=-100;
	if (positive(x,k))
	{
	temp=theta*power(x,alpha);
	}
	if (positive(-1*x,k))
	{
	temp=-1*theta*power(abs(x),alpha)+REAL(1);
	}
	return temp;
}




int IntervalDynamic(RATIONAL lend, RATIONAL rend, REAL *outlend, REAL *outrend)
{
	//Please note that the code is Lorenz Specific!!!
	int l=-200;
	// If one of the endpoints of the starting interval is 1/2 then we prescribe the output interval
	// respecting the discontinuity of the function
	if ((lend-RATIONAL(1,2)==0) || (rend-RATIONAL(1,2)==0))
	{
		if (lend-RATIONAL(1,2)==0) // if the input interval is [1/2,alpha], then the image interval is [T(0),T^2(\alpha)]
		{
			*outlend=lorenz(REAL(0));
			*outrend=dynamic(REAL(rend));
		}
		if (rend-RATIONAL(1,2)==0) // if the input interval is [alpha,1/2], then the image interval is [T^2(\alpha),T(1)]
		{
			*outlend=dynamic(REAL(lend));
			*outrend=lorenz(REAL(1));
		}
	}
	else if ((bound(dynamic(lend)-REAL(RATIONAL(1,2)),l)) || (bound(dynamic(rend)-REAL(RATIONAL(1,2)),l)))
	{
		if (bound(dynamic(lend)-REAL(RATIONAL(1,2)),l)) // if the image of the first interval is [1/2,\alpha], then the image interval is [0,T(\alpha)]
		{
			*outlend=REAL(0);
			*outrend=dynamic(REAL(rend));
		}
		if (bound(dynamic(rend)-REAL(RATIONAL(1,2)),l)) // if the image of the first interval is [\alpha,1/2], then the image interval is [0,T(\alpha)]
		{
			*outlend=dynamic(REAL(lend));
			*outrend=REAL(1);
		}	
	}
	else
	{
		*outlend=dynamic(REAL(lend));
		*outrend=dynamic(REAL(rend));
	}
	return 0;
}

/*
int IntervalDynamic(RATIONAL lend, RATIONAL rend, REAL *outlend, REAL *outrend)
{
	//Please note that the code is Lorenz first iterate Specific!!!
	int l=-200;
	// If one of the endpoints of the starting interval is 1/2 then we prescribe the output interval
	// respecting the discontinuity of the function
	if ((lend-RATIONAL(1,2)==0) || (rend-RATIONAL(1,2)==0))
	{
		if (lend-RATIONAL(1,2)==0) // if the input interval is [1/2,alpha], then the image interval is [T(0),T^2(\alpha)]
		{
			*outlend=REAL(0);
			*outrend=dynamic(REAL(rend));
		}
		if (rend-RATIONAL(1,2)==0) // if the input interval is [alpha,1/2], then the image interval is [T^2(\alpha),T(1)]
		{
			*outlend=dynamic(REAL(lend));
			*outrend=REAL(1);
		}
	}
	else
	{
		*outlend=dynamic(REAL(lend));
		*outrend=dynamic(REAL(rend));
	}
	return 0;
}
*/
