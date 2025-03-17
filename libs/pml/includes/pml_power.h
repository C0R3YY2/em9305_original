////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_power.h
///
/// @project    T9305
///
/// @brief      Power manager drivers
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
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


#ifndef _PML_POWER_H
#define _PML_POWER_H


#include "arc.h"
#include "types.h"
#include "pml.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup pml_power PML Power Driver
 * @brief PML Power Manager Driver.
 * @{
*/


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief VCC configuration for RF output power in StepDown mode
 */
typedef uint8_t stepDownVccLvlTable_t[8];


/**
 * @brief DCDC configuration in StepDown mode
 * Table stores [T1, T2] parameters
 */
typedef uint8_t dcdcTable_t[32];


/**
 * @brief PML Power configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// VCC configuration for RF output power in StepDown mode
    stepDownVccLvlTable_t   stepDownRfPowerPmlVccLvl;

    /// DCDC configuration in StepDown mode: RF off or RX, 4.7uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersRxL4;

    /// DCDC configuration in StepDown mode: TX 0dBm, 4.7uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx0dBmL4;

    /// DCDC configuration in StepDown mode: TX 3dBm, 4.7uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx3dBmL4;

    /// DCDC configuration in StepDown mode: TX 6dBm, 4.7uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx6dBmL4;

    /// DCDC configuration in StepDown mode: RF off or RX, 2.2uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersRxL2;

    /// DCDC configuration in StepDown mode: TX 0dBm, 2.2uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx0dBmL2;

    /// DCDC configuration in StepDown mode: TX 3dBm, 2.2uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx3dBmL2;

    /// DCDC configuration in StepDown mode: TX 6dBm, 2.2uH coil
    /// Used for both 24 and 48MHz CPU clock
    dcdcTable_t     dcdcStepDownParametersTx6dBmL2;

    /// DCDC configuration for StepUp mode with 2.2uH coil
    dcdcTable_t     dcdcStepUpParametersL2;

    /// DCDC configuration for StepUp mode with 4.7uH coil
    dcdcTable_t     dcdcStepUpParametersL4;

} PML_PowerConfiguration_t;


/**
 * @brief structure holding LDO VCC levels
 */
typedef enum
{
    /// LDO VCC 1.15V
    PML_LDO_VCC_1150 = 0x00,
    /// LDO VCC 1.25V
    PML_LDO_VCC_1250 = 0x01,
    /// LDO VCC 1.70V
    PML_LDO_VCC_1700 = 0x02,
    /// LDO VCC 2.50V
    PML_LDO_VCC_2500 = 0x03,
} PML_LdoVccLvl_t;


/**
 * @brief structure holding LDO DIG levels
 */
typedef enum
{
    /// LDO DIG 0.9V
    PML_LDO_DIG_0900 = 0x00,
    /// LDO DIG 1.0V
    PML_LDO_DIG_1000 = 0x01,
} PML_LdoDigLvl_t;


/**
 * @brief structure to enable VDD domains
 */
typedef enum
{
    /// Less than 0dBm output power of RF
    PML_RF_POWER_BELOW_0DBM = 0,
    /// 0dBm output power of RF
    PML_RF_POWER_0DBM       = 1,
    /// 3dBm output power of RF
    PML_RF_POWER_3DBM       = 2,
    /// 4dBm output power of RF
    PML_RF_POWER_4DBM       = 3,
    /// 5dBm output power of RF
    PML_RF_POWER_5DBM       = 4,
    /// 6dBm output power of RF
    PML_RF_POWER_6DBM       = 5,
    /// 8dBm output power of RF
    PML_RF_POWER_8DBM       = 6,
    /// 10dBm output power of RF
    PML_RF_POWER_10DBM      = 7,
} PML_RfPower_t;


/**
 * @brief Supported coil value
 */
typedef enum
{
    /// 2.2uH coil
    COIL_2_2_UH = 0x00,
    /// 4.7uH coil
    COIL_4_7_UH = 0x01,
} PML_Coil_t;


/**
 * @brief structure to enable VDD domains
 */
typedef struct
{
    /// VCC level
    uint8_t        *rfPmlVccLvl;
    /// DCDC parameters RX
    uint8_t        *rfPmlDCDCParamRx;
    /// DCDC parameters TX
    uint8_t        *rfPmlDCDCParamTx;
    /// RF power
    PML_RfPower_t  rfPower;
    /// Coil value
    PML_Coil_t      coil;
} PML_RfPowerSetting_t;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Enable DCDC
 * @param enable TRUE Enable DCDC, FALSE Disable
 * @note It should be used in Step-Up mode
 */
void PML_SetDcdcBoostEnable(bool enable);


/**
 * @brief Enable Voltage Doubler
 * @param enable TRUE Enable voltage doubler, FALSE Disable
 * @note It should be used in External-Up mode with
 * voltage doubler
 */
void PML_SetVoltageDoublerEnable(bool enable);


/**
 * @brief Set the voltage doubler init (active low)
 * @param initn Init (active low)
 */
void PML_SetVoltageDoublerInitn(bool initn);


/**
 * @brief Set the voltage doubler mode
 * @param mode The voltage doubler mode according to following values:
 *
 * * 0: 1.05v <= voltage < 1.9v optional : Switches open
 * * 1: 1.4v <= voltage < 1.75v, SC ratio 1.5x
 * * 2: voltage < 1.4v,          SC ratio 2x
 * * 3: voltage >= 1.75v         direct, top switches on, direct mode
 * @note Mode 1 shall not be used anymore, use Mode 2 instead of 1.
 */
void PML_SetVoltageDoublerMode(uint8_t mode);


/**
 * @brief Enable CPU reset when a NVM brown-out occurs
 * @param enable TRUE Enable CPU reset when NVM brown-out, FALSE Disable
 */
void PML_SetNvmBrownOutResetEnable(bool enable);


/**
 * @brief Set DCDC parameters
 * @param t1_rx T1 time in DCDC for RX operation
 * @param t2_rx T2 time in DCDC for RX operation
 * @param t1_tx T1 time in DCDC for TX operation
 * @param t2_tx T2 time in DCDC for TX operation
 * @return true if the timings have changed
 */
bool PML_SetDcdcTimingConst(uint8_t t1_rx, uint8_t t2_rx, uint8_t t1_tx, uint8_t t2_tx);


/**
 * @brief Enable/Disable LDO VCC
 * @param enable TRUE enables LDO VCC, FALSE disable it
 */
void PML_SetLdoVccEnable(bool enable);


/**
 * @brief Enable/Disable LDO VCC to be used in radio TX operation
 * @param enable TRUE enables LDO VCC to be used in radio TX operation, FALSE disable it
 */
void PML_SetLdoVccTxEnable(bool enable);


/**
 * @brief Set the LDO_VCC level
 * @param lvl The level to set
 */
void PML_SetLdoVccLevel(PML_LdoVccLvl_t lvl);


/**
 * @brief Set the LDO_VCC TX level
 * @param lvl The level to set
 */
void PML_SetLdoVccTxLevel(PML_LdoVccLvl_t lvl);


/**
 * @brief Enable/disable Sleep bypass switch
 * @param enable TRUE enables Bypass switch, FALSE disable it.
 */
void PML_SetSleepBypassEnable(bool enable);


/**
 * @brief Set power management to meet request for RF output power
 * @param reqRfPower Requested RF output power
 * @returns RF output power which was actually set. It might be lower value than
 * requested if battery is too low.
 */
PML_RfPower_t PML_SetRfOutputPower(PML_RfPower_t reqRfPower);


/**
 * @brief Updated DCDC parameters
 * @return true if DCDC timings have changed
 */
bool PML_UpdateDCDC(void);


/**
 * @brief Update switch cap doubler mode
 */
void PML_UpdateScDbMode(void);


/**
 * @brief Get currently configured RF output power in power management
 * @returns RF output power which is currently used. It might be lower than
 * previously requested value.
 */
PML_RfPower_t PML_GetCurrentRfOutputPower(void);


/**
 * @brief Get maximum RF output power currently available
 * @returns Maximum RF output power currently available
 */
PML_RfPower_t PML_GetMaximumRfOutputPower(void);


/**
 * @brief Helper function to convert PML RF power into RF power in dBm units.
 * @param pmlPower PML RF power - see @ref PML_RfPower_t.
 * @returns RF power in dBm units.
 */
int8_t PML_PmlRfPower2Dbm(PML_RfPower_t pmlPower);


/**
 * @brief Helper function to convert RF power in dBm units into PML RF power.
 * @param rfPower RF power in dBm units.
 * @returns PML RF power - see @ref PML_RfPower_t.
 */
PML_RfPower_t PML_Dbm2PmlRfPower(int8_t rfPower);


/**
 * @brief Power down NVM and sleep with mode
 * @param mode Type of sleep to decide if the nvm needs to be powered down.
 */
void PML_PowerDownNvmAndSleep(uint32_t mode);


/**
 * @brief Power up the NVM
 * @returns always True
 */
bool PML_PowerUpNvm(void);


/**
 * @brief Power down the NVM
 */
void PML_PowerDownNvm(void);


/**
 * @brief Reset the NVM
 */
void PML_ResetNvm(void);

/**
 * @brief Set DCDC Coil value to use correct DCDC Timings.
 * @param coil Coil value of the system
 * @note At power up the default value for the coil is COIL_4_7_H
 */
void PML_SetDcdcCoil(PML_Coil_t coil);


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/


/**
 * @brief Set the voltage doubler PWM value
 * @param value The voltage doubler PWM value
 */
static ALWAYS_INLINE void PML_SetVoltageDoublerPwmValue(uint8_t value)
{
    PML->RegPmlDCDCCtrl.regs.PmlScDbPwm = value;
}


/**
 * @brief Get the LDO_VCC level
 * @return LDO_VCC level
 */
static ALWAYS_INLINE uint8_t PML_GetLdoVccLevel(void)
{
    uint8_t value =GET_PML_LDO_VCC_LVL_R(PML->RegPmlLvl.regs.PmlLdoVcc);
    return value;
}


/**
 * @brief Set the LDO_DIG level
 * @param lvl The level to set
 */
static ALWAYS_INLINE void PML_SetLdoDigLevel(PML_LdoDigLvl_t lvl)
{
    // @note No need to write it in a safe way.
    PML->RegPmlLvl.regs.PmlLdoDig = (uint8_t)lvl;
}


/**
 * @brief Get the LDO_DIG level
 * @return LDO_DIG level
 */
static ALWAYS_INLINE uint8_t PML_GetLdoDigLevel(void)
{
    return GET_PML_LDO_DIG_LVL_R(PML->RegPmlLvl.regs.PmlLdoDig);
}


/**
 * @brief Get DCDC boost enable
 * @return true if enabled
 */
static ALWAYS_INLINE bool PML_GetDcdcBoostEnable(void)
{
    return (bool)GET_PML_DCDC_BOOST_EN_R(PML->RegPmlDCDCCtrl.regs.PmlDcdcCtrl);
}


/**
 * @brief Get Voltage doubler enable
 * @return true if enabled
 */
static ALWAYS_INLINE bool PML_GetVoltageDoublerEnable(void)
{
    return (bool)GET_PML_SC_DB_EN_R(PML->RegPmlDCDCCtrl.regs.PmlScDbCtrl);
}


/** @} */ // End of group pml_power
/** @} */ // End of addtogroup drivers


#endif // _PML_POWER_H
