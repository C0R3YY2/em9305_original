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
#ifndef _USBD_DEV_H_
#define _USBD_DEV_H_

#include "../../config/config_usbd.h"
#include "../../config/config_usbd_synopsys_otg.h"


#include "../version/ver_usbd_synopsys_otg.h"
#if ( ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MAJOR != 1 ) || ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MINOR != 5 ) )
 #error "Invalid USBD_SYNOPSYS_SHAREDFIFO_OTG version."
#endif


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ************************ Macro definitions ***********************************
 *****************************************************************************/

#define USBD_MEM_HANDLE HCC_MEM_DEFAULT
#define USBD_MEM_ALIGN  32u


/******************************************************************************
 ************************ Type definitions ************************************
 *****************************************************************************/

/* USB device hardware Endpoint information */
typedef struct
{
  uint8_t  ep_num;
  uint8_t  dir;
} usbd_hw_ep_info_t;


/******************************************************************************
 ************************ Exported functions **********************************
 *****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif /* ifndef _USBD_DEV_H_ */
