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
#include <adc.h>
#include <pml.h>
#include <uart.h>
#include <gpio.h>
#include <common.h>
#include <printf.h>
#include <timer.h>
#include <timer_hal.h>
#include <qep.h>
#include <qf_port.h>

#include "my_app.h"
#include <my_app_signals.h>
#include <my_app_task.h>

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// An ADC conversions counter
static uint16_t gCounter = 0;

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void MyApp_MainTimerCallback(Driver_Status_t status, void* pUserData);
static void MyApp_ADCTimerCallback(Driver_Status_t status, void* pUserData);
static void MyApp_ADCConversionCallback(Driver_Status_t status, void* pUserData);
static void MyApp_ConfigSourceVBAT1(void);

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/
/**
 * @brief timer callback
 *
 * This function is called when the timer triggers an interrupt.
 */
static void MyApp_MainTimerCallback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();
    // Post the timer event
    MyAppTask_PostEvent(MY_APP_TIMER_TICK, NULL);
    QK_ISR_EXIT();
}

/**
 * @brief ADC timer callback
 *
 * This function is called at ADC timer tick.
 *
 */
static void MyApp_ADCTimerCallback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();
    MyAppTask_PostEvent(MY_APP_ADC_TIMER_TICK, NULL);
    QK_ISR_EXIT();
}

/**
 * @brief ADC conversion callback
 *
 * This function is called at the end of an ADC conversion.
 */
static void MyApp_ADCConversionCallback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();
    MyAppTask_PostEvent(MY_APP_ADC_CONVERSION_DONE, NULL);
    QK_ISR_EXIT();
}

/**
 * @brief Configures the ADC
 *
 * Configures the ADC with the source voltage, the sampling rate frequency and the data width.
 *
 */
static void MyApp_ConfigSourceVBAT1(void)
{
    // Configure sampling source
    ADC_SetSourceSelection(ADC_SOURCE_VBAT1);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    int8_t timer1, timer2;
    uint16_t value;

    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            printf("----------\r\nEM Microelectronic - EM9305\r\n");

            // Create a timer once with a frequency of 10Hz.
            timer1 = Timer_SchedulePolling(10.0f, (Driver_Callback_t)MyApp_MainTimerCallback, NULL);
            // If the timer has been successfully created, we can start it.
            if(timer1 >= 0)
            {
                // Start the timer.
                Timer_Enable(timer1);
            }
            else
            {
                // An error occurred when creating timer 1. Send an error signal that will be
                // processed in the idle task.
                // The printf statement is just for informational purpose. It is not mandatory.
                printf("Timer 1 creation error\r\n");
                MyAppTask_PostEvent(MY_APP_TIMER_FAILURE, NULL);
            }

            // Create a timer to handle ADC measurement on a periodic basis.
            timer2 = Timer_SchedulePolling(0.5f, (Driver_Callback_t)MyApp_ADCTimerCallback, NULL);
            if(timer2 >= 0)
            {
                Timer_Enable(timer2);
            }
            else
            {
                // Same error management as above.
                printf("Timer 2 creation error\r\n");
                MyAppTask_PostEvent(MY_APP_TIMER_FAILURE, NULL);
            }

            // Force GPIO to 0.
            GPIO_SetLow(8);

            // Select VBat1 as the source for ADC measurement.
            MyApp_ConfigSourceVBAT1();

            // Enable the ADC for being able to make measurement.
            ADC_Enable();

            // Register the callback that the ADC driver will call when
            // a voltage sampling is completed.
            ADC_RegisterCallback(MyApp_ADCConversionCallback);

            // Toggle GPIO10 3 times with 100 us in between.
            for(uint8_t i = 0; i < 3; i++)
            {
                GPIO_Toggle(10);
                COMMON_WaitUs(100);
            }
        }
        break;

        case MY_APP_TIMER_TICK:
            GPIO_Toggle(2);
            // Nothing special to be done here. This is just for fun.
        break;

        case MY_APP_ADC_TIMER_TICK:
            printf("[%3d] ADC sampling...", gCounter);
            GPIO_Toggle(3);
            ADC_StartWithCallback();
        break;

        case MY_APP_ADC_CONVERSION_DONE:
            // Get the read voltage in raw data
            ADC_GetValue(&value);
            // Convert the raw value to millivolts and display it
            printf(" %d mV\r\n", ADC_ValueToMillivolt(value));
            // Toggle a GPIO
            GPIO_Toggle(3);
            // Increment the ADC number of conversions
            gCounter++;
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
