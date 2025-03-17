/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Call Control Profile server.
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
#ifndef CCPS_API_H
#define CCPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup CALL_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Call Control Profile Server service control block. */
typedef struct
{
  uint8_t             svcId;              /*! Call control service identifier */
  uint8_t             technology;         /*! Bearer Technology */
} ccpsCallInfo_t;

/* Call Control Profile Server action function */
typedef uint8_t (*ccpsAct_t)(ccpsCallInfo_t *pInfo, uint8_t opcode, uint8_t *pParam, uint8_t len);

/* Call Control Profile Server action function set */
typedef struct
{
  ccpsAct_t           originate;          /*! Call originate action function */
  ccpsAct_t           alert;              /*! Call alert action function */
  ccpsAct_t           answer;             /*! Call answer action function */
  ccpsAct_t           incoming;           /*! Call incoming action function */
  ccpsAct_t           accept;             /*! Call accept action function */
  ccpsAct_t           localhold;          /*! Call local hold action function */
  ccpsAct_t           localretrieve;      /*! Call local retrieve action function */
  ccpsAct_t           join;               /*! Call local join function */
  ccpsAct_t           remotehold;         /*! Call remote hold action function */
  ccpsAct_t           remoteretrieve;     /*! Call remote retrieve action function */
  ccpsAct_t           terminate;          /*! Call terminate action function */
} ccpsActionTbl_t;

/* Call Control Profile Server callback function set */
typedef struct
{
  int placehold;
} ccpsCbackTbl_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Called to set the provider name.
 *
 *  \param svcId     Call control service index.
 *  \param pName     Call provider name (UTF-8 string).
 *
 *  \return none.
 */
/*************************************************************************************************/
void CcpsSetCallProviderName(uint8_t svcId, const char *pName);

void CcpsSetBearerUci(uint8_t svcId, const char *pUci, uint16_t len);

void CcpsSetBearerTechnology(uint8_t svcId, uint8_t technology);

void CcpsSetBearerUriSchemes(uint8_t svcId, const char *pUri);

void CcpsSetCurrentCall(uint8_t svcId, uint8_t callIdx, uint8_t state, uint8_t flags, const char *pUri, uint8_t urilen);

void CcpsRemoveCurrentCall(uint8_t svcId, uint8_t callIndex);

void CcpsFormatBearerCurrCalls(uint8_t svcId, uint8_t callIndex, uint8_t *pBuf);

void CcpsSetStatusFlags(uint8_t svcId, uint8_t callIndex, uint16_t flags);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the call control state.
 *
 *  \param svcId     Call control service index.
 *  \param callIndex Call control index.
 *  \param state     Call control state.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CcpsSetState(uint8_t svcId, uint8_t callIndex, uint8_t state);

void CcpsSendTerminationReasonNtf(uint8_t svcId);

void CcpsSetIncomingCall(uint8_t svcId, uint8_t callIndex, const char *pUri);


/*************************************************************************************************/
/*!
 *  \brief  Get a pointer to the service info structure for the given service ID.
 *
 *  \param svcId     Call control service index.
 *
 *  \return Pointer to the service info structure.
 */
/*************************************************************************************************/
ccpsCallInfo_t *CcpsGetSvcInfo(uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Timeout function called by applications to simulate media player.
 *
 *  \param pInfo     Call control profile information.
 *  \param deltaMs   Change in milliseconds since last call.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpsSampleHandleTimeout(ccpsCallInfo_t *pInfo, uint32_t deltaMs);

/*************************************************************************************************/
/*!
 *  \brief  Call Control Profile server registration of callback and action functions.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpsRegister(uint8_t svcId, uint8_t cccStart, uint8_t callIdx, const ccpsActionTbl_t *pActions, const ccpsCbackTbl_t *pCbacks);

/*************************************************************************************************/
/*!
 *  \brief  Call Control Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Call Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the MCP server of ATT messages.
 *
 *  \param  pMsg    message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpsProcAttMsg(wsfMsgHdr_t *pMsg);

/*! \} */    /* CALL_CONTROL_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* CCPS_API_H */
