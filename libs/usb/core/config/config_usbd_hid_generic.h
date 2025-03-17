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
#ifndef _CONFIG_USBD_HID_GENERIC_H_
#define _CONFIG_USBD_HID_GENERIC_H_


#include "../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver version."
#endif

#define MAX_HID_GENERIC_IFC          1

/* Maximum value of USBD_GHID_IN_MAX_REPORT_NUM_x. */
#define USBD_GHID_IN_MAX_REPORT_NUM  1

/* Maximum value of USBD_GHID_OUT_MAX_REPORT_NUM_x. */
#define USBD_GHID_OUT_MAX_REPORT_NUM 1

#endif /* ifndef _CONFIG_USBD_HID_GENERIC_H_ */
