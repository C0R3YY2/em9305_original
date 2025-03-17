////////////////////////////////////////////////////////////////////////////////
///
/// @file       PRNG_NIST_SP_800_90A.h
///
/// @project    T9304
///
/// @author     SAS
///
/// @brief     Pseudo Random Number generator according to NIST SP 800_90A specifications
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

#ifndef _PRNG_NIST_H
#define _PRNG_NIST_H

/**
* @defgroup PRNG_NIST NIST PRNG
* @brief Generation of cryptographic random numbers according to SP 800-90A NIST specification
*
*/
/**
 * Structure of PRNG NIST context
 * \ingroup PRNG_NIST
 */
typedef struct
{
    /// PRNG context key value
    uint32_t Key[4];
    /// PRNG context internal state
    uint32_t V[4];
    /// PRNG's number of uses since last reseeding or reset
    uint32_t ResetCounter;
}PRNG_NIST_CTX;

/** Error status word for PRNG_NIST_SP_800_90A module
* \ingroup PRNG_NIST
*/
typedef enum
{
    ///Computation was successful
    SW_PRNG_NIST_SP_800_90A_OK=0,
    ///Perso string is too long compared to the seed
    SW_PRNG_NIST_PERSO_STRING_TOO_LONG=1,
    ///Random not generated. The PRNG must be reinitialized.
    SW_PRNG_NIST_SP_800_90A_RESEED= 2,
    ///Result pointer is null
    SW_PRNG_NIST_INCORRECT_RESULT_POINTER= 3,
    /// Error in AES computation
    SW_PRNG_NIST_AES_ERROR= 4
}PRNG_NIST_Error_t;


/** Support the reseeding option OFF
* \ingroup PRNG_NIST
*/
#define OFF             0
/** Support the reseeding option ON
* \ingroup PRNG_NIST
*/

#define ON              1
/** Support the reseeding option
* \ingroup PRNG_NIST
*/
#define RESEED_OPTION   OFF

//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------

/**
 * \fn PRNG_NIST_Error_t CTR_DRBG_Instantiate(AES_Select_t AESSelect,PRNG_NIST_CTX  * Ctx,uint32_t * EntropyInput, uint32_t * PersoString, uint32_t LenPersoString);
 * \brief  Instantiates the PRNG state without using the derivation function.The PRNG context is initialized after this function.
 * \param[in] AESSelect Choose either the AES_HARDWARE or the AES_SOFTWARE as underlying AES
 * \param[in,out] Ctx Context the PRNG NIST
 * \param[in] EntropyInput A SEEDL32EN buffer of FULL entropy
 * \param[in] PersoString  Additional perso string , of max 32 bytes
 * \param[in] LenPersoString Data length  of the perso string in word . Should be max 32/4
 * \return  Error Status
   \ingroup PRNG_NIST
 */
extern PRNG_NIST_Error_t CTR_DRBG_Instantiate(AES_Select_t AESSelect,PRNG_NIST_CTX  * Ctx,uint32_t * EntropyInput, uint32_t * PersoString, uint32_t LenPersoString);
/**
 * \fn PRNG_NIST_Error_t CTR_DRBG_Instantiate_WithDF(AES_Select_t AESSelect,PRNG_NIST_CTX   * Ctx,uint32_t * EntropyInput, uint32_t LenEntropyInput, uint32_t * Nonce, uint32_t LenNonce,uint32_t * PersoString, uint32_t LenPersoString);
 * \brief  Instantiates the PRNG state with the sderivation function. The PRNG context is initialized after this function.
 * \param[in] AESSelect Choose either the AES_HARDWARE or the AES_SOFTWARE as underlying AES
 * \param[in,out] Ctx Context the PRNG NIST
 * \param[in] EntropyInput A buffer with random bits (no need of full entropy)
 * \param[in] LenEntropyInput Number of words of EntropyInput
 * \param[in] Nonce A nonce
 * \param[in] LenNonce Number of words of the nonce
 * \param[in] PersoString A buffer of perso string
 * \param[in] LenPersoString Data length  of the perso string in u32 . Should be max 32/4
* \return  Error Status
   \ingroup PRNG_NIST
 */
extern PRNG_NIST_Error_t CTR_DRBG_Instantiate_WithDF(AES_Select_t AESSelect,PRNG_NIST_CTX   * Ctx,uint32_t * EntropyInput, uint32_t LenEntropyInput, uint32_t * Nonce, uint32_t LenNonce,uint32_t * PersoString, uint32_t LenPersoString);

#if RESEED_OPTION==ON
/**
 * \fn u8 CTR_DRBG_Reseed_FUNC(u32 * EntropyInput, u32 * AdditionalInput, u32 LenAdditionalInput);
 * \brief Reseeds the state without the derivation function
 * \param[in] EntropyInput A SEEDLEN buffer of full entropy
 * \param[in] AdditionalInput Additional perso string , of max 32/4
 * \param[in] LenAdditionalInput Data length of the perso string in word . Should be max 32/4
 * \return  SW_PRNG_NIST_SP_800_90A_OK  : Reseeed performed successfully
 * \return  SW_PRNG_NIST_SP_800_90A_NOK : NOK , the additionalInput is too big
   \ingroup PRNG_NIST
 */
    extern uint8_t CTR_DRBG_Reseed(uint32_t * EntropyInput, uint32_t * AdditionalInput, uint32_t LenAdditionalInput);
/**
 * \fn u8 CTR_DRBG_Reseed_WithDF_FUNC(u32 * EntropyInput, u32 EntropyLength,u32 * AdditionalInput, u32 LenAdditionalInput);
 * \brief Reseeds the state with the derivation function
 * \param[in] EntropyInput A buffer with entropy(not necessarily full)
 * \param[in] EntropyLength Length of the Entropy Input
 * \param[in] AdditionalInput Additional input
 * \param[in] LenAdditionalInput Length of the additional input
 * \return  SW_PRNG_NIST_SP_800_90A_OK  : Reseed performed successfully
 * \return  SW_PRNG_NIST_SP_800_90A_NOK : inputs are too big
   \ingroup PRNG_NIST
 */
extern uint8_t CTR_DRBG_Reseed_WithDF(uint32_t * EntropyInput, uint32_t EntropyLength,uint32_t * AdditionalInput, uint32_t LenAdditionalInput);
#endif


/**
 * \fn PRNG_NIST_Error_t CTR_DRBG_Generate(AES_Select_t AESSelect,PRNG_NIST_CTX * Ctx,uint32_t RequestedNumberOfBits,uint32_t * AdditionalInput, uint32_t LenAdditionalInput, uint32_t*Result);
 * \brief Generates random bits without the derivation function
 * \param[in] AESSelect Choose either the AES_HARDWARE or the AES_SOFTWARE as underlying AES
 * \param[in,out] Ctx Context the PRNG NIST
 * \param[in] RequestedNumberOfBits Number of bits to generate
 * \param[in] AdditionalInput An additional input (any size)
 * \param[in] LenAdditionalInput Number of bytes of AdditionalInput
 * \param[out] Result Buffer for the result
 * \return  Error Status
   \ingroup PRNG_NIST
 */
extern PRNG_NIST_Error_t CTR_DRBG_Generate(AES_Select_t AESSelect,PRNG_NIST_CTX * Ctx,uint32_t RequestedNumberOfBits,uint32_t * AdditionalInput, uint32_t LenAdditionalInput, uint32_t*Result);

/**
 * \fn  PRNG_NIST_Error_t CTR_DRBG_Generate_WithDF(AES_Select_t AESSelect,PRNG_NIST_CTX * Ctx,uint32_t RequestedNumberOfBits,uint32_t * AdditionalInput, uint32_t LenAdditionalInput, uint32_t*Result)
 * \brief Generates random bits with the derivation function
 * \param[in] AESSelect Choose either the AES_HARDWARE or the AES_SOFTWARE as underlying AES
 * \param[in,out] Ctx Context the PRNG NIST
 * \param[in] RequestedNumberOfBits Number of bits to generate
 * \param[in] AdditionalInput An additional input (any size)
 * \param[in] LenAdditionalInput Number of bytes of AdditionalInput
 * \param[out] Result Buffer for the result
 * \return  Error Status
   \ingroup PRNG_NIST
 */
extern PRNG_NIST_Error_t CTR_DRBG_Generate_WithDF(AES_Select_t AESSelect,PRNG_NIST_CTX  * Ctx,uint32_t RequestedNumberOfBits,uint32_t * AdditionalInput, uint32_t LenAdditionalInput, uint32_t*Result);


#endif
