////////////////////////////////////////////////////////////////////////////////
///
/// @file       ECDSA_Verify.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief      ECDSA signature verification
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

#ifndef ECDSA_H
#define ECDSA_H


/**
* @defgroup ECDSA ECDSA
* @ingroup ECC
* @brief ECDSA signature verification based on P-256
*
*/

/**
 * \fn ECC_Error_t ECDSA_Verify(PointCertificate *Signature, POINT * PubKey, uint8_t * Message)
 * \brief Performs an ECDSA verification.
 * \param[in] Signature Signature of the message (a point)
 * \param[in] PubKey Public key of the system (a point)
 * \param[in] Message Message that was signed (32 -bytes)
 * \retval #SW_ECDSA_SIGNATURE_VALID Signature is valid
 * \retval #SW_ECDSA_INVALID_SIGNATURE Signature is invalid
 * \retval #SW_ECC_INVALID_PUBLIC_POINT Invalid public point
 * \retval #SW_ECC_SIGNATURE_COORDINATE_X_INVALID Invalid coordinate X
 * \retval #SW_ECC_SIGNATURE_COORDINATE_Y_INVALID Invalid coordinate Y
 * \return  Error status
 * \ingroup ECDSA
 */

extern ECC_Error_t ECDSA_Verify(PointCertificate *Signature, POINT * PubKey, uint8_t* Message);

#endif
