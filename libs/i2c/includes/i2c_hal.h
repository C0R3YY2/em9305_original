////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/i2c/includes/i2c_hal.h
///
/// @project    T9305
///
/// @brief      I2C hardware abstraction layer that is specific to the chip.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2016 EM Microelectronic
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


#ifndef I2C_HAL_H
#define I2C_HAL_H


#include <t9305_periph.h>

/**
 * @addtogroup drivers
 * @{
 * @defgroup i2c_hal I2C HAL Driver
 * @{
*/

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// Maximum number of bytes that are sent in a single transaction.
#define I2C_MAX_TRANS_LENGTH (16)


/**
 * @brief I2C master control register.
 */
typedef RegI2CMCtrl_t I2C_ControlRegister_t;


/**
 * @brief Possible frequency rates supported by the I2C master.
 */
typedef enum {
    ClockFrequency1000 = 0, /**< I2C Fast+ mode, 1000Khz. */
    ClockFrequency833  = 1, /**< I2C Fast+ mode, 833Khz. */
    ClockFrequency400  = 2, /**< I2C Fast mode, 400Khz. */
    ClockFrequency333  = 3, /**< I2C Fast mode, 333Khz. */
    ClockFrequency100  = 4, /**< I2C Standard mode, 100Khz. */
    ClockFrequency83   = 5, /**< I2C Standard mode, 83Khz. */
} I2C_ClockFrequency_t;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Sets the I2C Master to use the specified clock frequency.
 * @param freq The frequency to use for I2C transactions.
 */
void I2C_HAL_SetClockFrequency(I2C_ClockFrequency_t freq);


/**
 * @brief Sets the I2C Master to use the specified clock frequency.
 * @param freq The frequency to use for I2C transactions.
 */
void I2C_HAL_SetClockMaxFrequency(uint16_t freq);


/**
 * @brief Reads out the I2C master clock frequency.
 * @returns The frequency that the I2C master is currently set to use.
 */
I2C_ClockFrequency_t I2C_HAL_GetClockFrequency(void);


/**
 * @brief Enables or disables clock stretching on the I2C master bus.
 * @param enable Enable or disable clock stretching.
 */
void I2C_HAL_SetClockStretching(bool enable);


/**
 * @brief Returns the state of the clock stretching enable bit.
 * @retval true  Clock stretching is currently enabled.
 * @retval false Clock stretching is currently disabled.
 */
bool I2C_HAL_GetClockStretching(void);


/**
 * @brief Begins an I2C transaction specified by the ctrl register.
 *
 * Data written using this function will be taken from the hardware registers.
 * Data read using this function will be stored in the hardware registers.
 *
 * @param ctrl   New control register value.
 * @retval true  The I2C transaction has started.
 * @retval false The I2C transaction was unable to start.
 */
bool I2C_HAL_BeginTransactionCtrl(I2C_ControlRegister_t ctrl);


/**
 * @brief Waits for the I2C transaction to complete.
 * @retval true  An ACK was received.
 * @retval false A NACK was received.
 */
bool I2C_HAL_DidAck(void);


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Determines if the I2C master is currently in use.
 * @retval true  The I2C master is in use.
 * @retval false The I2C master is available.
 */
static ALWAYS_INLINE bool I2C_HAL_IsBusy(void)
{
    bool value = I2CM_BUSY(I2CM->RegI2CMStat.r32);
    return value;
}


/**
 * @brief Generate a control register value to start an I2C register write.
 * @param i2cDevice The 7-bit I2C slave address.
 * @returns The value to set in the control register.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_StartWriteReg(
    uint8_t i2cDevice)
{
    I2C_ControlRegister_t ctrl;
    ctrl.r32 = I2CM_REG_ADDRESS_EN_MASK | I2CM_START_TRANS_MASK |
        I2CM_SLAVE_ADDRESS(i2cDevice);

    return ctrl;
}


/**
 * @brief Generate a control register value to start an I2C write.
 * @param i2cDevice The 7-bit I2C slave address.
 * @returns The value to set in the control register.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_StartWriteData(
    uint8_t i2cDevice)
{
    I2C_ControlRegister_t ctrl;
    ctrl.r32 = I2CM_START_TRANS_MASK | I2CM_SLAVE_ADDRESS(i2cDevice);

    return ctrl;
}


/**
 * @brief Generate a control register value to start an I2C register read.
 * @param i2cDevice The 7-bit I2C slave address.
 * @returns The value to set in the control register.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_StartReadReg(
    uint8_t i2cDevice)
{
    I2C_ControlRegister_t ctrl;
    ctrl.r32 = I2CM_REG_ADDRESS_EN_MASK | I2CM_START_TRANS_MASK |
        I2CM_SLAVE_ADDRESS(i2cDevice) | I2CM_READ_WRITE_SEL(1);

    return ctrl;
}


/**
 * @brief Generate a control register value to start an I2C read.
 * @param i2cDevice The 7-bit I2C slave address.
 * @returns The value to set in the control register.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_StartReadData(
    uint8_t i2cDevice)
{
    I2C_ControlRegister_t ctrl;
    ctrl.r32  = I2CM_START_TRANS_MASK | I2CM_READ_WRITE_SEL(1) |
        I2CM_SLAVE_ADDRESS(i2cDevice);

    return ctrl;
}


/**
 * @brief Set the register address in the I2C control register.
 * @param ctrl The original I2C control register.
 * @param reg The register address to set within the control register.
 * @returns The value to change the control register to.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_SetReg(
    I2C_ControlRegister_t ctrl, uint8_t reg)
{
    ctrl.r32 |= I2CM_REG_ADDRESS(reg);

    return ctrl;
}


/**
 * @brief Update the register address in the I2C control register (add to it).
 * @param ctrl The original I2C control register.
 * @param offset The offset to add to the register address.
 * @returns The value to change the control register to.
 */
static ALWAYS_INLINE I2C_ControlRegister_t I2C_HAL_ControlRegister_AddToReg(
    I2C_ControlRegister_t ctrl, uint32_t offset)
{
    ctrl.regs.I2CMRegAddress += offset;

    return ctrl;
}


/**
 * @brief Determine if the control register is set to write.
 * @param ctrl The I2C control register.
 * @retval true  The I2C control register is configured for a write.
 * @retval false The I2C control register is configured for a read.
 */
static ALWAYS_INLINE bool I2C_HAL_ControlRegister_IsWrite(
    I2C_ControlRegister_t ctrl)
{
    bool value = (0 == GET_I2CM_READ_WRITE_SEL(ctrl.r32));
    return value;
}


/**
 * @brief Determine if the control register is set to read.
 * @param ctrl The I2C control register.
 * @retval true  The I2C control register is configured for a read.
 * @retval false The I2C control register is configured for a write.
 */
static ALWAYS_INLINE bool I2C_HAL_ControlRegister_IsRead(
    I2C_ControlRegister_t ctrl)
{
    bool value = (0 != GET_I2CM_READ_WRITE_SEL(ctrl.r32));
    return value;
}


/**
 * @brief Set the transaction length in the I2C control register.
 * @param ctrl The original I2C control register.
 * @param dataLength The transaction length to set.
 * @returns The value to change the control register to.
 */
static ALWAYS_INLINE I2C_ControlRegister_t
    I2C_HAL_ControlRegister_SetTransactionLen(I2C_ControlRegister_t ctrl,
    uint8_t dataLength)
{
    ctrl.r32 |= I2CM_TRANS_LENGTH(dataLength);

    return ctrl;
}


/**
 * @brief Set/unset the NO_STOP bit in the I2C control register.
 * @param ctrl The original I2C control register.
 * @param enable True if the NO_STOP bit should be set, false otherwise
 * @returns The value to change the control register to.
 */
static ALWAYS_INLINE I2C_ControlRegister_t
    I2C_HAL_NoStop(I2C_ControlRegister_t ctrl, bool enable)
{
    if(enable)
    {
        ctrl.r32 |= I2CM_NO_STOP_MASK;
    }
    else
    {
        ctrl.r32 &= ~I2CM_NO_STOP_MASK;
    }

    return ctrl;
}


/** @} */ // End of group i2c_hal
/** @} */ // End of addtogroup drivers


#endif /* I2C_HAL_H */
