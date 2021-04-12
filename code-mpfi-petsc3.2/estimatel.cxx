//      genmatrix.cxx
//      
//      Copyright 2012 Isaia Nisoli <isaia@aiace2>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 
#include "functions-mpfi.h"

using namespace std;


int distortion(mpfi_t y,mpfi_t x, int precision)
{
	mpfi_t der_der_x,der_x;
	mpfi_init2 (der_der_x,precision);
	mpfi_init2 (der_x,precision);
	
	dynamic_der(der_x,x,precision);
	dynamic_der_der(der_der_x,x,precision);
	
	mpfi_div(y,der_der_x,der_x);
	mpfi_div(y,y,der_x);
	mpfi_abs(y,y);
	
	mpfi_clear(der_der_x);
	mpfi_clear(der_x);
	
	return 0;
}

int distortion_der(mpfi_t y,mpfi_t x, int precision)
{
	mpfi_t der_der_der_x,der_der_x,der_x,labile;
	mpfi_init2 (der_der_x,precision);
	mpfi_init2 (der_x,precision);
	mpfi_init2 (der_der_der_x,precision);
	mpfi_init2 (labile,precision);
	
	dynamic_der(der_x,x,precision);
	dynamic_der_der(der_der_x,x,precision);
	dynamic_der_der_der(der_der_der_x,x,precision);
	
	mpfi_mul(y,der_der_der_x,der_x);
	mpfi_mul(labile,der_der_x,der_der_x);
	mpfi_mul_si(labile,labile,-2);
	mpfi_add(y,y,labile);
	mpfi_div(y,y,der_x);
	mpfi_div(y,y,der_x);
	mpfi_div(y,y,der_x);
	
	mpfi_clear(der_der_der_x);
	mpfi_clear(der_der_x);
	mpfi_clear(der_x);
	mpfi_clear(labile);
	
	return 0;
}

int absdistortion(mpfi_t y,mpfi_t x, int precision)
{
	mpfi_t der_der_x,der_x;
	mpfi_init2 (der_der_x,precision);
	mpfi_init2 (der_x,precision);
	
	dynamic_der(der_x,x,precision);
	dynamic_der_der(der_der_x,x,precision);
	
	mpfi_div(y,der_der_x,der_x);
	mpfi_div(y,y,der_x);
	mpfi_abs(y,y);
	
	mpfi_clear(der_der_x);
	mpfi_clear(der_x);
	
	return 0;
}

int absdistortionprim(mpfi_t y,mpfi_t x, int precision)
{
	mpfi_t der_der_x,der_x,labile;
	mpfi_init2 (der_der_x,precision);
	mpfi_init2 (der_x,precision);
	mpfi_init2 (labile,precision);
	
	dynamic_der(der_x,x,precision);
	dynamic_der_der(der_der_x,x,precision);
	
	mpfi_div(labile,der_der_x,der_x);
	mpfi_div(labile,labile,der_x);
	//std::cout << "labile "; 
	//print(labile);
	//std::cout << std::endl;
	mpfi_set(y,labile);
	//std::cout << "y ";  
	//print(y);
	//std::cout << std::endl;
	mpfi_abs(labile,labile);
	//std::cout << "labile ";
	//print(labile);
	//std::cout << std::endl;
	mpfi_div(labile,y,labile);
	
	//std::cout << "segno " << print(labile)<< std::endl;
	
	mpfi_div(y,labile,der_x);
	
	mpfi_clear(der_der_x);
	mpfi_clear(der_x);
	mpfi_clear(labile);
	
	return 0;
}


int absdistortion_der(mpfi_t y,mpfi_t x, int precision)
{
	mpfi_t der_der_der_x,der_der_x,der_x,labile;
	mpfi_init2 (der_der_x,precision);
	mpfi_init2 (der_x,precision);
	mpfi_init2 (der_der_der_x,precision);
	mpfi_init2 (labile,precision);
	
	dynamic_der(der_x,x,precision);
	dynamic_der_der(der_der_x,x,precision);
	dynamic_der_der_der(der_der_der_x,x,precision);
	
	mpfi_mul(y,der_der_der_x,der_x);
	mpfi_mul(labile,der_der_x,der_der_x);
	mpfi_mul_si(labile,labile,-2);
	mpfi_add(y,y,labile);
	mpfi_div(y,y,der_x);
	mpfi_div(y,y,der_x);
	mpfi_div(y,y,der_x);
	
	mpfi_clear(der_der_der_x);
	mpfi_clear(der_der_x);
	mpfi_clear(der_x);
	mpfi_clear(labile);
	
	return 0;
}


int OrderDisc(mpfi_t *disc, int NumberDisc)
{
	bool swapped=true;
	while (swapped)
	{
		swapped=false;
		for (int i=0;i<NumberDisc-1;i++)
		{ 
			int cmp=mpfi_cmp(disc[i],disc[i+1]);
			if (cmp>0)
			{
				//std::cout << "swap " << i << " " << i+1 << std::endl;
				swapped=true;
				mpfi_swap(disc[i],disc[i+1]);
			}	
		}
	}
}

int main (int argc, char *argv[])
{		
	int size=1048576;
	int precision=4096;
	clock_t begin, end;
	double time_spent;
	mpfi_t x,y,domain,target;
	int NumberDisc=15;
	mpfi_t disc[NumberDisc];
	mpfi_t DiscOrdered[NumberDisc];
	
	int WhichDisc;
	int side=0;
	bool DiscInside=false;
	double relerror=0.0000000000000000000000000001;
	double Rowmeasure=0;
	mpq_t leftj,rightj,discPnt,perturb;
	mpfr_t FloatImin,FloatImax;
	unsigned int jdisc1min,jdisc1max;
	unsigned int jdisc2min,jdisc2max;
	unsigned int imin=0;
	unsigned int imax=size;
	unsigned int DiscMin=0;
	unsigned int DiscMax=size;
	double max=2;
	double loc=2;
	
	
	
	mpfr_t Left,Right;
	mpfi_t ImLeft;
	mpfi_t ImRight;
	mpfi_t diff;
	mpfi_t labile;
	mpfi_t dx;
	
	begin = clock();
	
	mpfi_init2 (x,precision);
	mpfi_init2 (y,precision);
	for (int l=0;l<NumberDisc;l++)
	{
		mpfi_init2 (disc[l],precision);
	}
	
	mpfi_init2 (domain,precision);
	mpfi_init2 (target,precision);
	mpfi_init2 (ImLeft,precision);
	mpfi_init2 (ImRight,precision);
	mpfi_init2 (diff,precision);
	mpfi_init2 (labile,precision);
	mpfi_init2 (dx,precision);
	
	mpq_init (leftj);
	mpq_init (rightj);
	mpq_init (discPnt);
	mpq_init (perturb);
	mpfr_init2(FloatImin,precision);
	mpfr_init2(FloatImax,precision);
	mpfr_init2(Left,precision);
	mpfr_init2(Right,precision);
	
	mpq_set_ui(discPnt,1,2);
	mpq_set_ui(perturb,1,4*size);
	
	mpfi_set_q(target,discPnt);
	mpfi_set_q(disc[0],discPnt);
	/* Find the discontinuities for the second iterate*/
	/* First discontinuity */
	mpfi_interv_d   (domain, 0, 0.5);
	bisection(dynamic1,disc[1],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[1]) <<std::endl;
	mpfi_mul_ui(y,disc[1],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc1min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc1max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc1min << " " << jdisc1max << std::endl;
	/* Second discontinuity */
	mpfi_interv_d   (domain, 0.5, 1);
	bisection(dynamic1,disc[2],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[2]) <<std::endl;
	mpfi_mul_ui(y,disc[2],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " " << jdisc2max << std::endl;
	
	/* Compute the discontinuities for the third iterate */
	
	mpfr_set_ui(Left,0,MPFR_RNDD);
	mpfi_get_left(Right,disc[1]);
	mpfi_interv_fr(domain,Left,Right);
	bisection(dynamic2,disc[3],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[3]) <<std::endl;
	mpfi_mul_ui(y,disc[3],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " " << jdisc2max << std::endl;
	
	mpfi_get_right(Left,disc[1]);
	mpfi_get_left(Right,disc[0]);
	mpfi_interv_fr(domain,Left,Right);
	bisection(dynamic2,disc[4],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[4]) <<std::endl;
	mpfi_mul_ui(y,disc[4],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 4 " << jdisc2max << std::endl;
	
	mpfi_get_right(Left,disc[0]);
	mpfi_get_left(Right,disc[2]);
	mpfi_interv_fr(domain,Left,Right);
	bisection(dynamic2,disc[5],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[5]) <<std::endl;
	mpfi_mul_ui(y,disc[5],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 5 " << jdisc2max << std::endl;
	
	mpfi_get_right(Left,disc[2]);
	mpfr_set_ui(Right,1,MPFR_RNDU);
	mpfi_interv_fr(domain,Left,Right);
	bisection(dynamic2,disc[6],domain,target,precision,relerror,false);
	std::cout <<mpfi_get_d(disc[6]) <<std::endl;
	mpfi_mul_ui(y,disc[6],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 6 " << jdisc2max << std::endl;
	
	/* Compute the discontinuities for the fourth iterate */
	
	mpfr_set_ui(Left,0,MPFR_RNDD);
	mpfi_get_left(Right,disc[3]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[7],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[7]) <<std::endl;
	mpfi_mul_ui(y,disc[7],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 7 " << jdisc2max << std::endl;
	
	mpfi_ui_div(labile,2,disc[7]);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[3],disc[7]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_get_right(Left,disc[3]);
	mpfi_get_left(Right,disc[1]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[8],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[8]) <<std::endl;
	mpfi_mul_ui(y,disc[8],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 8 " << jdisc2max << std::endl;
	
	mpfi_sub(labile,disc[8],disc[3]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[1],disc[8]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_get_right(Left,disc[1]);
	mpfi_get_left(Right,disc[4]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[9],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[9]) <<std::endl;
	mpfi_mul_ui(y,disc[9],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 9 " << jdisc2max << std::endl;
	
	mpfi_sub(labile,disc[1],disc[9]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[9],disc[4]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_get_right(Left,disc[4]);
	mpfi_get_left(Right,disc[0]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[10],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[10]) <<std::endl;
	mpfi_mul_ui(y,disc[10],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 10 " << jdisc2max << std::endl;
	
	mpfi_sub(labile,disc[4],disc[10]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[10],disc[0]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_get_right(Left,disc[0]);
	mpfi_get_left(Right,disc[5]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[11],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[11]) <<std::endl;
	mpfi_mul_ui(y,disc[11],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 11 " << jdisc2max << std::endl;
	
	mpfi_sub(labile,disc[11],disc[0]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[5],disc[11]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_get_right(Left,disc[5]);
	mpfi_get_left(Right,disc[2]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[12],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[12]) <<std::endl;
	mpfi_mul_ui(y,disc[12],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 12 " << jdisc2max << std::endl;
	
	mpfi_sub(labile,disc[12],disc[5]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	mpfi_sub(labile,disc[2],disc[5]);
	mpfi_abs(labile,labile);
	mpfi_ui_div(labile,2,labile);
	mpfi_get_right(Right,labile);
	loc=mpfr_get_d(Right,MPFR_RNDU);
	if (max<loc) max=loc;
	//std::cout << loc << std::endl;
	
	
	mpfi_get_right(Left,disc[2]);
	mpfi_get_left(Right,disc[6]);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[13],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[13]) <<std::endl;
	mpfi_mul_ui(y,disc[13],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 13 " << jdisc2max << std::endl;

	mpfi_get_right(Left,disc[6]);
	mpfr_set_ui(Right,1,MPFR_RNDU);
	mpfi_interv_fr(domain,Left,Right);
	if (bisection(dynamic3,disc[14],domain,target,precision,relerror,false)) std::cout << "problem" << std::endl;
	std::cout <<mpfi_get_d(disc[14]) <<std::endl;
	mpfi_mul_ui(y,disc[14],size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " 14 " << jdisc2max << std::endl;
	
	
	OrderDisc(disc,NumberDisc);
	
	for (int i=0;i<NumberDisc;i++)
	{
				std::cout << i << " " << mpfi_get_d(disc[i]) << std::endl;
	}
	
	for (int i=0;i<NumberDisc-1;i++)
	{
				mpfi_sub(labile,disc[i],disc[i+1]);
				mpfi_abs(labile,labile);
				mpfi_ui_div(labile,2,labile);
				mpfi_get_right(Right,labile);
				loc=mpfr_get_d(Right,MPFR_RNDU);
				if (max<loc) max=loc;
	}
	
	std::cout << "max 2/(d_i+1-d_i) " << max << std::endl;
	
	mpfi_t prova,derlabile;
	mpfi_init2(prova,precision);
	mpfi_init2(derlabile,precision);
	
	mpfr_t zero,one;
	mpfr_init2(zero,precision);
	mpfr_init2(one,precision);
	mpfi_get_right(Left,disc[6]);
	
	
	mpfr_set_ui(zero,0,MPFR_RNDD);
	mpfr_set_ui(one,1,MPFR_RNDU);
	mpfi_set_ui(prova,0);
	
	mpfi_get_left(Right,disc[0]);
	mpfi_interv_fr(domain,zero,Right);
	
	dynamic_der(derlabile,domain,precision);
	mpfi_abs(derlabile,derlabile);
	mpfi_ui_div(derlabile,1,derlabile);
	print(derlabile);
	
	mpfi_union(prova,prova,derlabile);
	
	for (int i=0;i<NumberDisc-1;i++)
	{
		mpfi_get_right(Left,disc[i]);
		mpfi_get_left(Right,disc[i+1]);
		mpfi_interv_fr(domain,Left,Right);
	
		dynamic_der(derlabile,domain,precision);
		mpfi_abs(derlabile,derlabile);
		mpfi_ui_div(derlabile,1,derlabile);
		print(derlabile);
		mpfi_union(prova,prova,derlabile);
	}
	
	mpfi_get_right(Left,disc[NumberDisc-1]);
	mpfi_interv_fr(domain,Left,one);
	dynamic_der(derlabile,domain,precision);
	mpfi_abs(derlabile,derlabile);
	mpfi_ui_div(derlabile,1,derlabile);
	print(derlabile);
	mpfi_union(prova,prova,derlabile);
	
	std::cout << "1/T' "<< std::endl;
	
	print(prova);
	/*
	std::cout << std::endl;
	
	mpfi_set_si(target,20);
	
	mpq_t left_q,right_q;
	mpfi_t integral,evalprim;
	
	int partition=200001;
	mpq_init(left_q);
	mpq_init(right_q);
	mpfi_init2(integral,precision);
	mpfi_init2(evalprim,precision);
	
	mpfi_set_ui(integral,0);
	
	for (int i=0;i<partition-1;i++)
	{
		if (i%128==0) std::cout << i << std::endl;
		mpq_set_ui(left_q,i,partition);
		mpq_set_ui(right_q,i+1,partition);
		mpfi_interv_q(domain,left_q,right_q);
		absdistortion(prova,domain,precision);
		mpfi_sub(prova,prova,target);
		//print(prova);
		if (mpfi_has_zero(prova))
		{
			std::cout << i << std::endl;
			//print(prova);
			//print(domain);
			absdistortionprim(evalprim,domain,precision);
			print(evalprim);
			mpfi_abs(evalprim,evalprim);
			mpfi_add(integral,integral,evalprim);
			//std::cout << std::endl;
			//std::cin.get();
		}
	}
	std::cout << "integral "; 
	print(integral);
	*/
	
	mpfi_get_left(Right,disc[0]);
	mpfi_interv_fr(domain,zero,Right);
	
	//absdistortion(prova,domain,precision);
	
	mpfi_t half1,half2,dyn,dyn_der,dyn_der_der,dyn_der_der_der;
	mpfr_t Diam;
	mpq_t point;
	
	mpfi_init2 (half1,precision);
	mpfi_init2 (half2,precision);	
	mpfi_init2 (dyn,precision);	
	mpfi_init2 (dyn_der,precision);	
	mpfi_init2 (dyn_der_der,precision);	
	mpfi_init2 (dyn_der_der_der,precision);	
	mpq_init(point);
	
	mpfr_init2 (Diam,precision);
	
	mpfi_set_si(target,0);
	
	
	mpfi_get_right(Left,disc[1]);
	mpfi_get_left(Right,disc[2]);
	
	mpfr_sub_d(Left,Right,0.00001,MPFR_RNDD);
	mpfi_interv_fr(domain,Left,Right);
	
	mpfi_diam_abs(Diam,domain);	
	//mpfi_set_si(target,10);
	
	ofstream out;
	
	out.open("func.txt");
	
	int partition=65536;
	
	for (int i=0;i<partition;i++)
	{
		std::cout << i << std::endl;
		mpq_set_ui(point,i,partition);
		mpfi_set_q(domain,point);
		dynamic(dyn,domain,precision);
		dynamic_der_der(dyn_der_der,domain,precision);
		absdistortion(prova,domain,precision);
		out << mpq_get_d(point) << " " << mpfi_get_d(dyn) << " "<< mpfi_get_d(dyn_der_der) << " " << mpfi_get_d(prova) << std::endl;
	}
	
	out.close();
	
	//mpfi_div_fr(prova,prova,Diam);
	
	print(prova);
	
	dynamic_der_der_der(prova,domain,precision);
	
	print(prova);
	
	
	//mpfi_set_si(target,-10);
	//if (newton(dynamic_der_der,dynamic_der_der_der,prova,domain,target,precision,0.000000000001)) std::cout << "problem" << std::endl;
	
	/*
	for (int i=0;i<NumberDisc-1;i++)
	{
		mpfi_get_right(Left,disc[i]);
		mpfi_get_left(Right,disc[i+1]);
		mpfi_interv_fr(domain,Left,Right);
		
		if (newton(distortion,distortion_der,prova,domain,target,precision,0.0001)) std::cout << "problem" << std::endl;
		print(prova);
		
	}
	
	mpfi_get_right(Left,disc[NumberDisc-1]);
	mpfi_interv_fr(domain,Left,one);
	
	if (newton(distortion,distortion_der,prova,domain,target,precision,0.0001)) std::cout << "problem" << std::endl;
	print(prova);
	*/		
	return 0;
}



