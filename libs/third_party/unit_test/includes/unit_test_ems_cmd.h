////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/unit_test/includes/unit_test_ems_cmd.h
///
/// @project    EM9305
///
/// @brief      EM System Layer - Commands for Unit Test.
///
///
/// @classification  Confidential
///
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

#ifndef __UNIT_TEST_EMS_CMD_H
#define __UNIT_TEST_EMS_CMD_H

#include "em_system.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * EMS Commands
 * Opcode: OGF (6bits) + OCF (10 bits)
 *         --> OGF = 0x3F (Vendor HCI Commands)
 *         --> OCF: EMOGF (4 bits) + EMOCF (6bits)
 */
/// EMS Command - Unit Test - Init
#define EMSUT_OPCODE_INIT                           0xFE01u
#define EMSUT_AUTHMIN_INIT                          EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Continue
#define EMSUT_OPCODE_CONTINUE                       0xFE02u
#define EMSUT_AUTHMIN_CONTINUE                      EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Group Info
#define EMSUT_OPCODE_GROUP_INFO                     0xFE03u
#define EMSUT_AUTHMIN_GROUP_INFO                    EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Test Info
#define EMSUT_OPCODE_TEST_INFO                      0xFE04u
#define EMSUT_AUTHMIN_TEST_INFO                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Number Of Test Groups
#define EMSUT_OPCODE_NUMBER_OF_GROUP                0xFE05u
#define EMSUT_AUTHMIN_NUMBER_OF_GROUP               EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Generic Runner
#define EMSUT_OPCODE_GENERIC_RUNNER                 0xFE06u
#define EMSUT_AUTHMIN_GENERIC_RUNNER                EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Unit Test - Test Runner
#define EMSUT_OPCODE_TEST_RUNNER                    0xFE07u
#define EMSUT_AUTHMIN_TEST_RUNNER                   EMS_AUTH_FLAG_VALUE_NONE

/******************************************************************************\
 *  Functions prototypes
\******************************************************************************/

void UnityMessageEvent(uint32_t step, uint32_t line, uint8_t status, const char* string);

/**
 * @brief Printf that will be sent over EM Transport layer.
 * @param format A string that specifies the format of the output.
 *               Exactly the same behavior as the standard printf.
 */
void UnityPrintf(const char* format, ...);

#endif // __UNIT_TEST_EMS_CMD_H
