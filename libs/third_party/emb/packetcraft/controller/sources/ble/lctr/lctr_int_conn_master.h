/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller master connection interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_CONN_MASTER_H
#define LCTR_INT_CONN_MASTER_H

#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_int_adv_slave_ae.h"
#include "lctr_api_conn.h"
#include "lctr_pdu_conn.h"
#include "ll_defs.h"
#include "bb_ble_api.h"
#include "wsf_msg.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Buffer offset of txWinOffset in a CONN_IND PDU. */
#define LCTR_CONN_IND_TX_WIN_OFFSET     (LL_DATA_HDR_LEN + 6 + 6 + 8)

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern LctrLlcpHdlr_t lctrMstLlcpSmTbl[LCTR_LLCP_SM_TOTAL];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrMstConnExecuteSm(lctrConnCtx_t *pConnCtx, uint8_t event);
void lctrMstLlcpExecuteSm(lctrConnCtx_t *pCtx, uint8_t event);
bool lctrMstLlcpExecuteConnUpdSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Builder */
void lctrMstConnBuildOp(lctrConnCtx_t *pCtx, lctrConnInd_t *pConnInd, lctrAdvSet_t *pAdvSet);

/* ISR */
void lctrMstConnBeginOp(BbOpDesc_t *pOp);
void lctrMstConnCleanupOp(BbOpDesc_t *pOp);
void lctrMstConnEndOp(BbOpDesc_t *pOp);
void lctrMstConnAbortOp(BbOpDesc_t *pOp);
void lctrMstConnTxCompletion(BbOpDesc_t *pOp, uint8_t status);
void lctrMstConnRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status);

/* Action routines. */
void lctrMstSetEstablishConn(lctrConnCtx_t *pCtx);
void lctrMstReloadDataPdu(lctrConnCtx_t *pCtx);

/* Helper */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CONN_MASTER_H */
