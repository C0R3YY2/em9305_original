////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app.c
///
/// @project    T9305
///
/// @brief      Application
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

#include "my_app.h"

#include <common.h>
#include <my_app_signals.h>
#include <my_app_task.h>
#include <printf.h>
#include <qdec.h>
#include <qf_port.h>
#include <types.h>

#if defined(USE_SINGLE_MODE)
# include <timer.h>
#endif

/******************************************************************************\
 *  Constants
\******************************************************************************/

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

#if defined(USE_CONTINUOUS_MODE)
/**
 * @brief QDEC callback
 * @param status status of ISR can be report ready, overflow or double movement
 * @param values accumulator and double movement accumulator values
 * @note This callback is called when report ready, double movement or overflow is detected.
 */
static void MyApp_QdecIsrCallback(QDEC_Status_t status, QDEC_Values_t values)
{
    QK_ISR_ENTRY();

    switch (status)
    {
        case REPORT_READY:
            // Post an event to the task.
            MyAppTask_PostEvent(QDEC_REPORT_READY_ACC_SIG,
                                (void *)values.accumulator);
            MyAppTask_PostEvent(QDEC_REPORT_READY_ACC_DBL_SIG,
                                (void *)(uint32_t)values.dbl_mvt_accumulator);
            break;

        case OVERFLOW_ERROR:
            // Post an event to the task.
            MyAppTask_PostEvent(QDEC_OVERFLOW_SIG, NULL);
            break;

        case DOUBLE_MOVEMENT_ERROR:
            // Post an event to the task.
            MyAppTask_PostEvent(QDEC_DBL_MVT_SIG, NULL);
            break;
    }

    QK_ISR_EXIT();
}
#endif

#if defined(USE_SINGLE_MODE)
/**
 * @brief Timer callback
 *
 * This callback is called when the timer triggers an interrupt.
 */
static void MyApp_TimerCallback(Driver_Status_t status, void *pUserData)
{
    QK_ISR_ENTRY();

    // Post an event to the task.
    MyAppTask_PostEvent(TIMER_TICK_SIG, NULL);

    QK_ISR_EXIT();
}
#endif

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            printf("EM Microelectronic\r\n");

            printf("EM9305\r\n");

#if defined(USE_CONTINUOUS_MODE)
            QDEC_RegisterCallback(MyApp_QdecIsrCallback);

            QDEC_StartContinuousCapture();
#endif

#if defined(USE_SINGLE_MODE)
            // Configure 5 Hz timer (200ms)
            int8_t idx = Timer_SchedulePolling(5, MyApp_TimerCallback, NULL);

            // Start timers
            Timer_Enable(idx);
#endif

            /* Write your application initialization code here. */
        }
        break;

        case MY_APP_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

#if defined(USE_SINGLE_MODE)
        case TIMER_TICK_SIG:
        {
            int32_t acc;
            uint8_t dblAcc;

            QDEC_StartSingleCapture(&acc, &dblAcc);

            printf("Acc %d Dbl %d\r\n", acc, dblAcc);
        }
        break;
#endif

#if defined(USE_CONTINUOUS_MODE)
        case QDEC_REPORT_READY_ACC_SIG:
        {
            int32_t accumulator = (int32_t)pParams;

            printf("Report Ready : Acc %d\r\n", accumulator);
        }
        break;

        case QDEC_REPORT_READY_ACC_DBL_SIG:
        {
            uint32_t accumulator = (uint32_t)pParams;

            printf("Dbl Mvt Acc %d\r\n", (uint8_t)accumulator);
        }
        break;

        case QDEC_OVERFLOW_SIG:
        {
            printf("Overflow Error\r\n");
        }
        break;

        case QDEC_DBL_MVT_SIG:
        {
            printf("Double Movement Error\r\n");
        }
        break;
#endif

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event!
            ret = MY_APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
