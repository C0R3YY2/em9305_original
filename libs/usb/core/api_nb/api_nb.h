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
#ifndef _API_NB_H
#define _API_NB_H

#include <stdint.h>
#include "../api/api_api_nb.h"
#include "../config/config_api_nb.h"
#include "../psp/include/psp_string.h"
#include "qf_port.h"

#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define API_NB_ST_FREE     0
#define API_NB_ST_USED     1
#define API_NB_ST_STARTED  2
#define API_NB_ST_RUNNING  3
#define API_NB_ST_FINISHED 4

typedef struct _t_api_nb_dsc  t_api_nb_dsc;
typedef void               ( *api_nb_proc )( t_api_nb_dsc * nb_dsc );

struct _t_api_nb_dsc
{
  api_nb_proc       proc;                       /* process function */
  uint16_t          fn_id;                      /* function ID */
  volatile uint8_t  state;                      /* current state of the job */
  uint8_t           rc[API_NB_RC_SIZE];         /* return code */
  uint8_t           param[API_NB_PAR_SIZE + 2]; /* parameters */
  uint32_t          id;                         /* ID to recognize invalid requests */
  enum_t            api_nb_signal;
  QActive *         requester;
};

#define API_NB_SET_PROC( dsc, proc_fn )    ( dsc )->proc = proc_fn
#define API_NB_SET_FN_ID( dsc, fn_id_val ) ( dsc )->fn_id = fn_id_val
#define API_NB_SET_SIGNAL( dsc, signal ) ( dsc )->api_nb_signal = signal
#define API_NB_SET_TASK( dsc, task ) ( dsc )->requester = task
#define API_NB_PUT_PAR( dsc, par ) \
  psp_memcpy( &( dsc )->param[( dsc )->param[0] + 2], &( par ), sizeof( par ) ); \
  ( dsc )->param[0] += sizeof( par )
#define API_NB_GET_PAR( dsc, par ) \
  psp_memcpy( &( par ), &( dsc )->param[( dsc )->param[1] + 2], sizeof( par ) ); \
  ( dsc )->param[1] += sizeof( par )
#define API_NB_PUT_RC( dsc, rc_val )       psp_memcpy( &((t_api_nb_dsc *) dsc )->rc, &( rc_val ), sizeof( rc_val ) )
#define API_NB_GET_RC( dsc, rc_val )       psp_memcpy( &( rc_val ), &( dsc )->rc, sizeof( rc_val ) )

int api_nb_alloc ( t_api_nb_hdl * p_api_nb_hdl, t_api_nb_dsc * * p_api_nb_dsc );
int api_nb_free ( t_api_nb_hdl api_nb_hdl );
int api_nb_start ( t_api_nb_hdl api_nb_hdl );
int api_nb_state ( t_api_nb_hdl api_nb_hdl, t_api_nb_dsc * * p_api_nb_dsc );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_NB_H */
