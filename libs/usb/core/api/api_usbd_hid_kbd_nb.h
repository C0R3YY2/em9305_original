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
#ifndef _API_USBD_HID_KBD_NB_H
#define _API_USBD_HID_KBD_NB_H

#include <stdint.h>
#include "api_api_nb.h"
#include "api_usbd_hid_kbd.h"
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

#define USBD_HID_KBD_NB_USBD_KBD_REGISTER_NTF_ID  0
#define USBD_HID_KBD_NB_USBD_KBD_SET_MODIFIER_ID  1
#define USBD_HID_KBD_NB_USBD_KBD_SET_KEY_STATE_ID 2
#define USBD_HID_KBD_NB_USBD_KBD_SET_REPORT_ID    3
#define USBD_HID_KBD_NB_USBD_KBD_CLEAR_REPORT_ID  4
#define USBD_HID_KBD_NB_USBD_KBD_GET_LED_STATE_ID 5


int usbd_kbd_register_ntf_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, t_usbd_hid_ntf ntf, t_usbd_hid_ntf_fn ntf_fn, uint32_t ntf_fn_param, enum_t signal, QActive * me );
int usbd_kbd_set_modifier_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t modifier, enum_t signal, QActive * me );
int usbd_kbd_set_key_state_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t key, uint8_t state, enum_t signal, QActive * me );
int usbd_kbd_set_report_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t modifier, uint8_t * p_keys, uint8_t key_count, enum_t signal, QActive * me );
int usbd_kbd_clear_report_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, enum_t signal, QActive * me );
int usbd_kbd_get_led_state_nb_start ( t_api_nb_hdl * p_api_nb_hdl, uint8_t unit, uint8_t * p_led_state, enum_t signal, QActive * me );



int usbd_hid_kbd_ret1_nb_state ( t_api_nb_hdl api_nb_hdl, int * p_rc );
#define usbd_kbd_register_ntf_nb_state( p_api_nb_hdl, p_rc )  usbd_hid_kbd_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_kbd_set_key_state_nb_state( p_api_nb_hdl, p_rc ) usbd_hid_kbd_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_kbd_set_report_nb_state( p_api_nb_hdl, p_rc )    usbd_hid_kbd_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
#define usbd_kbd_get_led_state_nb_state( p_api_nb_hdl, p_rc ) usbd_hid_kbd_ret1_nb_state( ( p_api_nb_hdl ), ( p_rc ) )
int usbd_hid_kbd_ret2_nb_state ( t_api_nb_hdl api_nb_hdl );
#define usbd_kbd_set_modifier_nb_state( p_api_nb_hdl )        usbd_hid_kbd_ret2_nb_state( ( p_api_nb_hdl ) )
#define usbd_kbd_clear_report_nb_state( p_api_nb_hdl )        usbd_hid_kbd_ret2_nb_state( ( p_api_nb_hdl ) )


#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_USBD_HID_KBD_NB_H */
