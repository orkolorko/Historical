#include "mpfi_obj_autder.h"

MPFIAUTDER::MPFIAUTDER()
{
	x=Interval(double(0));
	derx=Interval(double(0));
}

MPFIAUTDER::MPFIAUTDER(mp_prec_t precision)
{
	x=Interval(double(0),mpfr_prec_t(precision));
	derx=Interval(double(0),mpfr_prec_t(precision));
}

MPFIAUTDER::MPFIAUTDER(const Interval a)
{
	x=a;
	derx=Interval(double(1),a.get_precision());
}

MPFIAUTDER::MPFIAUTDER(const Interval a,const Interval b)
{
	x=a;
	derx=b;
}

/*
MPFIAUTDER::~MPFIAUTDER()
{
	mpfi_clear(x);
	mpfi_clear(derx);
}
*/
//Operators

MPFIAUTDER& MPFIAUTDER::operator=(const MPFIAUTDER &a)
{
	this->x=a.x;
	this->derx=a.derx;
	return *this;
}

MPFIAUTDER operator+(const MPFIAUTDER &a, const MPFIAUTDER &b)
{
    MPFIAUTDER temp;
    temp.x=a.x+b.x;
    temp.derx=a.derx+b.derx;
	return temp;
}

MPFIAUTDER operator-(const MPFIAUTDER &a, const MPFIAUTDER &b)
{
	MPFIAUTDER temp;
    temp.x=a.x-b.x;
    temp.derx=a.derx-b.derx;
	return temp;
}

MPFIAUTDER operator*(const MPFIAUTDER &a, const MPFIAUTDER &b)
{
	MPFIAUTDER temp;
    temp.x=a.x*b.x;
    temp.derx=a.derx*b.x+b.derx*a.x;
	return temp;
}

//Division has some important issues we have to deal with...
MPFIAUTDER operator/(const MPFIAUTDER &a, const MPFIAUTDER &b)
{
	MPFIAUTDER temp;

    temp.x=a.x/b.x;
    temp.derx=(a.derx-(temp.x*b.derx))/b.x;
    
    return temp;
}

MPFIAUTDER sin(MPFIAUTDER &a)
{
    MPFIAUTDER temp;
    
    temp.x=sin(a.x);
    temp.derx=a.derx*cos(a.x);
    
	return temp;
}

MPFIAUTDER cos(MPFIAUTDER &a)
{
	MPFIAUTDER temp;
    
    temp.x=cos(a.x);
    temp.derx=-1*(a.derx*sin(a.x));
    
	return temp;
}

MPFIAUTDER log(MPFIAUTDER &a)
{
	MPFIAUTDER temp;
    
    temp.x=log(a.x);
    temp.derx=a.derx/a.x;
    
	return temp;
}

MPFIAUTDER exp(MPFIAUTDER &a)
{
	MPFIAUTDER temp;
    
    temp.x=exp(a.x);
    temp.derx=a.derx*temp.x;
    
	return temp;
}

MPFIAUTDER power(MPFIAUTDER &a,const Interval &alpha)
{
	MPFIAUTDER temp;
    temp.x=power(a.x,alpha);
    temp.derx=a.derx*alpha*power(a.x,alpha-1);
	
	return temp;
}

MPFIAUTDER power(MPFIAUTDER &a,const double &alpha)
{
	MPFIAUTDER temp;
    temp.x=power(a.x,alpha);
    temp.derx=a.derx*alpha*power(a.x,alpha-1);
	
	return temp;
}

MPFIAUTDER power(MPFIAUTDER &a,const mpq_t &alpha)
{
	MPFIAUTDER temp;
    temp.x=power(a.x,alpha);
    temp.derx=a.derx*alpha*power(a.x,alpha-1);
	
	return temp;
}

MPFIAUTDER power(MPFIAUTDER &a,const mpfr_t &alpha)
{
	MPFIAUTDER temp;
    temp.x=power(a.x,alpha);
    temp.derx=a.derx*alpha*power(a.x,alpha-1);
	
	return temp;
}


MPFIAUTDER operator+ (const  MPFIAUTDER &a, const double &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const  MPFIAUTDER &a, const unsigned int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const  MPFIAUTDER &a, const signed int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const  MPFIAUTDER &a, const mpq_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}	

MPFIAUTDER operator+ (const  MPFIAUTDER &a, const mpfr_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}	

MPFIAUTDER operator+ (const double &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const unsigned int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}	

MPFIAUTDER operator+ (const signed int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const mpq_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const mpfr_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp+a;
	return temp;
}	

MPFIAUTDER operator+ (const  MPFIAUTDER &a, const Interval &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=temp+a;
	return temp;
}

MPFIAUTDER operator+ (const Interval &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=temp+a;
	return temp;
}	

MPFIAUTDER operator- (const  MPFIAUTDER &a, const double &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a-temp;
	return temp;
}	

MPFIAUTDER operator- (const  MPFIAUTDER &a, const unsigned int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a-temp;
	return temp;
}

MPFIAUTDER operator- (const  MPFIAUTDER &a, const signed int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a-temp;
	return temp;
}

MPFIAUTDER operator- (const  MPFIAUTDER &a, const mpq_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a-temp;
	return temp;
}

MPFIAUTDER operator- (const  MPFIAUTDER &a, const mpfr_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a-temp;
	return temp;
}
	
MPFIAUTDER operator- (const double &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp-a;
	return temp;
}

MPFIAUTDER operator- (const unsigned int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp-a;
	return temp;
}

MPFIAUTDER operator- (const signed int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp-a;
	return temp;
}

MPFIAUTDER operator- (const mpq_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp-a;
	return temp;
}

MPFIAUTDER operator- (const mpfr_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp-a;
	return temp;
}

MPFIAUTDER operator- (const  MPFIAUTDER &a, const Interval &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=a-temp;
	return temp;
}

MPFIAUTDER operator- (const Interval &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=temp-a;
	return temp;
}


MPFIAUTDER operator* (const  MPFIAUTDER &a, const double &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const  MPFIAUTDER &a, const unsigned int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const  MPFIAUTDER &a, const signed int &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const  MPFIAUTDER &a, const mpq_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const  MPFIAUTDER &a, const mpfr_t &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const unsigned int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const signed int &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const mpq_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const mpfr_t &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp*a;
	return temp;
}

MPFIAUTDER operator* (const  MPFIAUTDER &a, const Interval &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=a*temp;
	return temp;
}

MPFIAUTDER operator* (const Interval &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=temp*a;
	return temp;
}


MPFIAUTDER operator/ (const  MPFIAUTDER &a, const double &b)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const  MPFIAUTDER &a, const unsigned int &b)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const  MPFIAUTDER &a, const signed int &b)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const  MPFIAUTDER &a, const mpq_t &b)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const  MPFIAUTDER &a, const mpfr_t &b)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const double &b, const  MPFIAUTDER &a)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER operator/ (const unsigned int &b, const  MPFIAUTDER &a)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER operator/ (const signed int &b, const  MPFIAUTDER &a)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER operator/ (const mpq_t &b, const  MPFIAUTDER &a)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER operator/ (const mpfr_t &b, const  MPFIAUTDER &a)
{
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(Interval(b,precision),Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER operator/ (const  MPFIAUTDER &a, const Interval &b)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=a/temp;
	return temp;
}

MPFIAUTDER operator/ (const Interval &b, const  MPFIAUTDER &a)
{	
	mp_prec_t precision=(a.val()).get_precision();
	MPFIAUTDER temp(b,Interval(double(0),precision));
	temp=temp/a;
	return temp;
}

MPFIAUTDER dynamicautder(const MPFIAUTDER & domain)
{
	mpq_t alpha;
	mpq_init(alpha);
	mpq_set_ui(alpha,8,10);
	
	mp_prec_t precision=(domain.val()).get_precision();
	
	mpq_t theta;
	mpq_init(theta);
	mpq_set_ui(theta,17,10);
	//mpfi_t x;
	//mpfi_init2(x,precision);
	
	//Interval labile(0.5,0.5,mp_prec_t (precision));
	
	//std::cout << precision <<std::endl;
	
	//MPFIAUTDER half(labile);
	MPFIAUTDER temp(domain);
	temp=domain-0.5;
	
	if ((temp.val()).is_neg())
	{
		temp=-1*temp;
		temp=theta*power(temp,alpha);
	}
	else
	{
		temp=1-theta*power(temp,alpha);
	}
	
	mpq_clear(alpha);
	mpq_clear(theta);
	
	return temp;
}
/*
MPFIAUTDER abs(MPFIAUTDER &a)
{
	mp_prec_t precision=mpfi_get_prec(a.x);
    MPFIAUTDER temp(precision);
    
	mpfi_abs(temp.x,a.x);
	mpfi_div(temp.derx,a.x,temp.x);
	mpfi_mul(temp.derx,temp.derx,a.derx);

	return temp;
}

*/


// Trascendental functions





//Same problem as with division

/*
MPFIAUTDER power(MPFIAUTDER &a,REAL &alpha)
{
	REAL tempx;
	REAL tempderx;
	tempx=power(a.x,alpha);
	tempderx=a.derx*alpha*power(a.x,alpha-1);
	return MPFIAUTDER(tempx,tempderx);
}

MPFIAUTDER abs(MPFIAUTDER &a)
{
	REAL tempx;
	REAL tempderx;
	tempx=abs(a.x);
	tempderx=a.derx*(a.x/abs(a.x));
	return MPFIAUTDER(tempx,tempderx);
}


*/
/*
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
*/

