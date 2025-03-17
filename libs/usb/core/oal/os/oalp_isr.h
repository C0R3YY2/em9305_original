/***************************************************************************
 *
 *            Copyright (c) 2010-2020 by HCC Embedded
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
#ifndef OALP_ISR_H
#define OALP_ISR_H

#include "../../psp/include/psp_types.h"
#include "../../config/config_oal.h"

#include "../../version/ver_oal_os.h"
#if VER_OAL_NOS_APINB_QPC_MAJOR != 1 || VER_OAL_NOS_APINB_QPC_MINOR != 2
 #error Incompatible OAL_NOS_APINB_QPC version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OAL_ISR_SUPPORTED
void oal_isr_poll ( void );
#endif

typedef uint32_t  oal_isr_id_t;

#define OAL_ISR_FN( fn ) void( fn ) (void)
#define OAL_ISR_PRE
#define OAL_ISR_POST

#ifdef __cplusplus
}
#endif

#endif /* ifndef OALP_ISR_H */
