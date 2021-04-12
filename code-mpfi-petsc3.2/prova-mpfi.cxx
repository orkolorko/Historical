#include "mpfi.h"
#include "mpfi_io.h"
#include "mpfiautder.h"
//#include "functions-mpfi.h"
#include "interval.h"
#include "functions-interval.h"
#include "functions-mpfi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>

//int bisection(int (*ptrFunc) (mpfi_t,mpfi_t),mpfi_t root, mpfi_t domain, mpfi_t target);

int precision=128;

Interval cube(const Interval &domain)
{
	return power(domain,0.5);
}

Interval cube_der(const Interval &domain)
{
	return 0.5*power(domain,-0.5);
}


Interval dynamic1(const Interval & domain)
{
	mpq_t alpha;
	mpq_init(alpha);
	mpq_set_ui(alpha,8,10);
	mpq_canonicalize(alpha);
	mpq_t theta;
	mpq_init(theta);
	mpq_set_ui(theta,17,10);
	mpq_canonicalize(theta);
	
	Interval temp=domain-0.5;
	
	if (temp.is_neg())
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

Interval dynamic2(const Interval & domain)
{
	Interval Temp=dynamic1(domain);
	Temp=dynamic1(Temp);
	return Temp;
}

Interval dynamic1_der(const Interval & domain)
{
	mpq_t alpha,alphaminusone;
	mpq_init(alpha);
	mpq_set_ui(alpha,8,10);
	mpq_canonicalize(alpha);
	mpq_init(alphaminusone);
	mpq_set_si(alphaminusone,-2,10);
	mpq_canonicalize(alpha);
	
	mpq_t theta;
	mpq_init(theta);
	mpq_set_ui(theta,17,10);
	mpq_canonicalize(theta);
	
	
	Interval temp=domain-0.5;
	//std::cout << temp << std::endl;
	
	if (temp.is_neg())
	{
		temp=-1*temp;
		temp=-1*(theta*(alpha*power(temp,alphaminusone)));
	}
	else
	{
		temp=-1*(theta*(alpha*power(temp,alphaminusone)));
	}
	mpq_clear(alpha);
	mpq_clear(alphaminusone);
	mpq_clear(theta);
	return temp;
}

Interval dynamic2_der(const Interval & domain)
{
	Interval Temp=dynamic1(domain);
	Interval DerTx=dynamic1_der(Temp);
	Interval Derx=dynamic1_der(domain);
	return DerTx*Derx;
}

Interval dynamic4(const Interval & domain)
{
	Interval T2x=dynamic2(domain);
	return dynamic2(T2x);
}	

Interval dynamic4_der(const Interval &domain)
{
	Interval T2x=dynamic2(domain);
	Interval DerT2T2x=dynamic2_der(T2x);
	return DerT2T2x*dynamic2_der(domain);
}


Interval newton(Interval (*ptrFunc) (const Interval &), Interval (*ptrDer)(const Interval &),const Interval & Domain,const Interval & target, const double error)
{
	mp_prec_t precision=Domain.get_precision();
	mpfr_t ScalarMid;
	mpfr_init2(ScalarMid,precision);
	Domain.get_mid(ScalarMid);
	Interval Mid(ScalarMid);
	//std::cout<< "Mid " << Mid << std::endl;
	Interval Nx;
	Nx=Mid-(ptrFunc(Mid)-target)/ptrDer(Domain);
	
	//std::cout<< "Nx " << Nx << std::endl;
	Nx=intersect(Nx,Domain);
	//std::cout<< "Nx intersect domain" << Nx << std::endl;
	
	//std::cout << "f'(Domain) "<< ptrDer(Domain) << std::endl;
	Interval Prova=(ptrDer(Domain)*(Domain-Mid));
	//std::cout << "Domain-Mid " << Domain-Mid << std::endl;
	//std::cout << "f'(Domainx)*Domain-Mid " << Prova << std::endl;
	//std::cout << "f(Mid) " << ptrFunc(Mid) << std::endl;
	
	Interval NNx;
	//std::cout<< Nx << std::endl;
	//std::cin.get();	
	
	while (Nx.get_diam_d()>error)
	{
		Nx.get_mid(ScalarMid);
		Mid=Interval(ScalarMid);
		NNx=Mid-(ptrFunc(Mid)-target)/ptrDer(Nx);
		
		//std::cout << "f'(Nx) "<< ptrDer(Nx) << std::endl;
		Interval Prova=(ptrDer(Nx)*(Domain-Mid));
		//std::cout << "Nx-Mid " << Nx-Mid << std::endl;
		//std::cout << "f'(Nx)*(Domain-Mid) " << Prova << std::endl;
		//std::cout << "f(Mid) " << ptrFunc(Mid) << std::endl;
	
		//std::cout << "Mid " << Mid << std::endl;
		//std::cout<< "f'(Nx) " << ptrDer(Nx) << std::endl; 
		//std::cout<< "f(Mid)-target " << (ptrFunc(Mid)-target) << std::endl;
		//std::cout<< "(f(Mid)-target)/ f'(Nx)" << (ptrFunc(Mid)-target)/ptrDer(Nx) << std::endl;
		//std::cout<< "NNx " << NNx << std::endl;
		//std::cout<< "f(NNx) " << ptrFunc(NNx) << std::endl;
		Nx=intersect(Nx,NNx);
		std::cout<< "Nx " << Nx << std::endl;
		std::cout<< "f(Nx) " << ptrFunc(Nx) << std::endl;
		//std::cout<< Nx.get_diam_d() << std::endl;
		//std::cin.get();	
	
	}
	mpfr_clear(ScalarMid);
	return Nx;
}

int main(int argc, char *argv[])
{
	//double temp(17);
	
	FILE * out;
	out = fopen("prova2.txt", "w");
	
	double relerror=pow(2,-64);
	mpfr_t Left,Right;
	mpq_t righti,zero;
	
	mpq_init(zero);
	mpq_init(righti);
	mpfr_init2 (Left,precision);
	mpfr_init2 (Right,precision);
	
	mpq_set_ui(righti,1,2);
	mpq_set_ui(zero,0,1);
	
	Interval Domain(double(0.086),0.16,mp_prec_t (precision));
	std::cout << "Domain " << Domain << std::endl;
	
	Interval Temp=dynamic4(Domain);
	
	std::cout << "Image " << Temp << std::endl;
	
	std::cout << "Domain " << Domain << std::endl;
	
	Temp=dynamic4_der(Domain);
	
	std::cout << "Derivative " << Temp << std::endl;
	
	
	Interval target(0.3,mp_prec_t (precision));
	
	Interval root=newton(dynamic4,dynamic4_der,Domain,target,0.0000000000001);
	
	std::cout << "root " << root << std::endl;
	
	Interval labile(0.5,0.5,mp_prec_t (precision));
	
	std::cout << "labile " << labile << std::endl;
	
	MPFIAUTDER test(Domain);
	MPFIAUTDER testout;
	
	MPFIAUTDER temp(test);
	
	testout=temp;
	
	std::cout << test.val() << std::endl;
	std::cout << test.derval() << std::endl;
	
	std::cout << testout.val() << std::endl;
	std::cout << testout.derval() << std::endl;
	
	mpfi_t output,intdomain,inttarget;
	mpfi_init2(output,precision);
	mpfi_init2(intdomain,precision);
	mpfi_init2(inttarget,precision);
	
	mpfi_interv_d(intdomain,0.086,0.16);
	mpfi_set_d(inttarget,0.3);
	
	
	dynamic(output,intdomain,precision);
	Interval intout(output);
	std::cout << "Image "<< intout << std::endl;
	dynamic_der(output,intdomain,precision);
	intout=Interval(output);
	std::cout << "Der " << intout << std::endl;
	
	newton(dynamic,dynamic_der,output,intdomain,inttarget,precision,0.0000000000000001);
	intout=Interval(output);
	std::cout << intout << std::endl;
	
	
	mpq_clear(righti);
	mpq_clear(zero);
	mpfr_clear (Left);
	mpfr_clear (Right);
	
	fclose(out);
	return 0;
}
