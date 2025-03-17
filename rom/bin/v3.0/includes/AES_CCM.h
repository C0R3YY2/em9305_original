////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_CCM.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     AES CCM authenticated encryption according to NIST SP 800-38C
///            Underlying algorithm is AES-128,AES-192 or AES-256.
///            AES-128 option can be either hardware or software.
///            In case of hardware the key can be either in a key container or given explicitly
///
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2022 EM Microelectronic
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
#ifndef _AES_CCM_H
#define _AES_CCM_H

/**
* @defgroup AES_CCM AES_CCM
* @brief AES in CCM mode  according to NIST SP 800-38C
*
*/

//--------------------------------------------------------------------------------------------
// AES Key type
//--------------------------------------------------------------------------------------------
/**
 * @brief Select if the key is passed by value(explicitly) or if the key is contained in a key container
 * @ingroup AES_CCM
 */
typedef enum
{
    /// Key value is provided explicitly by value
    AES_CCM_KEY_VALUE=0,
    /// Key value is provided by its ID. The key is in a key container
    AES_CCM_KEY_ID=1
} AES_CCM_KeyType_t;

/**
 * @brief Error status words for AES CCM mode
 * @ingroup AES_CCM
 */
typedef enum
{
    /// AES CCM computation successful
        AES_CCM_SUCCESS=0,
        /// Incompatible key size with key type and AES type
        AES_CCM_INCOMPATIBLE_PARAMETER=0x11,
        /// Incorrect length- Data must be 16 bytes long
        AES_CCM_INCORRECT_LENGTH=0x12,
        /// Key length is incorrect.It shall be 128,192 or 256
        AES_CCM_INCORRECT_KEY_LENGTH=0x13,
        /// Result pointer is null
        AES_CCM_INCORRECT_RESULT_POINTER=0x14,
        /// Incorrect Nonce length. It shall be in [7..13] range
        AES_CCM_INCORRECT_NONCE_LENGTH=0x15,
        /// Incorrect MAC length. It shall be in [4..16] range
        AES_CCM_INCORRECT_MAC_LENGTH=0x16,
        /// LenP is not consistent with the nonce. q=number of significant byte in lenP is such that q+lenNonce=15
        AES_CCM_INCONSISTENT_NONCE_AND_P_LENGTHS=0x17

} AES_CCM_Lib_error_t;

//--------------------------------------------------------------------------------------------
// AES CCM context structure
//--------------------------------------------------------------------------------------------
/** AES CCM context
* \ingroup AES_CCM
*/
typedef struct
{
    AES_key_size_bit_t              keySize;        /**< Key size. */
    AES_Select_t                    AESType;        /**< AES Type: Either AES_HARDWARE or AES_SOFTWARE. */
    AES_CCM_KeyType_t               keyType;        /**< Key type. Either KEY_ID or KEY_VALUE*/
    uint8_t                         key[32];        /**< The key value or the key index*/
    uint8_t                         S0[16];         /**< S0 value to finalize the MAC*/
    uint8_t                         counter[16];    /**< Current counter*/              //TO DO
    uint8_t                         buffer[16];     /**< Intermediate buffer for encryption/decryption*/
    uint8_t                         MAC[16];        /**< MAC value*/
    uint8_t                         lenMAC;         /**< Length of the desired MAC*/
    uint8_t                         lenNonce;       /**< Length of the Nonce*/
    uint8_t                         nbByteReady;    /**< Indicate how many bytes in the buffer are available to encrypt/decrypt the message before executing a new AES*/
    uint8_t                         nbMacReady;     /**< Indicate how many bytes in the MAC buffer are accumulated*/
    uint8_t                         AdditionalOver; /**< Indicate if the treatment of additional data is over*/

}AES_CCM_CTX;


//--------------------------------------------------------------------------------------------
// AES CCM APIs
//--------------------------------------------------------------------------------------------


/**
* \fn AES_CCM_Lib_error_t AES_CCM_InitCtx(AES_CCM_CTX * ctx,AES_Select_t AESSelect, AES_CCM_KeyType_t keyType, AES_key_size_bit_t keySize,uint8_t * key,
        uint8_t lenMAC, uint8_t lenNonce, uint8_t * nonce, uint8_t totalLenP[8],uint8_t totalLenA[8]);
* \brief Initialize an AES CCM context. Choose the underlying AES, the key type and key size. Set the key value.
* Set the nonce. Set the MAC length, the total length of the message to encrypt or decrypt. Set the total length of the additional data.
* \param[out] ctx AES CCM context to set
* \param[in] AESSelect The underlying AES. Either AES_HARDWARE or AES_SOFTWARE
* \param[in] keyType The type of the key: Either KEY_VALUE for explicitly key value or KEY_ID for a key in a key container
* \param[in] keySize The size of the key in bits : AES_KEY_128,AES_KEY_192 or AES_KEY_256
* \param[in] key The key value(16 to 32 bytes) or the key ID (first  byte)
* \param[in] lenMAC Length in bytes of the MAC- It must be even and in range [4..16]
* \param[in] lenNonce Length in bytes of the nonce. It must be in range [7..13]
* \param[in] nonce The nonce value,a buffer of length lenNonce
* \param[in] totalLenP A 8-byte buffer indicating the total length of the plaintext or ciphertext to encrypt or decrypt. If q is the length of the significant size of totalLenP, q+lenNonce must be smaller or equal to 15.
* \param[in] totalLenA A 8-byte buffer indicating the total length of the additional data
* \return Error code
*
* \retval #AES_CCM_SUCCESS  Successful initialization
* \retval #AES_CCM_INCORRECT_KEY_LENGTH Incorrect key length. It must be 128,192 or 256
* \retval #AES_CCM_INCOMPATIBLE_PARAMETER Hardware/software selection is not compatible with the key length. Key containers are only support with 128-bit keys.
* \retval #AES_CCM_INCORRECT_MAC_LENGTH Incorrect MAC length. It must be even and in range [4..16]
* \retval #AES_CCM_INCORRECT_NONCE_LENGTH Incorrect nonce length. It must be in range [7..13]
* \retval #AES_CCM_INCONSISTENT_NONCE_AND_P_LENGTHS Nonce length and totalLenP are not compatible. Their combined length must be smaller or equal to 15.
* \ingroup AES_CCM
*/
extern AES_CCM_Lib_error_t AES_CCM_InitCtx(AES_CCM_CTX * ctx,AES_Select_t AESSelect, AES_CCM_KeyType_t keyType, AES_key_size_bit_t keySize,uint8_t * key,
        uint8_t lenMAC, uint8_t lenNonce, uint8_t * nonce, uint8_t totalLenP[8],uint8_t totalLenA[8]);


/**
* \fn  AES_CCM_Lib_error_t AES_CCM_Hash_Additional_Data(AES_CCM_CTX * ctx,uint8_t *lenA,uint8_t * A)
* \brief Deal with the additional data. Additional data are MACed but not encrypted,
* \param[in,out] ctx AES CCM context to set
* \param[in] lenA The length in bytes of the additional data dealt during this function call.
* \param[in] A Byte buffer of lenA that contains the additional data
* \return Error code
*
* \retval #AES_CCM_SUCCESS  Additional data successfully dealt
* \ingroup AES_CCM
*/
extern AES_CCM_Lib_error_t AES_CCM_Hash_Additional_Data(AES_CCM_CTX * ctx,uint8_t *lenA,uint8_t * A);



/**
* \fn AES_CCM_Lib_error_t AES_CCM_EncryptAndMAC(AES_CCM_CTX * ctx, uint8_t lenP[8], uint8_t * plainText, uint8_t *cipherText);
* \brief Encrpyt the plaintext and update the MAC.
* \param[in,out] ctx AES CCM context to set
* \param[in] lenP The length in bytes of the data to encrypt during this function call.
* \param[in] plainText Byte buffer of lenP that contains the data to encrypt
* \param[out] cipherText Byte buffer of lenP that contains the encrypted data
* \return Error code
*
* \retval #AES_CCM_SUCCESS  Successful encryption
* \ingroup AES_CCM
*/
extern AES_CCM_Lib_error_t AES_CCM_EncryptAndMAC(AES_CCM_CTX * ctx, uint8_t lenP[8], uint8_t * plainText, uint8_t *cipherText);

/**
* \fn AES_CCM_Lib_error_t AES_CCM_DecryptAndMAC(AES_CCM_CTX * ctx, uint8_t lenC[8], uint8_t *cipherText,uint8_t * plainText);
* \brief Decrpyt the cipherText and update the MAC.
* \param[in,out] ctx AES CCM context to set
* \param[in] lenC The length in bytes of the data to decrypt during this function call.
* \param[in] cipherText Byte buffer of lenP that contains the data to decrypt
* \param[out] plainText Byte buffer of lenP that contains the decrypted data
* \return Error code
*
* \retval #AES_CCM_SUCCESS  Successful encryption
* \ingroup AES_CCM
*/
extern AES_CCM_Lib_error_t AES_CCM_DecryptAndMAC(AES_CCM_CTX * ctx, uint8_t lenC[8], uint8_t *cipherText,uint8_t * plainText);


/**
* \fn AES_CCM_Lib_error_t AES_CCM_GetMAC(AES_CCM_CTX * ctx,uint8_t *MAC);
* \brief Finalize the MAC computation
* \param[in,out] ctx AES CCM context to set
* \param[out] MAC A buffer of the size lenMAC defined previously in the init function.
* \return Error code
*
* \retval #AES_CCM_SUCCESS  Successful encryption
* \retval #AES_CCM_INCORRECT_RESULT_POINTER MAC pointer not initialized
* \ingroup AES_CCM
*/
extern AES_CCM_Lib_error_t AES_CCM_GetMAC(AES_CCM_CTX * ctx,uint8_t *MAC);


#endif /* _AES_CCM_H */
