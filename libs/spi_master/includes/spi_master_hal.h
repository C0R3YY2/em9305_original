////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/spi_master/includes/spi_master_hal.h
///
/// @project    T9305
///
/// @brief      SPI master hardware abstraction layer.
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


#ifndef SPI_MASTER_HAL_H
#define SPI_MASTER_HAL_H


#include <t9305_periph.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup spim_hal SPI Master HAL Driver
 * @brief HAL Driver for the SPI master.
 *
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// Maximum number of bytes that are sent in a single transaction.
#define SPIM_MAX_TRANS_LENGTH (16)


/// Clock overhead for scheduling (in milliseconds).
#define SPIM_TIMER_OVERHEAD   (0.0f)


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief Prescaler value to divide the clock source by.
 */
typedef enum
{
    SPIM_PRESCALER_2 = 0,
    SPIM_PRESCALER_3,
    SPIM_PRESCALER_4,
    SPIM_PRESCALER_6,
    SPIM_PRESCALER_8,
    SPIM_PRESCALER_16,
    SPIM_PRESCALER_32,
    SPIM_PRESCALER_64,
} SPIM_Prescaler_t;


/**
 * @brief SPI Master mode (3 or 4-wires).
 */
typedef enum
{
    SPIM_MODE_4_WIRES = 0,
    SPIM_MODE_3_WIRES,
} SPIM_Mode_t;


/**
 * @brief SPI master configuration bits.
 */
//lint ++flb some unreferenced members, treat as library
typedef union
{
    /// SPI master config byte.
    uint8_t configByte;

    /// bit access
    struct
    {
        //lint -e46 Allow uint8_t as bitfield type

        /// Clock prescaler.
        SPIM_Prescaler_t prescaler : 3;

        /// Clock phase.
        uint8_t cpha     : 1;

        /// Clock polarity.
        uint8_t cpol     : 1;

        /// Send/receive data with the most significant bit (MSB) first.
        uint8_t msbFirst : 1;

        /// SPI mode, 4 or 3 wires.
        SPIM_Mode_t spiMode : 1;

        /// Invert RX phase clock.
        uint8_t rxPhase : 1;

        //lint +e46
    } bits;
} SPIM_Config_t;
//lint --flb


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Determines if the SPI master is currently in use.
 * @retval true  The SPI master is in use.
 * @retval false The SPI master is available.
 */
static ALWAYS_INLINE bool SPIM_HAL_IsBusy(void)
{
    return SPIM_BUSY(SPIM->RegSPIMStat.r32);
}


/**
 * @brief Start a SPI master transaction.
 * @param length Number of bytes in the transaction.
 */
static void ALWAYS_INLINE SPIM_HAL_StartTransaction(uint8_t length)
{
    SPIM->RegSPIMCtrl.r32 = SPIM_TRANS_LENGTH(length) |
        SPIM_START_TRANS(1);
}


/**
 * @brief Set the clock prescaler to the desired value.
 * @param prescaler The clock prescaler.
 */
static void ALWAYS_INLINE SPIM_HAL_SetPrescaler(SPIM_Prescaler_t prescaler)
{
    SPIM->RegSPIMCfg.r32 = (SPIM->RegSPIMCfg.r32 & ~SPIM_CLOCK_SELECT_MASK) |
            SPIM_CLOCK_SELECT(prescaler);
}


/**
 * @brief Get the clock prescaler value.
 * @returns The clock prescaler.
 */
static ALWAYS_INLINE SPIM_Prescaler_t SPIM_HAL_GetPrescaler(void)
{
    return (SPIM_Prescaler_t)GET_SPIM_CLOCK_SELECT(SPIM->RegSPIMCfg.r32);
}


/**
 * @brief Enable SPI master with configuration register to the desired value.
 * @param configByte The configuration to set.
 */
static void ALWAYS_INLINE SPIM_HAL_EnableWithConfig(uint8_t configByte)
{
    SPIM->RegSPIMCfg.r32 = SPIM_ENABLE(1) | configByte;
}


/**
 * @brief Set the SPI master configuration register to the desired value.
 * @param config The configuration to set.
 * @warning Do not call this function during a transfer.
 */
static void ALWAYS_INLINE SPIM_HAL_SetConfigurationRegister(SPIM_Config_t config)
{
    SPIM->RegSPIMCfg.r8[0] = config.configByte;
}


/**
 * @brief Set Read/Write Sel (for 3 wires mode).
 * @param rw Read=1, Write=0.
 */
static ALWAYS_INLINE void SPIM_HAL_SetRWSel(uint8_t rw)
{
    // SPIMRwSel: 0=write, 1=read
    SPIM->RegSPIMCfg.r32 = (SPIM->RegSPIMCfg.r32 & ~SPIM_RW_SEL_MASK) | SPIM_RW_SEL(rw);
}


/** @} */ // End of group spim_hal
/** @} */ // End of addtogroup drivers


#endif /* SPI_MASTER_HAL_H */
