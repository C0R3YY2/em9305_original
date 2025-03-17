////////////////////////////////////////////////////////////////////////////////
///
/// @file       PRNG.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     Pseudo Random Number Generator (PRNG)
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

#ifndef _PRNG_H
#define _PRNG_H

#include "PRNG_NIST_SP_800_90A.h"
#include "AES.h"

/**
* @defgroup PRNG PRNG
* @brief Generation of cryptographic random numbers
*
*/
//--------------------------------------------------------------------------------------------
//1-Choose the RNG Core
//--------------------------------------------------------------------------------------------
/** Selection of the Hardware RNG core
* \ingroup PRNG
*/
typedef enum
{
    ///RNG default core is core 0
    HW_DEFAULT_RNG_CORE=0,
    ///Core 0
    CORE_0=0,
    ///Core 1
    CORE_1=1,
    ///Core 2
    CORE_2=2,
    ///Core 3
    CORE_3=3

}PRNG_Core_t;
//--------------------------------------------------------------------------------------------
//2-Pseudo RNG configuration
//--------------------------------------------------------------------------------------------

/** Default entropy per bit of the hardware RNG.\n
* The pseudo RNG requires a HW random with an entropy of 256 bits.
* If the HW is not perfect, thus has not an entropy of 1 per bit, we need a longer HW random string.
* If the entropy per bit of the HW is p , ( a number between [0,1] ) then , we need a string of
* 256/p bits.\n
* The code supports entropy between 0.4 and 1.\n
* By default, before the characterisation, we expect that the entropy per bit of the HW is 0.8.\n
* \ingroup PRNG
*/
#define HW_DEFAULT_ENTROPY_PER_BIT      0.8
/** Default number of hardware random bits, picked by the pseudo random generator.
* \ingroup PRNG
*/
#define HW_PRNG_DEFAULT_CONFIG  (256 / HW_DEFAULT_ENTROPY_PER_BIT)

//--------------------------------------------------------------------------------------------
// 3-health tests
//--------------------------------------------------------------------------------------------
/** Health tests performed on the hardware RNG sequence
* \ingroup PRNG
*/
typedef enum
{
    /// Health tests disabled
    DISABLE_HEALTH_TESTS=0,
    /// Health tests enabled
    ENABLE_HEALTH_TESTS=1,
    /// Health test default configuration
    HEALTH_TESTS_DEFAULT_CONFIG=1,
}PRNG_Health_t;

//--------------------------------------------------------------------------------------------
// 4-Underlying AES
//--------------------------------------------------------------------------------------------
/** Underlying AES for the post processing.
* \ingroup PRNG
*/
typedef enum
{
    ///AES software
    AES_PRNG_HARDWARE=AES_HARDWARE,
    ///AES Software
    AES_PRNG_SOFTWARE=AES_SOFTWARE,
    ///Default AES is the AES hardware
    HW_PRNG_DEFAULT_AES_CONFIG= AES_HARDWARE
}PRNG_AES_t;

//--------------------------------------------------------------------------------------------
//Error Status
//--------------------------------------------------------------------------------------------
/** Error status for PRNG lib
* \ingroup PRNG
*/
typedef enum
{
    ///AES computation successful
    SW_PRNG_OK=0,
    ///Repetitive health test failed
    SW_PRNG_HEALTH_REPETITIVE_ERROR=1,
    ///Adaptive health test failed
    SW_PRNG_HEALTH_ADAPTIVE_ERROR=2,
    ///PRNG initialization not OK
    SW_PRNG_NOK=3,
    /// Result pointer is not initialized
    SW_PRNG_INCORRECT_RESULT_POINTER=4,
    /// PRNG was not initialized. Execute InitPRNG first
    SW_PRNG_NOT_INITIALZED=5
} PRNG_Lib_error_t;

//--------------------------------------------------------------------------------------------
//Context definition
//--------------------------------------------------------------------------------------------
/**
 * Structure of PRNG  context
 * \ingroup PRNG
 */
typedef struct
{
    ///Choose AES SOFTWARE or AES HARDWARE
    uint8_t             ConfigAES;
    ///Choose the RNG core
    uint8_t             ConfigRngCore;
    ///Enable or disable health tests
    uint8_t             ConfigHealthTests;
    /// Padding
    uint8_t             rfu ;
    ///Choose the number of bits required to have 256 bits of entropy (256 to 640)
    uint32_t            ConfigPRNG;
    ///PRNG NIST context
    PRNG_NIST_CTX       NistCTX;
}PRNG_CTX;


//--------------------------------------------------------------------------------------------
//Context dedicated to Crypto operations
//--------------------------------------------------------------------------------------------
extern PRNG_CTX PRNG_CRYPTO_CTX;

//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------
/**
 * \fn PRNG_Lib_error_t SetPRNGCtx( PRNG_CTX    * Ctx,PRNG_AES_t AESSelect, PRNG_Core_t CoreSelect, uint32_t PRNGConfig,PRNG_Health_t HealthTests);
 * \brief Set the configuration parameters of a PRNG context
 * \param[out] Ctx Context of the PRNG
 * \param[in] AESSelect Select the underlying AES (AES_HARDWARE or AES_SOFTWARE)
 * \param[in] CoreSelect Select the underlying hardware RNG core (from 0 to 3)
 * \param[in] PRNGConfig Choose the number of bits required to have 256 bits of entropy (256 to 640)
 * \param[in] HealthTests Determine if the health tests will be enabled or not
 * \return  Error status
 * \note SetPRNGCtx only defines the parameters. It does not pick any random hardware and does not initialize the NIST SP800-90A context.
   \ingroup PRNG
 */
extern PRNG_Lib_error_t SetPRNGCtx( PRNG_CTX    * Ctx,PRNG_AES_t AESSelect, PRNG_Core_t CoreSelect, uint32_t PRNGConfig,PRNG_Health_t HealthTests);

/**
 * \fn PRNG_Lib_error_t InitPRNG(PRNG_CTX   * Ctx,uint8_t * MAC);
 * \brief  Initializes the PRNG. Pick hardware random to initialize the NIST SP800-90A context. Perform the health tests if required.
 * \param[in] Ctx Context of the PRNG
 * \param[in] MAC a specific string of the device. MAC should be 6 bytes long
 * \return  Error status
 * \note The PRNG must be (re)initialized when the PRNG context has been lost.
   \ingroup PRNG
 */
extern PRNG_Lib_error_t InitPRNG(PRNG_CTX   * Ctx,uint8_t * MAC);


/**
 * \fn PRNG_Lib_error_t GetPRNG(PRNG_CTX    * Ctx,uint32_t NumberOfWords,uint32_t*Random)
 * \brief Generates random values with the PRNG.
 * \param[in] Ctx PRNG Ctx
 * \param[in] NumberOfWords Number of 32-bit words to generate.
 * \param[out] Random A buffer of NumberOfWords *32 bits that receives the random values.
 * \return Error status
 * \note The PRNG must be (re)initialized when the PRNG context has been lost.
   \ingroup PRNG
 */
extern PRNG_Lib_error_t GetPRNG(PRNG_CTX    * Ctx,uint32_t NumberOfWords,uint32_t*Random);


/**
 * \fn PRNG_Lib_error_t  AES_GenerateAuthenticateChallenge(uint8_t * Challenge);
 * \brief Generates a 16-byte challenge for AES Authentication protocol
 * \param[out] Challenge a pointer on 16-byte result
 * \return Error status
   \ingroup PRNG
 */
extern PRNG_Lib_error_t  AES_GenerateAuthenticateChallenge(uint8_t * Challenge);

#endif
