/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller isochronous master interface file.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef LCTR_INT_BIS_MASTER_H
#define LCTR_INT_BIS_MASTER_H

#include "lctr_api_bis_master.h"
#include "lctr_int_bis.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave BIG broadcasting states. */
enum
{
  LCTR_MST_BIG_STATE_DISABLED,          /*!< BIG master disabled state. */
  LCTR_MST_BIG_STATE_SYNCING,           /*!< BIG master synchronizing state. */
  LCTR_MST_BIG_STATE_SYNCED,            /*!< BIG master synchronized state. */
  LCTR_MST_BIG_STATE_SHUTDOWN,          /*!< BIG master shutdown in progress. */
  LCTR_MST_BIG_STATE_RESET,             /*!< BIG master reset in progress. */
  LCTR_MST_BIG_STATE_TOTAL              /*!< Total number of BIG master states. */
};
typedef uint8_t lctrMstBigState_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrMstBigMsg_t *pLctrMstBigMsg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Host Events */
void lctrNotifyHostBigTerminateComplete(LlStatus_t status, uint8_t bigHandle);
void lctrNotifyHostSyncLost(uint8_t bigHandle, LlStatus_t reason);

/* State machine for BIG master */
void lctrMstBigSendMsg(lctrBigCtx_t *pBigCtx, LctrMstBigMsg_t event);
void lctrMstBigExecuteSm(lctrBigCtx_t *pBigCtx, LctrMstBigMsg_t event);

/* Action routines */
void lctrMstBigActStart(lctrBigCtx_t *pBigCtx);
void lctrMstBigActBigSync(lctrBigCtx_t *pBigCtx);
void lctrMstBigActTerm(lctrBigCtx_t *pBigCtx);
void lctrMstBigActCancelSync(lctrBigCtx_t *pBigCtx);
void lctrMstBigActSyncLost(lctrBigCtx_t *pBigCtx);
void lctrMstBigActMicFailed(lctrBigCtx_t *pBigCtx);
void lctrMstBigActCleanup(lctrBigCtx_t *pBigCtx);

/* Builder */
void lctrMstBigBuildOp(lctrBigCtx_t *pBigCtx, LctrAcadBigInfo_t *pBigInfo);
void lctrMstSetupBigContext(lctrBigCtx_t *pBigCtx, LctrAcadBigInfo_t *pBigInfo);
void lctrMstSetupBigChannel(lctrBigCtx_t *pBigCtx, LctrAcadBigInfo_t *pBigInfo);

/* ISR: BOD handlers */
void lctrMstBisRxCompletion(BbOpDesc_t *pBod, uint8_t *pBuf, uint8_t status);
void lctrMstBigBeginOp(BbOpDesc_t *pOp);
void lctrMstBigAbortOp(BbOpDesc_t *pOp);
void lctrMstBigEndOp(BbOpDesc_t *pOp);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_BIS_MASTER_H */
