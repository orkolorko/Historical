#include "interval.h"
#include <iostream>

#ifndef MPFIAUTDER_H
#define MPFIAUTDER_H

class MPFIAUTDER
{
	Interval x;
	Interval derx;
	
	public:
	
	MPFIAUTDER();
	MPFIAUTDER(const mp_prec_t);
	MPFIAUTDER(const Interval );
	MPFIAUTDER(const Interval ,const Interval );
	//~MPFIAUTDER();
	
	MPFIAUTDER& operator=(const MPFIAUTDER& );
	
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const MPFIAUTDER &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const MPFIAUTDER &b);
	
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const MPFIAUTDER &b);
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const MPFIAUTDER &b);
	
	friend MPFIAUTDER sin(MPFIAUTDER &);
    friend MPFIAUTDER cos(MPFIAUTDER &);
    friend MPFIAUTDER exp(MPFIAUTDER &);
    friend MPFIAUTDER log(MPFIAUTDER &);
    
    friend MPFIAUTDER power(MPFIAUTDER &,const Interval &);
    friend MPFIAUTDER power(MPFIAUTDER &,const double &);
    friend MPFIAUTDER power(MPFIAUTDER &,const mpq_t &);
    friend MPFIAUTDER power(MPFIAUTDER &,const mpfr_t &);
    
    friend MPFIAUTDER abs(MPFIAUTDER &);
	
	Interval val() const { return x;};
    Interval derval() const {return derx;};
    
    friend MPFIAUTDER dynamicautder(Interval x, int precision);
	
	//Operations with scalars
	
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const Interval &b);
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const double &b);
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const unsigned int &b);
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const signed int &b);
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const mpq_t &b);
	friend MPFIAUTDER operator+ (const  MPFIAUTDER &a, const mpfr_t &b);
	
	friend MPFIAUTDER operator+ (const Interval &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator+ (const double &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator+ (const unsigned int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator+ (const signed int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator+ (const mpq_t &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator+ (const mpfr_t &b, const  MPFIAUTDER &a);
	
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const Interval &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const double &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const unsigned int &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const signed int &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const mpq_t &b);
	friend MPFIAUTDER operator- (const  MPFIAUTDER &a, const mpfr_t &b);
	
	friend MPFIAUTDER operator- (const Interval &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator- (const double &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator- (const unsigned int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator- (const signed int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator- (const mpq_t &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator- (const mpfr_t &b, const  MPFIAUTDER &a);
	
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const Interval &b);
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const double &b);
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const unsigned int &b);
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const signed int &b);
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const mpq_t &b);
	friend MPFIAUTDER operator* (const  MPFIAUTDER &a, const mpfr_t &b);
	
	friend MPFIAUTDER operator* (const Interval &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator* (const unsigned int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator* (const signed int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator* (const mpq_t &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator* (const mpfr_t &b, const  MPFIAUTDER &a);
	
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const Interval &b);	
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const double &b);
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const unsigned int &b);
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const signed int &b);
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const mpq_t &b);
	friend MPFIAUTDER operator/ (const  MPFIAUTDER &a, const mpfr_t &b);
	
	friend MPFIAUTDER operator/ (const Interval &b, const  MPFIAUTDER &a);	
	friend MPFIAUTDER operator/ (const double &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator/ (const unsigned int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator/ (const signed int &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator/ (const mpq_t &b, const  MPFIAUTDER &a);
	friend MPFIAUTDER operator/ (const mpfr_t &b, const  MPFIAUTDER &a);
	
	//int setconstant(REAL &);
	/*
	
    friend MPFIAUTDER sin(MPFIAUTDER &);
    friend MPFIAUTDER cos(MPFIAUTDER &);
    friend MPFIAUTDER exp(MPFIAUTDER &);
    friend MPFIAUTDER log(MPFIAUTDER &);
    friend MPFIAUTDER power(MPFIAUTDER &,REAL &);
    friend MPFIAUTDER abs(MPFIAUTDER &);
    
    friend int dynamicestimate(int*,double *);
    
    */
};

MPFIAUTDER dynamicautder(const MPFIAUTDER &x);

/*
template <int k> class Taylor;
template <int k> Taylor<k> operator+(const Taylor<k>&a, const Taylor<k> &b)
{
	Taylor<k> temp;
	for (int i=0;i<k;i++)
	{
		temp.coeff[i]=a.coeff[i]+b.coeff[i];
	}
	//iRRAM::cout << "Sum "<< temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}


template <int k> Taylor<k> operator-(const Taylor<k> &a, const Taylor<k> &b)
{
	Taylor<k> temp;
	for (int i=0;i<k;i++)
	{
		temp.coeff[i]=a.coeff[i]-b.coeff[i];
	}
	//iRRAM::cout << "Min "<<temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> Taylor<k> operator*(const Taylor<k>&a, const Taylor<k> &b)
{
	Taylor<k> temp;
	for (int i=0;i<k;i++)
	{
		for (int l=0;l<=i;l++)
		{
			temp.coeff[i]+=a.coeff[l]*b.coeff[i-l];
		}
	}
	//iRRAM::cout << "Mul " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}


template <int k> Taylor<k> operator/(const Taylor<k>&a, const Taylor<k> &b)
{
	Taylor<k> temp;
	for (int i=0;i<k;i++)
	{
		temp.coeff[i]=a.coeff[i];
		for (int l=0;l<i;l++)
		{
			temp.coeff[i]=temp.coeff[i]-(temp.coeff[l]*b.coeff[i-l]);
		}
		temp.coeff[i]=1/b.coeff[0]*temp.coeff[i];
	}
	//iRRAM::cout << "Div " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> Taylor<k> exp(const Taylor<k>&a)
{
	Taylor<k> temp;
	temp[0]=exp(a.coeff[0]);
	for (int i=0;i<k;i++)
	{
		for (int j=1;j<i;j++)
		{
			temp[i]+=RATIONAL(1,i)*RATIONAL(j,1)*a.coeff[j]*temp[i-j];
		}
	}
	//iRRAM::cout << "Exp " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> Taylor<k> abs(const Taylor<k>&a)
{
	Taylor<k> temp;
	temp.coeff[0]=abs(a.coeff[0]);
	for (int i=0;i<k;i++)
	{
		temp.coeff[i]=a.coeff[i]*(a.coeff[0]/abs(a.coeff[0]));;
	}
	//iRRAM::cout << "Abs " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> Taylor<k> log(const Taylor<k>&a)
{
	Taylor<k> temp;
	temp[0]=log(a.coeff[0]);
	for (int i=1;i<k;i++)
	{
		temp[i]=a.coeff[i];
		for (int j=1;j<i;j++)
		{
			temp[i]-=RATIONAL(1,i)*j*temp[j]*a.coeff[i-j];
		}
		temp[i]=1/a.temp[0]*temp[i];
	}
	//iRRAM::cout << "Log " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> Taylor<k> power(const Taylor<k> &a,REAL &alpha)
{
	Taylor<k> temp;
	temp.coeff[0]=power(a.coeff[0],alpha);
	for (int i=1;i<k;i++)
	{
		for (int j=1;j<i;j++)
		{
			temp.coeff[i]+=((alpha+1)*REAL(INTEGER(j)*RATIONAL(1,i))-1)*a.coeff[j]*temp.coeff[i-j];
		}
		temp.coeff[i]=1/a.coeff[0]*temp.coeff[i];
	}
	//iRRAM::cout << "Pow " << temp.viewcoeff(0) << " " << temp.viewcoeff(1) << std::endl;
	return temp;
}

template <int k> class Taylor
{
	REAL coeff[k];
	
	public:
		//constructors
		Taylor<k>();
		Taylor<k>(const REAL &);
		Taylor<k>(const REAL &,const REAL &);
		//viewer
		REAL viewcoeff(int degree) const;
		Taylor<k>& operator=(const Taylor<k>&);
	
		//operators
		
		friend Taylor<k> operator+<>(const Taylor<k>&, const Taylor<k> &);
		friend Taylor<k> operator-<>(const Taylor<k>&, const Taylor<k> &);
		friend Taylor<k> operator*<>(const Taylor<k>&, const Taylor<k> &);
		friend Taylor<k> operator/<>(const Taylor<k>&, const Taylor<k> &);
		friend Taylor<k> exp<>(const Taylor<k>&);
		friend Taylor<k> log<>(const Taylor<k>&);
		friend Taylor<k> abs<>(const Taylor<k>&);
		friend Taylor<k> power<>(const Taylor<k>&, REAL &);
};

template <int k> Taylor<k>& Taylor<k>::operator=(const Taylor<k>&a)
{
	if (this != &a)
	for (int i=0;i<k;i++)
	{
		this->coeff[i]=a.coeff[i];
	}
	return *this;
}

template <int k> REAL Taylor<k>::viewcoeff(int degree) const
{
	return this->coeff[degree];
}

template <int k> Taylor<k>::Taylor()
{
	for (int i=0;i<k;i++)
		{
			coeff[i]=REAL(0);
		}
	//std::cout << k << std::endl;
}

template <int k> Taylor<k>::Taylor(const REAL &x)
{
	coeff[0]=x;
	
	for (int i=1;i<k;i++)
		{
			coeff[i]=REAL(0);
		}
	//std::cout << k << std::endl;
}


template <int k> Taylor<k>::Taylor(const REAL &x,const REAL& derx)
{
	coeff[0]=x;
	coeff[1]=derx;
	
	for (int i=2;i<k;i++)
		{
			coeff[i]=REAL(0);
		}
	//std::cout << k << std::endl;
}

template <int k> int output(Taylor<k> &output)
{
	for (int i=0;i<k;i++)
	{
		iRRAM::cout<<output.viewcoeff(i) << std::endl;
	}
	iRRAM::cout<< std::endl;
	return 0;
}
*/
#endif
