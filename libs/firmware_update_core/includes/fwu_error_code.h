////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_update_core/includes/fwu_error_code.h
///
/// @project    T9305
///
/// @brief      Firmware update error code header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
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

#ifndef _FWU_ERROR_CODE_H
#define _FWU_ERROR_CODE_H

#include "types.h"

/**
 * @addtogroup fwu_core
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @def FWU_ERROR_CODE_SUCCESS
 * Returned code in case of successful operation.
 */
#define FWU_ERROR_CODE_SUCCESS                        0x00u

/**
 * @def FWU_ERROR_CODE_UNKNOWN_COMMAND
 * An unknown opcode has been received.
 */
#define FWU_ERROR_CODE_UNKNOWN_COMMAND                0x02u

/**
 * @def FWU_ERROR_CODE_INVALID_COMMAND_PARAMETERS
 * One or more command parameters contain invalid values (e.g. an invalid packet length).
 */
#define FWU_ERROR_CODE_INVALID_COMMAND_PARAMETERS     0x03u

/**
 * @def FWU_ERROR_CODE_INSUFFICIENT_RESSOURCE
 * Not enough memory or buffer size too short to store data.
 */
#define FWU_ERROR_CODE_INSUFFICIENT_RESSOURCE         0x04u

/**
 * @def FWU_ERROR_CODE_INVALID_FIRMWARE_SIGNATURE
 * The signature check of a firmware image failed.
 */
#define FWU_ERROR_CODE_INVALID_FIRMWARE_SIGNATURE     0x05u

/**
 * @def FWU_ERROR_CODE_CHANNEL_UNAVAILABLE
 * Reserved for future use.
 */
#define FWU_ERROR_CODE_CHANNEL_UNAVAILABLE            0x06u

/**
 * @def FWU_ERROR_CODE_VOLTAGE_TOO_LOW
 * Reserved for future use.
 */
#define FWU_ERROR_CODE_VOLTAGE_TOO_LOW                0x07u

/**
 * @def FWU_ERROR_CODE_PROCEDURE_NOT_PERMITTED
 * The FW update state machine has entered into an invalid state.
 */
#define FWU_ERROR_CODE_PROCEDURE_NOT_PERMITTED        0x08u

/**
 * @def FWU_ERROR_CODE_NVM_LOCKED
 * Reserved for future use.
 */
#define FWU_ERROR_CODE_NVM_LOCKED                     0x09u

/**
 * @def FWU_ERROR_CODE_OPERATION_FAILED
 * The requested operation has failed.
 */
#define FWU_ERROR_CODE_OPERATION_FAILED               0x0Au

/**
 * @def FWU_ERROR_CODE_UNSPECIFIED_ERROR
 * An error occurred but the root cause is not identified.
 */
#define FWU_ERROR_CODE_UNSPECIFIED_ERROR              0x0Bu

/**
 * @def FWU_ERROR_CODE_PUBLIC_KEY_ERROR
 * The provided public key for verifying the FW image signature is invalid.
 */
#define FWU_ERROR_CODE_PUBLIC_KEY_ERROR               0x0Cu

/**
 * @def FWU_ERROR_CODE_SHA256_ERROR
 * Use of the SHA256 crypto engine failed.
 */
#define FWU_ERROR_CODE_SHA256_ERROR                   0x0Du

/**
 * @def FWU_ERROR_CODE_AES_ERROR
 * Use of the AES crypto engine failed.
 *
 */
#define FWU_ERROR_CODE_AES_ERROR                      0x0Eu

/**
 * @brief Firmware Update Error Code
 */
typedef uint8_t FWU_ErrorCode_t;

/**
 * @}
*/
#endif // _FWU_ERROR_CODE_H
