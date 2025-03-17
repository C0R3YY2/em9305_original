////////////////////////////////////////////////////////////////////////////////
///
/// @file       ti_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Demonstrate how to use the Temperature Indicator driver with printf
///             over UART on GPIO 7
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic
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

#include <bsp.h>
#include <pml.h>
#include <uart.h>
#include <gpio.h>
#include <types.h>
#include <timer.h>
#include <macros.h>
#include <nvm_entry.h>
#include <timer_hal.h>
#include <temperature_indicator.h>

#include <my_app_task.h>

/******************************************************************************\
 *  Definitions
\******************************************************************************/

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     (MY_APP_TASK_EVENTS_QUEUE_SIZE)

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// The array that will be used to store the QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Init the QPC event pool
 */
static void Main_InitEventPool(void);

static void Main_SetupUartGpio();

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    UART_RegisterModule();
    Timer_RegisterModule();
    TI_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Enable unitimer.
    gTimer_Config.enabled = true;

    if (PML_DidBootFromSleep())
    {
        // Re-initialize the QPC event pool if we resume from a sleep period.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
    }
    else
    {
        // Initialize QPC
        QF_init();
    }

    // Initialize the event pool that will be used by QP/C to process events.
    // See this function at the end of this file.
    Main_InitEventPool();

    // Define 'ENABLE_JTAG' to use JTAG.
    #ifdef ENABLE_JTAG
        // Disable sleep mode if JTAG debug is used.
        gPML_Config.sleepModeForbiden = true;

        // Enable JTAG.
        GPIO_EnableJtag4Wires();
    #endif
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // Check if it is wake-up from sleep.
    if (PML_DidBootFromSleep())
    {
        // Resume unitimer
        Timer_Resume();

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Configure GPIOs
        Main_SetupUartGpio();

        // Create the user tasks
        MyAppTask_Create();

        // Start execution of the user tasks
        MyAppTask_Start();

        // and, last step, run QPC!
        (void)QF_run();
    }

    // If everything works perfectly, we should never reach the statements below.
    while (TRUE)
    {
        HaltCPU();
    }
}

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/


static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool), sizeof(QEventParams));
}


static void Main_SetupUartGpio(void)
{
    // Assign GPIO7 (Tx) to UART Txd function
    GPIO_SetOutputPinFunction(GPIO_PIN_UART_TXD, GPIO_PIN_FUNC_OUT_UART_TXD);
    // Disable GPIO6 (Rx)
    GPIO_SetInputFunctionPin(GPIO_PIN_UART_RXD, GPIO_FUNCTION_NOT_MAPPED);
    // Enable pull-up resistor on Tx pin
    GPIO_EnablePullUp(GPIO_PIN_UART_TXD);
    // Enable output for Tx pin
    GPIO_EnableOutput(GPIO_PIN_UART_TXD);
}
