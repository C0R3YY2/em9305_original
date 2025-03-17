/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller PAwR central interface file.
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

#ifndef LCTR_INT_ADV_CENTRAL_PAWR_H
#define LCTR_INT_ADV_CENTRAL_PAWR_H

#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Globals
**************************************************************************************************/
extern lctrMstPerScanIsr_t lctrMstPerScanIsr;

#if (LL_ENABLE_RSPOBS && LL_ENABLE_TESTER)
extern uint8_t *lctrCenSyncRspDataBufTbl[LL_MAX_PAWR_RSP_SLOTS][LL_MAX_PER_SCAN];
#else
extern uint8_t *lctrCenSyncRspDataBufTbl[LL_MAX_PER_SCAN];
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void lctrPerAcadPawrHandler(lctrPerScanCtx_t *pPerScanCtx, uint8_t acadLen, uint8_t opcode, const uint8_t *pBuf);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_CENTRAL_PAWR_H */
