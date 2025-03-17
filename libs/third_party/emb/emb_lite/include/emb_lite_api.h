////////////////////////////////////////////////////////////////////////////////
///
/// @file       emb_lite_api.h
///
/// @project    EMB
///
/// @brief      EM Bleu Lite APIs
///
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2024 EM Microelectronic
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef __EMB_LITE_API_H
#define __EMB_LITE_API_H

#include <stdint.h>
#include <qep.h>
#include <att_api.h>
#include <dm_api.h>
#include <wsf_buf.h>
#include <wsf_types.h>

/**
 * @defgroup emblite EMB Lite API
 * @brief EMBLite API
 *
 * This library is a Bluetooth LE Application wrapper add-on on top of the Bluetooth LE stack APIs
 * that provides a higher lever of abstraction to ease writing of an application. Currently it
 * exclusively supports BLE operations in Peripheral role and only limited selection of BLE features.
 * @{
 */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/
#define EMBLITE_START_SIG   (Q_USER_SIG)

enum
{
  EMBLITE_STACK_READY_SIG = EMBLITE_START_SIG,  /**< BLE Stack Ready Signal*/
  EMBLITE_STACK_INIT_FAIL_SIG,                  /**< BLE Stack Initialization Failed Signal*/
  EMBLITE_ADV_STARTED_SIG,                      /**< Start Advertising Signal*/
  EMBLITE_ADV_FAILED_SIG,                       /**< Advertising Failed Signal*/
  EMBLITE_ADV_STOPPED_SIG,                      /**< Advertising Stopped Signal*/
  EMBLITE_CONN_OPENED_SIG,                      /**< Connection Opened Signal*/
  EMBLITE_CONN_CLOSED_SIG,                      /**< Connection Closed Signal*/
  EMBLITE_CONN_UPDATED_SIG,                     /**< Connection Parameter Updated Signal*/
  EMBLITE_HANDLE_VALUE_CONFIRMED_SIG,           /**< ATT server handle value confirmation Signal*/
  EMBLITE_CCC_STATE_CHANGED_SIG,                /**< Client Characteristic Configuration changed Signal*/
  EMBLITE_SIG_SIGNALS_MAX                       /**< EMBLite Signal limit. Application QPC messages should start
                                                 from this number to avoid any collisions with EMBLite QPC messages*/
};

typedef uint8_t EMBLite_Signals;

enum
{
  // Internal signal for EMB LITE
  EMBLITE_CONN_UPDATE_TIMEOUT_IND = 0xA0,   /**< Connection Parameter Update Timeout Indicator*/
  EMBLITE_INTERNAL_MSG_MAX                  /**< EMBLite internal message limit. Application WSF messages should start
                                                 from this number to avoid any collisions with EMBLite internal messages*/
};

#define EMBLITE_USER_EVT_START  (EMBLITE_SIG_SIGNALS_MAX)

enum
{
    EMBLITE_SUCCESS = 0,
    EMBLITE_ADV_DATA_UNASSIGNED,  /**< Advertising Data not assigned */
    EMBLITE_PEER_ADDR_UNASSIGNED, /**< Advertising Peer Address not assigned */
    EMBLITE_NOT_LEGACY_MODE,      /**< Advertising not in Legacy mode */
};

typedef uint8_t EMBLite_Status_t;

typedef struct
{
    uint32_t   connUpdatePeriod;   /**< Connection period in ms before attempting connection parameter update */
    uint16_t   connIntervalMin;    /**< Minimum connection interval. */
    uint16_t   connIntervalMax;    /**< Maximum connection interval. */
    uint16_t   connLatency;        /**< Maximum peripheral latency. */
    uint16_t   supervisionTimeout; /**< Supervision timeout. */
    uint8_t    maxUpdateAttempts;  /**< Number of update attempts before giving up. */
} EMBLite_ConnectionUpdateParams_t;

/* Data structure for EMBLite Config */
typedef struct
{
    uint8_t           *deviceName;                          /**< Device Name */
    uint16_t          deviceNameLength;                     /**< Length of `deviceName` field */
    uint8_t           deviceNumberofPools;                  /**< Number of pools in the buffer pool description */
    wsfBufPoolDesc_t  *deviceBuffPoolDesc;                  /**< Buffer Pool Description */
    // Advertising Related Configurations
    uint16_t          advertisingDuration;                  /**< Duration of each advertising operation (ms). Duration of zero (0) means advertising would not stop until manually stopped. */
    uint16_t          advertisingInterval;                  /**< Advertising Interval (0.625ms increments). Min 20ms - Max 10.24 seconds. */
    uint8_t           *advertisingData;                     /**< Array of Advertising data */
    uint16_t          advertisingDataLength;                /**< Length of advertisingData */
    bool              advertisingIsScannable;               /**< Control for advertising ops to respond to scan requests */
    uint8_t           *advertisingScanData;                 /**< Array for Scan Response data */
    uint16_t          advertisingScanDataLength;            /**< Length of advertisingScanData */
    bool              advertisingIsDirected;                /**< Control for directed advertising. Peer Address must be specified. */
    uint8_t           *advertisingDirectedPeerAddress;      /**< BD Addr of peer device for directed advertising */
    // Connection Related Configurations
    bool              connectionsEnabled;                   /**< Control for connectable advertising */
    uint16_t          connectionsMaxAllowed;                /**< Max connections allowed by the application */
    uint16_t          connectionsDesiredMtuLength;          /**< Maximum ACL Length */
    bool              connectionsRequestUpdate;             /**< Control for app to request connection parameter update procedure after connection is established */
    EMBLite_ConnectionUpdateParams_t *requestedUpdateParams;/**< Data for peripheral requested connection update parameters */
    bool              connectionsAllowUpdates;              /**< Control to allow app to respond to connection parameter update procedure */
    EMBLite_ConnectionUpdateParams_t *updateParamsAllowed;  /**< Data for connection update parameters allowed */
} EMBLite_Config_t;

/* EMBLite Processable Message Types*/
typedef union
{
    wsfMsgHdr_t     hdr;      /**< Header structure */
    dmEvt_t         dm;       /**< Structure for Device Manager Events */
    attsCccEvt_t    ccc;      /**< Structure for ATTS Client Characteristic Configuration Events */
    attEvt_t        att;      /**< Structure for ATT Events */
} EMBLite_Msg_t;

/* BLE Subsystem Processor type */

typedef void (*EMBLite_BLESubsys_Proc_t)(EMBLite_Msg_t *pMessage);

/* App Signaler Task type */

typedef void (*AppSignaler_t)(EMBLite_Signals signal, void* pParams);

/**************************************************************************************************
  APIs
**************************************************************************************************/
/*************************************************************************************************/
 /*!
 *  \brief Configure the EMB Lite stack.
 *
 *  \param  EMBLite_Config_t Data structure with configuration parameters for the EMB Lite stack.
 *
 */
void EMBLite_ConfigureStack(EMBLite_Config_t *pCfg);

/*************************************************************************************************/
 /*!
 *  \brief Initialize the EMB Lite stack in Peripheral configuration.
 *
 *  \param  EMBLite_Config_t Data structure with configuration parameters for the EMB Lite stack.
 *
 */
void EMBLite_InitializeStack(EMBLite_Config_t *pCfg);

/*************************************************************************************************/
 /*!
 *  \brief Start running the EMB Lite stack in Peripheral configuration.
 *
 *  \param  EMBLite_Config_t Data structure with configuration parameters for the EMB Lite stack.
 *
 */
/*************************************************************************************************/
void EMBLite_StartStack(EMBLite_Config_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Start Advertising.
 *
 *  \param  EMBLite_Config_t Data structure with configuration parameters for the EMB Lite stack.
 *
 *  \return  Returns one of the following status code:
 *          - EMBLITE_SUCCESS if advertising is successfully started
 *          - EMBLITE_PEER_ADDR_UNASSIGNED in case no direct peer address has been previously assigned
 *          - EMBLITE_ADV_DATA_UNASSIGNED in case no advertising data has been previously assigned
 */
/*************************************************************************************************/
EMBLite_Status_t EMBLite_StartAdvertising(EMBLite_Config_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Stop Advertising.
 *
 *  \return Returns one of the following status code:
 *          - EMBLITE_SUCCESS if advertising is successfully stopped
 *          - EMBLITE_NOT_LEGACY_MODE if not in legacy mode
 */
/*************************************************************************************************/
EMBLite_Status_t EMBLite_StopAdvertising(void);

/*************************************************************************************************/
/*!
 *  \brief  Return the number of existing connections of the given role.
 *
 *  \return Number of connections, up to a max of 4 connections.
 */
/*************************************************************************************************/
uint8_t EMBLite_GetCurrentConnectionCount(void);

/*************************************************************************************************/
/*!
 *  \brief  Close the connection with the give connection identifier.
 *
 *  \param  connId    Connection identifier.
 *
 */
/*************************************************************************************************/
void EMBLite_CloseConnection(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Register an additional processor for Device Manager and ATT events. NOTE: These events
 *          are already handled by EMBLite, but with this API you can trigger more application
 *          level functionality from these events if desired. Please note that the registered
 *          function will be called from inside EMBLite, no additional WSF messaged or QPC signals
 *          will be generated.
 *
 *  \param  processor Additional processing function to run.
 */
/*************************************************************************************************/
void EMBLite_RegisterBLEProcessor(EMBLite_BLESubsys_Proc_t processor);

/*************************************************************************************************/
/*!
 *  \brief  Register a QPC Signal Generator to be used for sending EMB Lite Signals to a separate
 *          QPC context outside the context of the BLE task.
 *
 * \param  signaler    Handler for application signals task.
 */
/*************************************************************************************************/
void EMBLite_RegisterAppSignaler(AppSignaler_t signaler);

/** @} */

#endif // __EMB_LITE_API_H
