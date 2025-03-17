////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/irq.h
///
/// @project    T9305
///
/// @brief      IRQ drivers
///
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

#ifndef _IRQ_H
#define _IRQ_H

#include <arc.h>
#include <types.h>
#include <t9305_periph.h>
#include <config.h>
#include <interrupts.h>

/**
 * @defgroup IRQ IRQ peripheral API
 * @ingroup EM_Library
 *
 * IRQ system consists from two parts: IRQ controller (AHB periphery of ARC)
 * and IRQ manager (part of ARC CPU core).
 *
 * IRQ controller is responsible of capturing interrupt requests, enabling
 * capturing of selected interrupt sources, and masking selected interrupt
 * sources. Each IRQ source can be individually enabled/disabled,
 * masked/unmasked and captured.
 *
 * IRQ controller allows also to individually clear each IRQ source as well as
 * to set each IRQ request to generate IRQ by SW.
 *
 * Some IRQ sources (after enabling and masking) are then ORed together to one
 * final interrupt request going into IRQ manager (ARC part).
 *
 * IRQ manager is responsible of capturing interrupt requests from IRQ
 * controller, setting priority for each IRQ source (after ORing).
 *
 * Basic functions of drivers are:
 * - set priority level of each IRQ
 * - enable/disable each IRQ individually
 * - mask/unmask each IRQ individually
 * - read IRQ status
 * - clear/set interrupt request
 */

/******************************************************************************\
* GLOBALS
\******************************************************************************/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @brief The number of IRQ registers for each IRQ group.
 * @ingroup IRQ
 */
#define IRQ_NUMBER_OF_GROUP_REGISTERS (9)

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/**
 * @brief Access the enable register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 *
 * An example usage:
 * @code
 * // Check if the interrupt for GPIO 0 is enabled.
 * if(!(IRQ_REG_EN(IRQ_GROUP_GPIO) & 1))
 * {
 *     // Enable the interrupt for only GPIO 0.
 *     IRQ_REG_EN(IRQ_GROUP_GPIO) = 1;
 * }
 * @endcode
 * @ingroup IRQ
 */
#define IRQ_REG_EN(irqGroup)     \
    REG_OFFSET(IRQ->RegIRQProtTimEn,     r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the enable set register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_ENSET(irqGroup)  \
    REG_OFFSET(IRQ->RegIRQProtTimEnSet,  r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the enable clear register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_ENCLR(irqGroup)  \
    REG_OFFSET(IRQ->RegIRQProtTimEnClr,  r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the mask register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_MSK(irqGroup)    \
    REG_OFFSET(IRQ->RegIRQProtTimMsk,    r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the mask set (unmask) register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_MSKSET(irqGroup) \
    REG_OFFSET(IRQ->RegIRQProtTimMskSet, r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the mask clear (mask) register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_MSKCLR(irqGroup) \
    REG_OFFSET(IRQ->RegIRQProtTimMskClr, r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the status register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_STS(irqGroup)    \
    REG_OFFSET(IRQ->RegIRQProtTimSts,    r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the status set register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_STSSET(irqGroup) \
    REG_OFFSET(IRQ->RegIRQProtTimStsSet, r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/**
 * @brief Access the status clear register for the specified IRQ group.
 * @param irqGroup The IRQ group for the register to access.
 * @sa IRQ_REG_EN
 * @ingroup IRQ
 */
#define IRQ_REG_STSCLR(irqGroup) \
    REG_OFFSET(IRQ->RegIRQProtTimStsClr, r32, \
        (irqGroup) * IRQ_NUMBER_OF_GROUP_REGISTERS)

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief IRQ group numbers
 * @ingroup IRQ
 */
typedef enum
{
    IRQ_GROUP_PROTOCOL_TIMER = 0, /**< %Interrupt group for Protocol Timer */
    IRQ_GROUP_SLEEP_TIMER,        /**< %Interrupt group for Sleep Timer */
    IRQ_GROUP_RF,                 /**< %Interrupt group for RF */
    IRQ_GROUP_SPI_SLAVE,          /**< %Interrupt group for SPI Slave */
    IRQ_GROUP_UART,               /**< %Interrupt group for UART */
    IRQ_GROUP_GPIO,               /**< %Interrupt group for GPIO */
    IRQ_GROUP_UNIVERSAL_TIMER,    /**< %Interrupt group for Universal Timer */
    IRQ_GROUP_SPI_MASTER,         /**< %Interrupt group for SPI Master */
    IRQ_GROUP_I2C_MASTER,         /**< %Interrupt group for I2C Master */
    IRQ_GROUP_RC_CALIB,           /**< %Interrupt group for RC calibration */
    IRQ_GROUP_ADC,                /**< %Interrupt group for ADC */
    IRQ_GROUP_PML,                /**< %Interrupt group for PML */
    IRQ_GROUP_NVM,                /**< %Interrupt group for NVM */
    IRQ_GROUP_QDEC,               /**< %Interrupt group for QDec */
    IRQ_GROUP_USB,                /**< %Interrupt group for USB */
    IRQ_GROUP_CRYPTO_UNIT,        /**< %Interrupt group for Crypto Unit */
    IRQ_GROUP_I2S,                /**< %Interrupt group for I2S */
    IRQ_GROUP_SWI,                /**< %Interrupt group for Software interrupt */
    IRQ_NUMBER_OF_GROUPS          /**< Number of interrupt groups */
} IRQ_Group_t;

/**
 * @brief IRQ configuration data structure.
 * @ingroup IRQ
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Interrupt threshold.
    uint8_t irqThreshold;

    /// Pointer to interrupt priority level.
    const uint8_t *pIrqPriority;

} IRQ_Configuration_t;

/**
 * @brief Reference to the IRQ configuration structure.
 * @ingroup IRQ
 */
extern volatile IRQ_Configuration_t gIRQ_Config;

/******************************************************************************\
 *  Inline functions
\******************************************************************************/

/**
 * @brief Enable/Disable interrupts in a IRQ group.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQs to enable/disable.
 *
 * Enable/disable the IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is enabled and a cleared bit (0) indicates the IRQ is disabled.
 *
 * Example of enabling some UART interrupts and disabling others:
 * @code
 * IRQ_SetEnable(IRQ_GROUP_UART,
 *     IRQ_UART_TX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_TX_FIFO_EMPTY_MSK(1) |
 *     IRQ_UART_RX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_RX_FIFO_FULL_MSK(1)
 * );
 * @endcode
 *
 * @sa IRQ_Enable
 * @sa IRQ_Disable
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_SetEnable(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // There is no enable/disable for software interrupts.
    if(IRQ_GROUP_SWI <= irqGroup)
    {
        return;
    }

    IRQ_REG_EN(irqGroup) = bitMask;
}

/**
 * @brief Enable the specified interrupts.
 * @param irqGroup IRQ group number,
 * @param bitMask  IRQ(s) to enable.
 *
 * Enable the specified IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is enabled and a cleared bit (0) indicates the IRQ remains
 * unchanged.
 *
 * @note This function shall be used in case a subset of the interruptions need
 *       to be selectively enabled without impacting the other interruptions of
 *       the same group.
 *
 * Here is an example of enabling some UART interrupts:
 * @code
 * IRQ_Enable(IRQ_GROUP_UART,
 *     IRQ_UART_TX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_TX_FIFO_EMPTY_MSK(1) |
 *     IRQ_UART_RX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_RX_FIFO_FULL_MSK(1)
 * );
 * @endcode
 *
 * @sa IRQ_SetEnable
 * @sa IRQ_Disable
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Enable(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // There is no enable/disable for software interrupts.
    if(IRQ_GROUP_SWI <= irqGroup)
    {
        return;
    }

    IRQ_REG_ENSET(irqGroup) = bitMask;
}

/**
 * @brief Disable the specified interrupts.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to disable.
 *
 * Disable the specified IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is disabled and a cleared bit (0) indicates the IRQ remains
 * unchanged.
 *
 * Here is an example of disabling some UART interrupts:
 * @code
 * IRQ_Disable(IRQ_GROUP_UART,
 *     IRQ_UART_TX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_TX_FIFO_EMPTY_MSK(1) |
 *     IRQ_UART_RX_FIFO_LIMIT_MSK(1) |
 *     IRQ_UART_RX_FIFO_FULL_MSK(1)
 * );
 * @endcode
 *
 * A full list of interrupts can be found in t9305_irq.h.
 *
 * @sa IRQ_SetEnable
 * @sa IRQ_Enable
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Disable(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // There is no enable/disable for software interrupts.
    if(IRQ_GROUP_SWI <= irqGroup)
    {
        return;
    }

    IRQ_REG_ENCLR(irqGroup) = bitMask;
}

/**
 * @brief Mask/unmask the interrupts in a IRQ group.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQs to mask/unmask.
 * @return The value of the mask register before it is set (atomic operation).
 *
 * Mask/unmask the IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is unmasked and a cleared bit (0) indicates the IRQ is masked.
 *
 * @warning Software interrupt mask (group IRQ_GROUP_SWI) is not saved and
 *          restored when we wake up from sleep. The software interrupts are
 *          always masked when the device wakes from sleep, and it is the user's
 *          responsibility to unmask the software interrupt before using it.
 *
 * @ingroup IRQ
 */
static ALWAYS_INLINE uint32_t IRQ_SetMask(IRQ_Group_t irqGroup,
    uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return 0;
    }

    MEM_BARRIER();

    bitMask = _xchg(bitMask, &IRQ_REG_MSK(irqGroup));

    MEM_BARRIER();

    return bitMask;
}

/**
 * @brief Mask the specified interrupts.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to mask.
 *
 * Mask the specified IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is masked and a clear bit (0) indicates the IRQ remains
 * unchanged.
 *
 * @warning Software interrupt mask (group IRQ_GROUP_SWI) is not saved and
 *          restored when we wake up from sleep. The software interrupts are
 *          always masked when the device wakes from sleep, and it is the user's
 *          responsibility to unmask the software interrupt before using it.
 *
 *
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Mask(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return;
    }

    MEM_BARRIER();

    IRQ_REG_MSKCLR(irqGroup) = bitMask;

    MEM_BARRIER();

    // Ensure the pipeline is flushed.
    _sync();

    MEM_BARRIER();
}

/**
 * @brief Unmask the specified interrupts.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to unmask.
 *
 * Unmask the specified IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the IRQ is unmasked and a clear bit (0) indicates the IRQ remains
 * unchanged.
 *
 * @warning Software interrupt mask (group IRQ_GROUP_SWI) is not saved and
 *          restored when we wake up from sleep. The software interrupts are
 *          always masked when the device wakes from sleep, and it is the user's
 *          responsibility to unmask the software interrupt before using it.
 *
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Unmask(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return;
    }

    IRQ_REG_MSKSET(irqGroup) = bitMask;
}

/**
 * @brief Trigger the specified interrupts.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to trigger.
 *
 * Trigger the specified IRQ sources from the selected IRQ group. A set bit (1)
 * indicates the interrupt will trigger and a clear bit (0) indicates the
 * interrupt will not be triggered.
 *
 * @note This will trigger an interrupt regardless of if the interrupt is
 * enabled. This may have undesired behavior. Use of this API method is @em not
 * recommended.
 * @note The ARC interrupt will be triggered only if the interrupt is unmasked.
 *
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Trigger(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return;
    }

    IRQ_REG_STSSET(irqGroup) = bitMask;
}

/**
 * @brief Clear pending interrupts.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to clear.
 *
 * Clear the specified pending IRQ source(s) from the selected IRQ group. A set
 * bit (1) indicates the IRQ will be cleared and a clear bit (0) indicates the
 * IRQ will not be cleared.
 *
 * @ingroup IRQ
 */
static ALWAYS_INLINE void IRQ_Clear(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return;
    }

    MEM_BARRIER();

    IRQ_REG_STSCLR(irqGroup) = bitMask;

    MEM_BARRIER();

    // Ensure the pipeline is flushed.
    _sync();

    MEM_BARRIER();
}

/**
 * @brief Get IRQ status.
 * @param irqGroup IRQ group number.
 * @returns Pending IRQ(s).
 *
 * @details Get IRQ status from selected IRQ group. A set bit (1) indicates the
 * interrupt is pending and a clear bit (0) indicates the interrupt is not
 * pending.
 *
 * @sa IRQ_IsPending
 * @sa IRQ_ArePending
 * @ingroup IRQ
 */
static ALWAYS_INLINE uint32_t IRQ_GetStatus(IRQ_Group_t irqGroup)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return 0;
    }

    return IRQ_REG_STS(irqGroup);
}

/**
 * @brief Determine if an interrupt is pending.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if any of the interrupt(s) are pending, false otherwise.
 *
 * Determines if any of the specified interrupt(s) from the given interrupt
 * group are pending or not.
 *
 * @sa IRQ_GetStatus
 * @sa IRQ_ArePending
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_IsPending(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    return (IRQ_REG_STS(irqGroup) & bitMask) != 0;
}

/**
 * @brief Determine if the interrupt(s) are pending.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if all of the interrupt(s) are pending, false otherwise.
 *
 * Determines if all of the specified interrupt(s) from the given interrupt
 * group are pending.
 *
 * @sa IRQ_GetStatus
 * @sa IRQ_IsPending
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_ArePending(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    return (IRQ_REG_STS(irqGroup) & bitMask) == bitMask;
}

/**
 * @brief Determine if an interrupt is enabled.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if any of the interrupt(s) are enabled, false otherwise.
 *
 * Determines if any of the specified interrupt(s) from the given interrupt
 * group are enabled or not.
 *
 * @sa IRQ_AreEnabled
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_IsEnabled(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    // Sowftware interrupts are always enabled.
    if(IRQ_GROUP_SWI == irqGroup)
    {
        return true;
    }
    else
    {
        return (IRQ_REG_EN(irqGroup) & bitMask) != 0;
    }
}

/**
 * @brief Determine if the interrupt(s) are enabled.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if all of the interrupt(s) are enabled, false otherwise.
 *
 * Determines if all of the specified interrupt(s) from the given interrupt
 * group are enabled.
 *
 * @sa IRQ_IsEnabled
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_AreEnabled(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    // Sowftware interrupts are always enabled.
    if(IRQ_GROUP_SWI == irqGroup)
    {
        return true;
    }
    else
    {
        return (IRQ_REG_EN(irqGroup) & bitMask) == bitMask;
    }
}

/**
 * @brief Determine if the interrupt(s) is masked.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if any of the interrupt(s) are masked, false otherwise.
 *
 * Determines if any of the specified interrupts from the given interrupt group
 * are masked.
 *
 * @sa IRQ_AreMasked
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_IsMasked(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    return (IRQ_REG_MSK(irqGroup) & bitMask) != bitMask;
}

/**
 * @brief Determine if the interrupt(s) are masked.
 * @param irqGroup IRQ group number.
 * @param bitMask  IRQ(s) to check.
 * @returns true if all of the interrupt(s) are masked, false otherwise.
 *
 * Determines if all of the specified interrupts from the given interrupt group
 * are masked.
 *
 * @sa IRQ_IsMasked
 * @ingroup IRQ
 */
static ALWAYS_INLINE bool IRQ_AreMasked(IRQ_Group_t irqGroup, uint32_t bitMask)
{
    // Sanity check for the IRQ group.
    if(IRQ_NUMBER_OF_GROUPS <= irqGroup)
    {
        return false;
    }

    return (IRQ_REG_MSK(irqGroup) & bitMask) == 0;
}

/**
 * @brief IRQ Cleaning Up function
 * @ingroup IRQ
 */
void IRQ_Cleaning(void);

/**
 * @brief IRQ Enabling during restor
 * @ingroup IRQ
 */
void IRQ_EnableAfterRestore(void);

/**
 * @brief IRQ initialization function
 * @ingroup IRQ
 */
void IRQ_Init(void);

/**
 * @brief IRQ store config before sleep
 * @ingroup IRQ
 */
void IRQ_Store(void);

/**
 * @brief IRQ restore config
 * @ingroup IRQ
 */
void IRQ_Restore(void);

#endif // _IRQ_H
