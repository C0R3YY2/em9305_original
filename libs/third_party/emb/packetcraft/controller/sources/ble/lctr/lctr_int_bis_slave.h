/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller isochronous slave interface file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_BIS_SLAVE_H
#define LCTR_INT_BIS_SLAVE_H

#include "lctr_api_bis_slave.h"
#include "lctr_int_bis.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Get BIG index. */
#define LCTR_GET_BIG_IDX(pBigCtx)        (uint8_t)(pBigCtx - &pLctrBigTbl[0])

/*! \brief      Get reservation manager handle from BIG context. */
#define LCTR_BIG_TO_RM_HANDLE(pBigCtx)   (LL_MAX_CONN + LL_MAX_ADV_SETS + LL_MAX_CIG + (pBigCtx - &pLctrBigTbl[0]))

/*! \brief      Get BIG context from reservation manager handle. */
#define LCTR_RM_HANDLE_TO_BIG(rmHandle)  (&pLctrBigTbl[(rmHandle) - LL_MAX_CONN - LL_MAX_ADV_SETS - LL_MAX_CIG])

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave BIS broadcasting states. */
enum
{
  LCTR_SLV_BIG_STATE_DISABLED,          /*!< BIS slave broadcasting disabled state. */
  LCTR_SLV_BIG_STATE_ENABLED,           /*!< BIS slave broadcasting enabled state. */
  LCTR_SLV_BIG_STATE_SHUTDOWN,          /*!< BIS slave broadcasting shutdown in progress. */
  LCTR_SLV_BIG_STATE_RESET,             /*!< BIS slave broadcasting reset in progress. */
  LCTR_SLV_BIG_STATE_TOTAL              /*!< Total number of extended advertising states. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Host events */
void lctrNotifyHostCreateBigComplete(lctrBigCtx_t *pBigCtx, uint8_t status);
void lctrNotifyHostTerminateBigComplete(lctrBigCtx_t *pBigCtx);

/* State machine for BIS slave */
void lctrSlvBigSendMsg(lctrBigCtx_t *pBigCtx, uint8_t event);
void lctrSlvBigSendAcadMsg(lctrBigCtx_t *pBigCtx, uint8_t event);
void lctrSlvBigExecuteSm(lctrBigCtx_t *pBigCtx, uint8_t event);

/* Action routines */
void lctrSlvBigActStart(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActSendChMapUpd(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActSendTerm(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActShutdown(lctrBigCtx_t *pBigCtx);
void lctrSlvBigActCleanup(lctrBigCtx_t *pBigCtx);

/* Builder */
uint8_t lctrSlvBigBuildOp(lctrBigCtx_t *pBigCtx);
uint32_t lctrGetBigRefTime(uint8_t rmHandle, uint32_t *pDurUsec);

/* ISR: Packet handlers */
void lctrSlvBisTxCompletionSequential(BbOpDesc_t *pOp, uint8_t status);
void lctrSlvBisTxCompletionInterleaved(BbOpDesc_t *pOp, uint8_t status);

/* ISR: BOD handlers */
void lctrSlvBigBeginOp(BbOpDesc_t *pOp);
void lctrSlvBigEndOp(BbOpDesc_t *pOp);
void lctrSlvBigAbortOp(BbOpDesc_t *pOp);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_BIS_SLAVE_H */
