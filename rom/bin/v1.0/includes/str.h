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

/* str.h  -- header for string library */

#ifndef STR_H
#define STR_H

#include    "libhdr.h"
#include    <stddef.h>
#include    <string.h>
#include    <limits.h>

#ifdef __CPLUSPLUS__
extern "C" {
#endif

#if _I386
    #define SLEN(s) strlen(s)
#else
    #define SLEN(s) (SEND(s)-(char *)s)
#endif

#define SEND(s) (char *)memchr(s,'\0',INT_MAX)

/* Unicode definitions and functions */

#ifdef _IS_UNICODE
    #define _W(name) _u ## name     // prefix name with _u
    #define __W(name) _u ## name    // prefix name with _u
    #define ___W(name) __u ## name  // prefix name with __u
    #define _WCHAR _unichar     // "default" char type is 16 bits
    #define _WIDE(x) L ## x     // used by _WS and _WC
    #define _WS(s) _WIDE(s)     // a "wide" character string
    #define _WC(c) _WIDE(c)     // a "wide" character
#else
    #define _W(name) name       // don't prefix the name
    #define __W(name) _ ## name     // prefix name with _
    #define ___W(name) __ ## name   // prefix name with __
    #define _WCHAR char         // "default" char type is 8 bits
    #define _WS(s) s            // a string of 8-bit characters
    #define _WC(c) c            // an 8-bit character
#endif

#if _I386
    #define _uSLEN(s) _ustrlen(s)
#else
    #define _uSLEN(s) (_uSEND(s)-(_unichar *)s)
#endif

#define _uSEND(s)       _uchrchr(s,'\0',INT_MAX)


#ifdef __CPLUSPLUS__
}
#endif

#endif
