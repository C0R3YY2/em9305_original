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
#ifndef _PSP_USBD_SYNOPSYS_OTG_H
#define _PSP_USBD_SYNOPSYS_OTG_H

#include "../../../usb-device/usb-drivers/usbd_synopsys_otg_regs.h"

#include "../../../version/ver_psp_usbd_synopsys_otg.h"
#if VER_PSP_USBD_SYNOPSYS_SHAREDFIFO_OTG_MAJOR != 1 || VER_PSP_USBD_SYNOPSYS_SHAREDFIFO_OTG_MINOR != 1
 #error Incompatible USBD_SYNOPSYS_SHAREDFIFO_OTG version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

int psp_usbd_hw_init ( void );
int psp_usbd_hw_start ( void );
int psp_usbd_hw_stop ( void );
int psp_usbd_hw_delete ( void );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _PSP_USBD_SYNOPSYS_OTG_H */
