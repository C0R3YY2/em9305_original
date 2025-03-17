/***************************************************************************
 *
 *            Copyright (c) 2010-2020 by HCC Embedded
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
#ifndef _PSP_ISR_H
#define _PSP_ISR_H

#include "../../include/psp_types.h"
#include <qf_port.h>

#include "../../../version/ver_psp.h"
#ifndef PSP_ARCEM7D_COSTOMERBOARD_QPC_METAWARE
 #error Incompatible PSP!
#endif
#include "../../../version/ver_psp_isr.h"
#if VER_PSP_ISR_MAJOR != 1 || VER_PSP_ISR_MINOR != 1
 #error Incompatible PSP_ISR version number!
#endif
#include "../../../version/ver_oal_os.h"
#if VER_OAL_NOS_APINB_QPC_MAJOR != 1
 #error Incompatible OAL_NOS_APINB_QPC version number!
#endif
#include "../../../version/ver_oal.h"
#if VER_OAL_MAJOR != 2 || VER_OAL_MINOR < 2
 #error Incompatible OAL version number!
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef uint32_t  psp_isr_id_t;

#define OTG_FS_IRQn 53
#define OTG_HS_IRQn 53

#define PSP_ISR_FN( fn ) void( fn ) (void)
#define PSP_ISR_PRE
#define PSP_ISR_POST


#ifdef __cplusplus
}
#endif

#endif /* ifndef _PSP_ISR_H */
