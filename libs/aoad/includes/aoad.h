////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/aoad/includes/aoad.h
///
/// @project    T9305
///
/// @brief      AOAD drivers
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
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


#ifndef _AOAD_H
#define _AOAD_H

#include "driver.h"
#include "config.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup aoad AOAC Driver
 * @brief Driver for the AOAC.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// GPIO Selection.
typedef union
{
    /// GPIO Selection config bytes.
    uint32_t config;

    /// bit access
    struct
    {
        /// GPIO Out Sel 0, defines which LUT bit is selected to GPIO0
        uint32_t gpio_out_sel0           : 3;
        /// reserved.
        uint32_t reserved1               : 5;
        /// GPIO Out Sel 1, defines which LUT bit is selected to GPIO1
        uint32_t gpio_out_sel1           : 3;
        /// reserved.
        uint32_t reserved2               : 5;
        /// GPIO Out Sel 2, defines which LUT bit is selected to GPIO2
        uint32_t gpio_out_sel2           : 3;
        /// reserved.
        uint32_t reserved3               : 5;
        /// GPIO Out Sel 3, defines which LUT bit is selected to GPIO3
        uint32_t gpio_out_sel3           : 3;
        /// reserved.
        uint32_t reserved4               : 5;
    } bits;
} GPIO_Sel0_t;

/// GPIO Selection.
typedef union
{
    /// GPIO Selection config bytes.
    uint32_t config;

    /// bit access
    struct
    {
        /// GPIO Out Sel 4, defines which LUT bit is selected to GPIO4
        uint32_t gpio_out_sel4           : 3;
        /// reserved.
        uint32_t reserved1               : 5;
        /// GPIO Out Sel 5, defines which LUT bit is selected to GPIO5
        uint32_t gpio_out_sel5           : 3;
        /// reserved.
        uint32_t reserved2               : 5;
        /// GPIO Out Sel 6, defines which LUT bit is selected to GPIO6
        uint32_t gpio_out_sel6           : 3;
        /// reserved.
        uint32_t reserved3               : 5;
        /// GPIO Out Sel 7, defines which LUT bit is selected to GPIO7
        uint32_t gpio_out_sel7           : 3;
        /// reserved.
        uint32_t reserved4               : 5;
    } bits;
} GPIO_Sel1_t;

/// GPIO Selection.
typedef union
{
    /// GPIO Selection config bytes.
    uint32_t config;

    /// bit access
    struct
    {
        /// GPIO Out Sel 8, defines which LUT bit is selected to GPIO8
        uint32_t gpio_out_sel8           : 3;
        /// reserved.
        uint32_t reserved1               : 5;
        /// GPIO Out Sel 9, defines which LUT bit is selected to GPIO9
        uint32_t gpio_out_sel9           : 3;
        /// reserved.
        uint32_t reserved2               : 5;
        /// GPIO Out Sel 10, defines which LUT bit is selected to GPIO10
        uint32_t gpio_out_sel10          : 3;
        /// reserved.
        uint32_t reserved3               : 5;
        /// GPIO Out Sel 11, defines which LUT bit is selected to GPIO11
        uint32_t gpio_out_sel11          : 3;
        /// reserved.
        uint32_t reserved4               : 5;
    } bits;
} GPIO_Sel2_t;

/**
 * @brief AOAD configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// GPIO Sel 0, Defines which LUT bit is selected for GPIO0 to GPIO3
    GPIO_Sel0_t gpio_sel0;
    /// GPIO Sel 1, Defines which LUT bit is selected for GPIO4 to GPIO7
    GPIO_Sel1_t gpio_sel1;
    /// GPIO Sel 2, Defines which LUT bit is selected for GPIO8 to GPIO11
    GPIO_Sel2_t gpio_sel2;

    /// Table of GPIO used for AOAD
    bool gpio_for_aoad[12];

    /// If the driver is enabled or not.
    uint8_t enabled;
} AOAD_Configuration_t;

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/******************************************************************************\
 *  STRUCTURES
\******************************************************************************/

/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Add the AOAD module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void AOAD_RegisterModule(void);

/**
 * @brief Update configuration register from configuration structure
*/
void AOAD_Update(void);

/**
 * @brief Set Look Up table for Antenna Switch
 * @param antIdRef antenna id for reference period
 * @param pAntId antenna id pattern
 * @param swPattLen length of antenna id pattern
*/
void AOAD_SetMatrixLookUpTable(uint8_t antIdRef, const uint8_t* pAntId, uint8_t swPattLen);

/**
 * @brief Enable / Disable Antenna Switch matrix
 * @param antennaSwitchEnable enable antenna switching
 * @param aoaRx2us true if switching time period is 2 us
 * @param cteType CTE type, 0 = AoA, 1 = AoD 1 us, 2 = AoD 2 us
 * @param dataChannelPdu If true Data chanel PDU format is used to decode
 * CTEInfo field in packet, otherwise Advertising channel PDU format is used to
 * decode CTEInfo field in packet.
*/
static ALWAYS_INLINE void AOAD_AntennaSwitchEnable(bool antennaSwitchEnable, bool aoaRx2us,
    uint8_t cteType, bool dataChannelPdu)
{
    // CTE type configuration
    PP->RegPpBleCteCfg.r32 =
        (PP->RegPpBleCteCfg.r32 & (PP_BLE_CTE_RX_EN_MASK | PP_BLE_CTE_TX_EN_MASK)) |
        PP_BLE_CTE_PDU_TYPE((uint8_t)dataChannelPdu) |
        PP_BLE_CTE_IGNORE_CRC(0) |
        PP_BLE_CTE_IQ_ORDER(0) |
        PP_BLE_CTE_AOA_PERIOD((uint8_t)aoaRx2us);

    if (cteType == 0)
    {
        // AoA
        AOAD->RegAoadCtrl.r32 =
            AOAD_EN((uint8_t)antennaSwitchEnable) | // Enable/Disable
            AOAD_CTRL_MODE(0) |                     // HW control of Switch Matrix
            AOAD_REF_TIME(4)  |                     // Guard + Reference time = 10us ((1+4)*2)
            AOAD_SW_PER((uint8_t)aoaRx2us);
    }
    else
    {
        // AoD
        AOAD->RegAoadCtrl.r32 =
            AOAD_EN((uint8_t)antennaSwitchEnable) | // Enable/Disable
            AOAD_CTRL_MODE(0) |                     // HW control of Switch Matrix
            AOAD_REF_TIME(4)  |                     // Guard + Reference time = 10us ((1+4)*2)
            AOAD_SW_PER((uint8_t)(cteType - 1));
    }
}

/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/** @} */ // End of group aoad
/** @} */ // End of addtogroup drivers


#endif /* _AOAD_H */
