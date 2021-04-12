/*
    Multi-precision interval class for C++. 
    Based on MPFI library:    http://mpfr.org

    Project homepage:    http://www.holoborodko.com/pavel/mpfr
    Contact e-mail:      pavel@holoborodko.com

    Copyright (c) 2012 Isaia Nisoli
    
    ****************************************************************************
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    ****************************************************************************
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    
    3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cfloat>
#include <cmath>
#include <limits>

#define OBJECTIVEMPFI_HAVE_MSVC_DEBUGVIEW              // Enable Debugger Visualizer for "Debug" builds in MSVC.

#if defined(OBJECTIVEMPFI_HAVE_MSVC_DEBUGVIEW) && defined(_MSC_VER) && defined(_DEBUG)
#define OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE         DebugView = toString();
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_DATA     std::string DebugView;
#else
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE 
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_DATA 
#endif


#include "mpfi.h"



namespace mpfi_obj_namespace
{
	class mpfi_obj {
private:
    mpfi_t mp;
    
public:
    
    // Get default rounding mode & precision
    inline static mp_prec_t  get_default_prec()   {    return mpfr_get_default_prec();                            };
	double get_d ()								  {	   return mpfi_get_d(mp);									};
    
	// Constructors && type conversions
    mpfi_obj();
    mpfi_obj(const mpfi_obj& u);
    mpfi_obj(const mpfi_t u);
    mpfi_obj(const mpfr_t u);    
    mpfi_obj(const mpz_t u,             mp_prec_t prec = mpfi_obj::get_default_prec());    
    mpfi_obj(const mpq_t u,             mp_prec_t prec = mpfi_obj::get_default_prec());    
    mpfi_obj(const double u,            mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const unsigned long int u, mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const unsigned int u,      mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const long int u,          mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const int u,               mp_prec_t prec = mpfi_obj::get_default_prec());
	mpfi_obj(const char* s,             mp_prec_t prec = mpfi_obj::get_default_prec(), int base = 10);
    mpfi_obj(const std::string& s,      mp_prec_t prec = mpfi_obj::get_default_prec(), int base = 10);
	
	// Constructors taking two arguments
	
	mpfi_obj(const mpfr_t u,const mpfr_t v);    
    mpfi_obj(const mpz_t u,const mpz_t v,mp_prec_t prec = mpfi_obj::get_default_prec());    
    mpfi_obj(const mpq_t u,const mpq_t v,mp_prec_t prec = mpfi_obj::get_default_prec());    
    mpfi_obj(const double u,const double v,mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const unsigned long int u,const unsigned long int v,mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const unsigned int u,const unsigned int v,mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const long int u,const long int v,mp_prec_t prec = mpfi_obj::get_default_prec());
    mpfi_obj(const int u,const int v,mp_prec_t prec = mpfi_obj::get_default_prec());
	
    ~mpfi_obj();    
	
	// Bisection members
	mpfi_obj half1(mp_prec_t prec = mpfi_obj::get_default_prec()) const;
    mpfi_obj half2(mp_prec_t prec = mpfi_obj::get_default_prec()) const;
	// Mid and access to the endpoints
	/*
	mpfr_t mid(mp_prec_t prec = mpfi_obj::get_default_prec());
	mpfr_t left(mp_prec_t prec = mpfi_obj::get_default_prec());
	mpfr_t right(mp_prec_t prec = mpfi_obj::get_default_prec());
	*/
	inline int mid(mpfr_t mid) const;
	inline int left(mpfr_t left) const ;
	inline int right(mpfr_t right) const;
	inline int Diam_Abs(mpfr_t Diam_Abs) const {mpfi_diam_abs(Diam_Abs,mp);};
	
	// Operations
    // =
    // +, -, *, /, ++, --, <<, >> 
    // *=, +=, -=, /=,
    // <, >, ==, <=, >=

	// =
    mpfi_obj& operator=(const mpfi_obj& v);
    mpfi_obj& operator=(const mpfr_t v);
    mpfi_obj& operator=(const mpz_t v);
    mpfi_obj& operator=(const mpq_t v);
    mpfi_obj& operator=(const long double v);
    mpfi_obj& operator=(const double v);        
    mpfi_obj& operator=(const unsigned long int v);
    mpfi_obj& operator=(const unsigned int v);
    mpfi_obj& operator=(const long int v);
    mpfi_obj& operator=(const int v);
    mpfi_obj& operator=(const char* s);
    mpfi_obj& operator=(const std::string& s);
	
	// +
    mpfi_obj& operator+=(const mpfi_obj& v);
    mpfi_obj& operator+=(const mpfr_t v);
    mpfi_obj& operator+=(const mpz_t v);
    mpfi_obj& operator+=(const mpq_t v);
    mpfi_obj& operator+=(const long double u);
    mpfi_obj& operator+=(const double u);
    mpfi_obj& operator+=(const unsigned long int u);
    mpfi_obj& operator+=(const unsigned int u);
    mpfi_obj& operator+=(const long int u);
    mpfi_obj& operator+=(const int u);
    
    const mpfi_obj operator+() const;
    mpfi_obj& operator++ ();
    const mpfi_obj  operator++ (int); 
    
    // -
    mpfi_obj& operator-=(const mpfi_obj& v);
    mpfi_obj& operator-=(const mpfr_t v); //To Do
    mpfi_obj& operator-=(const mpz_t v);
    mpfi_obj& operator-=(const mpq_t v);
    mpfi_obj& operator-=(const long double u);
    mpfi_obj& operator-=(const double u);
    mpfi_obj& operator-=(const unsigned long int u);
    mpfi_obj& operator-=(const unsigned int u);
    mpfi_obj& operator-=(const long int u);
    mpfi_obj& operator-=(const int u);
    const mpfi_obj operator-() const;
    friend const mpfi_obj operator-(const mpfr_t b, const mpfi_obj& a); //To Do
    friend const mpfi_obj operator-(const unsigned long int b, const mpfi_obj& a);
    friend const mpfi_obj operator-(const unsigned int b, const mpfi_obj& a);
    friend const mpfi_obj operator-(const long int b, const mpfi_obj& a);
    friend const mpfi_obj operator-(const int b, const mpfi_obj& a);
    friend const mpfi_obj operator-(const double b, const mpfi_obj& a);
    mpfi_obj& operator-- ();    
    const mpfi_obj  operator-- (int);

	// *
    mpfi_obj& operator*=(const mpfi_obj& v);
    mpfi_obj& operator*=(const mpfr_t v); //To Do
    mpfi_obj& operator*=(const mpz_t v);
    mpfi_obj& operator*=(const mpq_t v);
    mpfi_obj& operator*=(const long double v);
    mpfi_obj& operator*=(const double v);
    mpfi_obj& operator*=(const unsigned long int v);
    mpfi_obj& operator*=(const unsigned int v);
    mpfi_obj& operator*=(const long int v);
    mpfi_obj& operator*=(const int v);
    
    // /
    mpfi_obj& operator/=(const mpfi_obj& v);
    mpfi_obj& operator/=(const mpfr_t v); //To Do
    mpfi_obj& operator/=(const mpz_t v);
    mpfi_obj& operator/=(const mpq_t v);
    mpfi_obj& operator/=(const long double v);
    mpfi_obj& operator/=(const double v);
    mpfi_obj& operator/=(const unsigned long int v);
    mpfi_obj& operator/=(const unsigned int v);
    mpfi_obj& operator/=(const long int v);
    mpfi_obj& operator/=(const int v);
    friend const mpfi_obj operator/(const mpfr_t b, const mpfi_obj& a); //To Do
    friend const mpfi_obj operator/(const unsigned long int b, const mpfi_obj& a);
    friend const mpfi_obj operator/(const unsigned int b, const mpfi_obj& a);
    friend const mpfi_obj operator/(const long int b, const mpfi_obj& a);
    friend const mpfi_obj operator/(const int b, const mpfi_obj& a);
    friend const mpfi_obj operator/(const double b, const mpfi_obj& a);
	
	// Output/ Input
    friend std::ostream& operator<<(std::ostream& os, const mpfi_obj& v);
	
	// Math Functions
    friend const mpfi_obj sqr (const mpfi_obj& v);
    friend const mpfi_obj sqrt(const mpfi_obj& v);
    friend const mpfi_obj sqrt(const unsigned long int v);
    friend const mpfi_obj cbrt(const mpfi_obj& v);
    friend const mpfi_obj root(const mpfi_obj& v);
    //friend const mpfi_obj pow (const mpfi_obj& a);
    //friend const mpfi_obj pow (const mpfi_obj& a, const unsigned long int b);
    //friend const mpfi_obj pow (const mpfi_obj& a, const long int b);
    //friend const mpfi_obj pow (const unsigned long int a, const mpfi_obj& b);
    //friend const mpfi_obj pow (const unsigned long int a, const unsigned long int b);
    friend const mpfi_obj fabs(const mpfi_obj& v);

    friend const mpfi_obj abs(const mpfi_obj& v);
    //friend const mpfi_obj dim(const mpfi_obj& a, const mpfi_obj& b);
    friend inline const mpfi_obj mul_2ui(const mpfi_obj& v, unsigned long int k);
    friend inline const mpfi_obj mul_2si(const mpfi_obj& v, long int k);
    friend inline const mpfi_obj div_2ui(const mpfi_obj& v, unsigned long int k);
    friend inline const mpfi_obj div_2si(const mpfi_obj& v, long int k);
    //friend int cmpabs(const mpfi_obj& a,const mpfi_obj& b);
    
    friend const mpfi_obj log  (const mpfi_obj& v);
    friend const mpfi_obj log2 (const mpfi_obj& v);
    friend const mpfi_obj log10(const mpfi_obj& v);
    friend const mpfi_obj exp  (const mpfi_obj& v); 
    friend const mpfi_obj exp2 (const mpfi_obj& v);
    //friend const mpfi_obj exp10(const mpfi_obj& v);
    friend const mpfi_obj log1p(const mpfi_obj& v);
    friend const mpfi_obj expm1(const mpfi_obj& v);
    
	// Get raw pointers so that mpfi_obj can correctly be used in raw mpfr_* functions
    ::mpfi_ptr mpfi_ptr();
    ::mpfi_srcptr mpfi_srcptr() const;

	// Set/Get instance properties
    inline mp_prec_t    get_prec() const;
    inline void         set_prec(mp_prec_t prec);    // Change precision with rounding mode
    
    // Aliases for get_prec(), set_prec() - needed for compatibility with std::complex<mpfi_obj> interface
    inline mpfi_obj&      setPrecision(int Precision);
    inline int          getPrecision() const;
    
    // Set mpfi_obj to +/- inf, NaN, +/-0
    /*
    mpfi_obj&        setInf  (int Sign = +1);    
    mpfi_obj&        setNan  ();
    mpfi_obj&        setZero (int Sign = +1);
    */
    std::string        toString() const;
    //std::string        toString(int n = 0, int b = 10, mp_rnd_t mode = mpfi_obj::get_default_rnd()) const;

	// Boolean Operators
    friend bool operator >  (const mpfi_obj& a, const mpfi_obj& b);
    friend bool operator >= (const mpfi_obj& a, const mpfi_obj& b);
    friend bool operator <  (const mpfi_obj& a, const mpfi_obj& b);
    friend bool operator <= (const mpfi_obj& a, const mpfi_obj& b);
    friend bool operator == (const mpfi_obj& a, const mpfi_obj& b);
    friend bool operator != (const mpfi_obj& a, const mpfi_obj& b);
    
    // Optimized specializations for boolean operators
    friend bool operator == (const mpfi_obj& a, const unsigned long int b);
    friend bool operator == (const mpfi_obj& a, const unsigned int b);
    friend bool operator == (const mpfi_obj& a, const long int b);
    friend bool operator == (const mpfi_obj& a, const int b);
    
    friend bool operator == (const mpfi_obj& a, const double b);
    
    // Instance Checkers
    friend bool isnan    (const mpfi_obj& v);
    friend bool isinf    (const mpfi_obj& v);
    friend bool isbounded (const mpfi_obj& v);

    friend bool haszero   (const mpfi_obj& v);
    
    friend bool ispos    (const mpfi_obj& v);
    friend bool isnonneg    (const mpfi_obj& v);
    friend bool isneg    (const mpfi_obj& v);
    friend bool isnonpos    (const mpfi_obj& v);
   
    
    
};

//////////////////////////////////////////////////////////////////////////
// sqrt
const mpfi_obj sqrt(const unsigned int v);
const mpfi_obj sqrt(const long int v);
const mpfi_obj sqrt(const int v);
//const mpfi_obj sqrt(const long double v);
const mpfi_obj sqrt(const double v);

//////////////////////////////////////////////////////////////////////////
// pow
/*
const mpfi_obj pow(const mpfi_obj& a, const unsigned int b);
const mpfi_obj pow(const mpfi_obj& a, const int b);
//const mpfi_obj pow(const mpfi_obj& a, const long double b);
const mpfi_obj pow(const mpfi_obj& a, const double b);

const mpfi_obj pow(const unsigned int a, const mpfi_obj& b);
const mpfi_obj pow(const long int a, const mpfi_obj& b);
const mpfi_obj pow(const int a, const mpfi_obj& b);
//const mpfi_obj pow(const long double a, const mpfi_obj& b);
const mpfi_obj pow(const double a, const mpfi_obj& b);

const mpfi_obj pow(const unsigned long int a, const unsigned int b);
const mpfi_obj pow(const unsigned long int a, const long int b);
const mpfi_obj pow(const unsigned long int a, const int b);
//const mpfi_obj pow(const unsigned long int a, const long double b);
const mpfi_obj pow(const unsigned long int a, const double b);

const mpfi_obj pow(const unsigned int a, const unsigned long int b);
const mpfi_obj pow(const unsigned int a, const unsigned int b);
const mpfi_obj pow(const unsigned int a, const long int b);
const mpfi_obj pow(const unsigned int a, const int b);
//const mpfi_obj pow(const unsigned int a, const long double b);
const mpfi_obj pow(const unsigned int a, const double b);

const mpfi_obj pow(const long int a, const unsigned long int b);
const mpfi_obj pow(const long int a, const unsigned int b);
const mpfi_obj pow(const long int a, const long int b);
const mpfi_obj pow(const long int a, const int b);
//const mpfi_obj pow(const long int a, const long double b);
const mpfi_obj pow(const long int a, const double b);

const mpfi_obj pow(const int a, const unsigned long int b);
const mpfi_obj pow(const int a, const unsigned int b);
const mpfi_obj pow(const int a, const long int b);
const mpfi_obj pow(const int a, const int b); 
//const mpfi_obj pow(const int a, const long double b);
const mpfi_obj pow(const int a, const double b); 

const mpfi_obj pow(const long double a, const long double b);    
const mpfi_obj pow(const long double a, const unsigned long int b);
const mpfi_obj pow(const long double a, const unsigned int b);
const mpfi_obj pow(const long double a, const long int b);
const mpfi_obj pow(const long double a, const int b);

const mpfi_obj pow(const double a, const double b);    
const mpfi_obj pow(const double a, const unsigned long int b);
const mpfi_obj pow(const double a, const unsigned int b);
const mpfi_obj pow(const double a, const long int b);
const mpfi_obj pow(const double a, const int b);
*/

//////////////////////////////////////////////////////////////////////////
// Constructors & converters
// Default constructor: creates mp number and initializes it to 0.
inline mpfi_obj::mpfi_obj() 
{ 
    mpfi_init2(mp,mpfi_obj::get_default_prec()); 
    mpfi_set_ui(mp,0);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpfi_obj& u) 
{
    mpfi_init2(mp,mpfi_get_prec(u.mp));
    mpfi_set(mp,u.mp);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpfr_t u)
{
    mpfi_init2(mp,mpfr_get_prec(u));
    mpfi_set_fr(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpfi_t u)
{
    mpfi_init2(mp,mpfi_get_prec(u));
    mpfi_set(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpz_t u, mp_prec_t prec)
{
    mpfi_init2(mp,prec);
    mpfi_set_z(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpq_t u, mp_prec_t prec)
{
    mpfi_init2(mp,prec);
    mpfi_set_q(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const double u, mp_prec_t prec)
{
     mpfi_init2(mp, prec);
/*
#if (OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW > -1)
	if(fits_in_bits(u, OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW))
	{
		mpfr_set_d(mp, u, mode);
	}else
		throw conversion_overflow();
#else
*/
	mpfi_set_d(mp, u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const unsigned long int u, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_set_ui(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const unsigned int u, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_set_ui(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const long int u, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_set_si(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const int u, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_set_si(mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const char* s, mp_prec_t prec, int base)
{
    mpfi_init2(mp, prec);
    mpfi_set_str(mp, s, base); 

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}
inline mpfi_obj::mpfi_obj(const std::string& s, mp_prec_t prec, int base)
{
    mpfi_init2(mp, prec);
    mpfi_set_str(mp, s.c_str(), base); 

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

//Constructors with two arguments
inline mpfi_obj::mpfi_obj(const mpfr_t u,const mpfr_t v)
{
	int u_prec=mpfr_get_prec(u);
    int v_prec=mpfr_get_prec(v);
    if (v_prec>u_prec) u_prec=v_prec;
    mpfi_init2(mp,u_prec);
    mpfi_interv_fr(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpz_t u,const mpz_t v, mp_prec_t prec)
{
    mpfi_init2(mp,prec);
    mpfi_interv_z(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const mpq_t u,const mpq_t v, mp_prec_t prec)
{
    mpfi_init2(mp,prec);
    mpfi_interv_q(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const double u, const double v,mp_prec_t prec)
{
     mpfi_init2(mp, prec);
/*
#if (OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW > -1)
	if(fits_in_bits(u, OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW))
	{
		mpfr_set_d(mp, u, mode);
	}else
		throw conversion_overflow();
#else
*/
	mpfi_interv_d(mp, u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const unsigned long int u, const unsigned long int v, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_interv_ui(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const unsigned int u, const unsigned int v, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_interv_ui(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const long int u,const long int v, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_interv_si(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::mpfi_obj(const int u, const int v, mp_prec_t prec)
{ 
    mpfi_init2(mp,prec);
    mpfi_interv_si(mp,u,v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}

inline mpfi_obj::~mpfi_obj() 
{ 
    mpfi_clear(mp);
}

// Bisection members
mpfi_obj mpfi_obj::half1(mp_prec_t prec) const
{
	mpfi_t half1,half2;
	mpfi_init2(half1,prec);
	mpfi_init2(half2,prec);
	mpfi_bisect(half1,half2,mp);
	
	return mpfi_obj(half1);
}

mpfi_obj mpfi_obj::half2(mp_prec_t prec) const
{
	mpfi_t half1,half2;
	mpfi_init2(half1,prec);
	mpfi_init2(half2,prec);
	mpfi_bisect(half1,half2,mp);
	
	return mpfi_obj(half2);
}

// Mid and access to the endpoints
inline int mpfi_obj::mid(mpfr_t mid) const
{
	return mpfi_mid(mid,mp);
}

inline int mpfi_obj::left(mpfr_t left) const
{
	return mpfi_get_left(left,mp);
}

inline int mpfi_obj::right(mpfr_t right) const 
{
	return mpfi_get_right(right,mp);
}


// SFINAE????

// internal namespace needed for template magic
namespace internal{

    // Use SFINAE to restrict arithmetic operations instantiation only for numeric types
    // This is needed for smooth integration with libraries based on expression templates, like Eigen.
    // TODO: Do the same for boolean operators.
    template <typename ArgumentType> struct result_type {};    
    
    template <> struct result_type<mpfi_obj>              {typedef mpfi_obj type;};    
    template <> struct result_type<mpz_t>               {typedef mpfi_obj type;};    
    template <> struct result_type<mpq_t>               {typedef mpfi_obj type;};    
    template <> struct result_type<long double>         {typedef mpfi_obj type;};    
    template <> struct result_type<double>              {typedef mpfi_obj type;};    
    template <> struct result_type<unsigned long int>   {typedef mpfi_obj type;};    
    template <> struct result_type<unsigned int>        {typedef mpfi_obj type;};    
    template <> struct result_type<long int>            {typedef mpfi_obj type;};    
    template <> struct result_type<int>                 {typedef mpfi_obj type;};    
}

// + Addition
template <typename Rhs> 
inline const typename internal::result_type<Rhs>::type 
    operator+(const mpfi_obj& lhs, const Rhs& rhs){ return mpfi_obj(lhs) += rhs;    }

template <typename Lhs> 
inline const typename internal::result_type<Lhs>::type 
    operator+(const Lhs& lhs, const mpfi_obj& rhs){ return mpfi_obj(rhs) += lhs;    } 

// - Subtraction
template <typename Rhs> 
inline const typename internal::result_type<Rhs>::type 
    operator-(const mpfi_obj& lhs, const Rhs& rhs){ return mpfi_obj(lhs) -= rhs;    }

template <typename Lhs> 
inline const typename internal::result_type<Lhs>::type 
    operator-(const Lhs& lhs, const mpfi_obj& rhs){ return mpfi_obj(lhs) -= rhs;    }

// * Multiplication
template <typename Rhs> 
inline const typename internal::result_type<Rhs>::type 
    operator*(const mpfi_obj& lhs, const Rhs& rhs){ return mpfi_obj(lhs) *= rhs;    }

template <typename Lhs> 
inline const typename internal::result_type<Lhs>::type 
    operator*(const Lhs& lhs, const mpfi_obj& rhs){ return mpfi_obj(rhs) *= lhs;    } 

// / Division
template <typename Rhs> 
inline const typename internal::result_type<Rhs>::type 
    operator/(const mpfi_obj& lhs, const Rhs& rhs){ return mpfi_obj(lhs) /= rhs;    }

template <typename Lhs> 
inline const typename internal::result_type<Lhs>::type 
    operator/(const Lhs& lhs, const mpfi_obj& rhs){ return mpfi_obj(lhs) /= rhs;    }
    
//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Operators - Assignment
inline mpfi_obj& mpfi_obj::operator=(const mpfi_obj& v)
{
    if (this != &v)
    {
		mp_prec_t tp = mpfi_get_prec(mp);
		mp_prec_t vp = mpfi_get_prec(v.mp);

		if(tp < vp){
			mpfi_clear(mp);
			mpfi_init2(mp, vp);
		}

        mpfi_set(mp, v.mp);

        OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    }
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const mpfr_t v)
{
    mpfi_set_fr(mp, v);
    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const mpz_t v)
{
    mpfi_set_z(mp, v);
    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const mpq_t v)
{
    mpfi_set_q(mp, v);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const double v)                
{  
	/* 
#if (OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW > -1)
	if(fits_in_bits(v, OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW))
	{
		mpfi_set_d(mp,v);
	}else
		throw conversion_overflow();
#else
	
#endif
*/
	mpfi_set_d(mp,v);
	OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const unsigned long int v)    
{    
    mpfi_set_ui(mp, v);    

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const unsigned int v)        
{    
    mpfi_set_ui(mp, v);    

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const long int v)            
{    
    mpfi_set_si(mp, v);    

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const int v)
{    
    mpfi_set_si(mp, v);    

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const char* s)
{
    // Use other converters for more precise control on base & precision & rounding:
    //
    //        mpfi_obj(const char* s,        mp_prec_t prec, int base, mp_rnd_t mode)
    //        mpfi_obj(const std::string& s,mp_prec_t prec, int base, mp_rnd_t mode)
    //
    // Here we assume base = 10 and we use precision of target variable.

    mpfi_t t;

    mpfi_init2(t, mpfi_get_prec(mp));

    if(0 == mpfi_set_str(t, s, 10))
    {
        mpfi_set(mp, t); 
        OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    }

    mpfi_clear(t);
    return *this;
}

inline mpfi_obj& mpfi_obj::operator=(const std::string& s)
{
    // Use other converters for more precise control on base & precision & rounding:
    //
    //        mpfi_obj(const char* s,        mp_prec_t prec, int base, mp_rnd_t mode)
    //        mpfi_obj(const std::string& s,mp_prec_t prec, int base, mp_rnd_t mode)
    //
    // Here we assume base = 10 and we use precision of target variable.

    mpfi_t t;

    mpfi_init2(t, mpfi_get_prec(mp));

    if(0 == mpfi_set_str(t, s.c_str(), 10))
    {
        mpfi_set(mp, t); 
        OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    }

    mpfi_clear(t);
    return *this;
}

//////////////////////////////////////////////////////////////////////////
// + Addition
inline mpfi_obj& mpfi_obj::operator+=(const mpfi_obj& v)
{
    mpfi_add(mp,mp,v.mp);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const mpfr_t u)
{
    *this += mpfi_obj(u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const mpz_t u)
{
    mpfi_add_z(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const mpq_t u)
{
    mpfi_add_q(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}
/*
inline mpfi_obj& mpfi_obj::operator+= (const long double u)
{
    *this += mpfi_obj(u);    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;    
}
*/
inline mpfi_obj& mpfi_obj::operator+= (const double u)
{
    mpfi_add_d(mp,mp,u);

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const unsigned long int u)
{
    mpfi_add_ui(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const unsigned int u)
{
    mpfi_add_ui(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const long int u)
{
    mpfi_add_si(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator+=(const int u)
{
    mpfi_add_si(mp,mp,u);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline const mpfi_obj mpfi_obj::operator+()const    {    return mpfi_obj(*this); }

inline const mpfi_obj operator+(const mpfi_obj& a, const mpfi_obj& b)
{
    // prec(a+b) = max(prec(a),prec(b))
    if(a.get_prec()>b.get_prec()) return mpfi_obj(a) += b;
    else                          return mpfi_obj(b) += a;
}

inline mpfi_obj& mpfi_obj::operator++() 
{
    return *this += 1;
}

inline const mpfi_obj mpfi_obj::operator++ (int)
{
    mpfi_obj x(*this);
    *this += 1;
    return x;
}

inline mpfi_obj& mpfi_obj::operator--() 
{
    return *this -= 1;
}

inline const mpfi_obj mpfi_obj::operator-- (int)
{
    mpfi_obj x(*this);
    *this -= 1;
    return x;
}

//////////////////////////////////////////////////////////////////////////
// - Subtraction
inline mpfi_obj& mpfi_obj::operator-= (const mpfi_obj& v)
{
    mpfi_sub(mp,mp,v.mp);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const mpfr_t v)
{
    mpfi_sub_fr(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const mpz_t v)
{
    mpfi_sub_z(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const mpq_t v)
{
    mpfi_sub_q(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}
/*
inline mpfi_obj& mpfi_obj::operator-=(const long double v)
{
    *this -= mpfi_obj(v);    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;    
}
*/
inline mpfi_obj& mpfi_obj::operator-=(const double v)
{
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    mpfi_sub_d(mp,mp,v);
#else
    *this -= mpfi_obj(v);    
#endif

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const unsigned long int v)
{
    mpfi_sub_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const unsigned int v)
{
    mpfi_sub_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const long int v)
{
    mpfi_sub_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator-=(const int v)
{
    mpfi_sub_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline const mpfi_obj mpfi_obj::operator-()const
{
    mpfi_obj u(*this);
    mpfi_neg(u.mp,u.mp);
    return u;
}

inline const mpfi_obj operator-(const mpfi_obj& a, const mpfi_obj& b)
{
    // prec(a-b) = max(prec(a),prec(b))
    if(a.getPrecision() >= b.getPrecision())    
    {
        return   mpfi_obj(a) -= b;
    }else{
        mpfi_obj x(a);
        x.setPrecision(b.getPrecision());
        return x -= b;        
    }
}

inline const mpfi_obj operator-(const double  b, const mpfi_obj& a)
{
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    mpfi_obj x(a);
    mpfi_d_sub(x.mp,b,a.mp);
    return x;
#else
    return mpfi_obj(b) -= a;
#endif
}

inline const mpfi_obj operator-(const unsigned long int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_ui_sub(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator-(const unsigned int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_ui_sub(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator-(const long int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_si_sub(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator-(const int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_si_sub(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator-(const mpfr_t b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_fr_sub(x.mp,b,a.mp);
    return x;
}

//////////////////////////////////////////////////////////////////////////
// * Multiplication
inline mpfi_obj& mpfi_obj::operator*= (const mpfi_obj& v)
{
    mpfi_mul(mp,mp,v.mp);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const mpz_t v)
{
    mpfi_mul_z(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const mpfr_t v)
{
    mpfi_mul_fr(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const mpq_t v)
{
    mpfi_mul_q(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

/*
inline mpfi_obj& mpfi_obj::operator*=(const long double v)
{
    *this *= mpfi_obj(v);    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;    
}
*/

inline mpfi_obj& mpfi_obj::operator*=(const double v)
{/*
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    mpfi_mul_d(mp,mp,v,mpfi_obj::get_default_rnd());
#else

#endif

*/
    *this *= mpfi_obj(v);    

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const unsigned long int v)
{
    mpfi_mul_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const unsigned int v)
{
    mpfi_mul_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const long int v)
{
    mpfi_mul_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator*=(const int v)
{
    mpfi_mul_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline const mpfi_obj operator*(const mpfi_obj& a, const mpfi_obj& b)
{
    // prec(a*b) = max(prec(a),prec(b))
    if(a.getPrecision() >= b.getPrecision())    return   mpfi_obj(a) *= b;
    else                                        return   mpfi_obj(b) *= a;        
}

//////////////////////////////////////////////////////////////////////////
// / Division
inline mpfi_obj& mpfi_obj::operator/=(const mpfi_obj& v)
{
    mpfi_div(mp,mp,v.mp);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const mpz_t v)
{
    mpfi_div_z(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const mpfr_t v)
{
    mpfi_div_fr(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}


inline mpfi_obj& mpfi_obj::operator/=(const mpq_t v)
{
    mpfi_div_q(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}
/*
inline mpfi_obj& mpfi_obj::operator/=(const long double v)
{
    *this /= mpfi_obj(v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;    
}
*/
inline mpfi_obj& mpfi_obj::operator/=(const double v)
{
	/*
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    mpfr_div_d(mp,mp,v,mpfi_obj::get_default_rnd());
#else
#endif

*/
    *this /= mpfi_obj(v);    
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const unsigned long int v)
{
    mpfi_div_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const unsigned int v)
{
    mpfi_div_ui(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const long int v)
{
    mpfi_div_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator/=(const int v)
{
    mpfi_div_si(mp,mp,v);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline const mpfi_obj operator/(const mpfi_obj& a, const mpfi_obj& b)
{
    // prec(a/b) = max(prec(a),prec(b))
    if(a.getPrecision() >= b.getPrecision())    
    {
        return   mpfi_obj(a) /= b;
    }else{

        mpfi_obj x(a);
        x.setPrecision(b.getPrecision());
        return x /= b;        
    }
}

inline const mpfi_obj operator/(const mpfr_t b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_fr_div(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator/(const unsigned long int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_ui_div(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator/(const unsigned int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_ui_div(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator/(const long int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_si_div(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator/(const int b, const mpfi_obj& a)
{
    mpfi_obj x(a);
    mpfi_si_div(x.mp,b,a.mp);
    return x;
}

inline const mpfi_obj operator/(const double  b, const mpfi_obj& a)
{
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    mpfi_obj x(a);
    mpfi_d_div(x.mp,b,a.mp);
    return x;
#else
    return mpfi_obj(b) /= a;
#endif
}

//////////////////////////////////////////////////////////////////////////
// Set/Get number properties

inline int mpfi_obj::getPrecision() const
{
    return mpfi_get_prec(mp);
}

inline mpfi_obj& mpfi_obj::setPrecision(int Precision)
{
    mpfi_set_prec(mp,Precision);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mp_prec_t mpfi_obj::get_prec() const
{
    return mpfi_get_prec(mp);
}

inline void mpfi_obj::set_prec(mp_prec_t prec)
{
    mpfi_set_prec(mp,prec);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
}
/*
inline mpfi_obj& mpfi_obj::setInf(int sign) 
{ 
    mpfr_set_inf(mp,sign);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}    

inline mpfi_obj& mpfi_obj::setNan() 
{
    mpfr_set_nan(mp);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj&    mpfi_obj::setZero(int sign)
{

#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
    mpfr_set_zero(mp, sign);
#else
    mpfr_set_si(mp, 0, (mpfr_get_default_rounding_mode)());
    setSign(sign);
#endif 

    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}
*/
// Get raw pointers so that mpfi_obj can correctly be used in raw mpfr_* functions
inline ::mpfi_ptr       mpfi_obj::mpfi_ptr()             {    return mp;    }
inline ::mpfi_srcptr    mpfi_obj::mpfi_srcptr() const    {    return const_cast< ::mpfi_srcptr >(mp);    }

//////////////////////////////////////////////////////////////////////////
// Mathematical Functions
//////////////////////////////////////////////////////////////////////////
inline const mpfi_obj sqr(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_sqr(x.mp,x.mp);
    return x;
}

inline const mpfi_obj sqrt(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_sqrt(x.mp,x.mp);
    return x;
}
/*
inline const mpfi_obj sqrt(const unsigned long int v)
{
    mpfi_obj x;
    mpfi_sqrt_ui(x.mp,v);
    return x;
}

inline const mpfi_obj sqrt(const unsigned int v)
{
    return sqrt(static_cast<unsigned long int>(v));
}

inline const mpfi_obj sqrt(const long int v)
{
    if (v>=0)    return sqrt(static_cast<unsigned long int>(v));
    else        return mpfi_obj().setNan(); // NaN  
}

inline const mpfi_obj sqrt(const int v)
{
    if (v>=0)    return sqrt(static_cast<unsigned long int>(v));
    else        return mpfi_obj().setNan(); // NaN
}
*/
/*
inline const mpfi_obj sqrt(const long double v)
{
    return sqrt(mpfi_obj(v));
}
*/
inline const mpfi_obj sqrt(const double v)
{
    return sqrt(mpfi_obj(v));
}

inline const mpfi_obj cbrt(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_cbrt(x.mp,x.mp);
    return x;
}
/*
inline const mpfi_obj root(const mpfi_obj& v, unsigned long int k)
{
    mpfi_obj x(v);
    mpfi_root(x.mp,x.mp,k);
    return x;
}
*/
inline const mpfi_obj fabs(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_abs(x.mp,x.mp);
    return x;
}

inline const mpfi_obj abs(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_abs(x.mp,x.mp);
    return x;
}
/*
inline const mpfi_obj dim(const mpfi_obj& a, const mpfi_obj& b)
{
    mpfi_obj x(a);
    mpfr_dim(x.mp,a.mp,b.mp);
    return x;
}

inline int cmpabs(const mpfi_obj& a,const mpfi_obj& b)
{
    return mpfr_cmpabs(a.mp,b.mp);
}
*/
////////////////////////////////
//////Pow function
/*
inline const mpfi_obj pow(const mpfi_obj& a, const mpfi_obj& b)
{
    mpfi_obj x(a);
    mpfi_pow(x.mp,x.mp,b.mp);
    return x;
}

inline const mpfi_obj pow(const mpfi_obj& a, const mpz_t b)
{
    mpfi_obj x(a);
    mpfi_pow_z(x.mp,x.mp,b);
    return x;
}

inline const mpfi_obj pow(const mpfi_obj& a, const unsigned long int b)
{
    mpfi_obj x(a);
    mpfi_pow_ui(x.mp,x.mp,b);
    return x;
}

inline const mpfi_obj pow(const mpfi_obj& a, const unsigned int b)
{
    return pow(a,static_cast<unsigned long int>(b));
}

inline const mpfi_obj pow(const mpfi_obj& a, const long int b)
{
    mpfi_obj x(a);
    mpfr_pow_si(x.mp,x.mp,b);
    return x;
}

inline const mpfi_obj pow(const mpfi_obj& a, const int b)
{
    return pow(a,static_cast<long int>(b));
}

inline const mpfi_obj pow(const mpfi_obj& a, const long double b)
{
    return pow(a,mpfi_obj(b));
}

inline const mpfi_obj pow(const mpfi_obj& a, const double b)
{
    return pow(a,mpfi_obj(b));
}

inline const mpfi_obj pow(const unsigned long int a, const mpfi_obj& b)
{
    mpfi_obj x(a);
    mpfr_ui_pow(x.mp,a,b.mp);
    return x;
}

inline const mpfi_obj pow(const unsigned int a, const mpfi_obj& b)
{
    return pow(static_cast<unsigned long int>(a),b);
}

inline const mpfi_obj pow(const long int a, const mpfi_obj& b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),b);
    else        return pow(mpfi_obj(a),b);
}

inline const mpfi_obj pow(const int a, const mpfi_obj& b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),b);
    else        return pow(mpfi_obj(a),b);
}

inline const mpfi_obj pow(const long double a, const mpfi_obj& b)
{
    return pow(mpfi_obj(a),b);
}

inline const mpfi_obj pow(const double a, const mpfi_obj& b)
{
    return pow(mpfi_obj(a),b);
}

// pow unsigned long int
inline const mpfi_obj pow(const unsigned long int a, const unsigned long int b)
{
    mpfi_obj x(a);
    mpfr_ui_pow_ui(x.mp,a,b);
    return x;
}

inline const mpfi_obj pow(const unsigned long int a, const unsigned int b)
{
    return pow(a,static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
}

inline const mpfi_obj pow(const unsigned long int a, const long int b)
{
    if(b>0)    return pow(a,static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
    else    return pow(a,mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned long int a, const int b)
{
    if(b>0)    return pow(a,static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
    else    return pow(a,mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned long int a, const long double b)
{
    return pow(a,mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned long int a, const double b)
{
    return pow(a,mpfi_obj(b)); //mpfr_ui_pow
}

// pow unsigned int
inline const mpfi_obj pow(const unsigned int a, const unsigned long int b)
{
    return pow(static_cast<unsigned long int>(a),b); //mpfr_ui_pow_ui
}

inline const mpfi_obj pow(const unsigned int a, const unsigned int b)
{
    return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
}

inline const mpfi_obj pow(const unsigned int a, const long int b)
{
    if(b>0)    return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
    else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned int a, const int b)
{
    if(b>0)    return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
    else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned int a, const long double b)
{
    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
}

inline const mpfi_obj pow(const unsigned int a, const double b)
{
    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
}

// pow long int
inline const mpfi_obj pow(const long int a, const unsigned long int b)
{
    if (a>0) return pow(static_cast<unsigned long int>(a),b); //mpfr_ui_pow_ui
    else     return pow(mpfi_obj(a),b); //mpfr_pow_ui
}

inline const mpfi_obj pow(const long int a, const unsigned int b)
{
    if (a>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b));  //mpfr_ui_pow_ui
    else     return pow(mpfi_obj(a),static_cast<unsigned long int>(b)); //mpfr_pow_ui
}

inline const mpfi_obj pow(const long int a, const long int b)
{
    if (a>0)
    {
        if(b>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
        else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    }else{
        return pow(mpfi_obj(a),b); // mpfr_pow_si
    }
}

inline const mpfi_obj pow(const long int a, const int b)
{
    if (a>0)
    {
        if(b>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
        else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    }else{
        return pow(mpfi_obj(a),static_cast<long int>(b)); // mpfr_pow_si
    }
}

inline const mpfi_obj pow(const long int a, const long double b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    else        return pow(mpfi_obj(a),mpfi_obj(b)); //mpfr_pow
}

inline const mpfi_obj pow(const long int a, const double b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    else        return pow(mpfi_obj(a),mpfi_obj(b)); //mpfr_pow
}

// pow int
inline const mpfi_obj pow(const int a, const unsigned long int b)
{
    if (a>0) return pow(static_cast<unsigned long int>(a),b); //mpfr_ui_pow_ui
    else     return pow(mpfi_obj(a),b); //mpfr_pow_ui
}

inline const mpfi_obj pow(const int a, const unsigned int b)
{
    if (a>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b));  //mpfr_ui_pow_ui
    else     return pow(mpfi_obj(a),static_cast<unsigned long int>(b)); //mpfr_pow_ui
}

inline const mpfi_obj pow(const int a, const long int b)
{
    if (a>0)
    {
        if(b>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
        else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    }else{
        return pow(mpfi_obj(a),b); // mpfr_pow_si
    }
}

inline const mpfi_obj pow(const int a, const int b)
{
    if (a>0)
    {
        if(b>0) return pow(static_cast<unsigned long int>(a),static_cast<unsigned long int>(b)); //mpfr_ui_pow_ui
        else    return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    }else{
        return pow(mpfi_obj(a),static_cast<long int>(b)); // mpfr_pow_si
    }
}

inline const mpfi_obj pow(const int a, const long double b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    else        return pow(mpfi_obj(a),mpfi_obj(b)); //mpfr_pow
}

inline const mpfi_obj pow(const int a, const double b)
{
    if (a>=0)     return pow(static_cast<unsigned long int>(a),mpfi_obj(b)); //mpfr_ui_pow
    else        return pow(mpfi_obj(a),mpfi_obj(b)); //mpfr_pow
}

// pow long double 
inline const mpfi_obj pow(const long double a, const long double b)
{
    return pow(mpfi_obj(a),mpfi_obj(b));
}

inline const mpfi_obj pow(const long double a, const unsigned long int b)
{
    return pow(mpfi_obj(a),b); //mpfr_pow_ui
}

inline const mpfi_obj pow(const long double a, const unsigned int b)
{
    return pow(mpfi_obj(a),static_cast<unsigned long int>(b)); //mpfr_pow_ui
}

inline const mpfi_obj pow(const long double a, const long int b)
{
    return pow(mpfi_obj(a),b); // mpfr_pow_si
}

inline const mpfi_obj pow(const long double a, const int b)
{
    return pow(mpfi_obj(a),static_cast<long int>(b)); // mpfr_pow_si
}

inline const mpfi_obj pow(const double a, const double b)
{
    return pow(mpfi_obj(a),mpfi_obj(b));
}

inline const mpfi_obj pow(const double a, const unsigned long int b)
{
    return pow(mpfi_obj(a),b); // mpfr_pow_ui
}

inline const mpfi_obj pow(const double a, const unsigned int b)
{
    return pow(mpfi_obj(a),static_cast<unsigned long int>(b)); // mpfr_pow_ui
}

inline const mpfi_obj pow(const double a, const long int b)
{
    return pow(mpfi_obj(a),b); // mpfr_pow_si
}

inline const mpfi_obj pow(const double a, const int b)
{
    return pow(mpfi_obj(a),static_cast<long int>(b)); // mpfr_pow_si
}
*/

inline const mpfi_obj log  (const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_log(x.mp,v.mp);
    return x;
}

inline const mpfi_obj log2(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_log2(x.mp,v.mp);
    return x;
}

inline const mpfi_obj log10(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_log10(x.mp,v.mp);
    return x;
}

inline const mpfi_obj exp(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_exp(x.mp,v.mp);
    return x;
}

inline const mpfi_obj exp2(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_exp2(x.mp,v.mp);
    return x;
}
/*
inline const mpfi_obj exp10(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_exp10(x.mp,v.mp);
    return x;
}
*/

inline const mpfi_obj log1p  (const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_log1p(x.mp,v.mp);
    return x;
}

inline const mpfi_obj expm1  (const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfi_expm1(x.mp,v.mp);
    return x;
}

////////////////// I/O

/*
inline std::string mpfi_obj::toString(int n, int b) const
{
    (void)b;
    (void)mode;

    // Use MPFR native function for output
    char format[128];
    int digits;

    digits = n > 0 ? n : bits2digits(mpfr_get_prec(mp));

    sprintf(format,"%%.%dRNg",digits);        // Default format

    return toString(std::string(format));

}
*/

inline std::string mpfi_obj::toString() const
{
    char *s = NULL;
    std::string str_left,str_right,out;
	
	mpfr_t left,right;
	mpfr_init2(left,get_prec());
	mpfr_init2(right,get_prec());
	mpfi_get_left(left,mp);
	mpfi_get_right(right,mp);
	
        if(!(mpfr_asprintf(&s,"%.16R*f", GMP_RNDN,left) < 0))
        {
            str_left = std::string(s);
            mpfr_free_str(s);
        }
        if(!(mpfr_asprintf(&s,"%.128R*f", GMP_RNDU,right) < 0))
        {
            str_right = std::string(s);

            mpfr_free_str(s);
        }
    
    out="["+str_left+","+str_right+"]";
   
    mpfr_clear(left);
	mpfr_clear(right);
	
    return out;
}

//I/O
inline std::ostream& operator<<(std::ostream& os, const mpfi_obj& v)
{
    return os<<v.toString();
}

//////////////////////////////////////////////////////////////////////////
//Boolean operators
inline bool operator >  (const mpfi_obj& a, const mpfi_obj& b){    return (-1*mpfi_cmp(a.mp,b.mp)      !=0);    }
inline bool operator >= (const mpfi_obj& a, const mpfi_obj& b){    return (-1*mpfi_cmp(a.mp,b.mp) !=0);    }
inline bool operator <  (const mpfi_obj& a, const mpfi_obj& b){    return (mpfi_cmp(a.mp,b.mp)         !=0);    }
inline bool operator <= (const mpfi_obj& a, const mpfi_obj& b){    return (mpfi_cmp(a.mp,b.mp)    !=0);    }
inline bool operator == (const mpfi_obj& a, const mpfi_obj& b){    return (mpfi_cmp(a.mp,b.mp)        !=0);    }
inline bool operator != (const mpfi_obj& a, const mpfi_obj& b){    return (mpfi_cmp(a.mp,b.mp)  !=0);    }

inline bool operator == (const mpfi_obj& a, const unsigned long int b ){    return (mpfi_cmp_ui(a.mp,b) == 0);    }
inline bool operator == (const mpfi_obj& a, const unsigned int b      ){    return (mpfi_cmp_ui(a.mp,b) == 0);    }
inline bool operator == (const mpfi_obj& a, const long int b          ){    return (mpfi_cmp_si(a.mp,b) == 0);    }
inline bool operator == (const mpfi_obj& a, const int b               ){    return (mpfi_cmp_si(a.mp,b) == 0);    }
inline bool operator == (const mpfi_obj& a, const double b            ){    return (mpfi_cmp_d(a.mp,b)  == 0);    }


inline bool isnan    (const mpfi_obj& v){    return (mpfi_nan_p(v.mp)     != 0);    }
inline bool isinf    (const mpfi_obj& v){    return (mpfi_inf_p(v.mp)     != 0);    }
inline bool isbounded (const mpfi_obj& v){    return (mpfi_bounded_p(v.mp)  != 0);    }
inline bool haszero   (const mpfi_obj& v){    return (mpfi_has_zero(v.mp)    != 0);    }
//inline bool isint    (const mpfi_obj& v){    return (mpfr_integer_p(v.mp) != 0);    }

inline bool ispos    (const mpfi_obj& v){    return (mpfi_is_pos(v.mp)     != 0);    }
inline bool isnonneg    (const mpfi_obj& v){    return (mpfi_is_nonneg(v.mp)     != 0);    }
inline bool isneg    (const mpfi_obj& v){    return (mpfi_is_neg(v.mp)     != 0);    }
inline bool isnonpos    (const mpfi_obj& v){    return (mpfi_is_nonpos(v.mp)     != 0);    }
    

}

