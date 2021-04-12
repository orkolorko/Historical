#include "mpfi.h"

#ifndef INTERVAL_H
#define INTERVAL_H



class Interval
{
	mpfi_t x;
	bool leftexact;
	bool rightexact;
	
	public:
	Interval();
	Interval(const mp_prec_t);
	Interval(const mpfi_t);
	Interval(const double);
	Interval(const double,const mp_prec_t);
	Interval(double,double);
	Interval(double,double,const mp_prec_t);
	Interval(const mpq_t);
	Interval(const mpq_t,const mp_prec_t);
	Interval(const mpq_t,const mpq_t);
	Interval(const mpq_t,const mpq_t,const mp_prec_t);
	Interval(const mpfr_t);
	Interval(const mpfr_t,const mp_prec_t);
	Interval(const mpfr_t,const mpfr_t);
	Interval(const mpfr_t,const mpfr_t, const mp_prec_t);
	~Interval();
	
	Interval& operator=(const Interval& );
	
	int is_pos(){return mpfi_is_pos(x);};
	int is_neg(){return mpfi_is_neg(x);};
	
	int get_left(mpfr_t & Left) const;
	int get_right(mpfr_t & Right) const;
	int get_mid(mpfr_t & Mid) const;
	
	int get_interval(mpfi_t &) const;
	
	mp_prec_t get_precision() const { return mpfi_get_prec(x);};
	 
	double get_left_d() const;
	double get_right_d() const;
	double get_diam_d() const;
	int bisect(Interval &,Interval &);
	
	friend Interval intersect(const Interval &a,const Interval &b);
	
	friend Interval operator+ (const  Interval &a, const Interval &b);
	friend Interval operator+ (const  Interval &a, const double &b);
	friend Interval operator+ (const  Interval &a, const unsigned int &b);
	friend Interval operator+ (const  Interval &a, const signed int &b);
	friend Interval operator+ (const  Interval &a, const mpq_t &b);
	friend Interval operator+ (const  Interval &a, const mpfr_t &b);
	
	friend Interval operator+ (const double &b, const  Interval &a);
	friend Interval operator+ (const unsigned int &b, const  Interval &a);
	friend Interval operator+ (const signed int &b, const  Interval &a);
	friend Interval operator+ (const mpq_t &b, const  Interval &a);
	friend Interval operator+ (const mpfr_t &b, const  Interval &a);
	
	friend Interval operator- (const  Interval &a, const Interval &b);
	friend Interval operator- (const  Interval &a, const double &b);
	friend Interval operator- (const  Interval &a, const unsigned int &b);
	friend Interval operator- (const  Interval &a, const signed int &b);
	friend Interval operator- (const  Interval &a, const mpq_t &b);
	friend Interval operator- (const  Interval &a, const mpfr_t &b);
	
	friend Interval operator- (const double &b, const  Interval &a);
	friend Interval operator- (const unsigned int &b, const  Interval &a);
	friend Interval operator- (const signed int &b, const  Interval &a);
	friend Interval operator- (const mpq_t &b, const  Interval &a);
	friend Interval operator- (const mpfr_t &b, const  Interval &a);
	
	friend Interval operator* (const  Interval &a, const Interval &b);
	friend Interval operator* (const  Interval &a, const double &b);
	friend Interval operator* (const  Interval &a, const unsigned int &b);
	friend Interval operator* (const  Interval &a, const signed int &b);
	friend Interval operator* (const  Interval &a, const mpq_t &b);
	friend Interval operator* (const  Interval &a, const mpfr_t &b);
	
	friend Interval operator* (const double &b, const  Interval &a);
	friend Interval operator* (const unsigned int &b, const  Interval &a);
	friend Interval operator* (const signed int &b, const  Interval &a);
	friend Interval operator* (const mpq_t &b, const  Interval &a);
	friend Interval operator* (const mpfr_t &b, const  Interval &a);
	
	friend Interval operator/ (const  Interval &a, const Interval &b);
	friend Interval operator/ (const  Interval &a, const double &b);
	friend Interval operator/ (const  Interval &a, const unsigned int &b);
	friend Interval operator/ (const  Interval &a, const signed int &b);
	friend Interval operator/ (const  Interval &a, const mpq_t &b);
	friend Interval operator/ (const  Interval &a, const mpfr_t &b);
	
	friend Interval operator/ (const double &b, const  Interval &a);
	friend Interval operator/ (const unsigned int &b, const  Interval &a);
	friend Interval operator/ (const signed int &b, const  Interval &a);
	friend Interval operator/ (const mpq_t &b, const  Interval &a);
	friend Interval operator/ (const mpfr_t &b, const  Interval &a);
	
	friend Interval sin(const Interval &);
    friend Interval cos(const Interval &);
    friend Interval exp(const Interval &);
    friend Interval log(const Interval &);
    friend Interval power(const Interval &,const Interval &);
    friend Interval power(const Interval &,const double &);
    friend Interval power(const Interval &,const mpq_t &);
    friend Interval power(const Interval &,const mpfr_t &);
    
    friend Interval abs(const Interval &);
};

std::ostream& operator<<(std::ostream&, const Interval&);





#endif
