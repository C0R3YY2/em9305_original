/***************************************************************************
 *
 *            Copyright (c) 2008-2020 by HCC Embedded
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
#ifndef _USBDI_DEV_H_
#define _USBDI_DEV_H_

#include "../../../oal/oal_isr.h"
#include "../usbd_dev.h"

#include "../../../version/ver_usbd.h"
#if VER_USBD_MAJOR != 3 || VER_USBD_MINOR != 31
 #error "Incorrect USBD version."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ************************ Exported functions **********************************
 *****************************************************************************/
void usbd_clr_stall ( int index );
void usbd_set_stall ( int index );
int usbd_get_stall ( int index );
int usbd_add_fifo ( int index );
int usbd_drop_fifo ( int index );

void usbd_set_testmode ( uint8_t testmode_selector );

void usbd_set_addr_pre ( uint8_t daddr );
void usbd_set_addr_post ( uint8_t daddr );

void usbd_set_cfg_pre ( void );
void usbd_set_cfg_post ( void );

int usbd_send ( usbd_transfer_t * tr );
int usbd_receive ( usbd_transfer_t * tr );

int usbd_add_ep ( int index );
void usbd_remove_ep ( int index );

int usbd_hw_init ( void );
int usbd_hw_start ( void );
int usbd_hw_stop ( void );
int usbd_hw_delete ( void );

int usbd_at_high_speed ( void );

int usbd_set_remote_wakeup ( void );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _USBDI_DEV_H_ */
