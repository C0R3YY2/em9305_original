////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/i2s/includes/i2s.h
///
/// @project    T9305
///
/// @brief      I2S driver interface
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2024 EM Microelectronic
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

#ifndef I2S_H
#define I2S_H

#include "config.h"
#include "i2s_hal.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup i2s I2S Driver
 * @brief I2S Driver
 * @{
*/

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief I2S configuration data structure
 */
typedef struct
{
    /// Base structure each module configuration must start with
    Config_ModuleConfiguration_t moduleConfig;

    /// Transaction queue depth
    uint8_t transactionDepth;

    /// Driver enable state
    bool enabled;

    /// GPIO for I2S serial clock / SCLK / BCLK
    uint8_t gpioSclk;

    /// GPIO for I2S serial data input / SDIN / DIN
    /// Unused in TX_ONLY transfer mode
    uint8_t gpioSdataIn;

    /// GPIO for I2S serial data output / SDOUT / DOUT
    /// Unused in RX_ONLY transfer mode
    uint8_t gpioSdataOut;

    /// GPIO for I2S Frame synchronization / FSYNC / WS / LRCLK
    uint8_t gpioFsync;

    /// GPIO for I2S master clock / MCLK / MCK
    /// Unused if gpioMclk >= NUM_GPIO_PINS
    uint8_t gpioMclk;

    /// Let the driver automatically enable and disable the clocks
    bool autoClk;

    /// I2S hardware configuration
    /// @see I2S_Cfg_t
    I2S_Cfg_t config;

    /// I2S phase0 configuration
    /// @see I2S_PhaseCfg_t
    I2S_PhaseCfg_t phase0;

    /// I2S phase1 configuration
    /// @see I2S_PhaseCfg_t
    I2S_PhaseCfg_t phase1;

    /// I2S transfer mode
    /// @see I2S_TransferMode_t
    I2S_TransferMode_t transferMode;

    /// Number of channels for phase0 TX
    uint8_t phase0TxChannels;

    /// Number of channels for phase1 TX
    uint8_t phase1TxChannels;

    /// Number of channels for phase0 RX
    uint8_t phase0RxChannels;

    /// Number of channels for phase1 RX
    uint8_t phase1RxChannels;

    /// I2S sampling rate
    /// @see I2S_SamplingRate_t
    I2S_SamplingRate_t samplingRate;

    /// Master clock ratio
    /// masterClockRatio * samplingRate = Master clock frequency (in hertz)
    uint16_t masterClockRatio;

    /// Serial clock ratio
    /// serialClockRatio * samplingRate = Serial clock frequency (in hertz)
    uint16_t serialClockRatio;

    /// Phase0 length in term of sclk cycles when configured by sw
    uint16_t phase0Length;

    /// Fsync length in term of sclk cycles
    uint16_t fsyncLength;

    /// DMA channel for TX operation
    /// Shouldn't be modified !
    uint8_t  dmaChannelTx;

    /// DMA channel for RX operation
    /// Shouldn't be modified !
    uint8_t  dmaChannelRx;

} I2S_Configuration_t;

/**
 * @brief I2S Buffer ready event
 */
typedef struct
{
    /// Buffer hold by the event
    uint8_t *pBuffer;

    /// Length of the buffer
    uint16_t length;

} I2S_BufferReadyEvt_t;

/**
 * @brief I2S event used to inform about the driver state
 */
typedef struct
{
    /// Event type
    /// @see I2S_EventType_t
    I2S_EventType_t type;

    /// Status of the event (TX or RX transaction)
    /// @see I2S_EventStatus_t
    I2S_EventStatus_t status;

    // Anonymous union of event parameters
    union
    {
        I2S_BufferReadyEvt_t bufRdyEvt; /// Available when type is I2S_EVT_BUFFER_READY
    };

} I2S_Event_t;

/**
 * @brief I2S callback function
 */
typedef void (*I2S_DriverCallback_t)(const I2S_Event_t *event, void *pUserData);

/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/

/// I2S configuration data structure
extern volatile I2S_Configuration_t gI2S_Config;

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Add the I2S module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void I2S_RegisterModule(void);

/**
 * @brief Manual enable of the clocks
 *
 * You can leave the driver manages the clocks by using the gI2S_Config.autoClk flag.
 * You shouldn't call this yourself if this flag is enabled.
 *
 * @param masterClk The master clock, true to enable
 * @param serialClk The serial clock, true to enable
 * @param fsync The fsync clock, true to enable
 */
void I2S_SetClocksState(bool masterClk, bool serialClk, bool fsync);

/**
 * @brief Set the clock frequencies for synchronized use
 * @param samplingRate The sampling rate (fsync frequency)
 * @param sclkRatio The ratio between fsync and the serial clock frequency
 *                  samplingRate * sclkRatio = serial clock frequency (in hertz)
 * @param mclkRatio The ratio between fsync and the master clock frequency
 *                  samplingRate * mclkRatio = master clock frequency (in hertz)
 *
 * The mclkRatio must be a multiple of the sclkRatio. For instance, passing sclkRatio=64
 * and mclkRatio=512 is correct (512/64=8) but passing sclkRatio=64 and mclkRatio=500 is
 * not and the function will return an error. This is to ensure clocks synchronization.
 *
 * The mclkRatio value can be 0 if the master clock is not used or if it is set with the
 * I2S_SetMclkFrequency() function. Passing 0 prevents the current clock configuration from being overridden.
 *
 * @see I2S_SetMclkFrequency() if you need to use a frequency that is not synchronized with fsync and sclk
 *
 * @note    These values are lost and reset to what you configured in the configModules() function
 *          after each sleep period. As a result, if changed at runtime, you'll need to call this
 *          function again after each wake up.
 *
 * This function will update the gI2S_Config structure.
 *
 * @return 0 on success
 * @return -EACCES if driver is running
 * @return -EINVAL if sclkRatio is 0 or mclkRatio is lower than sclkRatio or mclkRatio is not a multiple of sclkRatio
 * @return -ENOTSUP if the resulting mclk or sclk frequency is not in range [I2S_GetClkMinFrequency();I2S_GetClkMaxFrequency()]
 */
int32_t I2S_SetClockFrequencies(I2S_SamplingRate_t samplingRate, uint16_t sclkRatio, uint16_t mclkRatio);

/**
 * @brief Set the master clock frequency independently of other clocks
 *
 * @param freq Frequency in hertz
 * @return 0 on success
 * @return -EACCES if driver is running
 * @return -ENOTSUP if the frequency is not in the range [I2S_GetClkMinFrequency();I2S_GetClkMaxFrequency()]
 */
int32_t I2S_SetMclkFrequency(uint32_t freq);

/**
 * @brief Get mclk, sclk and fsync minimal achievable frequency
 *
 * @return uint32_t Minimal frequency in hertz
 */
static uint32_t ALWAYS_INLINE I2S_GetClkMinFrequency()
{
    // Divide the current clock frequency by the highest possible divider to get
    // the minimal achievable frequency. The divider is 16 bits integer and 16 bits fractional.
    uint32_t value = (I2S_HAL_GetSysClkFreq() / (UINT32_MAX >> 16u) + 1u);
    return value;
}

/**
 * @brief Get mclk, sclk and fsync maximal achievable frequency
 *
 * @return uint32_t Maximal frequency in hertz
 */
static uint32_t ALWAYS_INLINE I2S_GetClkMaxFrequency()
{
    // Maximal achievable frequency is half the current clock frequency.
    uint32_t value = (I2S_HAL_GetSysClkFreq() / 2u);
    return value;
}

/**
 * @brief Set phase 0 configuration
 *
 * @param delay Data delay (usually 1 for standard I2S and 0 for other format)
 * @param sampleSize Audio sample size for phase0
 * @param rxChannels Number of channels transmitted in RX (discarded if only TX)
 * @param txChannels Number of channels transmitted in TX (discarded if only RX)
 *
 * @note    These values are lost and reset to what you configured in the configModules() function
 *          after each sleep period. As a result, if changed at runtime, you'll need to call this
 *          function again after each wake up.
 *
 * This function directly update the gI2S_Config structure.
 *
 * @return 0 on success
 * @return -EACCES if driver is running
 */
int32_t I2S_SetPhase0Config(uint16_t delay, I2S_SampleSize_t sampleSize, uint8_t rxChannels, uint8_t txChannels);

/**
 * @brief Set phase 1 configuration
 *
 * @param delay Data delay (usually 1 for standard I2S and 0 for other format)
 * @param sampleSize Audio sample size for phase1
 * @param rxChannels Number of channels transmitted in RX (discarded if only TX)
 * @param txChannels Number of channels transmitted in TX (discarded if only RX)
 *
 * @note    These values are lost and reset to what you configured in the configModules() function
 *          after each sleep period. As a result, if changed at runtime, you'll need to call this
 *          function again after each wake up.
 *
 * This function directly update the gI2S_Config structure.
 *
 * @return 0 on success
 * @return -EACCES if driver is running
 */
int32_t I2S_SetPhase1Config(uint16_t delay, I2S_SampleSize_t sampleSize, uint8_t rxChannels, uint8_t txChannels);

/**
 * @brief Set phase 0 length in term of serial/bits clocks (sclk)
 *
 * This value has effect only when the phase length mode is configured as I2S_PHASE0_LENGTH_SW_VALUE.
 *
 * @param phase0Length Number of bit clocks for phase 0
 *
 * @note    These values are lost and reset to what you configured in the configModules() function
 *          after each sleep period. As a result, if changed at runtime, you'll need to call this
 *          function again after each wake up.
 *
 * This function directly update the gI2S_Config structure.
 *
 * @return 0 on success
 * @return -EACCES if driver is running
 */
int32_t I2S_SetPhase0Length(uint16_t phase0Length);

/**
 * @brief Set the number of serial/bit clocks fsync will keep its active polarity value
 *
 * @param fsyncLength Number of bit clocks
 *
 * @note    These values are lost and reset to what you configured in the configModules() function
 *          after each sleep period. As a result, if changed at runtime, you'll need to call this
 *          function again after each wake up.
 *
 * This function directly update the gI2S_Config structure.
 *
 * @return 0 on success
 * @return -EACCES if driver is running
 */
int32_t I2S_SetFsyncLength(uint16_t fsyncLength);

/**
 * @brief Add a TX transaction in the queue
 *
 * @param pTxBuffer Transaction buffer containing output data
 * @param length Length of the buffer
 * @param pCallback Callback called when transaction completes
 * @param pUserData Data to pass along with the callback
 * @return 0 on success
 * @return -EACCES if no TX channel is configured for phase 0 or phase 1
 * @return -EACCES if driver is running
 * @return -ESRCH if the driver wasn't initialized to work in TX direction
 * @return -EINVAL if provided buffer is NULL
 * @return -EINVAL if length isn't in range [I2S_TX_RX_FIFO_SIZE;DMA_MAX_TRANSFER_SIZE]
 * @return -EOVERFLOW if transactions queue is full
 *
 * @note The buffer length should be a multiple of I2S_GetBytesPerTxFrame(). If it's not the case the length will be automatically
 *      rounded down to the nearest multiple. This is mandatory as it's not possible to send incomplete frames.
 * @note The length must be in range [I2S_TX_RX_FIFO_SIZE;DMA_MAX_TRANSFER_SIZE] ([16, 4096])
 */
int32_t I2S_AddTxTransaction(uint8_t *pTxBuffer, uint16_t length, I2S_DriverCallback_t pCallback, void *pUserData);

/**
 * @brief Add a RX transaction in the queue
 *
 * @param pRxBuffer Transaction buffer to fill with input data
 * @param length Length of the buffer
 * @param pCallback Callback called when transaction completes
 * @param pUserData Data to pass along with the callback
 * @return 0 on success
 * @return -EACCES if no RX channel is configured for phase 0 or phase 1
 * @return -EACCES if driver is running
 * @return -ESRCH if the driver wasn't initialized to work in RX direction
 * @return -EINVAL if provided buffer is NULL
 * @return -EINVAL if length isn't in range [I2S_TX_RX_FIFO_SIZE;DMA_MAX_TRANSFER_SIZE]
 * @return -EOVERFLOW if transactions queue is full
 *
 * @note The buffer length should be a multiple of I2S_GetBytesPerRxFrame(). If it's not the case the length will be automatically
 *      rounded down to the nearest multiple. This is mandatory as it's not possible to send incomplete frames.
 * @note The length must be in range [I2S_TX_RX_FIFO_SIZE;DMA_MAX_TRANSFER_SIZE] ([16, 4096])
 */
int32_t I2S_AddRxTransaction(uint8_t *pRxBuffer, uint16_t length, I2S_DriverCallback_t pCallback, void *pUserData);

/**
 * @brief Get the number of transactions in the tx queue
 *
 * @return uint8_t Number of transactions
 */
uint8_t I2S_GetTxTransactionCount(void);

/**
 * @brief Get the number of transactions in the rx queue
 *
 * @return uint8_t Number of transactions
 */
uint8_t I2S_GetRxTransactionCount(void);

/**
 * @brief Clear transactions queues
 *
 * @note Queues cannot be cleared if I2S is running.
 *
 * @return 0 on success
 * @return -ACCES if driver is running
 */
int32_t I2S_ClearTransactions(void);

/**
 * @brief Start TX transactions
 *
 * One loop means each transaction in the queue is executed once.
 * You can pass 0 to start indefinitely.
 *
 * @param nLoops Number of loops to perform or 0 for infinite
 * @return 0 on success
 * @return -EACCES if driver is running
 * @return -ESRCH if the driver wasn't initialized to work in TX direction
 * @return -ENODATA if the transactions queue is empty
 */
int32_t I2S_StartTx(uint32_t nLoops);

/**
 * @brief Start RX transactions
 *
 * One loop means each transaction in the queue is executed once.
 * You can pass 0 to start indefinitely.
 *
 * @param nLoops Number of loops to perform or 0 for infinite
 * @return 0 on success
 * @return -EACCES if driver is running
 * @return -ESRCH if the driver wasn't initialized to work in RX direction
 * @return -ENODATA if the transactions queue is empty
 */
int32_t I2S_StartRx(uint32_t nLoops);

/**
 * @brief Start both RX and TX transactions
 *
 * @note This function should be preferred to a call to startTx() followed directly by a call to startRx() to
 *      ensure correct synchronization between the two directions.
 *
 * @param nLoops Number of loops to perform or 0 for infinite
 * @return 0 on success
 * @return -EACCES if driver is running
 * @return -ESRCH if the driver wasn't initialized to work in RX+TX direction
 * @return -ENODATA if one transactions queue is empty
 */
int32_t I2S_StartRxTx(uint32_t nLoops);

/**
 * @brief Safe stop tx
 *
 * Safe stop means the system waits for the current transaction to finish before disabling I2S and DMA.
 */
void I2S_StopTx(void);

/**
 * @brief Safe stop rx
 *
 * Safe stop means the system waits for the current transaction to finish before disabling I2S and DMA.
 */
void I2S_StopRx(void);

/**
 * @brief Force disabling I2S and DMA, regardless transactions are running
 */
void I2S_ForceStop(void);

/**
 * @return true If TX transactions are running
 */
bool I2S_IsTxWorking(void);

/**
 * @return true If RX transactions are running
 */
bool I2S_IsRxWorking(void);

/**
 * @return Number of loops that have to be executed or 0 if infinite or stopped
 */
uint32_t I2S_GetTxRemainingLoops(void);

/**
 * @return Number of loops that have to be executed or 0 if infinite or stopped
 */
uint32_t I2S_GetRxRemainingLoops(void);

/**
 * @brief Get the number of bits for a sample for the given sample size
 *
 * @param sampleSize sample size configuration
 * @return uint32_t Number of bits
 */
uint32_t I2S_GetBitsPerSample(I2S_SampleSize_t sampleSize);

/**
 * @brief Get the number of bytes for a sample for the given sample size
 *
 * @param sampleSize sample size configuration
 * @return uint32_t Number of bytes
 */
uint32_t I2S_GetBytesPerSample(I2S_SampleSize_t sampleSize);

/**
 * @brief Calculate and return the number of bytes that are used in one TX frame (phase0 + phase1)
 *
 * The transaction buffer length must be a multiple of this value.
 *
 * @return uint32_t Number of bytes
 */
uint32_t I2S_GetBytesPerTxFrame(void);

/**
 * @brief Calculate and return the number of bytes that are used in one RX frame (phase0 + phase1)
 *
 * The transaction buffer length must be a multiple of this value.
 *
 * @return uint32_t Number of bytes
 */
uint32_t I2S_GetBytesPerRxFrame(void);

/** @} */ // End of group i2s
/** @} */ // End of addtogroup drivers

#endif /* I2S_H */
