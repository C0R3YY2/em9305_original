////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm/timer_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Timer example to demonstrate how to use the one shot timer
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
/// This example starts every second a one shot timer of 200ms. The timer toggles GPIO6
/// when expired.
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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

#include "pml.h"
#include "gpio.h"
#include "nvm_entry.h"
#include "common.h"
#include "timer.h"
#include "timer_hal.h"

/******************************************************************************\
 *  Definitions
\******************************************************************************/

/// Timer1 GPIO
#define TIMER1_GPIO (6)

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

/**
 * @brief Timer1 callback
 *
 * This callback is called when the timer1 triggers an interrupt.
 */
static void Timer1_Callback(Driver_Status_t status, void *pUserData)
{
    GPIO_Toggle(TIMER1_GPIO);
}



/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    Timer_RegisterModule();

    // Enable unitimer.
    gTimer_Config.enabled = true;
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // Check if it is wake-up from sleep.
    if (PML_DidBootFromSleep())
    {
        // Resume unitimer
        Timer_Resume();
    }
    else
    {
        // Configure TIMER1_GPIO
        GPIO_DisablePullDown(TIMER1_GPIO);
        GPIO_DisablePullUp(TIMER1_GPIO);
        GPIO_DisableInput(TIMER1_GPIO);
        GPIO_EnableOutput(TIMER1_GPIO);
        GPIO_SetOutputPinFunction(TIMER1_GPIO, GPIO_PIN_FUNC_OUT_GPIO);
        GPIO_SetLow(TIMER1_GPIO);
    }

    // endless loop
    while(1)
    {
        // Put the GPIO to 1
        GPIO_Toggle(TIMER1_GPIO);

        // Start the timer to put gio to 0 after 200 ms
        (void)Timer_OneshotDelay(200, Timer1_Callback, NULL);

        // Wait 1 sec
        COMMON_WaitUs(1000000);
    }

}
