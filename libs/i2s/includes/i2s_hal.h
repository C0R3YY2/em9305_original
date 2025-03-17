////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/i2s/includes/i2s_hal.h
///
/// @project    T9305
///
/// @brief      I2S hardware abstraction layer
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

#ifndef I2S_HAL_H
#define I2S_HAL_H

#include "types.h"
#include "t9305_periph.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup i2s_hal I2S HAL Driver
 * @brief HAL Driver for the I2S driver.
 * @{
*/

/******************************************************************************\
 *  DEFINITIONS
\******************************************************************************/

/// TX FIFO access to push 1 byte
#define I2S_TX_FIFO_1B_ADDR (&I2S->RegI2sTxFifo1B.r32)
/// TX FIFO access to push 2 bytes
#define I2S_TX_FIFO_2B_ADDR (&I2S->RegI2sTxFifo2B.r32)
/// TX FIFO access to push 3 bytes
#define I2S_TX_FIFO_3B_ADDR (&I2S->RegI2sTxFifo3B.r32)
/// TX FIFO access to push 4 bytes
#define I2S_TX_FIFO_4B_ADDR (&I2S->RegI2sTxFifo4B.r32)

/// RX FIFO access to pop 1 byte
#define I2S_RX_FIFO_1B_ADDR (&I2S->RegI2sRxFifo1B.r32)
/// RX FIFO access to pop 2 bytes
#define I2S_RX_FIFO_2B_ADDR (&I2S->RegI2sRxFifo2B.r32)
/// RX FIFO access to pop 3 bytes
#define I2S_RX_FIFO_3B_ADDR (&I2S->RegI2sRxFifo3B.r32)
/// RX FIFO access to pop 4 bytes
#define I2S_RX_FIFO_4B_ADDR (&I2S->RegI2sRxFifo4B.r32)

/// TX/RX FIFO size
#define I2S_TX_RX_FIFO_SIZE (16u) // 16 bytes

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// I2S Transaction type
typedef enum
{
    /// Event transaction is of TX direction
    I2S_STATUS_TX = 0,
    /// Event transaction is of RX direction
    I2S_STATUS_RX
} I2S_EventStatus_t;

/// I2S Transaction status
typedef enum
{
    /// The buffer that is hold by the event is ready for use by the user application
    I2S_EVT_BUFFER_READY = 0,
    /// Transactions finished for the direction
    I2S_EVT_FINISHED,
    /// Driver and DMA synchronization error
    I2S_EVT_SYNC_ERROR,
    /// DMA error
    I2S_EVT_DMA_ERROR
} I2S_EventType_t;

/// I2S phase
typedef enum
{
    /// Phase 0
    I2S_PHASE0 = 0,
    /// Phase 1
    I2S_PHASE1
} I2S_Phase_t;

/// I2S mode
enum
{
    /// Slave mode
    I2S_SLAVE_MODE = 0,
    /// Master mode
    I2S_MASTER_MODE
};
/// @brief I2S mode enumeration
typedef uint8_t I2S_Mode_t;
// This ensures I2S_Mode_t will be a uint8_t and allows for serialization into bit streams

/// I2S protocol
enum
{
    /// TDM protocol
    I2S_PROTOCOL_TDM = 0,
    /// I2S protocol
    I2S_PROTOCOL_I2S,
    /// Right justified protocol
    I2S_PROTOCOL_RIGHT_JUSTIFIED
};
/// @brief I2S protocol enumeration
typedef uint8_t I2S_Protocol_t;
// This ensures I2S_Protocol_t will be a uint8_t and allows for serialization into bit streams

/// Sampling format of the I2S
enum
{
    /// 8 bits data format
    I2S_SAMPLE_FORMAT_8BIT = 0,
    /// 12 bits data format
    I2S_SAMPLE_FORMAT_12BIT,
    /// 16 bits data format
    I2S_SAMPLE_FORMAT_16BIT,
    /// 20 bits data format
    I2S_SAMPLE_FORMAT_20BIT,
    /// 24 bits data format
    I2S_SAMPLE_FORMAT_24BIT,
    /// 32 bits data format
    I2S_SAMPLE_FORMAT_32BIT
};
/// @brief I2S sample sizes enumeration
typedef uint16_t I2S_SampleSize_t;
// This ensures I2S_SampleSize_t will be a uint16_t and allows for serialization into bit streams

/// Sampling rate frequency
typedef enum
{
    /// 8000 Hz data rate
    I2S_SAMPLE_RATE_8KHZ = 8000,
    /// 11025 Hz data rate
    I2S_SAMPLE_RATE_11KHZ = 11025,
    /// 16000 Hz data rate
    I2S_SAMPLE_RATE_16KHZ = 16000,
    /// 22050 Hz data rate
    I2S_SAMPLE_RATE_22KHZ = 22050,
    /// 32000 Hz data rate
    I2S_SAMPLE_RATE_32KHZ = 32000,
    /// 44100 Hz data rate
    I2S_SAMPLE_RATE_44KHZ = 44100,
    /// 48000 Hz data rate
    I2S_SAMPLE_RATE_48KHZ = 48000,
    /// 88200 Hz data rate
    I2S_SAMPLE_RATE_88KHZ = 88200,
    /// 96000 Hz data rate
    I2S_SAMPLE_RATE_96KHZ = 96000,
    /// 128000 Hz data rate
    I2S_SAMPLE_RATE_128KHZ = 128000,
    /// 176400 Hz data rate
    I2S_SAMPLE_RATE_176KHZ = 176400,
    /// 192000 Hz data rate
    I2S_SAMPLE_RATE_192KHZ = 192000
} I2S_SamplingRate_t;

/// Transfer mode
typedef enum
{
    /// Only TX
    I2S_TX_ONLY = 0x01,
    /// Only RX
    I2S_RX_ONLY = 0x02,
    /// Both TX and RX
    I2S_TX_RX = 0x03
} I2S_TransferMode_t;

/// Phase 0 length mode
enum
{
    /**
     * Phase0 length is calculated as the frame period / 2
     */
    I2S_PHASE0_LENGTH_FSYNC_PERIOD_DIV2 = 0,

    /**
     * Phase0 length is determined by the polarity of fsync
     * Phase0 starts when fsync transitions from inactive to active, and ends when fsync transitions from active to inactive
     */
    I2S_PHASE0_LENGTH_FSYNC_EDGE,

    /**
     * Phase0 length is configured by software
     */
    I2S_PHASE0_LENGTH_SW_VALUE,

    /**
     * Phase0 length equals the frame period, meaning phase 1 is discarded
     */
    I2S_PHASE0_LENGTH_FRAME_PERIOD

};
/// @brief I2S Phase 0 length mode enumeration
typedef uint8_t I2S_PhaseLengthMode_t;
// This ensures I2S_PhaseLengthMode_t will be a uint8_t and allows for serialization into bit streams

/// Config structure of the I2S
typedef union
{
    /// 32-bit access
    uint32_t cfgBits;

    /// bits access
    struct
    {
        /// I2S mode (master or slave)
        I2S_Mode_t mode : 1;

        /// I2S protocol (TDM, I2S or Right Justified)
        I2S_Protocol_t protocol : 2;

        /// Clock Phase of SCLK
        /// 0 : Data generation at falling edge, capture at rising
        /// 1 : Data generation at rising edge, capture at falling
        uint8_t cpha : 1;

        /// Fsync polarity
        uint8_t fsyncPolarity : 1;

        // Unused bits
        uint8_t unused : 3;

        /// Phase 0 length mode
        I2S_PhaseLengthMode_t phaseLengthMode : 2;

        /// Send Least Significant bit first
        uint8_t lsbitFirst : 1;

        /// Send Least Significant byte first
        uint8_t lsbyteFirst : 1;

        /// Apply sign extension when data format is 12 or 20 bits
        uint8_t I2sSignExtendEn : 1;

        // Padding at the end
        uint8_t padding0 : 3;
        uint8_t padding1;
        uint8_t padding2;

    } bits;
} I2S_Cfg_t;

/// Config structure of the I2S phase
typedef union
{
    /// 32-bit access
    uint32_t cfgBits;

    /// bit access
    struct
    {
        /// Phase delay
        uint16_t delay : 12;

        // Unused bits
        uint16_t unused0 : 4;

        /// Phase sample size
        I2S_SampleSize_t sampleSize : 3;

        // Unused bits
        uint16_t unused1 : 13;

    } bits;
} I2S_PhaseCfg_t;

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Delay between start of phase and first data bit (expressed in SCLK cycles)
 * @param phase The phase (I2S_PHASE0 or I2S_PHASE1)
 * @param value The delay
 */
void I2S_HAL_SetDataDelay(I2S_Phase_t phase, uint16_t value);

/**
 * @brief Set the sample size for the given phase
 * @param phase The phase (I2S_PHASE0 or I2S_PHASE1)
 * @param value The sample size
 */
void I2S_HAL_SetSampleSize(I2S_Phase_t phase, I2S_SampleSize_t value);

/**
 * @brief Set the amount of TX channels mapped on phase (range : 0 to 32)
 * @param phase The phase (I2S_PHASE0 or I2S_PHASE1)
 * @param value The amount of TX channels
 */
void I2S_HAL_SetPhaseTxChannels(I2S_Phase_t phase, uint8_t value);

/**
 * @brief Set the amount of RX channels mapped on phase (range : 0 to 32)
 * @param phase The phase (I2S_PHASE0 or I2S_PHASE1)
 * @param value The amount of RX channels
 */
void I2S_HAL_SetPhaseRxChannels(I2S_Phase_t phase, uint8_t value);

/**
 * @brief Set the phase harware configuration
 * @param phase The phase (I2S_PHASE0 or I2S_PHASE1)
 * @param cfg The phase configuration
 */
void I2S_HAL_SetPhaseConfig(I2S_Phase_t phase, I2S_PhaseCfg_t cfg);

/**
 * @brief Get the phase harware configuration
 * @param phase The phase to get (I2S_PHASE0 or I2S_PHASE1)
 * @return The phase configuration
 */
I2S_PhaseCfg_t I2S_HAL_GetPhaseConfig(I2S_Phase_t phase);

/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Set the I2S master/slave mode configuration
 * @param mode The I2S mode
 */
static ALWAYS_INLINE void I2S_HAL_SetMode(I2S_Mode_t mode)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_MODE_MASK) | I2S_MODE((uint32_t) mode);
}

/**
 * @brief Set the I2S protocol
 * @param protocol The I2S protocol
 *
 * I2S Mode : I2S slaves immediately on incoming fsync (no frame debouncing)
 * TDM Mode : I2S trackes frame period (through debouncing). Start of phase data is aligned with start of phase
 * Right Justified mode : I2S tracks frame period (through debouncing). End of phase data is aligned with end of phase
 */
static ALWAYS_INLINE void I2S_HAL_SetProtocol(I2S_Protocol_t protocol)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_PROTOCOL_MASK) | I2S_PROTOCOL((uint32_t) protocol);
}

/**
 * @brief Set the clock phase of the serial clock
 * @param cpha Clock Phase
 *
 * false : Generation at falling edge of SCLK, capture at rising edge of SCLK
 * true : Generation at rising edge of SCLK, capture at falling edge of SCLK
 */
static ALWAYS_INLINE void I2S_HAL_SetCpha(bool cpha)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_CPHA_MASK) | I2S_CPHA((uint32_t) cpha);
}

/**
 * @brief Set the fsync active polarity
 * @param polarity Fsync active polarity
 *
 * false : Fsync polarity = 0.
 * Transition of fsync from 1 to 0 indicates the start of the frame, and the start of phase0
 *
 * true : Fsync polarity = 1.
 * Transition of fsync from 0 to 1 indicates the start of the frame, and the start of phase0
 */
static ALWAYS_INLINE void I2S_HAL_SetFsyncPolarity(bool polarity)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_FSYNC_POLARITY_MASK) | I2S_FSYNC_POLARITY((uint32_t) polarity);
}

/**
 * @brief Set the phase0 length mode
 * @param mode The phase length mode
 */
static ALWAYS_INLINE void I2S_HAL_SetPhaseLengthMode(I2S_PhaseLengthMode_t mode)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_PH_LENGTH_MODE_MASK) | I2S_PH_LENGTH_MODE((uint32_t) mode);
}

/**
 * @brief Set the LSBit first flag
 * @param en LSBit first flag
 *
 * false : Most significant bit of audio sample is transferred first
 * true : Least significant bit of audio sample is transferred first
 */
static ALWAYS_INLINE void I2S_HAL_SetLsbitFirst(bool en)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_LSBIT_FIRST_MASK) | I2S_LSBIT_FIRST((uint32_t) en);
}

/**
 * @brief Set the LSByte first flag
 * @param en LSByte first flag
 *
 * false : Most significant byte of audio sample is stored as most significant byte of sample in FIFO
 * true : Most significant byte of audio sample is stored as least significant byte of sample in FIFO
 */
static ALWAYS_INLINE void I2S_HAL_SetLsbyteFirst(bool en)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_LSBYTE_FIRST_MASK) | I2S_LSBYTE_FIRST((uint32_t) en);
}

/**
 * @brief Set the sign extension flag
 * @param en True to enable sign extension for 12 and 20 bits samples
 *
 * Apply sign extension when audio sample is not a multiple of 8-bits (only applicable for Rx direction)
 */
static ALWAYS_INLINE void I2S_HAL_SetSignExtendEnable(bool en)
{
    I2S->RegI2sCfg.r32 = (I2S->RegI2sCfg.r32 & ~I2S_SIGN_EXTEND_EN_MASK) | I2S_SIGN_EXTEND_EN((uint32_t) en);
}

/**
 * @brief Set the fsync length
 * @param value The length
 *
 * Amount of SCLK cycles the fsync signal keeps its active polarity value (only applicable in master mode)
 */
static ALWAYS_INLINE void I2S_HAL_SetFsyncLength(uint16_t value)
{
    I2S->RegI2sFsyncCfg.r32 = (I2S->RegI2sFsyncCfg.r32 & ~I2S_FSYNC_LENGTH_MASK) | I2S_FSYNC_LENGTH((uint32_t) value);
}

/**
 * @brief Get the fsync length
 * @return The length (amount of SCLK cycles the fsync signal keeps its active polarity value)
 */
static ALWAYS_INLINE uint16_t I2S_HAL_GetFsyncLength(void)
{
    return (uint16_t) GET_I2S_FSYNC_LENGTH(I2S->RegI2sFsyncCfg.r32);
}

/**
 * @brief Enable the I2S module
 */
static ALWAYS_INLINE void I2S_HAL_Enable(void)
{
    I2S->RegI2sCtrl.r32 |= I2S_EN(1);
}

/**
 * @brief Disable the I2S module
 */
static ALWAYS_INLINE void I2S_HAL_Disable(void)
{
    I2S->RegI2sCtrl.r32 &= ~I2S_EN(1);
}

/**
 * @brief Get I2S status (enabled or disabled)
 *
 * @return True if enable bit or clock(s) bit(s) is/are set
 */
static ALWAYS_INLINE bool I2S_HAL_IsI2SOrClocksEnabled(void)
{
    bool value= (GET_I2S_EN(I2S->RegI2sCtrl.r32) || GET_I2S_SCLK_EN(I2S->RegI2sCtrl.r32) ||
            GET_I2S_FSYNC_EN(I2S->RegI2sCtrl.r32) || GET_I2S_MCLK_EN(I2S->RegI2sCtrl.r32));
    return value;
}

/**
 * @brief Set the phase0 length in case it is configured by software
 * @param value The length (amount of SCLK cycles)
 */
static ALWAYS_INLINE void I2S_HAL_SetPhase0Length(uint16_t value)
{
    I2S->RegI2sPhLengthCfg.r32 = (I2S->RegI2sPhLengthCfg.r32 & ~I2S_PH0_LENGTH_MASK) | (I2S_PH0_LENGTH((uint32_t) value));
}

/**
 * @brief Get the phase0 length, available when configured by software
 * @return The length (amount of SCLK cycles)
 */
static ALWAYS_INLINE uint16_t I2S_HAL_GetPhase0Length(void)
{
    return (uint16_t) GET_I2S_PH0_LENGTH(I2S->RegI2sPhLengthCfg.r32);
}

/**
 * @brief Set the I2S harware configuration
 * @param cfg The I2S configuration
 */
static ALWAYS_INLINE void I2S_HAL_SetConfig(I2S_Cfg_t cfg)
{
    I2S->RegI2sCfg.r32 = cfg.cfgBits;
}

/**
 * @brief Get the I2S harware configuration
 * @return The I2S configuration
 */
static ALWAYS_INLINE I2S_Cfg_t I2S_HAL_GetConfig(void)
{
    I2S_Cfg_t cfg;
    cfg.cfgBits = I2S->RegI2sCfg.r32;
    return cfg;
}

/**
 * @brief Enable the Fsync clock
 */
static ALWAYS_INLINE void I2S_HAL_FSyncEnable(void)
{
    I2S->RegI2sCtrl.r32 |= I2S_FSYNC_EN(1);
}

/**
 * @brief Disable the Fsync clock
 */
static ALWAYS_INLINE void I2S_HAL_FSyncDisable(void)
{
    I2S->RegI2sCtrl.r32 &= ~I2S_FSYNC_EN(1);
}

/**
 * @brief Enable the serial clock
 */
static ALWAYS_INLINE void I2S_HAL_SClkEnable(void)
{
    I2S->RegI2sCtrl.r32 |= I2S_SCLK_EN(1);
}

/**
 * @brief Disable the serial clock
 */
static ALWAYS_INLINE void I2S_HAL_SClkDisable(void)
{
    I2S->RegI2sCtrl.r32 &= ~I2S_SCLK_EN(1);
}

/**
 * @brief Enable the master clock
 */
static ALWAYS_INLINE void I2S_HAL_MClkEnable(void)
{
    I2S->RegI2sCtrl.r32 |= I2S_MCLK_EN(1);
}

/**
 * @brief Disable the master clock
 */
static ALWAYS_INLINE void I2S_HAL_MClkDisable(void)
{
    I2S->RegI2sCtrl.r32 &= ~I2S_MCLK_EN(1);
}

/**
 * @brief Set the TX FIFO limit
 * @param limit The limit (0 to 16)
 */
static ALWAYS_INLINE void I2S_HAL_SetTxFifoLimit(uint8_t limit)
{
    I2S->RegI2sFifoCfg.r32 = (I2S->RegI2sFifoCfg.r32 & ~I2S_TX_FIFO_LIMIT_MASK) | (I2S_TX_FIFO_LIMIT((uint32_t) limit));
}

/**
 * @brief Set the RX FIFO limit
 * @param limit The limit (0 to 16)
 */
static ALWAYS_INLINE void I2S_HAL_SetRxFifoLimit(uint8_t limit)
{
    I2S->RegI2sFifoCfg.r32 = (I2S->RegI2sFifoCfg.r32 & ~I2S_RX_FIFO_LIMIT_MASK) | (I2S_RX_FIFO_LIMIT((uint32_t) limit));
}

/**
 * @brief Set the frame sync clock divider
 * This function sets the divider for the frame sync clock
 * @note The 16 LSBs are fractional (2.5 = 0x00028000)
 * @note System clock (48MHz or 24MHz) / div = clock frequency
 */
static ALWAYS_INLINE void I2S_HAL_SetFsyncDiv(uint32_t div)
{
    I2S->RegI2sFsyncDivCfg.r32 = (I2S->RegI2sFsyncDivCfg.r32 & ~I2S_FSYNC_DIV_MASK) | (I2S_FSYNC_DIV(div));
}

/**
 * @brief Set the serial clock divider
 * This function sets the divider for the serial clock
 * @note The 16 LSBs are fractional (2.5 = 0x00028000)
 * @note System clock (48MHz or 24MHz) / div = clock frequency
 */
static ALWAYS_INLINE void I2S_HAL_SetSClkDiv(uint32_t div)
{
    I2S->RegI2sSClkDivCfg.r32 = (I2S->RegI2sSClkDivCfg.r32 & ~I2S_SCLK_DIV_MASK) | (I2S_SCLK_DIV(div));
}

/**
 * @brief Set the master clock divider
 * This function sets the divider for the master clock
 * @note The 16 LSBs are fractional (2.5 = 0x00028000)
 * @note System clock (48MHz or 24MHz) / div = clock frequency
 */
static ALWAYS_INLINE void I2S_HAL_SetMClkDiv(uint32_t div)
{
    I2S->RegI2sMClkDivCfg.r32 = (I2S->RegI2sMClkDivCfg.r32 & ~I2S_MCLK_DIV_MASK) | (I2S_MCLK_DIV(div));
}

/**
 * @brief Get system clock frequency [Hz]
 *
 * @return System clock as uint32_t
 */
static ALWAYS_INLINE uint32_t I2S_HAL_GetSysClkFreq(void)
{
    // Get clock freq selection
    if(GET_CLK_SEL_FREQ(SYS->RegClkCfg.r32))
    {
        //48MHz
        return 48000000;
    }
    else
    {
        //24MHz
        return 24000000;
    }
}

/**
 * @brief Flush the RX FIFO
 * This function flushes the content of the RX FIFO
 */
static ALWAYS_INLINE void I2S_HAL_FlushRxFifo(void)
{
    I2S->RegI2sFifoCtrl.r32 = I2S_RX_FIFO_FLUSH(TRUE);
}

/**
 * @brief Flush the TX FIFO
 * This function flushes the content of the TX FIFO
 */
static ALWAYS_INLINE void I2S_HAL_FlushTxFifo(void)
{
    I2S->RegI2sFifoCtrl.r32 = I2S_TX_FIFO_FLUSH(TRUE);
}

/**
 * @brief Get the TX FIFO empty flag
 */
static ALWAYS_INLINE bool I2S_HAL_IsTxFifoEmpty(void)
{
    return (bool) GET_I2S_TX_FIFO_EMPTY(I2S->RegI2sFifoSts.r32);
}

/**
 * @brief Get the RX FIFO empty flag
 */
static ALWAYS_INLINE bool I2S_HAL_IsRxFifoEmpty(void)
{
    return (bool) GET_I2S_RX_FIFO_EMPTY(I2S->RegI2sFifoSts.r32);
}

/** @} */ // End of group i2s_hal
/** @} */ // End of addtogroup drivers

#endif /* I2S_HAL_H */
