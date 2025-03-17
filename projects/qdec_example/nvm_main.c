////////////////////////////////////////////////////////////////////////////////
///
/// @file       qdec_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Demonstrate how to use the QDEC driver
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

#include <bsp.h>
#include <gpio.h>
#include <macros.h>
#include <my_app_task.h>
#include <nvm_entry.h>
#include <pml.h>
#include <qdec.h>
#include <qf.h>
#include <stdint.h>
#include <types.h>
#include <uart.h>

#if defined(USE_SINGLE_MODE)
# include <timer.h>
# include <timer_hal.h>
#endif

/******************************************************************************\
 *  Constants
\******************************************************************************/

#define QPC_EVENT_POOL_SIZE (MY_APP_TASK_EVENTS_QUEUE_SIZE)

#define GPIO_UART_TX (7u)

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static void Main_InitEventPool(void);

static void Main_SetupUartGpio(void);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // In case we compile the example code using single mode
    // we use a timer to do a polling of the QDEC.
#if defined(USE_SINGLE_MODE)
    // Register the modules.
    Timer_RegisterModule();

    // Enable unitimer.
    gTimer_Config.enabled = true;
#endif

    // Register the modules.
    QDEC_RegisterModule();
    UART_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Enable QDEC.
    gQDEC_Config.enabled = true;
    gQDEC_Config.configBits.bits.led_en = 0; // Led Disabled
    gQDEC_Config.configBits.bits.led_pol = 0;
    gQDEC_Config.configBits.bits.debounce_en = 1;
    gQDEC_Config.configBits.bits.wakeup_rpt_rdy_en = 1;
    gQDEC_Config.configBits.bits.wakeup_ovf_en = 1;
    gQDEC_Config.configBits.bits.wakeup_dbl_mvt_en = 1;

    gQDEC_Config.pinBits.bits.phase_a = 9;  // GPIO 9
    gQDEC_Config.pinBits.bits.phase_b = 11; // GPIO 11
    gQDEC_Config.pinBits.bits.led = 5;      // GPIO 5

    gQDEC_Config.periodBits.bits.sample_period = (uint32_t)SAMPLING_PERIOD_512_US;
    gQDEC_Config.periodBits.bits.report_period = (uint32_t)REPORT_PERIOD_80_SAMPLE;

    gQDEC_Config.ledTimeBits.bits.led_time_on = 16;

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
    // GPIO_EnableJtag4Wires();
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
#if defined(USE_SINGLE_MODE)
        // Resume unitimer
        Timer_Resume();
#endif

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Setup GPIOs.
        Main_SetupUartGpio();

        // Set HF clock source Xtal.
        PML_SetHfClkSourceNonBlocking(PML_HF_CLK_XTAL);

        // Create and start the application task.
        MyAppTask_Create();
        MyAppTask_Start();

        // Run QPC
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

static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}

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

static void Main_SetupUartGpio(void)
{
    Main_SetupUartTxPin(GPIO_UART_TX);
    // Ensure UART RX is not mapped to a GPIO.
    GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_UART_RXD, GPIO_FUNCTION_NOT_MAPPED);
}
