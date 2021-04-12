#include "realautder.h"

REALAUTDER::REALAUTDER()
{
	x=REAL(0);
	derx=REAL(0);
}

REALAUTDER::REALAUTDER(const REAL &a)
{
	x=a;
	derx=REAL(1);
}

REALAUTDER::REALAUTDER(const REAL &a,const REAL &b)
{
	x=a;
	derx=b;
}

//Operators

REALAUTDER& REALAUTDER::operator=(const REALAUTDER &a)
{
	this->x=a.x;
	this->derx=a.derx;
	return *this;
}

REALAUTDER operator+(const REALAUTDER &a, const REALAUTDER &b)
{
    REALAUTDER temp;
    temp.x = a.x + b.x;
    temp.derx = a.derx + b.derx;
	return temp;
}

REALAUTDER operator-(const REALAUTDER &a, const REALAUTDER &b)
{
    REALAUTDER temp;
    temp.x = a.x - b.x;
    temp.derx = a.derx - b.derx;
	return temp;
}

REALAUTDER operator*(const REALAUTDER &a, const REALAUTDER &b)
{
    REALAUTDER temp;
    temp.x = (a.x) * (b.x);
    temp.derx = (a.derx)*(b.x) + (a.x)*(b.derx);
	return temp;
}

//Division has some important issues we have to deal with...
REALAUTDER operator/(const REALAUTDER &a, const REALAUTDER &b)
{
	
    REALAUTDER temp;
    temp.x = a.x / b.x;
    temp.derx = (a.derx-(temp.x*b.derx))/b.x;
	return temp;
}

// Trascendental functions

REALAUTDER sin(REALAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=sin(a.x);
	tempderx=a.derx*cos(a.x);
	return REALAUTDER(tempx,tempderx);
}

REALAUTDER cos(REALAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=cos(a.x);
	tempderx=-1*a.derx*sin(a.x);
	return REALAUTDER(tempx,tempderx);
}


//Same problem as with division
REALAUTDER log(REALAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=log(a.x);
	tempderx=a.derx*1/(a.x);
	return REALAUTDER(tempx,tempderx);
}

REALAUTDER power(REALAUTDER &a,REAL &alpha)
{
	REAL tempx;
	REAL tempderx;
	tempx=power(a.x,alpha);
	tempderx=a.derx*alpha*power(a.x,alpha-1);
	return REALAUTDER(tempx,tempderx);
}

REALAUTDER abs(REALAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=abs(a.x);
	tempderx=a.derx*(a.x/abs(a.x));
	return REALAUTDER(tempx,tempderx);
}

REALAUTDER exp(REALAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=exp(a.x);
	tempderx=a.derx*exp(a.x);
	return REALAUTDER(tempx,tempderx);
}

template <> Taylor<2> power(const Taylor<2> &a,REAL &alpha)
{
	Taylor<2> temp;
	temp.coeff[0]=power(a.coeff[0],alpha);
	temp.coeff[1]=a.coeff[1]*alpha*power(a.coeff[0],alpha-1);
	//iRRAM::cout << "Pow2 " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <> Taylor<2> operator /(const Taylor<2> &a,const Taylor<2> &b)
{
	Taylor<2> temp;
	//it is missing the check for division by 0!
	temp.coeff[0]=a.coeff[0]/b.coeff[0];
	temp.coeff[1]=(a.coeff[1]-temp.coeff[0]*b.coeff[1])/b.coeff[0];
	return temp;
}



template <> Taylor<3> power(const Taylor<3> &a,REAL &alpha)
{
	Taylor<3> temp;
	temp.coeff[0]=power(a.coeff[0],alpha);
	temp.coeff[1]=a.coeff[1]*alpha*power(a.coeff[0],alpha-REAL(1));
	temp.coeff[2]=alpha*(alpha-REAL(1))*power(a.coeff[0],alpha-2)*power(a.coeff[1],2)+a.coeff[2]*alpha*power(a.coeff[0],alpha-1);
	//iRRAM::cout << "Pow3 " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <> Taylor<4> power(const Taylor<4> &a,REAL &alpha)
{
	Taylor<4> temp;
	temp.coeff[0]=power(a.coeff[0],alpha);
	temp.coeff[1]=a.coeff[1]*alpha*power(a.coeff[0],alpha-REAL(1));
	temp.coeff[2]=alpha*(alpha-REAL(1))*power(a.coeff[0],alpha-2)*power(a.coeff[1],2)+a.coeff[2]*alpha*power(a.coeff[0],alpha-1);
	temp.coeff[3]=alpha*(alpha-REAL(1))*(alpha-REAL(2))*power(a.coeff[0],alpha-3)*power(a.coeff[1],2)+alpha*(alpha-REAL(1))*power(a.coeff[0],alpha-2)*REAL(2)*a.coeff[2]*a.coeff[1]+a.coeff[3]*alpha*power(a.coeff[0],alpha-1)+a.coeff[2]*alpha*(alpha-REAL(1))*power(a.coeff[0],alpha-2)*a.coeff[1];
	//iRRAM::cout << "Pow3 " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}


