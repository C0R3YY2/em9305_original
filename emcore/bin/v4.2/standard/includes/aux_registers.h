////////////////////////////////////////////////////////////////////////////////
///
/// @file       common/9305/includes/aux_registers.h
///
/// @project    T9305
///
/// @brief      ARC AUX registers definition and utility functions to read
///             and write the ARC AUX registers.
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

#ifndef _AUX_REGISTERS_H
#define _AUX_REGISTERS_H

#include <types.h>

#ifdef _ARCVER /* Ensure code compiles when not using mcc/ccac. */
#include <emm_arc_reg.h>
#include "apexextensions.h"


typedef struct {
    uint32_t addr;
} ARC_AUX_Register;

#ifndef STACK_REGION_BUILD
#define STACK_REGION_BUILD  (0xC5)
#endif // STACK_REGION_BUILD

#ifndef KSTACK_BASE
#define KSTACK_BASE         (0x265)
#endif // KSTACK_BASE

#ifndef KSTACK_TOP
#define KSTACK_TOP          (0x264)
#endif // KSTACK_TOP


// DMA Controller
// -----------------------------------------------------------------------------
// @todo: This section can be removed once AUX registers are defined in files
// provided by tool (registers should be in arg_reg.h)
 #ifndef DMACTRL
 #define DMACTRL            (0x680)     // r/w  1  DMA controller control/configuration register
 #endif // DMACTRL

 #ifndef DMACENB
 #define DMACENB            (0x681)     // r/w  8  DMA channel enable register
 #endif // DMACENB

 #ifndef DMACDSB
 #define DMACDSB            (0x682)     // w  8  DMA channel disable register
 #endif // DMACDSB

 #ifndef DMACHPRI
 #define DMACHPRI           (0x683)     // r/w  8  DMA channel high priority level register
 #endif // DMACHPRI

 #ifndef DMACNPRI
 #define DMACNPRI           (0x684)     // w  8  DMA channel low priority level register
 #endif // DMACNPRI

 #ifndef DMACREQ
 #define DMACREQ            (0x685)     // w  8  DMA software transfer request
 #endif // DMACREQ

 #ifndef DMASTAT0
 #define DMASTAT0           (0x686)     // r  8  DMA status register 0
 #endif // DMASTAT0

 #ifndef DMASTAT1
 #define DMASTAT1           (0x687)     // r/w  32  DMA status register 1
 #endif // DMASTAT1

 #ifndef DMACIRQ
 #define DMACIRQ            (0x688)     // r/w  8  DMA interrupt status register
 #endif // DMACIRQ

 #ifndef DMACBASE
 #define DMACBASE           (0x689)     // r/w  32  DMA channel base address register
 #endif // DMACBASE

 #ifndef DMACRST
 #define DMACRST            (0x68a)     // r/w  32  DMA channel reset register
 #endif // DMACRST

 #ifndef DMACTRL0
 #define DMACTRL0           (0x690)     // r/w  32  DMA control register for channel 0, doesn't exist, used as an offset only
 #endif // DMACTRL0

 #ifndef DMASAR0
 #define DMASAR0            (0x691)     // r/w  32  DMA source address register for channel 0, doesn't exist, used as an offset only
 #endif // DMASAR0

 #ifndef DMADAR0
 #define DMADAR0            (0x692)     // r/w  32  DMA destination address register for channel 0, doesn't exist, used as an offset only
 #endif // DMADAR0

 #ifndef DMACTRL1
 #define DMACTRL1           (0x693)     // r/w  32  DMA control register for channel 1, doesn't exist, used as an offset only
 #endif // DMACTRL1

 #ifndef DMASAR1
 #define DMASAR1            (0x694)     // r/w  32  DMA source address register for channel 1, doesn't exist, used as an offset only
 #endif // DMASAR1

 #ifndef DMADAR1
 #define DMADAR1            (0x695)     // r/w  32  DMA destination address register for channel 1, doesn't exist, used as an offset only
 #endif // DMADAR1

 #ifndef DMACTRL2
 #define DMACTRL2           (0x696)     // r/w  32  DMA control register for channel 2
 #endif // DMACTRL2

 #ifndef DMASAR2
 #define DMASAR2            (0x697)     // r/w  32  DMA source address register for channel 2
 #endif // DMASAR2

 #ifndef DMADAR2
 #define DMADAR2            (0x698)     // r/w  32  DMA destination address register for channel 2
 #endif // DMADAR2

 #ifndef DMACTRL3
 #define DMACTRL3           (0x699)     // r/w  32  DMA control register for channel 3
 #endif // DMACTRL3

 #ifndef DMASAR3
 #define DMASAR3            (0x69a)     // r/w  32  DMA source address register for channel 3
 #endif // DMASAR3

 #ifndef DMADAR3
 #define DMADAR3            (0x69b)     // r/w  32  DMA destination address register for channel 3
 #endif // DMADAR3

 #ifndef DMACTRL4
 #define DMACTRL4           (0x69c)     // r/w  32  DMA control register for channel 4
 #endif // DMACTRL4

 #ifndef DMASAR4
 #define DMASAR4            (0x69d)     // r/w  32  DMA source address register for channel 4
 #endif // DMASAR4

 #ifndef DMADAR4
 #define DMADAR4            (0x69e)     // r/w  32  DMA destination address register for channel 4
 #endif // DMADAR4

 #ifndef DMACTRL5
 #define DMACTRL5           (0x69f)     // r/w  32  DMA control register for channel 5
 #endif // DMACTRL5

 #ifndef DMASAR5
 #define DMASAR5            (0x6a0)     // r/w  32  DMA source address register for channel 5
 #endif // DMASAR5

 #ifndef DMADAR5
 #define DMADAR5            (0x6a1)     // r/w  32  DMA destination address register for channel 5
 #endif // DMADAR5

 #ifndef DMACTRL6
 #define DMACTRL6           (0x6a2)     // r/w  32  DMA control register for channel 6
 #endif // DMACTRL6

 #ifndef DMASAR6
 #define DMASAR6            (0x6a3)     // r/w  32  DMA source address register for channel 6
 #endif // DMASAR6

 #ifndef DMADAR6
 #define DMADAR6            (0x6a4)     // r/w  32  DMA destination address register for channel 6
 #endif // DMADAR6

 #ifndef DMACTRL7
 #define DMACTRL7           (0x6a5)     // r/w  32  DMA control register for channel 7
 #endif // DMACTRL7

 #ifndef DMASAR7
 #define DMASAR7            (0x6a6)     // r/w  32  DMA source address register for channel 7
 #endif // DMASAR7

 #ifndef DMADAR7
 #define DMADAR7            (0x6a7)     // r/w  32  DMA destination address register for channel 7
 #endif // DMADAR7

 #ifndef DMAC_BUILD
 #define DMAC_BUILD         (0xcd )     // r  32  DMA build configuration register
 #endif // DMAC_BUILD
// -----------------------------------------------------------------------------


/**
 * ARC AUX registers to be used with @ref ReadAUX and @ref WriteAUX.
 */
#define EM_ARC_STATUS32         ((ARC_AUX_Register*)STATUS32)
#define EM_ARC_AUX_IRQ_ACT      ((ARC_AUX_Register*)AUX_IRQ_ACT)
#define EM_ARC_AUX_USER_SP      ((ARC_AUX_Register*)AUX_USER_SP)
#define EM_ARC_ICAUSE           ((ARC_AUX_Register*)ICAUSE)
#define EM_ARC_IRQ_PRIORITY     ((ARC_AUX_Register*)IRQ_PRIORITY)
#define EM_ARC_IRQ_INTERRUPT    ((ARC_AUX_Register*)IRQ_INTERRUPT)
#define EM_ARC_IRQ_SELECT       ((ARC_AUX_Register*)IRQ_SELECT)
#define EM_ARC_INT_VECTOR_BASE  ((ARC_AUX_Register*)REG_INT_VECTOR_BASE)
#define EM_ARC_JLI_BASE         ((ARC_AUX_Register*)JLI_BASE)
#define EM_STACK_REGION_BUILD   ((ARC_AUX_Register*)STACK_REGION_BUILD)
#define EM_KSTACK_BASE          ((ARC_AUX_Register*)KSTACK_BASE)
#define EM_KSTACK_TOP           ((ARC_AUX_Register*)KSTACK_TOP)
#define EM_REG_COUNT0           ((ARC_AUX_Register*)REG_COUNT0)
#define EM_REG_CONTROL0         ((ARC_AUX_Register*)REG_CONTROL0)
#define EM_REG_LIMIT0           ((ARC_AUX_Register*)REG_LIMIT0)
#define EM_REG_COUNT1           ((ARC_AUX_Register*)REG_COUNT1)
#define EM_REG_CONTROL1         ((ARC_AUX_Register*)REG_CONTROL1)
#define EM_REG_LIMIT1           ((ARC_AUX_Register*)REG_LIMIT1)
#define EM_REG_RTC_CTRL         ((ARC_AUX_Register*)AUX_RTC_CTRL)
#define EM_REG_RTC_LOW          ((ARC_AUX_Register*)AUX_RTC_LOW)
#define EM_REG_RTC_HIGH         ((ARC_AUX_Register*)AUX_RTC_HIGH)
#define EM_REG_WDT_PASSWD       ((ARC_AUX_Register*)WDT_PASSWD)
#define EM_REG_WDT_CTRL         ((ARC_AUX_Register*)WDT_CTRL)
#define EM_REG_WDT_PERIOD       ((ARC_AUX_Register*)WDT_PERIOD)
#define EM_REG_WDT_COUNT        ((ARC_AUX_Register*)WDT_COUNT)
#define EM_REG_DMACTRL          ((ARC_AUX_Register*)DMACTRL)
#define EM_REG_DMACENB          ((ARC_AUX_Register*)DMACENB)
#define EM_REG_DMACDSB          ((ARC_AUX_Register*)DMACDSB)
#define EM_REG_DMACHPRI         ((ARC_AUX_Register*)DMACHPRI)
#define EM_REG_DMACNPRI         ((ARC_AUX_Register*)DMACNPRI)
#define EM_REG_DMACREQ          ((ARC_AUX_Register*)DMACREQ)
#define EM_REG_DMASTAT0         ((ARC_AUX_Register*)DMASTAT0)
#define EM_REG_DMASTAT1         ((ARC_AUX_Register*)DMASTAT1)
#define EM_REG_DMACIRQ          ((ARC_AUX_Register*)DMACIRQ)
#define EM_REG_DMACBASE         ((ARC_AUX_Register*)DMACBASE)
#define EM_REG_DMACRST          ((ARC_AUX_Register*)DMACRST)
#define EM_REG_DMACTRL0         ((ARC_AUX_Register*)DMACTRL0)                // Doesn't exist. Used as an offset only.
#define EM_REG_DMASAR0          ((ARC_AUX_Register*)DMASAR0)                 // Doesn't exist. Used as an offset only.
#define EM_REG_DMADAR0          ((ARC_AUX_Register*)DMADAR0)                 // Doesn't exist. Used as an offset only.
#define EM_REG_DMACTRL1         ((ARC_AUX_Register*)DMACTRL1)                // Doesn't exist. Used as an offset only.
#define EM_REG_DMASAR1          ((ARC_AUX_Register*)DMASAR1)                 // Doesn't exist. Used as an offset only.
#define EM_REG_DMADAR1          ((ARC_AUX_Register*)DMADAR1)                 // Doesn't exist. Used as an offset only.
#define EM_REG_DMACTRL2         ((ARC_AUX_Register*)DMACTRL2)
#define EM_REG_DMASAR2          ((ARC_AUX_Register*)DMASAR2)
#define EM_REG_DMADAR2          ((ARC_AUX_Register*)DMADAR2)
#define EM_REG_DMACTRL3         ((ARC_AUX_Register*)DMACTRL3)
#define EM_REG_DMASAR3          ((ARC_AUX_Register*)DMASAR3)
#define EM_REG_DMADAR3          ((ARC_AUX_Register*)DMADAR3)
#define EM_REG_DMACTRL4         ((ARC_AUX_Register*)DMACTRL4)
#define EM_REG_DMASAR4          ((ARC_AUX_Register*)DMASAR4)
#define EM_REG_DMADAR4          ((ARC_AUX_Register*)DMADAR4)
#define EM_REG_DMACTRL5         ((ARC_AUX_Register*)DMACTRL5)
#define EM_REG_DMASAR5          ((ARC_AUX_Register*)DMASAR5)
#define EM_REG_DMADAR5          ((ARC_AUX_Register*)DMADAR5)
#define EM_REG_DMACTRL6         ((ARC_AUX_Register*)DMACTRL6)
#define EM_REG_DMASAR6          ((ARC_AUX_Register*)DMASAR6)
#define EM_REG_DMADAR6          ((ARC_AUX_Register*)DMADAR6)
#define EM_REG_DMACTRL7         ((ARC_AUX_Register*)DMACTRL7)
#define EM_REG_DMASAR7          ((ARC_AUX_Register*)DMASAR7)
#define EM_REG_DMADAR7          ((ARC_AUX_Register*)DMADAR7)
#define EM_REG_DMAC_BUILD       ((ARC_AUX_Register*)DMAC_BUILD)


/**
 * @brief Used to read registers in the Aux register space.
 *
 * This function is a wrapper around _lr. If the register is a host writable
 * register, we read twice to ensure validity.
 *
 * @param reg The register address to read.
 * @returns The value in the specified register.
 */
static uint32_t _Inline ReadAUX(ARC_AUX_Register *reg)
{
    /* Add code for double read if necessary for async registers */
    return (uint32_t)_lr((uint32_t)reg);
}


/**
 * @brief Used to write registers in the Aux register space.
 *
 * This function is a wrapper around _sr.
 *
 * @param value The value to write to the register.
 * @param reg The register address to write to. One of @ref ARC_AUX_Register/
 */
static void _Inline WriteAUX(uint32_t value, ARC_AUX_Register *reg)
{
// If the compiler version is newer than 8.x.x
#if __clang_major__ > 8
    _sr(value, (uint32_t)reg);
#else
    _sr((int)value, (uint32_t)reg);
#endif
}


#endif /* _ARCVER */


#endif /* _AUX_REGISTERS_H */
