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
#ifndef OALP_EVENT_H
#define OALP_EVENT_H

#include "../../psp/include/psp_types.h"

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

/* 495 S : MISRA-C:2004 6.3, MISRA-C:2012/AMD1/TC1 D.4.6: Typedef name has no size indication. */
/*LDRA_INSPECTED 495 S*/
typedef uint32_t  oal_event_t;

/*LDRA_INSPECTED 495 S*/
typedef uint16_t  oal_event_flags_t;

/*LDRA_INSPECTED 495 S*/
typedef uint32_t  oal_event_timeout_t;

#define OAL_WAIT_FOREVER (oal_event_timeout_t)UINT32_MAX

#ifdef __cplusplus
}
#endif

#endif /* ifndef OALP_EVENT_H */
