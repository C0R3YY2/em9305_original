////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/spi_master/includes/spi_master.h
///
/// @project    T9305
///
/// @brief      SPI master driver.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2021 EM Microelectronic
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


#ifndef SPI_MASTER_H
#define SPI_MASTER_H


#include "driver.h"
#include <types.h>
#include "gpio.h"
#include "spi_master_hal.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup spim SPI Master Driver
 * @brief Driver for the SPI master.
 *
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/// Use this define if the driver should not toggle a chip select GPIO.
#define SPIM_CSN_GPIO_NONE (0xFFu)


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief SPI Master enable control.
 */
typedef uint8_t SPIM_Enable_t;


/**
 * @brief Variable type to store a queue index or depth.
 */
typedef uint8_t SPIM_QueueIndex_t;


/**
 * @brief Function to assert the slave device.
 * @param param Slave device parameter. It is usually the CSN GPIO.
 */
typedef void (*SPIM_AssertChipFct_t)(uint8_t param);


/**
 * @brief Function to deassert the slave device.
 * @param param Slave device parameter. It is usually the CSN GPIO.
 */
typedef void (*SPIM_DeassertChipFct_t)(uint8_t param);


/**
 * @brief SPI master configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Configuration bits for the SPI master.
    SPIM_Config_t configBits;

    /// Function to assert the slave device.
    SPIM_AssertChipFct_t assertChipFct;

    /// Function to deassert the slave device.
    SPIM_DeassertChipFct_t deassertChipFct;

    /// SPI Master driver enabled.
    SPIM_Enable_t enabled;

    /// Size of the transaction queue.
    SPIM_QueueIndex_t transactionDepth;

    /// GPIO for the SPI master SCK.
    uint8_t gpioSck;

    /// GPIO for the SPI master MISO.
    uint8_t gpioMiso;

    /// GPIO for the SPI master MOSI.
    uint8_t gpioMosi;

    /// Reserved bytes for alignment.
    uint8_t reserved[3];
} SPIM_Configuration_t;


/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/


/// SPI master configuration data.
extern volatile SPIM_Configuration_t gSPIM_Config;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @brief Add the SPI master module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void SPIM_RegisterModule(void);


/**
 * @brief Performs a read operation.
 *
 * This is a non-blocking operation. The input buffer will be overwritten once
 * the transaction has completed.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param bytes The number of bytes to read.
 * @param[in] callbackFunction The function to call after the transaction is
 *   complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
bool SPIM_ReadBytes(uint8_t chipSelectGpio, bool keepChipSelectLow,
    uint8_t *pReadBuffer, uint32_t bytes, Driver_Callback_t callbackFunction,
    void *pUserData);


/**
 * @brief Performs a write operation.
 *
 * This is a non-blocking operation, the input buffer cannot be modified until
 * the transaction is complete.
 *
 * @note If the buffer is on the stack, you must ensure that the transaction
 *   completes before the function exits.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param bytes The number bytes to write.
 * @param[in] callbackFunction The function to call after the transaction is
 *   complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
bool SPIM_WriteBytes(uint8_t chipSelectGpio, bool keepChipSelectLow,
    const uint8_t *pWriteBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Performs a read and write operation.
 *
 * This is a non-blocking operation. The read buffer will be overwritten once
 * the transaction has completed.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param bytes The number of bytes to transfer.
 * @param[in] callbackFunction The function to call after the transaction is
 *   complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
bool SPIM_TransferBytes(uint8_t chipSelectGpio, bool keepChipSelectLow,
    const uint8_t *pWriteBuffer, uint8_t *pReadBuffer, uint32_t bytes,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 *
 * @brief Performs a write operation with number of writesbytes then performs a
 *        read operation with number of readbytes.
 *
 * This is a non-blocking operation. The read buffer will be overwritten once
 * the transaction has completed.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param writeBytes The number of bytes to write.
 * @param readBytes The number of bytes to read.
 * @param[in] callbackFunction The function to call after the transaction is
 *   complete, the return status is passed as an argument.
 * @param[in] pUserData The data to pass to the callback function.
 * @returns true if the transaction was scheduled, false otherwise.
 */
bool SPIM_TransferBytesHalfDuplex(uint8_t chipSelectGpio,
        bool keepChipSelectLow, const uint8_t *pWriteBuffer,
        uint8_t *pReadBuffer, uint32_t writeBytes, uint32_t readBytes,
        Driver_Callback_t callbackFunction,void *pUserData);


/**
 *
 * @brief Performs a write operation with number of writesbytes then performs a
 *        read operation with number of readbytes.
 *
 * This is a blocking operation. The read buffer will be overwritten once
 * the transaction has completed.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param writeBytes The number of bytes to write.
 * @param readBytes The number of bytes to read.
 * @returns true if the transaction was scheduled, false otherwise.
 */
bool SPIM_TransferBytesHalfDuplexBlocking(uint8_t chipSelectGpio,
        bool keepChipSelectLow, const uint8_t *pWriteBuffer,
        uint8_t *pReadBuffer, uint32_t writeBytes, uint32_t readBytes);


/**
 * @brief Performs a write operation.
 *
 * This is a blocking operation.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param bytes The number of bytes to write.
 * @returns true if the transaction was executed, false otherwise.
 */
bool SPIM_WriteBytesBlocking(uint8_t chipSelectGpio, bool keepChipSelectLow,
        uint8_t *pWriteBuffer, uint32_t bytes);


/**
 * @brief Performs a read operation.
 *
 * This is a blocking operation.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param bytes The number of bytes to read.
 * @returns true if the transaction was executed, false otherwise.
 */
bool SPIM_ReadBytesBlocking(uint8_t chipSelectGpio, bool keepChipSelectLow,
         uint8_t *pReadBuffer, uint32_t bytes);


/**
 * @brief Performs a read and write operation.
 *
 * This is a blocking operation.
 *
 * @param chipSelectGpio Number of the GPIO to use for the chip select.
 * @param keepChipSelectLow True if the CSN should be kept asserted after
 *                          transaction.
 * @param[in] pWriteBuffer An array of data to write to the device.
 * @param[out] pReadBuffer The memory location to save the read data.
 * @param bytes The number of bytes to transfer.
 * @returns true if the transaction was executed, false otherwise.
 */
bool SPIM_TransferBytesBlocking(uint8_t chipSelectGpio, bool keepChipSelectLow,
    const uint8_t *pWriteBuffer, uint8_t *pReadBuffer, uint32_t bytes);


/**
 * @brief Return the number of pending transactions.
 * @returns Number of pending SPI master transactions.
 */
uint32_t SPIM_PendingTransactionCount(void);


/**
 * @brief Set the SPI master clock to a frequency at or bellow the given
 *        frequency.
 * @param frequency Frequency that the SPI master clock may not exceed.
 * @returns Highest prescaler that produces a frequency that approaches but is
 * not above the requested frequency.
 * @note If the frequency is too low, the lowest frequency possible is used.
 */
SPIM_Prescaler_t SPIM_GetPrescalerForMaxClockFrequency(uint32_t frequency);


/**
 * @brief Configure the clock frequency at or bellow the given frequency.
 * @param frequency Frequency that the SPI master clock may not exceed.
 * @note If the frequency is too low, the lowest frequency possible is used.
 * @note The clock frequency is applied on next transaction but not
 * on transaction that are already in the queue.
 * @note This configuration is lost during sleep.
 */
void SPIM_SetClockFrequency(uint32_t frequency);


/**
 * @brief Configure the clock prescaler.
 * @param prescaler The clock prescaler.
 * @note The prescaler is applied on next transaction but not
 * on transaction that are already in the queue.
 * @note This configuration is lost during sleep.
 */
void SPIM_SetClockPrescaler(SPIM_Prescaler_t prescaler);


/**
 * @brief Set the configuration for next transaction.
 * @param config The SPI master config.
 * @note The configuration is applied on next transaction but not
 * on transaction that are already in the queue.
 * @note This configuration is lost during sleep.
 */
void SPIM_SetConfiguration(SPIM_Config_t config);


/**
 * @brief Assert the slave device on the given GPIO.
 * @param gpio The CSN GPIO.
 */
void SPIM_AssertChip(uint8_t gpio);


/**
 * @brief De-assert the slave device on the given GPIO.
 * @param gpio The CSN GPIO.
 */
void SPIM_DeassertChip(uint8_t gpio);


/** @} */ // End of group spim
/** @} */ // End of addtogroup drivers


#endif /* SPI_MASTER_H */
