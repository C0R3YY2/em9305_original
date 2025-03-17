////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/radio/includes/radio_private.h
///
/// @project    T9305
///
/// @brief      Radio internal header for driver.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic
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


#ifndef _RADIO_INTERNAL_H
#define _RADIO_INTERNAL_H

#include <config.h>
#include <types.h>
#include <t9305_periph.h>
#include <arc.h>
#include <radio.h>
#include <radio_config.h>

/**
 * @cond Internal use only
 */

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// Tx was the last BLE operation
#define WAS_TX TRUE

/// Rx was the last BLE operation
#define WAS_RX FALSE

/// Use radio config in NVM
#ifndef RADIO_CONFIG_IN_NVM
#define RADIO_CONFIG_IN_NVM (0)
#endif

#if (1 == RADIO_CONFIG_IN_NVM)
#include "memory_map.h"
#define NVM_INFO_P1_RADIO_FECTRIM           (MEMORY_NVMINFO_START + 0x2000)
#define NVM_INFO_P1_RADIO_CONFIG            (MEMORY_NVMINFO_START + 0x2080)
#define NVM_INFO_P1_RADIO_RSSI_GAIN         (MEMORY_NVMINFO_START + 0x2200)
#define NVM_INFO_P1_RADIO_RSSI_GAIN_DATA    (NVM_INFO_P1_RADIO_RSSI_GAIN + 8)
#define NVM_INFO_P1_RADIO_CONFIG_FLAG       (0x55AA3C3C)
#endif // (1 == RADIO_CONFING_IN_NVM)

// RF timing constant
// Timing is taken from:
// 12_external_doc\ip_radio\From Aura\Google_drive\docs\Radio board documents\AU5060B_SPI_Writes.pdf
// 12_external_doc\ip_radio\From Aura\Google_drive\docs\Radio board documents\AU5060B_Radio_Board_User_Guide.pdf
// Timing granularity is 0.25us

/// RF RX mode start-up time [us]
#define RF_RX_START_TIME                    (85)        // Taken from RF IP docs
/// RF TX mode start-up time [us]
#define RF_TX_START_TIME                    (112.5)     // Taken from RF IP docs
/// RF RX stop time [us]
#define RF_RX_STOP_TIME                     (2.5)       // @todo RF: estimation
/// RF RX stop time [us]
#define RF_TX_STOP_TIME                     (2.5)       // @todo RF: estimation

/**
 * @brief Size of the RF transmit FIFO (in bytes).
 */
#define RF_TX_FIFO_DEPTH        (16)

/**
 * @brief Size of the RF receive FIFO (in bytes).
 */
#define RF_RX_FIFO_DEPTH        (32)

/// default value of RegRfModeCfgTxrx register
#define RF_MODE_CFG_TX_RX 0x8A04

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// Function pointer prototype for optimized interruption
/// routine for specific protocol
typedef void (*RadioIsrCallback_t)(void);

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief Last Tx / Rx operation configuration of the driver.
 */
typedef enum
{
    NONE        = 0,
    TX_154_MODE = 1,
    RX_154_MODE = 2,
    BLE_MODE    = 3
} TXRX_Mode_t;

/// Data structure holding the information needed for a RF transaction.
typedef struct
{
    /// Pointer to the data buffer having the data to be sent or received
    uint8_t* pData;
    /// Function to call once RF transaction (send/receive) is finished
    RF_TransactionCallback_t pCallback;
    /// Default function to call once RF transaction (send/receive) is finished
    RF_TransactionCallback_t pDefaultCallback;
    /// Number of bytes in data buffer to be sent in TX mode, maximum number
    /// of bytes to be received in RX mode
    uint16_t dataLength;
    /// Actual number of bytes which were sent in TX mode or received in RX mode
    uint16_t actualLength;
} RF_Transaction_t;

/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

#if (1 == FPGA_BUILD)
/**
 * @brief Radio test chip configuration - Au5060B
 */
extern const RF_config_t cRF_configAu5060B;

/**
 * @brief Radio test chip configuration - Au5070A
 */
extern const RF_config_t cRF_configAu5070A;

/**
 * @brief Radio test chip configuration - Au5080B
 */
extern const RF_config_t cRF_configAu5080B;

/// RF test chip pointer to configuration
extern  RF_config_t* gRfFpgaConfig;
#endif // (1 == FPGA_BUILD)

extern int16_t rx_switch_time_delta;
extern int16_t rx_sample_time_delta;
extern int16_t tx_switch_time_delta;

extern uint32_t regRfModeCfgTxrxCache;
extern bool enableLnaProtection;

extern volatile RF_Transaction_t gRfRxTransaction;
extern volatile RF_Transaction_t gRfTxTransaction;

/******************************************************************************\
 *  INTERNAL FUNCTIONS
\******************************************************************************/

/**
 * @brief Set the RX FIFO limit.
 * The RX FIFO limit affects when an IRQ is generated. Once the RX FIFO
 * contains more bytes than the RX FIFO limit, an IRQ is generated.
 * @param fifoLimit RX FIFO limit to be set.
 */
static ALWAYS_INLINE void RF_SetRxFifoLimit(uint8_t fifoLimit)
{
    PP->RegPpFifoCfg.regs.PpRxFifoLimit = fifoLimit;
}

/**
 * @brief Set the TX FIFO limit.
 * The TX FIFO limit affects when an IRQ is generated. Once the TX FIFO
 * contains less bytes than the TX FIFO limit, an IRQ is generated.
 * @param fifoLimit TX FIFO limit to be set.
 */
static ALWAYS_INLINE void RF_SetTxFifoLimit(uint8_t fifoLimit)
{
    PP->RegPpFifoCfg.regs.PpTxFifoLimit = fifoLimit;
}

/**
 * @brief Flush RF RX FIFO.
 * This function flushes the content of the RX FIFO.
 * @note No check is performed if the RF is running or not.
 */
static ALWAYS_INLINE void RF_FlushRxFifo(void)
{
    PP->RegPpFifoCmd.r32 = PP_RX_FIFO_FLUSH(1);
}

/**
 * @brief Flush RF TX FIFO.
 * This function flushes the content of the TX FIFO.
 * @note No check is performed if the RF is running or not.
 */
static ALWAYS_INLINE void RF_FlushTxFifo(void)
{
    PP->RegPpFifoCmd.r32 = PP_TX_FIFO_FLUSH(1);
}

#if (1 == FPGA_BUILD)
/**
 * @brief Configure XTAL oscillator in radio test chip.
 */
void RF_FpgaConfigXtalOsc(void);
#endif // (1 == FPGA_BUILD)

/**
 * @brief Write Radio register value, this operation start a SPI transaction
 * between RF controller and radio. The function return only when the
 * transaction is finished.
 * @param address address of radio register
 * @param data date to write in radio register
 */
void RF_WriteReg(uint8_t address, uint16_t data);

/**
 * @brief Set BLE CTE Rx Timing (switch & sample)
 * @param switchTime switch time
 * @param sampleTime sample time
 */
void RF_BleCteSetRxTiming(int16_t switchTime, int16_t sampleTime);

/**
 * @brief Set BLE CTE Tx Timing (switch)
 * @param switchTime switch time
 */
void RF_BleCteSetTxTiming(int16_t switchTime);

/**
 * @brief Try to Apply RF Output power requested
 * @param power Request RF output power
 * @return Actual used RF Output power
 */
int8_t RF_ApplyOutputPower(int8_t power);

/**
 * @brief Natural logarithm implementation using polynomial approximation
 * @param x Function argument
 * @returns Calculated logarithm value (ln(x)).
 */
float RF_Ln(uint32_t x);

/**
 * @brief Update LDO ANT and LNA HATTN for next radio operation
 * @param was_tx true if the last operation was a transmit otherwise false
 */
void RF_UpdateLdoAntConfig(bool was_tx);

/**
 * @brief Update LNA protection flag from current RF output power used
 * @param rfPowerLevelConfigIndex index of RF output power used
 */
void RF_UpdateLNAProtectionFlag(uint8_t rfPowerLevelConfigIndex);

/******************************************************************************\
 *  INTERNAL FUNCTIONS USED BY ISR
\******************************************************************************/

/**
 * @brief Configure Radio ISR
 */
void RF_IrqConfigure(void);

/**
 * @endcond
 */

#endif //_RADIO_INTERNAL_H
