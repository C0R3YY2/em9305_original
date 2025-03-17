/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM PHY module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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
#ifndef DM_PHY_H
#define DM_PHY_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* component interface */
void dmPhyHciHandler(hciEvt_t *pEvent);

#ifdef __cplusplus
};
#endif

#endif /* DM_PHY_H */
