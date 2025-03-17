/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller enhanced connection update interface file.
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

#ifndef LCTR_INT_ECU_H
#define LCTR_INT_ECU_H

#include "lctr_api.h"
#include "lctr_int_conn.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************
 * Function Declarations
*************************************************************************************************/

/* State machine */
bool lctrPerExecuteEcuSm(lctrConnCtx_t *pCtx, uint8_t event);
bool lctrCenExecuteEcuSm(lctrConnCtx_t *pCtx, uint8_t event);

/* Host notification */
void lctrNotifyHostSubrateChangeIndSuccess(uint16_t handle, uint16_t srFactor, uint16_t perLatency,
                                     uint16_t contNum, uint16_t svt);
void lctrNotifyHostSubrateChangeIndFailed(uint16_t handle, uint8_t status);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ECU_H */
