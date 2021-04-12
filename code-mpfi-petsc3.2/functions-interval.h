#include "interval.h"
/*
int bisection(int (*ptrFunc) (mpfi_t,mpfi_t,int),mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror);
int newton(int (*ptrFunc) (mpfi_t,mpfi_t, int precision), int (*ptrFuncDer) (mpfi_t,mpfi_t, int precision), mpfi_t root, mpfi_t domain, mpfi_t target, int precision, double relerror);
int dynamic1(mpfi_t y,mpfi_t x,int precision);
int der_dynamic1(mpfi_t y,mpfi_t x,int precision);
int dynamic2(mpfi_t y,mpfi_t x,int precision);
int der_dynamic2(mpfi_t y,mpfi_t x,int precision);
int dynamic(mpfi_t y,mpfi_t x,int precision);
int der_dynamic3(mpfi_t y,mpfi_t x,int precision);

int SeekDomain(mpfi_t seek, mpfi_t domain, int size, mpfi_t * disc,int NumberDisc, int precision);
double mixing(int i, int j,int size, mpfi_t * disc, int NumberDisc, int precision, int side, double relerror);
double disc_mixing_left(mpfi_t Ii,mpfi_t disc, int i, int size,mpfi_t inc, int precision, double relerror);
double disc_mixing_right(mpfi_t Ii,mpfi_t disc, int i, int size, mpfi_t inc, int precision, double relerror);
double near_disc_mixing_left(mpfi_t Ii,mpfi_t disc, int i, int size, int precision, double relerror);
*/

//Interval dynamic(const Interval &);
