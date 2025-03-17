////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware - EMB ANCS application
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

#include <my_app_task.h>
#include <ble_task.h>
#include <bsp.h>
#include <gpio.h>
#include <nvm_entry.h> // Must be included to add the header of the firmware.
#include <pml.h>
#include <radio.h>
#include <types.h>
#include <irq_user_isr.h>
#include <common.h>
#include <uart.h>

// Choose the GPIO pin to set up for button presses
#define BUTTON_GPIO 5

#define QPC_EVENT_POOL_SIZE (BLE_TASK_EVENTS_QUEUE_SIZE + MY_APP_TASK_EVENTS_QUEUE_SIZE)

extern void ButtonHandler(void);

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static void Main_SetupUartTxPin(uint8_t gpio);
static void Main_SetupUartRxPin(uint8_t gpio);
static void Main_SetupButtonPin(uint8_t pin);
static void Button_IRQUserHandler_GPIO(uint8_t gpio);
static void Main_InitEventPool(void);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    RF_RegisterModule();

    // Configured by default in this call to 115.2k 8N1,
    // FIFOs are flushed and IRQs are cleared (uart.c)
    UART_RegisterModule();
    gUART_Config.enabled = true;

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

    // Disable sleep mode if JTAG debug is used
    // Or if you want to quickly receive UART characters
    gPML_Config.sleepModeForbiden = true;

    // Enable JTAG.
    // GPIO_EnableJtag2Wires();
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // On wakeup from sleep, no need to reconfigure things since it is stored in NV RAM
    if (PML_DidBootFromSleep())
    {
        // Resume QPC.
        (void)QF_resume();
    }
    else
    {
        // Init UART I/O
        Main_SetupUartTxPin(7);     // GPIO7
        Main_SetupUartRxPin(6);     // GPIO6

        // Init button input
        Main_SetupButtonPin(BUTTON_GPIO);
        IRQ_SetIRQUserHandlerGPIO(Button_IRQUserHandler_GPIO);

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

/*
 * Configure the UART transmit pin
 *
 */
static void Main_SetupUartTxPin(uint8_t gpio)
{
    // only gpio 0, 1, 2, 3, 7 and 8 can be used as UART Tx.
    if ((gpio <= 3) || (gpio == 7) || (gpio == 8))
    {
        // Configure GPIO as UART Tx
        GPIO_DisableInput(gpio);
        GPIO_DisablePullDown(gpio);
        // Shouldn't need the following but it is in 19.5.1 of the SoC Implementers Guide v2.0
        GPIO_EnablePullUp(gpio);
        GPIO_SetOutputPinFunction(gpio, GPIO_PIN_FUNC_OUT_UART_TXD);
        GPIO_EnableOutput(gpio);
    }
}

/*
 * Configure the UART receive pin
 *
 */
static void Main_SetupUartRxPin(uint8_t gpio)
{
    // only gpio 0, 1, 2, 3, 4 and 6 can be used as UART Rx.
    if ((gpio <= 4) || (gpio == 6))
    {
        // Configure GPIO as UART Rx
        GPIO_DisableOutput(gpio);
        GPIO_DisablePullDown(gpio);
        GPIO_EnablePullUp(gpio);
        GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_UART_RXD, gpio);
        GPIO_EnableInput(gpio);
    }
}

/*
 * Configure a GPIO to trigger on edges taken to ground, ideally by a momentary push button
 *
 */
static void Main_SetupButtonPin(uint8_t pin)
{
    GPIO_DisableOutput(pin);

    // Button down = low, closed = high
    GPIO_DisablePullDown(pin);
    GPIO_EnablePullUp(pin);         // Because a positive user action is taking this to ground
    GPIO_SetPolarityFalling(pin);   // This sets the IRQ edge trigger

    // Setup the debounce
    (void) GPIO_SetMinimumDebounce(4096);  // 4.1ms
    GPIO_EnableDebouncer(pin);
    GPIO_EnableInput(pin);

    // CPU wake up on button press
    PML_ConfigWakeUpByGpio((uint8_t)pin, false, true, true);
    PML_RegisterWakeUpAction(PML_WAKEUP_ACTION_RUN_HF_XTAL, (uint32_t) GPIO_MASK(pin), true);
}

/*
 * This isn't the actual ISR but it is called from it so treat things as so.
 *
 */
static void Button_IRQUserHandler_GPIO(uint8_t gpio)
{
    QK_ISR_ENTRY();

    bool gpioVal;
    gpioVal = GPIO_GetValue(BUTTON_GPIO);

    if(gpioVal == false)
    {
        // Call over to app layer with this event
        ButtonHandler();
    }

    QK_ISR_EXIT();
}



static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}
