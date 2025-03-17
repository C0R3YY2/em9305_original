/***************************************************************************
 *
 *            Copyright (c) 2008-2020 by HCC Embedded
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
#ifndef _USBD_H_
#define _USBD_H_


/*  World-wide common declarations.

    This file contains world wide declarations common for all USB
    peripheral (device) drivers. */

#include "../../../api/api_usbd.h"
#include "../../../config/config_usbd.h"
#include "usbd_descriptor.h"
#include "../../../oal/oal_event.h"

#if USBD_ISOCHRONOUS_SUPPORT
 #include "../../../version/ver_hcc_rngbuf.h"
 #if VER_RNGBUF_MAJOR != 1
  #error "Unsupported version number."
 #endif
#endif

#if USBD_SOFTMR_SUPPORT
 #include "sof_timer.h"
 #include "../../../version/ver_usbd_sof_timer.h"
 #if VER_SOFTMR_MAJOR != 2
  #error "Unsupported SOFTMR version number."
 #endif
#endif

#include "../../../version/ver_usbd.h"
#if VER_USBD_MAJOR != 3 || VER_USBD_MINOR != 31
 #error "Invalid USBD version."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ************************ Type definitions ************************************
 *****************************************************************************/

/*Endpoint identifyer type. */
typedef uint16_t  usbd_ep_handle_t;


/* USB transfer descriptor.
    This structure is used to describe/manage an USB transfer. */
typedef struct
{
  usbd_ep_handle_t   eph;       /* Endpoint handle. */
  uint8_t          * buffer;    /* Data buffer*/
  uint32_t           length;    /* Number of bytes to send/receive */
  uint32_t           csize;     /* Number of bytes already sent/received */
  uint32_t           slength;   /* Used when sending zero length packet. */
  uint32_t           scsize;    /* Used when sending zero length packet. */
  oal_event_t      * event;     /* Event to notify caller when transfer ends */
  oal_event_flags_t  ev_flags;  /* Event flags to use. */
  oal_task_id_t      task_id;   /* task to be notifyed */
  volatile int       state;     /* State of transfer */
  uint8_t            dir;       /* Direction of transfer (#USBDTRDIR_IN #USBDTRDIR_OUT) */
  uint8_t            zp_needed; /* Set to one if transfer shall be closed with short packet. */
} usbd_transfer_t;


/******************************************************************************
 ************************ Macro definitions ***********************************
 *****************************************************************************/


/* USB transfer state values. Used by the low-level layer to
           talk to the common part. */
#define USBDTRST_DONE      0       /* Transfer ended. */
#define USBDTRST_BUSY      1       /* Low-level is busy. */
#define USBDTRST_CHK       2       /* Check status. */
#define USBDTRST_SHORTPK   3       /* Ended with short packet. */
#define USBDTRST_EP_KILLED 4       /* Failed, endpoint gone. */
#define USBDTRST_COMM      5       /* Communication error. */

/* Transfer direction values */
#define USBDTRDIR_SETUP    2   /* Setup transaction */
#define USBDTRDIR_HS_IN    3   /* Hand-shake in (rx 0 length packet) */
#define USBDTRDIR_HS_OUT   4   /* Hand-shake out (tx 0 length packet) */

/* Error codes. */
#define USBDERR_NONE       0       /* no error */
#define USBDERR_BUSY       1       /* endpoint is busy */
#define USBDERR_INVALIDEP  3       /* invalid endpoint */
#define USBDERR_NOTREADY   5       /* transfer can not be started */
#define USBDERR_INTERNAL   6       /* internal error (shall only happen
                                        during development) */
#define USBDERR_COMM       7       /* communication error */

/* Transfer direction values. */
#define USBDTRDIR_IN       0       /* peripheral to host */
#define USBDTRDIR_OUT      1       /* host to peripheral */

/* Endpoint handle management. */
#define USBD_EPH_AGE( eph )         ( (uint8_t)( ( eph ) >> 8 ) )                    /* Endpoint handle -> age. */
#define USBD_EPH_NDX( eph )         ( (uint8_t)( ( eph ) & 0x00ff ) )                /* Endpoint handle -> index*/
#define USBD_EPH_CREATE( age, ndx ) (usbd_ep_handle_t)( ( ( age ) << 8 ) | ( ndx ) ) /* Cerate a handle value from the age and index. */

#define USBD_SET_TR_EVENT( tr, ev, flg, tid ) \
  ( ( tr ).event = ( ev ), ( tr ).ev_flags = ( flg ), ( tr ).task_id = ( tid ) )

#define NOTIFY_TR_FROM_IRQ( tr ) \
  (void)( ( tr )->event != NULL ? \
         oal_event_set_int( ( tr )->event \
                           , ( tr )->ev_flags \
                           , ( tr )->task_id ) \
         : 0 )
#define NOTIFY_TR( tr ) \
  (void)( ( tr )->event != NULL ? \
         oal_event_set( ( tr )->event \
                       , ( tr )->ev_flags \
                       , ( tr )->task_id ) \
         : 0 )

/* Test mode related macros */
#define USBD_TEST_J_MODE      1
#define USBD_TEST_K_MODE      2
#define USBD_TEST_SE0_NAK     3
#define USBD_TEST_PACKET_MODE 4


/******************************************************************************
 ************************ Inported functions **********************************
 *****************************************************************************/


/******************************************************************************
 ************************ Exported functions **********************************
 *****************************************************************************/

/* Initialize transfer sturcture to defaultvalue. */
void usbd_init_tr_struct ( usbd_transfer_t * tr );


/* Start a non-blocking transfer.

    Using this function a transfer can be requested to be done on the USB bus.
    The function will "enque" the request and return and will not wayt till the
    transfer is done.
    The return value can be used to determine if the transfer is successfully
    enqued or not. If yes, the event specifyed in the transfer descriptor
    will be set if the transfer end due to any reason. If the transfer can not
    be enqued the event will newer be set.*/
usbd_error_t usbd_transfer ( usbd_transfer_t * tr );


/* Start a blocking transfer.

    Using this function a transfer can be requested to be done on the USB bus.
    The function will not return till the transfer is not finished and will
    block execution of the calling task.*/
usbd_error_t usbd_transfer_b ( usbd_transfer_t * tr );


/* Query status of a transfer.

    This function is to be used with non-blocking transfers. It works well for
    blocking transfers too but uting it that way is pointless since the
    usbd_transfer_b() will return the same status as this call.
    This function allocates CPU time for transfer management and must be called
    by the transfer initiator (application or class-driver) when the event
    specifyed in the transfer descriptor becomes set. */
usbd_error_t usbd_transfer_status ( usbd_transfer_t * tr );


/* Halt a bulk or interrupt endpoint.

    This function can be used to report an error to the USB host. After the call
    any communication to the specifyedendpoint will result in a STALL handshake
    and thus will fail with an error condition on the host side.*/
void usbd_set_halt ( usbd_ep_handle_t ep );


/* Enable host to clear halted state.

    When the firmware is recovered the error condition due to the endpoint has
    been halted it shall call this function. This will enable the host to
    restart communication on the endpoint.*/
void usbd_clr_halt ( usbd_ep_handle_t ep );


/* Check if endpoint is halted.

    A halted endpoint can be restarted ony by the host. This function can be
    used to see if the host restarted the endpoint or not. */
int usbd_get_halt ( usbd_ep_handle_t ep );


/* Return current value of frame counter.

    While the bus is not suspended, the host is sending a SOF packet at the
    start of each frame. Any hadware counts the number of SOF:s seen on the
    bus. The counter is at least 11 bits wide.*/
uint16_t usbd_get_frame_ctr ( void );

#if USBD_ISOCHRONOUS_SUPPORT

/* Attach a FIFO to an endpoint. */
int usbd_stream_attach ( usbd_ep_handle_t ep, rngbuf_t * fifo );

/* Detach a FIFO from an endpoint. */
int usbd_stream_detach ( usbd_ep_handle_t ep );
#endif

#ifdef __cplusplus
}
#endif

#include "usbdi_dev.h"
#include "usbd_std.h"

#endif /* ifndef _USBD_H_ */
