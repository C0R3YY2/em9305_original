/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP responder main module.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.  All rights reserved.
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
#ifndef SMPR_MAIN_H
#define SMPR_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* state machine interface */
extern const smpSmIf_t smprSmIf;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void smprActSendSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendPairRsp(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairCnf(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairCnfCalc1(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActCnfVerify(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendPairRandom(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSetupKeyDist(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendKey(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActRcvKey(smpCcb_t *pCcb, smpMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* SMPR_MAIN_H */
