/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller Constant tone extension interface file.
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

#ifndef LCTR_INT_CTE_H
#define LCTR_INT_CTE_H

#include "lctr_api.h"
#include "wsf_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine. */
bool lctrLlcpExecuteCteSm(lctrConnCtx_t *pCtx, uint8_t event);

/* State machine actions. */
void lctrSendCteRequest(lctrConnCtx_t *pCtx);
void lctrStoreCteRequest(lctrConnCtx_t *pCtx);
void lctrSendCteResponse(lctrConnCtx_t *pCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CTE_H */
