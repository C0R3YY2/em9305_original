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
#ifndef _USBD_STD_H_
#define _USBD_STD_H_


/* World-wide declarations for the STD module.

    This file contains all world-wide public declarations of the standard module.
    The standard mosule is responsible to implement handlers for standard
    USB requests and everyting tightly related to these. */

#include "../../../oal/oal_task.h"
#include "../../../oal/oal_mutex.h"

#include "../../../version/ver_usbd.h"
#if VER_USBD_MAJOR != 3 || VER_USBD_MINOR != 31
 #error "Incorrect USBD version."
#endif

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ************************ Macro definitions ***********************************
 *****************************************************************************/


/* Invalid endpoint handle value.
    Use this value to initialize endpoint handles. */
#define USBD_INVALID_EP_HANDLE_VALUE ( (uint16_t)0xff00u )


/* Invalid endpoint handle value.
    Use this value to initialize endpoint handles. */
#define USBD_INVALID_IFC_NDX         ( (uint8_t)0xffu )

/* Bitmask for the Request type field of the setup packet (bmRequestType) */
#define USBRQT_DIR_IN                ( 1u << 7 ) /* IN request */
#define USBRQT_DIR_OUT               ( 0u << 7 ) /* OUT request */
#define USBRQT_TYP_STD               ( 0u << 5 ) /* Standard request */
#define USBRQT_TYP_CLASS             ( 1u << 5 ) /* Class-specific request */
#define USBRQT_TYP_VENDOR            ( 2u << 5 ) /* Vendor-specific request */
#define USBRQT_TYP_MASK              ( 3u << 5 ) /* Mask for request type bits */
#define USBRQT_RCP_DEVICE            ( 0u << 0 ) /* Recipient is the device */
#define USBRQT_RCP_IFC               ( 1u << 0 ) /* Recipient is an interface */
#define USBRQT_RCP_EP                ( 2u << 0 ) /* Recipient is an endpoint */
#define USBRQT_RCP_OTHER             ( 3u << 0 ) /* Recipient is something else */
#define USBRQT_RCP_MASK              ( 3u << 0 ) /* Mask for recipient bits */

/* Values for the request filed of the setup packet.(bmRequest). */
#define USBRQ_GET_STATUS             0u  /* Get status of something */
#define USBRQ_CLEAR_FEATURE          1u  /* Clear a feature */
#define USBRQ_SET_FEATURE            3u  /* Set a feature */
#define USBRQ_SET_ADDRESS            5u  /* Set USB address of the device */
#define USBRQ_GET_DESCRIPTOR         6u  /* Read out a descriptor */
#define USBRQ_SET_DESCRIPTOR         7u  /* Change a descriptor */
#define USBRQ_GET_CONFIGURATION      8u  /* Read out the ID of the active configuration */
#define USBRQ_SET_CONFIGURATION      9u  /* Change the active configuration */
#define USBRQ_GET_INTERFACE          10u /* Read the state of an interface */
#define USBRQ_SET_INTERFACE          11u /* Set alternate setting of an interface */
#define USBRQ_SYNCH_FRAME            12u /* Set and report ISO endpoint sych frame */


/* Standard USB descriptor type vaules.

    Used in configuration data and in standared request processing.*/
#define STDD_DEVICE                  0x1u /* Device descriptor */
#define STDD_CONFIG                  0x2u /* Configuration descriptor */
#define STDD_STRING                  0x3u /* String descriptor */
#define STDD_INTERFACE               0x4u /* Interface descriptor */
#define STDD_ENDPOINT                0x5u /* Endpoint descriptor */
#define STDD_DEV_QUALIF              0x6u /* Device qualifier descriptor */
#define STDD_OTHER_SPEED             0x7u /* Other speed configuration descriptor */
#define STDD_OTG                     0x9u /* On-The-Go descriptor */
#define STDD_IFC_ASSOC               0xbu /* Interface association descriptor */


/* USB configuration descriptor attribute masks.

  Values for the attrib field of the configuration descriptor. */
#define CFGD_ATTR_BUS_PWR            ( 3u << 6 ) /* devide ic BUS powered */
#define CFGD_ATTR_SELF_PWR           ( 1u << 6 ) /* device is self powered */
#define CFGD_ATTR_RWAKEUP            ( 1u << 5 ) /* device can signal remote wakeup */

/* Endpoint direction specifyers */
/* IN endpoint (device to host) */
#define EPD_DIR_TX                   0x80

/* Out endpoint (host todevice) */
#define EPD_DIR_RX                   0

/* Control endpoint */
#define EPD_ATTR_CTRL                0

/* Isochronous endpoint. */
#define EPD_ATTR_ISO                 1

/* Bulk endpoint. */
#define EPD_ATTR_BULK                2

/* Interrupt endpoint. */
#define EPD_ATTR_INT                 3

/* Iso endpoint synchronisation type: none */
#define EPD_ATTR_ISO_SYNC_NONE       ( 0u << 2 )

/* Iso endpoint synchronisation type: asynchronous */
#define EPD_ATTR_ISO_SYNC_ASYNC      ( 1u << 2 )

/* Iso endpoint synchronisation type: adaptive */
#define EPD_ATTR_ISO_SYNC_ADAPT      ( 2u << 2 )

/* Iso endpoint synchronisation type: synchronous */
#define EPD_ATTR_ISO_SYNC_SYNC       ( 3u << 2 )

/* Iso endpoint usage type: data endpoint */
#define EPD_ATTR_ISO_USAGE_DATA      ( 0u << 4 )

/* Iso endpoint usage type: feedback endpoint */
#define EPD_ATTR_ISO_USAGE_FEEDB     ( 1u << 4 )

/* Iso endpoint usage type: explicite feedback endpoint */
#define EPD_ATTR_ISO_USAGE_EFEEDB    ( 2u << 4 )

/* Standard USB feature selector values. */
#define FEAT_ENDPOINT_HALT           0u /* Endpoin halt feature selector */
#define FEAT_DEVICE_REMOTE_WAKEUP    1u /* Remote wakeup feature selector */
#define FEAT_DEVICE_TEST_MODE        2u /* Tewst Mode feature selector */
#define FEAT_SEL_B_HNP_ENABLE        3u /* OTG A-Device enebles B-device to become a host  */
#define FEAT_SEL_A_HNP_SUPPORT       4u /* OTG A-Device informs B-Device about supporting HNP */

/* Event used by the control task. */
#define USBD_STATE_EVENT_FLAG        1

#if ( defined ( OAL_TASK_DELETE_SELF ) && ( OAL_TASK_DELETE_SELF != 0 ) )

/* Event flag for deleting control task */
 #define USBD_CONTROL_TASK_DELETE_EVENT_FLAG 2
#endif

/* Events for the ep0_task. These flags must not be the same. */
#define USBD_STPRX_EVENT_FLAG                1
#define USBD_EP0_EVENT_FLAG                  2

#if ( defined ( OAL_TASK_DELETE_SELF ) && ( OAL_TASK_DELETE_SELF != 0 ) )

/* Event flag for deleting EP0 task */
 #define USBD_EP0_TASK_DELETE_EVENT_FLAG 4
#endif

/* OS descriptor string index */
#define USBD_GET_MS_OS_STRING_IDX        0xEEu
#define USBD_STRING_LANG_EN              0x0409u /* English */

/* Get Status index values */
#define USBD_GET_STATUS_OTG_STATUS       0xF000


/******************************************************************************
 ************************ Type definitions ************************************
 *****************************************************************************/

/*Return values for ep0 callbacks. */
typedef enum
{
  clbstc_error     /* Error encountered, stop endpoint. */
  , clbstc_in      /* Start IN transfer. */
  , clbstc_out     /* Start OUT transfer. */
} usbd_callback_state_t;


/* Strucutre to hold setup transaction data.
    Used in ep0 callback functions. */
typedef struct
{
  uint8_t   bmRequestType;
  uint8_t   bRequest;
  uint16_t  wValue;
  uint16_t  wIndex;
  uint16_t  wLength;
} usbd_setup_data_t;


/* Endpoint descriptor.

    Holds information about an endpoint. The driver uses this to manage the
    endpoint. It is only world wide public because class drivers need read only
    access to this information during initialization. */
typedef struct usbd_ep_info_s
{
  usbd_transfer_t       * tr;
  struct usbd_ep_info_s * next;
#if USBD_ISOCHRONOUS_SUPPORT
  rngbuf_t              * fifo;
#endif
  usbd_ep_handle_t        eph;
  usbd_hw_ep_info_t       hw_info;
  uint16_t                psize;
  uint8_t                 ep_type;
  uint8_t                 addr;
  uint8_t                 age;
  uint8_t                 halted;
} usbd_ep_info_t;

typedef enum
{
  vbst_off
  , vbst_to_on
  , vbst_to_off
  , vbst_on
} usbd_vbus_state_t;

typedef enum
{
  rst_stop
  , rst_run
  , rst_to_run
  , rst_to_stop
} usbd_run_state_t;


/* Class-driver call-back function type.

    This type defines the class-driver call-back function. It is called by
    the standard request handler task when the USB configuration of the
    device is changeing. The change can be triggered by the following events:
      -bus reset
      -set configuration request sent by the host
      -set interface request sent by the host */
typedef void  usbd_cdrv_cb_t ( const int is_started        /* 1 if interface is started */
                              , const usbd_ep_info_t * eps /* array with endpoint information for this interface */
                              , const int ifc_ndx          /* id field of interface descriptor */
                              , const int param );         /* parameter spefified by usdb_register_cdrv() */


/* Endpoint 0 call-back type.

    This type defines the EP0 (default pipe) call-back function. It is called
    by the standard request handler task and is used to extend the set of
    request the device is ablt to handle on EP0. */
typedef usbd_callback_state_t  usbd_ep0_cb_t ( const usbd_setup_data_t * stp
                                              , usbd_transfer_t * tr );


/* Descriptor parse extension callback. Using this call-back the class-driver
   can do class driver specific processing on the configuration data. If the
   return value is 0, the class driver accepst the intercafe. if not, the
   class driver denies using this interface. */
typedef int   usbd_parse_cb_t ( const uint8_t * start, const uint8_t * end );

typedef void  usbd_notify_state_change_cb_t ( const usbd_conn_state_t new_state );


/******************************************************************************
 ************************ Global variables ************************************
 *****************************************************************************/


/* Handle of endpoint 0.

    This variable can be used if some transfer needs to be done on EP0. Note:
    EP0 is a shared resource. The driver will lock the endpoint and report
    USBDERR_BUSY to avoid concurrency problems.*/
extern usbd_ep_handle_t  ep0_handle;

/* Pointer to the current configuration data. */
extern const usbd_config_t * usbd_config;

/* The current driver status see USBDST_XXX values. */
extern int  usbd_state;

/* Remote wakeup state. Nonzero if enabled. */
extern uint8_t  usbd_rw_en;

/* Setup packet received event. */
extern oal_event_t  usbd_stprx_event;


/* Event to signal stack state chage. (suspended, resume, reset, start, stop,
    vbus on, vbus off ) */
extern oal_event_t    usbd_state_event;

extern oal_task_id_t  usbd_ep0_task_id;
extern oal_task_id_t  usbd_control_task_id;

/* Endpoint list */
extern usbd_ep_info_t  usbd_ep_list[];


/* Synchronization object to make reconfiguration of an endpoints
           atomic.*/
extern oal_mutex_t        usbd_cfg_mutex;

extern usbd_vbus_state_t  usbd_vbus_state;

extern usbd_run_state_t   usbd_run_state;


/******************************************************************************
 ************************ Imported functions **********************************
 *****************************************************************************/


/******************************************************************************
 ************************ Exported functions **********************************
 *****************************************************************************/

/* Inicialize module. */
int usbd_std_init ( void );

/* Start module operation. */
int usbd_std_start ( void );

/* Stop module. */
int usbd_std_stop ( void );

/* Kill module (free resources, etc.) */
int usbd_std_delete ( void );

/* Read out setup data. Implemented by the low-level driver. */
void usbd_get_setup_data ( usbd_setup_data_t * setup_data );


/* Register a class driver

    This function registers a class driver. The class driver can specify what
    interface type it is able to drive and what call-back functions the driver
    shall call. drv_cb is used to tell the class-driver which endpoint handles
    it shall use for communication. ep0_cb is called to enable extednig the set
    of requests the device is able to handle on EP0. */
int usdb_register_cdrv ( uint8_t _class, uint8_t sclass, uint8_t proto
                        , usbd_cdrv_cb_t * drv_cb
                        , int param
                        , usbd_ep0_cb_t * ep0_cb
                        , usbd_parse_cb_t * parse_cb );

/* Register a class driver with state change notification call-back request */
int usdb_register_cdrv_state_change ( uint8_t _class, uint8_t sclass
                                     , uint8_t proto
                                     , usbd_cdrv_cb_t * drv_cb
                                     , int param
                                     , usbd_ep0_cb_t * ep0_cb
                                     , usbd_parse_cb_t * parse_cb
                                     , usbd_notify_state_change_cb_t * p_notify_state_change_cb );

/* Get active alternate interface under an interface index. */
uint8_t usbd_get_alternate_ifc_idx ( uint8_t ifc_idx );


/* This function shal be called by the low-level driver to notify the stack
   of a status change (suspend, wakeup, reset). */
void usbd_bus_state_chg_event ( int new_state );

/* Sets OTG mode on/off. */
void usbd_set_otg_mode ( uint8_t on );

/* Return OTG mode. */
uint8_t  usbd_get_otg_mode ( void );

/* The standard request handling task. */
OAL_TASK_FN( usbd_ep0_task );

/* USB bus state change handling task. */
OAL_TASK_FN( usbd_control_task );

#ifdef __cplusplus
}
#endif


#endif /* ifndef _USBD_STD_H_ */
