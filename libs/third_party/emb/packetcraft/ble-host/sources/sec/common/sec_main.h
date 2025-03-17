/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal security service structures.
 *
 *  Copyright (c) 2010-2019 Arm Ltd. All Rights Reserved.
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
#ifndef SEC_MAIN_H
#define SEC_MAIN_H

#include "hci_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/


/*! AES, CMAC and HCI algorithm block length */
#define SEC_BLOCK_LEN             16U

/* CMAC constant Rb */
#define SEC_CMAC_RB               0x87U

/*! Number or random bytes to keep in the secCb_t rand data buffer */
#define SEC_RAND_DATA_LEN         32

/*! Multiple of HCI_RAND_LEN kept in the secCb_t rand data buffer */
#define SEC_HCI_RAND_MULT         (SEC_RAND_DATA_LEN / HCI_RAND_LEN)

/*! Compile time ECC configuration */
#define SEC_ECC_CFG_DEBUG         0
#define SEC_ECC_CFG_UECC          1
#define SEC_ECC_CFG_HCI           2

/*! Compile time CMAC configuration */
#define SEC_CMAC_CFG_PLATFORM     0
#define SEC_CMAC_CFG_HCI          1

/*! Compile time CCM configuration */
#define SEC_CCM_CFG_PLATFORM      0
#define SEC_CCM_CFG_HCI           1

/*! CCM Operation (Encryption or Decryption) */
#define SEC_CCM_OP_ENCRYPT        0
#define SEC_CCM_OP_DECRYPT        1

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Enumeration of security callback types */
enum
{
  SEC_TYPE_AES,
  SEC_TYPE_CMAC,
  SEC_TYPE_DH,
  SEC_TYPE_CCM_ENC,
  SEC_TYPE_CCM_DEC,
  SEC_TYPE_AES_REV,
  SEC_NUM_TYPES
};

/*! Security queue element for CMAC operations */
typedef struct
{
  uint8_t        *pPlainText;
  uint8_t        key[SEC_CMAC_KEY_LEN];
  uint8_t        subkey[SEC_CMAC_KEY_LEN];
  uint16_t       position;
  uint16_t       len;
  wsfHandlerId_t handlerId;
  uint8_t        state;
} secCmacSecCb_t;

/*! Security queue element for CCM-Mode operations */
typedef struct
{
  uint8_t        operation;
  uint8_t        *pText;
  uint8_t        *pClear;
  uint8_t        *pRcvMic;
  uint16_t       textLen;
  uint16_t       clearLen;
  uint8_t        micLen;
  uint8_t        key[SEC_CCM_KEY_LEN];
  uint8_t        scratch[SEC_BLOCK_LEN];
  uint16_t       counter;
  uint16_t       position;
  wsfHandlerId_t handlerId;
  uint8_t        state;
  uint8_t        *pWorking;
} secCcmSecCb_t;

/*! Security queue element for AES - Note: must match structure of secQueueBuf_t */
typedef struct
{
  secMsg_t       msg;
  uint8_t        plainText[SEC_BLOCK_LEN];
  uint8_t        key[SEC_BLOCK_LEN];
  void           *pCb;
  uint8_t        type;
  uint8_t        handlerId;
} secAesSecCb_t;

/*! Security queue element */
typedef struct
{
  secMsg_t       msg;
  uint8_t        ciphertext[SEC_BLOCK_LEN];
  uint8_t        reserved[SEC_BLOCK_LEN];
  void           *pCb;
  uint8_t        type;
} secQueueBuf_t;

/* Encryption HCI callback function type. */
typedef void secHciCback_t(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId);
typedef secHciCback_t *pSecHciCback_t;

/* AES operation function type. */
typedef void secAesOp_t(secQueueBuf_t *pCb);
typedef secAesOp_t *pSecAesOp_t;

/* Control block */
typedef struct
{
  uint8_t        rand[SEC_RAND_DATA_LEN];                /* Random data buffer */
  wsfQueue_t     aesQueue;                               /* Queue for access to HCI LE Encrypt command */
  wsfQueue_t     pubKeyQueue;                            /* Queue for read p256 public key requests */
  wsfQueue_t     dhKeyQueue;                             /* Queue for generate dh key requests */
  uint8_t        token;                                  /* Token value */
  uint8_t        randTop;                                /* Random buffer insert point (HCI_RAND_LEN bytes) */
  uint8_t        randBtm;                                /* Random buffer copy point (HCI_RAND_LEN bytes) */
  pSecHciCback_t hciCbackTbl[SEC_NUM_TYPES];
  pSecAesOp_t    opActionTbl[SEC_NUM_TYPES];
  secQueueBuf_t  *pQueueBuf;
  uint8_t        aesHandler;
  bool           aesInProgress;
} secCb_t;

/**************************************************************************************************
  External Variables
**************************************************************************************************/

extern secCb_t secCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Queue or execute the action functions for a security operation requiring use of the
 *          HCI LE Encrypt command.
 *
 *  \param  type        Operation type.
 *  \param  pCb         Pointer to the control block for the operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecQueueOperation(uint8_t type, void *pCb);

/*************************************************************************************************/
/*!
 *  \brief  Register the HCI callback and action function for a security algorithm that
 *          requires use of the HCI LE Encrypt command.
 *
 *  \param  type        Operation type.
 *  \param  actionFunc  Action function.
 *  \param  hciCback    HCI callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRegisterOpFunc(uint8_t type, pSecAesOp_t actionFunc, pSecHciCback_t hciCback);

/*************************************************************************************************/
/*!
 *  \brief  Queue callback and call LE encrypt given most significant byte ordered key and data.
 *
 *  \param  pKey        Pointer to key.
 *  \param  pText       Pointer to text to encrypt.
 *  \param  pBuf        Pointer to queue block.
 *  \param  handlerId   Handler ID.
 *  \param  msb         true if the input is MSB first.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecLeEncryptCmd(uint8_t *pKey, uint8_t *pText, void *pBuf, wsfHandlerId_t handlerId, bool msb);

#ifdef __cplusplus
};
#endif

#endif /* SEC_MAIN_H */
