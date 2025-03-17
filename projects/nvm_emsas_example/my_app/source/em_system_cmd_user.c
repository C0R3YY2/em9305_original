////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_system_user_cmd.c
///
/// @project    T9305
///
/// @brief      EM System Layer - Vendor Application Specific Commands.
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

#include "emsas_example_cmd.h"
#include <em_system.h>
#include <macros.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * @brief Sorted array of command parsers.
 *
 * This is an array of function pointers to functions that parse a specific
 * command packet. Each function pointer is associated with a command opcode.
 * These parsers are sorted in ascending opcode order (for a binary search of
 * the list).
 */
const EMSystem_CommandParser_t gEMSAS_ExampleCommandParsers[] =
{
    {
        .opcode     = EMSAS_OPCODE_TIMER_TEST,
        .function   = EMSAS_TimerCmd,
        .authMin    = EMSAS_AUTHMIN_TIMER_TEST,
        .reserved   = 0u,
    },
};

/// Number of command parsers in the array.
const uint16_t gEMSAS_ExampleNumberOfCommandParsers =
    ARRAY_DIM(gEMSAS_ExampleCommandParsers);
