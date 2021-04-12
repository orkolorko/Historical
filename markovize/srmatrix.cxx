#include "linalg.h"

// This routine makes a graph of the nonzero entries of the Markov chain
// it fills a file with two columns, on one are there the row coordinates
// of the non-zeros, on the second column are there the col coordinates of
// the non-zeros. Plotting it we get an idea of the sparsity pattern of the 
// matrix. Please note that this gives also an idea of the graph of the Markov chain.
// Trivially, since we are aproximating a function by a Markov chain, this 
// pattern resembles also the graph of the function.
int srmatrix::Skeleton()
{
		std::ofstream myfile;
		myfile.open("Skeleton.txt");
		for (it1 i1 = A.begin1(); i1 != A.end1(); ++i1) 
				{
						for (it2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							myfile << i2.index2() << " " << i2.index1() << endl;
						}	
				}
		myfile.close();
		cout << "Ho calcolato Skeleton"	<< endl;
		return 0;
}

// This routine assures that the matrix is a Markov matrix
int srmatrix::Markovize()
{
	cout << "We  'Markovize' the matrix" << endl;
	
	// This variable holds the maximum of the difference between the Row Sums and 1
	// in some way it gives us an idea of how far is our matrix from being a Markov matrix
	// and euristically tells us if it is better to request a smaller error when we are computing
	// the coefficients of the matrix
	 
	bool Markov=true;
	APFloat truncerror=0;
	cout << "Error before markovization " << this->error << endl;
	
	
	for (it1 i1 = A.begin1(); i1 != A.end1(); ++i1) 
				{
						//Computes the sum of each row
						int nnzrow=0;
						double Rowsum=APFloat(0);
						
						for (it2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							Rowsum+=*i2;
							nnzrow++;
						}
						if (Rowsum<=0) cout << "There is a big problem" << endl;
				}		
	
	SMatrixInt B(A.size1(),A.size2());
	for (it1 i1 = A.begin1(); i1 != A.end1(); ++i1) 
				{
						for (it2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							B.push_back(i2.index1(),i2.index2(),(unsigned long int)(1125899906842624*(*i2)));
						}																		   
				}
	
	SMatrixInt C(A.size1(),A.size2());
	for (itInt1 i1 = B.begin1(); i1 != B.end1(); ++i1) 
				{
						unsigned long int intdiff=1125899906842624;
						int nnz=0;
						for (itInt2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							nnz++;
							intdiff-=*i2;
						}
						
						APFloat loctruncerr=double(intdiff)/1125899906842624;
						
						if (loctruncerr>=truncerror) truncerror=loctruncerr; 
						
						int spread=intdiff/nnz;
						bool restoflag=true;
						int resto=intdiff%nnz;
						for (itInt2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							if (restoflag==true)
							{
								C.push_back(i2.index1(),i2.index2(),*i2+spread+resto);
								restoflag=false;
							}
							else
							{
								C.push_back(i2.index1(),i2.index2(),*i2+spread);
							}
						}
				}
		SMatrixND D(A.size1(),A.size2());
				for (itInt1 i1 = C.begin1(); i1 != C.end1(); ++i1)
				{
					for (itInt2 i2 = i1.begin(); i2 != i1.end(); ++i2)
					{
						D.push_back(i2.index1(),i2.index2(),double(*i2)/1125899906842624);
					}
				}
		A=D;
		for (it1 i1 = A.begin1(); i1 != A.end1(); ++i1) 
				{
						//Computes the sum of each row
						double Rowsum=APFloat(0);
						
						for (it2 i2 = i1.begin(); i2 != i1.end(); ++i2)
						{
							Rowsum+=*i2;
						}
						//cout << "Alla linea " << i1.index1() << "rowsum vale " << Rowsum << endl;  
						if (Rowsum!=1) 
						{
							cout << "Not Markov on line" << i1.index1() << endl;
							Markov=false;
						}
				}
		if (Markov==true) cout << "Yippie! It is Markov!" << endl;
		if (Markov==false) cout << "Gosh! It is not Markov!" << endl;
		
		if (truncerror>=error) error=truncerror;
		error=3*error;
		cout << "Error after markovization " << error << endl;
		return 0;
}
