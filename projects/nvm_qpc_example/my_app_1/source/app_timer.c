////////////////////////////////////////////////////////////////////////////////
///
/// @file       app_timer.c
///
/// @project    T9305
///
/// @brief      Application timer source file.
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

#include "app_timer.h"
#include <irq.h>
#include <my_app_1_task.h>
#include <my_app_signals.h>
#include <prot_timer.h>
#include <qf_port.h>
#include <types.h>

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

/**
 * @brief Called when the timer expired.
 * @param timestamp Timestamp that trigged this event.
 */
void AppTimer_TimerExpired(SystemTime_t timestamp)
{
    // Stop the timer.
    AppTimer_StopTimer();

    // Post an event to the Task1 task.
    QK_ISR_ENTRY();
    MyApp1Task_PostEvent(TIMER_TICK_SIG, (void*)timestamp);
    QK_ISR_EXIT();
}

/**
 * @brief Initialize the timer peripheral.
 */
void AppTimer_Init(void)
{
    // Set timer clock frequency to 24MHz.
    ProtTimer_SetDivider(1);
    // Auto reload mode.
    ProtTimer_SetConfiguration(true, false);
    // Set full value to maximum.
    ProtTimer_SetFullValue(0xFFFFFFFF);

    // Configure IRQs for PT.
    IRQ_Mask(IRQ_GROUP_PROTOCOL_TIMER, 0xFFFFFFFF);
    IRQ_Clear(IRQ_GROUP_PROTOCOL_TIMER, 0xFFFFFFFF);

    // Enable IRQ on channel 0.
    IRQ_Clear(IRQ_GROUP_PROTOCOL_TIMER, IRQ_PT_OUT_CMP_EN(0x01));
    IRQ_Enable(IRQ_GROUP_PROTOCOL_TIMER, IRQ_PT_OUT_CMP_EN(0x01));
    IRQ_Unmask(IRQ_GROUP_PROTOCOL_TIMER, IRQ_PT_OUT_CMP_EN(0x01));

    // Init timer channel 0.
    ProtTimer_SetCompareEnable(0, false);
    // Start timer and keep it running all the time.
    ProtTimer_Clear();
    ProtTimer_Start();
}

/**
 * @brief Get the current timer timestamp.
 * @return Current timer timestamp.
 */
SystemTime_t AppTimer_GetCurrentTimestamp(void)
{
    return ProtTimer_GetCurrentValue();
}

/**
 * @brief Compute a timestamp.
 * @param anchorTimestamp Anchor timestamp to use to compute timestamp.
 * @param offsetInUs Number of us after/before anchor timestamp.
 * @return Computed timestamp.
 */
SystemTime_t AppTimer_GetTimestamp(SystemTime_t anchorTimestamp, int32_t offsetInUs)
{
    return (anchorTimestamp + ((SystemTime_t)(offsetInUs * CLOCK_FREQ_MHZ)));
}

/**
 * @brief Start the timer.
 * @param timestamp Timeout timestamp to use in compare match interrupt.
 */
void AppTimer_StartTimer(SystemTime_t timestamp)
{
    // Set the compare value of the channel 0.
    ProtTimer_SetCompareValue(0, timestamp);
    // Enable compare channel 0.
    ProtTimer_SetCompareEnable(0, true);
}

/**
 * @brief Stop the timer.
 */
void AppTimer_StopTimer(void)
{
    // Disable compare channel 0.
    ProtTimer_SetCompareEnable(0, false);
}

/******************************************************************************\
 *  Interrupt service routines
\******************************************************************************/

/**
 * @brief Protocol Timer Full Value Interrupt.
 */
void _Interrupt IRQHandler_ProtocolTimerFullVal(void)
{
    // Clear IRQ.
    IRQ_Clear(IRQ_GROUP_PROTOCOL_TIMER, IRQ_PT_FULL_VAL_EN(0x01));
}

/**
 * @brief Protocol Timer Channel 0 Interrupt.
 */
void _Interrupt IRQHandler_ProtocolTimerOutCmp0(void)
{
    // Clear IRQ.
    IRQ_Clear(IRQ_GROUP_PROTOCOL_TIMER, IRQ_PT_OUT_CMP_EN(0x01));

    // Call the timer callback.
    AppTimer_TimerExpired(ProtTimer_GetCompareValue(0));
}
