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
#ifndef _CONFIG_USBD_HID_MOUSE_H_
#define _CONFIG_USBD_HID_MOUSE_H_

#include "../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver version."
#endif

#define MAX_HID_MOUSE_IFC         1

/* Interface definitions. */
#if ( MAX_HID_MOUSE_IFC > 0 )
 #define USBD_HID_MOUSE_IFC_NDX_0 0
 #define USBD_MHID_IN_REPORT_ID_0 3
#endif
#if ( MAX_HID_MOUSE_IFC > 1 )
 #define USBD_HID_MOUSE_IFC_NDX_1 1
 #define USBD_MHID_IN_REPORT_ID_1 0
#endif
#if ( MAX_HID_MOUSE_IFC > 2 )
 #define USBD_HID_MOUSE_IFC_NDX_2 2
 #define USBD_MHID_IN_REPORT_ID_2 0
#endif
#if ( MAX_HID_MOUSE_IFC > 3 )
 #define USBD_HID_MOUSE_IFC_NDX_3 3
 #define USBD_MHID_IN_REPORT_ID_3 0
#endif

#endif /* ifndef _CONFIG_USBD_HID_MOUSE_H_ */
