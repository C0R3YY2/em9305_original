/***************************************************************************
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
#ifndef PSP_IDLE_H
#define PSP_IDLE_H

#include "../../version/ver_psp_idle.h"
#if VER_PSP_IDLE_MAJOR != 1  ||  VER_PSP_IDLE_MINOR != 1
 #error Incompatible VER_PSP_IDLE_MAJOR version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif

void psp_idle ( void );

#ifdef __cplusplus
}
#endif

#endif /* PSP_IDLE_H */
