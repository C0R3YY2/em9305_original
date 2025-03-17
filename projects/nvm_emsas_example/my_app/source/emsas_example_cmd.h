////////////////////////////////////////////////////////////////////////////////
///
/// @file       emsas_example_cmd.h
///
/// @project    T9305
///
/// @brief      EMSAS command examples.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic
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

#ifndef __EMSAS_EXAMPLE_CMD_H
#define __EMSAS_EXAMPLE_CMD_H

#include <em_system.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * EMS Commands
 * Opcode: OGF (6bits) + OCF (10 bits)
 *         --> OGF = 0x3F (Vendor HCI Commands)
 *         --> OCF: EMOGF (4 bits) + EMOCF (6bits)
 */
/// EMS Application Specific Commands
#define EMSAS_OPCODE_TIMER_TEST               0xFFC0u
#define EMSAS_AUTHMIN_TIMER_TEST              EMS_AUTH_FLAG_VALUE_NONE

/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void EMSAS_TimerCmd(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

#endif // __EMSAS_EXAMPLE_CMD_H
