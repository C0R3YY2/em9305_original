/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller power control interface file.
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

#ifndef LCTR_INT_PC_H
#define LCTR_INT_PC_H

#include "lctr_api.h"
#include "lctr_int_conn_slave.h"
#include "lctr_int_conn_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************
 * Function Declarations
*************************************************************************************************/

uint8_t lctrCalcPathLossZone(lctrConnCtx_t *pCtx);
void lctrNotifyHostPathLossRpt(lctrConnCtx_t *pCtx);
void lctrPathLossMonitorAct(lctrConnCtx_t *pCtx);

/* Power indication actions. */
void lctrStorePeerPowerInd(lctrConnCtx_t *pCtx);
void lctrSendPeerPowerRsp(lctrConnCtx_t *pCtx);

/* Power control actions. */
void lctrStorePowerControlAction(lctrConnCtx_t *pCtx);
void lctrSendPeerPowerControlReq(lctrConnCtx_t *pCtx);
void lctrStorePeerPowerControlReq(lctrConnCtx_t *pCtx);
void lctrSendPeerPowerControlRsp(lctrConnCtx_t *pCtx);
void lctrStorePeerPowerControlRsp(lctrConnCtx_t *pCtx);

/* Power monitoring actions. */
void lctrConnServicePowerMonitor(lctrConnCtx_t *pCtx);

/* Power reporting actions. */
void lctrNotifyPowerReportInd(lctrConnCtx_t *pCtx, uint8_t status, uint8_t reason, uint8_t phy, int8_t txPower, uint8_t limits, int8_t delta);
int8_t lctrAttemptTxPowerChange(lctrConnCtx_t *pCtx, uint8_t phy, int8_t delta);
void lctrSendPowerChangeInd(lctrConnCtx_t *pCtx, uint8_t phy, int8_t delta, int8_t txPower, bool phyChange);

/* State machine */
bool lctrLlcpExecutePclSm(lctrConnCtx_t *pCtx, uint8_t event);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_PC_H */
