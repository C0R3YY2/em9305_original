/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller power control interface file.
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

#ifndef LCTR_API_PC_H
#define LCTR_API_PC_H

#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization functions. */
void LctrPowerControlInit(void);

/* HCI functions. */
uint8_t lctrSetPathLossReportingParams(uint16_t handle, uint8_t highThresh, uint8_t highHyst, uint8_t lowThresh, uint8_t lowHyst, uint16_t minTime);
uint8_t lctrSetPathLossReportingEnable(uint16_t handle, uint8_t enable);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PC_H */
