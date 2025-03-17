/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  L2CAP main module.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#ifndef L2C_MAIN_H
#define L2C_MAIN_H

#include "l2c_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Return the next L2CAP signaling req identifier.  Cannot be zero. */
#define L2C_NEXT_ID(id)         (((id) == 255) ? (1) : ((id) + 1))

/* L2C event handler message types */
#define L2C_MSG_REQ_TIMEOUT             1     /* L2CAP slave signaling request timeout */
#define L2C_MSG_TYPE_MAX                1

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data callback with CID */
typedef void (*l2cDataCidCback_t)(uint16_t handle, uint16_t cid, uint16_t len, uint8_t *pPacket);

/* Main control block of the L2C subsystem */
typedef struct
{
  l2cDataCback_t    attDataCback;             /* Data callback for ATT */
  l2cDataCback_t    smpDataCback;             /* Data callback for SMP */
  l2cDataCback_t    l2cSignalingCback;        /* Data callback for L2CAP signaling */
  l2cCtrlCback_t    attCtrlCback;             /* Control callback for ATT */
  l2cCtrlCback_t    smpCtrlCback;             /* Control callback for SMP */
  l2cCtrlCback_t    l2cCocCtrlCback;          /* Control callback for L2CAP connection oriented channels */
  l2cDataCback_t    masterRxSignalingPkt;     /* Master signaling packet processing function */
  l2cDataCback_t    slaveRxSignalingPkt;      /* Slave signaling packet processing function */
  l2cDataCidCback_t l2cDataCidCback;          /* Data callback for L2CAP on other CIDs */
  uint8_t           identifier;               /* Signaling request identifier */
} l2cCb_t;

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

void l2cSendCmdReject(uint16_t handle, uint8_t identifier, uint16_t reason);
void l2cRxSignalingPkt(uint16_t handle, uint16_t len, uint8_t *pPacket);
void *l2cMsgAlloc(uint16_t len);

/* Control block */
extern l2cCb_t l2cCb;

#ifdef __cplusplus
};
#endif

#endif /* L2C_MAIN_H */
