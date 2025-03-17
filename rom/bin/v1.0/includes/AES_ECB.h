////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_ECB.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     AES in ECB mode.Underlying algorithm is AES-128 , with or without key container, AES-192 or AES-256
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


#ifndef _AES_ECB_H
#define _AES_ECB_H

/**
* @defgroup AES_ECB AES_ECB
* @brief AES in ECB mode
*
*/

//--------------------------------------------------------------------------------------------
// AES ECB context structure
//--------------------------------------------------------------------------------------------
/** AES ECB context
* \ingroup AES_ECB
*/
typedef struct
{
    AES_key_size_bit_t              KeySize;        /**< Key size. */
    AES_Select_t                    AESType;        /**< AES Type: Either AES_HARDWARE or AES_SOFTWARE. */
    uint8_t                         KeyType;        /**< Key type. Either KEY_ID or KEY_VALUE*/
    uint8_t                         Key[32];        /**< The key value or the key index*/
}AES_ECB_CTX;


//--------------------------------------------------------------------------------------------
// AES Key type
//--------------------------------------------------------------------------------------------
/**
 * @brief Select if the key is passed by value(explicitly) or the key is contained is a key container
 * @ingroup AES_ECB
 */
typedef enum
{
    /// Key value is provided explicitly by value
    KEY_VALUE=0,
    /// Key value is provied by its ID. The key is in a key container
    KEY_ID=1
} AES_ECB_KeyType_t;

/**
 * @brief Error status words for AES ECB mode
 * @ingroup AES_ECB
 */
typedef enum
{
    /// AES ECB computation successful
        AES_ECB_SUCCESS=0,
        /// Incompatible key size with key type and AES type
        AES_ECB_INCOMPATIBLE_PARAMETER=0x11,
        /// Incorrect length- Data must be 16 bytes long
        AES_ECB_INCORRECT_LENGTH=0x12,
        /// Key length is incorrect.It shall be 128,192 or 256
        AES_ECB_INCORRECT_KEY_LENGTH=0x13,
        /// Result pointer is null
        AES_ECB_INCORRECT_RESULT_POINTER=0x14,


} AES_ECB_Lib_error_t;


//--------------------------------------------------------------------------------------------
// AES ECB APIs
//--------------------------------------------------------------------------------------------
/**
 * \fn AES_ECB_Lib_error_t AES_ECB_InitCtx(AES_ECB_CTX * Ctx,AES_Select_t AESSelect, uint8_t KeyType, AES_key_size_bit_t KeySize, uint8_t * key)
 * \brief Initialize an AES ECB context. Choose the underlying AES, the key type and key size. Set the key value
 * \param[out] Ctx AES ECB context to set
 * \param[in] AESSelect The underlying AES. Either AES_HARDWARE or AES_SOFTWARE
 * \param[in] KeyType The type of the key: Either KEY_VALUE for explicitely key value or KEY_ID for a key in a key container
 * \param[in] KeySize The size of the key in bits : AES_KEY_128,AES_KEY_192 or AES_KEY_256
 * \param[in] key The key value(16 to 32 bytes) or the key ID (first  byte)
 *
 * \ingroup AES_ECB
 */
extern AES_ECB_Lib_error_t AES_ECB_InitCtx(AES_ECB_CTX * Ctx,AES_Select_t AESSelect, uint8_t KeyType, AES_key_size_bit_t KeySize, uint8_t * key);


/**
 * \fn AES_ECB_Lib_error_t AES_ECB_Encrypt(AES_ECB_CTX * Ctx, uint8_t * plainText, uint8_t *cipherText, uint32_t sizeInBytes);
 * \brief Encrypt data in ECB mode.
 * \param[in] Ctx AES ECB context
 * \param[in] plainText Pointer on data to encrypt. It should be sizeInBytes long
 * \param[out] cipherText Pointer on the result. It should be sizeInBytes long
 * \param[in] sizeInBytes Size of the plaintext=Size of the cipher text in bytes. It must be a multiple of 16.
 *
 * \ingroup AES_ECB
 */
extern AES_ECB_Lib_error_t AES_ECB_Encrypt(AES_ECB_CTX * Ctx, uint8_t * plainText, uint8_t *cipherText, uint32_t sizeInBytes);

/**
 * \fn AES_ECB_Lib_error_t AES_ECB_Decrypt(AES_ECB_CTX * Ctx, uint8_t *cipherText, uint8_t * plainText,uint32_t sizeInBytes);
 * \brief Decrypt data in ECB mode.
 * \param[in] Ctx AES ECB context
 * \param[in] cipherText Pointer on data to decrypt. It should be sizeInBytes long
 * \param[out] plainText Pointer on the result. It should be sizeInBytes long
 * \param[in] sizeInBytes Size of the cipherText=Size of the cipher text in bytes. It must be a multiple of 16.
 *
 * \ingroup AES_ECB
 */
extern AES_ECB_Lib_error_t AES_ECB_Decrypt(AES_ECB_CTX * Ctx, uint8_t *cipherText, uint8_t * plainText,uint32_t sizeInBytes);
#endif /* _AES_H */
