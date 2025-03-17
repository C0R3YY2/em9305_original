/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller master connected isochronous stream interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_CIS_MASTER_H
#define LCTR_INT_CIS_MASTER_H

#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_int_cis.h"
#include "lctr_api_cis_master.h"
#include "lmgr_api_cis_master.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      CIS master establishment states. */
enum
{
  LCTR_CIS_MST_EST_STATE_IDLE,                         /*!< Idle state. */
  LCTR_CIS_MST_EST_STATE_CIS_RSP,                      /*!< Wait for LL_CIS_RSP state. */
  LCTR_CIS_MST_EST_STATE_INSTANT,                      /*!< Wait for establishment instant. */
  LCTR_CIS_MST_EST_STATE_TOTAL                         /*!< Total CIS master establishment states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Create CIS pending. */
typedef struct
{
  uint8_t           numCis;                           /*!< Number of CISs to create. */
  uint16_t          cisHandle[LL_MAX_CIS];            /*!< CIS handle array. */
  uint16_t          aclHandle[LL_MAX_CIS];            /*!< ACL handle array. */
  uint8_t           isCreateCisDone[LL_MAX_CIS];      /*!< 1 if create CIS is done, either succeed or fail. */
} lctrCreateCisPend_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/* State machine for CIS LLCP */
bool lctrMstLlcpExecuteCisEstSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Builder */
void lctrMstCisBuildCigOp(lctrCigCtx_t *pCigCtx);
void lctrMstCisCigOpCommit(lctrCigCtx_t *pCigCtx, lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisBuildCisData(lctrCisCtx_t *pCisCtx);

/* ISR: Packet handlers */
void lctrMstCisCigTxCompletion(BbOpDesc_t *pOp, uint8_t status);
void lctrMstCisCigRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status);

/* ISR: BOD handlers */
uint32_t lctrMstCisCheckContOp(BbOpDesc_t *pOp, bool *pNewCisCtx);
uint32_t lctrMstCisCheckContOpPostCback(BbOpDesc_t *pOp, bool *pNewCisCtx);
void lctrMstCisCigBeginOp(BbOpDesc_t *pOp);
void lctrMstCisCigContOp(BbOpDesc_t *pOp);
void lctrMstCisCigPostSubEvt(BbOpDesc_t *pOp, uint8_t status);
void lctrMstCisCigEndOp(BbOpDesc_t *pOp);
void lctrMstCisCigCleanupOp(BbOpDesc_t *pOp);
void lctrMstCisCigAbortOp(BbOpDesc_t *pOp);

/* Action routines for CIS master LLCP state machine */
void lctrMstCisLlcpActHostCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisLlcpActPeerRej(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisLlcpActPeerCisRsp(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisLlcpActRspTimeout(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisLlcpActLocalReject(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrMstCisLlcpActLocalCancel(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/* Reservation */
uint32_t lctrGetCigRefTime(uint8_t connHandle, uint32_t *pDurUsec);

/* Utility function */
void lctrMstCreateCisDone(lctrCisCtx_t *pCisCtx);
void lctrCancelPendCreateCis(uint16_t aclHandle);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CIS_MASTER_H */
