/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS master interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_CIS_MASTER_H
#define LCTR_API_CIS_MASTER_H

#include "lctr_api.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstCisInit(void);

/* Control */
uint8_t LctrSetCigParam(LlCisCigParams_t *pSetCigParam, uint16_t *pCisHandles);
uint8_t LctrSetCigParamTest(LlCisCigParamsTest_t *pSetCigParamTest, uint16_t *pCisHandles);
uint8_t LctrRemoveCig(uint8_t cigId);
uint8_t LctrCreateCis(uint8_t numCis, LlCisCreateCisParams_t *pCreateCisParam);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_CIS_MASTER_H */
