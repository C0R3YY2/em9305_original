/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Application framework device database.
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
#ifndef APP_DB_H
#define APP_DB_H

#include "wsf_os.h"
#include "dm_api.h"
#include "att_api.h"
#include "app_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup APP_FRAMEWORK_DB_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief No device database record handle  */
#define APP_DB_HDL_NONE                   NULL

#ifndef APP_DB_MAX_PROFILES
#define APP_DB_MAX_PROFILES               1
#endif

#ifndef APP_DB_PROFILE_TBL_SIZE
#define APP_DB_PROFILE_TBL_SIZE           3
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Simplified app database device information */
typedef struct
{
  bdAddr_t    addr;                            /*!< Peer device address */
  dmSecLtk_t  ltk;                             /*!< LTK */
  uint8_t     ltkSecLevel;                     /*!< LTK security level */
  uint8_t     keyMask;
  union
  {
    uint16_t  cccTbl[APP_DB_NUM_CCCD];         /*! Client characteristic configuration descriptors */
    uint16_t  hdlList[APP_DB_HDL_LIST_LEN];    /*! Cached handle list */
  } att;
} appDbSimpleDevInfo_t;

/*! \brief Device database record handle type */
typedef void *appDbHdl_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name App Database
 * Store known device and security information.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the device database.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Create a new device database record.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Database record handle.
 */
/*************************************************************************************************/
appDbHdl_t AppDbNewRecord(uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
*  \brief  Get next device database record for a given database record. For the first database
*          record, the function should be called with 'hdl' set to 'APP_DB_HDL_NONE'.
*
*  \param  hdl  Database record handle.
*
*  \return Next database record handle found. APP_DB_HDL_NONE, otherwise.
*/
/*************************************************************************************************/
appDbHdl_t AppDbGetNextRecord(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Delete a new device database record.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbDeleteRecord(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Validate a new device database record.  This function is called when pairing is
 *          successful and the devices are bonded.
 *
 *  \param  hdl       Database record handle.
 *  \param  keyMask   Bitmask of keys to validate.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbValidateRecord(appDbHdl_t hdl, uint8_t keyMask);

/*************************************************************************************************/
/*!
 *  \brief  Check if a record has been validated.  If it has not, delete it.  This function
 *          is typically called when the connection is closed.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbCheckValidRecord(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
*  \brief  Check if a database record is in use.

*  \param  hdl       Database record handle.
*
*  \return TURE if record in use. false, otherwise.
*/
/*************************************************************************************************/
bool AppDbRecordInUse(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Check if there is a stored bond with any device.
 *
 *  \return true if a bonded device is found, false otherwise.
 */
/*************************************************************************************************/
bool AppDbCheckBonded(void);

/*************************************************************************************************/
/*!
 *  \brief  Delete all database records.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbDeleteAllRecords(void);

/*************************************************************************************************/
/*!
 *  \brief  Find a device database record by peer address.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Database record handle or APP_DB_HDL_NONE if not found.
 */
/*************************************************************************************************/
appDbHdl_t AppDbFindByAddr(uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Get the peer BD address from a record.
 *
 *  \param  hdl       Database record handle.
 *  \param  pAddr     Peer device address.
 *
 *  \return The address type.
 */
/*************************************************************************************************/
uint8_t AppDbGetBdAddr(appDbHdl_t hdl, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Find a device database record from data in an LTK request.
 *
 *  \param  encDiversifier  Encryption diversifier associated with key.
 *  \param  pRandNum        Pointer to random number associated with key.
 *
 *  \return Database record handle or APP_DB_HDL_NONE if not found.
 */
/*************************************************************************************************/
appDbHdl_t AppDbFindByLtkReq(uint16_t encDiversifier, uint8_t *pRandNum);

/*************************************************************************************************/
/*!
 *  \brief  Get the device database record handle associated with an open connection.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return Database record handle or APP_DB_HDL_NONE.
 */
/*************************************************************************************************/
appDbHdl_t AppDbGetHdl(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Get a key from a device database record.
 *
 *  \param  hdl       Database record handle.
 *  \param  type      Type of key to get.
 *  \param  pSecLevel If the key is valid, returns the security level of the key.
 *
 *  \return Pointer to the key if the key is valid or NULL if not valid.
 */
/*************************************************************************************************/
dmSecKey_t *AppDbGetKey(appDbHdl_t hdl, uint8_t type, uint8_t *pSecLevel);

/*************************************************************************************************/
/*!
 *  \brief  Set a key in a device database record.
 *
 *  \param  hdl       Database record handle.
 *  \param  pKey      Key data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetKey(appDbHdl_t hdl, dmSecKeyIndEvt_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  Get the client characteristic configuration descriptor table.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return Pointer to client characteristic configuration descriptor table.
 */
/*************************************************************************************************/
uint16_t *AppDbGetCccTbl(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set a value in the client characteristic configuration table.
 *
 *  \param  hdl       Database record handle.
 *  \param  idx       Table index.
 *  \param  value     Client characteristic configuration value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetCccTblValue(appDbHdl_t hdl, uint16_t idx, uint16_t value);

/*************************************************************************************************/
/*!
 *  \brief  Get the change aware state and client supported features record.
 *
 *  \param  hdl             Database record handle.
 *  \param  pIsChangeAware  Pointer to peer client's change aware status to a change in the database.
 *  \param  pCsf            Pointer to csf value pointer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbGetCsfRecord(appDbHdl_t hdl, uint8_t *pIsChangeAware, uint8_t **pCsf);

/*************************************************************************************************/
/*!
 *  \brief  Set a client supported features record.
 *
 *  \param  hdl              Database record handle.
 *  \param  changeAwareState The state of awareness to a change.
 *  \param  pCsf             pointed client supported features.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetCsfRecord(appDbHdl_t hdl, uint8_t changeAwareState, uint8_t *pCsf);

/*************************************************************************************************/
/*!
 *  \brief  Set client's state of awareness to a change in the database.
 *
 *  \param  hdl        Database record handle. If \ref hdl == \ref NULL, state is set for all
 *                     clients.
 *  \param  state      The state of awareness to a change, see ::attClientAwareStates.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetClientChangeAwareState(appDbHdl_t hdl, uint8_t state);

/*************************************************************************************************/
/*!
 *  \brief  Get device's GATT database hash.
 *
 *  \return Pointer to database hash.
 */
/*************************************************************************************************/
uint8_t *AppDbGetDbHash(void);

/*************************************************************************************************/
/*!
 *  \brief  Set device's GATT database hash.
 *
 *  \param  pHash    GATT database hash to store.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetDbHash(uint8_t *pHash);

/*************************************************************************************************/
/*!
 *  \brief  Get the peer's database hash.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return Pointer to database hash.
 */
/*************************************************************************************************/
uint8_t *AppDbGetPeerDbHash(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set a new peer database hash.
 *
 *  \param  hdl       Database record handle.
 *  \param  dbHash    Pointer to new hash.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetPeerDbHash(appDbHdl_t hdl, uint8_t *dbHash);

/*************************************************************************************************/
/*!
 *  \brief  Check if cached handles' validity are ascertained by reading the peer's database hash
 *
 *  \param  hdl       Database record handle.
 *
 *  \return \ref true if peer's database hash must be read to verify handles have not changed.
 */
/*************************************************************************************************/
bool AppDbIsCacheCheckedByHash(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set if cached handles' validity are determined by reading the peer's database hash.
 *
 *  \param  hdl            Database record handle.
 *  \param  cacheByHash   \ref true if peer's database must be read to verify cached handles.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetCacheByHash(appDbHdl_t hdl, bool cacheByHash);

/*************************************************************************************************/
/*!
*  \brief  Get the discovery status.
*
*  \param  hdl       Database record handle.
*
*  \return Discovery status.
*/
/*************************************************************************************************/
uint8_t AppDbGetDiscStatus(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Get the discovery status.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return Discovery status.
 */
/*************************************************************************************************/
uint8_t AppDbGetDiscStatus(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set the discovery status.
 *
 *  \param  hdl       Database record handle.
 *  \param  status    Discovery status.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetDiscStatus(appDbHdl_t hdl, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Get the cached handle list.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return Pointer to handle list.
 */
/*************************************************************************************************/
uint16_t *AppDbGetHdlList(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set the cached handle list.
 *
 *  \param  hdl       Database record handle.
 *  \param  pHdlList  Pointer to handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetHdlList(appDbHdl_t hdl, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Get the device name.
 *
 *  \param  pLen      Returned device name length.
 *
 *  \return Pointer to UTF-8 string containing the device name or NULL if not set.
 */
/*************************************************************************************************/
char *AppDbGetDevName(uint8_t *pLen);

/*************************************************************************************************/
/*!
 *  \brief  Set the device name.
 *
 *  \param  len       Device name length.
 *  \param  pStr      UTF-8 string containing the device name.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetDevName(uint8_t len, char *pStr);

/*************************************************************************************************/
/*!
*  \brief  Get address resolution attribute value read from a peer device.
*
*  \param  hdl        Database record handle.
*
*  \return true if address resolution is supported in peer device. false, otherwise.
*/
/*************************************************************************************************/
bool AppDbGetPeerAddrRes(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
*  \brief  Set address resolution attribute value for a peer device.
*
*  \param  hdl        Database record handle.
*  \param  addrRes    Peer address resolution attribue value.
*
*  \return None.
*/
/*************************************************************************************************/
void AppDbSetPeerAddrRes(appDbHdl_t hdl, uint8_t addrRes);

/*************************************************************************************************/
/*!
*  \brief  Get sign counter for a peer device.
*
*  \param  hdl        Database record handle.
*
*  \return Sign counter for peer device.
*/
/*************************************************************************************************/
uint32_t AppDbGetPeerSignCounter(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
*  \brief  Set sign counter for a peer device.
*
*  \param  hdl          Database record handle.
*  \param  signCounter  Sign counter for peer device.
*
*  \return None.
*/
/*************************************************************************************************/
void AppDbSetPeerSignCounter(appDbHdl_t hdl, uint32_t signCounter);

/*************************************************************************************************/
/*!
 *  \brief  Get the peer device added to resolving list flag value.
 *
 *  \param  hdl        Database record handle.
 *
 *  \return true if peer device's been added to resolving list. false, otherwise.
 */
/*************************************************************************************************/
bool AppDbGetPeerAddedToRl(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set the peer device added to resolving list flag to a given value.
 *
 *  \param  hdl           Database record handle.
 *  \param  peerAddedToRl Peer device added to resolving list flag value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetPeerAddedToRl(appDbHdl_t hdl, bool peerAddedToRl);

/*************************************************************************************************/
/*!
 *  \brief  Get resolvable private address only attribute present flag for a peer device.
 *
 *  \param  hdl        Database record handle.
 *
 *  \return true if RPA Only attribute is present on peer device. false, otherwise.
 */
/*************************************************************************************************/
bool AppDbGetPeerRpao(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Set resolvable private address only attribute present flag for a peer device.
 *
 *  \param  hdl        Database record handle.
 *  \param  peerRpao   Resolvable private address only attribute present flag.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetPeerRpao(appDbHdl_t hdl, bool peerRpao);

/*************************************************************************************************/
/*!
 *  \brief  Get the profile specific table.
 *
 *  \param  hdl        Database record handle.
 *  \param  pId        Profile identifier.
 *
 *  \return Pointer to the profile specific table.
 */
/*************************************************************************************************/
uint8_t *AppDbGetProfileTbl(appDbHdl_t hdl, uint8_t pId);

/*************************************************************************************************/
/*!
 *  \brief  Set the profile specific table.
 *
 *  \param  hdl        Database record handle.
 *  \param  pId        Profile identifier.
 *  \param  pTbl       Pointer to data to add the table.
 *  \param  offset     Offset in bytes to the location in the table to write pTbl.
 *  \param  len        Length of pTbl in bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbSetProfileTbl(appDbHdl_t hdl, uint8_t pId, uint8_t *pTbl, uint8_t offset, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Store the resolvable private address only attribute flag for a device record in NVM.
 *
 *  \param  hdl        Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStorePeerRpao(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the client characteristic configuration table for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreCccTbl(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Restore the client characteristic configuration table for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void AppDbNvmRestoreCccTbl(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the cached attribute handle list for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreHdlList(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the peer sign counter for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStorePeerSignCounter(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the address resolution attribute value for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStorePeerAddrRes(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the client's state of awareness to a change for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreChangeAwareState(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the client supported features for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreCsfRecord(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the method of validating the cache handle for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreCacheByHash(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store the GATT database hash for a device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreDbHash(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Store bonding information for device record in NVM.
 *
 *  \param  hdl       Database record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmStoreBond(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Read all device database records from NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmReadAll(void);

/*************************************************************************************************/
/*!
 *  \brief  Delete the device database record with the given handle from NVM.
 *
 *  \param  hdl           Record handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmDeleteRec(appDbHdl_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Delete all device database records from NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppDbNvmDeleteAll(void);

/*************************************************************************************************/
/*!
 *  \brief  Restore the database using a list of simple device information.
 *
 *  \param  pList     Pointer to a list of simple device information.
 *  \param  count     The number of elements in pList.
 *
 *  \return None.
 *
 *  \note   This function restores the DB using data collected with AppDbGetSimpleDevInfoList.
 *          This function only restores the BD Address, LTK, att handles, and CCC values.  All
 *          other DB parameters and their related features are disabled.
 */
/*************************************************************************************************/
void AppDbRestoreFromSimpleDevInfoList(appDbSimpleDevInfo_t *pList, uint8_t count);

/*************************************************************************************************/
/*!
 *  \brief  Restore one record in the database using simple device information.
 *
 *  \param  pInfo       Pointer to simple device information.
 *
 *  \return true if restored, else false.
 *
 *  \note   This function restores one record in the DB using data collected with
 *          AppDbGetSimpleDevInfo. This function only restores the BD Address, LTK, att handles,
 *          and CCC values.  All other DB parameters and their related features are disabled.
 */
/*************************************************************************************************/
bool AppDbRestoreFromSimpleDevInfo(appDbSimpleDevInfo_t *pInfo);

/*************************************************************************************************/
/*!
 *  \brief  Get a list of simple device information from the database.
 *
 *  \param  pList     Pointer to a list of device information.
 *  \param  max       The max number of elements that can be stored in pList.
 *
 *  \return The number of elements stored in pList.
 *
 *  \note   This function collects data for use with AppDbRestoreFromSimpleDevInfoList.
 */
/*************************************************************************************************/
uint8_t AppDbGetSimpleDevInfoList(appDbSimpleDevInfo_t *pList, uint8_t max);

/*************************************************************************************************/
/*!
 *  \brief  Get simple device information from the database for the peer with the given bdaddr.
 *
 *  \param  pInfo       Pointer to simple device information.
 *  \param  bdaddr      BD Address of peer.
 *
 *  \return true if found, else false.
 *
 *  \note   This function collects data for use with AppDbRestoreFromSimpleDevInfo.
 */
/*************************************************************************************************/
bool AppDbGetSimpleDevInfodByBdAddr(appDbSimpleDevInfo_t *pInfo, bdAddr_t bdaddr);

/**@}*/

/*! \} */    /*! APP_FRAMEWORK_DB_API */

#ifdef __cplusplus
};
#endif

#endif /* APP_DB_H */
