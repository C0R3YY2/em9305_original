////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief      AES 128, 192 and 256 , both in encryption and decryption modes
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


#ifndef _AES_H
#define _AES_H

/**
* @defgroup AES AES
* @brief AES algorithm according to FIPS 197
*
*/
//--------------------------------------------------------------------------------------------
// AES HW or SW selector
//--------------------------------------------------------------------------------------------
/**
 * @brief Select the AES between AES hardware(128 bit only) and the AES software(all key sizes). This module only concerns the encryption or decryption of a 16-byte block.
 * See documentation of the AES container for implicit keys
 * @ingroup AES
 */
typedef enum
{
    ///AES HARDWARE (only valid for 128-bit keys)
    AES_HARDWARE=0,
    ///AES SOFTWARE (valid for all key sizes)
    AES_SOFTWARE=1
} AES_Select_t;

//--------------------------------------------------------------------------------------------
// AES Mode encryption or decryption
//--------------------------------------------------------------------------------------------
/**
 * @brief Select the mode of operation of AES
 * @ingroup AES
 */
typedef enum
{
    /// AES encryption mode
    AES_ENCRYPTION=0,
    /// AES decryption mode
    AES_DECRYPTION=1
} AES_OpMode_t;

//--------------------------------------------------------------------------------------------
// AES Key Lengths
//--------------------------------------------------------------------------------------------
/**
 * @brief Select the AES key size in bits
 * @ingroup AES
 */
typedef enum
{
    /// 128-bit key size
    AES_KEY_128=128,
    /// 192-bit key size
    AES_KEY_192=192,
    /// 256-bit key size
    AES_KEY_256=256,

} AES_key_size_bit_t;

/**
 * @brief AES key sizes in byte
 * @ingroup AES
 */
typedef enum
{
    ///128-bit key size(16 bytes)
    AES_BYTE_SIZE_KEY128=16,
    ///192-bit key size(24 bytes)
    AES_BYTE_SIZE_KEY192=24,
    ///256-bit key size(32 bytes)
    AES_BYTE_SIZE_KEY256=32,

} AES_key_size_byte_t;

/**
 * @brief Error status words for AES
 * @ingroup AES
 */
typedef enum
{
    /// AES computation successful
    AES_SUCCESS=0,
    /// AES Hardware only supports the key size 128-bit
    AES_KEY_SIZE_NOT_SUPPORTED_WITH_HARDWARE=1,
    /// An error occur in the AES hardware
    AES_HW_ERROR=2,
    ///Result pointer is null
    AES_INCORRECT_RESULT_POINTER=3,
    ///AES type Hardware or software is not correct
    AES_INCORRECT_AES_TYPE=4,
    /// AES key lentgh is not correct
    AES_INCORRECT_KEY_LENGTH=5,
    /// AES mode is not correct
    AES_INCORRECT_MODE=6
} AES_Lib_error_t;
//--------------------------------------------------------------------------------------------
// AES Block size
//--------------------------------------------------------------------------------------------
/**
 * @brief Size of an AES block in byte
 * @ingroup AES
 */
#define AES_BLOCK_SIZE_BYTE 16

//--------------------------------------------------------------------------------------------
// AES API
//--------------------------------------------------------------------------------------------
/**
 * \fn AES_Lib_error_t AES(AES_Select_t AESSelect, uint8_t * Key , uint8_t*Message, uint8_t*Result,AES_key_size_bit_t KeyLength,AES_OpMode_t Mode)
 * \brief Perform AES in encryption or decryption mode with 128,192 or 256-bit key.
 * \param[in] AESSelect Select the AES to execute. Either the AES Hardware or the AES software
 * \param[in] Key 16, 24 or 32-byte key array according to the key size
 * \param[in] Message 16-byte message to encrypt or decrypt according to the mode
 * \param[out] Result 16-byte result
 * \param[in] KeyLength Length of the key in bits: 128, 192 or 256
 * \param[in] Mode Mode of operation. Either AES_ENCRYPTION or AES_DECRYPTION
 * \return Error status
 * \ingroup AES
 */
AES_Lib_error_t AES(AES_Select_t AESSelect, uint8_t * Key , uint8_t*Message, uint8_t*Result,AES_key_size_bit_t KeyLength,AES_OpMode_t Mode);


#endif /* _AES_H */
