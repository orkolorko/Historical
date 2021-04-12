#include "mpfi_obj.h"

using namespace mpfi_obj_namespace;

typedef mpfi_obj (*ptrfunc) (const mpfi_obj &);


void bisect(ptrfunc f, const mpfi_obj & X,const mpfi_obj & target,double tol)
{
	
	mpfr_t Diam;
	mpfr_init(Diam);
	
	if (haszero(f(X)-target))
	{
		X.Diam_Abs(Diam);
		double diam=mpfr_get_d(Diam,MPFR_RNDU);
		/*
		std::cout << X << std::endl;
		std::cout << f(X)-target << std::endl;
		std::cout << diam << std::endl;
		std::cin.get();
		*/
		if (diam<tol)
		{
			std::cout<<X<<std::endl;
		}
		else
		{
			bisect(f,X.half1(),target,tol);
			bisect(f,X.half2(),target,tol);
		}
	}
}

void bisect(ptrfunc f, const mpfi_obj & X, const mpfi_obj target,double tol, mpfi_obj & root)
{
	
	mpfr_t Diam;
	mpfr_init(Diam);
	
	if (haszero(f(X)-target))
	{
		X.Diam_Abs(Diam);
		double diam=mpfr_get_d(Diam,MPFR_RNDU);
		/*
		std::cout << X << std::endl;
		std::cout << f(X)-target << std::endl;
		std::cout << diam << std::endl;
		std::cin.get();
		*/
		if (diam<tol)
		{
			root=X;
		}
		else
		{
			bisect(f,X.half1(),target,tol);
			bisect(f,X.half2(),target,tol);
		}
	}
}

mpfi_obj pow(const mpfi_obj base,const mpfi_obj exponent)
{
	mpfi_obj y;
	y.set_prec(16*y.get_default_prec());
	y=log(base);
	
	y=exponent*y;
	return exp(y);
}

mpfi_obj dynamic(const mpfi_obj &x)
{
	mpfi_obj y;
	mpfi_obj labile(x);
	
	mpq_t alpha_q,theta_q,half_q;
	mpq_init(alpha_q);
	mpq_init(theta_q);
	mpq_init(half_q);
	
	mpq_set_ui(alpha_q,8,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(theta_q,17,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(half_q,1,2);
	mpq_canonicalize(half_q);
	
	mpfi_obj alpha(alpha_q);
	mpfi_obj theta(theta_q);
	mpfi_obj half(half_q);
	
	labile-=half;
	
	if (isneg(labile))
	{
		y=theta*pow(abs(labile),alpha);
	}
	if (ispos(labile))
	{
		y=1-theta*pow(abs(labile),alpha);
	}
	return y;
}

mpfi_obj dynamic_der(const mpfi_obj &x)
{
	mpfi_obj y;
	mpfi_obj labile(x);
	
	mpq_t alpha_q,theta_q,half_q;
	mpq_init(alpha_q);
	mpq_init(theta_q);
	mpq_init(half_q);
	
	mpq_set_ui(alpha_q,8,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(theta_q,17,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(half_q,1,2);
	mpq_canonicalize(half_q);
	
	mpfi_obj alpha(alpha_q);
	mpfi_obj theta(theta_q);
	mpfi_obj half(half_q);
	
	labile-=half;
	
	if (isneg(labile))
	{
		y=-theta*alpha*pow(abs(labile),alpha-1);
	}
	if (ispos(labile))
	{
		y=-theta*alpha*pow(abs(labile),alpha-1);
	}
	return y;
}

mpfi_obj dynamic_der_der(const mpfi_obj &x)
{
	mpfi_obj y;
	mpfi_obj labile(x);
	
	mpq_t alpha_q,theta_q,half_q;
	mpq_init(alpha_q);
	mpq_init(theta_q);
	mpq_init(half_q);
	
	mpq_set_ui(alpha_q,8,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(theta_q,17,10);
	mpq_canonicalize(alpha_q);
	
	mpq_set_ui(half_q,1,2);
	mpq_canonicalize(half_q);
	
	mpfi_obj alpha(alpha_q);
	mpfi_obj theta(theta_q);
	mpfi_obj half(half_q);
	
	labile-=half;
	
	if (isneg(labile))
	{
		y=theta*alpha*(alpha-1)*pow(abs(labile),alpha-2);
	}
	if (ispos(labile))
	{
		y=-theta*alpha*(alpha-1)*pow(abs(labile),alpha-2);
	}
	return y;
}

mpfi_obj dynamic2(const mpfi_obj &x)
{
	mpfi_obj y;
	y=dynamic(x);
	y=dynamic(y);
	return y;
}

int main()
{
	mpfr_set_default_prec(1024);
	
	mpq_t half_q;
	mpq_init(half_q);
	mpq_set_ui(half_q,1,2);
	mpq_canonicalize(half_q);
	mpfi_obj half(half_q);
	
	std::cout << "The default precision is " << mpfr_get_default_prec() << std::endl;
	
	mpfi_obj prova(double(0),0.5);
	std::cout << prova <<std::endl;
	std::cout << dynamic(prova) <<std::endl;
	std::cout << dynamic_der(prova) <<std::endl;
	std::cout << dynamic_der_der(prova) <<std::endl;
	std::cout << prova.half1() << std::endl;
	std::cout << prova.half2() << std::endl;
	bisect(dynamic,prova,half,0.000000001);
	
	prova=mpfi_obj(0.5,double(1));
	std::cout << prova <<std::endl;	
	std::cout << dynamic(prova) <<std::endl;
	std::cout << dynamic_der(prova) <<std::endl;
	std::cout << dynamic_der_der(prova) <<std::endl;
	bisect(dynamic,prova,half,0.000000001);
	
	return 0;
}
