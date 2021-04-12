#include "mpfi.h"
#include "mpfi_io.h"
#include "functions-mpfi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>


int main(int argc, char *argv[])
{
	double temp(17);
	
	FILE * out;
	out = fopen("prova2.txt", "w");
	
	mpfi_t x,y,disc1,disc2,domain,target;
	mpfi_init2 (x,precision);
	mpfi_init2 (y,precision);
	mpfi_init2 (disc1,precision);
	mpfi_init2 (disc2,precision);
	mpfi_init2 (domain,precision);
	mpfi_init2 (target,precision);
	
	
	int size=8096;
	double Rowmeasure=0;
	mpq_t leftj,rightj,disc;
	mpq_init (leftj);
	mpq_init (rightj);
	mpq_init (disc);
	mpq_set_ui(disc,1,2);
	mpfr_t FloatImin,FloatImax;
	mpfr_init2(FloatImin,precision);
	mpfr_init2(FloatImax,precision);
	
	mpfi_set_q(target,disc);
	
	unsigned int jdisc1min,jdisc1max;
	unsigned int jdisc2min,jdisc2max;
	
	mpfi_interv_d   (domain, 0, 0.5);
	bisection(dynamic1,disc1,domain,target);
	std::cout <<mpfi_get_d(disc1) <<std::endl;
	mpfi_mul_ui(y,disc1,size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc1min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc1max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc1min << " " << jdisc1max << std::endl;
	
	mpfi_interv_d   (domain, 0.5, 1);
	bisection(dynamic1,disc2,domain,target);
	std::cout <<mpfi_get_d(disc2) <<std::endl;
	mpfi_mul_ui(y,disc2,size);
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	jdisc2min=mpfr_get_ui(FloatImin,MPFR_RNDD);
	jdisc2max=mpfr_get_ui(FloatImax,MPFR_RNDU);
	std::cout << jdisc2min << " " << jdisc2max << std::endl;
	
	
	clock_t begin, end;
	double time_spent;
	begin = clock();
	unsigned int imin=0;
	unsigned int imax=size;
			
	for (int j=0;j<size;j++)
	{
	/*Set the endpoints of the interval Ij*/	
	mpq_set_ui(leftj,j,size);
	mpq_canonicalize(leftj);
	mpq_set_ui(rightj,j+1,size);
	mpq_canonicalize(rightj);
	
	mpfi_interv_q(x,leftj,rightj);
	dynamic(y,x);
	
	mpfi_mul_ui(y,y,size);
	/* We reduce the number of intervals we study in the image by using the image itself*/
	mpfi_get_left(FloatImin,y);
	mpfi_get_right(FloatImax,y);
	
	imin=mpfr_get_ui(FloatImin,MPFR_RNDD);
	imax=mpfr_get_ui(FloatImax,MPFR_RNDU);
	/*Please note that, if there are discontinuities, the code above gives strange results */
	
	/* Start of the Lorenz specific code in which we study the discontinuities*/
	if (mpfi_is_inside(disc1,x))
	{
		mpfi_set_q(x,leftj);
		mpfi_get_left(FloatImin,disc1);
		mpfi_set_fr(y,FloatImin);
		mpfi_union(x,x,y);
		dynamic(y,x);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		//std::cout << "imin " << mpfr_get_d(FloatImin,MPFR_RNDD) << std::endl;
		//std::cout << "imax " << mpfr_get_d(FloatImax,MPFR_RNDU) << std::endl;
		
		mpfi_mul_ui(y,y,size);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		//std::cout << "imin " << mpfr_get_d(FloatImin,MPFR_RNDD) << std::endl;
		//std::cout << "imax " << mpfr_get_d(FloatImax,MPFR_RNDU) << std::endl;
		
		imin=mpfr_get_ui(FloatImin,MPFR_RNDD);
		imax=mpfr_get_ui(FloatImax,MPFR_RNDU);
		
		//std::cout << "imin " << imin << std::endl;
		//std::cout << "imax " << imax << std::endl;
	
		for (int i=imin;i<imax-1;i++)
		{
			
			//std::cout << "Near the discontinuity" << std::endl;
			double prova=interval_mixing_left(x,i,size);
				
			//std::cout << "j "<< j<< " i "<< i <<" meas "<< prova << std::endl;
			Rowmeasure+=fabs(prova);
				//if (prova!=0) std::cout << i<<" " << prova << std::endl;			
		}
		//std::cout << "j" << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
		//std::cout << "Discontinuity" << std::endl;
		double prova=disc_mixing_left(x,disc1,imax-1,size);
		std::cout << prova << std::endl;
		Rowmeasure+=prova;
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//std::cout << "Left Side" << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
		
		
		mpfi_set_q(x,rightj);
		mpfi_get_right(FloatImax,disc1);
		mpfi_set_fr(y,FloatImax);
		mpfi_union(x,x,y);

		dynamic(y,x);
		mpfi_mul_ui(y,y,size);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		imin=mpfr_get_ui(FloatImin,MPFR_RNDD);
		imax=mpfr_get_ui(FloatImax,MPFR_RNDU);
		//std::cout << "imin " << imin << std::endl;
		//std::cout << "imax " << imax << std::endl;
		
		prova=disc_mixing_right(x,disc1,imin,size);
		std::cout << prova << std::endl;
		Rowmeasure+=prova;
		for (int i=imin+1;i<imax;i++)
		{
			double prova=interval_mixing_right(x,i,size);
			//std::cout << "j "<< j<< " i "<< i <<" meas "<< prova << std::endl;
			Rowmeasure+=fabs(prova);
			//if (prova!=0) std::cout << i<<" " << prova << std::endl;
		}
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//std::cout << "Right Side " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
	}
	if (mpfi_is_inside(disc2,x))
	{
		mpfr_t Diam;
		mpfr_init2(Diam,precision);
		double DiamLeft;
		double DiamRight;
		
		/* We build a new interval, with left endpoint the endpoint of I_j and right endpoint the left endpoint of the interval enclosing the discontinuity*/
		mpfi_set_q(x,leftj);
		mpfi_get_left(FloatImin,disc2);
		mpfi_set_fr(y,FloatImin);
		mpfi_union(x,x,y);
		mpfi_diam_abs(Diam,x);
		DiamLeft=mpfr_get_d(Diam,MPFR_RNDU)*size;
		//std::cout<< "Diameter left side (renormalized)" << DiamLeft << std::endl;
		
		/* We look at the image of this interval*/
		dynamic(y,x);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		//std::cout << "imin " << mpfr_get_d(FloatImin,MPFR_RNDD) << std::endl;
		//std::cout << "imax " << mpfr_get_d(FloatImax,MPFR_RNDU) << std::endl;
		/*We get the interval of elements that we treat*/
		mpfi_mul_ui(y,y,size);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		imin=mpfr_get_ui(FloatImin,MPFR_RNDD);
		imax=mpfr_get_ui(FloatImax,MPFR_RNDU);
		//std::cout << "imin " << imin << std::endl;
		//std::cout << "imax " << imax << std::endl;
		/* We treat the points near the discontinuity */
		//std::cout << "Near the discontinuity" << std::endl;
		double prova=0;
		for (int i=imin-1;i<imax-1;i++)
		{
			prova=interval_mixing_left(x,i,size);	
			//std::cout << "j "<< j<< " i "<< i <<" meas "<< prova << std::endl;
			Rowmeasure+=fabs(prova);
		}
		
		//std::cout << "j " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
		prova=disc_mixing_left(x,disc2,imax-1,size);
		//std::cout << "Discontinuity " << prova << std::endl;
		Rowmeasure+=prova;
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//std::cout << "Left Side " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
		
		mpfi_set_q(x,rightj);
		mpfi_get_right(FloatImax,disc2);
		mpfi_set_fr(y,FloatImax);
		mpfi_union(x,x,y);
		
		mpfi_diam_abs(Diam,x);
		DiamRight=mpfr_get_d(Diam,MPFR_RNDU)*size;
		//std::cout<< "Diameter right side (renormalized)" << DiamRight  << std::endl;
		//std::cout<< "Sum of diameters" << DiamLeft+DiamRight << std::endl;
		
		dynamic(y,x);
		mpfi_mul_ui(y,y,size);
		mpfi_get_left(FloatImin,y);
		mpfi_get_right(FloatImax,y);
		imin=mpfr_get_ui(FloatImin,MPFR_RNDD);
		imax=mpfr_get_ui(FloatImax,MPFR_RNDU);
		//std::cout << "imin " << imin << std::endl;
		//std::cout << "imax " << imax << std::endl;
		
		prova=disc_mixing_right(x,disc2,imin,size);
		std::cout << prova << std::endl;
		Rowmeasure+=prova;
		for (int i=imin+1;i<=imax;i++)
		{
			double prova=interval_mixing_right(x,i,size);
			//std::cout << "j "<< j<< " i "<< i <<" meas "<< prova << std::endl;
			Rowmeasure+=fabs(prova);
			//if (prova!=0) std::cout << i<<" " << prova << std::endl;
		}
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//std::cout << "Right Side " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
	}	
	/* End of the Lorenz specific code*/
	
	/* here, we treat "generic" intervals */
	else
	{
		double prova;
		for (int i=imin;i<imax;i++)
		{
			if ((j>=jdisc1min-1) && (j<=jdisc1max+1))
			{
				
				mpfi_interv_q(x,leftj,rightj);
				if (j<jdisc1min) 
				{
					prova=near_disc_mixing_left(x,disc1,i,size);
					//std::cout << "Neat the discontinuity disc_mixing_left " << j << std::endl;
				}
				if (j>jdisc1max)
				{ 
					prova=near_disc_mixing_right(x,disc1,i,size);
					//std::cout << "Neat yhe discontinuity " << j << std::endl;
				}
				Rowmeasure+=fabs(prova);
				//std::cout << "j " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
			}
			else if ((j>=jdisc2min-1) && (j<=jdisc2max+1))
			{
				mpfi_interv_q(x,leftj,rightj);
				if (j<jdisc2min)
				{
					prova=near_disc_mixing_left(x,disc2,i,size);
					//std::cout << "Neat the discontinuity disc_mixing_left " << j << std::endl;
				}
				if (j>jdisc2max)
				{
					prova=near_disc_mixing_right(x,disc2,i,size);
					//std::cout << "Neat yhe discontinuity " << j << std::endl;
				}
				Rowmeasure+=fabs(prova);
			}
			else
			{
				prova=mixing(j,i,size,true,disc1,disc2);
				Rowmeasure+=fabs(prova);
				//if (prova!=0) std::cout << i<<" " << prova << std::endl;
			}
		}
		if ((fabs(1-Rowmeasure)>0.0000000001))
		{
			end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			std::cout << "j " << j <<" Rowmeasure " << Rowmeasure << " time " << time_spent << std::endl;
		}
		Rowmeasure=0;
		}
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	std::cout <<  " time " << time_spent << std::endl;
		
	mpq_clear(leftj);
	mpq_clear(rightj);
	mpfi_clear (x);
	mpfi_clear (y);
	mpfi_clear (disc1);
	mpfi_clear (disc2);
	mpfi_clear (domain);
	mpfi_clear (target);
	mpfr_clear(FloatImin);
	mpfr_clear(FloatImax);
	
	fclose(out);
	return 0;
}
