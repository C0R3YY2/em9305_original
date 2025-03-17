////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_GCM.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     AES GCM authenticated encryption according to NIST SP 800-90D
///            AES GMAC according to NIST SP 800-90D
///            Underlying algorithm is AES-128,AES-192 or AES-256.
///            AES-128 option can be either hardware or software.
///            In case of hardware the key can be either in a key container or given explicitly
//
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
#ifndef _AES_GCM_H
#define _AES_GCM_H

/**
* @defgroup AES_GCM AES_GCM
* @brief AES in GCM mode  according to NIST SP 800-38D
*
*/

//--------------------------------------------------------------------------------------------
// AES Key type
//--------------------------------------------------------------------------------------------
/**
 * @brief Select if the key is passed by value(explicitly) or if the key is contained in a key container
 * @ingroup AES_GCM
 */
typedef enum
{
    /// Key value is provided explicitly by value
    AES_GCM_KEY_VALUE=0,
    /// Key value is provided by its ID. The key is in a key container
    AES_GCM_KEY_ID=1
} AES_GCM_KeyType_t;

/**
 * @brief Error status words for AES GCM mode
 * @ingroup AES_GCM
 */
typedef enum
{
    /// AES GCM computation successful
        AES_GCM_SUCCESS=0,
        /// Incompatible key size with key type and AES type
        AES_GCM_INCOMPATIBLE_PARAMETER=0x11,
        /// Incorrect length- Data must be 16 bytes long
        AES_GCM_INCORRECT_LENGTH=0x12,
        /// Key length is incorrect.It shall be 128,192 or 256
        AES_GCM_INCORRECT_KEY_LENGTH=0x13,
        /// Result pointer is null
        AES_GCM_INCORRECT_RESULT_POINTER=0x14,
} AES_GCM_Lib_error_t;

//--------------------------------------------------------------------------------------------
// AES GCM context structure
//--------------------------------------------------------------------------------------------
/** AES GCM context
* \ingroup AES_GCM
*/
typedef struct
{
    AES_key_size_bit_t              keySize;        /**< Key size. */
    AES_Select_t                    AESType;        /**< AES Type: Either AES_HARDWARE or AES_SOFTWARE. */
    AES_GCM_KeyType_t               keyType;        /**< Key type. Either KEY_ID or KEY_VALUE*/
    uint8_t                         key[32];        /**< The key value or the key index*/
    uint8_t                         H[16];          /**< The Hash subkey*/
    uint8_t                         S[16];          /**< Current MAC value*/
    uint8_t                         J0[16];         /**< J0 value*/
    uint8_t                         counter[16];    /**< Current counter*/
    uint8_t                         lengthA[8];     /**< Accumulated length of Additional data*/
    uint8_t                         lengthP[8];     /**< Accumulated length of plaintext or ciphertext*/
    uint8_t                         buffer[16];     /**< Intermediate buffer for encryption/decryption*/
    uint8_t                         nbByteReady;    /**< Indicate how many bytes in the buffer are available to encrypt/decrypt the message before executing a new AES*/
    uint8_t                         GMAC;           /**< Indicate if the computation is a GMAC or not*/

}AES_GCM_CTX;


//--------------------------------------------------------------------------------------------
// AES GCM APIs
//--------------------------------------------------------------------------------------------
/**
* \fn extern AES_GCM_Lib_error_t AES_GCM_InitCtx(AES_GCM_CTX * ctx,AES_Select_t AESSelect, AES_GCM_KeyType_t keyType, AES_key_size_bit_t keySize,uint8_t * key, uint8_t lenIV[8],uint8_t * IV);
* \brief Initialize an AES GCM context. Choose the underlying AES, the key type and key size. Set the key value. Set the initial value.
* \param[out] ctx AES GCM context to set
* \param[in] AESSelect The underlying AES. Either AES_HARDWARE or AES_SOFTWARE
* \param[in] keyType The type of the key: Either KEY_VALUE for explicitly key value or KEY_ID for a key in a key container
* \param[in] keySize The size of the key in bits : AES_KEY_128,AES_KEY_192 or AES_KEY_256
* \param[in] key The key value(16 to 32 bytes) or the key ID (first  byte)
* \param[in] lenIV 8-byte array representing the length in bytes of the IV. E.g uint8_t lenIV[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C} indicates a 12 bytes IV.
* \param[in] IV Buffer of lenIV byte with the initial value
*
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
* \retval #AES_GCM_INCORRECT_KEY_LENGTH Incorrect key length
* \retval #AES_GCM_INCOMPATIBLE_PARAMETER Hardware/software selection is not compatible with the key length
* \ingroup AES_GCM
*/
extern AES_GCM_Lib_error_t AES_GCM_InitCtx(AES_GCM_CTX * ctx,AES_Select_t AESSelect, AES_GCM_KeyType_t keyType, AES_key_size_bit_t keySize,uint8_t * Key, uint8_t lenIV[8],uint8_t * IV);


/**
* \fn AES_GCM_Lib_error_t AES_GCM_Hash_Additional_Data(AES_GCM_CTX * ctx, uint8_t lenA[8],uint8_t * A);
* \brief Hash the additional data that precedes the data to encrypt/decrypt
* \param[in,out] ctx AES GCM
* \param[in] lenA 8-byte array representing the length in bytes of the additional data provided at this call. E.g uint8_t lenIV[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00} indicates a 256-byte additional data
* \param[in] A Buffer of lenA byte with the additional data
* \parblock
* \note This function can be called several times consecutively with different parts of the additional data. LenA does not represent the total length of the data. It represents the length of the data provided for this call.
* \endparblock
* \parblock
* \note This function can be skipped in case there is no additional data.
* \endparblock
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
*
* \ingroup AES_GCM
*/
extern AES_GCM_Lib_error_t AES_GCM_Hash_Additional_Data(AES_GCM_CTX * ctx, uint8_t lenA[8],uint8_t * A);


/**
* \fn AES_GCM_Lib_error_t AES_GCM_EncryptAndMAC(AES_GCM_CTX * ctx, uint8_t lenP[8], uint8_t * plainText, uint8_t *cipherText);
* \brief Encrpyt the provided plaintext and update the MAC
* \param[in,out] ctx AES GCM
* \param[in] lenP 8-byte array representing the length in bytes of the plaintext data provided at this call. E.g uint8_t lenP[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00} indicates a 512-byte plaintext.
* \param[in] plainText Buffer of lenP byte with the data to encrypt
* \param[out] cipherText Buffer of lenP byte with the encrypted data
* \note This function can be called several times consecutively with different parts of the plaintext. LenP does not represent the total length of the data. It represents the length of the data provided for this call.
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
*
* \ingroup AES_GCM
*/
extern AES_GCM_Lib_error_t AES_GCM_EncryptAndMAC(AES_GCM_CTX * ctx, uint8_t lenP[8], uint8_t * plainText, uint8_t *cipherText);


/**
* \fn AES_GCM_Lib_error_t AES_GCM_DecryptAndMAC(AES_GCM_CTX * ctx, uint8_t lenC[8], uint8_t *cipherText,uint8_t * plainText);
* \brief Decrypt the provided ciphertext and update the MAC
* \param[in,out] ctx AES GCM
* \param[in] lenC 8-byte array representing the length in bytes of the cipherText data provided at this call. E.g uint8_t lenC[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00} indicates a 512-byte plaintext.
* \param[in] cipherText Buffer of lenC byte with the data to decrypt
* \param[out] plainText Buffer of lenC byte with the decrypted data
* \note This function can be called several times consecutively with different parts of the ciphertext. LenC does not represent the total length of the data. It represents the length of the data provided for this call.
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
* \retval #AES_GCM_INCORRECT_RESULT_POINTER Result pointer not initialized
* \retval #AES_GCM_INCORRECT_LENGTH The length of the message should be snaller than 2^39 -256 = 000000 7F FF FF FF 00
*
* \ingroup AES_GCM
*/


extern AES_GCM_Lib_error_t AES_GCM_DecryptAndMAC(AES_GCM_CTX * ctx, uint8_t lenC[8], uint8_t *cipherText,uint8_t * plainText);
/**
* \fn AES_GCM_Lib_error_t AES_GCM_GMAC(AES_GCM_CTX * ctx, uint8_t lenD[8],uint8_t * data)
* \brief Perform a GMAC computation ( GCM mode with no encryption)
* \param[in,out] ctx AES GCM
* \param[in] lenD 8-byte array representing the length in bytes of the data provided at this call. E.g uint8_t lenD[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00} indicates a 512-byte plaintext.
* \param[in] data Buffer of lenD byte with the data to MAC
* \note This function can be called several times consecutively with different parts of the additional data. LenD does not represent the total length of the data. It represents the length of the data provided for this call.
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
* \retval #AES_GCM_INCORRECT_RESULT_POINTER Result pointer not initialized
* \retval #AES_GCM_INCORRECT_LENGTH The length of the message should be smaller than 2^39 -256 = 000000 7F FF FF FF 00
*
* \ingroup AES_GCM
*/
extern AES_GCM_Lib_error_t AES_GCM_GMAC(AES_GCM_CTX * ctx, uint8_t lenD[8],uint8_t * data);


/**
* \fn AES_GCM_Lib_error_t AES_GCM_GetMAC(AES_GCM_CTX * ctx, uint8_t lenMAC, uint8_t *MAC);
* \brief Finalize the MAC computation once all the data have been provided.
* \param[in,out] ctx AES GCM
* \param[in] lenMAC Length in bytes of the MAC. It must be a value in range [4..16].
* \param[out] MAC Buffer to receive the MAC. It must be at least of length LenMAC.
*
* \return Error code
*
* \retval #AES_GCM_SUCCESS  No error occurred
* \retval #AES_GCM_INCORRECT_LENGTH The MAC length shall be in range [4..16].
* \retval #AES_GCM_INCORRECT_RESULT_POINTER MAC pointer not initialized
*
* \ingroup AES_GCM
*/
extern AES_GCM_Lib_error_t AES_GCM_GetMAC(AES_GCM_CTX * Ctx, uint8_t lenMAC, uint8_t *MAC);


#endif /* _AES_GCM_H */
