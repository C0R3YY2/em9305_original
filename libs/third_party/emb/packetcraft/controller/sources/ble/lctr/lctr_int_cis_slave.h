/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connected isochronous stream slave interface file.
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

#ifndef LCTR_INT_CIS_SLAVE_H
#define LCTR_INT_CIS_SLAVE_H

#include "lctr_int.h"
#include "lctr_api_cis.h"
#include "lctr_int_conn.h"
#include "lctr_int_cis.h"
#include "lctr_api_cis_slave.h"
#include "lmgr_api_cis_slave.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Constants
**************************************************************************************************/
/*! \brief      CIS slave establishment states. */
enum
{
  LCTR_CIS_SLV_EST_STATE_IDLE,                        /*!< Idle state. */
  LCTR_CIS_SLV_EST_STATE_HOST_REPLY,                  /*!< Wait for host reply state. */
  LCTR_CIS_SLV_EST_STATE_CIS_IND,                     /*!< Wait for LL_CIS_IND state. */
  LCTR_CIS_SLV_EST_STATE_INSTANT,                     /*!< Wait for establishment instant. */
  LCTR_CIS_SLV_EST_STATE_TOTAL                        /*!< Total CIS slave establishment states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Globals
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/* State machine for CIS LLCP */
bool lctrSlvLlcpExecuteCisEstSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Builder */
void lctrSlvCisBuildCigOp(lctrCigCtx_t *pCigCtx);
void lctrSlvCisCigOpCommit(lctrCigCtx_t *pCigCtx, lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisBuildCisData(lctrCisCtx_t *pCisCtx);

/* ISR: Packet handlers */
void lctrSlvCisCigTxCompletion(BbOpDesc_t *pOp, uint8_t status);
void lctrSlvCisCigRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status);

/* ISR: BOD handlers */
void lctrSlvCisCigBeginOp(BbOpDesc_t *pOp);
void lctrSlvCisCigContOp(BbOpDesc_t *pOp);
void lctrSlvCisCigPostSubEvt(BbOpDesc_t *pOp, uint8_t status);
uint32_t lctrSlvCisCheckContOp(BbOpDesc_t *pOp, bool *pNewCisCtx);
uint32_t lctrSlvCisCheckContOpPostCback(BbOpDesc_t *pOp, bool *pNewCisCtx);
void lctrSlvCisCigEndOp(BbOpDesc_t *pOp);
void lctrSlvCisCigCleanupOp(BbOpDesc_t *pOp);
void lctrSlvCisCigAbortOp(BbOpDesc_t *pOp);

/* Action routines for CIS slave LLCP state machine */
void lctrSlvCisLlcpActPeerCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisLlcpActRejCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisLlcpActAcpCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisLlcpActPeerCisInd(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisLlcpActPeerCisRej(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrSlvCisLlcpActIntPeerCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/* Reservation */

/* Helper */
void lctrCisStorePeerCisReq(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CIS_SLAVE_H */
