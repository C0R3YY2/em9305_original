////////////////////////////////////////////////////////////////////////////////
///
/// @file       bsp.c
///
/// @project    T9305
///
/// @brief      Board Support Package for QPC
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

#include <em_transport_manager.h>
#include <interrupts.h>
#include <prot_timer.h>
#include <types.h>

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static ProtTimer_EventTimeResult BSP_TimeEventCallback(
    SystemTime_t *pSystemTimeOfEvent);

/******************************************************************************\
 *  Global variables
\******************************************************************************/

static ProtTimer_EventTimeCallback_t gBSP_TimeEventCallback =
{
    .pTimeToNextEvent = BSP_TimeEventCallback,
    .pNext = NULL,
};

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static ProtTimer_EventTimeResult BSP_TimeEventCallback(
    SystemTime_t *pSystemTimeOfEvent)
{
    ProtTimer_EventTimeResult result = PT_EVENT_TIME_SLEEP_FORBIDDEN;

    // Timer enabled?
    if (0 != (PTIM->RegProtTimCompareCfg.r32 & PT_COMPARE_EN(1u << 0u)))
    {
        // Set the next event timestamp.
        *pSystemTimeOfEvent = ProtTimer_GetCompareValue(0);
        result = PT_EVENT_TIME_STAMP_VALID;
    }
    else
    {
        // No next event planned.
        result = PT_EVENT_TIME_INFINITE_TIME;
    }

    return result;
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

/**
 * @brief This is called by QPC when it is started.
 * @note Application specific part called from the QF_onStartup function.
 */
void QF_onStartupExt(void)
{
    // Register the sleep callback function.
    (void)ProtTimer_RegisterEventTimeCallback(&gBSP_TimeEventCallback);
}

/**
 * @brief This is called by QPC when there are no more tasks to run.
 * @note Application specific part called from the QK_onIdle function.
 */
void QK_onIdleExt(void)
{
    // Process all the EM Transport Manager requests.
    uint32_t irqThreshold = IRQ_DisableAndSaveInterrupts();
    irqThreshold = EMTransportManager_Process(irqThreshold);
    IRQ_RestoreInterrupts(irqThreshold);
}
