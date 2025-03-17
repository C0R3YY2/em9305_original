////////////////////////////////////////////////////////////////////////////////
///
/// @file       spi_slave_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      SPI slave example with sleep. RDY pin not used
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

#include <boot.h>
#include <bsp.h>
#include <gpio.h>
#include <irq.h>
#include <macros.h>
#include <my_app_task.h>
#include <nvm_entry.h>
#include <pml.h>
#include <spi_slave.h>
#include <types.h>
#include <uart.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

#define QPC_EVENT_POOL_SIZE (MY_APP_TASK_EVENTS_QUEUE_SIZE)

#define GPIO_SPIS_CSN  (11u)
#define GPIO_SPIS_SCK  (8u)
#define GPIO_SPIS_MISO (9u)
#define GPIO_SPIS_MOSI (10u)

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

static void Main_SetupWakeUpByGpio(uint8_t gpio);

static void Main_SetupUartGpio(void);

static void Main_SetupSpiGpio(void);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules
    UART_RegisterModule();
    SPIS_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Enable SPIS
    gSPIS_Config.enabled = true;

    // SPI Slave config
    gSPIS_Config.configBits.bits.msbFirst = 1;
    gSPIS_Config.configBits.bits.cpol = 0;
    gSPIS_Config.configBits.bits.cpha = 0;
    gSPIS_Config.configBits.bits.mode = SPIS_MODE_FULL;

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
        // Post an event to the application task that we woke up from sleep.
        MyAppTask_PostEvent(BOOT_FROM_SLEEP_SIG, NULL);

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Setup GPIOs.
        Main_SetupUartGpio();
        Main_SetupSpiGpio();

        // Configure wake up.
        Main_SetupWakeUpByGpio(GPIO_SPIS_CSN);

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
 *  Private functions definition
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

static void Main_SetupSpiGpio(void)
{
    GPIO_EnableInput(GPIO_SPIS_CSN);
    GPIO_EnableInput(GPIO_SPIS_MOSI);
    GPIO_EnableInput(GPIO_SPIS_SCK);

    GPIO_EnableOutput(GPIO_SPIS_MISO);

    GPIO_SetOutputPinFunction(GPIO_PIN_FUNC_OUT_SPIS_MISO, GPIO_SPIS_MISO);
    GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_SPIS_CSN, GPIO_SPIS_CSN);
    GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_SPIS_SCK, GPIO_SPIS_SCK);
    GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_SPIS_MOSI, GPIO_SPIS_MOSI);
}

static void Main_SetupWakeUpByGpio(uint8_t gpio)
{
    PML_ConfigWakeUpByGpio(gpio, false, true, false);
    PML_RegisterWakeUpAction(PML_WAKEUP_ACTION_RUN_PROT_TIM,
        (uint32_t)SET_BOOT_ACTION_FLAGS_GPIO(1u << gpio), true);
    PML_EnableWakeUpFlags(false);
}
