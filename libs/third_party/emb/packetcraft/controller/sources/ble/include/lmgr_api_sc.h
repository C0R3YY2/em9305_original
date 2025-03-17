/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager secure connections interface file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
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

#ifndef LMGR_API_SC_H
#define LMGR_API_SC_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_SC
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Secure connections parameter definition. */
typedef struct
{
  uint8_t   privKey[LL_ECC_KEY_LEN];    /*!< P-256 private key. */
  bool      eccOpActive;                /*!< An ECC generation operation is active. */
  bool      privKeySet;                 /*!< P-256 private key set; do not generate new one. */
  bool      pubKeyValid;                /*!< DHKey won't start to generate unless public key is valid. */
  uint8_t   validatePubKeyMode;         /*!< Validate public key mode between ALT1 and ALT2. */
} lmgrScCtrlBlk_t;

/*! \brief      Key validation mode between ALT1 and ALT2. */
enum
{
  KEY_VALIDATE_MODE_ALT2 = 0,           /*!< Return error code 0x12 in command complete event. (Default mode) */
  KEY_VALIDATE_MODE_ALT1 = 1,           /*!< Return error code 0x12 in command status event. */
  KEY_VALIDATE_MODE_MAX  = 1            /*!< Key validate mode maximum value. */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrScCtrlBlk_t lmgrScCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrScInit(void);

/*! \} */    /* LL_LMGR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_SC_H */
