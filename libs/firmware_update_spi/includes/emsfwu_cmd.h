////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_update_spi/includes/emsfwu_cmd.h
///
/// @project    T9305
///
/// @brief      Firmware update commands over SPI header file.
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

#ifndef EMSFWU_CMD_H
#define EMSFWU_CMD_H

#include <em_system.h>

/**
 * @addtogroup drivers
 * @{
 * @defgroup fwu_spi Firmware Update SPI library
 * @brief SPI support for Firmware Update feature.
 * @{
*/

/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * @brief Sorted array of command parsers for the firmware update.
 *
 * This is an array of function pointers to functions that parse a specific
 * command packet. Each function pointer is associated with a command opcode.
 * These parsers are sorted in ascending opcode order (for a binary search of
 * the list).
 */
extern const EMSystem_CommandParser_t gEMSCmdFWUCommandParsers[];

/**
 * @brief Number of firmware update command parsers.
 */
extern const uint16_t gEMSCmdFWUNumberOfCommandParsers;

/** @} */ // End of group fwu_spi
/** @} */ // End of addtogroup drivers

#endif // EMSFWU_CMD_H
