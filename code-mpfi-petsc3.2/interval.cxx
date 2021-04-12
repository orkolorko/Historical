#include "interval.h"
#include <iostream>

Interval::Interval()
{
	mpfi_init(x);
}

Interval::Interval(const mp_prec_t precision)
{
	mpfi_init2(x,precision);
}

Interval::Interval(const mpfi_t a)
{
	mp_prec_t precision=mpfi_get_prec(a);
	mpfi_init2(x,precision);
	mpfi_set(x,a);
}

Interval::Interval(const double value)
{
	mpfi_init(x);
	mpfi_set_d(x,value);
}

Interval::Interval(const double value,const mp_prec_t precision)
{
	mpfi_init2(x,precision);
	mpfi_set_d(x,value);
}

Interval::Interval(double left,double right)
{
	mpfi_init(x);
	mpfi_interv_d(x,left,right);
}

Interval::Interval(double left ,double right,const mp_prec_t precision)
{
	//mpfr_t Left;
	//mpfr_t Right;
	
	mpfi_init2(x,precision);
	//mpfr_init2(Left,precision);
	//mpfr_init2(Right,precision);
	/*
	std::cout << left << std::endl;
	std::cout << right << std::endl;
	std::cout <<*/ 
	mpfi_interv_d(x,left,right);/* << std::endl;
	mpfi_get_left(Left,x);
	mpfi_get_right(Right,x);
	mpfr_out_str ((FILE *) 0, 10, 20, Left, MPFR_RNDD);
	std::cout << std::endl;
	mpfr_out_str ((FILE *) 0, 10, 20, Right, MPFR_RNDU);
	std::cout << std::endl;
	
	mpfr_clear(Left);
	mpfr_clear(Right);
	*/
}

Interval::Interval(const mpq_t value)
{
	mpfi_init(x);
	mpfi_set_q(x,value);
}

Interval::Interval(const mpq_t value,const mp_prec_t precision)
{
	mpfi_init2(x,precision);
	mpfi_set_q(x,value);
}

Interval::Interval(const mpq_t left,const mpq_t right)
{
	mpfi_init(x);
	mpfi_interv_q(x,left,right);
}

Interval::Interval(const mpq_t left,const mpq_t right,const mp_prec_t precision)
{
	mpfi_init2(x,precision);
	mpfi_interv_q(x,left,right);
}

Interval::Interval(const mpfr_t value)
{
	mpfi_init(x);
	mpfi_set_fr(x,value);
}

Interval::Interval(const mpfr_t value,const mp_prec_t precision)
{
	mpfi_init2(x,precision);
	mpfi_set_fr(x,value);
}
	
Interval::Interval(const mpfr_t left,const mpfr_t right)
{
	mpfi_init(x);
	mpfi_interv_fr(x,left,right);
} 

Interval::Interval(const mpfr_t left,const mpfr_t right, const mp_prec_t precision)
{
	mpfi_init2(x,precision);
	mpfi_interv_fr(x,left,right);
}

Interval::~Interval()
{
	mpfi_clear(x);
}

Interval& Interval::operator=(const Interval& a)
{
	if (this==&a) return *this;
	
	mpfi_clear(this->x);
	mpfi_init2(this->x,mpfi_get_prec(a.x));
	mpfi_set(this->x,a.x);
	return *this;
}

double Interval::get_left_d() const
{
	double output;
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_t Left;
	mpfr_init2(Left,precision);
	mpfi_get_left(Left,x);
	output=mpfr_get_d(Left,MPFR_RNDD);
	mpfr_clear(Left);
	return output;
}

double Interval::get_right_d() const
{
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_t Right;
	mpfr_init2(Right,precision);
	mpfi_get_right(Right,x);
	double output=mpfr_get_d(Right,MPFR_RNDU);
	mpfr_clear(Right);
	return output;
}

double Interval::get_diam_d() const
{
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_t Diam;
	mpfr_init2(Diam,precision);
	mpfi_diam(Diam,x);
	double output=mpfr_get_d(Diam,MPFR_RNDU);
	mpfr_clear(Diam);
	return output;
}


int Interval::get_left(mpfr_t & Left) const
{
	mpfr_clear(Left);
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_init2(Left,precision);
	mpfi_get_left(Left,x);
	//mpfr_out_str ((FILE *) 0, 10, 20, Left, MPFR_RNDD);
	return 0;
}

int Interval::get_right(mpfr_t & Right) const
{
	mpfr_clear(Right);
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_init2(Right,precision);
	mpfi_get_right(Right,x);
	//mpfr_out_str ((FILE *) 0, 10, 20, Right, MPFR_RNDU);
	return 0;
}

int Interval::get_mid(mpfr_t & Mid) const
{
	mpfr_clear(Mid);
	mp_prec_t precision=mpfi_get_prec(x);
	mpfr_init2(Mid,precision);
	mpfi_mid(Mid,x);
	//mpfr_out_str ((FILE *) 0, 10, 20, Right, MPFR_RNDU);
	return 0;
}

int Interval::get_interval(mpfi_t & y) const
{
	mpfi_clear(y);
	mp_prec_t precision=mpfi_get_prec(x);
	mpfi_init2(y,precision);
	mpfi_set(y,x);
	//mpfr_out_str ((FILE *) 0, 10, 20, Right, MPFR_RNDU);
	return 0;
}

int Interval::bisect(Interval &a,Interval &b)
{
	mpfi_t half1,half2;
	mp_prec_t precision=mpfi_get_prec(x);
	mpfi_init2(half1,precision);
	mpfi_init2(half2,precision);
	mpfi_bisect(half1,half2,x);
	a=Interval(half1);
	b=Interval(half2);
	return 0;
}

Interval intersect (const Interval &a, const Interval &b)
{
	mp_prec_t precision;
	mp_prec_t precision1=mpfi_get_prec(a.x);
	mp_prec_t precision2=mpfi_get_prec(b.x);
	precision=precision1;
	if (precision2>precision) precision=precision2;
	Interval temp(precision);
    
    mpfi_intersect (temp.x,a.x,b.x);
    return temp;
}

	
Interval operator+ (const Interval &a, const Interval &b)
{
	mp_prec_t precision;
	mp_prec_t precision1=mpfi_get_prec(a.x);
	mp_prec_t precision2=mpfi_get_prec(b.x);
	precision=precision1;
	if (precision2>precision) precision=precision2;
	Interval temp(precision);
    
    mpfi_add (temp.x,a.x,b.x);
    return temp;
}

Interval operator+ (const Interval &a, const double &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_add_d(temp.x,a.x,b);
	return temp;
}

Interval operator+ (const  Interval &a, const unsigned int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_add_ui(temp.x,a.x,b);
	return temp;
}

Interval operator+ (const  Interval &a, const signed int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_add_si(temp.x,a.x,b);
	return temp;
}

Interval operator+ (const  Interval &a, const mpq_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_add_q(temp.x,a.x,b);
	return temp;
}

Interval operator+ (const  Interval &a, const mpfr_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_add_fr(temp.x,a.x,b);
	return temp;
}

Interval operator+ (const double &b, const  Interval &a)
{
	return (a+b);
}

Interval operator+ (const unsigned int &b, const  Interval &a)
{
	return (a+b);
}

Interval operator+ (const signed int &b, const  Interval &a)
{
	return (a+b);
}	

Interval operator+ (const mpq_t &b, const  Interval &a)
{
	return (a+b);
}	

Interval operator+ (const mpfr_t &b, const  Interval &a)
{
	return (a+b);
}

Interval operator- (const  Interval &a, const Interval &b)
{
	mp_prec_t precision;
	mp_prec_t precision1=mpfi_get_prec(a.x);
	mp_prec_t precision2=mpfi_get_prec(b.x);
	precision=precision1;
	if (precision2>precision) precision=precision2;
	Interval temp(precision);
    
    mpfi_sub (temp.x,a.x,b.x);
    return temp;
}

Interval operator- (const  Interval &a, const double &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_sub_d(temp.x,a.x,b);
	return temp;
}

Interval operator- (const  Interval &a, const unsigned int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_sub_ui(temp.x,a.x,b);
	return temp;	
}

Interval operator- (const  Interval &a, const signed int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_sub_si(temp.x,a.x,b);
	return temp;
	
}

Interval operator- (const  Interval &a, const mpq_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_sub_q(temp.x,a.x,b);
	return temp;
}

Interval operator- (const  Interval &a, const mpfr_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_sub_fr(temp.x,a.x,b);
	return temp;
}

Interval operator- (const double &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_d_sub(temp.x,b,a.x);
	return temp;
}

Interval operator- (const unsigned int &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_ui_sub(temp.x,b,a.x);
	return temp;
}

Interval operator- (const signed int &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_si_sub(temp.x,b,a.x);
	return temp;
}

Interval operator- (const mpq_t &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_q_sub(temp.x,b,a.x);
	return temp;
}

Interval operator- (const mpfr_t &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_fr_sub(temp.x,b,a.x);
	return temp;
}

Interval operator* (const  Interval &a, const Interval &b)
{
	mp_prec_t precision;
	mp_prec_t precision1=mpfi_get_prec(a.x);
	mp_prec_t precision2=mpfi_get_prec(b.x);
	precision=precision1;
	if (precision2>precision) precision=precision2;
	Interval temp(precision);
    
    mpfi_mul (temp.x,a.x,b.x);
    return temp;
}

Interval operator* (const  Interval &a, const double &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_mul_d(temp.x,a.x,b);
	return temp;
}

Interval operator* (const  Interval &a, const unsigned int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_mul_ui(temp.x,a.x,b);
	return temp;
}

Interval operator* (const  Interval &a, const signed int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_mul_si(temp.x,a.x,b);
	return temp;
}

Interval operator* (const  Interval &a, const mpq_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_mul_q(temp.x,a.x,b);
	return temp;
}

Interval operator* (const  Interval &a, const mpfr_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_mul_fr(temp.x,a.x,b);
	return temp;
}

Interval operator* (const double &b, const  Interval &a)
{
	return (a*b);
}

Interval operator* (const unsigned int &b, const  Interval &a)
{
	return (a*b);
}

Interval operator* (const signed int &b, const  Interval &a)
{
	return (a*b);
}	

Interval operator* (const mpq_t &b, const  Interval &a)
{
	return (a*b);
}	

Interval operator* (const mpfr_t &b, const  Interval &a)
{
	return (a*b);
}

Interval operator/ (const  Interval &a, const Interval &b)
{
	mp_prec_t precision;
	mp_prec_t precision1=mpfi_get_prec(a.x);
	mp_prec_t precision2=mpfi_get_prec(b.x);
	precision=precision1;
	if (precision2>precision) precision=precision2;
	Interval temp(precision);
    
    mpfi_div (temp.x,a.x,b.x);
    return temp;
}

Interval operator/ (const  Interval &a, const double &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_div_d(temp.x,a.x,b);
	return temp;
}

Interval operator/ (const  Interval &a, const unsigned int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_div_ui(temp.x,a.x,b);
	return temp;
}

Interval operator/ (const  Interval &a, const signed int &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_div_si(temp.x,a.x,b);
	return temp;
}

Interval operator/ (const  Interval &a, const mpq_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_div_q(temp.x,a.x,b);
	return temp;
}

Interval operator/ (const  Interval &a, const mpfr_t &b)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_div_fr(temp.x,a.x,b);
	return temp;
}

Interval operator/ (const double &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_d_div(temp.x,b,a.x);
	return temp;
}

Interval operator/ (const unsigned int &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_ui_div(temp.x,b,a.x);
	return temp;
}

Interval operator/ (const signed int &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_si_div(temp.x,b,a.x);
	return temp;
}

Interval operator/ (const mpq_t &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_q_div(temp.x,b,a.x);
	return temp;
}

Interval operator/ (const mpfr_t &b, const  Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
	mpfi_fr_div(temp.x,b,a.x);
	return temp;
}


Interval sin(const Interval & a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_sin(temp.x,a.x);
    return temp;
}

Interval cos(const Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_cos(temp.x,a.x);
    return temp;
}

Interval exp(const Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_exp(temp.x,a.x);
    return temp;
}


Interval log(const Interval & a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_log(temp.x,a.x);
    return temp;
}

Interval power(const Interval & a,const Interval & alpha)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_log(temp.x,a.x);
    mpfi_mul(temp.x,temp.x,alpha.x);
    mpfi_exp(temp.x,temp.x);
    return temp;
}

Interval power(const Interval & a,const double & alpha)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_log(temp.x,a.x);
    mpfi_mul_d(temp.x,temp.x,alpha);
    mpfi_exp(temp.x,temp.x);
    return temp;
}

Interval power(const Interval & a,const mpq_t & alpha)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_log(temp.x,a.x);
    mpfi_mul_q(temp.x,temp.x,alpha);
    mpfi_exp(temp.x,temp.x);
    return temp;
}

Interval power(const Interval & a,const mpfr_t & alpha)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_log(temp.x,a.x);
    mpfi_mul_fr(temp.x,temp.x,alpha);
    mpfi_exp(temp.x,temp.x);
    return temp;
}

Interval abs(const Interval &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    Interval temp(precision);
    mpfi_abs(temp.x,a.x);
    return temp;
}

std::ostream& operator<<(std::ostream& os, const Interval& obj) 
{ 
  os << "["<< obj.get_left_d() << "," << obj.get_right_d() << "]";
  return os;
} 

