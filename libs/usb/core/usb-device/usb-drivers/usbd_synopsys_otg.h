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
#ifndef _USBD_SYNOPSYS_OTG_H_
#define _USBD_SYNOPSYS_OTG_H_


/************************************************************************
*  Include section
*************************************************************************/
#include "../../psp/include/psp_types.h"

#include "../../config/config_usbd_synopsys_otg.h"

#include "common/usbd.h"
#include "common/usbd_std.h"

#include "../../version/ver_usbd.h"
#if VER_USBD_MAJOR != 3
 #error "Invalid USBD version."
#endif

#include "../version/ver_usbd_synopsys_otg.h"
#if ( ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MAJOR != 1 ) || ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MINOR != 5 ) )
 #error "Invalid USBD_SYNOPSYS_SHAREDFIFO_OTG version."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
*  Defines section
*************************************************************************/
#define USBD_GET_MIN( val1, val2 ) ( ( val1 ) < ( val2 ) ? ( val1 ) : ( val2 ) )

#define USBD_EP0_INDEX             0u
#define USBD_EP_INVALID            0xFFu

#define USBD_EP0_PACKET_LENGTH_8   8u
#define USBD_EP0_PACKET_LENGTH_16  16u
#define USBD_EP0_PACKET_LENGTH_32  32u
#define USBD_EP0_PACKET_LENGTH_64  64u

#define USBD_EP0_MAX_PACKET_LENGTH USBD_EP0_PACKET_LENGTH_64

#define USBD_SETUP_PACKET_LENGTH   8u

/* Number of hardware endpoints */
#define USBD_NUM_OF_HW_EP0         1u
#define USBD_NUM_OF_HW_EP_IN       OTG_NUM_BD_EP
#define USBD_NUM_OF_HW_EP_OUT      OTG_NUM_BD_EP
#define USBD_NUM_OF_HW_EP_TOTAL    ( USBD_NUM_OF_HW_EP_IN + USBD_NUM_OF_HW_EP_OUT + USBD_NUM_OF_HW_EP0 )

#define USBD_MAX_TRANSFER_LENGTH   ( ( 1u << 19 ) - 1u )

#define USBD_EP_DIR_CONTROL        1u
#define USBD_EP_DIR_IN             2u
#define USBD_EP_DIR_OUT            3u

#define USBD_GET_HW_EP_INFO( p_ep_info ) ( &( ( p_ep_info )->hw_info ) )


/************************************************************************
* Type Definitions
*************************************************************************/


/************************************************************************
* Function Prototype Section
*************************************************************************/
void usbd_ep0_transfer_end ( usbd_ep_info_t * p_ep_info );
void usbd_epX_transfer_end ( usbd_ep_info_t * p_ep_info );
uint32_t usbd_ep_fifo_in_get_write_length ( usbd_ep_info_t * p_ep_info, uint32_t requested_length  );
void usbd_transfer_handle_rx_fifo_non_empty ( void );
void usbd_transfer_handle_out ( uint8_t ep_num );
void usbd_transfer_handle_in ( uint8_t ep_num );
int usbd_ep_fifo_configure ( uint32_t const otg_fifo_config[] );
int usbd_ep_fifo_flush_tx ( uint32_t fifo_num );
int usbd_ep_fifo_flush_rx ( void );
void usbd_ep_fifo_write ( uint8_t ep_num, uint8_t * p_buffer, uint32_t length );
void usbd_transfer_ep0_setup_start ( void );
int usbd_transfer_ep0_out_start ( usbd_ep_info_t * p_ep_info );
int usbd_transfer_ep0_in_start ( usbd_ep_info_t * p_ep_info );
int usbd_transfer_epX_start ( usbd_ep_info_t * p_ep_info );

int usbd_get_ep_info ( uint8_t ep_num, uint8_t ep_dir, usbd_ep_info_t * * pp_ep_info );
void usbd_ep0_handle_setup_done ( usbd_ep_info_t * p_ep_info );
void usbd_transfer_completed ( usbd_ep_info_t * p_ep_info, uint8_t from_int );


/************************************************************************
* Global Variables
*************************************************************************/
extern uint8_t  g_usbd_setup_data[USBD_SETUP_PACKET_LENGTH];


#ifdef __cplusplus
}
#endif


#endif /* ifndef _USBD_SYNOPSYS_OTG_H_*/
