/*********************************************************************
(C) Copyright 2002-2004 ARC International (ARC);  Santa Cruz, CA 95060
This program is the unpublished property and trade secret of ARC.   It
is to be  utilized  solely  under  license  from  ARC  and it is to be
maintained on a confidential basis for internal company use only.  The
security  and  protection  of  the program is paramount to maintenance
of the trade secret status.  It is to  be  protected  from  disclosure
to unauthorized parties, both within the Licensee company and outside,
in a manner  not less stringent than  that utilized for Licensee's own
proprietary internal information.  No  copies of  the source or Object
Code are to leave the premises of Licensee's business except in strict
accordance with the license agreement signed by Licensee with ARC.
*********************************************************************/

#ifndef FP_H
#define FP_H

#if __DOUBLE4__
    /* Compiling for VideoCore with -fdouble4 option set */
#   define _DOUBLE_IS_FLOAT 1
#endif


#include <libhdr.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <cnv.h>

#ifdef __CPLUSPLUS__
extern "C" {
#endif

/* normally struct _exception is defined in math.h, but may not be if not
   including the MetaWare math.h, as with IBM RTP (__OS_OPEN) for example */
#ifndef _EXCEPTION_DEFINED
    struct _exception {
    int type;           /* exception type - see constants below */
    char *name;         /* name of function where error occured */
    double arg1;        /* first argument to function           */
    double arg2;        /* second argument (if any) to function */
    double retval;      /* value to be returned by function     */
    } ;

    #define _EXCEPTION_DEFINED
    #if defined(__HIGHC__) || defined(__CCAC__)
    #if _NA_NAMES || _MSDOS
        #define exception _exception
    #endif
    #define DOMAIN      1   /* argument domain error                */
    #define SING        2   /* argument singularity                 */
    #define OVERFLOW    3   /* overflow range error                 */
    #define UNDERFLOW   4   /* underflow range error                */
    #define TLOSS       5   /* total loss of precision              */
    #define PLOSS       6   /* partial loss of precision            */
    #endif
#endif


#define _B              FLT_RADIX
#define _t              DBL_MANT_DIG
#define _tf             FLT_MANT_DIG
#define BINARY          (_B==2)
#define FIXED_POINT     0

#if _SOL || _UPA || _PPC
    #define _LONG_DOUBLE_IS_FAST    0
#else
    #define _LONG_DOUBLE_IS_FAST    1
#endif

#ifndef _CRTIMP
    #if _MSNT && _DLL
    #define _CRTIMP __declspec(dllimport)
    #else
    #define _CRTIMP
    #endif
#endif


/* float */
#define _bf             24                      /* 24 bits in mant */
#define _kf             1                       /* 2**k == _B */
#define _ksf            0                       /* shf for *_k */
#define GUARD_DIGITf    0

/* double */
#if _DOUBLE_IS_FLOAT
    #define _b              _bf
    #define _k              _kf
    #define _ks             _ksf
    #define GUARD_DIGIT     GUARD_DIGITf
#else
    #define _b              53                      /* 53 bits in mant */
    #define _k              1                       /* 2**k == _B */
    #define _ks             0                       /* shf for *_k */
    #define GUARD_DIGIT     0
#endif


/*****************************
 *
 * Configure for sharing source between single
 * and double-precision math.h functions
 *
 *****************************/
#if _DOING_FLOAT
    /* _DOING_FLOAT is set by various files such as "sqrtf.c" to indicate
       that we are implementing the "float" version of the math function.
    */
    typedef float float_type;
    #define FPGETX _fpgetxf
    #define FPSETX _fpsetxf
    #define FPADDX _fpaddxf
    #define FPTRN _fptrnf
    #define FPRND _fprndf
    #define FPDOM _fpdomf
    #define FPOVF _fpovff
    #define FPUDF _fpudff
    #define FPEVAL _fpevalf
    #define FPODD _fpoddf
    #define _SINGLE 1
    #if __HIGHC__
    #pragma off(double_constants);
    #endif
#else
    typedef double float_type;
    #define FPGETX _fpgetx
    #define FPSETX _fpsetx
    #define FPTRN _fptrn
    #define FPRND _fprnd
    #define FPADDX _fpaddx
    #define FPDOM _fpdom
    #define FPOVF _fpovf
    #define FPUDF _fpudf
    #define FPEVAL _fpeval
    #define FPODD _fpodd

    #if _DOUBLE_IS_FLOAT
    #define _SINGLE 1
    #else
    #define _SINGLE 0
    #endif
#endif

/* macro to determine whether a floating point literal is single or double */
#if __HIGHC__
    /* double_constants is used to interpret as single when possible */
    #define _C(x) x
    #define FABS(x) _abs(x)
#elif _SINGLE
    #define _C(x) x ## f
    #define FABS(x) fabsf(x)
#else
    #define _C(x) x
    #define FABS(x) fabs(x)
#endif

/* support macros */

#include <lsbmsb.h>

/****************/
/*              */
/* float macros */
/*              */
/****************/

#define _fi(f)              *(unsigned long *)&f

#define REDUCEf(f)           (_fptrnf(_C(16.0)*f)*_C(0.0625))
#define _fprndf(f)          _fptrnf((f) + ((f) < _C(0.0) ? _C(-0.5) : _C(0.5)))

#if _NAM
    #include <nam/rts-nam.h>
    /*
     *  Since _NAM has 64-bit words, the usual _fpgetx and _fpsetx macros
     *  will not work.
     */
    #define _lshiftf(i,j)   rts_lshift(i,j)
    #define _rshiftf(i,j)   rts_rshift(i,j)
    #define _fptrnf(f)      rts_fptrn(f)
    #define _fpsetxf(f,x)   (f = rts_setexp(f,x+0x7f))  /* unbias exponent first */
    #define _fpgetxf(f)     rts_getexp(f)
    #define reciprocalf(f)  rts_reciprocal(f)
#else
    #define _fpgetxf(f)     ((int)(((_fi(f) >> 23) & 0xff)-0x7e))
    #define _fpsetxf(f,x)   (_fi(f) = _fi(f) & 0x807FFFFF \
                |((((x)+0x7e) & 0xff) << 23))
#endif

#define _fpaddxf(f,x)       _fpsetxf(f, _fpgetxf(f) +x)

typedef union _fu {
    float f;
    struct {
#if LSB_FIRST
    unsigned u:23;  /* used to be :20 (MJMauro) */
    unsigned x:8;   /* used to be :11 (MJMauro) */
    unsigned s:1;
#else
    unsigned s:1;
    unsigned x:8;
    unsigned u:23;
#endif
    } s;
    } FU;

#define EXPOf   s.x
#define Uf      s.u

/*******************/
/*                 */
/* float constants */
/*                 */
/*******************/

/* note that for these float calculations "t" is 24, and t/2 is thus 12 */

#define EPSf            2.44140625e-4f    /* B**(-t/2) */
#define EPSXf               2.9802322388e-8f  /* (B**-t)/2 */
#define BIGXf               8.8722839052e1f   /* ~ln(FLT_MAX) */
#define SMALLXf            -8.8733654475e1f  /* ~ln(FLT_MIN) */
#define LOG10_OF_Ef         _fpl10ef[0]              /* log10(e) */
#define SQRT_1_OVER_2f      _fpsq1o2f[0]      /* sqrt(1/2)  */
#define SQRT_1_OVER_Bf      SQRT_1_OVER_2f    /* sqrt(1/B) */
#define PI_OVER_2f          _fppio2f[0]              /* pi/2 */
#define PI_OVER_3f          _fppio3f[0]              /* pi/3 */
#define PI_OVER_4f          _fppio4f[0]              /* pi/4 */
#define PI_OVER_6f          _fppio6f[0]              /* pi/6 */
#define PI_OVER_180f        1.7453292519943295e-2f   /* pi / 180 */
#define _180_OVER_PIf       5.7295779513082323e+1f   /* 180 / pi */
#define _2_OVER_PIf         6.3661977236758137e-1f  /* 2 / pi */
#define _1_OVER_PIf         0.31830988618379f       /* 1 / pi */
//    #define PI_OVER_2f          1.5707963267949f        /* pi / 2 */
#define PIf             3.14159265358979323846f /* pi */
#define HEX_1_DOT_0f        0x3f800000              /* 1.0 */

/*****************/
/*               */
/* double macros */
/*               */
/*****************/

#define REDUCE(d)           (_fptrn(16.0*d)*0.0625)
#if LSB_FIRST && _DOUBLE_IS_FLOAT
    #define _di(d)          *(unsigned long *)&d
#elif LSB_FIRST
    #define _di(d)          *(((unsigned long *)&d)+1)
#else
    #define _di(d)          *(unsigned long *)&d
#endif

#define _fprnd(d)           _fptrn((d) + ((d) < 0.0 ? -0.5 : 0.5))

#if _DOUBLE_IS_FLOAT
    #define _fpsetx(d,x)    _fpsetxf(d,x)
    #define _fpgetx(d)  _fpgetxf(d)
#else
    #define _fpgetx(d)      ((int)(((_di(d) >> 20) & 0x7ff) - 0x3fe))
    #define _fpsetx(d,x)    (_di(d)=_di(d)&0x800FFFFF \
                |((((x)+0x3fe)&0x7ffL)<<20))
#endif

#define _fpaddx(d,x)        _fpsetx(d,_fpgetx(d)+x)

typedef union _du {
    double d;
    struct {
#if LSB_FIRST && _DOUBLE_IS_FLOAT
    unsigned u1:23;
    unsigned x:8;
    unsigned s:1;
#elif LSB_FIRST
    unsigned u2;
    unsigned u1:20;
    unsigned x:11;
    unsigned s:1;
#else
    unsigned s:1;
    unsigned x:11;
    unsigned u1:20;
    unsigned u2;
#endif
    } s;
    } DU;

#define EXPO    s.x
#define U1      s.u1
#define U2      s.u2

/********************/
/*                  */
/* double constants */
/*                  */
/********************/

/* general constants */
/* for these double calculations "t" is 53, and t/2 is thus 26.5 */
#define EPS             5.551115123125782e-17   /* B**(-t/2) */
#define EPSX            1.053671212772334e-08   /* (B**-t)/2 */
#define BIGX            7.097827128933832e+02   /* ~ln(DBL_MAX) */
#define SMALLX          -7.083964185322740e+02  /* ~ln(DBL_MIN) */
#define SQRT_1_OVER_B   SQRT_1_OVER_2           /* sqrt(1/B) */
#define SQRT_1_OVER_2   _fpsq1o2[0]             /* sqrt(1/2)  */
#define LOG10_OF_E      _fpl10e[0]              /* log10(e) */
#define PI_OVER_2       _fppio2[0]              /* pi/2 */
#define PI_OVER_3       _fppio3[0]              /* pi/3 */
#define PI_OVER_4       _fppio4[0]              /* pi/4 */
#define PI_OVER_6       _fppio6[0]              /* pi/6 */
#define PI_OVER_180     1.7453292519943295e-2   /* pi / 180 */
#define _180_OVER_PI    5.7295779513082323e+1   /* 180 / pi */
#define _2_OVER_PI      6.3661977236758137e-1   /* 2 / pi */
#define _1_OVER_PI      0.31830988618379067154  /* 1 / pi */
#define PI              3.14159265358979323846  /* pi */

/* pow */
#define POWMAX           16383                  /* 16*log2(MAX)-1 */
#define POWMIN          -16335                  /* 16*log2(MIN)+1 */
#define POWMAXf          2047                   /* 16*log2(MAX)-1 */
#define POWMINf         -1999                   /* 16*log2(MIN)+1 */
#define POWA1(p)        _fpa1pow[p]             /* powers of 1/16 */
#define POWA2(p)        _fpa2pow[p]             /* adjusters */
#define POWA1f(p)       _fpa1powf[p]            /* powers of 1/16 */
#define POWA2f(p)       _fpa2powf[p]            /* adjusters */

/* exp/log */
#define EXPC1           0.693359375             /* 355/512 */
#define EXPC1f      _fpexpc1f[0]            /* 355/512 exactly */
#define LOGC1       EXPC1
#define LOGC1f      EXPC1f

/* sin/cos */
#define SINMAX          2.981568268647692e+08   /* pi*(B**(t/2)) */
#define SINMAXf     1.28679635e4f           /* pi*(B**(t/2)) */
#define SINC1           3.1416015625            /* 3217/1024 */
#define SINC1f          _fpsinc1f[0]            /* 201/64 */


/* tan */
#define TANMAX          1.490784134323849e+08   /* B**(t/2)*pi/2 */
#define TANMAXf         6.433981755e+03f        /* B**(t/2)*pi/2 */
#define TANC1           1.57080078125           /* 3217/2048 */
#define TANC1f          _fptanc1f[0]            /* 201/128  exactly */

/* atan */
#define ATANC1          _fp2msq3[0]             /* 2-sqrt(3) */
#define ATANC2          _fpsqrt3[0]             /* sqrt(3) */
#define ATANC1f         _fp2msq3f[0]            /* 2-sqrt(3) */
#define ATANC2f         _fpsqrt3f[0]            /* sqrt(3) */

/* sinh/cosh */
#define SINHMAX         7.097795518826411e+02   /* BIGX-ln(v)+0.69 */
#define SINHMAXf        8.871967803e+1f         /* BIGXf-ln(v)+0.69 */
#define SINHBAR         18.9                    /* 0.35*(b+1) */
#define SINHBARf        8.75                    /* 0.35*(b+1) */
#define SINH1           _fplnv[0]               /* ln(v) */
#define SINH1f          _fplnvf[0]              /* ln(v) */

/* tanh */
#define TANHMAX         1.906154746539838e+01   /*(ln(2)+(t+1)ln(B))/2*/
#define TANHMAXf        9.010913347f            /*(ln(2)+(t+1)ln(B))/2*/

#if _NAM
    extern dualpar int  rts_lshift(int, int);
    extern dualpar int  rts_rshift(int, int);
    extern parpar  double   rts_fptrn(double);
    extern parpar  double   rts_setexp(double, int);
    extern parpar  int  rts_getexp(double);
    extern parpar  double   rts_reciprocal(double);
#else
    extern double _fptrn(double d);
    extern float _fptrnf(float f);
#endif

/* Cody functions ... adjust original primitives for hidden bit */
#define INTXP(d)        (_fpgetx(d))
#define INTXPf(f)       (_fpgetxf(f))
#define SETXP(d,n)      _fpsetx(d,)
#define SETXPf(f,n)     _fpsetxf(f,n)
#define ADX(d,n)        _fpaddx(d,n)
#define ADXf(f,n)       _fpaddxf(f,n)
#define AINTRND(d)      _fprnd(d)
#define AINTRNDf(f)     _fprndf(f)
#define INTRND(d)       ((long int)AINTRND(d))
#define INTRNDf(f)      ((long int)AINTRNDf(f))
#define AINT(d)     _fptrn(d)
#define AINTf(f)        _fptrnf(f)

/*****************************/
/*                           */
/* double constants, fpcon.c */
/*                           */
/*****************************/
extern  double  _fpl10e[];              /* log10(e)     */
extern  double          _fpsq1o2[];             /* sqrt(1/2)    */
extern  double          _fppio2[];              /* pi/2         */
extern  double          _fppio3[];              /* pi/3         */
extern  double          _fppio4[];              /* pi/4         */
extern  double          _fppio6[];              /* pi/6         */
extern  double          _fpsqrt3[];             /* sqrt(3)      */
extern  double          _fp2msq3[];             /* 2-sqrt(3)    */
extern  double          _fplnv[];               /* ln(v)        */
extern  double          _fpa1pow[];             /* array for pow*/
extern  double          _fpa2pow[];             /* array 2, pow */

/****************************/
/*                          */
/* float constants, fpcon.c */
/*                          */
/****************************/
extern  float       _fpl10ef[];              /* log10(e)     */
extern  float           _fpsq1o2f[];             /* sqrt(1/2)    */
extern  float           _fppio2f[];              /* pi/2         */
extern  float           _fppio3f[];              /* pi/3         */
extern  float           _fppio4f[];              /* pi/4         */
extern  float           _fppio6f[];              /* pi/6         */
extern  float           _fpsqrt3f[];             /* sqrt(3)      */
extern  float           _fp2msq3f[];             /* 2-sqrt(3)    */
extern  float           _fplnvf[];               /* ln(v)        */
extern  float           _fpa1powf[];             /* array for pow*/
extern  float           _fpa2powf[];             /* array 2, pow */
extern  float           _fptanc1f[];
extern  float           _fpsinc1f[];
extern  float           _fpexpc1f[];


/**************************/
/*                        */
/* support routines, fp.c */
/*                        */
/**************************/
extern  double      _mwmatherr(int e_type, char *name,
                   double arg1, double arg2, double dflt_ret,
                   int nargs, int dflt_errno);

extern  float           _fpovff(char *name, int nargs, float arg1, float arg2);
extern  float           _fpudff(char *name, int nargs, float arg1, float arg2);
extern  float           _fpdomf(char *name, int nargs, float arg1, float arg2);

extern  double          _fpovf(char *name, int nargs, double arg1, double arg2);
extern  double          _fpudf(char *name, int nargs, double arg1, double arg2);
extern  double          _fpdom(char *name, int nargs, double arg1, double arg2);

extern  double          _fpeval(double g, double coef[], int cnt);
extern  float           _fpevalf(float g, float coef[], int cnt);

/*****************************/
/*                           */
/* support routines, float.c */
/*                           */
/*****************************/
extern  int             _fpodd(double d);   /* in float.c */
extern  int             _fpoddf(float f);   /* in float.c */

// Return  true if argument is 0, NaN, or INF
// Assumes IEEE
static _Inline int __is_zero_inf_nan(double d){
    unsigned l = (unsigned)(*(long long *)(void*)&d >> 32);
    l <<= 1;  // Shift out sign bit
    if (l == 0 && (unsigned)*(long long *)(void*)&d == 0)
    return 1;
    // At this point we know the number is not zero.
    // Now check if INF or NaN
    return l >= 0xFFE00000;
}
static _Inline int __is_inf_nan(double d){
    unsigned l = (unsigned)(*(long long *)(void*)&d >> 32);
    l <<= 1;  // Shift out sign bit
    return l >= 0xFFE00000;
}
static _Inline int __is_inf(double d){
    unsigned l = (unsigned)(*(long long *)(void*)&d >> 32);
    l <<= 1;  // Shift out sign bit
    return l == 0xFFE00000;
}

static _Inline int __is_zero_inf_nanf(float f){
    unsigned l = *(unsigned *)(void*)&f;
    l <<= 1;  // Shift out sign bit
    if (l == 0)
    return 1;
    return l >= 0xFF000000;
}
static _Inline int __is_inf_nanf(float f){
    unsigned l = *(unsigned *)(void*)&f;
    l <<= 1;  // Shift out sign bit
    return l >= 0xFF000000;
}
static _Inline int __is_inff(float f){
    unsigned l = *(unsigned *)(void*)&f;
    l <<= 1;  // Shift out sign bit
    return l == 0xFF000000;
}

// is the sign bit set (even if value is 0?)
static _Inline int __is_negative(double d){
    return *(long long *)(void*)&d < 0;
}

// is the sign bit set (even if value is 0?)
static _Inline int __is_negativef(float f){
    return *(int *)(void*)&f < 0;
}

#ifdef __CPLUSPLUS__
    }
#endif

// Equate funcl to func if double and long double are the same size
#if _ARC
    #define __stringify(a) #a
    #if __HIGHC__
    #define DEFINE_LONG_DOUBLE_FUNC(f) pragma Asm("\t.global\t" __stringify(f) "l\n\t" __stringify(f) "l=" __stringify(f) );
    #else
    #define DEFINE_LONG_DOUBLE_FUNC(f) asm("\t.global\t" __stringify(f) "l\n\t" __stringify(f) "l=" __stringify(f));
    #endif
#else
    #define DEFINE_LONG_DOUBLE_FUNC(f)
#endif

#endif /* FP_H */
