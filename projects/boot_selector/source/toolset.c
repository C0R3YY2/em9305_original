////////////////////////////////////////////////////////////////////////////////
///
/// @file       app_boot_selection/toolset.c
///
/// @project    T9305
///
/// @brief      Show how to jump to application mode.
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


#include "gpio.h"

void configure_gpio_for_uart(void)
{
    // Disable all inputs
    gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)0x000;

    // Enable outputs on GPIO7 for UART TX, disable output on other pins
    gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

    // Disable pull-downs on all pins
    gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

    // Enable pull-up on GPIO7 only
    gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

    // Set the UART TX output function on GPIO7
    gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
        (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

    // Disable the UART RX input function
    gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
        (uint8_t)GPIO_FUNCTION_NOT_MAPPED;

}
