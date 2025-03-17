////////////////////////////////////////////////////////////////////////////////
///
/// @file       app_timer.h
///
/// @project    T9305
///
/// @brief      Application timer header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
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

#ifndef __APP_TIMER_H
#define __APP_TIMER_H

#include <types.h>

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the timer peripheral.
 */
void AppTimer_Init(void);

/**
 * @brief Get the current timer timestamp.
 * @return Current timer timestamp.
 */
SystemTime_t AppTimer_GetCurrentTimestamp(void);

/**
 * @brief Compute a timestamp.
 * @param anchorTimestamp Anchor timestamp to use to compute timestamp.
 * @param offsetInUs Number of us after/before anchor timestamp.
 * @return Computed timestamp.
 */
SystemTime_t AppTimer_GetTimestamp(SystemTime_t anchorTimestamp, int32_t offsetInUs);

/**
 * @brief Start the timer.
 * @param timestamp Timeout timestamp to use in compare match interrupt.
 */
void AppTimer_StartTimer(SystemTime_t timestamp);

/**
 * @brief Stop the timer.
 */
void AppTimer_StopTimer(void);

#endif // __APP_TIMER_H
