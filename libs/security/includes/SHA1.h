////////////////////////////////////////////////////////////////////////////////
///
/// @file       SHA1.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     SHA1 algorithm
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
#ifndef SHA1_H
#define SHA1_H
/**
* @defgroup SHA1 SHA1
* @brief SHA1 algorithm as defined in FIPS 180-4.
*
*/
/** Digest size for SHA1 in bytes
* \ingroup SHA1
*/
#define SHA1_DIGEST_SIZE        20              //20 bytes = 160 bits

/** Block size for SHA1 in bytes
* \ingroup SHA1
*/
#define SHA1_BLOCK_SIZE         64

/** SHA1 context
* \ingroup SHA1
*/
typedef struct
{
    uint32_t Hash[SHA1_DIGEST_SIZE / 4];        /**< Digest buffer*/
    uint8_t  Block[SHA1_BLOCK_SIZE];            /**< Working buffer to accumulate data to hash */
    uint32_t MessageLen;                        /**< Size in bytes of the message accumulated  till now */
    uint8_t  Index;                             /**< Size of the data accumulated in the Block buffer*/
} SHA1_CTX;


/**
 * @brief Error status words for SHA1
 * @ingroup SHA1
 */
typedef enum
{
    /// SHA1 computation successful
    SHA1_SUCCESS=0,
    /// Result pointer is null
    SHA1_INCORRECT_RESULT_POINTER
} SHA1_Lib_error_t;


//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------

/**
 * \fn void SHA1(uint8_t * Message, uint8_t * Result,  uint32_t MessageLength)
 * \brief  Computes the hash of the message with SHA1
 * \param[in] Message Message to hash
 * \param[out] Result Digest of the message
 * \param[in] MessageLength Message length in bytes
 * \retval #SHA1_SUCCESS Successful computation
 * \retval #SHA1_INCORRECT_RESULT_POINTER Result buffer not initialized
 * \return Error status
   \ingroup SHA1
 */
extern SHA1_Lib_error_t SHA1(uint8_t * Message, uint8_t * Result,  uint32_t MessageLength);

/**
* \fn void SHA1Init(SHA1_CTX * ctx);
* \brief  Initializes a SHA1 computation
* \param[out] ctx SHA-1 context
* \retval #SHA1_SUCCESS Successful initialization
* \return Error status
\ingroup SHA1
*/
extern SHA1_Lib_error_t SHA1Init(SHA1_CTX * ctx);
/**
* \fn void SHA1Update(SHA1_CTX * ctx, uint8_t * pbInput, uint32_t InputLength);
* \brief Updates the context with the data given in input
* \param[in,out] ctx SHA-1 context
* \param[in] pbInput Data to hash
* \param[in,out] InputLength Length of the data pointed by pbInput (in bytes)
* \retval #SHA1_SUCCESS Successful computation
* \return Error status
\ingroup SHA1
*/

extern SHA1_Lib_error_t SHA1Update(SHA1_CTX * ctx, uint8_t * pbInput, uint32_t InputLength);
/**
* \fn void SHA1Final(SHA1_CTX * ctx, uint8_t * pbResult);
* \brief Finalizes the hash computation and returns the result
* \param[in] ctx SHA-1 context
* \param[out] pbResult Final digest
* \retval #SHA1_SUCCESS Successful computation
* \retval #SHA1_INCORRECT_RESULT_POINTER Result buffer not initialized
* \return Error status
\ingroup SHA1
*/
extern SHA1_Lib_error_t SHA1Final(SHA1_CTX * ctx, uint8_t * pbResult);

#endif
