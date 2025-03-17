/***************************************************************************
 *
 *            Copyright (c) 2011-2020 by HCC Embedded
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
 * Vaci Ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _API_NB_OALP_MUTEX_H
#define _API_NB_OALP_MUTEX_H

#include <stdint.h>

#include "../../version/ver_api_nb_oal_os.h"
#if VER_API_NB_OAL_OS_MAJOR != 1 || VER_API_NB_OAL_OS_MINOR != 1
 #error Incompatible API_NB_OAL_OS version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t  t_api_nb_oal_mutex;

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_NB_OALP_MUTEX_H */
