////////////////////////////////////////////////////////////////////////////////
///
/// @file       SHA384.h
///
/// @project    T9304
///
/// @author     SAS
///
/// @brief     SHA384 algorithm
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
#ifndef SHA384_H
#define SHA384_H

/**
* @defgroup SHA384 SHA384
* @brief SHA384 algorithm as defined in FIPS 180-4.
*
*/

/** Digest size for SHA384 in bytes
* \ingroup SHA384
*/
#define SHA384_DIGEST_SIZE      48              //48 bytes = 384 bits

/** Internal Digest size for SHA384 in bytes
* \ingroup SHA384
*/
#define SHA384_INTERNAL_DIGEST_SIZE     64      //internal digest is 64 bytes. Only 48 bytes are kept for the digest

/** Block size for SHA384 in bytes
* \ingroup SHA384
*/
#define SHA384_BLOCK_SIZE           128

/** SHA384 context
* \ingroup SHA384
*/
typedef struct
{
    uint32_t Hash[SHA384_INTERNAL_DIGEST_SIZE/4];       /**< Internal digest size */
    uint8_t  Block[SHA384_BLOCK_SIZE];                  /**< Working buffer to accumulate data to hash */
    uint32_t MessageLen;                                /**< Size in byte of the message accumulated  till now */
    uint8_t  Index;                                     /**< Size of the data accumulated in the Block buffer*/
} SHA384_CTX;

/**
 * @brief Error status words for SHA384
 * @ingroup SHA384
 */
typedef enum
{
    /// SHA384 computation successful
    SHA384_SUCCESS=0,
    /// Result pointer is null
    SHA384_INCORRECT_RESULT_POINTER
} SHA384_Lib_error_t;

//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------
/**
 * \fn SHA384_Lib_error_t SHA384(uint8_t * Message, uint8_t * Result,  uint32_t MessageLength)
 * \brief  Computes the hash of the message with SHA384
 * \param[in] Message Message to hash
 * \param[out] Result Digest of the message
 * \param[in] MessageLength Message length in bytes.
 * \return Error status
   \ingroup SHA384
 */
extern  SHA384_Lib_error_t SHA384(uint8_t * Message, uint8_t * Result, uint32_t MessageLength);

/**
* \fn SHA384_Lib_error_t SHA384Init(SHA384_CTX * ctx)
*\brief  Initialize a SHA384 computation
* \param[out] ctx SHA384 context
* \return Error status
\ingroup SHA384
*/
extern SHA384_Lib_error_t SHA384Init(SHA384_CTX * ctx);

/**
* \fn SHA384_Lib_error_t SHA384Update(SHA384_CTX * ctx, uint8_t * pbInput, uint32_t InputLength)
* \brief Update the context with the data given in input
* \param[in,out] ctx SHA384 context
* \param[in] pbInput data to hash
* \param[in,out] InputLength Length of the data pointed by pbInput(in byte)
* \return Error status
\ingroup SHA384
*/
extern SHA384_Lib_error_t SHA384Update(SHA384_CTX * ctx, uint8_t * pbInput, uint32_t InputLength);

/**
* \fn SHA384_Lib_error_t SHA384Final(SHA384_CTX * ctx, uint8_t * pbResult);
* \brief Update the context with the data given in input
* \param[in] ctx SHA384 context
* \param[out] pbResult Final digest (64 bytes)
* \return Error status
\ingroup SHA384
*/
extern SHA384_Lib_error_t SHA384Final(SHA384_CTX * ctx, uint8_t * pbResult);

#endif
