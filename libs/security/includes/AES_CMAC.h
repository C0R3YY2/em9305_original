////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_CMAC.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     AES CMAC according to NIST SP 800-90B
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
#ifndef _AES_CMAC_H
#define _AES_CMAC_H

/**
* @defgroup AES_CMAC AES_CMAC
* @brief AES in CMAC mode  according to NIST SP 800-38B
*
*/

//--------------------------------------------------------------------------------------------
// AES Key type
//--------------------------------------------------------------------------------------------
/**
 * @brief Select if the key is passed by value(explicitly) or if the key is contained in a key container
 * @ingroup AES_CMAC
 */
typedef enum
{
    /// Key value is provided explicitly by value
    AES_CMAC_KEY_VALUE=0,
    /// Key value is provided by its ID. The key is in a key container
    AES_CMAC_KEY_ID=1
} AES_CMAC_KeyType_t;

/**
 * @brief Error status words for AES CMAC mode
 * @ingroup AES_CMAC
 */
typedef enum
{
        /// AES CMAC computation successful
        AES_CMAC_SUCCESS=0,
        /// Incompatible key size with key type and AES type
        AES_CMAC_INCOMPATIBLE_PARAMETER=0x11,
        /// Incorrect length- MAC length should be in [0..16]
        AES_CMAC_INCORRECT_LENGTH=0x12,
        /// Key length is incorrect.It shall be 128,192 or 256
        AES_CMAC_INCORRECT_KEY_LENGTH=0x13,
        /// Result pointer is null
        AES_CMAC_INCORRECT_RESULT_POINTER=0x14,
} AES_CMAC_Lib_error_t;

//--------------------------------------------------------------------------------------------
// AES CMAC context structure
//--------------------------------------------------------------------------------------------
/** AES CMAC context
* \ingroup AES_CMAC
*/
typedef struct
{
    AES_key_size_bit_t              keySize;        /**< Key size. */
    AES_Select_t                    AESType;        /**< AES Type: Either AES_HARDWARE or AES_SOFTWARE. */
    AES_CMAC_KeyType_t              keyType;        /**< Key type. Either KEY_ID or KEY_VALUE*/
    uint8_t                         key[32];        /**< The key value or the key index*/
    uint8_t                         buffer[16];     /**< Buffer to accumulate data*/
    uint8_t                         nbByteInBuffer; /**< Number of bytes in the accumulation buffer*/
    uint8_t                         isNullMessage;  /**< Special tag to indicate if the message is null*/
}AES_CMAC_CTX;

//--------------------------------------------------------------------------------------------
// AES CMAC APIs
//--------------------------------------------------------------------------------------------
/**
 * \fn AES_CMAC_Lib_error_t AES_CMAC_InitCtx(AES_CMAC_CTX * ctx,AES_Select_t AESSelect, AES_CMAC_KeyType_t keyType, AES_key_size_bit_t keySize, uint8_t * key);
 * \brief Initialize an AES CMAC context. Choose the underlying AES, the key type and key size. Set the key value. Set the initial value.
 * \param[out] ctx AES CMAC context to set
 * \param[in] AESSelect The underlying AES. Either AES_HARDWARE or AES_SOFTWARE
 * \param[in] keyType The type of the key: either KEY_VALUE for explicitly key value or KEY_ID for a key in a key container
 * \param[in] keySize The size of the key in bits : AES_KEY_128,AES_KEY_192 or AES_KEY_256
 * \param[in] key The key value(16 to 32 bytes) or the key ID (first  byte)
 * \note When using key container, the key must be typed either AES_HW_ENC_DEC or AES_HW_ENC_ONLY. This is due to the fact, that AES always encrypts the blocks to compute a CMAC.
 *
 * \return Error code
 *
 * \retval #AES_CMAC_SUCCESS    No error occurred
 * \retval #AES_CMAC_INCORRECT_KEY_LENGTH Incorrect key length
 * \retval #AES_CMAC_INCOMPATIBLE_PARAMETER Hardware/software selection is not compatible with the key length
 *
 * \ingroup AES_CMAC
 */
extern AES_CMAC_Lib_error_t AES_CMAC_InitCtx(AES_CMAC_CTX * ctx,AES_Select_t AESSelect, AES_CMAC_KeyType_t keyType, AES_key_size_bit_t keySize, uint8_t * key);


/**
 * \fn AES_CMAC_Lib_error_t AES_CMAC_Compute(AES_CMAC_CTX * ctx, uint8_t * data, uint32_t sizeInBytes);
 * \brief Accumulate data for the MAC computation and compute intermediate MAC
 * \param[in,out] ctx AES CMAC context
 * \param[in] data Pointer on data to MAC. It should be sizeInBytes long
 * \param[in] sizeInBytes Size of the data. It can be any size.
 *
 * \return Error code
 *
 * \retval #AES_CMAC_SUCCESS    No error occurred
 * \retval #AES_CMAC_INCORRECT_KEY_LENGTH Incorrect key length
 * \retval #AES_CMAC_INCOMPATIBLE_PARAMETER Hardware/software selection is not compatible with the key length
 *
 * \ingroup AES_CMAC
 */
extern AES_CMAC_Lib_error_t AES_CMAC_Compute(AES_CMAC_CTX * ctx, uint8_t * data, uint32_t sizeInBytes);


/**
 * \fn AES_CMAC_Lib_error_t AES_CMAC_GetMAC(AES_CMAC_CTX * ctx, uint8_t * MAC, uint8_t sizeMACInByte)
 * \brief return the sizeMACInByte MAC
 * \param[in,out] ctx AES CMAC context
 * \param[in] sizeMACInByte MAC size required . It should be in [0..16]
 * \param[out] MAC MAC value( sizeMACInByte byte long)
 *
 * \return Error code
 *
 * \retval #AES_CMAC_SUCCESS    No error occurred
 * \retval #AES_CMAC_INCORRECT_RESULT_POINTER   Result pointer not initialized.
 * \retval #AES_CMAC_INCORRECT_LENGTH MAC length should be lower or equal to 16.
 *
 * \ingroup AES_CMAC
 */
extern AES_CMAC_Lib_error_t AES_CMAC_GetMAC(AES_CMAC_CTX * ctx, uint8_t * MAC, uint8_t sizeMACInByte);

#endif /* _AES_CMAC_H */
