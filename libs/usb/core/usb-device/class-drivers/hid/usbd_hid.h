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
#ifndef _USBD_HID_H_
#define _USBD_HID_H_

#include "../../../psp/include/psp_types.h"

#include "../../usb-drivers/common/usbd.h"
#include "../../../oal/oal_task.h"
#include "../../../config/config_usbd_hid.h"

#include "../../../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver header version."
#endif

#define MAX_IFC_SUPPORTED 4

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  void    * start_addr;
  uint16_t  size;
} descriptor_info_t;

typedef struct
{
  void ( * cb )( uint8_t ntf, int param );
  int  param;
} usbd_hid_cb_def_t;

typedef enum
{
  rpt_invalid
  , rpt_in
  , rpt_out
  , rpt_feature
} hid_report_type;

#define HID_INVALID_REPORT_NDX 0xff

#define REPORT_ABSOLUTE        0
#define REPORT_RELATIVE        1

#define ST_WAIT_CFG            0
#define ST_CHK_REPORT          1
#define ST_SENDING             2
#define ST_START_RX            3
#define ST_RECEIVING           4

/* Notifications in In and OUT callbacks. */
#define USBD_HID_READ          0
#define USBD_HID_DONE          1


#define USE_RX_IT_EP           ( USE_RX_IT_EP_0 | USE_RX_IT_EP_1 | USE_RX_IT_EP_2 | USE_RX_IT_EP_3 )

uint8_t usbd_hid_add_report ( uint8_t ifc_ndx, uint8_t id, hid_report_type type, uint16_t size, uint8_t relative, uint16_t idle_time, usbd_hid_cb_def_t * rx_cb );
void usbd_hid_write_report ( uint8_t r, uint8_t * data );
void usbd_hid_read_report ( uint8_t r, uint8_t * data );
void usbd_hid_get_report_received_size ( uint8_t r, uint16_t * length );
uint8_t usbd_hid_report_pending ( uint8_t r );
uint16_t usbd_hid_get_idle_time ( uint8_t r );
void usbd_hid_tx_ntf ( void );

usbd_callback_state_t usbd_hid_ep0_event ( const usbd_setup_data_t * stp
                                          , usbd_transfer_t * tr );
void usbd_hid_cdrv_cb ( const int is_stared, const usbd_ep_info_t * eps, const int ifc_info, const int param );

#if USE_RX_IT_EP
OAL_TASK_FN( usbd_hidrx_task );
#endif
OAL_TASK_FN( usbd_hidtx_task );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _USBD_HID_H_ */
