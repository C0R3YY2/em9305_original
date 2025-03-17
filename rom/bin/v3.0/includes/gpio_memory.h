////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/gpio/includes/gpio_memory.h
///
/// @project    T9305
///
/// @brief      GPIO driver memory.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
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


#ifndef _GPIO_MEMORY_H
#define _GPIO_MEMORY_H


#include <types.h>
#include <t9305_gpio.h>
#include <t9305_pml.h>


/**
 * @brief Persistent memory for the GPIO module.
 */
typedef struct
{
    /// GPIO debouncer configuration (in PML part)
    RegGPIOCfgDeb_t         RegGPIOCfgDeb;
    /// GPIO input enable
    RegGPIOInputEn_t        RegGPIOInputEn;
    /// GPIO output enable
    RegGPIOOutputEn_t       RegGPIOOutputEn;
    /// GPIO pull up enable
    RegGPIOPuEn_t           RegGPIOPuEn;
    /// GPIO pull down enable
    RegGPIOPdEn_t           RegGPIOPdEn;
    /// GPIO open drain enable
    RegGPIOOdEn_t           RegGPIOOdEn;
    /// GPIO output switch matrix selection 0: P0-P3
    RegGPIOOutSel0_t        RegGPIOOutSel0;
    /// GPIO output switch matrix selection 1: P4-P7
    RegGPIOOutSel1_t        RegGPIOOutSel1;
    /// GPIO output switch matrix selection 2: P8-P11
    RegGPIOOutSel2_t        RegGPIOOutSel2;
    /// GPIO input switch matrix selection 0
    RegGPIOInpSel0_t        RegGPIOInpSel0;
    /// GPIO input switch matrix selection 1
    RegGPIOInpSel1_t        RegGPIOInpSel1;
    /// GPIO input switch matrix selection 2
    RegGPIOInpSel2_t        RegGPIOInpSel2;
    /// GPIO input switch matrix selection 3
    RegGPIOInpSel3_t        RegGPIOInpSel3;
    /// GPIO data output to pins
    RegGPIODataOut_t        RegGPIODataOut;
    /// GPIO IRQ polarity
    RegGPIOIRQPolarity_t    RegGPIOIRQPolarity;
    /// GPIO configuration
    RegGPIOCfg_t            RegGPIOCfg;
}  GPIO_PersistentMemory_t;


// This is defined as an extern to a uint32_T array to prevent it's use within
// the module. Using this within the module would make the memory address
// hardcoded within the ROM.
#ifndef GPIO_MEMORY_INTERNAL


/**
 * @brief Reference to the persistent memory for the GPIO module.
 * @note Do not use this directly.
 */
extern char gGPIO_PersistentMemory[];


#endif // GPIO_MEMORY_INTERNAL


#endif /* _GPIO_MEMORY_H */
