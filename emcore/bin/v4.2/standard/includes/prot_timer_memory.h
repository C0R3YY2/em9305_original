////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/prot_timer/includes/prot_timer_memory.h
///
/// @project    T9305
///
/// @brief      Protocol timer driver memory.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
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


#ifndef _PROT_TIM_MEMORY_H
#define _PROT_TIM_MEMORY_H


#include <types.h>
#include <prot_timer.h>



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief Persistent memory for the Protocol Timer module.
 * @ingroup prottim
 */
typedef struct
{
    /// State of the Protocol Timer hardware.
    ProtTimer_HardwareState_t hardwareState;

    /// Restore state indicating actions after wake-up
    ProtTimer_RestoreState_t  restoreState;
}  ProtTimer_PersistentMemory_t;



/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

// This is defined as an extern to a uint32_t array to prevent it's use within
// the module. Using this within the module would make the memory address
// hardcoded within the ROM.
#ifndef PROT_TIM_MEMORY_INTERNAL

/**
 * @brief Reference to the persistent memory for the Protocol Timer module.
 * @note Do not use this directly.
 * @ingroup prottim
 */
extern char gProtTimer_PersistentMemory[];


#endif // PROT_TIM_MEMORY_INTERNAL


#endif /* _PROT_TIM_MEMORY_H */
