////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_update_crypto/includes/fwu_crypto.h
///
/// @project    T9305
///
/// @brief      Firmware update crypto library.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef _FWU_CRYPTO_H
#define _FWU_CRYPTO_H

#include "types.h"
#include "fwu_error_code.h"
#include "ECCTypedef.h"
#include "AES.h"
#include "AES_CBC.h"
#include "AES_CTR.h"
#include "AES_ECB.h"
#include "SHA256.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup fwu_crypto Firmware Update Crypto library
 * @brief Cryptographic toolbox for firmware update feature.
 * @{
*/


// Type definition

/**
 * @brief Firmware Update Crypto library - Public Key Location
 */

typedef enum
{
    FWU_CRYPTO_PUBLIC_KEY_EM = 0,
    FWU_CRYPTO_PUBLIC_KEY_USER,
} FWU_CRYPTO_Pub_Key_Location_t;

/**
 * @brief Firmware Update Crypto library - Crypto Algorithm Type Location
 */

typedef enum
{
    FWU_CRYPTO_NONE = 0,
    FWU_CRYPTO_AES_ECB,
    FWU_CRYPTO_AES_CBC,
    FWU_CRYPTO_AES_CTR
} FWU_CRYPTO_Pub_Key_Type_t;

/**
 * @brief  Convert uint8 based vector to uint32 vector.
 *
 * @param src Pointer to the source vector
 * @param dst Pointer to the destination vertor
 * @param size size in 32-bit word
 */
void FWUCrypto_ConvertByteToU32(uint8_t * src, uint32_t* dst, uint32_t size);

/**
 * @brief Reads the public key from the user or EM information page.
 *
 * @param keyLocation Location of key (either FWU_CRYPTO_KEY_TYPE_EM or FWU_CRYPTO_KEY_TYPE_USER)
 * @param pPublicKey Pointer to the selected public key
 * @return FWU_ERROR_CODE_SUCCESS if the procedure succeeds, or FWU_ERROR_CODE_PUBLIC_KEY_ERROR if something is wrong with the public key.
 */
FWU_ErrorCode_t FWUCrypto_GetSignaturePublicKey(FWU_CRYPTO_Pub_Key_Location_t keyLocation, POINT_P256 *pPublicKey);


/**
 * @brief  Initialize the SHA256 calculation
 *
 * @return FWU_ERROR_CODE_SUCCESS if the procedure succeeds, FWU_ERROR_CODE_SHA256_ERROR otherwise.
 */
FWU_ErrorCode_t FWUCrypto_SHA256_Init(void);


/**
 * @brief  Update intermediate SHA256 calculation with new data
 *
 * @param pData pointer to the new data
 * @param length length of the new data to process
 * @return FWU_ERROR_CODE_SUCCESS if the procedure succeeds, FWU_ERROR_CODE_SHA256_ERROR otherwise.
 */
FWU_ErrorCode_t FWUCrypto_SHA256_Update(uint8_t *pData, uint32_t length);


/**
 * @brief  Finalize the SHA256 calculation and generates a Digest.
 *
 * @param digest Pointer to digest being calculated since the last
 *        call to FWU_Crypto_Init_SHA256
 * @return FWU_ERROR_CODE_SUCCESS if the procedure succeeds, FWU_ERROR_CODE_SHA256_ERROR otherwise.
 */
FWU_ErrorCode_t FWUCrypto_SHA256_Finalize(uint8_t digest[SHA256_DIGEST_SIZE]);


/**
 * @brief  Verify the signature of a firmware.
 *
 * @param pSignature Pointer to the public key to use for signature verification.
 * @param pPublicKey Length of the command parameters.
 * @param digest Pointer to the digest used to verify the signature
 * @return FWU_ERROR_CODE_SUCCESS if the Signature is valid,
 *         FWU_ERROR_CODE_INVALID_FIRMWARE_SIGNATURE if the signature verification fails,
 *         FWU_ERROR_CODE_PUBLIC_KEY_ERROR if the public key is invalid or
 *         FWU_ERROR_CODE_UNSPECIFIED_ERROR for any other unspecified error.
 */
FWU_ErrorCode_t FWUCrypto_VerifyFirmwareSignature(PointCertificate *pSignature,
    POINT *pPublicKey, uint8_t digest[SHA256_DIGEST_SIZE]);


/**
 * @brief  Initialize the AES ECB engine.
 *
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_ECB_KEY_ID of AES_ECB_KEY_VALUE
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR in case of initialization error.
 */
FWU_ErrorCode_t FWUCrypto_AES_ECB_Init(uint8_t *pKey, AES_ECB_KeyType_t keyType);


/**
 * @brief Decrypt an AEC ECB encrypted block.
 * @details  Decrypt block of data using AES ECB. The size corresponds to the
 *        encryptedDataSize as the padding is not removed and shall be done
 *        manually. This function is useful for large firmware that must be
 *        decrypted by block, where padding is removed to the last block.
 *
 *        FWUCrypto_AES_ECB_Init shall be called prior calling this function.
 *
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pSize Pointer to the size of the decrypted data
 *        after removal of the padding.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if a data block cannot be decrypted.
 */
FWU_ErrorCode_t FWUCrypto_AES_ECB_Decrypt_Block(uint8_t *pEncryptedData,
    uint32_t encryptedDataSize, uint8_t *pData, uint32_t *pSize);


/**
 * @brief  Decrypt a complete block of data using AES ECB.
 * @details The returned size is the exact size of the decrypted data
 *        where the padding has been removed.
 *
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_ECB_KEY_ID of AES_ECB_KEY_VALUE
 * @param pSize Pointer to the Size of the decrypted data
 *        after removal of the padding.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if the AEC ECB crypto engine cannot be initialized or if an error occurs during data decryption.
 */
FWU_ErrorCode_t FWUCrypto_AES_ECB_Decrypt(uint8_t *pEncryptedData,
    uint32_t encryptedDataSize, uint8_t *pData, uint8_t *pKey,
    AES_ECB_KeyType_t keyType, uint32_t *pSize);


/**
 * @brief  Initialize the AES CBC engine.
 *
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_CBC_KEY_ID of AES_CBC_KEY_VALUE
 * @param pIv Pointer to the Initialization Vector
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if the key is not valid.
 */
FWU_ErrorCode_t FWUCrypto_AES_CBC_Init(uint8_t *pKey, AES_CBC_KeyType_t keyType, uint8_t *pIv);


/**
 * @brief Decrypt an AES CBC encrypted data block.
 * @details  Decrypt block of data using AES CBC. The size corresponds to the
 *        encryptedDataSize as the padding is not removed and shall be done
 *        manually. This function is useful for large firmware that must be
 *        decrypted by block, where padding is removed to the last block.
 *
 *        FWUCrypto_AES_CBC_Init shall be called prior calling this function.
 *
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pSize Pointer to the size of the decrypted data
 *        after removal of the padding.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if a data block cannot be decrypted.
 */
FWU_ErrorCode_t FWUCrypto_AES_CBC_Decrypt_Block(uint8_t *pEncryptedData, uint32_t encryptedDataSize,
                                         uint8_t *pData, uint32_t *pSize);

/**
 * @brief  Decrypt block of data using AES CBC.
 *
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_CBC_KEY_ID of AES_CBC_KEY_VALUE
 * @param pIv Pointer to the Initialization Vector
 * @param pSize Pointer to the size of the decrypted data
 *        after removal of the padding.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if the AES CBC crypto engine cannot be initialized or if a data block cannot be decrypted.
 */
FWU_ErrorCode_t FWUCrypto_AES_CBC_Decrypt(uint8_t *pEncryptedData,
    uint32_t encryptedDataSize, uint8_t *pData, uint8_t *pKey,
    AES_CBC_KeyType_t keyType, uint8_t *pIv, uint32_t *pSize);


/**
 * @brief  Initialize the AES CTR engine.
 *
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_CTR_KEY_ID of AES_CTR_KEY_VALUE
 * @param pCounter Pointer to the Initialization Counter
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR in case of key error or if the AES CTR crypto engine cannot be initialized.
 */
FWU_ErrorCode_t FWUCrypto_AES_CTR_Init(uint8_t *pKey, AES_CTR_KeyType_t keyType, uint8_t *pCounter);


/**
 * @brief Decrypt an AES CTR encrypted data block.
 * @details  Decrypt block of data using AES CTR. The size corresponds to the
 *        encryptedDataSize as the padding is not removed and shall be done
 *        manually. This function is useful for large firmware that must be
 *        decrypted by block.
 *
 *        FWUCrypto_AES_CTR_Init shall be called prior calling this function.
 *
 *        AES CTR does not use padding. Size must match encryptedDataSize.
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pSize Pointer to the size of the decrypted data
 *        after removal of the padding.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR in case a data block cannot be decrypted.
 */
FWU_ErrorCode_t FWUCrypto_AES_CTR_Decrypt_Block(uint8_t *pEncryptedData, uint32_t encryptedDataSize,
                                         uint8_t *pData, uint32_t *pSize);

/**
 * @brief  Decrypt block of data using AES CTR.
 *
 * @param pEncryptedData Pointer to encrypted data.
 * @param encryptedDataSize Size of the encrypted data.
 * @param pData Pointer to the decrypted data.
 * @param pKey Pointer to the AES key or Key Container Index depending
 *        on the keyType value.
 * @param keyType Either AES_CTR_KEY_ID of AES_CTR_KEY_VALUE
 * @param pCounter Pointer to the Initialization Counter
 * @param pSize Pointer to the size of the decrypted data
 *        which is equal to encryptedDataSize since AES CTR does not
 *        use padding on initial data to encrypt.
 * @return FWU_ERROR_CODE_SUCCESS if successful or FWU_ERROR_CODE_AES_ERROR if the AES CTR crypto engine cannot be initialized or if a data block cannot be decrypted.
 */
FWU_ErrorCode_t FWUCrypto_AES_CTR_Decrypt(uint8_t *pEncryptedData,
    uint32_t encryptedDataSize, uint8_t *pData, uint8_t *pKey,
    AES_CTR_KeyType_t keyType, uint8_t *pCounter, uint32_t *pSize);

/** @} */ // End of group fwu_crypto
/** @} */ // End of addtogroup drivers

#endif // _FWU_CRYPTO_H
