/***************************************************************************
 *
 *            Copyright (c) 2018-2019 by HCC Embedded
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
#ifndef PSP_ASSERT_H_
#define PSP_ASSERT_H_

#include "psp_types.h"

#include "../../version/ver_psp_assert.h"

#if VER_PSP_ASSERT_MAJOR != 1 || VER_PSP_ASSERT_MINOR != 1
 #error Incompatible PSP_ASSERT version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
 #define PSP_DEBUG _DEBUG
#endif

#ifndef PSP_DEBUG

 #define PSP_ASSERT( expression )

#else

extern void psp_assert ( uint8_t expression, const char * p_filename, long line );

 #define PSP_ASSERT( expression ) psp_assert( (uint8_t)( expression ), __FILE__, __LINE__ )

#endif
#ifdef __cplusplus
}
#endif
#endif /* ifndef PSP_ASSERT_H_ */
