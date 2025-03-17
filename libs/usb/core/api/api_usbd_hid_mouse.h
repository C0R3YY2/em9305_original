/***************************************************************************
 *
 *            Copyright (c) 2009-2020 by HCC Embedded
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
#ifndef _API_USBD_HID_MOUSE_H_
#define _API_USBD_HID_MOUSE_H_

#include "../psp/include/psp_types.h"
#include "api_usbd_hid.h"

#include "../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver version."
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define USBD_MOUSE_IN_CB 1

int usbd_mhid_init ( void );
int usbd_mhid_start ( void );
int usbd_mhid_stop ( void );
int usbd_mhid_delete ( void );

int usbd_mhid_register_ntf ( uint8_t unit, t_usbd_hid_ntf ntf, t_usbd_hid_ntf_fn ntf_fn, uint32_t ntf_fn_param );
void usbd_mhid_set_xy ( uint8_t unit, int8_t dx, int8_t dy );
void usbd_mhid_set_btn_state ( uint8_t unit, uint8_t btn, uint8_t state );
int usbd_mhid_set_report ( uint8_t unit, int8_t dx, int8_t dy, uint8_t buttons );
void usbd_mhid_clear_report ( uint8_t unit );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_HID_MOUSE_H_ */
