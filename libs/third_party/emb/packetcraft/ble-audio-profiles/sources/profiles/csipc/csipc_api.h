/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Coordinated Set Identification Profile client.
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

#ifndef CSIPC_API_H
#define CSIPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup COORDINATED_SET_IDENTIFICATION_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of CSIS coordinated sets in the CSIS Database */
#ifndef CSIPC_DB_MAX_SETS
#define CSIPC_DB_MAX_SETS                 2
#endif

/* Maximum number of CSIS the client can discover */
#ifndef CSIPC_CSIS_MAX
#define CSIPC_CSIS_MAX                    2
#endif

/* CSIPC value update notification callback types */
#define CSIPC_TYPE_LOCK_NTF               1
#define CSIPC_TYPE_RANK_NTF               2
#define CSIPC_TYPE_SIZE_NTF               3
#define CSIPC_TYPE_REQ_LOCK_SUCCESS_IND   4
#define CSIPC_TYPE_REQ_LOCK_FAIL_IND      5

/*! \brief Enumeration of handle indexes of characteristics to be discovered - CSIS */
#define CSIPC_CSIS_DISC_HANDLES(_a, _b)                                                           \
enum                                                                                              \
{                                                                                                 \
  CSIPC_CSIS_SIRK_HDL_INDX_##_a=_b,       /*!< \brief Coordinated Set Identity Resolving key. */  \
  CSIPC_CSIS_SIRK_CCC_INDX_##_a,          /*!< \brief Coordinated SIRK CCC descriptor. */         \
  CSIPC_CSIS_SIZE_HDL_INDX_##_a,          /*!< \brief Coordinated Set Size. */                    \
  CSIPC_CSIS_SIZE_CCC_INDX_##_a,          /*!< \brief Coordinated Set Size CCC descriptor. */     \
  CSIPC_CSIS_LOCK_HDL_INDX_##_a,          /*!< \brief Coordinated Set Lock. */                    \
  CSIPC_CSIS_LOCK_CCC_INDX_##_a,          /*!< \brief Coordinated Set Lock CCC descriptor. */     \
  CSIPC_CSIS_RANK_HDL_INDX_##_a,          /*!< \brief Coordinated Set Rank. */                    \
  CSIPC_CSIS_HDL_LIST_LEN_##_a,           /*!< \brief Handle list length. */                      \
};

/* CSIS service handle sub-index */
#define CSIPC_CSIS_SIRK_HDL_INDX          CSIPC_CSIS_SIRK_HDL_INDX_0
#define CSIPC_CSIS_SIRK_CCC_INDX          CSIPC_CSIS_SIRK_CCC_INDX_0
#define CSIPC_CSIS_SIZE_HDL_INDX          CSIPC_CSIS_SIZE_HDL_INDX_0
#define CSIPC_CSIS_SIZE_CCC_INDX          CSIPC_CSIS_SIZE_CCC_INDX_0
#define CSIPC_CSIS_LOCK_HDL_INDX          CSIPC_CSIS_LOCK_HDL_INDX_0
#define CSIPC_CSIS_LOCK_CCC_INDX          CSIPC_CSIS_LOCK_CCC_INDX_0
#define CSIPC_CSIS_RANK_HDL_INDX          CSIPC_CSIS_RANK_HDL_INDX_0

#define CSIPC_CSIS_HDL_LIST_LEN           CSIPC_CSIS_HDL_LIST_LEN_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - CSIS */
/* TODO: A device can have multiple CSIS services.
 *       Add service handle indicies as needed. */
CSIPC_CSIS_DISC_HANDLES(0, 0);
CSIPC_CSIS_DISC_HANDLES(1, CSIPC_CSIS_HDL_LIST_LEN_0);

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  bdAddr_t            addr;                     /*!< \brief Address. */
  uint8_t             addrType;                 /*!< \brief Address type. */
} csipcIdentity_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Callback to notify a higher layer that a value was updated.
 *
 *  \param  connId      DM Connection Identifier.
 *  \param  svcId       Service identifier
 *  \param  type        Notification type.
 *  \param  value       New value.
 *
 *  \return none.
 */
/*************************************************************************************************/
typedef void (*csipcCback_t)(dmConnId_t connId, uint8_t svcId, uint8_t type, uint8_t value);

/*************************************************************************************************/
/*!
 *  \brief  Callback to notify a higher layer that an identity was resolved successfully.
 *
 *  \param  pIdentity   Identity of the device.
 *
 *  \return none.
 */
/*************************************************************************************************/
typedef void (*csipcDbResCback_t)(csipcIdentity_t *pIdentity);

/*************************************************************************************************/
/*!
 *  \brief  Resolve a Private Set Random Identifier.
 *
 *  \param  handlerId   Handler Identifier
 *  \param  event       AES complete event
 *  \param  pSirk       The 16 byte Set Identify Resolving Key.
 *  \param  pPsri       The 3 byte prand of the set identifier to resolve.
 *
 *  \return none.
 *
 *  \note   After the application reveives the AES complete event, the application should call
 *          CsipsResolvePsriComplete to complete the calculation and return the local hash.
 */
/*************************************************************************************************/
void CsipcResolvePsri(wsfHandlerId_t handlerId, uint8_t event, uint8_t *pSirk, uint8_t *pPsri);

/*************************************************************************************************/
/*!
 *  \brief  Complete the process of resolving a Private Set Random Identifier.
 *
 *  \param  pAes    The result of the SecAes operation performed in CsipsResolvePsri.
 *  \param  pHash   The hash portion of the set identifier.
 *
 *  \return true if the set identifier resolves, else false.
 */
/*************************************************************************************************/
bool CsipcResolvePsriComplete(secAes_t *pAes, uint8_t *pHash);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Coordinated Set Identification service.
 *          Parameter pHdlList must point to an array of length \ref CSIPC_CSIS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     Identifier of service being discovered.
 *  \param  startHdl  Handle to begin searching from.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipcCsisDiscover(dmConnId_t connId, uint8_t svcId, uint16_t startHdl, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Called to request a coordinated set lock from the CSIS server.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     Service identifier.
 *  \param  lock      Lock value (CSIS_LOCK or CSIS_UNLOCK).
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcCsisWriteLock(dmConnId_t connId, uint8_t svcId, uint8_t lock);

/*************************************************************************************************/
/*!
 *  \brief  Called to request a read lock from the CSIS server.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     Service identifier.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcCsisReadLock(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the CSIPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function for notification of updated CSIS attributes.
 *
 *  \param  cbackId   Application specific identifier for the callback function.
 *  \param  cback     Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipcRegisterCback(uint8_t cbackId, csipcCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref CSIPC_CSIS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t CsipcCsisValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to initialize the CSIPC.
 *
 *  \param  handlerId      Application handler ID.
 *  \param  sirkDecodeEvt  SIRK decode CMAC indication event ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipcInit(wsfHandlerId_t handlerId, uint8_t sirkDecodeEvt);

/*************************************************************************************************/
/*!
 *  \brief  For test purposes only.  Verify an SIRK's value.
 *
 *  \param  connId      DM Connection Identifier.
 *  \param  svcId       Service identifier.
 *  \param  pKey        The key to compare.
 *
 *  \return true if the keys match, else false.
 */
/*************************************************************************************************/
bool CsipcTestCheckSirk(dmConnId_t connId, uint8_t svcId, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  Process an extended scan report and record Private Set Random Identifiers.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcDbProcExtScanReport(dmEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process an extended scan report and record Private Set Random Identifiers.
 *
 *  \param  pSirk    Pointer to DM callback event message.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcDbAddSet(uint8_t *pSirk);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Coordinated Set Identification service.
 *          Parameter pHdlList must point to an array of length \ref CSIPC_CSIS_HDL_LIST_LEN times
 *          the number of CSIS services to be discovered. If discovery is successful the handles
 *          of discovered characteristics and descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Pointer to applications array of CSIS attribute handles.
 *
 *  \return false to continue discovery, else true.
 */
/*************************************************************************************************/
bool CsipcDbCsisDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the CSICP Database of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipcDbProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Register the device resolved callback.
 *
 *  \param  cback       Callback function.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcDbSetResolveCback(csipcDbResCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the coordinated set database.
 *
 *  \param  handlerId  Application task handler ID.
 *  \param  hashEvent  Event identifier for hash operations.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipcDbInit(wsfHandlerId_t handlerId, uint8_t hashEvent);

/*! \} */    /* COORDINATED_SET_IDENTIFICATION_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* CSIPC_API_H */
