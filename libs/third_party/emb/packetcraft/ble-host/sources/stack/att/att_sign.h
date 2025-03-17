/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  ATT optional signed PDU processing functions.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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
#ifndef ATT_SIGN_H
#define ATT_SIGN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* CMAC algorithm subkey length */
#define ATT_CMAC_SUBKEY_LEN       16

/* CMAC algorithm block length */
#define ATT_CMAC_BLOCK_LEN        16

/* CMAC constant Rb */
#define ATT_CMAC_RB               0x87

/* CMAC signature result length */
#define ATT_CMAC_RESULT_LEN       8


#ifdef __cplusplus
};
#endif

#endif /* ATT_SIGN_H */
