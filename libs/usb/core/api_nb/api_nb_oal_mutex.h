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
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _API_NB_OAL_MUTEX_H
#define _API_NB_OAL_MUTEX_H

#include "os/api_nb_oalp_mutex.h"

#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

int api_nb_oal_mutex_create ( t_api_nb_oal_mutex * p_mutex );
int api_nb_oal_mutex_delete ( t_api_nb_oal_mutex * p_mutex );
int api_nb_oal_mutex_get ( t_api_nb_oal_mutex * p_mutex );
int api_nb_oal_mutex_put ( t_api_nb_oal_mutex * p_mutex );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_NB_OAL_MUTEX_H */
