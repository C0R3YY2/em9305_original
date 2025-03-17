/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI core platform-specific interfaces for for single-chip.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#ifndef HCI_CORE_PS_H
#define HCI_CORE_PS_H

#include "ll_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* HCI event generated internally by HCI APIs */
#define HCI_EVT_INT_TYPE          (1 << 7)

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciCoreNumCmplPkts(uint16_t handle, uint8_t numBufs);
bool hciCoreEvtProcessLlEvt(LlEvt_t *pEvt);
void hciCoreAclRecvPending(uint16_t handle, uint8_t numBufs);
void hciCoreEvtSendIntEvt(uint8_t *pEvt, uint8_t evtSize);

#ifdef __cplusplus
};
#endif

#endif /* HCI_CORE_PS_H */
