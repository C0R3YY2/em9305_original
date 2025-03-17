/*  (C) Copyright (c) 2002 ARC International;  Santa Cruz, CA 95060
 */

#ifndef LSBMSB_H
#define LSBMSB_H

#ifdef __CPLUSPLUS__
extern "C" {
#endif

/* Is this machine LSB or MSB? */
#if defined(i386) || (_I860 && !(_860_BE||_I860BE)) || _VAX || _I286 || _I960 || _SGL || _HOBBIT_L || _LE_PPC \
    || MIPSEL || _ARM_LE || _ARMVCP || _ARC_LE || _VIDEOCORE
#define LSB_FIRST 1
#else
#define LSB_FIRST 0
#endif

#define WORD(x) (*((unsigned long *)&x))

#if LSB_FIRST
/* DU_list is passed the most-siginficant 4 bytes, then the least. */
#    define DU_LIST(MS4,LS4) {LS4,MS4}
#    define HIWORD(x) (*((unsigned long *)&x+1))
#    define LOWORD(x) (*(unsigned long *)&x)
#else
#    define DU_LIST(MS4,LS4) {MS4,LS4}
#    define HIWORD(x) (*(unsigned long *)&x)
#  ifdef _NAM
#    define LOWORD(x) get_int_from_float_adr(&x)
#  else
#    define LOWORD(x) (*((unsigned long *)&x+1))
#  endif
#endif

#ifdef __CPLUSPLUS__
}
#endif

#endif
