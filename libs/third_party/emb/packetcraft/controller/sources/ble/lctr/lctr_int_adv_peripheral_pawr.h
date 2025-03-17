/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller PAwR peripheral interface file.
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

#ifndef LCTR_INT_ADV_PERIPHERAL_PAWR_H
#define LCTR_INT_ADV_PERIPHERAL_PAWR_H

#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void lctrSlvPeriodicAdvWithRspEndOp(BbOpDesc_t *pOp);
void lctrSlvPeriodicAdvWithRspAbortOp(BbOpDesc_t *pOp);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_MASTER_H */
