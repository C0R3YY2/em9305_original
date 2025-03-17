////////////////////////////////////////////////////////////////////////////////
///
/// @file       printf_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Simple printf over UART on GPIO 7
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
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

#include "types.h"
#include "macros.h"
#include "nvm_entry.h"
#include "pml.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"
#include "printf.h"
#include "puts.h"

/******************************************************************************\
 *  External variables
\******************************************************************************/

extern volatile UART_Configuration_t gUART_Config;


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Counter
uint32_t g_counter = 0x00;


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Callback for the UART Rx
 */
static void UART_RxCallback(Driver_Status_t status, void* pUserData);


/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    UART_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Do not modify the GPIO config after a wake-up from sleep because
    // the configuration is kept in persistent memory.
    if (!PML_DidBootFromSleep())
    {
        // Update the GPIO default configuration

        // Disable all inputs
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)0x000;

        // Enable inputs on GPIO6 for UART RX, disable output on other pins
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)GPIO_MASK_PIN_6;

        // Enable output on GPIO7 for UART TX, disable output on other pins
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

        // Disable pull-downs on all pins
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

        // Enable pull-up on GPIO6 and GPIO7 only
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = (uint32_t)GPIO_MASK_PIN_6 | (uint32_t)GPIO_MASK_PIN_7;

        // Set the UART TX output function on GPIO7
        gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 = (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

        // Set the UART RX input function on GPIO6
        gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX = (uint8_t)GPIO_PIN_UART_RXD;
    }

    // Enable JTAG.
    GPIO_EnableJtag4Wires();
}


NO_RETURN void NVM_ApplicationEntry(void)
{
    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    const char string[] = "EM Microelectronic";

    // Print EM Microelectronic
    printf("%s\r\n", string);

    // Print EM9305
    printf("EM%d\r\n", 9305);

    const char input[1] = {0};
    if (!UART_ReceiveDataWithCallback((uint8_t*)input, 1, UART_RxCallback, NULL)){
        printf("Error in UART Rx");
    }

    // Infinite loop
    while (true)
    {
        // Wait 1 sec
        COMMON_WaitUs(1000000);

        // Print counter value
        printf("Counter=%u\r\n", g_counter++);
    }
}

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void UART_RxCallback(Driver_Status_t status, void* pUserData)
{
    // reset the counter
    g_counter = 0x00;

    const char input[1] = {0};
    if (!UART_ReceiveDataWithCallback((uint8_t*)input, 1, UART_RxCallback, NULL)){
        printf("Error in UART Rx");
    }
}
