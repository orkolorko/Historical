#include "actualgen.h"
#include "dynamic.h"

using namespace std;

PetscInt genmatrix(PetscInt size, PetscScalar step, Mat * const Aaddr, PetscScalar * const MatErrorAddr)
{
	PetscScalar * out;
	out=new PetscScalar[2];
	PetscInt *input;
	input=new PetscInt[2];
	PetscInt target[2];
	PetscInt supder[2];
	PetscInt supderv;
	PetscInt targetv;
	PetscInt intreport=64;
	
	PetscErrorCode        ierr;
	bool debug=false;
	
	input[1]=size;
	PetscInt partsize=size;
	for (PetscInt i=0;i<size;i++)
	{
		for (int k=0;k<2;k++)
		{
			target[k]=0;
			supder[k]=size;
		}
		
		//The code below is specific to the Lorenz 1-dim map
		
		input[0]=i;
		if (input[0]!=size/2)
		{
		iRRAM_exec(dynamicestimate,input,out);
		target[0]=PetscInt(out[0]*size);
		supder[0]=2*PetscInt(out[1])+1;
		}
		else
		{
			target[0]=0;
		}
		input[0]=i+1;
		if (input[0]!=size/2)
		{
		iRRAM_exec(dynamicestimate,input,out);
		target[1]=PetscInt(out[0]*size);
		supder[1]=2*PetscInt(out[1])+1;
		}
		else
		{
			target[1]=size;
		}
		//end of the specific code
		
		supderv=supder[1];
		if (supder[0]>supder[1]) supderv=supder[0];
		PetscScalar rowmeasure=0;
		PetscScalar rowerror=0;
		bool report=false;
		if (supderv>100) 
		{
		//report=true;
		std::cout << "Qua c'è tanto da calcolare " << supderv << " " << i << endl;
		}	
			//if (abs(target[0]-target[1])<supderv)
			{
			targetv=target[0];
			unsigned long int unit=partsize;
			PetscInt jmin=targetv-supderv;
			PetscInt jmax=targetv+supderv;
			PetscScalar length=1/double(unit);
			if (jmin<0) jmin=0;
			if (jmax>size) jmax=size;
			
			
			// The code below is specific for the Lorenz 1-dim map
			
			if (i+1==size/2)
			{
				PetscScalar temp=iRRAM_exec(DoubleDynamic,i,size);
				jmin=PetscInt(temp*size)-10;
				jmax=size;
				std::cout <<"jmin e jmax "<< jmin << " " << jmax << endl;
				supderv=2*(jmax-jmin);
				std::cout <<"supderv " << supderv <<endl;
				std::cin.get();
				report=true;
			}
			
			if (i==size/2)
			{
				PetscScalar temp=iRRAM_exec(DoubleDynamic,i+1,size);
				jmax=PetscInt(temp*size)+10;
				jmin=0;
				std::cout <<"jmin e jmax "<< jmin << " " << jmax << endl;
				supderv=2*(jmax-jmin);
				std::cout <<"supderv " << supderv << endl;
				std::cin.get();
				report=true;
			}
			//debug=true;
			//end of the specific code
			
			if (i%intreport==0) std::cout<<"i "<< i <<" target "<< targetv <<" supder "<< supderv <<" jmin "<< jmin <<" jmax "<< jmax<<endl;
			for (PetscInt j=jmin;j<jmax;j++)
			{
				if (debug) std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
				
				if (report)
				{
					
				if (j%1==0) std::cout<<"j " << j<< endl;
				debug=false;
				}
				PetscScalar measure=0;
				PetscScalar locerror=0;
				
				// Here there is a problem, even if many intervals are senti
				ierr=iRRAM_exec(mixing,inputmixing(i,j,supderv,unit,partsize,step,debug),outputmixing(&measure,&locerror));CHKERRQ(ierr);
				measure/=length;
				//std::cout << "La misura è " << measure<< endl;
				locerror/=length;
				//std::cout << "Locerror " << locerror << endl;
				
				rowmeasure+=measure;
				rowerror+=locerror;
				
				ierr=MatSetValue(*Aaddr,j,i,measure,INSERT_VALUES);CHKERRQ(ierr);
				if (locerror>=*MatErrorAddr) *MatErrorAddr=locerror;
				
				if (rowmeasure>=1) j=size;
				if (debug)
				{
				std::cout<< "Coefficient " << i <<" "<< j <<" measure " << measure << endl;
				std::cout<< "Row " << i <<" Rowmeasure " << rowmeasure << " Rowerror " << rowerror << endl;
				if (debug) std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
				}
				
			}
			//std::cout << "************************************************************************************************" << endl;
			//std::cout<< "Sum (should be as near as 1 as possible!)" << rowerror+rowmeasure << endl;
			//std::cin.get();
			}
			/*
			if (abs(target[0]-target[1])>supderv)
			{
				for (int l=0;l<2;l++)
				{
				targetv=target[l];
				supderv=supder[l];
				unsigned long int unit=partsize;
				PetscInt jmin=targetv-supderv;
				PetscInt jmax=targetv+supderv;
				PetscScalar length=1/double(unit);
				if (jmin<0) jmin=0;
				if (jmax>size) jmax=size;
				//std::cout<<"i "<< i <<" target "<< target <<" supder "<< supder <<" jmin "<< jmin <<" jmax "<< jmax<<endl;
				for (PetscInt j=jmin;j<jmax;j++)
					{
						//std::cout<<"j " << j<< endl;
						PetscScalar measure=0;
						PetscScalar locerror=0;
						ierr=iRRAM_exec(mixing,inputmixing(i,j,supderv,unit,partsize,step,debug),outputmixing(&measure,&locerror));CHKERRQ(ierr);
						measure/=length;
						rowmeasure+=measure;
						locerror/=length;
						rowerror+=locerror;
				
						ierr=MatSetValue(*Aaddr,j,i,measure,INSERT_VALUES);CHKERRQ(ierr);
						//std::cout<<"Fine j"<< endl;
						//std::cin.get();
						if (rowmeasure>=1) j=size;
					}
				
				}
			}
				*/
		if ((i%intreport==0) || (report)) std::cout<< "Row " << i <<" Rowmeasure " << rowmeasure << " Rowerror " << rowerror << endl;
	}
	delete input;
	delete out;
	return 0;
}



PetscErrorCode mixing(inputmixing a,outputmixing b)
{
	RATIONAL imin(a.i,a.partsize);
	RATIONAL imax(a.i+1,a.partsize); 
	
	RATIONAL jmin(a.j,a.partsize);
	RATIONAL jmax(a.j+1,a.partsize);
	
	RATIONAL root1;
	RATIONAL root2;
	
	
	INTEGER supder(a.supder);
	
	INTEGER partsize(a.partsize);
	INTEGER unit(a.unit);
	bool debug=a.debug;
	
	unit=unit*supder;
	
	if (debug)
	{
		iRRAM::cout << "imin " << imin << " immagine " << dynamic(imin) << endl;
		iRRAM::cout << "imax " << imax << " immagine " << dynamic(imax) <<endl;
		iRRAM::cout << "jmin " << double(REAL(jmin)) << endl;
		iRRAM::cout << "jmax " << double(REAL(jmax)) << endl;
			
		iRRAM::cout << "************************************************************"<< endl;
		iRRAM::cout << "supder " << supder << endl;
		iRRAM::cout<<"Unit è "<< unit << endl;
	}
	RATIONAL unitinverse(INTEGER(1),unit);
	REAL step(a.step);
	
	REAL measureloc(0);
	REAL errorloc(0);
	int contcount=0;
	int k=-300; //this is an hack, find a way to connect this to the required error on the coefficients of the matrix	
	
	
	//Qua basta solo un int, forse!
	for (INTEGER i=0;i<supder;i+=1)
	{
		PetscInt test=0;
		
		//Please note that the code below is specific to the Lorentz 1-dim
		RATIONAL lend=imin+i*unitinverse;
		RATIONAL rend=imin+(i+1)*unitinverse;
		
		REAL immagine1(0);
		REAL immagine2(1);	
		
		IntervalDynamic(lend,rend,&immagine1,&immagine2);
		if (debug)
		{
			iRRAM::cout << "Lend" << lend << endl;
			iRRAM::cout << "Rend" << rend << endl;
			
			iRRAM::cout << "Immagine1 " << immagine1 << endl;
			iRRAM::cout << "Immagine2 " <<immagine2 << endl;
		}	
		
		
		if ( (bound(REAL(jmin)-immagine1,k)) || (bound(REAL(jmax)-immagine1,k)))
		{
			iRRAM::cout << "L'inizio dell'intervallo coincide" << endl;
			test++;
			
			if (debug)
			{
				iRRAM::cout << jmin << " " << immagine1 << endl;
				iRRAM::cout << jmax << " " << immagine1 << endl;
				iRRAM::cout << "test " << test;
			
			}
			errorloc+=power(2,k);
		}
		else
		{
			if ( (REAL(jmin)<immagine1) && (immagine1<REAL(jmax))) test++;
		}

		if ( (bound(REAL(jmin)-immagine2,k)) || (bound(REAL(jmax)-immagine2,k)) )
		{
			iRRAM::cout << "La fine dell'intervallo coincide" << endl;
			test++;
			if (debug)
			{
				iRRAM::cout << jmin << " " << immagine2 << endl;
				iRRAM::cout << jmax << " " << immagine2 << endl;
				iRRAM::cout << "test " << test;
				
			}
			
			errorloc+=power(2,k);
		}
		else
		{
			if ((REAL(jmin)<immagine2) && (immagine2<REAL(jmax))) test++;
		}
		
		//iRRAM::cout << "test " << test << endl;
		if (test==2) 
		{
			measureloc+=unitinverse;
			contcount++;
			if (debug)
			{
			iRRAM::cout << "!!!l'intervallo è contenuto" << endl;
			}
		}
		//Controllare per bene questa doppia condizione, la seconda è un LAZYBOOLEAN
		if ((test==1) && (double(REAL(unitinverse)*REAL(a.partsize))>=double(step)))
		{	
			if (debug)
			{
				iRRAM::cout << "Incertezza e itero" << endl;
				
			}
			REALmixing(REALinputmixing(imin+i*unitinverse,imax+i*unitinverse,jmin,jmax,supder,unit,partsize,step,debug),REALoutputmixing(&measureloc,&errorloc,&root1,&root2));  
			if (debug)
			{
			iRRAM::cout << "Sono tornato!" << endl;
			iRRAM::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
			iRRAM::cout<<"Unit è "<< unit << endl;
			
			}
		}
		if ((test==1) && (double(REAL(unitinverse)*REAL(a.partsize))<double(step))) 
		{
			if (debug)
			{
			iRRAM::cout << "Incertezza e sommo all'errore" << endl;
			
			iRRAM::cout<<"Unit è "<< unit << endl;
			
			}
			errorloc+=unitinverse;
			//std::cin.get();
		}
		if ((debug) && (test==0)) iRRAM::cout <<"Non contenuto" << endl;
		
	}
	if (debug)
	{
		
		iRRAM::cout<<"Salgo un livello"<< endl;
		iRRAM::cout<<"contcount "<< contcount << endl;
		iRRAM::cout<<"percentuale contenuto " << double(contcount)/double(supder)*100 << endl;
	}
	if (debug)
	{

			iRRAM::cout << "Rowmeasure-?"<< REAL(root1-imin)*REAL(partsize) << endl;
			
			iRRAM::cout << "Rowmeasure+?"<<REAL(root2-imin)*REAL(partsize) << endl;
			
	}
			
	
	*b.measureAddr=double(measureloc);
	*b.locerrorAddr=double(errorloc);
	return 0;
}

PetscErrorCode REALmixing(REALinputmixing a,REALoutputmixing b)
{
	//std::cout << "in REALmixing" << endl;
	INTEGER unit(a.unit);
	unit=unit*a.supder;
	//iRRAM::cout<<"Unit è "<< unit << endl;
	RATIONAL unitinverse(INTEGER(1),unit);
	REAL step(a.step);
	
	int contcount=0;
	
	int k=-1000; //this is an hack, find a way to connect this to the required error on the coefficients of the matrix
	
	//Qua basta solo un int, forse!
	for (int i=0;i<a.supder;i++)
	{
		PetscInt test=0;
		
		
		//Please note that the code below is specific to the Lorentz 1-dim
		RATIONAL lend=a.imin+i*unitinverse;
		RATIONAL rend=a.imin+(i+1)*unitinverse;
		
		REAL immagine1=0;
		REAL immagine2=1;	
		
		IntervalDynamic(lend,rend,&immagine1,&immagine2);
		/*
		if (a.debug)
		{
		iRRAM::cout << "jmin " << a.jmin << " immagine1 " << immagine1<< " jmax " << a.jmax << endl;
		iRRAM::cout << "jmin " << a.jmin << " immagine2 " << immagine2<< " jmax " << a.jmax << endl;
		std::cin.get();
		}
		*/
		//the following code checks if the images are on the endpoint of the target interval 
		//up to an error, if so, adds this error to the error in the computation of the coefficient
		
		bool leftcoincide=false;
		bool rightcoincide=false;
		bool leftcontained=false;
		bool rightcontained=false;
		
		if ((bound(REAL(a.jmin)-immagine1,k))|| (bound(REAL(a.jmax)-immagine1,k)))
		{
			iRRAM::cout << "Uno degli estremi coincide" << endl;
			
			iRRAM::cout << "Estremi/Immagine" <<endl;
			iRRAM::cout << REAL(a.jmin) << " " << immagine1 << endl;
			iRRAM::cout << REAL(a.jmax) << " " << immagine1 << endl;
			
			*b.locerrorAddr+=power(2,k);
			leftcoincide=true;	
		}
		else 
		{
			if ((REAL(a.jmin)<immagine1) && (immagine1<REAL(a.jmax))) leftcontained=true;
		}
		
		if ((bound(REAL(a.jmin)-immagine2,k)) || (bound(REAL(a.jmax)-immagine2,k)))
		{
			iRRAM::cout << "La fine dell'intervallo coincide" << endl;
			iRRAM::cout << "Estremi/Immagine" <<endl;
			iRRAM::cout << REAL(a.jmin) << " " << immagine2 << endl;
			iRRAM::cout << REAL(a.jmax) << " " << immagine2 << endl;
			
			*b.locerrorAddr+=power(2,k);
			rightcoincide=true;
		}
		else
		{
			if ((REAL(a.jmin)<immagine2) && (immagine2<REAL(a.jmax))) rightcontained=true;
		}
		
		
		if (leftcoincide) test=1;
		if (rightcoincide) test=1;
		if (leftcontained) test=1;
		if (rightcontained) test=1;
		
		if ((leftcoincide) && (rightcoincide)) test=2;
		if ((leftcoincide) && (rightcontained)) test=2;		
		if ((leftcontained) && (rightcoincide)) test=2;		
		if ((leftcontained) && (rightcontained)) test=2;
		/*
		if (a.debug)
		{
		std::cout << "leftcoincide " << leftcoincide << endl;
		std::cout << "rightcoincide " << rightcoincide << endl;
		std::cout << "leftcontained " << leftcontained << endl;
		std::cout << "rightcontained " << rightcontained << endl;
		std::cout << "test " << test << endl;		
		}
		*/
		
		
		
		if (test==2) 
		{
			contcount++;
			*b.measureAddr+=unitinverse;
			//iRRAM::cout << "l'intervallo è contenuto" << endl;
			//if (a.debug) std::cin.get();
		}
		//Controllare per bene questa doppia condizione, la seconda è un LAZYBOOLEAN
		if ((test==1) && (double(REAL(unitinverse)*REAL(a.partsize))>=double(step)))
		{	
			if (a.debug) iRRAM::cout << "Incertezza e itero" << endl;
			REALmixing(REALinputmixing(a.imin+i*unitinverse,a.imax+i*unitinverse,a.jmin,a.jmax,a.supder,unit,a.partsize,step,a.debug),REALoutputmixing(b.measureAddr,b.locerrorAddr,b.root1,b.root2));  
			//iRRAM::cout << "Sono tornato!" << endl;
			//iRRAM::cout<<"Unit è "<< unit << endl;
	
			//if (a.debug) std::cin.get();
			
		}
		if ((test==1) && (double(REAL(unitinverse)*REAL(a.partsize))<double(step))) 
		{
			if (a.debug) iRRAM::cout << "Incertezza e sommo all'errore" << endl;
			if (a.debug)
			{
				*b.root1=lend;
				*b.root2=rend;
			}
			//iRRAM::cout << step << endl;
			*b.locerrorAddr+=unitinverse;
			//if (a.debug) std::cin.get();
		}
		
		//iRRAM::cout << "Measure " << *b.measureAddr <<endl;
		//iRRAM::cout << "Locerror " << *b.locerrorAddr << endl;
	
	}
	//iRRAM::cout << "contcount "<< contcount << endl;
	
	//iRRAM::cout << "Salgo un livello"<< endl;
	return 0;
}

REAL inverse(RATIONAL lend,RATIONAL rend,RATIONAL target)
{
	REAL realstart((lend+rend)/2);
	REALAUTDER startpoint(realstart,0);
	REALAUTDER value=dynamicautder(startpoint);
	realstart=realstart-(value.val()/value.derval());
	for (int i=0;i<200;i++)
	{
		REALAUTDER startpoint(realstart,0);
		REALAUTDER value=dynamicautder(startpoint);
		realstart=realstart-(value.val()/value.derval());
	}
	return realstart;
}
