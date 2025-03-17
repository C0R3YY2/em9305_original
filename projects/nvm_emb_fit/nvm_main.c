////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware - EMB FIT application
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-2024, EM Microelectronic
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

#include <ble_task.h>
#include <bsp.h>
#include <gpio.h>
#include <my_app_task.h>
#include <nvm_entry.h> // Must be included to add the header of the firmware.
#include <pml.h>
#include <radio.h>
#include <types.h>

#ifdef PRINT_LTK
#include <uart.h>
#endif


/******************************************************************************\
 *  Constants
\******************************************************************************/

#define QPC_EVENT_POOL_SIZE (BLE_TASK_EVENTS_QUEUE_SIZE + MY_APP_TASK_EVENTS_QUEUE_SIZE)

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

#ifdef PRINT_LTK
static void Main_SetupUartTxPin(uint8_t gpio);
#endif

static void Main_InitEventPool(void);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    RF_RegisterModule();

#ifdef PRINT_LTK
    UART_RegisterModule();
#endif // PRINT_LTK

    // Check if it wakes up from sleep.
    if (PML_DidBootFromSleep())
    {
       // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
    }
    else
    {
        // Initialize QPC
        QF_init();
    }

    // Initialize the QPC event pool.
    Main_InitEventPool();

    // Disable sleep mode if JTAG debug is used.
    // gPML_Config.sleepModeForbiden = true;

    // Enable JTAG.
    // GPIO_EnableJtag2Wires();
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    if (PML_DidBootFromSleep())
    {
        // Resume QPC.
        (void)QF_resume();
    }
    else
    {
#ifdef PRINT_LTK
        Main_SetupUartTxPin(7);
#endif // PRINT_LTK

        // Create and start the Bluetooth task.
        BleTask_Create();
        BleTask_Start();

        // Create and start the application task.
        // The application task must be started after the BLE task is started.
        MyAppTask_Create();
        MyAppTask_Start();

        // Run QPC.
        (void)QF_run();
    }

    // Initialization failed!
    while (true)
    {
        HaltCPU();
    }
}


/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

#ifdef PRINT_LTK
static void Main_SetupUartTxPin(uint8_t gpio)
{
    // only gpio 0, 1, 2, 3, 7 and 8 can be used as UART Tx.
    if ((gpio <= 3) || (gpio == 7) || (gpio == 8))
    {
        // Configure GPIO as UART Tx
        GPIO_DisableInput(gpio);
        GPIO_DisablePullDown(gpio);
        GPIO_EnablePullUp(gpio);
        GPIO_SetOutputPinFunction(gpio, GPIO_PIN_FUNC_OUT_UART_TXD);
        GPIO_EnableOutput(gpio);
    }
}
#endif

static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}
