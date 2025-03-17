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
#ifndef _API_API_NB_H
#define _API_API_NB_H

#include <stdint.h>

#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define API_NB_SUCCESS 0
#define API_NB_BUSY    1
#define API_NB_FAIL    2
#define API_NB_INVALID 3

typedef uint32_t  t_api_nb_hdl;

int api_nb_init ( void );
int api_nb_delete ( void );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_NB_H */
