/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller common ISO interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_ISO_H
#define LCTR_API_ISO_H

#include "lctr_api.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
uint32_t LctrInitIsoMem(uint8_t *pFreeMem, uint32_t freeMemSize);
void LctrCodecInit(void);

uint8_t LctrGetConnectionAcceptTimeout(uint8_t *pBuf);
uint8_t LctrSetConnAcceptTimeout(uint16_t connTimeout);

/* Data path */
void LctrTxIso(uint8_t *pIsoBuf);
uint8_t *LctrRxIso(void);
void LctrRxIsoComplete(uint8_t numBufs);
uint8_t LctrReadIsoTxSync(uint16_t handle, uint16_t *pPktSn, uint32_t *pTs, uint32_t *pTimeOffs);
uint8_t LctrConfigureDataPath(LlIsoConfigDataPath_t *pConfigDataPath);
uint8_t LctrSetupIsoDataPath(LlIsoSetupDataPath_t *pSetupDataPath);
uint8_t LctrRemoveIsoDataPath(uint16_t handle, uint8_t dpDir);
uint8_t LctrIsoTxTest(uint16_t handle, uint8_t pldType);
uint8_t LctrIsoRxTest(uint16_t handle, uint8_t pldType);
uint8_t LctrIsoReadTestCounter(uint16_t handle, LlIsoTestCtrs_t *pStats);
uint8_t LctrIsoTestEnd(uint16_t handle, LlIsoTestCtrs_t *pStats);
uint8_t LctrReadIsoLinkQual(uint16_t handle, LlIsoLinkQual_t *pStats);
uint8_t LctrGetIsoRssi(uint16_t handle, int8_t *pRssi);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ISO_H */
