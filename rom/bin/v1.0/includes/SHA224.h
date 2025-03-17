////////////////////////////////////////////////////////////////////////////////
///
/// @file       SHA224.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief      SHA224 algorithm
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
#ifndef SHA224_H
#define SHA224_H

/**
* @defgroup SHA224 SHA224
* @brief SHA224 algorithm as defined in FIPS 180-4.
*
*/

/** Digest size for SHA224 in bytes
* \ingroup SHA224
*/
#define SHA224_DIGEST_SIZE      28              //28 bytes = 224 bits

/** Internal Digest size for SHA224 in bytes
* \ingroup SHA224
*/
#define SHA224_INTERNAL_DIGEST_SIZE     32

/** Block size for SHA224 in bytes
* \ingroup SHA224
*/
#define SHA224_BLOCK_SIZE       64

/** SHA224 context
* \ingroup SHA224
*/
typedef struct
{
    uint32_t Hash[SHA224_INTERNAL_DIGEST_SIZE / 4];         /**< Internal digest size */
    uint8_t  Block[SHA224_BLOCK_SIZE];                      /**< Working buffer to accumulate data to hash */
    uint32_t MessageLen;                                    /**< Size in byte of the message accumulated  till now */
    uint8_t  Index;                                         /**< Size of the data accumulated in the Block buffer*/
} SHA224_CTX;

/**
 * @brief Error status words for SHA224
 * @ingroup SHA224
 */
typedef enum
{
    /// SHA224 computation successful
    SHA224_SUCCESS=0,
    /// Result pointer is null
    SHA224_INCORRECT_RESULT_POINTER
} SHA224_Lib_error_t;

//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------
/**
 * \fn SHA224_Lib_error_t SHA224(uint8_t * Message, uint8_t * Result,  uint32_t MessageLength)
 * \brief  Computes the hash of the message with SHA224
 * \param[in] Message Message to hash
 * \param[out] Result Digest of the message
 * \param[in] MessageLength Message length in bytes.
 * \return Error status
   \ingroup SHA224
 */
extern  SHA224_Lib_error_t SHA224(uint8_t * Message, uint8_t * Result, uint32_t MessageLength);


/**
* \fn SHA224_Lib_error_t SHA224Init(SHA224_CTX * ctx);
* \brief  Initialize a SHA224 computation
* \param[out] ctx SHA224 context
* \return Error status
\ingroup SHA224
*/
extern SHA224_Lib_error_t SHA224Init(SHA224_CTX * ctx);

/**
* \fn SHA224_Lib_error_t SHA224Update(SHA224_CTX * ctx, uint8_t * pbInput, uint32_t InputLength);
* \brief Update the context with the data given in input
* \param[in,out] ctx SHA224 context
* \param[in] pbInput data to hash
* \param[in,out] InputLength Length of the data pointed by pbInput(in byte)
* \return Error status
\ingroup SHA224
*/
extern SHA224_Lib_error_t SHA224Update(SHA224_CTX * ctx, uint8_t * pbInput, uint32_t InputLength);

/**
* \fn SHA224_Lib_error_t SHA224Final(SHA224_CTX * ctx, uint8_t * pbResult);
* \brief Update the context with the data given in input
* \param[in] ctx SHA224 context
* \param[out] pbResult Final digest (28 bytes)
* \return Error status
\ingroup SHA224
*/
extern SHA224_Lib_error_t SHA224Final(SHA224_CTX * ctx, uint8_t * pbResult);

#endif
