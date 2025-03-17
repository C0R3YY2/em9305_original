////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/i2c/includes/i2c.h
///
/// @project    T9305
///
/// @brief      I2C driver.
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


#ifndef I2C_H
#define I2C_H


#include "driver.h"
#include "types.h"
#include "config.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup i2c I2C Driver
 * @{
*/


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief Variable type to store a queue index or depth.
 */
typedef uint8_t I2C_QueueIndex_t;


/**
 * @brief I2C configuration data structure.
 */
 //lint ++flb some unreferenced structure members, treat as library code
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Size of the transaction queue.
    I2C_QueueIndex_t transactionDepth;

    /// If the driver is enabled or not.
    uint8_t enabled;

    /// A clock frequency from I2C_ClockFrequency_t.
    uint8_t clockFrequency;

    /// If clock stretching is enabled.
    uint8_t clockStretching;

    /// GPIO for the SCK signal.
    uint8_t gpioSck;

    /// GPIO for the SDA signal.
    uint8_t gpioSda;

    /// Reserved bytes.
    uint8_t reserved[2];
} I2C_Configuration_t;
//lint --flb


/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/

/// I2C configuration data.
extern volatile I2C_Configuration_t gI2C_Config;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @brief Add the I2C module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void I2C_RegisterModule(void);


/**
 * @brief Performs an I2C write operation on the selected
 *   register of the I2C device.
 *
 * This is a non-blocking operation, the input buffer cannot be modified until
 * the transaction is complete.
 *
 * @note If the buffer is on the stack, you must ensure that the transaction
 *   completes before the function exits.
 *
 * @param i2cDevice The I2C device address to communicate with.
 * @param reg The register to write to.
 * @param[in] pBuffer                An array of data to write to the device.
 * @param bytes The number of entries in the source array.
 * @param[in] callbackFunction The function to call after the I2C
 *   transaction is complete, the return status is passed as an argument. If
 *   you do not need a callback you may set this to NULL.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
//lint -function( fopen(1), I2C_WriteRegister(3) ) // arg cannot be null
bool I2C_WriteRegister(uint8_t i2cDevice, uint8_t reg, uint8_t *pBuffer,
    uint32_t bytes, Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Performs one or more I2C read operations on the
 * selected register of the I2C device.
 *
 * This is a non-blocking operation. The input buffer will be overwritten
 * once the I2C transaction has completed.
 *
 * @param i2cDevice The I2C device address to communicate with.
 * @param reg The register to read from.
 * @param[out] pBuffer The memory location to save the read data.
 * @param bytes The number of bytes to read.
 * @param[in] callbackFunction The function to call after the I2C
 *   transaction is complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
//lint -function( fopen(1), I2C_ReadRegister(3) ) // arg cannot be null
bool I2C_ReadRegister(uint8_t i2cDevice, uint8_t reg, uint8_t *pBuffer,
    uint32_t bytes, Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Performs one or more I2C read operations on the
 *   selected I2C device.
 *
 * This is a non-blocking operation. The input buffer will be overwritten
 * once the I2C transaction has completed.
 *
 * @param i2cDevice The I2C device address to communicate with.
 * @param[out] pBuffer The memory location to save the read data.
 * @param bytes The number of bytes to read.
 * @param[in] callbackFunction The function to call after the I2C
 *   transaction is complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
//lint -function( fopen(1), I2C_Read(2) ) // arg cannot be null
bool I2C_Read(uint8_t i2cDevice, uint8_t *pBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Performs one or more I2C read operations on the
 *   selected I2C device.
 *
 * This is a non-blocking operation. The input buffer will be overwritten
 * once the I2C transaction has completed.
 *
 * @param i2cDevice The I2C device address to communicate with.
 * @param[in] pBuffer An array of data to write to the device.
 * @param bytes The number of entries in the source array.
 * @param[in] callbackFunction The function to call after the I2C
 *   transaction is complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
//lint -function( fopen(1), I2C_Write(2) ) // arg cannot be null
bool I2C_Write(uint8_t i2cDevice, uint8_t *pBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Return the number of pending transactions.
 * @returns Number of pending I2C transactions.
 */
uint32_t I2C_PendingTransactionCount(void);


/** @} */ // End of group i2c
/** @} */ // End of addtogroup drivers


/******************************************************************************\
 *  DEPRECATED PROTOTYPES
\******************************************************************************/

/// I2C_WriteBytes is deprecated and replaced by I2C_Write
static inline __attribute__((deprecated("Use I2C_Write instead of I2C_WriteBytes.")))
bool I2C_WriteBytes(uint8_t i2cDevice, uint8_t *pBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData)
{
    return I2C_Write(i2cDevice, pBuffer, bytes,
        callbackFunction, pUserData);
}

/// I2C_ReadBytes is deprecated and replaced by I2C_Read
static inline __attribute__((deprecated("Use I2C_Read instead of I2C_ReadBytes.")))
bool I2C_ReadBytes(uint8_t i2cDevice, uint8_t *pBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData)
{
    return I2C_Read(i2cDevice, pBuffer, bytes,
        callbackFunction, pUserData);
}

/// I2C_WriteData is deprecated and replaced by I2C_WriteRegister
static inline __attribute__((deprecated("Use I2C_WriteRegister instead of I2C_WriteData.")))
bool I2C_WriteData(uint8_t i2cDevice, uint8_t reg, uint8_t *pBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData)
{
    return I2C_WriteRegister(i2cDevice, reg, pBuffer, bytes,
        callbackFunction, pUserData);
}

/// I2C_ReadData is deprecated and replaced by I2C_ReadRegister
static inline __attribute__((deprecated("Use I2C_ReadRegister instead of I2C_ReadData.")))
bool I2C_ReadData(uint8_t i2cDevice, uint8_t reg, uint8_t *pBuffer,
    uint32_t bytes, Driver_Callback_t callbackFunction, void *pUserData)
{
    return I2C_ReadRegister(i2cDevice, reg, pBuffer, bytes,
        callbackFunction, pUserData);
}


#endif /* I2C_H */
