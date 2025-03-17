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

#include <types.h>
#include <macros.h>
#include <pml.h>
#include <uart.h>
#include <gpio.h>
#include <common.h>
#include <printf.h>
#include <temperature_indicator.h>
#include <timer.h>
#include <timer_hal.h>
#include <qep.h>
#include <qf_port.h>
#include <bsp.h>

#include "my_app.h"
#include <my_app_signals.h>
#include <my_app_task.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

static uint8_t gCounter = 0;

/// Temperature measured by temperature indicator
static float gTemperature = 0.0f;

/// Task active object.
QL_Task_t AO_Task;

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static void Timer_Callback(Driver_Status_t status, void* pUserData);
static void TI_Callback(float temp);

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/
/**
 * @brief Timer callback
 *
 * This callback is called when the timer triggers an interrupt.
 */
static void Timer_Callback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();
    // Post the timer event
    MyAppTask_PostEvent(MY_APP_TIMER_TICK, NULL);
    QK_ISR_EXIT();
}

/**
 * @brief Temperature Indicator callback
*/
static void TI_Callback(float temp)
{
    // The action on interrupt handler is to store the measured temperature.
    QK_ISR_ENTRY();
    // Store the measured temperature.
    gTemperature = temp;
    // Then post the event indicating that a new temperature has been received, for further
    // processing in the user defined events handling function. Note that the new measured
    // temperature is passed to the post event function.
    MyAppTask_PostEvent(MY_APP_TEMP_INDICATOR_ISR, (void*)&gTemperature);
    QK_ISR_EXIT();
}

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
            printf("----------\r\nEM Microelectronic - EM9305\r\n");

            // Register a temperature indicator callback that will be called once one measurement is complete.
            TI_SetMeasurementCallback(TI_Callback);

            // Check if calibration data exist in info page. If not, then just display a warning for the user
            // to be aware that the measurement will be error prone.
            if (!TI_IsCalibrated())
            {
                printf("Warning: Temperature indicator is not calibrated\r\n");
            }

            // Configure the timer to make one temperature measurement every 2 seconds (0.5 Hz).
            int8_t idx = Timer_SchedulePolling(0.5f, (Driver_Callback_t)Timer_Callback, NULL);
            if(idx < 0)
            {
                // The timer has not been created. Report an error.
                // Note that custom error signals can be created for specific processing. Here,
                // a dedicated timer failure signal is used for a further specific processing.
                printf("Timer error\r\n");
                MyAppTask_PostEvent(MY_APP_TIMER_FAILURE, NULL);
            }
            else
            {
                // Start timer
                Timer_Enable(idx);
                if(!Timer_IsEnabled(idx))
                {
                    // It has not been possible to enable the timer. Report an error.
                    printf("Error, timer not started\r\n");
                    MyAppTask_PostEvent(MY_APP_TIMER_FAILURE, NULL);
                }
            }
        }
        break;

        case MY_APP_TIMER_TICK:
        {
            // This is the timer tick. We can now start a temperature measurement. This is a non blocking function.
            // Once the measurement process is started, the CPU can do something else.
            (void) TI_StartMeasure();
        }
        break;

        case MY_APP_TEMP_INDICATOR_ISR:
        {
            // Reaching this point means that a temperature measurement has been achieved. We can compute the
            // temperature from the frequency jitter, apply calibration correction is available, and display
            // the result on the console.
            float temp = *(float*)pParams;

            // In case of calibration procedure, the LF-RC LP frequency is available in 24.8 fixed point format.
            uint32_t fpFreq = TI_GetFrequency();
            float frequency = (float)(fpFreq >> 8) + (float)((fpFreq & 0xFF) / 256.0f);

            // The API TI_GetLastTemperature(void); can also be used to get the last temperature measurement.
            printf("[%3d] : temperature = %.1f C, frequency = %.3f Hz\r\n", gCounter, temp, frequency);

            // Increment the step gCounter
            gCounter++;
        }
        break;

        case MY_APP_ERROR_SIG:
        {
            // Error!
            printf("Error event received\r\n");
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

        case MY_APP_TIMER_FAILURE:
        {
            printf("Timer general failure. I have to sacrifice the device. Sorry...\r\n");
            COMMON_WaitUs(1000);
            HaltCPU();
        }
        break;

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event
            ret = MY_APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
