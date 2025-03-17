/*
 * $Log: cnv.h,v $
 * Revision 1.13  1999/05/07 18:29:15  richf
 * more long-long functions
 *
 * Revision 1.12  1999/05/06  22:49:23  richf
 * long-long functions for string-to-long-long conversions
 *
 */
/*********************************************************************
(C) Copyright 2002      ARC International (ARC);  Santa Cruz, CA 95060
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
/*
 *   cnv.h -- internal header for the ANSI library.
 *
 *   Integer and floating-point conversions
 *
 */

/*
 * floating point conversions
 */

#ifndef CNV_H
#define CNV_H

#ifdef __CPLUSPLUS__
extern "C" {
#endif

#ifndef _UNICHAR_DEFINED
    typedef unsigned short _unichar;
    #define _UNICHAR _unichar
    #define _UNICHAR_DEFINED
#endif

#if _HOBBIT || _I387
    /* Does this machine support long doubles that are fast enough
     * to make plain doubles unnecessary?
     */
    #define _FAST_LONG_DOUBLE 1
#endif

extern  int             _fpexpo(long double *d);
extern  int             _fpexpod(double *d);
extern  const char  *_fpecvt(long double *d, int xtype, int ndigit,
                int *dcpt, int *sgn);
extern  const char  *_fpecvtd(double *d, int xtype, int ndigit,
                int *dcpt, int *sgn);
extern  long double     _pow10(int p);
extern  double      _pow10d(int p);
extern  long double     _round[];
extern  double      _roundd[];
extern  long double     _doatof(const char *nptr, char **endptr, int *err);
extern  double      _doatofd(const char *nptr, char **endptr, int *err);
extern  char            *_doecvt(char *buf, int bufsize,
                long double d, int xtype, int ndigit,
                                int *dcpt, int *sgn);
extern  char            *_doecvtd(char *buf, int bufsize, double d, int xtype,
                int ndigit, int *dcpt, int *sgn);
/*
 * integer conversions
 */

extern  unsigned long   _cnvs2n(const char *a,int base, char **endptr,int *err);
extern  unsigned long   _cnvs2u(const char *a, char **e, int *err);
extern  long            _cnvs2i(const char *a, char **e, int *err);

extern  void            *_cnvs2p(const char *a, char **endptr, int *err);

extern  char            *_cnvn2s(char *buf, int base,unsigned long u,
                            int *cnt, int low);
extern  char            *_cnvu2s(char *buf, unsigned long u, int *cnt);
extern  char            *_cnvi2s(char *buf, long i, int *cnt);
extern  char            *_cnvp2s(char *buf, void *p, int *cnt);
extern  unsigned long   _cnvstrtoul(const char *nptr, char **endptr, int base, int *errno_val);

#ifdef _HAS_LONG_LONG
extern  unsigned long long _cnvstrtoull(const char *nptr, char **endptr, int base, int *errno_val);
extern  unsigned long long _cnvs2nll(const char *a,int base, char **endptr,int *err);
extern  unsigned long long _cnvs2ull(const char *a, char **e, int *err);
extern  long long          _cnvs2ll(const char *a, char **e, int *err);

extern  unsigned long long _ucnvstrtoull(const _unichar *nptr, _unichar **endptr,int base, int *errno_val);
extern  unsigned long long _ucnvs2nll(const _unichar *a, int base, _unichar **endptr,int *err);
extern  unsigned long long _ucnvs2ull(const _unichar *a, _unichar **e, int *err);
extern  long long          _ucnvs2ll(const _unichar *a, _unichar **e, int *err);

extern  char *_cnvll2s(char *buf,int base,unsigned long long u,int *cnt,int low);
extern  char *_cnvull2s(char *buf, unsigned long long u, int *cnt);
extern  _unichar *_ucnvll2s(_unichar *buf, int base,unsigned long long u, int *cnt, int low);
extern  _unichar *_ucnvull2s(_unichar *buf, unsigned long long u,int *cnt);
#endif

/* Unicode conversions */

extern  _unichar       *_ufpecvt(long double *d, int xtype, int ndigit,
                int *dcpt, int *sgn);
extern  _unichar       *_ufpecvtd(double *d, int xtype, int ndigit,
                int *dcpt, int *sgn);
extern  long double     _udoatof(const _unichar *nptr, _unichar **endptr,
                int *err);
extern  double      _udoatofd(const _unichar *nptr, _unichar **endptr,
                int *err);
extern  _unichar       *_udoecvt(_unichar *buf, int bufsize,
                long double d, int xtype, int ndigit,
                                int *dcpt, int *sgn);
extern  _unichar       *_udoecvtd(_unichar *buf, int bufsize,
                double d, int xtype, int ndigit,
                                int *dcpt, int *sgn);

extern  unsigned long   _ucnvs2n(const _unichar *a, int base, _unichar **endptr,int *err);
extern  unsigned long   _ucnvs2u(const _unichar *a, _unichar **e, int *err);
extern  long            _ucnvs2i(const _unichar *a, _unichar **e, int *err);
extern  unsigned long   _ucnvstrtoul(const _unichar *nptr, _unichar **endptr,int base, int *errno_val);
extern  void           *_ucnvs2p(const char *a, char **endptr, int *err);

extern  _unichar       *_ucnvn2s(_unichar *buf, int base,unsigned long u,
                            int *cnt, int low);
extern  _unichar       *_ucnvu2s(_unichar *buf, unsigned long u, int *cnt);
extern  _unichar       *_ucnvi2s(_unichar *buf, long i, int *cnt);
extern  _unichar       *_ucnvp2s(_unichar *buf, void *p, int *cnt);

#if __HIGHC_DSPC__

extern char     * _doecvtsr(char*,    int bufsize, _Fract, int ndg, int *dcp, int *sgn);
extern _unichar *_udoecvtsr(_unichar*,int bufsize, _Fract, int ndg, int *dcp, int *sgn);

extern char     * _doecvtur(char*,    int bufsize, unsigned _Fract, int ndg, int *dcp, int *sgn);
extern _unichar *_udoecvtur(_unichar*,int bufsize, unsigned _Fract, int ndg, int *dcp, int *sgn);

#endif /*__HIGHC_DSPC__*/

#ifdef __CPLUSPLUS__
}
#endif

#endif
