////////////////////////////////////////////////////////////////////////////////
///
/// @file       ECCTypedef.h
///
/// @project    T9305
///
/// @author     SAS
///
/// @brief      Specific typedef for ECC
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
#ifndef ECC_TYPEDFE_H
#define ECC_TYPEDFE_H



/**
 * @brief Type definition for ECC module
 * @ingroup ECC
 */

//Curve sizes
/**
 * @brief Size of P-128 coordinats in 32-bit words
 * @ingroup ECC
 */
#define CURVE_SIZE_P128     4
/**
 * @brief Size of P-256 coordinats in 32-bit words
 * @ingroup ECC
 */
#define CURVE_SIZE_P256     8

/**
 * @brief Size max of a coordinates supported by this lib
 * @ingroup ECC
 */
#define CURVE_SIZE_MAX      8

/**
 * @brief Size of a P-128 scalar expressed in 32-bit words
 * @ingroup ECC
 */
#define SCALAR_SIZE_P128  4

/**
 * @brief Size of a P-256 scalar expressed in 32-bit words
 * @ingroup ECC
 */
#define SCALAR_SIZE_P256  8

/**
 * @brief Size max of a scalar expressed in 32-bit words
 * @ingroup ECC
 */
#define SCALAR_SIZE_MAX   SCALAR_SIZE_P256

/**
 * @brief Generic ECC point affine structure
 * @ingroup ECC
 */
typedef struct{
    ///Coordinate X
    uint32_t x[CURVE_SIZE_MAX];
    ///Coordinate Y
    uint32_t y[CURVE_SIZE_MAX];
}POINT;


/**
 * @brief Point on P-128
 * @ingroup ECC
 */
typedef struct{
    ///Coordinate X
    uint32_t x[CURVE_SIZE_P128];
    ///Coordinate Y
    uint32_t y[CURVE_SIZE_P128];
}POINT_P128;

/**
 * @brief Point on P-256
 * @ingroup ECC
 */
typedef struct{
    ///Coordinate X
    uint32_t x[CURVE_SIZE_P256];
    ///Coordinate Y
    uint32_t y[CURVE_SIZE_P256];
}POINT_P256;



/**
 * @brief signature/certificate structure
 * @ingroup ECC
 */
typedef struct{
    ///Coordinate X
    uint32_t x[SCALAR_SIZE_MAX];
    ///Coordinate Y
    uint32_t y[SCALAR_SIZE_MAX];
}PointCertificate;



#endif
