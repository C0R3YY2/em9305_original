////////////////////////////////////////////////////////////////////////////////
///
/// @file       common/9305/includes/arc.h
///
/// @project    T9305
///
/// @brief      Main arc include file, standard macros for
///             manipulating interrupts, registers, and cpu states.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2015 EM Microelectronic
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

#ifndef _ARC_H
#define _ARC_H

#include <types.h>
#include <hw_versions.h>
#include <aux_registers.h>
#include <interrupts.h>

/// Clock frequency the ARC is running at (in MHz).
#define CLOCK_FREQ_MHZ (24) // MHz

/// Clock frequency the ARC is running at (in Hz).
#define CLOCK_FREQ (CLOCK_FREQ_MHZ * 1000000)

/**
 * @fn      void HaltCPU();
 * @brief   Cause the CPU to enter a HALT state.
 *
 * NOTE: The ARC will ignore the halt instruction if the CPURunReq bit is set.
 */
static void _Inline HaltCPU(void)
{
    /* Set the CPU Halt bit */
#if _ARCVER == ARCV2_EM4_CORE0
    __asm__("flag   1");
#elif _ARCVER == ARCV2_EM4_CORE1
    __asm__("kflag  1");
#elif _ARCVER == ARCV2_EM4_CORE4
    __asm__("kflag  1");
#else
#error Unknown CPU version _ARCVER
#endif
}

static void _Inline _nop(void)
{
    __asm__("nop");
}

/**
 * @fn      void SleepCPU();
 * @brief   Cause the CPU to enter a sleep state.
 *
 * NOTE: This function does not modify interrupt enables.
 */
static void _Inline SleepCPU(void)
{
    /* Issue a sleep instruction */
#if _ARCVER == ARCV2_EM4_CORE0
    __asm__("sleep");
#elif _ARCVER == ARCV2_EM4_CORE1
    // allow interrupt levels 1 and 2 to wake us up.
    //__asm__("sleep    0x12");

    // Don't change interrupt enables.
    __asm__("sleep");
#elif _ARCVER == ARCV2_EM4_CORE4
    // allow interrupt levels 1 and 2 to wake us up.
    //__asm__("sleep    0x12");

    // Don't change interrupt enables.
    __asm__("sleep");
#else
#error Unknown CPU version _ARCVER
#endif
}

/**
 * @brief Reset the CPU by calling the reset vector entry.
 */
void NO_RETURN ResetCPU(void);

/**
 * @brief Mask for user defined sleep mode bits.
 */
#define USER_DEFINED_SLEEP_MODE_MASK (0x07)

/**
 * @brief Bit shift value for user defined sleep mode bits.
 */
#define USER_DEFINED_SLEEP_MODE_SHIFT (5)

/**
 * @brief Cause the chip to enter a sleep mode.
 * @param mode Sleep mode for the chip to enter.
 */
static void ALWAYS_INLINE SleepWithMode(uint32_t mode)
{
    __asm__("sleep %0" : : "r"((mode & USER_DEFINED_SLEEP_MODE_MASK)
        << USER_DEFINED_SLEEP_MODE_SHIFT) : "memory");

}

/**
 * @breif Atomic exchange of a value into a memory address.
 * @param value Value to place into the memory.
 * @param pAddress Pointer to the memory location to replace the value in.
 * @returns Value that was at memory address before the exchange.
 */
static uint32_t ALWAYS_INLINE _xchg(uint32_t value, volatile uint32_t *pAddress)
{
    __asm__ volatile (
        "ex %0, [%1]"
        : "=r" (value),"=r"(pAddress)
        : "0" (value), "1" (pAddress)
        : "memory" );

    return value;
}

#include <interrupts.h>


#endif /* _ARC_H */
