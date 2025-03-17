/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Crypto driver definition.
 *
 *  Copyright (c) 2018-2019 ARM Ltd. All Rights Reserved.
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

#ifndef PAL_CRYPTO_H
#define PAL_CRYPTO_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_CRYPTO
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      AES block size. */
#define PAL_CRYPTO_AES_BLOCK_SIZE                16

#define PAL_CRYPTO_LL_KEY_LEN                    16      /*!< Encryption key length. */
#define PAL_CRYPTO_LL_IV_LEN                     8       /*!< Initialization vector length. */
#define PAL_CRYPTO_LL_DATA_MIC_LEN               4       /*!< Data channel PDU MIC length. */

/*! \brief      CCM-Mode algorithm lengths. */
#define SEC_CCM_KEY_LEN                          16

/*! \brief      CCM-Mode algorithm maximum additional length. */
#define SEC_CCM_MAX_ADDITIONAL_LEN               ((1U<<16) - (1U<<8))

/*! \brief      CCM-Mode algorithm length. */
#define SEC_CCM_L                                2

/*! \brief      CCM-Mode algorithm nonce length. */
#define SEC_CCM_NONCE_LEN                        (15-SEC_CCM_L)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Operational states. */
enum
{
  PAL_CRYPTO_STATE_UNINIT = 0,      /*!< Uninitialized state. */
  PAL_CRYPTO_STATE_ERROR  = 0,      /*!< Error state. */
  PAL_CRYPTO_STATE_READY            /*!< Ready state. */
};
typedef uint8_t PalCryptoState_t;

/*! \brief      Encryption data. */
typedef struct
{
  /* SK placed here for 32-bit alignment. */
  uint8_t       sk[PAL_CRYPTO_LL_KEY_LEN]; /*!< Session/Encryption key. */
  uint8_t       iv[PAL_CRYPTO_LL_IV_LEN];  /*!< Initialization vector. */
  bool          enaEncrypt;         /*!< Tx/Encryption enabled flag. */
  bool          enaDecrypt;         /*!< Rx/Decryption enabled flag. */
  bool          enaAuth;            /*!< Enable authentication. */
  uint8_t       nonceMode;          /*!< Nonce mode. */
  uint16_t      *pEventCounter;     /*!< Connection event counter. */
  uint64_t      *pTxPktCounter;     /*!< Tx packet counter. Set when nonceMode = PAL_BB_NONCE_MODE_EXT64_CNTR. */
  uint64_t      *pRxPktCounter;     /*!< Rx packet counter. Set when nonceMode = PAL_BB_NONCE_MODE_EXT64_CNTR. */
  uint8_t       dir;                /*!< Direction value. */
  uint8_t       type;               /*!< Type, ACL, CIS, BIS */
  void          *pEncryptCtx;       /*!< Tx/Encryption context. */
  void          *pDecryptCtx;       /*!< Rx/Decryption context. */
} PalCryptoEnc_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CCM-Mode security.
 */
/*************************************************************************************************/
void PalCryptoInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to de-initialize CCM-Mode security.
 */
/*************************************************************************************************/
void PalCryptoDeInit(void);

/* Key generation */

/*************************************************************************************************/
/*!
 *  \brief      Computes the ECC public key from the provided private key.
 *
 *  \param      pPrivKey      The ECC private Key.
 *  \param      pPubKey       The generated ECC public key.
 *
 *  \note       Input and output parameters are in little endian format.
 */
/*************************************************************************************************/
void PalCryptoGenerateP256KeyPair(const uint8_t *pPrivKey, uint8_t *pPubKey);

/*************************************************************************************************/
/*!
 *  \brief      Computes the shared key between 2 devices.
 *
 *  \param      pPubKey      The ECC public Key.
 *  \param      pPrivKey     The ECC private Key.
 *  \param      pDhKey       The generated ECC shared key.
 *
 *  \note   Input and output parameters are in little endian format.
 */
/*************************************************************************************************/
void PalCryptoGenerateDhKey(const uint8_t *pPubKey, const uint8_t *pPrivKey, uint8_t *pDhKey);

/*************************************************************************************************/
/*!
 *  \brief      Validates the public ECC key.
 *
 *  \param      pPubKey      The ECC public Key.
 *  \param      generateKey  TBD.
 *
 *  \return     TBD
 */
/*************************************************************************************************/
bool PalCryptoValidatePublicKey(const uint8_t *pPubKey, bool generateKey);

/*************************************************************************************************/
/*!
 *  \brief      Generate cryptographic grade random number.
 *
 *  \param      pBuf        Buffer to store random number.
 *  \param      len         Number of bytes.
 */
/*************************************************************************************************/
void PalCryptoGenerateRandomNumber(uint8_t *pBuf, uint8_t len);

/* CCM */

/*************************************************************************************************/
/*!
 *  \brief      Execute the CCM-Mode verify and decrypt algorithm.
 *
 *  \param      pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param      pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param      pCypherText   Pointer to text to decrypt.
 *  \param      textLen       Length of pCypherText in bytes.
 *  \param      pClear        Pointer to additional, unencrypted authentication text.
 *  \param      clearLen      Length of pClear in bytes.
 *  \param      pMic          Pointer to authentication digest.
 *  \param      micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param      pResult       Buffer to hold result (returned in complete event).
 *  \param      handlerId     Task handler ID to receive complete event.
 *  \param      param         Optional parameter passed in complete event.
 *  \param      event         Event ID of complete event.
 *
 *  \return Error code.
 */
/*************************************************************************************************/
uint32_t PalCryptoCcmDec(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pCypherText, uint16_t textLen,
                         uint8_t *pClear, uint16_t clearLen, uint8_t *pMic, uint8_t micLen,
                         uint8_t *pResult, uint8_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief      Execute the CCM-Mode encryption algorithm.
 *
 *  \param      pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param      pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param      pPlainText    Pointer to text to encrypt.
 *  \param      textLen       Length of pPlainText in bytes.
 *  \param      pClear        Pointer to additional, unencrypted authentication text.
 *  \param      clearLen      Length of pClear in bytes.
 *  \param      micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param      pResult       Buffer to hold result (returned in complete event).
 *  \param      handlerId     Task handler ID to receive complete event.
 *  \param      param         Optional parameter passed in complete event.
 *  \param      event         Event ID of complete event.
 */
/*************************************************************************************************/
void PalCryptoCcmEnc(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pPlainText, uint16_t textLen,
                     uint8_t *pClear, uint16_t clearLen, uint8_t micLen, uint8_t *pResult,
                     uint8_t handlerId, uint16_t param, uint8_t event);

/* Crypto AES */

/*************************************************************************************************/
/*!
 *  \brief      Calculate AES CBC.
 *
 *  \param      pKey        Encryption key.
 *  \param      pOut        Output data.
 *  \param      pIn         Input data.
 *
 *  \note       Packet length is 16 bytes.
 */
/*************************************************************************************************/
void PalCryptoAesEcb(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn);

/*************************************************************************************************/
/*!
 *  \brief      Calculate AES CMAC.
 *
 *  \param      pKey        Encryption key.
 *  \param      pOut        Output data.
 *  \param      pIn         Input data.
 *  \param      len         Length.
 *
 *  \note       Packet length is 16 bytes.
 */
/*************************************************************************************************/
void PalCryptoAesCmac(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Enable AES cipher block.
 *
 *  \param      pEnc        Encryption parameters.
 *  \param      id          Context ID.
 *  \param      localDir    Direction bit of local device (0=slave, 1=master).
 */
/*************************************************************************************************/
void PalCryptoAesEnable(PalCryptoEnc_t *pEnc, uint8_t id, uint8_t localDir);

/*************************************************************************************************/
/*!
 *  \brief      Execute AES CBC transformation on payload and add 4 byte MIC.
 *
 *  \param      pEnc        Encryption parameters.
 *  \param      pHdr        Packet header.
 *  \param      pBuf        Packet data.
 *  \param      pMic        Storage for MIC.
 *
 *  \return     true if the MIC was set.
 *
 *  This routine completes the transformation in a blocking manner. This routine modifies the
 *  length field of the PDU to account for the MIC.
 *
 *  \note       Leave this implementation empty if inline hardware encryption is available.
 */
/*************************************************************************************************/
bool PalCryptoAesCcmEncrypt(PalCryptoEnc_t *pEnc, uint8_t *pHdr, uint8_t *pBuf, uint8_t *pMic);

/*************************************************************************************************/
/*!
 *  \brief      Execute AES CBC transformation on payload and return MIC evaluation status.
 *
 *  \param      pEnc        Encryption parameters.
 *  \param      pBuf        Packet data.
 *
 *  \return     true if authentication successful, false otherwise.
 *
 *  This routine completes the transformation in a blocking manner. This routine modifies the
 *  length field of the PDU to account for the MIC.
 *
 *  \note       Leave this implementation empty if inline hardware encryption is available.
 */
/*************************************************************************************************/
bool PalCryptoAesCcmDecrypt(PalCryptoEnc_t *pEnc, uint8_t *pBuf);

/*! \} */    /* PAL_CRYPTO */

#ifdef __cplusplus
};
#endif

#endif /* PAL_CRYPTO_H */
