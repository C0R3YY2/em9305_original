/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Application framework API.
 *
 *  Copyright (c) 2011-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef APP_API_H
#define APP_API_H

#include "wsf_os.h"
#include "dm_api.h"
#include "att_api.h"
#include "app_db.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup APP_FRAMEWORK_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name App Advertising Mode
 * Discoverable/connectable mode used by function \ref AppAdvStart.
 */
/**@{*/
#define APP_MODE_CONNECTABLE        0          /*!< Connectable mode */
#define APP_MODE_DISCOVERABLE       1          /*!< Discoverable mode */
#define APP_MODE_AUTO_INIT          2          /*!< Automatically configure mode based on bonding info */
#define APP_MODE_NONE               255        /*!< For internal use only */

/*! \brief Number of Discoverable/connectable modes */
#define APP_NUM_MODES               2
/**@}*/

/*! \brief Advertising states */
enum
{
  APP_ADV_STATE1,                              /*!< Advertising state 1 */
  APP_ADV_STATE2,                              /*!< Advertising state 2 */
  APP_ADV_STATE3,                              /*!< Advertising state 3 */
  APP_ADV_STOPPED                              /*!< Advertising stopped */
};

/*! \brief Advertising and scan data storage locations */
enum
{
  APP_ADV_DATA_CONNECTABLE,                    /*!< Advertising data for connectable mode */
  APP_SCAN_DATA_CONNECTABLE,                   /*!< Scan data for connectable mode */
  APP_ADV_DATA_DISCOVERABLE,                   /*!< Advertising data for discoverable mode */
  APP_SCAN_DATA_DISCOVERABLE,                  /*!< Scan data for discoverable mode */
  APP_NUM_DATA_LOCATIONS                       /*!< number of data storage locations. */
};

/*! \brief Number of advertising configurations */
#define APP_ADV_NUM_CFG             APP_ADV_STOPPED

/*! \brief Service discovery and configuration client status */
enum
{
  APP_DISC_INIT,                               /*!< No discovery or configuration complete */
  APP_DISC_READ_DATABASE_HASH,                 /*!< Read peer's database hash */
  APP_DISC_SEC_REQUIRED,                       /*!< Security required to complete configuration */
  APP_DISC_START,                              /*!< Service discovery started */
  APP_DISC_CMPL,                               /*!< Service discovery complete */
  APP_DISC_FAILED,                             /*!< Service discovery failed */
  APP_DISC_CFG_START,                          /*!< Service configuration started */
  APP_DISC_CFG_CONN_START,                     /*!< Configuration for connection setup started */
  APP_DISC_CFG_CMPL,                           /*!< Service configuration complete */
};

/*! \brief Actions for incoming requests */
enum
{
  APP_ACT_ACCEPT,                              /*!< Accept incoming request */
  APP_ACT_REJECT,                              /*!< Reject incoming request */
  APP_ACT_NONE                                 /*!< Do nothing - app will handle incoming request */
};

#define APP_RESOLVE_ADV_RPA         0          /*!< Resolving the advertiser's RPA (AdvA) */
#define APP_RESOLVE_DIRECT_RPA      1          /*!< Resolving RPA the directed advertisement is being
                                                    directed to (InitA) */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Configurable parameters for advertising */
typedef struct
{
  uint16_t    advDuration[APP_ADV_NUM_CFG];    /*!< Advertising durations in ms */
  uint16_t    advInterval[APP_ADV_NUM_CFG];    /*!< Advertising intervals in 0.625 ms units (20 ms to 10.24 s). */
} appAdvCfg_t;

/*! \brief Configurable parameters for extended and periodic advertising */
typedef struct
{
  uint16_t    advDuration[DM_NUM_ADV_SETS];    /*!< Advertising durations for extended advertising in ms */
  uint16_t    advInterval[DM_NUM_ADV_SETS];    /*!< Advertising intervals for extended advertising in 0.625
                                                    ms units (20 ms to 10.24 s). */
  uint8_t     maxEaEvents[DM_NUM_ADV_SETS];    /*!< Maximum number of extended advertising events Controller
                                                    will send prior to terminating extended advertising */
  bool        useLegacyPdu[DM_NUM_ADV_SETS];   /*!< Whether to use legacy advertising PDUs with extended
                                                    advertising. If set to true then length of advertising
                                                    data cannot exceed 31 octets. */
  uint16_t    perAdvInterval[DM_NUM_ADV_SETS]; /*!< Advertising intervals for periodic advertising in 1.25
                                                    ms units (7.5 ms to 81.91875 s). */
} appExtAdvCfg_t;

/*! \brief Configurable parameters for extended and periodic advertising */
typedef struct
{
  uint8_t     numSubevents[DM_NUM_ADV_SETS];    /*!< The number of subevents in the advertisement
                                                     (0 for no PAWR) */
  uint8_t     interval[DM_NUM_ADV_SETS];        /*!< Subevent intervals for PAWR subevents in 1.25
                                                     ms units (7.5 ms to 318.75 ms). */
  uint8_t     slotDelay[DM_NUM_ADV_SETS];       /*!< Time between start of advertising and the first
                                                     response slot 1.25 ms (7.5 ms to 318.75 ms). */
  uint8_t     slotSpacing[DM_NUM_ADV_SETS];     /*!< The time between two consecutive response slots
                                                     .125 ms (.125 ms to 31.875 ms). */
  uint8_t     numSlots[DM_NUM_ADV_SETS];        /*!< The number of response slots (0x01 to 0xFF) */
} appPawrAdvCfg_t;

/*! \brief Configurable parameters for slave */
typedef struct
{
  uint8_t     connMax;                         /*!< Maximum connections */
} appSlaveCfg_t;

/*! \brief Configurable parameters for master */
typedef struct
{
  uint16_t    scanInterval;                    /*!< The scan interval, in 0.625 ms units */
  uint16_t    scanWindow;                      /*!< The scan window, in 0.625 ms units.   Must be
                                                    less than or equal to scan interval. */
  uint16_t    scanDuration;                    /*!< The scan duration in ms.  Set to zero to scan
                                                    until stopped. */
  uint8_t     discMode;                        /*!< The GAP discovery mode (general, limited, or none) */
  uint8_t     scanType;                        /*!< The scan type (active or passive) */
} appMasterCfg_t;

/*! \brief Configurable parameters for extended master */
typedef struct
{
  uint16_t    scanInterval[DM_NUM_PHYS];       /*!< The scan interval, in 0.625 ms units */
  uint16_t    scanWindow[DM_NUM_PHYS];         /*!< The scan window, in 0.625 ms units.   Must be
                                                    less than or equal to scan interval. */
  uint16_t    scanDuration;                    /*!< The scan duration in ms.  Set to zero or both duration
                                                    and period to non-zero to scan until stopped. */
  uint16_t    scanPeriod;                      /*!< The scan period, in 1.28 sec units.  Set to zero to
                                                    disable periodic scanning. */
  uint8_t     discMode;                        /*!< The GAP discovery mode (general, limited, or none) */
  uint8_t     scanType[DM_NUM_PHYS];           /*!< The scan type (active or passive) */
} appExtMasterCfg_t;

/*! \brief Configurable parameters for security */
typedef struct
{
  uint8_t     auth;                            /*!< Authentication and bonding flags */
  uint8_t     iKeyDist;                        /*!< Initiator key distribution flags */
  uint8_t     rKeyDist;                        /*!< Responder key distribution flags */
  bool        oob;                             /*!< true if Out-of-band pairing data is present */
  bool        initiateSec;                     /*!< true to initiate security upon connection */
} appSecCfg_t;

/*! \brief Configurable parameters for connection parameter update */
typedef struct
{
  wsfTimerTicks_t idlePeriod;                  /*!< Connection idle period in ms before attempting
                                                    connection parameter update; set to zero to disable */
  uint16_t    connIntervalMin;                 /*!< Minimum connection interval in 1.25ms units */
  uint16_t    connIntervalMax;                 /*!< Maximum connection interval in 1.25ms units */
  uint16_t    connLatency;                     /*!< Connection latency */
  uint16_t    supTimeout;                      /*!< Supervision timeout in 10ms units */
  uint8_t     maxAttempts;                     /*!< Number of update attempts before giving up */
} appUpdateCfg_t;

/*! \brief Configurable parameters for incoming request actions */
typedef struct
{
  uint8_t     remConnParamReqAct;              /*!< Action for the remote connection parameter request */
} appReqActCfg_t;

/*! \brief Configurable parameters for service and characteristic discovery */
typedef struct
{
  bool        waitForSec;                      /*!< true to wait for a secure connection before initiating
                                                    discovery */
  bool        readDbHash;                      /*!< true to try and read peer's database hash rather than
                                                    perform service discovery.  */
} appDiscCfg_t;

/*! \brief Configurable parameters for application */
typedef struct
{
  bool        abortDisc;                       /*!< true to abort service discovery if service not found */
  bool        disconnect;                      /*!< true to disconnect if ATT transaction times out */
} appCfg_t;

/*! \brief Device information data type */
typedef struct
{
  bdAddr_t    addr;                            /*!< Peer device address */
  uint8_t     addrType;                        /*!< Peer address type */
  uint8_t     directAddrType;                  /*!< Address directed advertisement is addressed to */
  bdAddr_t    directAddr;                      /*!< Type of address directed advertisement is addressed to */
} appDevInfo_t;

/*!
 *  \brief  Service discovery and configuration callback.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Service or configuration status.
 *
 *  \return None.
 */
typedef void (*appDiscCback_t)(dmConnId_t connId, uint8_t status);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief This is a pointer to the advertising configurable parameters used by the application.
 *  If advertising is used, the application must set this variable during system initialization.
 */
extern appAdvCfg_t *pAppAdvCfg;

/*! \brief This is a pointer to the extended advertising configurable parameters used by the
 *  application.  If extended advertising is used, the application must set this variable during
 *  system initialization.
 */
extern appExtAdvCfg_t *pAppExtAdvCfg;

/*! \brief This is a pointer to the periodic advertising with response configuration used by the
 *  application.  If PAWR advertising is used, the application must set this variable during
 *  system initialization.
 */
extern appPawrAdvCfg_t *pAppPawrCfg;

/*! \brief This is a pointer to the slave configurable parameters used by the application.
 *  If slave mode is used, the application must set this variable during system initialization.
 */
extern appSlaveCfg_t *pAppSlaveCfg;

/*! \brief This is a pointer to the master configurable parameters used by the application.
 *  If master mode is used, the application must set this variable during system initialization.
 */
extern appMasterCfg_t *pAppMasterCfg;

/*! \brief This is a pointer to the extended master configurable parameters used by the application.
 *  If master mode is used, the application must set this variable during system initialization.
 */
extern appExtMasterCfg_t *pAppExtMasterCfg;

/*! \brief This is a pointer to the security-related configurable parameters used by the application.
 *   The application must set this variable during system initialization.
 */
extern appSecCfg_t *pAppSecCfg;

/*! \brief This is a pointer to the connection parameter update parameters used by the application.
 *  The application must set this variable during system initialization.
 */
extern appUpdateCfg_t *pAppUpdateCfg;

/*! \brief This is a pointer to the discovery parameters used by the application.  The application
 *  must set this variable during system initialization.
 */
extern appDiscCfg_t *pAppDiscCfg;

/*! \brief This is a pointer to the application parameters used by the application.  The application
 * must set this variable during system initialization.
 */
extern appCfg_t *pAppCfg;

/*! \brief This is a pointer to the master incoming request actions used by the application.  The
 * application must set this variable during system initialization.
 */
extern appReqActCfg_t *pAppMasterReqActCfg;

/*! \brief This is a pointer to the master incoming request actions used by the application.
 * The application must set this variable during system initialization.
 */
extern appReqActCfg_t *pAppSlaveReqActCfg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name App Initialization Functions
 * Application role initialization.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the App Framework for operation as a Bluetooth LE slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the App Framework for operation as a Bluetooth LE master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppMasterInit(void);

/**@}*/

/** \name App Advertising Functions
 * Configure and send legacy, extended and periodic advertisements.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Set advertising type.
 *
 *  \param  advType   Advertising type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSetAdvType(uint8_t advType);

/*************************************************************************************************/
/*!
 *  \brief  Set advertising peer address and its type.
 *
 *  \param  peerAddrType  Peer address type.
 *  \param  pPeerAddr     Peer address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSetAdvPeerAddr(uint8_t peerAddrType, uint8_t *pPeerAddr);

/*************************************************************************************************/
/*!
 *  \brief  Set advertising or scan data.  Separate advertising and scan data can be set for
 *          connectable and discoverable modes.  The application must allocate and maintain
 *          the memory pointed to by pData while the device is advertising.
 *
 *  \param  location  Data location.
 *  \param  len       Length of the data.  Maximum length is 31 bytes.
 *  \param  pData     Pointer to the data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppAdvSetData(uint8_t location, uint8_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of an advertising data element in the advertising or scan
 *          response data.  If the element already exists in the data then it is replaced
 *          with the new value.  If the element does not exist in the data it is appended
 *          to it, space permitting.
 *
 *          There is special handling for the device name (AD type DM_ADV_TYPE_LOCAL_NAME).
 *          If the name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  location  Data location.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.  Maximum length is 29 bytes.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added to the data, false otherwise.
 */
/*************************************************************************************************/
bool AppAdvSetAdValue(uint8_t location, uint8_t adType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Start advertising using the parameters for the given mode.
 *
 *  \param  mode      Discoverable/connectable mode.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppAdvStart(uint8_t mode);

/*************************************************************************************************/
/*!
 *  \brief  Stop advertising.  The device will no longer be connectable or discoverable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppAdvStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Accept a connection to a peer device with the given address.
 *
 *  \param  advType   Advertising type.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *  \param  dbHdl     Device database handle.
 *
 *  \return Connection identifier.
 */
 /*************************************************************************************************/
dmConnId_t AppConnAccept(uint8_t advType, uint8_t addrType, uint8_t *pAddr, appDbHdl_t dbHdl);

/*************************************************************************************************/
/*!
 *  \brief  Check if the local device's currently advertising.
 *
 *  \return true if device's advertising. false, otherwise.
 */
/*************************************************************************************************/
bool AppSlaveIsAdvertising(void);

/*************************************************************************************************/
/*!
 *  \brief  Clear all bonding information on a slave device and make it bondable.
 *
 *  \return None.
 *
 *  \Note   This API should not be used when Advertising (other than periodic advertising) is
 *          enabled. Otherwise, clearing the resolving list in the Controller may fail.
 */
/*************************************************************************************************/
void AppSlaveClearAllBondingInfo(void);

/*************************************************************************************************/
/*!
 *  \brief  Set advertising peer address and its type for the given advertising set.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  peerAddrType  Peer address type.
 *  \param  pPeerAddr     Peer address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtSetAdvPeerAddr(uint8_t advHandle, uint8_t peerAddrType, uint8_t *pPeerAddr);

/*************************************************************************************************/
/*!
 *  \brief  Set extended advertising data.
 *
 *  \param  advHandle Advertising handle.
 *  \param  location  Data location.
 *  \param  len       Length of the data.  Maximum length is 31 bytes if advertising set uses
 *                    legacy advertising PDUs with extended advertising.
 *  \param  pData     Pointer to the data.
 *  \param  bufLen    Length of the data buffer maintained by Application.  Minimum length is
 *                    31 bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtAdvSetData(uint8_t advHandle, uint8_t location, uint16_t len, uint8_t *pData, uint16_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Start extended advertising using the parameters for the given mode.
 *
 *  \param  numSets     Number of advertising sets.
 *  \param  pAdvHandles Advertising handles array.
 *  \param  mode        Discoverable/connectable mode.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtAdvStart(uint8_t numSets, uint8_t *pAdvHandles, uint8_t mode);

/*************************************************************************************************/
/*!
 *  \brief  Stop extended advertising.  If the number of sets is set to 0 then all advertising
 *          sets are disabled.
 *
 *  \param  numSets     Number of advertising sets.
 *  \param  pAdvHandles Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtAdvStop(uint8_t numSets, uint8_t *pAdvHandles);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of an advertising data element in the extended advertising or scan
 *          response data.  If the element already exists in the data then it is replaced
 *          with the new value.  If the element does not exist in the data it is appended
 *          to it, space permitting.
 *
 *          There is special handling for the device name (AD type DM_ADV_TYPE_LOCAL_NAME).
 *          If the name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  advHandle Advertising handle.
 *  \param  location  Data location.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.  Maximum length is 31 bytes if advertising set uses
 *                    legacy advertising PDUs with extended advertising.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added to the data, false otherwise.
 */
/*************************************************************************************************/
bool AppExtAdvSetAdValue(uint8_t advHandle, uint8_t location, uint8_t adType, uint8_t len,
                         uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Append the value of an advertising data element to the extended advertising or scan
 *          response data.  Regardless if the element already exists or doesn't in the data, it
 *          is appended with the element, space permitting.
 *
 *          There is special handling for the device name (AD type DM_ADV_TYPE_LOCAL_NAME).
 *          If the name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  advHandle Advertising handle.
 *  \param  location  Data location.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.  Maximum length is 31 bytes if advertising set uses
 *                    legacy advertising PDUs with extended advertising.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added to the data, false otherwise.
 */
 /*************************************************************************************************/
bool AppExtAdvAppendAdValue(uint8_t advHandle, uint8_t location, uint8_t adType, uint8_t len,
                            uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Set extended advertising type.
 *
 *  \param  advHandle Advertising handle.
 *  \param  advType   Advertising type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtSetAdvType(uint8_t advHandle, uint8_t advType);

/*************************************************************************************************/
/*!
 *  \brief  Accept a connection to a peer device with the given address using a given advertising
 *          set.
 *
 *  \param  advHandle Advertising handle.
 *  \param  advType   Advertising type.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *  \param  dbHdl     Device database handle.
 *
 *  \return Connection identifier.
 */
 /*************************************************************************************************/
dmConnId_t AppExtConnAccept(uint8_t advHandle, uint8_t advType, uint8_t addrType, uint8_t *pAddr,
                            appDbHdl_t dbHdl);

/*************************************************************************************************/
/*!
 *  \brief  Set periodic advertising data.
 *
 *  \param  advHandle Advertising handle.
 *  \param  len       Length of the data.
 *  \param  pData     Pointer to the data.
 *  \param  bufLen    Length of the data buffer maintained by Application.  Minimum length is
 *                    31 bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppPerAdvSetData(uint8_t advHandle, uint16_t len, uint8_t *pData, uint16_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Start periodic advertising for the given advertising handle.
 *
 *  \param  advHandle Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppPerAdvStart(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Stop periodic advertising for the given advertising handle.
 *
 *  \param  advHandle Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppPerAdvStop(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of an advertising data element in the periodic advertising data.
 *          If the element already exists in the data then it is replaced with the new value.
 *          If the element does not exist in the data it is appended to it, space permitting.
 *
 *          There is special handling for the device name (AD type DM_ADV_TYPE_LOCAL_NAME).
 *          If the name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  advHandle Advertising handle.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.  Maximum length is 31 bytes if advertising set uses
 *                    legacy advertising PDUs with extended advertising.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added to the data, false otherwise.
 */
/*************************************************************************************************/
bool AppPerAdvSetAdValue(uint8_t advHandle, uint8_t adType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Update the Advertising DID of the periodic advertising (the behavior is the same as if
 *          the current periodic advertising data had been sent again).
 *
 *  \param  advHandle Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppPerAdvUpdateAdvDid(uint8_t advHandle);

/**@}*/

/** \name App Scan Functions
 * configure and enable legacy and extended scanning and periodic synch.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Start scanning.   A scan is performed using the given discoverability mode,
 *          scan type, and duration.
 *
 *  \param  mode      Discoverability mode.
 *  \param  scanType  Scan type.
 *  \param  duration  The scan duration, in milliseconds.  If set to zero, scanning will
 *                    continue until AppScanStop() is called.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppScanStart(uint8_t mode, uint8_t scanType, uint16_t duration);

/*************************************************************************************************/
/*!
 *  \brief  Stop scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppScanStop(void);

/*************************************************************************************************/
/*!
*  \brief  Resolve the advertiser's RPA (AdvA) or the initiator's RPA (InitA) of a directed
*          advertisement report. If the addresses resolved then a connection will be initiated.
*
*  \param  pMsg         Pointer to DM callback event message.
*  \param  dbHdl        Database record handle.
*  \param  resolveType  Which address in advertising report to be resolved.
*
*  \return None.
*/
/*************************************************************************************************/
void AppMasterResolveAddr(dmEvt_t *pMsg, appDbHdl_t dbHdl, uint8_t resolveType);

/*************************************************************************************************/
/*!
 *  \brief  Start scanning.   A scan is performed using the given scanner PHYs, discoverability
 *          mode, scan type, duration, and period.
 *
 *  \param  scanPhys  Scanner PHYs.
 *  \param  mode      Discoverability mode.
 *  \param  pScanType Scan type array.
 *  \param  duration  The scan duration, in milliseconds.  If set to zero or both duration and
 *                    period set to non-zero, scanning will continue until DmExtScanStop() is
 *                    called.
 *  \param  period    The scan period, in 1.28 sec units.  Set to zero to disable periodic scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtScanStart(uint8_t scanPhys, uint8_t mode, const uint8_t *pScanType, uint16_t duration,
                     uint16_t period);

/*************************************************************************************************/
/*!
 *  \brief  Stop scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppExtScanStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Get a stored scan result from the scan result list.  The first result is at
 *          index zero.
 *
 *  \param  idx     Index of result in scan result list.
 *
 *  \return Pointer to scan result device info or NULL if index contains no result.
 */
/*************************************************************************************************/
appDevInfo_t *AppScanGetResult(uint8_t idx);

/*************************************************************************************************/
/*!
 *  \brief  Get the number of stored scan results.
 *
 *  \return Number of stored scan results.
 */
/*************************************************************************************************/
uint8_t AppScanGetNumResults(void);

/*************************************************************************************************/
/*!
 *  \brief  Synchronize with periodic advertising from the given advertiser, and start receiving
 *          periodic advertising packets.
 *
 *  \param  advSid        Advertising SID.
 *  \param  advAddrType   Advertiser address type.
 *  \param  pAdvAddr      Advertiser address.
 *  \param  skip          Number of periodic advertising packets that can be skipped after
 *                        successful receive.
 *  \param  syncTimeout   Synchronization timeout, in the units of 10ms.
 *  \param  syncCteType   Whether to only synchronize to periodic advertising with certain types
 *                        of Constant Tone Extension (0 indicates that the presence or absence of
 *                        a Constant Tone Extension is irrelevant).
 *
 *  \return Sync identifier.
 */
/*************************************************************************************************/
dmSyncId_t AppSyncStart(uint8_t advSid, uint8_t advAddrType, const uint8_t *pAdvAddr, uint16_t skip,
                        uint16_t syncTimeout, uint8_t syncCteType);

/*************************************************************************************************/
/*!
 *  \brief  Stop reception of the periodic advertising identified by the given sync identifier.
 *
 *  \param  syncId        Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSyncStop(dmSyncId_t syncId);

/**@}*/

/** \name App Connection Functions
 * Initiate and update connections
 */
/**@{*/

/*************************************************************************************************/
/*!
*  \brief  Open a connection to a peer device with the given address.
*
*  \param  addrType  Address type.
*  \param  pAddr     Peer device address.
*  \param  dbHdl     Device database handle.
*
*  \return Connection identifier.
*/
/*************************************************************************************************/
dmConnId_t AppConnOpen(uint8_t addrType, uint8_t *pAddr, appDbHdl_t dbHdl);

/*************************************************************************************************/
/*!
 *  \brief  Close a connection with the given connection identifier.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppConnClose(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Open a connection to a peer device with the given address.
 *
 *  \param  initPhys  Initiator PHYs.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *  \param  dbHdl     Device database handle.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t AppExtConnOpen(uint8_t initPhys, uint8_t addrType, uint8_t *pAddr, appDbHdl_t dbHdl);

/*************************************************************************************************/
/*!
 *  \brief  Open a connection to a peer device with the given address from a periodic advertisement.
 *
 *  \param  advHandle Advertising handle.
 *  \param  subevent  Subevent identifier.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *  \param  dbHdl     Device database handle.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t AppPawrConnOpen(uint8_t advHandle, uint8_t subevent, uint8_t addrType, uint8_t *pAddr,
                           appDbHdl_t dbHdl);

/*************************************************************************************************/
/*!
 *  \brief  Check if a connection is open.
 *
 *  \return Connection ID of open connection or DM_CONN_ID_NONE if no open connections.
 */
/*************************************************************************************************/
dmConnId_t AppConnIsOpen(void);

/*************************************************************************************************/
/*!
 *  \brief  Gets a list of connection identifiers of open connections.
 *
 *  \param  pConnIdList    Buffer to hold connection IDs (must be DM_CONN_MAX bytes).
 *
 *  \return Number of open connections.
 *
 */
/*************************************************************************************************/
uint8_t AppConnOpenList(dmConnId_t *pConnIdList);

/*************************************************************************************************/
/*!
 *  \brief  Find the connection ID with matching handle.
 *
 *  \param  handle  Handle to find.
 *
 *  \return Connection ID or DM_CONN_ID_NONE if error.
 */
/*************************************************************************************************/
dmConnId_t AppConnIdByHandle(uint16_t handle);

/**@}*/

/** \name App Security and Bonding Functions
 * Security and Bonding functions for configuration and interaction with \ref STACK_SMP Pairing
 * procedures.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Set the bondable mode of the device.
 *
 *  \param  bondable  true to set device to bondable, false to set to non-bondable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSetBondable(bool bondable);

/*************************************************************************************************/
/*!
 *  \brief  Initiate security as a master device.  If there is a stored encryption key
 *          for the peer device this function will initiate encryption, otherwise it will
 *          initiate pairing.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppMasterSecurityReq(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a request for security as a slave device.  This function will send a
 *          message to the master peer device requesting security.  The master device should
 *          either initiate encryption or pairing.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSlaveSecurityReq(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Handle a passkey request during pairing.  If the passkey is to be displayed, a
 *          random passkey is generated and displayed.  If the passkey is to be entered
 *          the user is prompted to enter the passkey.
 *
 *  \param  pAuthReq  DM authentication requested event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHandlePasskey(dmSecAuthReqIndEvt_t *pAuthReq);

/*************************************************************************************************/
/*!
 *  \brief  Handle a numeric comparison indication during pairing.  The confirmation value is
 *          displayed and the user is prompted to verify that the local and peer confirmation
 *          values match.
 *
 *  \param  pCnfInd  DM confirmation indication event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHandleNumericComparison(dmSecCnfIndEvt_t *pCnfInd);

/**@}*/

/** \name App Event Processing
 * Process received messages for which there is a general process.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Process connection-related DM messages for a slave.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSlaveProcDmMsg(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process security-related DM messages for a slave.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppSlaveSecProcDmMsg(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process connection-related DM messages for a master.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppMasterProcDmMsg(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process security-related DM messages for a master.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppMasterSecProcDmMsg(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Application Server initialization routine.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppServerInit(void);

/*************************************************************************************************/
/*!
 *  \brief  ATT connection callback for app framework.  This function is used when the application
 *          is operating as an ATT server and it uses notifications or indications.  This
 *          function can be called by the application's ATT connection callback or it can be
 *          installed as the ATT connection callback.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppServerConnCback(dmEvt_t *pDmEvt);

/*************************************************************************************************/
/*!
 *  \brief  Process ATT messages.
 *
 *  \param  pMsg    message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppServerProcAttMsg(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process DM messages.
 *
 *  \param  pMsg    message containing event.
 *  \param  role    connection role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppServerProcDmMsg(wsfMsgHdr_t *pMsg, uint8_t role);

/**@}*/

/** \name APP Service Discovery
 * GATT Service Discovery initialization, configuration and execution.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize app framework discovery.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to service discovery status.
 *
 *  \param  cback   Application service discovery callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscRegister(appDiscCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Set the discovery cached handle list for a given connection.
 *
 *  \param  connId       Connection identifier.
 *  \param  hdlListLen   Length of characteristic and handle lists.
 *  \param  pHdlList     Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscSetHdlList(dmConnId_t connId, uint8_t hdlListLen, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Service discovery or configuration procedure complete.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Service or configuration status.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscComplete(dmConnId_t connId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a given service.
 *
 *  \param  connId    Connection identifier.
 *  \param  uuidLen   Length of service UUID (2 or 16).
 *  \param  pUuid     Pointer to service UUID.
 *  \param  listLen   Length of characteristic and handle lists.
 *  \param  pCharList Characterisic list for discovery.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscFindService(dmConnId_t connId, uint8_t uuidLen, uint8_t *pUuid, uint8_t listLen,
                        attcDiscChar_t **pCharList, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a given service beginning with the
 *          last service discovered.
 *
 *  \param  connId    Connection identifier.
 *  \param  uuidLen   Length of UUID (2 or 16).
 *  \param  pUuid     Pointer to UUID data.
 *  \param  listLen   Length of characteristic and handle lists.
 *  \param  pCharList Characterisic list for discovery.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscFindNextService(dmConnId_t connId, uint8_t uuidLen, uint8_t *pUuid, uint8_t listLen,
                            attcDiscChar_t **pCharList, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a given service.
 *
 *  \param  connId    Connection identifier.
 *  \param  uuidLen   Length of UUID (2 or 16).
 *  \param  pUuid     Pointer to UUID data.
 *  \param  listLen   Length of characteristic and handle lists.
 *  \param  pCharList Characterisic list for discovery.
 *  \param  pHdlList  Characteristic handle list.
 *  \param  offsetHdl The starting handle to search from.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscFindServiceFromOffset(dmConnId_t connId, uint8_t uuidLen, uint8_t *pUuid, uint8_t listLen,
                                  attcDiscChar_t **pCharList, uint16_t *pHdlList, uint16_t offsetHdl);

/*************************************************************************************************/
/*!
 *  \brief  Perform characteristic discovery for a given service.
 *
 *  \param  connId    Connection identifier.
 *  \param  uuidLen   Length of UUID (2 or 16).
 *  \param  pUuid     Pointer to UUID data.
 *  \param  startHdl  Start handle.
 *  \param  endHdl    End handle.
 *  \param  listLen   Length of characteristic and handle lists.
 *  \param  pCharList Characterisic list for discovery.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscCharacteristics(dmConnId_t connId, uint8_t uuidLen, uint8_t *pUuid, uint16_t startHdl,
                            uint16_t endHdl, uint8_t listLen, attcDiscChar_t **pCharList,
                            uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Configure characteristics for discovered services.
 *
 *  \param  connId      Connection identifier.
 *  \param  status      APP_DISC_CFG_START or APP_DISC_CFG_CONN_START.
 *  \param  cfgListLen  Length of characteristic configuration list.
 *  \param  pCfgList    Characteristic configuration list.
 *  \param  hdlListLen  Length of characteristic handle list.
 *  \param  pHdlList    Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscConfigure(dmConnId_t connId, uint8_t status, uint8_t cfgListLen,
                      attcDiscCfg_t *pCfgList, uint8_t hdlListLen, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Perform the GATT service changed procedure.  This function is called when an
 *          indication is received containing the GATT service changed characteristic.  This
 *          function may initialize the discovery state and initiate service discovery
 *          and configuration.
 *
 *  \param  pMsg    Pointer to ATT callback event message containing received indication.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscServiceChanged(attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Start GATT discovery or configuration.
 *
 *  \param  dmConnId_t  Connection ID.
 *
 *  \return None.
 *
 *  \note   For automatic GATT discovery, \ref AppDiscProcDmMsg should be used. This function may
 *          be called to perform additional discovery.
 */
/*************************************************************************************************/
void AppDiscStart(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Process discovery-related DM messages.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscProcDmMsg(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process discovery-related ATT messages.  This function should be called
 *          from the application's event handler.
 *
 *  \param  pMsg    Pointer to ATT callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscProcAttMsg(attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Get the handle range of the latest service discovery operation.
 *
 *  May be called after receiving a \ref APP_DISC_CMPL event, but before calling AppDiscComplete().
 *
 *  \param  connId       connection identifier.
 *  \param  pStartHdl    output parameter for start handle.
 *  \param  pEndHdl      output parameter for end handle.
 *
 *  \return \ref true if handles were set, \ref false otherwise.
 */
/*************************************************************************************************/
bool AppDiscGetHandleRange(dmConnId_t connId, uint16_t *pStartHdl, uint16_t *pEndHdl);

/*************************************************************************************************/
/*!
 *  \brief  Read peer's database hash
 *
 *  \param  dmConnId_t  Connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDiscReadDatabaseHash(dmConnId_t connId);
/**@}*/

/** \name App Privacy Functions
 * Maintain privacy information about known peer devices.
 */
/**@{*/
/*************************************************************************************************/
/*!
*  \brief  Add device to resolving list.
*
*  \param  pMsg    Pointer to DM callback event message.
*  \param  connId  Connection identifier.
*
*  \return None.
*/
/*************************************************************************************************/
void AppAddDevToResList(dmEvt_t *pMsg, dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Add next device to resolving list. For the first device, the function should be
 *          called with 'hdl' set to 'APP_DB_HDL_NONE'.
 *
 *  \param  hdl     The last handle returned by AppAddNextDevToResList or APP_DB_HDL_NONE to begin.
 *
 *  \return The handle being restored or APP_DB_HDL_NONE when the operation is complete.
 *
 *  \note   Applications supporting address resolution should call this functions after DmDevReset.
 *          This function will restore the resolving list in the Controller using information in the
 *          app device database.
 *
 *  \note   After each device is added to resolving list, the DM will send
 *          DM_PRIV_ADD_DEV_TO_RES_LIST_IND to the application.  The application must call
 *          AppAddNextDevToResList again to continue the restore process until
 *          AppAddNextDevToResList returns APP_DB_HDL_NONE.
 */
/*************************************************************************************************/
appDbHdl_t AppAddNextDevToResList(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Clear all bonding information.
 *
 *  \return None.
 *
 *  \Note   This API should not be used when:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 *
 *          Otherwise, clearing the resolving list in the Controller may fail.
 */
/*************************************************************************************************/
void AppClearAllBondingInfo(void);

/*************************************************************************************************/
/*!
 *  \brief  Update privacy mode for a given peer device.
 *
 *  \param  hdl     Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUpdatePrivacyMode(appDbHdl_t hdl);

/**!@}*/

/*! \} */    /* APP_FRAMEWORK_API */


/*! \addtogroup STACK_EVENT
 *  \{ */

/** \name App Event Handling
 * Message passing interface to App Framework from other tasks through WSF.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  App framework handler init function called during system initialization.
 *
 *  \param  handlerId  WSF handler ID for App.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHandlerInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \brief  Application calls the EAD DM encrypted API.
 *
 *  \param  pData           Pointer to text to be encrypted.
 *  \param  pKey            Pointer to encryption key (16 bytes).
 *  \param  pIv             Pointer to encryption iv (8 bytes).
 *  \param  len             Length of pIn in bytes.
 *  \param  cbFunc          Parameter passed in complete event.
 *
 *  \return true if successful, else false.
 */
 /*************************************************************************************************/
bool AppEadEncryptData(uint8_t* pData, uint8_t* pKey, uint8_t* pIv, uint16_t len, void* cbFunc);

/*************************************************************************************************/
/*!
 *  \brief  Application calls the EAD DM decrypted API.
 *
 *  \param  pData           Pointer to text to be Decrypted.
 *  \param  pKey            Pointer to encryption key (16 bytes).
 *  \param  pIv             Pointer to encryption iv (8 bytes).
 *  \param  len             Length of pIn in bytes.
 *  \param  cbFunc          Parameter passed in complete event.
 *
 *  \return true if successful, else false.
 */
 /*************************************************************************************************/
bool AppEadDecyptData(uint8_t* pData, uint8_t* pKey, uint8_t* pIv, uint16_t len, void* cbFunc);

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for app framework.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for app framework. Can be used instead of AppHandler() if the
 *          UI should not be compiled into the project.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHandlerWithoutUi(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**@}*/

/*! \} */    /* STACK_EVENT */


#ifdef __cplusplus
};
#endif

#endif /* APP_API_H */
