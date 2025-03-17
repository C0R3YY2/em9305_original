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
#ifndef _USBD_DESCRIPTOR_H_
#define _USBD_DESCRIPTOR_H_

#include "../../../psp/include/psp_types.h"

#include "../../../version/ver_usbd.h"
#if VER_USBD_MAJOR != 3 || VER_USBD_MINOR != 31
 #error "Incorrect USBD version."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Find a descriptor in the spefifyed range with matching type. The return
   value is either a pointer ot the matching descriptor or end. */
const uint8_t * usbd_find_descriptor ( const uint8_t * start, const uint8_t * end, uint8_t type );


/* Return a pointer to the next descriptor. Start must refer to a valid USB
   descriptor. */
const uint8_t * usbd_next_descriptor ( const uint8_t * start );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _USBD_DESCRIPTOR_H_ */
