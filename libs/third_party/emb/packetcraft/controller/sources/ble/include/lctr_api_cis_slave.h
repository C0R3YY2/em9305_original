/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS slave interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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

#ifndef LCTR_API_CIS_SLAVE_H
#define LCTR_API_CIS_SLAVE_H

#include "lctr_api.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
uint32_t LctrInitCisMem(uint8_t *pFreeMem, uint32_t freeMemSize);

/* Control */
uint8_t LctrRejectCisReq(uint16_t cisHandle, uint8_t reason);
uint8_t LctrAcceptCisReq(uint16_t cisHandle);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_CIS_SLAVE_H */
