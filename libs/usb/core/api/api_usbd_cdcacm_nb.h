/****************************************************************************
 *
 *            Copyright (c) 2020 by HCC Embedded
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
#ifndef _API_USBD_CDCACM_NB_H
#define _API_USBD_CDCACM_NB_H

#include <stdint.h>
#include "api_api_nb.h"
#include "api_usbd_cdcacm.h"
#include "qf_port.h"

#include "../version/ver_usbd_cdcacm.h"
#if VER_USBD_CDCACM_MAJOR != 5
 #error Incompatible USBD_CDCACM version number!
#endif
#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

/* AUTO GENERATED FILE BY NB_CONV 1.0 */

#ifdef __cplusplus
extern "C" {
#endif

#define USBD_CDCACM_NB_USBD_CDCACM_PRESENT_ID                0
#define USBD_CDCACM_NB_USBD_CDCACM_SEND_ID                   1
#define USBD_CDCACM_NB_USBD_CDCACM_SEND_STATUS_ID            2
#define USBD_CDCACM_NB_USBD_CDCACM_RECEIVE_START_ID          3
#define USBD_CDCACM_NB_USBD_CDCACM_RECEIVE_ID                4
#define USBD_CDCACM_NB_USBD_CDCACM_RECEIVE_STATUS_ID         5
#define USBD_CDCACM_NB_USBD_CDCACM_SET_RX_MODE_ID            6
#define USBD_CDCACM_NB_USBD_CDCACM_REG_NTF_FN_ID             7
#define USBD_CDCACM_NB_USBD_CDCACM_GET_LINE_CODING_ID        8
#define USBD_CDCACM_NB_USBD_CDCACM_SET_LINE_CODING_ID        9
#define USBD_CDCACM_NB_USBD_CDCACM_GET_CONTROL_LINE_STATE_ID 10
#define USBD_CDCACM_NB_USBD_CDCACM_SET_LSFLAGS_ID            11


int usbd_cdcacm_par1_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint16_t fn_id, const uint8_t p0, enum_t signal, QActive * me );
#define usbd_cdcacm_present_nb_start( p_api_nb_hdl, line, signal, task )                   usbd_cdcacm_par1_nb_start( ( p_api_nb_hdl ), USBD_CDCACM_NB_USBD_CDCACM_PRESENT_ID, \
                                                                                                       ( line ), ( signal ), ( task ) )
#define usbd_cdcacm_send_status_nb_start( p_api_nb_hdl, line, signal, task )               usbd_cdcacm_par1_nb_start( ( p_api_nb_hdl ), \
                                                                                                       USBD_CDCACM_NB_USBD_CDCACM_SEND_STATUS_ID, \
                                                                                                       ( line ), ( signal ), ( task ) )
int usbd_cdcacm_par2_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint16_t fn_id, const uint8_t p0, uint8_t * const p1, const uint32_t p2, enum_t signal, QActive * me );
#define usbd_cdcacm_send_nb_start( p_api_nb_hdl, line, p_buf, len, signal, task )          usbd_cdcacm_par2_nb_start( ( p_api_nb_hdl ), USBD_CDCACM_NB_USBD_CDCACM_SEND_ID, \
                                                                                                       ( line ), ( p_buf ), ( len ), ( signal ), ( task ) )
#define usbd_cdcacm_receive_start_nb_start( p_api_nb_hdl, line, p_buf, len, signal, task ) usbd_cdcacm_par2_nb_start( ( p_api_nb_hdl ), \
                                                                                                       USBD_CDCACM_NB_USBD_CDCACM_RECEIVE_START_ID, \
                                                                                                       ( line ), \
                                                                                                       ( p_buf ), \
                                                                                                       ( len ), ( signal ), ( task ) )
int usbd_cdcacm_receive_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const uint8_t line, uint8_t * const p_buf, const uint32_t len_req, uint32_t * const p_len_done, enum_t signal, QActive * me );
int usbd_cdcacm_receive_status_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const uint8_t line, uint32_t * const p_len, enum_t signal, QActive * me );
int usbd_cdcacm_set_rx_mode_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const uint8_t line, const t_usbd_cdcacm_rx_mode mode, enum_t signal, QActive * me );
int usbd_cdcacm_reg_ntf_fn_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const t_usbd_cdcacm_ntf_type ntf, const t_usbd_cdcacm_ntf_fn ntf_fn, enum_t signal, QActive * me );
int usbd_cdcacm_par7_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint16_t fn_id, const uint8_t p0, t_usbd_cdcacm_line_coding * const p1, enum_t signal, QActive * me );
#define usbd_cdcacm_get_line_coding_nb_start( p_api_nb_hdl, line, p_ln_coding, signal, task ) usbd_cdcacm_par7_nb_start( ( p_api_nb_hdl ), \
                                                                                                          USBD_CDCACM_NB_USBD_CDCACM_GET_LINE_CODING_ID, \
                                                                                                          ( line ), \
                                                                                                          ( p_ln_coding ), ( signal ), ( task ) )
#define usbd_cdcacm_set_line_coding_nb_start( p_api_nb_hdl, line, p_ln_coding, signal, task ) usbd_cdcacm_par7_nb_start( ( p_api_nb_hdl ), \
                                                                                                          USBD_CDCACM_NB_USBD_CDCACM_SET_LINE_CODING_ID, \
                                                                                                          ( line ), \
                                                                                                          ( p_ln_coding ), ( signal ), ( task ) )
int usbd_cdcacm_get_control_line_state_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const uint8_t line, uint16_t * const p_flags, enum_t signal, QActive * me );
int usbd_cdcacm_set_lsflags_nb_start ( t_api_nb_hdl * p_api_nb_hdl, const uint8_t line, const uint8_t flags, enum_t signal, QActive * me );



int usbd_cdcacm_ret1_nb_state ( t_api_nb_hdl api_nb_hdl, t_usbd_cdcacm_ret * p_rc );
#define usbd_cdcacm_present_nb_state( p_api_nb_hdl, p_rc )                usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_send_nb_state( p_api_nb_hdl, p_rc )                   usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_send_status_nb_state( p_api_nb_hdl, p_rc )            usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_receive_start_nb_state( p_api_nb_hdl, p_rc )          usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_receive_nb_state( p_api_nb_hdl, p_rc )                usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_receive_status_nb_state( p_api_nb_hdl, p_rc )         usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_set_rx_mode_nb_state( p_api_nb_hdl, p_rc )            usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_reg_ntf_fn_nb_state( p_api_nb_hdl, p_rc )             usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_get_line_coding_nb_state( p_api_nb_hdl, p_rc )        usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_set_line_coding_nb_state( p_api_nb_hdl, p_rc )        usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_get_control_line_state_nb_state( p_api_nb_hdl, p_rc ) usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_cdcacm_set_lsflags_nb_state( p_api_nb_hdl, p_rc )            usbd_cdcacm_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_USBD_CDCACM_NB_H */
