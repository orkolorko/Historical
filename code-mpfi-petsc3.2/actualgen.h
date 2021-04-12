#include "petscmat.h"

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> 

PetscInt genmatrix(PetscInt size, PetscScalar step, Mat * const Aaddr, PetscScalar * const MatErrorAddr);
