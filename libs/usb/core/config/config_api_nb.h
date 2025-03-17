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
#ifndef _CONFIG_API_NB_H
#define _CONFIG_API_NB_H

#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

#define API_NB_TASK_STACK_SIZE    1024   /* SYNC stack size of the job task */
#define API_NB_TASK_PRIORITY      15     /* SYNC job task priority */

#define API_NB_MAX_JOBS           4      /* no. concurrent jobs (max 32) */

#define API_NB_RC_SIZE            4      /* size of the return code in bytes */
#define API_NB_PAR_SIZE           32     /* total size of the parameters in bytes */

#define API_NB_MUTEX_MAXTASK_PRIO 16u    /* Max priority of the task can preempt the current task requesting for a mutex.
                                            Used for QPC Mutex. */

#endif /* ifndef _CONFIG_API_NB_H */
