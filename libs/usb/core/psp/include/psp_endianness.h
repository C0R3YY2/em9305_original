/***************************************************************************
 *
 *            Copyright (c) 2010-2019 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef PSP_ENDIANNESS_H
#define PSP_ENDIANNESS_H

#include "psp_types.h"

#include "../../version/ver_psp_endianness.h"
#if VER_PSP_ENDIANNESS_MAJOR != 1 || VER_PSP_ENDIANNESS_MINOR != 10
 #error Incompatible PSP_ENDIANNESS version number!
#endif

/* 79 S : MISRA-C:2004 19.4: Macro contains unacceptable items. */
/*LDRA_INSPECTED 79 S*/
#define PSP_RD_LE16 psp_rd_le16

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_LE24 psp_rd_le24

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_LE32 psp_rd_le32

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_LE48 psp_rd_le48

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_LE64 psp_rd_le64

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_LE16 psp_wr_le16

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_LE24 psp_wr_le24

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_LE32 psp_wr_le32

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_LE48 psp_wr_le48

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_LE64 psp_wr_le64

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_BE16 psp_rd_be16

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_BE24 psp_rd_be24

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_BE32 psp_rd_be32

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_BE48 psp_rd_be48

/*LDRA_INSPECTED 79 S*/
#define PSP_RD_BE64 psp_rd_be64

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_BE16 psp_wr_be16

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_BE24 psp_wr_be24

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_BE32 psp_wr_be32

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_BE48 psp_wr_be48

/*LDRA_INSPECTED 79 S*/
#define PSP_WR_BE64 psp_wr_be64

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
/* little endian read functions */
static inline uint16_t psp_rd_le16 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint16_t  tmp16_h1 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint16_t  tmp16_h2 = p[0];

  tmp16_h1 <<= 8;
  return tmp16_h1 | tmp16_h2;
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint32_t psp_rd_le24 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q1 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q2 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q3 = p[0];

  tmp32_q1 <<= 16;
  tmp32_q2 <<= 8;
  return tmp32_q1 | tmp32_q2 | tmp32_q3;
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint32_t psp_rd_le32 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q1 = p[3];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q2 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q3 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q4 = p[0];

  tmp32_q1 <<= 24;
  tmp32_q2 <<= 16;
  tmp32_q3 <<= 8;
  return tmp32_q1 | tmp32_q2 | tmp32_q3 | tmp32_q4;
} /* psp_rd_le32 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint64_t psp_rd_le48 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o1 = p[5];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o2 = p[4];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o3 = p[3];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o4 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o5 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o6 = p[0];

  tmp64_o1 <<= 40;
  tmp64_o2 <<= 32;
  tmp64_o3 <<= 24;
  tmp64_o4 <<= 16;
  tmp64_o5 <<= 8;
  return tmp64_o1 | tmp64_o2 | tmp64_o3 | tmp64_o4 | tmp64_o5 | tmp64_o6;
} /* psp_rd_le48 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint64_t psp_rd_le64 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o1 = p[7];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o2 = p[6];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o3 = p[5];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o4 = p[4];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o5 = p[3];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o6 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o7 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o8 = p[0];

  tmp64_o1 <<= 56;
  tmp64_o2 <<= 48;
  tmp64_o3 <<= 40;
  tmp64_o4 <<= 32;
  tmp64_o5 <<= 24;
  tmp64_o6 <<= 16;
  tmp64_o7 <<= 8;
  return tmp64_o1 | tmp64_o2 | tmp64_o3 | tmp64_o4 | tmp64_o5 | tmp64_o6 | tmp64_o7 | tmp64_o8;
} /* psp_rd_le64 */



/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
/* little endian write functions */
static inline void psp_wr_le16 ( uint8_t * p, uint16_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_le24 ( uint8_t * p, uint32_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 16 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_le32 ( uint8_t * p, uint32_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)( v >> 24 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_le48 ( uint8_t * p, uint64_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)( v >> 24 );

  /*LDRA_INSPECTED 436 S*/
  p[4] = (uint8_t)( v >> 32 );

  /*LDRA_INSPECTED 436 S*/
  p[5] = (uint8_t)( v >> 40 );
} /* psp_wr_le48 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_le64 ( uint8_t * p, uint64_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)( v >> 24 );

  /*LDRA_INSPECTED 436 S*/
  p[4] = (uint8_t)( v >> 32 );

  /*LDRA_INSPECTED 436 S*/
  p[5] = (uint8_t)( v >> 40 );

  /*LDRA_INSPECTED 436 S*/
  p[6] = (uint8_t)( v >> 48 );

  /*LDRA_INSPECTED 436 S*/
  p[7] = (uint8_t)( v >> 56 );
} /* psp_wr_le64 */


/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
/* big endian read functions */
static inline uint16_t psp_rd_be16 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint16_t  tmp16_h1 = p[0];

  /*LDRA_INSPECTED 436 S*/
  uint16_t  tmp16_h2 = p[1];

  tmp16_h1 <<= 8;
  return tmp16_h1 | tmp16_h2;
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint32_t psp_rd_be24 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q1 = p[0];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q2 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q3 = p[2];

  tmp32_q1 <<= 16;
  tmp32_q2 <<= 8;
  return tmp32_q1 | tmp32_q2 | tmp32_q3;
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint32_t psp_rd_be32 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q1 = p[0];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q2 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q3 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint32_t  tmp32_q4 = p[3];

  tmp32_q1 <<= 24;
  tmp32_q2 <<= 16;
  tmp32_q3 <<= 8;
  return tmp32_q1 | tmp32_q2 | tmp32_q3 | tmp32_q4;
} /* psp_rd_be32 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint64_t psp_rd_be48 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o1 = p[0];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o2 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o3 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o4 = p[3];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o5 = p[4];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o6 = p[5];

  tmp64_o1 <<= 40;
  tmp64_o2 <<= 32;
  tmp64_o3 <<= 24;
  tmp64_o4 <<= 16;
  tmp64_o5 <<= 8;
  return tmp64_o1 | tmp64_o2 | tmp64_o3 | tmp64_o4 | tmp64_o5 | tmp64_o6;
} /* psp_rd_be48 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline uint64_t psp_rd_be64 ( const uint8_t * const p )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o1 = p[0];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o2 = p[1];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o3 = p[2];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o4 = p[3];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o5 = p[4];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o6 = p[5];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o7 = p[6];

  /*LDRA_INSPECTED 436 S*/
  uint64_t  tmp64_o8 = p[7];

  tmp64_o1 <<= 56;
  tmp64_o2 <<= 48;
  tmp64_o3 <<= 40;
  tmp64_o4 <<= 32;
  tmp64_o5 <<= 24;
  tmp64_o6 <<= 16;
  tmp64_o7 <<= 8;
  return tmp64_o1 | tmp64_o2 | tmp64_o3 | tmp64_o4 | tmp64_o5 | tmp64_o6 | tmp64_o7 | tmp64_o8;
} /* psp_rd_be64 */



/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
/* big endian write macros */
static inline void psp_wr_be16 ( uint8_t * p, uint16_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)( v >> 8 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_be24 ( uint8_t * p, uint32_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)( v >> 16 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_be32 ( uint8_t * p, uint32_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)( v >> 24 );
}

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_be48 ( uint8_t * p, uint64_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[5] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[4] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 24 );

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 32 );

  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)( v >> 40 );
} /* psp_wr_be48 */

/* 35 S : MISRA-C:2012/AMD1/TC1 R.2.1: Static procedure is not explicitly called in code analysed. */
/*LDRA_INSPECTED 35 S*/
static inline void psp_wr_be64 ( uint8_t * p, uint64_t v )
{
  /* 436 S : MISRA-C:2004 17.1,17.4, MISRA-C:2012/AMD1/TC1 R.18.1: Declaration does not specify an array. */
  /*LDRA_INSPECTED 436 S*/
  p[7] = (uint8_t)v;

  /*LDRA_INSPECTED 436 S*/
  p[6] = (uint8_t)( v >> 8 );

  /*LDRA_INSPECTED 436 S*/
  p[5] = (uint8_t)( v >> 16 );

  /*LDRA_INSPECTED 436 S*/
  p[4] = (uint8_t)( v >> 24 );

  /*LDRA_INSPECTED 436 S*/
  p[3] = (uint8_t)( v >> 32 );

  /*LDRA_INSPECTED 436 S*/
  p[2] = (uint8_t)( v >> 40 );

  /*LDRA_INSPECTED 436 S*/
  p[1] = (uint8_t)( v >> 48 );

  /*LDRA_INSPECTED 436 S*/
  p[0] = (uint8_t)( v >> 56 );
} /* psp_wr_be64 */

/* 286 S : MISRA-C:2004 8.5: Functions defined in a header file. */
/*LDRA_INSPECTED 286 S*/

#endif /* ifndef PSP_ENDIANNESS_H_ */
