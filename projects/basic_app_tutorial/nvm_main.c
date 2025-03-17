////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Application task
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
#include <adc.h>
#include <bsp.h>
#include <pml.h>
#include <uart.h>
#include <gpio.h>
#include <timer.h>
#include <macros.h>
#include <common.h>
#include <nvm_entry.h>
#include <timer_hal.h>
#include <my_app_task.h>

/******************************************************************************\
 *  Definitions
\******************************************************************************/

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     (MY_APP_TASK_EVENTS_QUEUE_SIZE)

/******************************************************************************\
 *  Global variables
\******************************************************************************/

// Reserve some static space for storing events. This is required by QP/C.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[8 * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Init the QPC event pool
 */
static void MAIN_InitEventPool(void);
static void MAIN_ConfigureUART(void);
static void MAIN_SetupGpios();

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules()
{
    // Uncomment the following line to prevent the system to go to sleep.
    // Then rebuild the application example.
    // gPML_Config.sleepModeForbiden = true;

    // Register modules before use
    UART_RegisterModule();
    Timer_RegisterModule();

    // Enable UART device
    gUART_Config.enabled = true;
    gTimer_Config.enabled = true;

    if(PML_DidBootFromSleep())
    {
        // Indicate to QP/C that there is no initialized events pool to resume from.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
    }
    else
    {
        // Set the QP/C framework (QF) in a clean state by initializing all internal variables.
        // This is mandatory when resuming from a POR or a SWreset.
        QF_init();
    }

    MAIN_InitEventPool();

    // Define 'ENABLE_JTAG' to use JTAG.
    #ifdef ENABLE_JTAG
        // Disable sleep mode if JTAG debug is used.
        gPML_Config.sleepModeForbiden = true;

        // Enable JTAG.
        GPIO_EnableJtag4Wires();
    #endif
}

/**
 *
 * @brief Application entry function for this example.
 *
 * This function is the example application entry that will reside in NVM. It is where
 * the application starts.
 *
 */
NO_RETURN void NVM_ApplicationEntry()
{
    // Init the BSP QP/C module.
    BSP_Init();

    // Enable interruptions (seti instruction)
    IRQ_EnableInterrupts();

    // If the device resumes from sleep
    if(PML_DidBootFromSleep())
    {
        // Restart timer
        Timer_Resume();

        // Resume ADC
        ADC_Resume();

        // If we resume from a sleep period, just resume QP/C execution.
        // Cast returned value to void to avoid Linter error.
        (void)QF_resume();
    }
    else
    {
        // Configure the UART (at least Tx) to send messages over the serial line.
        MAIN_ConfigureUART();

        // Configure GPIOs used for this sample application.
        MAIN_SetupGpios();

        // Initialize the ADC by checking the calibration data availability.
        ADC_Init();

        // Set clock source to HF crystal to have better accuracy.
        PML_SetHfClkSourceNonBlocking(PML_HF_CLK_XTAL);

        // Create the QP/C tasks
        MyAppTask_Create();

        // Start the tasks
        MyAppTask_Start();

        // Give control to QP/C
        (void)QF_run();
    }

    // This point should never be reached, otherwise there is a big issue!
    while(true)
    {
        HaltCPU();
    }
}

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static void MAIN_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool), sizeof(QEventParams));
}

static void MAIN_ConfigureUART(void)
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

static void MAIN_SetupGpios(void)
{
    uint8_t gpios[] = {2, 3, 4, 5, 8, 9};

    // Configure some other GPIOs as output with no special functions.
    for(uint8_t index = 0; index <= sizeof(gpios) / sizeof(uint8_t); index++)
    {
        GPIO_DisablePullDown(gpios[index]);
        GPIO_DisableOpenDrain(gpios[index]);
        GPIO_SetOutputPinFunction(gpios[index], GPIO_PIN_FUNC_OUT_GPIO);
        GPIO_DisableInput(gpios[index]);
        GPIO_EnablePullUp(gpios[index]);
        GPIO_SetLow(gpios[index]);
        GPIO_EnableOutput(gpios[index]);
    }
}
