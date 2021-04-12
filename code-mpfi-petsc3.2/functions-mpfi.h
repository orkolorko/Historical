#include "mpfi.h"
#include "mpfi_io.h"

double print(mpfi_t );
int bisection(int (*ptrFunc) (mpfi_t,mpfi_t,int),mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror,bool debug);
int newton(int (*ptrFunc) (mpfi_t,mpfi_t, int precision), int (*ptrFuncDer) (mpfi_t,mpfi_t, int precision), mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror,bool debug);

int dynamic(mpfi_t y,mpfi_t x,int precision);
int dynamic_der(mpfi_t y,mpfi_t x,int precision);
int dynamic_non_quot(mpfi_t y,mpfi_t x,int precision);

int SeekDomain(mpfi_t seek, mpfi_t domain, int size, mpfi_t * disc,int NumberDisc, int precision);
double mixing(int i, int j,int size, mpfi_t * disc, int NumberDisc, int precision, int side, double relerror);
double disc_mixing_left(mpfi_t Ii,mpfi_t disc, int i, int size,mpfi_t inc, int precision, double relerror);
double disc_mixing_right(mpfi_t Ii,mpfi_t disc, int i, int size, mpfi_t inc, int precision, double relerror);
double near_disc_mixing_left(mpfi_t Ii,mpfi_t disc, int i, int size, int precision, double relerror);
