////////////////////////////////////////////////////////////////////////////////
///
/// @file       rtc_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Demonstrate how to use the RTC driver
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
#include <rtc.h>
#include <types.h>
#include <uart.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Use LF-XTAL with calibration
//#define USE_LF_XTAL_WITH_CALIBRATION

/// Use LF-XTAL without calibration
//#define USE_LF_XTAL

#ifdef USE_LF_XTAL_WITH_CALIBRATION
#define USE_LF_XTAL
#endif

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

static void Main_RtcCallback(RTC_CallbackReason_t reason);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules
    UART_RegisterModule();
    RTC_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Configure RTC
    gRTC_Config.callback = Main_RtcCallback;
    gRTC_Config.sleepTimerChannel = SLEEP_TIMER_CHANNEL_3;

#ifdef USE_LF_XTAL
    // Configure PML to use LF-XTAL
    gPML_Config.lfClkSourceType     = (uint8_t)PML_LF_CLK_XTAL_EN;
    gPML_Config.lfClkFreqRatioHf    = 46875;    // 48MHz & 32.768kHz
    gPML_Config.lfClkFreqRatioLf    = 64;
    gPML_Config.lfClkSourceAccuracy = 150;      // 150ppm
    gPML_Config.rcCalibSkip         = true;     // Disable calibration
#ifdef USE_LF_XTAL_WITH_CALIBRATION
    // Use LF-XTAL with calibration
    gPML_Config.lfClkSourceAccuracy = 50;       // 50ppm
    gPML_Config.rcCalibSkip         = false;    // Do not skip calibration
    gPML_Config.rcCalibHfLimit      = 0;
    gPML_Config.rcCalibLfLimit      = 38;       // 1.2 ms @32 kHz
    gPML_Config.rcCalibPeriod       = 16384;    // 500ms @32kHz
    gPML_Config.rcCalibCorrection   = 0;        // 0ppm
#endif
#endif

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

static void Main_RtcCallback(RTC_CallbackReason_t reason)
{
    QK_ISR_ENTRY();

    // Post an event to the task.
    MyAppTask_PostEvent(RTC_EVENT_SIG, (void*)reason);

    QK_ISR_EXIT();
}
