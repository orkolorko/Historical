#include "mpfi.h"
#include "mpfi_io.h"
#include "functions-mpfi.h"
#include <math.h>
#include <iostream>

double print(mpfi_t I)
{
	mpfr_t left;
	mpfr_t right;
	
	mpfr_init2(left,mpfi_get_prec(I));
	mpfr_init2(right,mpfi_get_prec(I));
	
	mpfi_get_left(left,I);
	mpfi_get_right(right,I);
	std::cout << "[" << mpfr_get_d(left,MPFR_RNDD) <<"," << mpfr_get_d(right,MPFR_RNDU) << "]" << std::endl;
	
	mpfr_clear(left);
	mpfr_clear(right);
	
	return mpfi_get_d(I);
}

int bisection(int (*ptrFunc) (mpfi_t,mpfi_t, int precision),mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror, bool debug)
{
	mpfr_t Diam;
	double DoubleDiam=1;
	mpfi_t y;
	mpfi_t half1,half2,image;
	mpfi_init2 (y,precision);
	mpfi_init2 (half1,precision);
	mpfi_init2 (half2,precision);	
	mpfi_init2 (image,precision);	
	
	mpfr_init2(Diam,precision);
	//mpfi_out_str(out,10,20,domain);
	mpfi_set(root,domain);
	//mpfi_out_str(out,10,20,root);
	
	mpfi_diam_abs(Diam,root);
	DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
	
	ptrFunc(y,root,precision);
	//mpfi_out_str(NULL,10,5,y);
	
	mpfi_sub(image,y,target);
	//mpfi_out_str(out,10,5,image);
	
	if (mpfi_is_inside_ui(0,image))
	{	
		while(DoubleDiam>relerror)
		{
			mpfi_bisect(half1,half2,root);
			
			ptrFunc(y,half1,precision);
			
			mpfi_sub(image,y,target);
			if (debug)
			{
				std::cout << "half1 ";
				print(half1);
				std::cout << "Image half1 ";
				print(image);
			}
			
			if (mpfi_is_inside_ui(0,image))
			{
				mpfi_set(root,half1);
			}
			
			ptrFunc(y,half2,precision);
			mpfi_sub(image,y,target);
			if (debug)
			{
				std::cout << "half2 ";
				print(half2);
				std::cout << "Image half2 ";
				print(image);
			}
			
			if (mpfi_is_inside_ui(0,image))
			{
				mpfi_set(root,half2);
			}
			
			mpfi_diam_abs(Diam,root);
			DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
			if (debug)
			{
				std::cout << DoubleDiam << std::endl;
				std::cin.get();
			}
		}
		mpfr_clear(Diam);
		mpfi_clear (y);
		mpfi_clear (half1);
		mpfi_clear (half2);
		mpfi_clear (image);
		return 0;
	}
	
	mpfr_clear(Diam);
	mpfi_clear (y);
	mpfi_clear (half1);
	mpfi_clear (half2);
	mpfi_clear (image);
	return 1;
}

int newton(int (*ptrFunc) (mpfi_t,mpfi_t, int precision), int (*ptrFuncDer) (mpfi_t,mpfi_t, int precision), mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror,bool debug)
{
	mpfr_t Diam;
	double DoubleDiam=1;
	mpfi_t Image;
	mpfi_t ImageMid,Der;
	
	mpfi_init2 (Image,precision);
	mpfi_init2 (ImageMid,precision);
	mpfi_init2 (Der,precision);	
	mpfr_init2 (Diam,precision);
	
	mpfi_diam_abs(Diam,domain);
	
	DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
	
	ptrFunc(Image,domain,precision);
	//mpfi_out_str(NULL,10,5,y);
	
	mpfi_sub(Image,Image,target);
	//mpfi_out_str(out,10,5,image);
	
	if (mpfi_is_inside_ui(0,Image))
	{	
		mpfi_set(root,domain);
		
		mpfr_t MidFloat;
		mpfi_t MidInterval;
		mpfi_t Nx;
		mpfi_t NNx;
		
		mpfr_init2(MidFloat,precision);
		mpfi_init2(MidInterval,precision);
		mpfi_init2(Nx,precision);
		mpfi_init2(NNx,precision);
		
		while(DoubleDiam>relerror)
		{
			mpfi_mid(MidFloat,root);
			mpfi_set_fr(MidInterval,MidFloat);
			
			ptrFunc(ImageMid,MidInterval,precision);
			if (debug) 
			{
				print(ImageMid);
			} 
			

			ptrFuncDer(Der,root,precision);
			
			if (debug) 
			{
				print(Der);
			} 
			
			mpfi_sub(ImageMid,ImageMid,target);
			if (debug) 
			{
				print(ImageMid);
			} 
			
			mpfi_div(ImageMid,ImageMid,Der);
			
			if (debug) 
			{
				print(ImageMid);
			} 
			
			
			mpfi_sub(Nx,MidInterval,ImageMid);
			
			mpfi_intersect(root,root,Nx);
			
			if (debug) 
			{
				print(root);
				std::cin.get();
			}
			mpfi_diam_abs(Diam,root);
			DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
		}
		mpfi_intersect(root,root,domain);
		
		mpfr_clear (Diam);
		mpfi_clear(Nx);
		mpfi_clear(NNx);
		mpfr_clear (MidFloat);
		mpfi_clear(MidInterval);
		mpfi_clear (Image);
		mpfi_clear (ImageMid);
		mpfi_clear (Der);
		return 0;
	}
	mpfr_clear(Diam);
	mpfi_clear (Image);
	mpfi_clear (ImageMid);
	mpfi_clear (Der);
	return 1;
}

int dynamic_non_quot(mpfi_t y, mpfi_t x,int precision)
{
	mpfi_t labile;
	mpq_t exponent;
	
	mpq_init(exponent);
	mpfi_init2(labile,precision);
	
	mpq_set_ui(exponent,11,10);
	
	mpfi_set(y,x);
	mpfi_log(labile,x);
	mpfi_mul_q(labile,labile,exponent);
	mpfi_exp(labile,labile);
	mpfi_add(y,y,labile);
	
	mpq_clear(exponent);
	mpfi_clear(labile);
	
	return 0;
}

int dynamic(mpfi_t y,mpfi_t x,int precision)
{
	dynamic_non_quot(y,x,precision);
	//print(y);
	int cmp=mpfi_cmp_ui (y,1);
	//std::cout << "cmp "<< cmp << std::endl;
	
	if (cmp>=0)
	{
		//print(y);
		mpfi_sub_ui(y,y,1);
	}
	return 0;
	
}

int dynamic_der(mpfi_t y,mpfi_t x,int precision)
{
	mpfi_t labile;
	mpq_t exponent;
	mpq_t mult;
	
	
	mpq_init(exponent);
	mpq_init(mult);
	
	mpfi_init2(labile,precision);
	
	
	mpq_set_ui(exponent,1,10);
	mpq_set_ui(mult,11,10);
	
	
	/* y=1*/
	mpfi_set_ui(y,1);
	/* labile=x^0.5*/
	mpfi_log(labile,x);
	mpfi_mul_q(labile,labile,exponent);
	mpfi_exp(labile,labile);
	/*labile=1.5*x^0.5*/
	mpfi_mul_q(labile,labile,mult);
	/*y=1+1.5*x^0.5 */
	mpfi_add(y,y,labile);
	
	mpq_clear(exponent);
	mpq_clear(mult);
	
	mpfi_clear(labile);
	
	return 0;
	
}
/*
int dynamic_der_der(mpfi_t y,mpfi_t x,int precision)
{
	return der_der_dynamic4(y,x,precision);
}


int dynamic_der_der_der(mpfi_t y,mpfi_t x,int precision)
{
	return der_der_der_dynamic4(y,x,precision);
}
*/

int SeekDomain(mpfi_t seek, mpfi_t domain, int size, mpfi_t * disc,int NumberDisc, int precision, int side)
{
	mpq_t diff;
	mpq_init(diff);
	mpq_set_ui(diff,1,size);
	
	mpfi_t debug;
	mpfi_init2(debug,precision);
	mpfi_blow(seek,domain,2);
	for (int i=0;i<NumberDisc;i++)
	{
		mpfi_intersect(debug,seek,disc[i]);
		//print(debug);
		if (mpfi_is_empty(debug)==0)
		{	
			/*
			mpfr_t FloatImin;
			mpfr_t discmin;
			mpfr_init2 (FloatImin,precision);
			mpfr_init2 (discmin,precision);
			mpfr_t FloatImax;
			mpfr_t discmax;
			mpfr_init2 (FloatImax,precision);
			mpfr_init2 (discmax,precision);
			
			if(mpfr_cmp(discmin,FloatImin)>0)
			{
				mpfr_sub_q(FloatImin,FloatImin,diff,MPFR_RNDD);
				mpfi_interv_fr(seek,FloatImin,discmin);
			}	
			
			if(mpfr_cmp(FloatImax,discmax)>0)
			{
				mpfr_add_q(FloatImax,FloatImax,diff,MPFR_RNDU);
				mpfi_interv_fr(seek,discmax,FloatImax);
			}
			
			mpfr_clear(FloatImin);
			mpfr_clear(discmin);
			mpfr_clear(FloatImax);
			mpfr_clear(discmax);
			*/
			
			int cmp=mpfi_cmp(domain,disc[i]);
			
			if (cmp<0)
			{
				mpfr_t FloatImin;
				mpfr_t discmin;
				mpfr_init2 (FloatImin,precision);
				mpfr_init2 (discmin,precision);
				mpfi_get_left(FloatImin,domain);
				mpfi_get_left(discmin,disc[i]);
				mpfr_sub_q(FloatImin,FloatImin,diff,MPFR_RNDD);
				mpfi_interv_fr(seek,FloatImin,discmin);
				
				mpfr_clear(FloatImin);
				mpfr_clear(discmin);
			}
			if (cmp>0)
			{
				mpfr_t FloatImax;
				mpfr_t discmax;
				mpfr_init2 (FloatImax,precision);
				mpfr_init2 (discmax,precision);
				mpfi_get_right(FloatImax,domain);
				mpfi_get_right(discmax,disc[i]);
				mpfr_add_q(FloatImax,FloatImax,diff,MPFR_RNDU);
				mpfi_interv_fr(seek,discmax,FloatImax);
				
				mpfr_clear(FloatImax);
				mpfr_clear(discmax);
			}
			if (cmp==0)
			{
				if (side<0)
				{
					mpfr_t FloatImin;
					mpfr_t discmin;
					mpfr_init2 (FloatImin,precision);
					mpfr_init2 (discmin,precision);
					mpfi_get_left(FloatImin,domain);
					mpfi_get_left(discmin,disc[i]);
					mpfr_sub_q(FloatImin,FloatImin,diff,MPFR_RNDD);
					mpfi_interv_fr(seek,FloatImin,discmin);
				
					mpfr_clear(FloatImin);
					mpfr_clear(discmin);
				}
				if (side>0)
				{
					mpfr_t FloatImax;
					mpfr_t discmax;
					mpfr_init2 (FloatImax,precision);
					mpfr_init2 (discmax,precision);
					mpfi_get_right(FloatImax,domain);
					mpfi_get_right(discmax,disc[i]);
					mpfr_add_q(FloatImax,FloatImax,diff,MPFR_RNDU);
					mpfi_interv_fr(seek,discmax,FloatImax);
				
					mpfr_clear(FloatImax);
					mpfr_clear(discmax);
				}
			}
		}
	}
	//print(seek);
	mpq_clear(diff);
	mpfi_clear(debug);
	return 0;
}

double mixing(int i, int j,int size, mpfi_t * disc, int NumberDisc, int precision, int side, double relerror)
{
	
	mpq_t lefti,righti;
	mpq_t leftj,rightj;
	mpq_init (lefti);
	mpq_init (righti);
	mpq_init (leftj);
	mpq_init (rightj);
	
	mpq_set_ui(lefti,i,size);
	mpq_set_ui(righti,i+1,size);
	mpq_set_ui(leftj,j,size);
	mpq_set_ui(rightj,j+1,size);
	
	//std::cout << i << std::endl;
	//std::cout << size << std::endl;
	
	//std::cout << j << std::endl;
	//std::cout << size << std::endl;
	
	
	mpq_canonicalize(lefti);
	mpq_canonicalize(righti);
	mpq_canonicalize(leftj);
	mpq_canonicalize(rightj);
	
	mpfi_t root1;
	mpfi_t root2;
	
	mpfi_t preimage;
	mpfi_t intersection;
	mpfi_t domain;
	mpfi_t seek;
	mpfi_t target;
	mpfi_t debug;
	mpfi_init2 (root1,precision);
	mpfi_init2 (root2,precision);
	
	mpfi_init2 (preimage,precision);
	mpfi_init2 (intersection,precision);
	mpfi_init2 (domain,precision);
	mpfi_init2 (seek,precision);
	mpfi_init2 (target,precision);
	mpfi_init2 (debug,precision);
	
	mpfi_interv_q(domain,lefti,righti);
	
	SeekDomain(seek,domain,size,disc,NumberDisc,precision,side);
	
	
	
	if (side!=0)
	{
		mpfi_set_q(target,leftj);
		if (bisection(dynamic,root1,seek,target,precision,relerror,false))
		{
			mpq_clear(lefti);
			mpq_clear(righti);
			mpq_clear(leftj);
			mpq_clear(rightj);
	
			mpfi_clear (root1);
			mpfi_clear (root2);
			mpfi_clear (preimage);
			mpfi_clear (intersection);
			mpfi_clear (domain);
			mpfi_clear (seek);
			mpfi_clear (target);
			mpfi_clear (debug);
			return 0;
		}
	
		mpfi_set_q(target,rightj);
	
		if (bisection(dynamic,root2,seek,target,precision,relerror,false))
		{
			mpq_clear(lefti);
			mpq_clear(righti);
			mpq_clear(leftj);
			mpq_clear(rightj);
	
			mpfi_clear (root1);
			mpfi_clear (root2);
			mpfi_clear (preimage);
			mpfi_clear (intersection);
			mpfi_clear (domain);
			mpfi_clear (seek);
			mpfi_clear (target);
			mpfi_clear (debug);
			return 0;
		}
	}
	
	if (side==0)
	{
		if (j!=0)
		{
			mpfi_set_q(target,leftj);
			if (newton(dynamic,dynamic_der,root1,seek,target,precision,relerror,false))
			{
				std::cout << "root1 not found" << std::endl;
				print(seek);
				print(target);
			
				mpq_clear(lefti);
				mpq_clear(righti);
				mpq_clear(leftj);
				mpq_clear(rightj);
	
				mpfi_clear (root1);
				mpfi_clear (root2);
				mpfi_clear (preimage);
				mpfi_clear (intersection);
				mpfi_clear (domain);
				mpfi_clear (seek);
				mpfi_clear (target);
				mpfi_clear (debug);
				return 0;
			}
		}
		if (j==0)
		{
			if (i!=0)
			{
				mpfi_set(root1,disc[1]);
			}
			if (i==0)
			{
				mpfi_set_q(target,leftj);
				if (newton(dynamic,dynamic_der,root1,seek,target,precision,relerror,false))
				{
					std::cout << "root1 not found" << std::endl;
					print(seek);
					print(target);
			
					mpq_clear(lefti);
					mpq_clear(righti);
					mpq_clear(leftj);
					mpq_clear(rightj);
	
					mpfi_clear (root1);
					mpfi_clear (root2);
					mpfi_clear (preimage);
					mpfi_clear (intersection);
					mpfi_clear (domain);
					mpfi_clear (seek);
					mpfi_clear (target);
					mpfi_clear (debug);
					return 0;
				}
			}
			
			
		}
	
		mpfi_set_q(target,rightj);
		/* This is a hack to treat the discontinuity of Mann-Pomeau */
		
		if (j+1!=size)
		{
			if (newton(dynamic,dynamic_der,root2,seek,target,precision,relerror,false))
			{
			
				std::cout << "root2 not found" << std::endl;
				print(seek);
				print(target);
			
				mpq_clear(lefti);
				mpq_clear(righti);
				mpq_clear(leftj);
				mpq_clear(rightj);
	
				mpfi_clear (root1);
				mpfi_clear (root2);
				mpfi_clear (preimage);
				mpfi_clear (intersection);
				mpfi_clear (domain);
				mpfi_clear (seek);
				mpfi_clear (target);
				mpfi_clear (debug);
				return 0;
			}
		}
		if (j+1==size)
		{
			if (i+1!=size)
			{
				mpfi_set(root2,disc[1]);
			}
			if (i+1==size)
			{
				if (newton(dynamic,dynamic_der,root2,seek,target,precision,relerror,false))
				{	
			
				std::cout << "root2 not found" << std::endl;
				print(seek);
				print(target);
			
				mpq_clear(lefti);
				mpq_clear(righti);
				mpq_clear(leftj);
				mpq_clear(rightj);
	
				mpfi_clear (root1);
				mpfi_clear (root2);
				mpfi_clear (preimage);
				mpfi_clear (intersection);
				mpfi_clear (domain);
				mpfi_clear (seek);
				mpfi_clear (target);
				mpfi_clear (debug);
				return 0;
				}
			}
		}
	}
	
	//mpfi_set(preimage,root1);
	mpfi_union(preimage,root1,root2);
	
	mpfi_revert_if_needed(preimage);
	if (mpfi_is_empty(preimage)) 
	{
		std::cout << "empty preimage" << std::endl;
		
		mpq_clear(lefti);
		mpq_clear(righti);
		mpq_clear(leftj);
		mpq_clear(rightj);
	
		mpfi_clear (root1);
		mpfi_clear (root2);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (debug);
		mpfi_clear (target);
		return 0;
	}
	
	//std::cout << "root1 " << mpfi_get_d(root1) << std::endl;
	//std::cout << "root3 " << mpfi_get_d(root3) << std::endl;
	
	//std::cout << "root2 " << mpfi_get_d(root2) << std::endl;
	//std::cout << "root4 " << mpfi_get_d(root4) << std::endl;
	
	//std::cin.get();
	
	mpfi_intersect(intersection,preimage,domain);
	
	if (mpfi_is_empty(intersection)) 
	{
		std::cout << "empty intersection" << std::endl;
		mpq_clear(lefti);
		mpq_clear(righti);
		mpq_clear(leftj);
		mpq_clear(rightj);
	
		mpfi_clear (root1);
		mpfi_clear (root2);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (debug);
		mpfi_clear (target);
		return 0;
	}
	
	
	mpfr_t Diam;
	mpfr_init2(Diam,precision);
	
	
	double DoubleDiam;
	mpfi_diam_abs(Diam,intersection);
	DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
	
	mpq_clear(lefti);
	mpq_clear(righti);
	mpq_clear(leftj);
	mpq_clear(rightj);
	
	mpfr_clear (Diam);
	mpfi_clear (root1);
	mpfi_clear (root2);
	mpfi_clear (domain);
	mpfi_clear (seek);
	mpfi_clear (preimage);
	mpfi_clear (intersection);
	mpfi_clear (debug);
	mpfi_clear (target);
	//std::cout << DoubleDiam*size << std::endl;
	
	return DoubleDiam*size;
	
	return 0;
}

double disc_mixing_left(mpfi_t Ii,mpfi_t disc, int i, int size, mpfi_t inc, int precision, double relerror)
{
	
	mpq_t lefti;
	mpq_t righti;
	mpq_t diff;
	mpfr_t FloatImin;
	mpfi_t root1;
	mpfi_t preimage;
	mpfi_t intersection;
	mpfi_t domain;
	mpfi_t seek;
	mpfi_t target;
	
	mpq_init(lefti);
	mpq_init(righti);
	
	mpq_init (diff);
	mpfr_init2 (FloatImin,precision);
	mpfi_init2 (root1,precision);
	mpfi_init2 (preimage,precision);
	mpfi_init2 (intersection,precision);
	mpfi_init2 (domain,precision);
	mpfi_init2 (seek,precision);
	mpfi_init2 (target,precision);
	
	mpq_set_ui(lefti,i,size);
	mpq_set_ui(righti,i+1,size);
	
	mpq_set_ui(diff,1,size);
	mpq_canonicalize(lefti);
	mpfi_set(domain,Ii);
	mpfi_get_left(FloatImin,Ii);
	mpfr_sub_q(FloatImin,FloatImin,diff,MPFR_RNDD);
	mpfi_set_fr(seek,FloatImin);
	mpfi_union(seek,seek,Ii);
	//We use now the bisection method to find the point
	if (mpfi_is_pos(inc))
	{
		mpfi_set_q(target,lefti);
	}
	else
	{
		mpfi_set_q(target,righti);
	}
	
	if (bisection(dynamic,root1,seek,target,precision,relerror,false))
	{
		mpq_clear(lefti);
		mpq_clear(righti);
		mpq_clear(diff);
		mpfr_clear (FloatImin);
		mpfi_clear (root1);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (target);
		//fclose(out);
		return 0;
	}
	
	mpfi_get_left(FloatImin,disc);
	mpfi_set_fr(preimage,FloatImin);
	mpfi_union(preimage,preimage,root1);
	
	mpfi_intersect(intersection,preimage,domain);
	
	if (mpfi_is_empty(intersection)) 
	{
		
		mpq_clear(lefti);
		mpq_clear(righti);
		
		mpq_clear(diff);
		mpfr_clear (FloatImin);
		mpfi_clear (root1);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (target);
		//fclose(out);
		return 0;
	}
	
	mpfr_t Diam;
	mpfr_init2(Diam,precision);
	
	double DoubleDiam;
	mpfi_diam_abs(Diam,intersection);
	DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
	mpfr_clear(Diam);
	
	mpq_clear(lefti);
	mpq_clear(righti);
	mpq_clear(diff);
	mpfr_clear (FloatImin);
	mpfi_clear (root1);
	mpfi_clear (preimage);
	mpfi_clear (intersection);
	mpfi_clear (domain);
	mpfi_clear (seek);
	mpfi_clear (target);
	//fclose(out);
	return DoubleDiam*size;
}

double disc_mixing_right(mpfi_t Ii,mpfi_t disc, int i, int size, mpfi_t inc, int precision, double relerror)
{
	mpq_t lefti;
	mpq_t righti;
	mpq_t diff;
	mpfr_t FloatImax;
	mpfi_t root2;
	mpfi_t preimage;
	mpfi_t intersection;
	mpfi_t domain;
	mpfi_t seek;
	mpfi_t target;
	
	mpq_init(lefti);
	mpq_init(righti);
	mpq_init (diff);
	mpfr_init2 (FloatImax,precision);
	mpfi_init2 (root2,precision);
	mpfi_init2 (preimage,precision);
	mpfi_init2 (intersection,precision);
	mpfi_init2 (domain,precision);
	mpfi_init2 (seek,precision);
	mpfi_init2 (target,precision);
	
	mpq_set_ui(lefti,i,size);
	mpq_set_ui(righti,i+1,size);
	mpq_set_ui(diff,1,size);
	
	mpq_canonicalize(righti);
	mpfi_set(domain,Ii);
	//The interval seek is larger than Ii, only on the left
	mpfi_get_right(FloatImax,Ii);
	mpfr_add_q(FloatImax,FloatImax,diff,MPFR_RNDD);
	mpfi_set_fr(seek,FloatImax);
	mpfi_union(seek,seek,Ii);
	
	//We use now the bisection method to find the point
	
	if (mpfi_is_pos(inc))
	{
		mpfi_set_q(target,righti);
	}
	else
	{
		mpfi_set_q(target,lefti);
	}
	
	if (bisection(dynamic,root2,seek,target,precision,relerror,false))
	{
		mpq_clear(lefti);
		mpq_clear(righti);
		mpq_clear(diff);
		mpfr_clear (FloatImax);
		mpfi_clear (root2);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (target);
		return 0;
	}
	
	mpfi_get_left(FloatImax,disc);
	mpfi_set_fr(preimage,FloatImax);
	mpfi_union(preimage,preimage,root2);
	
	mpfi_intersect(intersection,preimage,domain);
	
	if (mpfi_is_empty(intersection)) 
	{
		
		mpq_clear(lefti);
		mpq_clear(righti);
		mpq_clear(diff);
		mpfr_clear (FloatImax);
		mpfi_clear (root2);
		mpfi_clear (preimage);
		mpfi_clear (intersection);
		mpfi_clear (domain);
		mpfi_clear (seek);
		mpfi_clear (target);
		return 0;
	}
	
	mpfr_t Diam;
	mpfr_init2(Diam,precision);
	
	double DoubleDiam;
	mpfi_diam_abs(Diam,intersection);
	DoubleDiam=mpfr_get_d(Diam,MPFR_RNDU);
	mpfr_clear(Diam);
	
	mpq_clear(lefti);
	mpq_clear(righti);
	mpq_clear(diff);
	mpfr_clear (FloatImax);
	mpfi_clear (root2);
	mpfi_clear (preimage);
	mpfi_clear (intersection);
	mpfi_clear (domain);
	mpfi_clear (seek);
	mpfi_clear (target);
	return DoubleDiam*size;
}
