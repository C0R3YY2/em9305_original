/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP initiator main module.
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
#ifndef SMPI_MAIN_H
#define SMPI_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* state machine interface */
extern const smpSmIf_t smpiSmIf;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* state machine action functions */
void smpiActPairReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActCheckSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcPairRsp(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcPairCnf(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActCnfVerify(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActStkEncrypt(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActSetupKeyDist(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActRcvKey(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActSendKey(smpCcb_t *pCcb, smpMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* SMPI_MAIN_H */
