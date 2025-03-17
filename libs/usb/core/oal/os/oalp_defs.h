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
#ifndef OALP_DEFS_H
#define OALP_DEFS_H

#include "../../config/config_oal_os.h"

#include "../../version/ver_oal_os.h"
#if VER_OAL_NOS_APINB_QPC_MAJOR != 1 || VER_OAL_NOS_APINB_QPC_MINOR != 2
 #error Incompatible OAL_NOS_APINB_QPC version number!
#endif
#include "../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

/* tasks are polled - only set with no OS */
#define OAL_TASK_POLL_MODE    1

/* preemptive system */
#define OAL_PREEMPTIVE        0

/* stack of a task needs to be allocated statically */
#define OAL_STATIC_TASK_STACK 0

/* use platform ISR routines */
#if OAL_INTERRUPT_ENABLE
 #define OAL_USE_PLATFORM_ISR 1
#else
 #define OAL_USE_PLATFORM_ISR 0
#endif

#endif /* ifndef OALP_DEFS_H */
