/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller enhanced connection update interface file.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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

#ifndef LCTR_API_ECU_H
#define LCTR_API_ECU_H

#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_ECU
 *  \{
 */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrEnhConnUpdateInit(void);

/* Control */
uint8_t LctrSetDefaultSubrate(LlSubrateParam_t *pSubrate);
uint8_t LctrSubrateReq(uint16_t handle, LlSubrateParam_t *pSubrate);

/*!
 *  \}
 */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ECU_H */
