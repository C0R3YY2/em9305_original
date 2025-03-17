/***************************************************************************
 *
 *            Copyright (c) 2010-2013 by HCC Embedded
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
#ifndef _CONFIG_SYNC_H
#define _CONFIG_SYNC_H

#include "../version/ver_sync.h"
#if VER_SYNC_MAJOR != 3 || VER_SYNC_MINOR != 3
 #error Incompatible SYNC version number!
#endif

/* When set SYNC will run forever, in this mode sync_run will never */
/* return and will run as a standalone OS. */
#define SYNC_RUN_FOREVER       0

/* When this option is set time slicing is enabled. */
#define SYNC_TIME_SLICE_ENABLE 1

/* When set debug functions are available. */
#define SYNC_DEBUG_ENABLE      0

#endif /* ifndef _CONFIG_SYNC_H */
