////////////////////////////////////////////////////////////////////////////////
///
/// @file       common/9305/includes/irq.h
///
/// @project    T9305
///
/// @brief      IRQ drivers
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

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <types.h>
#include <memory_map.h>
#include <hw_versions.h>
#include <aux_registers.h>
#include <t9305_periph.h>
#include <string.h>

/**
 * @addtogroup IRQ
 * @{
 */

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// Number of bytes that the IVT address must be aligned to.
#define IRQ_IVT_ALIGNMENT               (1024)

/**
 * @name IRQ constants
 * @{
 */
#define IRQ_NUMBER_OF_INTERRUPTS        50      /**< Number of interrupt vectors */
#define IRQ_NUMBER_OF_PRIORITIES        10      /**< Number of interrupt priorities */
#define IRQ_NUMBER_OF_ARC_INTERRUPTS    5       /**< Number of ARC interrupt */
/** @} */

#if _ARCVER == ARCV2_EM4_CORE0
#define INTERRUPT_OFFSET                3
#elif _ARCVER == ARCV2_EM4_CORE1
#define INTERRUPT_OFFSET                16
#elif _ARCVER == ARCV2_EM4_CORE4
#define INTERRUPT_OFFSET                16
#else
#error Unknown CPU version _ARCVER
#endif

/**
 * @name Interrupt Priority Levels
 * @{
 */
#define PRIORITY_0                      0
#define PRIORITY_1                      1
#define PRIORITY_2                      2
#define PRIORITY_3                      3
#define PRIORITY_4                      4
#define PRIORITY_5                      5
#define PRIORITY_6                      6
#define PRIORITY_7                      7
#define PRIORITY_8                      8
#define PRIORITY_9                      9
#define PRIORITY_M                      255
#define PRIORITY_LOWEST                 PRIORITY_9
/** @} */

/**
 * @name IRQ vector numbers
 * @{
 */
#define IRQ_ARC_TIMER_0                 0       /**< Interrupt number for ARC Timer 0 */
#define IRQ_ARC_TIMER_1                 1       /**< Interrupt number for ARC Timer 1 */
#define IRQ_ARC_WATCHDOG                2       /**< Interrupt number for ARC Watchdog */
#define IRQ_ARC_DMA_DONE                3       /**< Interrupt number for ARC DMA done */
#define IRQ_ARC_DMA_ERROR               4       /**< Interrupt number for ARC DMA error */

#define IRQ_PROTOCOL_TIMER_OUT_CMP_0    5       /**< Interrupt number for Protocol Timer: Output compare 0 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_1    6       /**< Interrupt number for Protocol Timer: Output compare 1 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_2    7       /**< Interrupt number for Protocol Timer: Output compare 2 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_3    8       /**< Interrupt number for Protocol Timer: Output compare 3 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_4    9       /**< Interrupt number for Protocol Timer: Output compare 4 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_5    10      /**< Interrupt number for Protocol Timer: Output compare 5 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_6    11      /**< Interrupt number for Protocol Timer: Output compare 6 */
#define IRQ_PROTOCOL_TIMER_OUT_CMP_7    12      /**< Interrupt number for Protocol Timer: Output compare 7 */
#define IRQ_PROTOCOL_TIMER_FULL_VAL     13      /**< Interrupt number for Protocol Timer: Full value */
#define IRQ_PROTOCOL_TIMER_SYNC         14      /**< Interrupt number for Protocol Timer: Synchronous start/stop */

#define IRQ_SLEEP_TIMER_OUT_CMP_0       15      /**< Interrupt number for Sleep Timer: Output compare 0 */
#define IRQ_SLEEP_TIMER_OUT_CMP_1       16      /**< Interrupt number for Sleep Timer: Output compare 1 */
#define IRQ_SLEEP_TIMER_OUT_CMP_2       17      /**< Interrupt number for Sleep Timer: Output compare 2 */
#define IRQ_SLEEP_TIMER_OUT_CMP_3       18      /**< Interrupt number for Sleep Timer: Output compare 3 */
#define IRQ_SLEEP_TIMER_FULL_VAL        19      /**< Interrupt number for Sleep Timer: Full value */

#define IRQ_RADIO_TX                    20      /**< Interrupt number for Radio: TX operations */
#define IRQ_RADIO_RX                    21      /**< Interrupt number for Radio: RX operations */

#define IRQ_SPI_SLAVE_TX                22      /**< Interrupt number for SPI Slave: TX */
#define IRQ_SPI_SLAVE_RX                23      /**< Interrupt number for SPI Slave: RX */

#define IRQ_UART_TX                     24      /**< Interrupt number for UART: TX */
#define IRQ_UART_RX                     25      /**< Interrupt number for UART: RX */

#define IRQ_GPIO                        26      /**< Interrupt number for GPIO 0 - 11 */

#define IRQ_UNIVERSAL_TIMER_2           27      /**< Interrupt number for Universal Timer 2 */
#define IRQ_UNIVERSAL_TIMER_3           28      /**< Interrupt number for Universal Timer 3 */

#define IRQ_SPI_MASTER                  29      /**< Interrupt number for SPI Master: TX */

#define IRQ_I2C_MASTER                  30      /**< Interrupt number for I2C Master: TX */

#define IRQ_RC_CALIB                    31      /**< Interrupt number for RC calibration */

#define IRQ_ADC                         32      /**< Interrupt number for ADC */

#define IRQ_PML_SVLD                    33      /**< Interrupt number for PML SVLD */
#define IRQ_PML_CLOCK                   34      /**< Interrupt number for PML clock switch */

#define IRQ_NVM                         35      /**< Interrupt number for NVM */

#define IRQ_QDEC                        36      /**< Interrupt number for QDec */

#define IRQ_USB                         37      /**< Interrupt number for USB */

#define IRQ_CRYPTO_UNIT                 38      /**< Interrupt number for Crypto Unit */

#define IRQ_I2S                         39      /**< Interrupt number for I2S */

#define IRQ_SWI_0                       40      /**< Interrupt number for the software interrupt 0 **/
#define IRQ_SWI_1                       41      /**< Interrupt number for the software interrupt 1 **/
#define IRQ_SWI_2                       42      /**< Interrupt number for the software interrupt 2 **/
#define IRQ_SWI_3                       43      /**< Interrupt number for the software interrupt 3 **/
#define IRQ_SWI_4                       44      /**< Interrupt number for the software interrupt 4 **/
#define IRQ_SWI_5                       45      /**< Interrupt number for the software interrupt 5 **/
#define IRQ_SWI_6                       46      /**< Interrupt number for the software interrupt 6 **/
#define IRQ_SWI_7                       47      /**< Interrupt number for the software interrupt 7 **/
#define IRQ_SWI_8                       48      /**< Interrupt number for the software interrupt 6 **/
#define IRQ_SWI_9                       49      /**< Interrupt number for the software interrupt 7 **/
/** @} */


/******************************************************************************\
 *  Inline functions
\******************************************************************************/

/// Type for a function pointer to an interrupt service routine.
typedef _Interrupt void (*IRQ_InterruptServiceRoutine_t)(void);

/**
 * @brief Update the ISR in the IVT for the given interrupt.
 * @param index Interrupt to replace the ISR for.
 * @param interruptServiceRoutine Pointer to the new ISR to place into the IVT.
 */
static ALWAYS_INLINE void IRQ_InstallInterruptHandler(uint32_t index,
    IRQ_InterruptServiceRoutine_t interruptServiceRoutine)
{
    IRQ_InterruptServiceRoutine_t *pIVT =
        (IRQ_InterruptServiceRoutine_t*)ReadAUX(EM_ARC_INT_VECTOR_BASE);
    pIVT[INTERRUPT_OFFSET + index] = interruptServiceRoutine;
}

/**
 * @brief Get the ISR from the IVT for the given interrupt.
 * @param index Interrupt to get the ISR for.
 * @returns Pointer to the ISR function.
 */
static ALWAYS_INLINE IRQ_InterruptServiceRoutine_t IRQ_GetInterruptHandler(
    uint32_t index)
{
    IRQ_InterruptServiceRoutine_t *pIVT =
        (IRQ_InterruptServiceRoutine_t*)ReadAUX(EM_ARC_INT_VECTOR_BASE);

    return pIVT[INTERRUPT_OFFSET + index];
}

/**
 * @brief Copy the IVT and update the ARC to use the IVT from the new location.
 * @param pDestination Destination of the new IVT.
 * @returns true if the IVT was moved, false if the IVT is not 1KiB aligned.
 */
static ALWAYS_INLINE bool IRQ_MoveVectorTable(void *pDestination)
{
    if(0 == ((uint32_t)pDestination % IRQ_IVT_ALIGNMENT))
    {
        // Get the address of the original IVT.
        void *pSource = (void*)ReadAUX(EM_ARC_INT_VECTOR_BASE);

        // Copy the IVT.
        memcpy(pDestination, pSource, MEMORY_IVT_SIZE);

        // Update the address of the IVT.
        WriteAUX((uint32_t)pDestination, EM_ARC_INT_VECTOR_BASE);

        return true;
    }

    return false;
}

/**
 * @brief Globally enable interrupts.
 *
 * Interrupt priority threshold is not modified.
 */
static ALWAYS_INLINE void IRQ_EnableInterrupts(void)
{
    // Enable interrupts without changing interrupt threshold.
    __asm__("seti" ::: "memory");
}

/**
 * @brief Globally disable interrupts.
 *
 * Interrupt priority threshold is not modified.
 */
static ALWAYS_INLINE void IRQ_DisableInterrupts(void)
{
    // Flush the pipeline before continuing to ensure interrupts are really
    // disabled.
    __asm__("clri\n"
            "sync"
            :
            :
            : "memory"
            );
}

/**
 * @brief Globally disable interrupts and return the threshold.
 *
 * Interrupt priority threshold is not modified.
 * @returns Interrupt priority threshold.
 */
static ALWAYS_INLINE uint32_t IRQ_DisableAndSaveInterrupts(void)
{
    uint32_t threshold;

    __asm__ __volatile__(
        "clri %0\n"
        // Flush the pipeline before continuing to ensure interrupts are really
        // disabled.
        "sync"
        : "=r"(threshold)
        :
        : "memory"
        );

    return threshold;
}

/**
 * @brief Globally enable interrupts and restore the interrupt priority
 * threshold.
 *
 * @param threshold The interrupt priority threshold is modified to this value.
 * This value should match the seti value exactly. If you restore the value
 * returned from @ref IRQ_DisableAndSaveInterrupts, you should be OK.
 */
static ALWAYS_INLINE void IRQ_RestoreInterrupts(uint32_t threshold)
{
    __asm__ __volatile__(
        "seti %0"
        :
        : "r"(threshold)
        : "memory"
    );
}

/**
 * @brief Globally enable interrupts, restore the interrupt priority threshold
 * and go to CPU sleep mode 0 (Watchdog timer enabled).
 *
 * @param threshold The interrupt priority threshold is modified to this value.
 * This value should match the seti value exactly. If you restore the value
 * returned from @ref IRQ_DisableAndSaveInterrupts, you should be OK.
 */
static ALWAYS_INLINE void IRQ_RestoreInterruptsAndSleep(uint32_t threshold)
{
    __asm__ __volatile__(
        "sleep %0"
        :
        : "r"((0 << 5) | (threshold & 0x1F))
        : "memory"
    );
}

/**
 * @brief Globally enable interrupts, restore the interrupt priority threshold
 * and go to CPU sleep mode 1 (Watchdog timer disabled).
 *
 * @param threshold The interrupt priority threshold is modified to this value.
 * This value should match the seti value exactly. If you restore the value
 * returned from @ref IRQ_DisableAndSaveInterrupts, you should be OK.
 */
static ALWAYS_INLINE void IRQ_RestoreInterruptsAndSleepWithoutWatchdog(
    uint32_t threshold)
{
    __asm__ __volatile__(
        "sleep %0"
        :
        : "r"((1 << 5) | (threshold & 0x1F))
        : "memory"
    );
}

/**
 * @brief Set interrupt priority threshold and globally enable interrupts.
 *
 * Sets interrupt priority threshold and at the same time enables globally
 * interrupts. Interrupt priority threshold cannot be set atomically thus it is
 * merged with enabling interrupts.
 *
 * @param threshold Interrupt priority threshold <0, 15>.
 */
static ALWAYS_INLINE void IRQ_EnableInterruptsSetThreshold(uint8_t threshold)
{
    // Enable interrupts with specified interrupt threshold.
    __asm__("seti %0": : "r"(0x10 | (threshold & 0x0F)) : "memory");
}

/**
 * @brief Modify the interrupt priority level.
 * @param entry The interrupt to modify.
 * @param priority The priority level to specify, 0 being highest.
 */
static ALWAYS_INLINE void IRQ_SetPriority(uint32_t entry, uint8_t priority)
{
    WriteAUX(entry + INTERRUPT_OFFSET, EM_ARC_IRQ_SELECT);
    WriteAUX(priority, EM_ARC_IRQ_PRIORITY);
}

/**
 * @brief Get the interrupt priority level.
 * @param entry The interrupt to read the priority of.
 * @returns The priority level of the interrupt.
 */
static ALWAYS_INLINE uint8_t IRQ_GetPriority(uint32_t entry)
{
    WriteAUX(entry + INTERRUPT_OFFSET, EM_ARC_IRQ_SELECT);
    return (uint8_t)ReadAUX(EM_ARC_IRQ_PRIORITY);
}

/**
 * @brief Get interrupt index.
 *
 * Returns interrupt index of currently running interrupt.
 *
 * @returns Interrupt index af active interrupt.
 */
static ALWAYS_INLINE uint32_t IRQ_GetInterruptCause(void)
{
    return ReadAUX(EM_ARC_ICAUSE) - INTERRUPT_OFFSET;
}


static ALWAYS_INLINE void IRQ_ClearIlink(void)
{
    __asm__ ("mov %ilink,0x0000_0000");
}

static ALWAYS_INLINE uint32_t IRQ_ReadIlink(void)
{
    uint32_t irqRaised = 0;

    __asm__ ("mov %0, %%ilink" : "=r"(irqRaised) : : "memory");

    return irqRaised;
}

#define SWI(__N__)          (1 << (__N__))
#define SWI0                SWI(0)
#define SWI1                SWI(1)
#define SWI2                SWI(2)
#define SWI3                SWI(3)
#define SWI4                SWI(4)
#define SWI5                SWI(5)
#define SWI6                SWI(6)
#define SWI7                SWI(7)

/** @} (end addtogroup IRQ) */

#endif // _INTERRUPT_H
