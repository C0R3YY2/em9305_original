/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller scanning master interface file.
 *
 *  Copyright (c) 2013-2019 ARM Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_INIT_MASTER_AE_H
#define LCTR_INT_INIT_MASTER_AE_H

#include "lctr_api_init_master_ae.h"
#include "lctr_int.h"
#include "lctr_int_adv_master_ae.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Resolve the extended initiate handle from the context pointer. */
#define LCTR_GET_EXT_INIT_HANDLE(pCtx)  (pCtx->handle)

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master initiate states. */
enum
{
  LCTR_EXT_INIT_STATE_DISABLED,             /*!< Initiate disabled state. */
  LCTR_EXT_INIT_STATE_ENABLED,              /*!< Initiate enabled state. */
  LCTR_EXT_INIT_STATE_SHUTDOWN,             /*!< Initiate shutdown in progress. */
  LCTR_EXT_INIT_STATE_RESET,                /*!< Initiate reset in progress. */
  LCTR_EXT_INIT_STATE_TOTAL                 /*!< Total number of initiate states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief      Extended initiating control block. */
typedef struct
{
  /* Initiate */
  uint64_t          peerAddr;           /*!< Initiating peer address. */
  uint8_t           peerAddrType;       /*!< Initiating peer address type. */
  uint8_t           estConnPhys;        /*!< PHYs which established connections. */

  /* State. */
  uint8_t           enaPhys;            /*!< Enabled PHYs. */

} lctrExtInitCtrlBlk_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/
#if LL_SHARE_INIT_SCAN_MEM == 0
extern lctrExtScanCtx_t * const pLctrMstExtInitTbl[LL_SCAN_PHY_MAX];
#else
extern lctrExtScanCtx_t *pLctrMstExtInitTbl[LL_SCAN_PHY_MAX];
#endif
extern lctrExtInitCtrlBlk_t lctrMstExtInit;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrMstExtInitExecuteSm(lctrExtScanCtx_t *pExtInitCtx, uint8_t event);

/* Builder */
uint8_t lctrMstExtInitiateBuildOp(lctrExtScanCtx_t *pExtInitCtx, LlConnSpec_t *pConnSpec, uint64_t peerAddr, uint8_t peerAddrType);
uint8_t lctrMstAuxInitiateBuildOp(lctrExtScanCtx_t *pExtInitCtx, LlConnSpec_t *pConnSpec, uint64_t peerAddr, uint8_t peerAddrType);
void lctrMstExtInitiateOpCommit(lctrExtScanCtx_t *pExtInitCtx);

/* ISR: Initiate packet handlers */
bool lctrMstInitiateRxExtAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
void lctrMstInitiateRxExtAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool lctrMstInitiateRxAuxAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool lctrMstInitiateRxAuxConnRspHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf);
bool lctrMstExtConnIndTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pIndBuf);

/* ISR: Initiate BOD callback */
void lctrMstExtInitiateEndOp(BbOpDesc_t *pOp);
void lctrMstAuxInitiateEndOp(BbOpDesc_t *pOp);
void lctrMstExtInitiateScanEndOp(BbOpDesc_t *pOp);
void lctrMstAuxInitiateScanEndOp(BbOpDesc_t *pOp);

/* Action routines. */
void lctrExtInitActConnect(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActShutdown(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActScanTerm(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActDisallowInitiate(lctrExtScanCtx_t *pExtInitCtx);
void lctrExtInitActDisallowCancel(lctrExtScanCtx_t *pExtInitCtx);

/* Helper routines. */
void lctrMstExtInitCleanupOp(lctrExtScanCtx_t *pExtInitCtx);
uint8_t lctrExtInitSetupInitiate(lctrExtScanCtx_t *pExtInitCtx, uint8_t peerAddrType, uint64_t peerAddr, uint8_t filtPolicy, uint8_t ownAddrType);

/* Messaging */
void lctrSendExtInitMsg(lctrExtScanCtx_t *pExtScanCtx, uint8_t event);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_INIT_MASTER_AE_H */
