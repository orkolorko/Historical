/*
    Multi-precision floating point number class for C++. 
    Based on MPFR library:    http://mpfr.org

    Project homepage:    http://www.holoborodko.com/pavel/mpfr
    Contact e-mail:      pavel@holoborodko.com

    Copyright (c) 2008-2012 Pavel Holoborodko

    Contributors:
    Dmitriy Gubanov, Konstantin Holoborodko, Brian Gladman, 
    Helmut Jarausch, Fokko Beekhof, Ulrich Mutze, Heinz van Saanen, 
    Pere Constans, Peter van Hoof, Gael Guennebaud, Tsai Chia Cheng, 
    Alexei Zubanov, Jauhien Piatlicki, Victor Berger, John Westwood.

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

#ifndef __OBJECTIVEMPFI_H__
#define __OBJECTIVEMPFI_H__

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cfloat>
#include <cmath>
#include <limits>

// Options
#define OBJECTIVEMPFI_HAVE_INT64_SUPPORT               // Enable int64_t support if possible. Available only for MSVC 2010 & GCC. 


// Detect compiler using signatures from http://predef.sourceforge.net/
#if defined(__GNUC__) && defined(__INTEL_COMPILER)
    #define IsInf(x) isinf(x)                   // Intel ICC compiler on Linux 

#elif defined(_MSC_VER)                         // Microsoft Visual C++ 
    #define IsInf(x) (!_finite(x))                           

#else
    #define IsInf(x) std::isinf(x)              // GNU C/C++ (and/or other compilers), just hope for C99 conformance
#endif

#if defined(OBJECTIVEMPFI_HAVE_INT64_SUPPORT)
    
    #define MPFR_USE_INTMAX_T                   // Should be defined before mpfr.h

    #if defined(_MSC_VER)                       // <stdint.h> is available only in msvc2010!
        #if (_MSC_VER >= 1600)                    
            #include <stdint.h>                    
        #else                                   // MPFR relies on intmax_t which is available only in msvc2010
            #undef OBJECTIVEMPFI_HAVE_INT64_SUPPORT    // Besides, MPFR & MPIR have to be compiled with msvc2010
            #undef MPFR_USE_INTMAX_T            // Since we cannot detect this, disable x64 by default
                                                // Someone should change this manually if needed.
        #endif
    #endif
    
    #if defined (__MINGW32__) || defined(__MINGW64__)
            #include <stdint.h>                 // Equivalent to msvc2010

    #elif defined (__GNUC__)
        #if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(__ia64) || defined(__itanium__) || defined(_M_IA64)
            #undef OBJECTIVEMPFI_HAVE_INT64_SUPPORT    // Remove all shaman dances for x64 builds since
            #undef MPFR_USE_INTMAX_T            // GCC already supports x64 as of "long int" is 64-bit integer, nothing left to do
        #else
            #include <stdint.h>                 // use int64_t, uint64_t otherwise.
        #endif
    #endif

#endif 

#if defined(OBJECTIVEMPFI_HAVE_MSVC_DEBUGVIEW) && defined(_MSC_VER) && defined(_DEBUG)
#define OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE         DebugView = toString();
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_DATA     std::string DebugView;
#else
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE 
    #define OBJECTIVEMPFI_MSVC_DEBUGVIEW_DATA 
#endif

#include <mpfi.h>

#if (MPFR_VERSION < MPFR_VERSION_NUM(3,0,0))
    #include <cstdlib>                          // Needed for random()
#endif

// Less important options
#define OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW -1          // Triggers overflow exception during conversion to double if OBJECTIVEMPFI 
                                                // cannot fit in OBJECTIVEMPFI_DOUBLE_BITS_OVERFLOW bits
                                                // = -1 disables overflow checks (default)
namespace mpfi_obj {

class mpfi_obj {
private:
    mpfi_t mp;
    
public:
    
    // Get default rounding mode & precision
    inline static mp_rnd_t   get_default_rnd()    {    return (mp_rnd_t)(mpfr_get_default_rounding_mode());       }
    inline static mp_prec_t  get_default_prec()   {    return mpfr_get_default_prec();                            }

                         

    // Operations
    // =
    // +, -, *, /, ++, --, <<, >> 
    // *=, +=, -=, /=,
    // <, >, ==, <=, >=

    //<<= Fast Multiplication by 2^u
    mpfi_obj& operator<<=(const unsigned long int u);
    mpfi_obj& operator<<=(const unsigned int u);
    mpfi_obj& operator<<=(const long int u);
    mpfi_obj& operator<<=(const int u);

    //>>= Fast Division by 2^u
    mpfi_obj& operator>>=(const unsigned long int u);
    mpfi_obj& operator>>=(const unsigned int u);
    mpfi_obj& operator>>=(const long int u);
    mpfi_obj& operator>>=(const int u);

    

    

    // Type Conversion operators
    long            toLong      (mp_rnd_t mode = GMP_RNDZ)    const;
    unsigned long   toULong     (mp_rnd_t mode = GMP_RNDZ)    const;
    double          toDouble    (mp_rnd_t mode = GMP_RNDN)    const;
    long double     toLDouble   (mp_rnd_t mode = GMP_RNDN)    const;

    
    // Convert mpfi_obj to string with n significant digits in base b
    // n = 0 -> convert with the maximum available digits 
    //
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    
#endif

    // Math Functions
    
    
    

    friend const mpfi_obj cos(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj sin(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj tan(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj sec(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj csc(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj cot(const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend int sin_cos(mpfi_obj& s, mpfi_obj& c, const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    friend const mpfi_obj acos  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj asin  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj atan  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj atan2 (const mpfi_obj& y, const mpfi_obj& x, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj acot  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj asec  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj acsc  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    friend const mpfi_obj cosh  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj sinh  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj tanh  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj sech  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj csch  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj coth  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj acosh (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj asinh (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj atanh (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj acoth (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj asech (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj acsch (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    friend const mpfi_obj hypot (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    friend const mpfi_obj fac_ui (unsigned long int v,  mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj eint   (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    friend const mpfi_obj gamma    (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj lngamma  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj lgamma   (const mpfi_obj& v, int *signp = 0, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj zeta     (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj erf      (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj erfc     (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj besselj0 (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd()); 
    friend const mpfi_obj besselj1 (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd()); 
    friend const mpfi_obj besseljn (long n, const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj bessely0 (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj bessely1 (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj besselyn (long n, const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd()); 
    friend const mpfi_obj fma      (const mpfi_obj& v1, const mpfi_obj& v2, const mpfi_obj& v3, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj fms      (const mpfi_obj& v1, const mpfi_obj& v2, const mpfi_obj& v3, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj agm      (const mpfi_obj& v1, const mpfi_obj& v2, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj sum      (const mpfi_obj tab[], unsigned long int n, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend int sgn(const mpfi_obj& v); // returns -1 or +1

// MPFR 2.4.0 Specifics
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))
    friend int          sinh_cosh   (mpfi_obj& s, mpfi_obj& c, const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj li2         (const mpfi_obj& v,                       mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj fmod        (const mpfi_obj& x, const mpfi_obj& y,      mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj rec_sqrt    (const mpfi_obj& v,                       mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    // MATLAB's semantic equivalents
    friend const mpfi_obj rem (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd()); // Remainder after division
    friend const mpfi_obj mod (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd()); // Modulus after division
#endif

// MPFR 3.0.0 Specifics
#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
    friend const mpfi_obj digamma (const mpfi_obj& v,        mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj ai      (const mpfi_obj& v,        mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj urandom (gmp_randstate_t& state, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());     // use gmp_randinit_default() to init state, gmp_randclear() to clear
#endif
    
    // Uniformly distributed random number generation in [0,1] using
    // Mersenne-Twister algorithm by default.
    // Use parameter to setup seed, e.g.: random((unsigned)time(NULL))
    // Check urandom() for more precise control.
    friend const mpfi_obj random(unsigned int seed = 0);
    
    // Exponent and mantissa manipulation
    friend const mpfi_obj frexp(const mpfi_obj& v, mp_exp_t* exp);    
    friend const mpfi_obj ldexp(const mpfi_obj& v, mp_exp_t exp);

    // Splits mpfi_obj value into fractional and integer parts.
    // Returns fractional part and stores integer part in n.
    friend const mpfi_obj modf(const mpfi_obj& v, mpfi_obj& n);    

    // Constants
    // don't forget to call mpfr_free_cache() for every thread where you are using const-functions
    friend const mpfi_obj const_log2      (mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj const_pi        (mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj const_euler     (mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj const_catalan   (mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    // returns +inf iff sign>=0 otherwise -inf
    friend const mpfi_obj const_infinity(int sign = 1, mp_prec_t prec = mpfi_obj::get_default_prec(), mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

    // Output/ Input
    
    friend std::istream& operator>>(std::istream& is, mpfi_obj& v);

    // Integer Related Functions
    friend const mpfi_obj rint (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj ceil (const mpfi_obj& v);
    friend const mpfi_obj floor(const mpfi_obj& v);
    friend const mpfi_obj round(const mpfi_obj& v);
    friend const mpfi_obj trunc(const mpfi_obj& v);
    friend const mpfi_obj rint_ceil   (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj rint_floor  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj rint_round  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj rint_trunc  (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj frac        (const mpfi_obj& v, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj remainder   (         const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj remquo      (long* q, const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    
    // Miscellaneous Functions
    friend const mpfi_obj nexttoward (const mpfi_obj& x, const mpfi_obj& y);
    friend const mpfi_obj nextabove  (const mpfi_obj& x);
    friend const mpfi_obj nextbelow  (const mpfi_obj& x);

    // use gmp_randinit_default() to init state, gmp_randclear() to clear
    friend const mpfi_obj urandomb (gmp_randstate_t& state); 

// MPFR < 2.4.2 Specifics
#if (MPFR_VERSION <= MPFR_VERSION_NUM(2,4,2))
    friend const mpfi_obj random2 (mp_size_t size, mp_exp_t exp);
#endif

    
#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
    friend bool isregular(const mpfi_obj& v);
#endif

    

    
    
    

    //Exponent
    mp_exp_t get_exp();
    int set_exp(mp_exp_t e);
    int check_range  (int t, mp_rnd_t rnd_mode = get_default_rnd());
    int subnormalize (int t,mp_rnd_t rnd_mode = get_default_rnd());

    // Inexact conversion from float
    inline bool fits_in_bits(double x, int n);

    // Set/Get global properties
    static void            set_default_prec(mp_prec_t prec);
    static void            set_default_rnd(mp_rnd_t rnd_mode);

    static mp_exp_t  get_emin (void);
    static mp_exp_t  get_emax (void);
    static mp_exp_t  get_emin_min (void);
    static mp_exp_t  get_emin_max (void);
    static mp_exp_t  get_emax_min (void);
    static mp_exp_t  get_emax_max (void);
    static int       set_emin (mp_exp_t exp);
    static int       set_emax (mp_exp_t exp);

    // Efficient swapping of two mpfi_obj values - needed for std algorithms
    friend void swap(mpfi_obj& x, mpfi_obj& y);
    
    friend const mpfi_obj fmax(const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());
    friend const mpfi_obj fmin(const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode = mpfi_obj::get_default_rnd());

private:
    // Human friendly Debug Preview in Visual Studio.
    // Put one of these lines:
    //
    // mpfr::mpfi_obj=<DebugView>                                ; Show value only
    // mpfr::mpfi_obj=<DebugView>, <mp[0]._mpfr_prec,u>bits    ; Show value & precision
    // 
    // at the beginning of
    // [Visual Studio Installation Folder]\Common7\Packages\Debugger\autoexp.dat
    */
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_DATA
};

//////////////////////////////////////////////////////////////////////////
// Exceptions
class conversion_overflow : public std::exception {
public:
    std::string why() { return "inexact conversion from floating point"; }
};



//////////////////////////////////////////////////////////////////////////
// Estimate machine epsilon for the given precision
// Returns smallest eps such that 1.0 + eps != 1.0
inline mpfi_obj machine_epsilon(mp_prec_t prec = mpfi_obj::get_default_prec());

// Returns smallest eps such that x + eps != x (relative machine epsilon)
inline mpfi_obj machine_epsilon(const mpfi_obj& x);        

// Gives max & min values for the required precision, 
// minval is 'safe' meaning 1 / minval does not overflow
// maxval is 'safe' meaning 1 / maxval does not underflow
inline mpfi_obj minval(mp_prec_t prec = mpfi_obj::get_default_prec());
inline mpfi_obj maxval(mp_prec_t prec = mpfi_obj::get_default_prec());

// 'Dirty' equality check 1: |a-b| < min{|a|,|b|} * eps
inline bool isEqualFuzzy(const mpfi_obj& a, const mpfi_obj& b, const mpfi_obj& eps);

// 'Dirty' equality check 2: |a-b| < min{|a|,|b|} * eps( min{|a|,|b|} )
inline bool isEqualFuzzy(const mpfi_obj& a, const mpfi_obj& b);

// 'Bitwise' equality check
//  maxUlps - a and b can be apart by maxUlps binary numbers. 
inline bool isEqualUlps(const mpfi_obj& a, const mpfi_obj& b, int maxUlps);

//////////////////////////////////////////////////////////////////////////
//     Convert precision in 'bits' to decimal digits and vice versa.
//        bits   = ceil(digits*log[2](10))
//        digits = floor(bits*log[10](2))

inline mp_prec_t digits2bits(int d);
inline int       bits2digits(mp_prec_t b);

//////////////////////////////////////////////////////////////////////////
// min, max
const mpfi_obj (max)(const mpfi_obj& x, const mpfi_obj& y);
const mpfi_obj (min)(const mpfi_obj& x, const mpfi_obj& y);

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// Shifts operators - Multiplication/Division by power of 2
inline mpfi_obj& mpfi_obj::operator<<=(const unsigned long int u)
{
    mpfr_mul_2ui(mp,mp,u,mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator<<=(const unsigned int u)
{
    mpfr_mul_2ui(mp,mp,static_cast<unsigned long int>(u),mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator<<=(const long int u)
{
    mpfr_mul_2si(mp,mp,u,mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator<<=(const int u)
{
    mpfr_mul_2si(mp,mp,static_cast<long int>(u),mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator>>=(const unsigned long int u)
{
    mpfr_div_2ui(mp,mp,u,mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator>>=(const unsigned int u)
{
    mpfr_div_2ui(mp,mp,static_cast<unsigned long int>(u),mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator>>=(const long int u)
{
    mpfr_div_2si(mp,mp,u,mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline mpfi_obj& mpfi_obj::operator>>=(const int u)
{
    mpfr_div_2si(mp,mp,static_cast<long int>(u),mpfi_obj::get_default_rnd());
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return *this;
}

inline const mpfi_obj operator<<(const mpfi_obj& v, const unsigned long int k)
{
    return mul_2ui(v,k);
}

inline const mpfi_obj operator<<(const mpfi_obj& v, const unsigned int k)
{
    return mul_2ui(v,static_cast<unsigned long int>(k));
}

inline const mpfi_obj operator<<(const mpfi_obj& v, const long int k)
{
    return mul_2si(v,k);
}

inline const mpfi_obj operator<<(const mpfi_obj& v, const int k)
{
    return mul_2si(v,static_cast<long int>(k));
}

inline const mpfi_obj operator>>(const mpfi_obj& v, const unsigned long int k)
{
    return div_2ui(v,k);
}

inline const mpfi_obj operator>>(const mpfi_obj& v, const long int k)
{
    return div_2si(v,k);
}

inline const mpfi_obj operator>>(const mpfi_obj& v, const unsigned int k)
{
    return div_2ui(v,static_cast<unsigned long int>(k));
}

inline const mpfi_obj operator>>(const mpfi_obj& v, const int k)
{
    return div_2si(v,static_cast<long int>(k));
}

// mul_2ui
inline const mpfi_obj mul_2ui(const mpfi_obj& v, unsigned long int k, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_mul_2ui(x.mp,v.mp,k,rnd_mode);
    return x;
}

// mul_2si
inline const mpfi_obj mul_2si(const mpfi_obj& v, long int k, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_mul_2si(x.mp,v.mp,k,rnd_mode);
    return x;
}

inline const mpfi_obj div_2ui(const mpfi_obj& v, unsigned long int k, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_div_2ui(x.mp,v.mp,k,rnd_mode);
    return x;
}

inline const mpfi_obj div_2si(const mpfi_obj& v, long int k, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_div_2si(x.mp,v.mp,k,rnd_mode);
    return x;
}



#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
inline bool isregular(const mpfi_obj& v){    return (mpfr_regular_p(v.mp));}
#endif 

//////////////////////////////////////////////////////////////////////////
// Type Converters
inline long             mpfi_obj::toLong   (mp_rnd_t mode)  const    {    return mpfr_get_si(mp, mode);    }
inline unsigned long    mpfi_obj::toULong  (mp_rnd_t mode)  const    {    return mpfr_get_ui(mp, mode);    }
inline double           mpfi_obj::toDouble (mp_rnd_t mode)  const    {    return mpfr_get_d (mp, mode);    }
inline long double      mpfi_obj::toLDouble(mp_rnd_t mode)  const    {    return mpfr_get_ld(mp, mode);    }

#if defined (OBJECTIVEMPFI_HAVE_INT64_SUPPORT)
inline int64_t      mpfi_obj::toInt64 (mp_rnd_t mode)    const{    return mpfr_get_sj(mp, mode);    }
inline uint64_t     mpfi_obj::toUInt64(mp_rnd_t mode)    const{    return mpfr_get_uj(mp, mode);    }
#endif



template <class T>
inline std::string toString(T t, std::ios_base & (*f)(std::ios_base&))
{
    std::ostringstream oss;
    oss << f << t;
    return oss.str();
}

#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))



#endif

inline std::string mpfi_obj::toString(int n, int b, mp_rnd_t mode) const
{
    (void)b;
    (void)mode;

#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))

    // Use MPFR native function for output
    char format[128];
    int digits;

    digits = n > 0 ? n : bits2digits(mpfr_get_prec(mp));

    sprintf(format,"%%.%dRNg",digits);        // Default format

    return toString(std::string(format));

#else

    char *s, *ns = NULL; 
    size_t slen, nslen;
    mp_exp_t exp;
    std::string out;

    if(mpfr_inf_p(mp))
    { 
        if(mpfr_sgn(mp)>0) return "+Inf";
        else               return "-Inf";
    }

    if(mpfr_zero_p(mp)) return "0";
    if(mpfr_nan_p(mp))  return "NaN";

    s  = mpfr_get_str(NULL,&exp,b,0,mp,mode);
    ns = mpfr_get_str(NULL,&exp,b,n,mp,mode);

    if(s!=NULL && ns!=NULL)
    {
        slen  = strlen(s);
        nslen = strlen(ns);
        if(nslen<=slen) 
        {
            mpfr_free_str(s);
            s = ns;
            slen = nslen;
        }
        else {
            mpfr_free_str(ns);
        }

        // Make human eye-friendly formatting if possible
        if (exp>0 && static_cast<size_t>(exp)<slen)
        {
            if(s[0]=='-')
            {
                // Remove zeros starting from right end
                char* ptr = s+slen-1;
                while (*ptr=='0' && ptr>s+exp) ptr--; 

                if(ptr==s+exp) out = std::string(s,exp+1);
                else           out = std::string(s,exp+1)+'.'+std::string(s+exp+1,ptr-(s+exp+1)+1);

                //out = string(s,exp+1)+'.'+string(s+exp+1);
            }
            else
            {
                // Remove zeros starting from right end
                char* ptr = s+slen-1;
                while (*ptr=='0' && ptr>s+exp-1) ptr--; 

                if(ptr==s+exp-1) out = std::string(s,exp);
                else             out = std::string(s,exp)+'.'+std::string(s+exp,ptr-(s+exp)+1);

                //out = string(s,exp)+'.'+string(s+exp);
            }

        }else{ // exp<0 || exp>slen
            if(s[0]=='-')
            {
                // Remove zeros starting from right end
                char* ptr = s+slen-1;
                while (*ptr=='0' && ptr>s+1) ptr--; 

                if(ptr==s+1) out = std::string(s,2);
                else         out = std::string(s,2)+'.'+std::string(s+2,ptr-(s+2)+1);

                //out = string(s,2)+'.'+string(s+2);
            }
            else
            {
                // Remove zeros starting from right end
                char* ptr = s+slen-1;
                while (*ptr=='0' && ptr>s) ptr--; 

                if(ptr==s) out = std::string(s,1);
                else       out = std::string(s,1)+'.'+std::string(s+1,ptr-(s+1)+1);

                //out = string(s,1)+'.'+string(s+1);
            }

            // Make final string
            if(--exp)
            {
                if(exp>0) out += "e+"+mpfr::toString<mp_exp_t>(exp,std::dec);
                else       out += "e"+mpfr::toString<mp_exp_t>(exp,std::dec);
            }
        }

        mpfr_free_str(s);
        return out;
    }else{
        return "conversion error!";
    }
#endif
}


//////////////////////////////////////////////////////////////////////////
// I/O


inline std::istream& operator>>(std::istream &is, mpfi_obj& v)
{
    // ToDo, use cout::hexfloat and other flags to setup base
    std::string tmp;
    is >> tmp;
    mpfr_set_str(v.mp, tmp.c_str(), 10, mpfi_obj::get_default_rnd());
    return is;
}

//////////////////////////////////////////////////////////////////////////
//     Bits - decimal digits relation
//        bits   = ceil(digits*log[2](10))
//        digits = floor(bits*log[10](2))

inline mp_prec_t digits2bits(int d)
{
    const double LOG2_10 = 3.3219280948873624;

    return (mp_prec_t) std::ceil( d * LOG2_10 );
}

inline int bits2digits(mp_prec_t b)
{
    const double LOG10_2 = 0.30102999566398119;

    return (int) std::floor( b * LOG10_2 );
}

//////////////////////////////////////////////////////////////////////////
// Set/Get number properties
inline int sgn(const mpfi_obj& v)
{
    int r = mpfr_signbit(v.mp);
    return (r>0?-1:1);
}




inline mp_exp_t mpfi_obj::get_exp ()
{
    return mpfr_get_exp(mp);
}

inline int mpfi_obj::set_exp (mp_exp_t e)
{
    int x = mpfr_set_exp(mp, e);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return x;
}

inline const mpfi_obj frexp(const mpfi_obj& v, mp_exp_t* exp)
{
    mpfi_obj x(v);
    *exp = x.get_exp();
    x.set_exp(0);
    return x;
}

inline const mpfi_obj ldexp(const mpfi_obj& v, mp_exp_t exp)
{
    mpfi_obj x(v);

    // rounding is not important since we just increasing the exponent
    mpfr_mul_2si(x.mp,x.mp,exp,mpfi_obj::get_default_rnd()); 
    return x;
}

inline mpfi_obj machine_epsilon(mp_prec_t prec)
{
    /* the smallest eps such that 1 + eps != 1 */
    return machine_epsilon(mpfi_obj(1, prec));
}

inline mpfi_obj machine_epsilon(const mpfi_obj& x)
{    
    /* the smallest eps such that x + eps != x */
    if( x < 0)
    {
        return nextabove(-x)+x;
    }else{
        return nextabove(x)-x;
    }
}

// minval is 'safe' meaning 1 / minval does not overflow
inline mpfi_obj minval(mp_prec_t prec)
{
    /* min = 1/2 * 2^emin = 2^(emin - 1) */
    return mpfi_obj(1, prec) << mpfi_obj::get_emin()-1;
}

// maxval is 'safe' meaning 1 / maxval does not underflow
inline mpfi_obj maxval(mp_prec_t prec)
{
    /* max = (1 - eps) * 2^emax, eps is machine epsilon */
    return (mpfi_obj(1, prec) - machine_epsilon(prec)) << mpfi_obj::get_emax(); 
}

inline bool isEqualUlps(const mpfi_obj& a, const mpfi_obj& b, int maxUlps)
{
  return abs(a - b) <= machine_epsilon((max)(abs(a), abs(b))) * maxUlps;
}

inline bool isEqualFuzzy(const mpfi_obj& a, const mpfi_obj& b, const mpfi_obj& eps)
{
    return abs(a - b) <= (min)(abs(a), abs(b)) * eps;
}

inline bool isEqualFuzzy(const mpfi_obj& a, const mpfi_obj& b)
{
    return isEqualFuzzy(a, b, machine_epsilon((min)(abs(a), abs(b))));
}

inline const mpfi_obj modf(const mpfi_obj& v, mpfi_obj& n)
{
    mpfi_obj frac(v);

    // rounding is not important since we are using the same number
    mpfr_frac(frac.mp,frac.mp,mpfi_obj::get_default_rnd());    
    mpfr_trunc(n.mp,v.mp);
    return frac;
}

inline int mpfi_obj::check_range (int t, mp_rnd_t rnd_mode)
{
    return mpfr_check_range(mp,t,rnd_mode);
}

inline int mpfi_obj::subnormalize (int t,mp_rnd_t rnd_mode)
{
    int r = mpfr_subnormalize(mp,t,rnd_mode);
    OBJECTIVEMPFI_MSVC_DEBUGVIEW_CODE;
    return r;
}

inline mp_exp_t mpfi_obj::get_emin (void)
{
    return mpfr_get_emin();
}

inline int mpfi_obj::set_emin (mp_exp_t exp)
{
    return mpfr_set_emin(exp);
}

inline mp_exp_t mpfi_obj::get_emax (void)
{
    return mpfr_get_emax();
}

inline int mpfi_obj::set_emax (mp_exp_t exp)
{
    return mpfr_set_emax(exp);
}

inline mp_exp_t mpfi_obj::get_emin_min (void)
{
    return mpfr_get_emin_min();
}

inline mp_exp_t mpfi_obj::get_emin_max (void)
{
    return mpfr_get_emin_max();
}

inline mp_exp_t mpfi_obj::get_emax_min (void)
{
    return mpfr_get_emax_min();
}

inline mp_exp_t mpfi_obj::get_emax_max (void)
{
    return mpfr_get_emax_max();
}

//////////////////////////////////////////////////////////////////////////
// Mathematical Functions
//////////////////////////////////////////////////////////////////////////


inline const mpfi_obj cos(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_cos(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj sin(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_sin(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj tan(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_tan(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj sec(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_sec(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj csc(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_csc(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj cot(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_cot(x.mp,v.mp,rnd_mode);
    return x;
}

inline int sin_cos(mpfi_obj& s, mpfi_obj& c, const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return mpfr_sin_cos(s.mp,c.mp,v.mp,rnd_mode);
}

inline const mpfi_obj acos (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_acos(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj asin (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_asin(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj atan (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_atan(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj acot (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return atan(1/v, rnd_mode);
}

inline const mpfi_obj asec (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return acos(1/v, rnd_mode);
}

inline const mpfi_obj acsc (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return asin(1/v, rnd_mode);
}

inline const mpfi_obj acoth (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return atanh(1/v, rnd_mode);
}

inline const mpfi_obj asech (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return acosh(1/v, rnd_mode);
}

inline const mpfi_obj acsch (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return asinh(1/v, rnd_mode);
}

inline const mpfi_obj atan2 (const mpfi_obj& y, const mpfi_obj& x, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t yp, xp;

    yp = y.get_prec(); 
    xp = x.get_prec(); 

    a.set_prec(yp>xp?yp:xp);

    mpfr_atan2(a.mp, y.mp, x.mp, rnd_mode);

    return a;
}

inline const mpfi_obj cosh (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_cosh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj sinh (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_sinh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj tanh (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_tanh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj sech (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_sech(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj csch (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_csch(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj coth (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_coth(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj acosh  (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_acosh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj asinh  (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_asinh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj atanh  (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_atanh(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj hypot (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t yp, xp;

    yp = y.get_prec(); 
    xp = x.get_prec(); 

    a.set_prec(yp>xp?yp:xp);

    mpfr_hypot(a.mp, x.mp, y.mp, rnd_mode);

    return a;
}

inline const mpfi_obj remainder (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{    
    mpfi_obj a;
    mp_prec_t yp, xp;

    yp = y.get_prec(); 
    xp = x.get_prec(); 

    a.set_prec(yp>xp?yp:xp);

    mpfr_remainder(a.mp, x.mp, y.mp, rnd_mode);

    return a;
}

inline const mpfi_obj remquo (long* q, const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t yp, xp;

    yp = y.get_prec(); 
    xp = x.get_prec(); 

    a.set_prec(yp>xp?yp:xp);

    mpfr_remquo(a.mp,q, x.mp, y.mp, rnd_mode);

    return a;
}

inline const mpfi_obj fac_ui (unsigned long int v, mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x(0,prec);
    mpfr_fac_ui(x.mp,v,rnd_mode);
    return x;
}



inline const mpfi_obj eint   (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_eint(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj gamma (const mpfi_obj& x, mp_rnd_t rnd_mode)
{
    mpfi_obj FunctionValue(x);

    // x < 0: gamma(-x) = -pi/(x * gamma(x) * sin(pi*x))

    mpfr_gamma(FunctionValue.mp, x.mp, rnd_mode);

    return FunctionValue;
}

inline const mpfi_obj lngamma (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_lngamma(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj lgamma (const mpfi_obj& v, int *signp, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    int tsignp;

    if(signp)
        mpfr_lgamma(x.mp,signp,v.mp,rnd_mode);
    else
        mpfr_lgamma(x.mp,&tsignp,v.mp,rnd_mode);

    return x;
}

inline const mpfi_obj zeta (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_zeta(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj erf (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_erf(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj erfc (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_erfc(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj besselj0 (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_j0(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj besselj1 (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_j1(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj besseljn (long n, const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_jn(x.mp,n,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj bessely0 (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_y0(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj bessely1 (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_y1(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj besselyn (long n, const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_yn(x.mp,n,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj fma (const mpfi_obj& v1, const mpfi_obj& v2, const mpfi_obj& v3, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t p1, p2, p3;

    p1 = v1.get_prec(); 
    p2 = v2.get_prec(); 
    p3 = v3.get_prec(); 

    a.set_prec(p3>p2?(p3>p1?p3:p1):(p2>p1?p2:p1));

    mpfr_fma(a.mp,v1.mp,v2.mp,v3.mp,rnd_mode);
    return a;
}

inline const mpfi_obj fms (const mpfi_obj& v1, const mpfi_obj& v2, const mpfi_obj& v3, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t p1, p2, p3;

    p1 = v1.get_prec(); 
    p2 = v2.get_prec(); 
    p3 = v3.get_prec(); 

    a.set_prec(p3>p2?(p3>p1?p3:p1):(p2>p1?p2:p1));

    mpfr_fms(a.mp,v1.mp,v2.mp,v3.mp,rnd_mode);
    return a;
}

inline const mpfi_obj agm (const mpfi_obj& v1, const mpfi_obj& v2, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t p1, p2;

    p1 = v1.get_prec(); 
    p2 = v2.get_prec(); 

    a.set_prec(p1>p2?p1:p2);

    mpfr_agm(a.mp, v1.mp, v2.mp, rnd_mode);

    return a;
}

inline const mpfi_obj sum (const mpfi_obj tab[], unsigned long int n, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    mpfr_ptr* t;
    unsigned long int i;

    t = new mpfr_ptr[n];
    for (i=0;i<n;i++) t[i] = (mpfr_ptr)tab[i].mp;
    mpfr_sum(x.mp,t,n,rnd_mode);
    delete[] t;
    return x;
}

//////////////////////////////////////////////////////////////////////////
// MPFR 2.4.0 Specifics
#if (MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0))

inline int sinh_cosh(mpfi_obj& s, mpfi_obj& c, const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    return mpfr_sinh_cosh(s.mp,c.mp,v.mp,rnd_mode);
}

inline const mpfi_obj li2(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_li2(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj rem (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    /*  R = rem(X,Y) if Y != 0, returns X - n * Y where n = trunc(X/Y). */
    return fmod(x, y, rnd_mode);
}

inline const mpfi_obj mod (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    (void)rnd_mode;
    
    /*  

    m = mod(x,y) if y != 0, returns x - n*y where n = floor(x/y)

    The following are true by convention:
    - mod(x,0) is x
    - mod(x,x) is 0
    - mod(x,y) for x != y and y != 0 has the same sign as y.    
    
    */

    if(iszero(y)) return x;
    if(x == y) return 0;

    mpfi_obj m = x - floor(x / y) * y;
    
    m.setSign(sgn(y)); // make sure result has the same sign as Y

    return m;
}

inline const mpfi_obj fmod (const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mp_prec_t yp, xp;

    yp = y.get_prec(); 
    xp = x.get_prec(); 

    a.set_prec(yp>xp?yp:xp);

    mpfr_fmod(a.mp, x.mp, y.mp, rnd_mode);

    return a;
}

inline const mpfi_obj rec_sqrt(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rec_sqrt(x.mp,v.mp,rnd_mode);
    return x;
}
#endif //  MPFR 2.4.0 Specifics

//////////////////////////////////////////////////////////////////////////
// MPFR 3.0.0 Specifics
#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))

inline const mpfi_obj digamma(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_digamma(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj ai(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_ai(x.mp,v.mp,rnd_mode);
    return x;
}

#endif // MPFR 3.0.0 Specifics

//////////////////////////////////////////////////////////////////////////
// Constants
inline const mpfi_obj const_log2 (mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    x.set_prec(prec);
    mpfr_const_log2(x.mp,rnd_mode);
    return x;
}

inline const mpfi_obj const_pi (mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    x.set_prec(prec);
    mpfr_const_pi(x.mp,rnd_mode);
    return x;
}

inline const mpfi_obj const_euler (mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    x.set_prec(prec);
    mpfr_const_euler(x.mp,rnd_mode);
    return x;
}

inline const mpfi_obj const_catalan (mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    x.set_prec(prec);
    mpfr_const_catalan(x.mp,rnd_mode);
    return x;
}

inline const mpfi_obj const_infinity (int sign, mp_prec_t prec, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    x.set_prec(prec,rnd_mode);
    mpfr_set_inf(x.mp, sign);
    return x;
}

//////////////////////////////////////////////////////////////////////////
// Integer Related Functions
inline const mpfi_obj rint(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rint(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj ceil(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfr_ceil(x.mp,v.mp);
    return x;

}

inline const mpfi_obj floor(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfr_floor(x.mp,v.mp);
    return x;
}

inline const mpfi_obj round(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfr_round(x.mp,v.mp);
    return x;
}

inline const mpfi_obj trunc(const mpfi_obj& v)
{
    mpfi_obj x(v);
    mpfr_trunc(x.mp,v.mp);
    return x;
}

inline const mpfi_obj rint_ceil (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rint_ceil(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj rint_floor(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rint_floor(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj rint_round(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rint_round(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj rint_trunc(const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_rint_trunc(x.mp,v.mp,rnd_mode);
    return x;
}

inline const mpfi_obj frac (const mpfi_obj& v, mp_rnd_t rnd_mode)
{
    mpfi_obj x(v);
    mpfr_frac(x.mp,v.mp,rnd_mode);
    return x;
}

//////////////////////////////////////////////////////////////////////////
// Miscellaneous Functions
inline void swap(mpfi_obj& a, mpfi_obj& b) 
{
    mpfr_swap(a.mp,b.mp);
}

inline const mpfi_obj (max)(const mpfi_obj& x, const mpfi_obj& y)
{
    return (x>y?x:y);
}

inline const mpfi_obj (min)(const mpfi_obj& x, const mpfi_obj& y)
{
    return (x<y?x:y);
}

inline const mpfi_obj fmax(const mpfi_obj& x, const mpfi_obj& y, mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mpfr_max(a.mp,x.mp,y.mp,rnd_mode);
    return a;
}

inline const mpfi_obj fmin(const mpfi_obj& x, const mpfi_obj& y,  mp_rnd_t rnd_mode)
{
    mpfi_obj a;
    mpfr_min(a.mp,x.mp,y.mp,rnd_mode);
    return a;
}

inline const mpfi_obj nexttoward (const mpfi_obj& x, const mpfi_obj& y)
{
    mpfi_obj a(x);
    mpfr_nexttoward(a.mp,y.mp);
    return a;
}

inline const mpfi_obj nextabove  (const mpfi_obj& x)
{
    mpfi_obj a(x);
    mpfr_nextabove(a.mp);
    return a;
}

inline const mpfi_obj nextbelow  (const mpfi_obj& x)
{
    mpfi_obj a(x);
    mpfr_nextbelow(a.mp);
    return a;
}

inline const mpfi_obj urandomb (gmp_randstate_t& state)
{
    mpfi_obj x;
    mpfr_urandomb(x.mp,state);
    return x;
}

#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
// use gmp_randinit_default() to init state, gmp_randclear() to clear
inline const mpfi_obj urandom (gmp_randstate_t& state, mp_rnd_t rnd_mode)
{
    mpfi_obj x;
    mpfr_urandom(x.mp,state,rnd_mode);
    return x;
}
#endif 

#if (MPFR_VERSION <= MPFR_VERSION_NUM(2,4,2))
inline const mpfi_obj random2 (mp_size_t size, mp_exp_t exp)
{
    mpfi_obj x;
    mpfr_random2(x.mp,size,exp);
    return x;
}
#endif

// Uniformly distributed random number generation
// a = random(seed); <- initialization & first random number generation
// a = random();     <- next random numbers generation
// seed != 0
inline const mpfi_obj random(unsigned int seed)
{

#if (MPFR_VERSION >= MPFR_VERSION_NUM(3,0,0))
    static gmp_randstate_t state;
    static bool isFirstTime = true;

    if(isFirstTime)
    {
        gmp_randinit_default(state);
        gmp_randseed_ui(state,0);
        isFirstTime = false;
    }

    if(seed != 0)    gmp_randseed_ui(state,seed);

    return mpfr::urandom(state);
#else
    if(seed != 0)    std::srand(seed);
    return mpfr::mpfi_obj(std::rand()/(double)RAND_MAX);
#endif

}

//////////////////////////////////////////////////////////////////////////
// Set/Get global properties
inline void mpfi_obj::set_default_prec(mp_prec_t prec)
{ 
    mpfr_set_default_prec(prec); 
}

inline void mpfi_obj::set_default_rnd(mp_rnd_t rnd_mode)
{ 
    mpfr_set_default_rounding_mode(rnd_mode); 
}

inline bool mpfi_obj::fits_in_bits(double x, int n)
{   
    int i;
    double t;
    return IsInf(x) || (std::modf ( std::ldexp ( std::frexp ( x, &i ), n ), &t ) == 0.0);
}


} // End of mpfr namespace

// Explicit specialization of std::swap for mpfi_obj numbers
// Thus standard algorithms will use efficient version of swap (due to Koenig lookup)
// Non-throwing swap C++ idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap
namespace std
{
	// only allowed to extend namespace std with specializations
    template <>
    inline void swap(mpfr::mpfi_obj& x, mpfr::mpfi_obj& y) 
    { 
        return mpfr::swap(x, y); 
    }

    template<>
    class numeric_limits<mpfr::mpfi_obj>
    {
    public:
        static const bool is_specialized    = true;
        static const bool is_signed         = true;
        static const bool is_integer        = false;
        static const bool is_exact          = false;
        static const int  radix             = 2;    

        static const bool has_infinity      = true;
        static const bool has_quiet_NaN     = true;
        static const bool has_signaling_NaN = true;

        static const bool is_iec559         = true;        // = IEEE 754
        static const bool is_bounded        = true;
        static const bool is_modulo         = false;
        static const bool traps             = true;
        static const bool tinyness_before   = true;

        static const float_denorm_style has_denorm  = denorm_absent;
        
        inline static float_round_style round_style()
        {
            mp_rnd_t r = mpfr::mpfi_obj::get_default_rnd();

            switch (r)
            {
                case MPFR_RNDN: return round_to_nearest;
                case MPFR_RNDZ: return round_toward_zero; 
                case MPFR_RNDU: return round_toward_infinity; 
                case MPFR_RNDD: return round_toward_neg_infinity; 
                default: return round_indeterminate;
            }
        }

        inline static mpfr::mpfi_obj (min)    (mp_prec_t precision = mpfr::mpfi_obj::get_default_prec()) {  return  mpfr::minval(precision);  }
        inline static mpfr::mpfi_obj (max)    (mp_prec_t precision = mpfr::mpfi_obj::get_default_prec()) {  return  mpfr::maxval(precision);  }
        inline static mpfr::mpfi_obj lowest   (mp_prec_t precision = mpfr::mpfi_obj::get_default_prec()) {  return -mpfr::maxval(precision);  }

        // Returns smallest eps such that 1 + eps != 1 (classic machine epsilon)
        inline static mpfr::mpfi_obj epsilon(mp_prec_t precision = mpfr::mpfi_obj::get_default_prec()) {  return  mpfr::machine_epsilon(precision); }
		
        // Returns smallest eps such that x + eps != x (relative machine epsilon)
	inline static mpfr::mpfi_obj epsilon(const mpfr::mpfi_obj& x) {  return mpfr::machine_epsilon(x);  }

        inline static mpfr::mpfi_obj round_error(mp_prec_t precision = mpfr::mpfi_obj::get_default_prec())
        {
            mp_rnd_t r = mpfr::mpfi_obj::get_default_rnd();

            if(r == MPFR_RNDN) return mpfr::mpfi_obj(0.5, precision); 
            else               return mpfr::mpfi_obj(1.0, precision);    
        }

        inline static const mpfr::mpfi_obj infinity()         { return mpfr::const_infinity();     }
        inline static const mpfr::mpfi_obj quiet_NaN()        { return mpfr::mpfi_obj().setNan();    }
        inline static const mpfr::mpfi_obj signaling_NaN()    { return mpfr::mpfi_obj().setNan();    }
        inline static const mpfr::mpfi_obj denorm_min()       { return (min)();                    }

        // Please note, exponent range is not fixed in MPFR
        static const int min_exponent = MPFR_EMIN_DEFAULT;
        static const int max_exponent = MPFR_EMAX_DEFAULT;
        static const int min_exponent10 = (int) (MPFR_EMIN_DEFAULT * 0.3010299956639811); 
        static const int max_exponent10 = (int) (MPFR_EMAX_DEFAULT * 0.3010299956639811); 

        // Should be constant according to standard, but 'digits' depends on precision in MPFR

        inline static int digits()                        {    return mpfr::mpfi_obj::get_default_prec();    }
        inline static int digits(const mpfr::mpfi_obj& x)   {    return x.getPrecision();                    }

        inline static int digits10(mp_prec_t precision = mpfr::mpfi_obj::get_default_prec())
        {
            return mpfr::bits2digits(precision);
        }

        inline static int digits10(const mpfr::mpfi_obj& x)
        {
            return mpfr::bits2digits(x.getPrecision());
        }

        inline static int max_digits10(mp_prec_t precision = mpfr::mpfi_obj::get_default_prec())
        {
            return digits10(precision);
        }
    };

}

#endif /* __OBJECTIVEMPFI_H__ */
