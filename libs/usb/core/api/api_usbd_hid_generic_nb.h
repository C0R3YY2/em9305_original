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
#ifndef _API_USBD_HID_GENERIC_NB_H
#define _API_USBD_HID_GENERIC_NB_H

#include <stdint.h>
#include "api_api_nb.h"
#include "api_usbd_hid_generic.h"
#include "qf_port.h"

#include "../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8
 #error Incompatible USBD_HID version number!
#endif
#include "../version/ver_api_nb.h"
#if VER_POLL_QPC_API_NB_MAJOR != 1 || VER_POLL_QPC_API_NB_MINOR != 2
 #error Incompatible POLL_QPC_API_NB version number!
#endif

/* AUTO GENERATED FILE BY NB_CONV 1.0 */

#ifdef __cplusplus
extern "C" {
#endif

#define USBD_HID_GENERIC_NB_USBD_GHID_REGISTER_NTF_ID 0
#define USBD_HID_GENERIC_NB_USBD_GHID_READ_DATA_ID    1
#define USBD_HID_GENERIC_NB_USBD_GHID_WRITE_DATA_ID   2


int usbd_ghid_register_ntf_nb_start ( t_api_nb_hdl * p_api_nb_hdl,
                                     uint8_t unit,
                                     uint8_t id,
                                     t_usbd_hid_ntf ntf,
                                     t_usbd_hid_ntf_fn ntf_fn,
                                     uint32_t ntf_fn_param,
                                     enum_t signal,
                                     QActive * task );
int usbd_ghid_read_data_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t id, uint8_t * p_buf, uint16_t * length, enum_t signal, QActive * me );
int usbd_ghid_write_data_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t id, uint8_t * p_buf, enum_t signal, QActive * me );



int usbd_hid_generic_ret1_nb_state ( t_api_nb_hdl api_nb_hdl, int * p_rc );
#define usbd_ghid_register_ntf_nb_state( p_api_nb_hdl, p_rc ) usbd_hid_generic_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_ghid_read_data_nb_state( p_api_nb_hdl, p_rc )    usbd_hid_generic_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_ghid_write_data_nb_state( p_api_nb_hdl, p_rc )   usbd_hid_generic_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_USBD_HID_GENERIC_NB_H */
