//Standard libraries
#include <iostream>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_of_vector.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/timer.hpp>

using namespace std;
using namespace boost::numeric::ublas;

typedef double APFloat;

typedef boost::numeric::ublas::compressed_matrix<APFloat> SMatrixND;
typedef SMatrixND::const_iterator1 it1;
typedef SMatrixND::const_iterator2 it2;

typedef boost::numeric::ublas::compressed_matrix<unsigned long int> SMatrixInt;
typedef SMatrixInt::const_iterator1 itInt1;
typedef SMatrixInt::const_iterator2 itInt2;

class srmatrix
{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & A;
			ar & error;
		}
		private:
		APFloat error;
		SMatrixND A;
		
		public:
		
		//Constructor
		srmatrix(){SMatrixND B(1,1,0); A=B; error=APFloat(0);};
		srmatrix(const SMatrixND &B,const APFloat &errore){A=B;error=errore;};
		 
		int Skeleton();
		int Markovize();
		
		//Member Function
		
		APFloat GetError(){return error;};

		//Friend functions
		//friend int convertPETSc(srmatrix &, Mat &, PetscInt,PetscInt);
};
