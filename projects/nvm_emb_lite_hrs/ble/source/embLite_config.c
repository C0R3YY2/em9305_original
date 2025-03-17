////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble_config.c
///
/// @project    T9305
///
/// @brief      EMBLite BLE Configuration Implementation
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
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

#include "ble.h"
#include <util\bstream.h>

/**************************************************************************************************
  Constants
**************************************************************************************************/
#define MAX_CONNECTIONS_ALLOWED     4
#define ATT_DESIRED_MTU_LENGTH      247
#define NUM_POOLS_SIZE  (sizeof(gMyBLEBuffPoolDesc) / sizeof(gMyBLEBuffPoolDesc[0]))

/// Buffer Pool runtime configuration. Must be manually tuned for each application
const wsfBufPoolDesc_t gMyBLEBuffPoolDesc[] =
{
    { .len = 16,  .num = 4 },
    { .len = 32,  .num = 15},
    { .len = 64,  .num = 8 },
    { .len = 112, .num = 4 },
    { .len = 272, .num = 7 },
    { .len = 340, .num = 1 }
};

/// Device Name
char gMyDeviceName[] = "EM9305_HRS";

/**
 * Data to be used for advertising packets. Each Advertising Data (AD) Element must be prefixed with
 * the element (excluding the actual length byte), an AD type from the BLE GAP Spec, followed by the data itself
 * Various AD type defintions and flags are found in the Device Manager API
 */
uint8_t gMyAdvertisingData[] =
{
    // Flags.
    2,                          // Length.
    DM_ADV_TYPE_FLAGS,          // AD type.
    DM_FLAG_LE_GENERAL_DISC |
    DM_FLAG_LE_BREDR_NOT_SUP,

    // TX Power.
    2,                          // Length.
    DM_ADV_TYPE_TX_POWER,       // AD type.
    0,

    // Service UUID list.
    9,                          // Length.
    DM_ADV_TYPE_16_UUID,        // AD type.
    UINT16_TO_BYTES(ATT_UUID_HEART_RATE_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_RUNNING_SPEED_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_DEVICE_INFO_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_BATTERY_SERVICE)
};

/// Data to be used for scan response packets when enabled. Follows the same structure as Advertising Data above
uint8_t gMyScanData[] =
{
    // Device name.
    11,                         // Length.
    DM_ADV_TYPE_LOCAL_NAME,     // AD type.
    'E', 'M', '9', '3', '0', '5', '_', 'H', 'R', 'S'
};

/// Parameters to request upon connection
EMBLite_ConnectionUpdateParams_t gMyConnectionUpdateParams =
{
    .connUpdatePeriod   = 5000,     // 5000ms
    .connIntervalMin    = 640,      // 800ms
    .connIntervalMax    = 800,      // 1000ms
    .connLatency        = 0,
    .supervisionTimeout = 900,      // 9000ms
    .maxUpdateAttempts  = 3
};

/// EMB Lite Configuration Structure
EMBLite_Config_t gMyEMBLite_Config =
{
    /// Device Related Configurations
    .deviceName                     = (uint8_t*)gMyDeviceName,
    .deviceNameLength               = sizeof(gMyDeviceName),
    .deviceBuffPoolDesc             = (wsfBufPoolDesc_t*)gMyBLEBuffPoolDesc,
    .deviceNumberofPools            = NUM_POOLS_SIZE,
    /// Advertising Related Configurations
    .advertisingDuration            = 0,    // Infinite duration
    .advertisingInterval            = 160,  // 100ms
    .advertisingData                = gMyAdvertisingData,
    .advertisingDataLength          = sizeof(gMyAdvertisingData),

    .advertisingIsScannable         = true,
    .advertisingScanData            = gMyScanData,
    .advertisingScanDataLength      = sizeof(gMyScanData),

    .advertisingIsDirected          = false,
    .advertisingDirectedPeerAddress = NULL,

    /// Connection Related Configurations
    .connectionsEnabled             = true,
    .connectionsMaxAllowed          = MAX_CONNECTIONS_ALLOWED,
    .connectionsDesiredMtuLength    = ATT_DESIRED_MTU_LENGTH,

    .connectionsRequestUpdate       = true,
    .requestedUpdateParams          = &gMyConnectionUpdateParams,

    .connectionsAllowUpdates        = true,
    .updateParamsAllowed            = &gMyConnectionUpdateParams,
};
