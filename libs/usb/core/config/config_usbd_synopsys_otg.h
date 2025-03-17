/***************************************************************************
 *
 *            Copyright (c) 2020-2022 by HCC Embedded
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
#ifndef _CONFIG_USBD_SYNOPSYS_OTG_H_
#define _CONFIG_USBD_SYNOPSYS_OTG_H_

#include "config_usbd.h"

#include "../version/ver_usbd_synopsys_otg.h"
#if ( ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MAJOR != 1 ) || ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MINOR != 5 ) )
 #error "Invalid USBD_SYNOPSYS_SHAREDFIFO_OTG version."
#endif


/* Number of hardware endpoints */
#define OTG_NUM_BD_EP                    3u


#define OTG_FIFO_SIZE                    256u


#define OTG_USE_VBUS_IN                  0 /* set to 1 in case VBUS detection can
                                            be performed ( VBUS is connected
                                            properly ) */

#define OTG_FS_VBUS_CONNECTED            0   /* Set this to 1 if VBUS is
                                                connected to OTG_FS_VBUS pin.
                                                In this case VBUS sensing and
                                                pulling up D+ is automatically
                                                done by USB FS core. */
#define OTG_ISR_ID                       53u /* OTG_FS it ID = 53 */
#define OTG_BASE                         0x00F80000u

#define OTG_IT_PRIO                      1u


#endif /* ifndef _CONFIG_USBD_SYNOPSYS_OTG_H_ */
