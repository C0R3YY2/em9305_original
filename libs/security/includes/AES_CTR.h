////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_CTR.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     AES Counter mode encryption and decryption according to NIST SP 800-90A
///            Underlying algorithm is AES-128,AES-192 or AES-256.
///            AES-128 option can be either hardware or software.
///            In case of hardware the key can be either in a key container or given explicitly
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2020 EM Microelectronic
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


#ifndef _AES_CTR_H
#define _AES_CTR_H

/**
* @defgroup AES_CTR AES_CTR
* @brief AES in Counter mode according to NIST SP 800-38A
*
*/


//--------------------------------------------------------------------------------------------
// AES Key type
//--------------------------------------------------------------------------------------------
/**
 * @brief Select if the key is passed by value(explicitly) or if the key is contained in a key container
 * @ingroup AES_CTR
 */
typedef enum
{
    /// Key value is provided explicitly by value
    AES_CTR_KEY_VALUE=0,
    /// Key value is provied by its ID. The key is in a key container
    AES_CTR_KEY_ID=1
} AES_CTR_KeyType_t;

/**
 * @brief Error status words for AES CTR mode
 * @ingroup AES_CTR
 */
typedef enum
{
    /// AES CTR computation successful
        AES_CTR_SUCCESS=0,
        /// Incompatible key size with key type and AES type
        AES_CTR_INCOMPATIBLE_PARAMETER=0x11,
        /// Incorrect length- Data must be 16 bytes long
        AES_CTR_INCORRECT_LENGTH=0x12,
        /// Key length is incorrect.It shall be 128,192 or 256
        AES_CTR_INCORRECT_KEY_LENGTH=0x13,
        /// Result pointer is null
        AES_CTR_INCORRECT_RESULT_POINTER=0x14,


} AES_CTR_Lib_error_t;

//--------------------------------------------------------------------------------------------
// AES CTR context structure
//--------------------------------------------------------------------------------------------
/** AES CTR context
* \ingroup AES_CTR
*/
typedef struct
{
    AES_key_size_bit_t              keySize;        /**< Key size. */
    AES_Select_t                    AESType;        /**< AES Type: Either AES_HARDWARE or AES_SOFTWARE. */
    AES_CTR_KeyType_t               keyType;        /**< Key type. Either KEY_ID or KEY_VALUE*/
    uint8_t                         key[32];        /**< The key value or the key index*/
    uint8_t                         counter[16];    /**< Current Counter Value*/
    uint8_t                         buffer[16];     /**< Current AES result-Accumulation buffer*/
    uint8_t                         nbByteReady;    /**< Indicate how many bytes in the buffer are available to encrypt/decrypt the message before executing a new AES*/
}AES_CTR_CTX;


//--------------------------------------------------------------------------------------------
// AES CTR APIs
//--------------------------------------------------------------------------------------------
/**
 * \fn AES_CTR_Lib_error_t AES_CTR_InitCtx(AES_CTR_CTX * ctx,AES_Select_t AESSelect, AES_CTR_KeyType_t keyType, AES_key_size_bit_t keySize, uint8_t *initialCounter, uint8_t * key);
 * \brief Initialize an AES CTR context. Choose the underlying AES, the key type and key size. Set the key value. Set the initial value.
 * \param[out] ctx AES CTR context to set
 * \param[in] AESSelect The underlying AES. Either AES_HARDWARE or AES_SOFTWARE
 * \param[in] keyType The type of the key: Either KEY_VALUE for explicitly key value or KEY_ID for a key in a key container
 * \param[in] keySize The size of the key in bits : AES_KEY_128,AES_KEY_192 or AES_KEY_256
 * \param[in] initialCounter The 16-byte initial counter value
 * \param[in] key The key value(16 to 32 bytes) or the key ID (first  byte)
 *
 * \return Error code
 *
 * \retval #AES_CTR_SUCCESS No error occurred
 * \retval #AES_CTR_INCORRECT_KEY_LENGTH Incorrect key length
 * \retval #AES_CTR_INCOMPATIBLE_PARAMETER Hardware/software selection is not compatible with the key length
 *
 * \ingroup AES_CTR
 */
extern AES_CTR_Lib_error_t AES_CTR_InitCtx(AES_CTR_CTX * ctx,AES_Select_t AESSelect, AES_CTR_KeyType_t keyType, AES_key_size_bit_t keySize, uint8_t *initialCounter, uint8_t * key);


/**
 * \fn AES_CTR_Lib_error_t AES_CTR_Encrypt(AES_CTR_CTX * ctx, uint8_t * plainText, uint8_t *cipherText, uint32_t sizeInBytes);
 * \brief Encrypt data in CTR mode.
 * \param[in,out] ctx AES CTR context
 * \param[in] plainText Pointer on data to encrypt. It should be sizeInBytes long
 * \param[out] cipherText Pointer on the result. It should be sizeInBytes long
 * \param[in] sizeInBytes Size of the plaintext=Size of the cipher text in bytes. It can be any size
 *
 * \return Error code
 *
 * \retval #AES_CTR_SUCCESS No error occurred
 * \retval #AES_CTR_INCORRECT_RESULT_POINTER Result pointer not initialized
 * \ingroup AES_CTR
 */
extern AES_CTR_Lib_error_t AES_CTR_Encrypt(AES_CTR_CTX * ctx, uint8_t * plainText, uint8_t *cipherText, uint32_t sizeInBytes);

/**
 * \fn AES_CTR_Lib_error_t AES_CTR_Decrypt(AES_CTR_CTX * ctx, uint8_t *cipherText, uint8_t * plainText,uint32_t sizeInBytes);
 * \brief Decrypt data in CTR mode.
 * \param[in,out] ctx AES CTR context
 * \param[in] cipherText Pointer on data to decrypt. It should be sizeInBytes long
 * \param[out] plainText Pointer on the result. It should be sizeInBytes long
 * \param[in] sizeInBytes Size of the cipherText=Size of the cipher text in bytes. It can be any size.
 *
 * \retval #AES_CTR_SUCCESS No error occurred
 * \retval #AES_CTR_INCORRECT_RESULT_POINTER Result pointer not initialized
 *
 * \note When using key container, the key must be typed either AES_HW_ENC_DEC or AES_HW_ENC_ONLY. This is due to the fact that, in CTR mode, the underlying AES performs an encryption even to decrypt.
 * \ingroup AES_CTR
 */
extern AES_CTR_Lib_error_t AES_CTR_Decrypt(AES_CTR_CTX * ctx, uint8_t *cipherText, uint8_t * plainText,uint32_t sizeInBytes);
#endif /* _AES_CTR_H */
