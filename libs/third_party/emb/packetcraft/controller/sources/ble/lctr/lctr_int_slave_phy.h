/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller PHY features (slave) interface file.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_PHY_SLAVE_H
#define LCTR_INT_PHY_SLAVE_H

#include "lctr_int.h"
#include "lctr_int_conn.h"

#ifdef __cplusplus
"C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
bool lctrSlvLlcpExecutePhyUpdateSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Action routines */
void lctrStoreHostPhyUpdate(lctrConnCtx_t *pCtx);
void lctrStorePeerPhyReq(lctrConnCtx_t *pCtx);
void lctrStorePeerPhyUpdateInd(lctrConnCtx_t *pCtx);

void lctrSendPhyReqPdu(lctrConnCtx_t *pCtx, uint8_t txPhys, uint8_t rxPhys);
void lctrSendPhyRspPdu(lctrConnCtx_t *pCtx, uint8_t txPhys, uint8_t rxPhys);
void lctrSendPhyUpdateIndPdu(lctrConnCtx_t *pCtx, uint8_t txPhys, uint8_t rxPhys);

void lctrNotifyHostPhyUpdateInd(lctrConnCtx_t *pCtx, uint8_t status);

bool lctrSlvCheckEncOverridePhyUpdate(lctrConnCtx_t *pCtx);

/* Utility routines */
void lctrActPeerRejectPhyReq(lctrConnCtx_t *pCtx);
void lctrActNotifyHostPhyUpdateSuccess(lctrConnCtx_t *pCtx);
uint8_t lctrChoosePreferredPhy(uint8_t val);
void lctrActStartPhyUpdate(lctrConnCtx_t *pCtx);
void lctrActFlushArq(lctrConnCtx_t *pCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_PHY_SLAVE_H */
