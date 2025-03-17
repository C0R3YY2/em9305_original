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

#ifndef LCTR_API_CTE_H
#define LCTR_API_CTE_H

#include "lctr_api.h"
#include "ll_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization functions. */
void LctrConstantToneExtInit(void);

/* HCI functions. */
uint8_t LctrReadAntennaInfo(uint8_t *pBuf);
uint8_t LctrSetConnlessCteTxParams(uint8_t advHandle, LlConnLessCteTxParam_t connlessCteTxParam);
uint8_t LctrSetConnlessCteTxEnable(uint8_t advHandle, uint8_t enable);
uint8_t LctrSetConnlessIqSampEnable(uint16_t syncHandle, uint8_t enable, LlConnLessIqSampEnableParam_t connlessIqSampEnableParam);
uint8_t LctrSetConnCteRecvParams(uint16_t handle, uint8_t sampEnable, uint8_t slotDur, uint8_t switchPatternLen, uint8_t *pAntennaId);
uint8_t LctrSetConnCteTransParams(uint16_t handle, uint8_t cteTypes, uint8_t switchPatternLen, uint8_t *pAntennaId);
uint8_t LctrConnCteReqEnable(uint16_t handle, uint8_t enable, uint16_t cteReqInt, uint8_t reqCteLen, uint8_t reqCteType);
uint8_t LctrConnCteRspEnable(uint16_t handle, uint8_t enable);

/* Event generation. */
void lctrNotifyCteReqFailEvt(uint16_t handle, uint8_t status);

/* Helper functions. */
bool LctrIsPerAdvCteEnabled(uint8_t handle);
bool LctrIsOneUsSwitchingSamplingSupported(void);
bool LctrIsCteTypesSupported(uint8_t cteTypes);
bool LctrIsAntennaConfigSupported(uint8_t switchLen, uint8_t *pSwitchPattern);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_CTE_H */
