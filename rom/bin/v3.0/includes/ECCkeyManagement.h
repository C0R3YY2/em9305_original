////////////////////////////////////////////////////////////////////////////////
///
/// @file       ECCKeyManagement.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief     ECC key generation functions
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2015 EM Microelectronic
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

#ifndef ECCKEYMANAGEMENT_H
#define ECCKEYMANAGEMENT_H

/**
* @defgroup ECCKEY ECC Key
* @ingroup ECC
* @brief ECC P-256 Key management functions
*
*/
//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------

/**
 * \fn ECC_Error_t ECC_GeneratePrivateKey(uint32_t* PrivateKey)
 * \brief Generates an ECC private key.
 * \param[out] PrivateKey The generated private key, a scalar of the size of the curve (32 -bytes)
 * \return Error status
 *  \retval #SW_ECC_OK Successful private key generation
 *  \retval #SW_ECC_RANDOM_ERROR Error in random generation
 *
\ingroup ECCKEY
 */
extern ECC_Error_t ECC_GeneratePrivateKey(uint32_t* PrivateKey);

/**
 * \fn ECC_Error_t ECC_ComputePubKey(uint32_t* PrivateKey,POINT* PubKey)
 * \brief Computes the ECC public key associated to a private key.
 * \param[in] PrivateKey ECC private key (a scalar)
 * \param[out] PubKey ECC generated public key (a point in affine coordinates)
 * \return Error status
 *
 * \retval #SW_ECC_OK   No error occurred
 * \retval #SW_ECC_REDUCED_PRIVATE_KEY  Computation was performed but the private key was reduced modulo N
 * \retval #SW_ECC_NULL_PRIVATE_KEY The private key is null. No computation performed.
 *
 * \ingroup ECCKEY
 */
extern ECC_Error_t ECC_ComputePubKey(uint32_t* PrivateKey,POINT* PubKey);

#endif
