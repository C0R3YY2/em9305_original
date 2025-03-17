/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  AES and random number security service API.
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
#ifndef SEC_API_H
#define SEC_API_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_SECURITY_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief CMAC algorithm key length. */
#define SEC_CMAC_KEY_LEN              16

/*! \brief CMAC algorithm key length. */
#define SEC_AES_BLK_LEN               16

/*! \brief CMAC algorithm result length. */
#define SEC_CMAC_HASH_LEN             16

/*! \brief ECC algorithm key length. */
#define SEC_ECC_KEY_LEN               32

/** \name CCM-Mode algorithm lengths
 *
 */
/**@{*/
#define SEC_CCM_KEY_LEN               16
#define SEC_CCM_MAX_ADDITIONAL_LEN    ((1U<<16) - (1U<<8))
#define SEC_CCM_L                     2
#define SEC_CCM_NONCE_LEN             (15-SEC_CCM_L)
/**@}*/

/*! \brief Invalid AES Token. */
#define SEC_TOKEN_INVALID             0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief AES Security callback parameters structure. */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< Header. */
  uint8_t       *pCiphertext;        /*!< Pointer to 16 bytes of ciphertext data. */
} secAes_t;

/*! \brief CMAC Security callback parameters structure. */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< Header. */
  uint8_t       *pCiphertext;        /*!< Pointer to 16 bytes of ciphertext data. */
  uint8_t       *pPlainText;         /*!< Pointer to pPlaintext parameter passed to SecCmac. */
} secCmacMsg_t;

/*! \brief CCM-Mode encrypt callback parameters structure. */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< Header. */
  uint8_t       *pCiphertext;        /*!< Pointer to ciphertext data. */
  uint16_t      textLen;             /*!< Length of pCiphertext in bytes. */
} secCcmEncMsg_t;

/*! \brief CCM-Mode decrypt and authenticate callback parameters structure. */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< Header. */
  uint8_t       *pText;              /*!< Pointer to decrypted text within result buffer. */
  uint8_t       *pResult;            /*!< Pointer to result buffer (passed into SecCcmDec). */
  uint16_t      textLen;             /*!< Length of pText in bytes. */
  bool          success;             /*!< true if message is authenticated. */
} secCcmDecMsg_t;

/*! \brief Generic security callback parameters structure. */
typedef union
{
  wsfMsgHdr_t    hdr;              /*!< Header. */
  secAes_t       aes;              /*!< AES complete message. */
  secCmacMsg_t   cmac;             /*!< CMAC complete message. */
  secCcmEncMsg_t ccmEnc;           /*!< CCM-Mode Encrypt complete message. */
  secCcmDecMsg_t ccmDec;           /*!< CCM-Mode Decrypt complete message. */
} secMsg_t;

/*! \brief ECC Security public/private key pair. */
typedef struct
{
  struct {
    uint8_t x[SEC_ECC_KEY_LEN]; /*!< x component of ECC public key. */
    uint8_t y[SEC_ECC_KEY_LEN]; /*!< y component of ECC public key. */
  } pubKey;
  uint8_t privKey[SEC_ECC_KEY_LEN];  /*!< ECC private key. */
} secEccKey_t;

/*! \brief ECC security DH Key shared secret. */
typedef struct
{
  uint8_t secret[SEC_ECC_KEY_LEN];   /*!< DH Key Shared secret. */
} secEccSharedSec_t;


/*! \brief ECC Security callback parameters structure. */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< Header. */
  union
  {
    secEccSharedSec_t sharedSecret;  /*!< Shared secret. */
    secEccKey_t key;                 /*!< ECC public/private key pair. */
    bool keyValid;                 /*!< true if ECC public/private key pair is valid. */
  } data;                            /*!< ECC message data union. */
} secEccMsg_t;

/*! \brief Block encryption function. */
typedef void (*SecBlkEncFunc_t)(uint8_t *pKey, uint8_t *pMessage, void *pParam);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name Security Initialization Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the security service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the random number service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRandInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the AES service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecAesInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the AES (reverse) service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecAesRevInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CMAC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCmacInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CCM security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCcmInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize ECC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecEccInit(void);

/**@}*/

/** \name Security AES, CMAC and CCM Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Execute an AES calculation.  When the calculation completes, a WSF message will be
 *          sent to the specified handler.  This function returns a token value that
 *          the client can use to match calls to this function with messages.
 *
 *  \param  pKey        Pointer to 16 byte key.
 *  \param  pPlaintext  Pointer to 16 byte plaintext.
 *  \param  handlerId   WSF handler ID.
 *  \param  param       Client-defined parameter returned in message.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return Token value.
 */
/*************************************************************************************************/
uint8_t SecAes(uint8_t *pKey, uint8_t *pPlaintext, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Execute an AES calculation.  When the calculation completes, a WSF message will be
 *          sent to the specified handler.  This function returns a token value that
 *          the client can use to match calls to this function with messages. Note this version
 *          reverses the key and plaintext bytes.
 *
 *  \param  pKey        Pointer to 16 byte key.
 *  \param  pPlaintext  Pointer to 16 byte plaintext.
 *  \param  handlerId   WSF handler ID.
 *  \param  param       Client-defined parameter returned in message.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return Token value.
 */
/*************************************************************************************************/
uint8_t SecAesRev(uint8_t *pKey, uint8_t *pPlaintext, wsfHandlerId_t handlerId,
                  uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Execute the CMAC algorithm.
 *
 *  \param  pKey          Key used in CMAC operation.
 *  \param  pPlaintext    Plain text buffer - buffer must persist until secCmacMsg_t callback.
 *  \param  textLen       Size of pPlaintext in bytes.
 *  \param  handlerId     WSF handler ID for client.
 *  \param  param         Optional parameter sent to client's WSF handler.
 *  \param  event         Event for client's WSF handler.
 *
 *  \return true if successful, else false.
 */
/*************************************************************************************************/
bool SecCmac(const uint8_t *pKey, uint8_t *pPlaintext, uint16_t textLen, wsfHandlerId_t handlerId,
             uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Execute the CCM-Mode encryption algorithm.
 *
 *  \param  pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param  pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param  pPlainText    Pointer to text to encrypt.
 *  \param  textLen       Length of pPlainText in bytes.
 *  \param  pClear        Pointer to additional, unencrypted authentication text.
 *  \param  clearLen      Length of pClear in bytes.
 *  \param  micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param  pResult       Buffer to hold result (returned in complete event).
 *  \param  handlerId     Task handler ID to receive complete event.
 *  \param  param         Optional parameter passed in complete event.
 *  \param  event         Event ID of complete event.

 *  \return true if successful, else false.
 */
/*************************************************************************************************/
bool SecCcmEnc(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pPlainText, uint16_t textLen,
               uint8_t *pClear, uint16_t clearLen, uint8_t micLen, uint8_t *pResult,
               wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Execute the CCM-Mode verify and decrypt algorithm.
 *
 *  \param  pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param  pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param  pCypherText   Pointer to text to decrypt.
 *  \param  textLen       Length of pCypherText in bytes.
 *  \param  pClear        Pointer to additional, unencrypted authentication text.
 *  \param  clearLen      Length of pClear in bytes.
 *  \param  pMic          Pointer to authentication digest.
 *  \param  micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param  pResult       Buffer to hold result (returned in complete event).
 *  \param  handlerId     Task handler ID to receive complete event.
 *  \param  param         Optional parameter passed in complete event.
 *  \param  event         Event ID of complete event.
 *
 *  \return true if successful, else false.
 */
/*************************************************************************************************/
bool SecCcmDec(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pCypherText, uint16_t textLen,
               uint8_t *pClear, uint16_t clearLen, uint8_t *pMic, uint8_t micLen,
               uint8_t *pResult, wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/**@}*/

/** \name Security ECC Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Generate an ECC key.
 *
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return true if successful, else false.
 */
/*************************************************************************************************/
bool SecEccGenKey(wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Generate an ECC key.
 *
 *  \param  pKey        ECC Key structure.
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return true if successful, else false.
 */
/*************************************************************************************************/
bool SecEccGenSharedSecret(secEccKey_t *pKey, wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/**@}*/

/** \name Security Random Number Generator Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  This function returns up to 16 bytes of random data to a buffer provided by the
 *          client.
 *
 *  \param  pRand       Pointer to returned random data.
 *  \param  randLen     Length of random data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRand(uint8_t *pRand, uint8_t randLen);

/**@}*/

/*! \} */    /* STACK_SECURITY_API */

#ifdef __cplusplus
};
#endif

#endif /* SEC_API_H */
