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
 *  Forward declaration of functions
\******************************************************************************/

void configure_gpio_for_uart(void);


/******************************************************************************\
 *  Other variable declarations
\******************************************************************************/

// Create a persistent variable. It will keep its latest value through sleep and
// wake-up cycles. Note the "SECTION_PERSISTENT" statement defined in macro.h which
// forces the variable to be mapped in a section that is stored in a persistent
// block of RAM.
SECTION_PERSISTENT uint16_t stage = 0;


/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/


void NVM_ConfigModules(void)
{
    // Register the modules.
    UART_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Are we resuming from a CPU reset?
    if(PML_DidBootFromReset() || PML_DidBootFromPOR()) {
        // Configure GPIOs for being able to output data on the serial line to the
        // host computer (printf calls will then be sent to the serial line).
        configure_gpio_for_uart();
    }

    // If we resume from a power on cycle (power off to power on), then we set initial
    // value for persistent variables. For other types of power on (like CPU reset),
    // then this block will not be executed.
    if(PML_DidBootFromPOR()) {
        // Set stage initial value. This counter is incremented over the CPUreset cycles.
        stage = 0;
    }

    // Enable JTAG.
    GPIO_EnableJtag4Wires();
}


NO_RETURN void NVM_ApplicationEntry(void)
{
    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // The counter, which value will be sent through the serial line.
    uint32_t counter = 0x00;

    // Copyright message ;)
    const char string[] = "EM Microelectronic - Boot selector";

    // Print EM Microelectronic
    printf("%s\r\n", string);

    // Print EM9305
    printf("EM%d\r\n", 9305);

    // Read the stored boot configuration and concatenete all bits into one single word:
    // bit 0: application mode flag
    // bit 1: bootloader mode flag
    // bit 2: EMCore mode flag
    uint16_t mode = (uint16_t)PML_GetApplicationModeFlag() |
                    (uint16_t)(PML_GetBootloaderModeFlag() << 1) |
                    (uint16_t)(PML_GetEmCoreModeFlag() << 2);

    // Print this status
    printf("Mode flag is: %d\r\n", mode);

    // Infinite loop
    while(true)
    {
        // Wait 1 sec (parameter is s)
        COMMON_WaitUs(1000000);

        // Print counter value
        printf("Counter=%u, stage=%d\r\n", counter++, stage);

        // After 20 seconds, jump to another mode.
        if (counter == 20) {
            // Clear all mode flags. They have been memorized earlier in the "mode" variable.
            PML_ClearModeFlag();

            // If we come from nowhere (mode == 0) or from application (mode == 1), then restart
            // in bootloader mode.
            if(mode == 0 || mode == 1) {
                // Configure next boot to be configuration mode.
                PML_SetApplicationModeFlag();
            }
            // If we come from bootloader, then restart in application mode.
            if(mode == 2) {
                // Configure next boot to be application.
                PML_SetBootloaderModeFlag();
            }

            // Increment the stage counter.
            stage++;

            // Reset the CPU for the reboot to be triggered.
            ResetCPU();

            // After the reset, the counter should restart from 0 but the stage shall be incremented.
        }
    }
}
