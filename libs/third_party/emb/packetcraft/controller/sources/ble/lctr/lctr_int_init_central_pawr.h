/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller PAwR master interface file.
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

#ifndef LCTR_INT_INIT_CENTRAL_PAWR_H
#define LCTR_INT_INIT_CENTRAL_PAWR_H

#include "lctr_api_init_master_ae.h"
#include "lctr_int.h"
#include "lctr_int_adv_master_ae.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrCenExtInitPawrExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event);

/* Action routines. */
void lctrExtInitPawrActInitiate(lctrAdvSet_t *pAdvSet);
void lctrExtInitPawrActCancel(lctrAdvSet_t *pAdvSet);
void lctrExtInitPawrActConnect(lctrAdvSet_t *pAdvSet);
void lctrExtInitPawrActTerm(lctrAdvSet_t *pAdvSet);
void lctrExtInitPawrActDisallowInitiate(lctrAdvSet_t *pAdvSet);

/* Helper routines. */
void lctrMstExtInitPawrCleanupOp(void);
void lctrAdvPawrNotifyHostInitiateError(uint8_t handle, uint8_t reason, uint8_t peerAddrType, uint64_t peerAddr);
uint8_t lctrExtInitSetupInitiatePawr(lctrAdvSet_t *pAdvSet, uint8_t peerAddrType, uint64_t peerAddr, uint8_t filtPolicy, uint8_t ownAddrType);

/* Messaging */
void lctrSendExtInitPawrMsg(lctrAdvSet_t *pAdvSet, uint8_t event);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_INIT_CENTRAL_PAWR_H */
