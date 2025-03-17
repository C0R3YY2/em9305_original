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
#ifndef _CONFIG_USBD_HID_H_
#define _CONFIG_USBD_HID_H_

#include "../psp/include/psp_types.h"

#include "../version/ver_usbd_hid.h"
#include "config_usbd_config.h"

#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver version."
#endif


#include "config_oal_os.h"
#include "config_usbd_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/* public array definitions: */
extern unsigned char   hid_report_descriptor[];

extern const uint8_t * hid_descriptor[];

/* HID-specific definitions: */
#define HID_INTERFACE_COUNT   1
#define HID_DESCRIPTOR_LENGTH 9
#define HID_PHYSICAL_ADDR     NULL
#define HID_PHYSICAL_LENGTH   0

extern const uint8_t * report_descriptor[];
extern const uint16_t  report_descriptor_length[];


/* This must be set to:
      1 if the HID report uses report ID:s.
      0 if not. */
#if ( ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_HIDGENERIC ) || ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_CDCACM_HID ) )
 /* For HID Generic we use a report ID */
 #define USE_REPORT_ID            1
#else
 #define USE_REPORT_ID            0
#endif


/* If this set to
      1 OUT report size is checked and report discarded if doesn't
        match OUT report size reported to host.
      0 OUT report size is not checked and report is received.  */
#define HID_CHECK_OUT_REPORT_SIZE 1

#if ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_HIDGENERIC )

 #if ( HID_INTERFACE_COUNT > 0 )
  #define HID_IFC_0                0
/* Set this to 1 if you intend to use a OUT interrupt pipe on this interface.
   Note: the descriptors shall define additional endpoints too. */
  #define USE_RX_IT_EP_0           1
  #define USE_RX_IT_EP_1           0
  #define USE_RX_IT_EP_2           0
  #define USE_RX_IT_EP_3           0
 #endif
 #if ( HID_INTERFACE_COUNT > 1 )
  #define HID_IFC_1                1
  #define USE_RX_IT_EP_0           0
  #define USE_RX_IT_EP_1           1
  #define USE_RX_IT_EP_2           0
  #define USE_RX_IT_EP_3           0
 #endif
 #if ( HID_INTERFACE_COUNT > 2 )
  #define HID_IFC_2                2
  #define USE_RX_IT_EP_0           0
  #define USE_RX_IT_EP_1           0
  #define USE_RX_IT_EP_2           1
  #define USE_RX_IT_EP_3           0
 #endif
 #if ( HID_INTERFACE_COUNT > 3 )
  #define HID_IFC_3                3
  #define USE_RX_IT_EP_0           0
  #define USE_RX_IT_EP_1           0
  #define USE_RX_IT_EP_2           0
  #define USE_RX_IT_EP_3           1
 #endif

#elif ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_CDCACM_HID )

#if ( HID_INTERFACE_COUNT > 0 )
 #define HID_IFC_0                2
/* Set this to 1 if you intend to use a OUT interrupt pipe on this interface.
  Note: the descriptors shall define additional endpoints too. */
 #define USE_RX_IT_EP_0           0
#endif
#if ( HID_INTERFACE_COUNT > 1 )
 #define HID_IFC_1                1
 #define USE_RX_IT_EP_1           1
#endif
#if ( HID_INTERFACE_COUNT > 2 )
 #define HID_IFC_2                2
 #define USE_RX_IT_EP_2           1
#endif
#if ( HID_INTERFACE_COUNT > 3 )
 #define HID_IFC_3                3
 #define USE_RX_IT_EP_3           1
#endif

#else /* #if ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_HIDGENERIC ) */

 #if ( HID_INTERFACE_COUNT > 0 )
  #define HID_IFC_0                0
 /* Set this to 1 if you intend to use a OUT interrupt pipe on this interface.
    Note: the descriptors shall define additional endpoints too. */
  #define USE_RX_IT_EP_0           0
 #endif
 #if ( HID_INTERFACE_COUNT > 1 )
  #define HID_IFC_1                1
  #define USE_RX_IT_EP_1           1
 #endif
 #if ( HID_INTERFACE_COUNT > 2 )
  #define HID_IFC_2                2
  #define USE_RX_IT_EP_2           1
 #endif
 #if ( HID_INTERFACE_COUNT > 3 )
  #define HID_IFC_3                3
  #define USE_RX_IT_EP_3           1
 #endif

#endif /* #if ( USBD_DEVICE_CLASS == USBD_DEVICE_CLASS_HIDGENERIC ) */

/* Define the maximum number of reports supported. */
#define MAX_NO_OF_REPORTS         5

/* Define the maximum length of a report. (Excluding report id prefix.) */
#define MAX_REPORT_LENGTH         64

#define HID_TXTASK_PRIORITY       OAL_LOW_PRIORITY
#define HID_TXTASK_STACK_SIZE     256

#define HID_RXTASK_PRIORITY       OAL_NORMAL_PRIORITY
#define HID_RXTASK_STACK_SIZE     256

#ifdef __cplusplus
}
#endif

#endif /* ifndef _CONFIG_USBD_HID_H_ */
