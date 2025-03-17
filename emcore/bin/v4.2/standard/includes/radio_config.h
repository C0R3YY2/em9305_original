////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/radio/includes/radio_config.h
///
/// @project    T9305
///
/// @brief      Radio drivers - radio configuration data.
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


#ifndef _RADIO_CONFIG_H
#define _RADIO_CONFIG_H


#include <types.h>


/**
 * @addtogroup radio
 * @{
 */


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// Default value of RegRfModeCfgTxrx register
#define RF_MODE_CFG_TX_RX 0x8A04

/******************************************************************************\
 *  MACROS
\******************************************************************************/



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/// RF single mode configuration - pairs [address, data]
typedef struct
{
    /// RX mode configuration
    uint16_t    rx[3*2];
    /// TX mode configuration
    uint16_t    tx[1*2];
} RF_modeConfig_t;


/// RF modes configuration structure
typedef struct
{
    /// RF configuration for BLE 1M (1Mbps)
    RF_modeConfig_t     ble1M;
    /// RF configuration for BLE 2M (2Mbps)
    RF_modeConfig_t     ble2M;
    /// RF configuration for BLE 1M Coded S=8
    RF_modeConfig_t     ble1M_S8;
    /// RF configuration for BLE 1M Coded S=2
    RF_modeConfig_t     ble1M_S2;
} RF_modesConfig_t;


/// RSSI gain data
typedef struct
{
    /// RSSI gain for BLE 1M (1Mbps)
    uint16_t            ble1M[6];
    /// RSSI gain for BLE 2M (2Mbps)
    uint16_t            ble2M[6];
} RF_rssiGain_t;


/// RF master configuration structure
typedef struct
{
    /// RF configuration (one time configuration)
    uint16_t            config[2*20];
    /// RF front end capacitor trimming RX/TX for BLE channels
    uint8_t             feCTrim[40];
    /// RF front end capacitor trimming RX/TX for 802.15.4 channels
    uint8_t             feCTrim154[40];
    /// RSSI gain data
    RF_rssiGain_t       rssiGain;
    /// Modes configuration
    RF_modesConfig_t    modeConfig;
} RF_config_t;


/// MAC timing parameters for RX->TX and TX->RX transitions
typedef struct
{
    /// RX->TX transition
    // rx2tx[rx_mode][tx_mode]
    uint16_t   rx2tx[4][4];
    /// TX->RX transition
    // tx2rx[tx_mode][rx_mode]
    uint16_t   tx2rx[4][4];
    /// TX->TX transition
    // tx2tx[tx_mode]
    uint16_t   tx2tx[4];
    /// RX->RX transition
    // rx2rx[rx_mode]
    uint16_t   rx2rx[4];
} RF_MacTimingRxTxModes_t;


/// MAC timing parameters master structure
typedef struct
{
    /// 48MHz operation
    RF_MacTimingRxTxModes_t freq48MHz;
} RF_MacTiming_t;


/// BLE CTE timing for RX and TX operation
typedef struct
{
    /// CTE TX switch time
    int16_t txSwitchTime;
    /// CTE RX switch time
    int16_t rxSwitchTime;
    /// CTE RX sample time
    int16_t rxSampleTime;
} RF_BleCteTimingRxTx_t;


/// BLE CTE timing for 1M and 2M modes
typedef struct
{
    /// CTE timing for 1Mbps
    RF_BleCteTimingRxTx_t ble1M;
    /// CTE timing for 2Mbps
    RF_BleCteTimingRxTx_t ble2M;
} RF_BleCteTimingModes_t;


/// BLE CTE timing
typedef struct
{
    /// CTE timing for 48MHz
    RF_BleCteTimingModes_t freq48MHz;
} RF_BleCteTiming_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

/**
 * @brief Radio IP configuration - Au5070CE
 */
extern const RF_config_t cRF_config;

/**
 * @brief Radio MAC timing parameters master structure
 */
extern const RF_MacTiming_t cRF_MacTiming;

/**
 * @brief Radio BLE CTE timing parameters master structure
 */
extern const RF_BleCteTiming_t cRF_BleCteTiming;

/**
 * @brief RF output power configuration data
 * Table with configuration data for RF output power levels and actual RF power
 * in dBm units. It contains configuration data for RF output power in range
 * from -57dBm up to +10dBm with step of 1dBm.
 * Each power level has 3 configuration parameters:
 * RF power [dBm],  LDO_ANT_TRIMM value,  TX_PA_PWR value
 */
extern const int8_t cRF_powerLevelConfig[68][3];

/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/


/** @} */ // End addtogroup radio


#endif // _RADIO_CONFIG_H
