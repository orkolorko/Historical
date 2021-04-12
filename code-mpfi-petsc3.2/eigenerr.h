
//Standard libraries
#include <iostream>
#include <fstream>
//PETSc
#include "petscmat.h" 
#include "petscksp.h"
using namespace std;
typedef PetscScalar APFloat;

int DecayTime(Mat const * const,APFloat,APFloat,PetscInt,PetscInt,int * const,int * const,int * const); 

int rigerror(APFloat, APFloat, APFloat, APFloat, PetscInt, const int, const int, APFloat * const);
