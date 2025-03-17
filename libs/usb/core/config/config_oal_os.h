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
#ifndef CONFIG_OAL_OS_H
#define CONFIG_OAL_OS_H

#include "../psp/include/psp_types.h"

/* OAL user definition file for no OS */
#include "../version/ver_oal_os.h"
#if VER_OAL_NOS_APINB_QPC_MAJOR != 1 || VER_OAL_NOS_APINB_QPC_MINOR != 2
 #error Incompatible OAL_NOS_APINB_QPC version number!
#endif
#include "../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

/* maximum number of mutexes */
#define OAL_MUTEX_COUNT      16

/* maximum number of events */
#define OAL_EVENT_COUNT      16

/* priorities - no meaning without OS */
#define OAL_HIGHEST_PRIORITY 0
#define OAL_HIGH_PRIORITY    0
#define OAL_NORMAL_PRIORITY  0
#define OAL_LOW_PRIORITY     0
#define OAL_LOWEST_PRIORITY  0

/* Event flag to use for user tasks invoking internal functions. */
/* NOTE: The value of this flag should be > 0x80 because lower bits */
/* might be used by internal tasks */
#define OAL_EVENT_FLAG       0x100u

/* maximum number of tasks */
#define OAL_TASK_COUNT       8

/* allow interrupts */
#define OAL_INTERRUPT_ENABLE 1

/* no. of max. interrupt sources if interrupts are disabled */
#define OAL_ISR_COUNT        2

/* tick rate in ms */
#define OAL_TICK_RATE        10u

/* Task ID type */
/* 495 S : MISRA-C:2004 6.3, MISRA-C, MISRA-C:2012/AMD1/TC1 D.4.6:2012/AMD1/TC1 D.4.6: Typedef name has no size indication. */
/*LDRA_INSPECTED 495 S*/
typedef uint32_t  oal_task_id_t;

/* Signal ID which triggers a task poll */
#define OAL_DO_TASK_POLL_SIG Q_USER_SIG

#endif /* ifndef CONFIG_OAL_OS_H */
