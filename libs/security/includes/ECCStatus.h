////////////////////////////////////////////////////////////////////////////////
///
/// @file       ECCStatus.h
///
/// @project    T9304
///
/// @author     SAS
///
/// @brief     ECC status words
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
/////////////////////////////////////////////////////////////////////////////////*----------------------------------------------------------------


#ifndef ECC_STATUS_H
#define ECC_STATUS_H

/**
 * @defgroup ECC ECC
 */

/**
 * @brief Error status for ECC
 * @ingroup ECC
 */
typedef enum
{
    /// Operation succeeded
    SW_ECC_OK=0,
    /// ECDSA signature is valid
    SW_ECDSA_SIGNATURE_VALID=0,
    /// Schnoor signature is valid
    SW_SCHNOOR_SIGNATURE_VALID=0,
    /// The ECDSA signature is not valid
    SW_ECDSA_INVALID_SIGNATURE=1,
    /// The Schnoor signature is not valid
    SW_SCHNOOR_SIGNATURE_INVALID=1,
    /// Public point is either not on the curve or is the infinity point
    SW_ECC_INVALID_PUBLIC_POINT=2,
    /// The private key is null. Computation failed.
    SW_ECC_NULL_PRIVATE_KEY=3,
    /// The private key is bigger than N. Computation was performed but the private key was reduced modulo N
    SW_ECC_REDUCED_PRIVATE_KEY=4,
    ///The coordinate X(r) of the signature is not valid (either bigger than N or null)
    SW_ECC_SIGNATURE_COORDINATE_X_INVALID=5,
    ///The coordinate Y(s) of the signature is not valid (either bigger than N or null)
    SW_ECC_SIGNATURE_COORDINATE_Y_INVALID=6,
    /// ECC Schnoor commitment is not valid. It is either not on the curve or is the infinity point
    SW_ECC_INVALID_COMMITMENT=7,
    /// Error in the random generation
    SW_ECC_RANDOM_ERROR=8,
    ///Non initialized result pointer
    SW_ECC_INCORRECT_RESULT_POINTER=9
} ECC_Error_t;
#endif
