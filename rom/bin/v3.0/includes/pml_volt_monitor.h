////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_volt_monitor.h
///
/// @project    T9305
///
/// @brief      Voltage Monitor drivers
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


#ifndef _VOLT_MON_H
#define _VOLT_MON_H


#include "types.h"
#include "t9305_periph.h"
#include "pml.h"
#include "config.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup pml_voltmon PML voltage monitor
 * @brief PML voltage monitor.
 * @{
*/


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief Data structure holding flags
 */
typedef union
{
    /// Flag bit access
    struct
    {
        /// Limit RF output power to +8/10dBm
        bool flagLimitRf10dBm : 1;
        /// Limit RF output power to +6dBm
        bool flagLimitRf6dBm : 1;
        /// Limit RF output power to +5dBm
        bool flagLimitRf5dBm : 1;
        /// Limit RF output power to +4dBm
        bool flagLimitRf4dBm : 1;
        /// Limit RF output power to +3dBm
        bool flagLimitRf3dBm : 1;
        /// Limit RF output power to +0dBm
        bool flagLimitRf0dBm : 1;
        /// System run (opposite to shut-down in spec)
        bool flagSystemRun : 1;
        /// Reserved bits
        uint8_t reserved : 3;
    } bits;

    /// Flags byte access
    uint8_t flags;
} VoltMon_Flags_t;


/**
 * @brief Voltage limit values for different RF output power
 */
typedef struct
{
    /// Limit RF output power to +8/10dBm
    int8_t rf10dBm;
    /// Limit RF output power to +6dBm
    int8_t rf6dBm;
    /// Limit RF output power to +5dBm
    int8_t rf5dBm;
    /// Limit RF output power to +4dBm
    int8_t rf4dBm;
    /// Limit RF output power to +3dBm
    int8_t rf3dBm;
    /// Limit RF output power to +0dBm
    int8_t rf0dBm;
    /// System run (opposite to shut-down in spec)
    int8_t systemRun;
} VoltMon_LimitValues_t;


/**
 * @brief Voltage limits valid
 */
typedef union
{
    struct {
        /// Limit RF output power to +8/10dBm
        bool rf10dBm : 1;
        /// Limit RF output power to +6dBm
        bool rf6dBm : 1;
        /// Limit RF output power to +5dBm
        bool rf5dBm : 1;
        /// Limit RF output power to +4dBm
        bool rf4dBm : 1;
        /// Limit RF output power to +3dBm
        bool rf3dBm : 1;
        /// Limit RF output power to +0dBm
        bool rf0dBm : 1;
        /// System run (opposite to shut-down in spec)
        bool systemRun : 1;
        /// Reserved
        uint8_t reserved : 3;
    };
    /// LimitValid byte access
    uint8_t valids;
} VoltMon_LimitValid_t;


/**
 * @brief Data structure holding the voltage monitor limit configuration
 */
typedef struct
{
    /// Voltage limit values
    VoltMon_LimitValues_t limit;

    /// Voltage limits valid
    VoltMon_LimitValid_t valid;

    /// Number of all flags
    uint8_t totalFlagNumber;

} VoltMon_Limits_t;


/**
 * @brief PML configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Voltage measurement period in LF clock numbers. If time elapsed from
    /// last measurement is bigger than value defined here voltage measurement
    /// is performed during RF_Init().
    /// 0 means this value is ignored.
    uint32_t voltMonitorPeriod;
    /// delay for voltage comparator [number of loop] for 48MHz clock
    uint16_t comparatorDelay48MHz;
    /// delay for voltage comparator [number of loop] for 24MHz clock
    uint16_t comparatorDelay24MHz;
    /// Reserved
    uint16_t reserved;
} VoltMon_Configuration_t;


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/


/**
 * @brief
 *   Get VBAT1 comparator level.
 *
 * @details
 *   Function returns a level of VBAT1 comparator.
 *
 * @return level of VBAT1 comparator
 */
static ALWAYS_INLINE Voltage_t VoltMon_GetVbat1Lvl(void)
{
    return (Voltage_t)GET_PML_VBAT_LVL_R(PML->RegPmlLvl.regs.PmlVbat);
}


/**
 * @brief
 *   Get VBAT 1P7 comparator level.
 *
 * @details
 *   Function returns the level of VBAT 1P7 comparator.
 *
 * @return level of VBAT 1P7 comparator
 */
static ALWAYS_INLINE bool VoltMon_GetVbat1p7Lvl(void)
{
    return (Voltage_t)GET_PML_VBAT_1P7_LVL_R(PML->RegPmlLvl.regs.PmlVbat);
}


/**
 * @brief
 *   Get VCC comparator level.
 *
 * @details
 *   Function returns a level of VCC comparator.
 *
 * @return level of VCC comparator
 */
static ALWAYS_INLINE Voltage_t VoltMon_GetVccLvl(void)
{
    return (Voltage_t)GET_PML_VCC_LVL_R(PML->RegPmlLvl.regs.PmlVcc);
}


/**
 * @brief
 *   Get VCC TX comparator level.
 *
 * @details
 *   Function returns a level of VCC TX comparator.
 *
 * @return level of VCC TX comparator
 */
static ALWAYS_INLINE Voltage_t VoltMon_GetVccLvlTx(void)
{
    return (Voltage_t)GET_PML_VCC_LVL_TX_R(PML->RegPmlLvl.regs.PmlVcc);
}


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief
 *   Get VBAT1 comparator status bit.
 *
 * @details
 *   Function returns status bit of VBAT1 comparator.
 *
 * @return true, If the VBAT1 level is higher than the value set in
 * RegPmlLvl.PmlVBATLvl reg, false otherwise
 *
 */
bool VoltMon_GetVbat1CompStat(void);


/**
 * @brief
 *   Get VCC comparator status bit.
 *
 * @details
 *   Function returns status bit of VCC comparator.
 *
 * @return true, If the VCC level is higher than the value set in
 * RegPmlLvl.PmlVCCLvl reg, false otherwise
 *
 */
bool VoltMon_GetVccCompStat(void);


/**
 * @brief
 *   Get Brown-Out comparator status bit.
 *
 * @details
 *   Function returns status bit of Brown-Out comparator.
 *
 * @return true, If the VBAT level is higher than 1.7V, false otherwise
 *
 */
bool VoltMon_GetBrownOutCompStat(void);


/**
 * @brief
 *   Set VBAT1 comparator level.
 *
 * @details
 *   Function sets a level of VBAT1 comparator.
 *
 * @param level of VBAT1 comparator
 *
 * @note Vbat1p7 level is cleared when setting the
 * Vbat1 level.
 */
void VoltMon_SetVbat1Lvl(Voltage_t level);


/**
 * @brief
 *   Set VBAT1 comparator level to 1.7V.
 *
 * @details
 *   Function sets 1.7V level to VBAT1 comparator.
 *
 * @param enable TRUE enables VBAT1 comparator level to 1.7V, FALSE disable VBAT1 comparator level to 1.7V
 *
 */
void VoltMon_SetVbat1p7Lvl(bool enable);


/**
 * @brief
 *   Set VCC comparator level.
 *
 * @details
 *   Function sets a level of VCC comparator.
 *
 * @param level of VCC comparator
 *
 */
void VoltMon_SetVccLvl(Voltage_t level);


/**
 * @brief Set VCC comparator level for TX mode in Step-down configuration.
 * Function sets a level of VCC comparator. This level is used only during TX
 * operation if Step-down configuration is used.
 * @param level of VCC comparator
 */
void VoltMon_SetVccLvlTx(Voltage_t level);


/**
 * @brief Enable/disable battery voltage monitor.
 * @param enable TRUE enables voltage monitor, FALSE disable voltage monitor
 * @returns TRUE is voltage monitor was running before, FALSE otherwise
 * @details
 *   Function enables or disabled  battery voltage monitor.
 */
bool VoltMon_SetVoltageMonitorEnable(bool enable);


/**
 * @brief
 *   Get VCC comparator level.
 *
 * @details
 *   Function sets a level of VCC comparator.
 *
 * @param level of VCC comparator
 *
 * @return true, If the VCC level is higher that value set in RegPmlLvl.PmlVCCLvl reg, false otherwise
 */
bool VoltMon_CompareVccLvl(Voltage_t level);


/**
 * @brief
 *   Get VBAT1 comparator level.
 *
 * @details
 *   Function sets a level of VBAT1 comparator.
 *
 * @param level of VBAT1 comparator
 *
 * @return true, If the VBAT1 level is higher that value set in RegPmlLvl.PmlVBATLvl reg, false otherwise
 */
bool VoltMon_CompareVbat1Lvl(Voltage_t level);


/**
 * @brief
 *   Get voltage level of VCC domain.
 *
 * @details
 *   Function returns voltage level of VCC supply domain.
 *
 * @return Voltage level of VCC supply domain.
 */
Voltage_t VoltMon_GetVccVoltage(void);


/**
 * @brief
 *   Get voltage level of VBAT1 domain.
 *
 * @details
 *   Function returns voltage level of VBAT1 supply domain.
 *
 * @return Voltage level of VBAT1 supply domain.
 */
Voltage_t VoltMon_GetVbat1Voltage(void);


/**
 * @brief
 *   Get voltage of supply domain using a DCDC converter according to application mode.
 *
 * @details
 *   Function returns voltage level of supply domain using DCDC according to
 *   application mode.
 *
 * @return Voltage level of supply domain using a DCDC converter.
 */
Voltage_t VoltMon_GetVoltage(void);


/**
 * @brief Perform voltage measurement routine
 * @param always If TRUE voltage measurement is performed always. If FALSE it is
 * performed only if time elapsed from last measurement is bigger than defined
 * value.
 * @return Return the last voltage measurement
 */
Voltage_t VoltMon_DoMeasurement(bool always);


/**
 * @brief Set voltage level to comparator according to application mode.
 *
 * @details
 *   Function sets voltage level of appropriate supply domain according to
 *   application mode.
 */
// void VoltMon_SetVoltageLevel(uint8_t level);


/**
 * @brief Get limit RF output power to 6dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 6dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf10dBm(void);


/**
 * @brief Get limit RF output power to 6dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 6dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf6dBm(void);

/**
 * @brief Get limit RF output power to 5dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 5dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf5dBm(void);

/**
 * @brief Get limit RF output power to 4dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 4dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf4dBm(void);

/**
 * @brief Get limit RF output power to 3dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 3dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf3dBm(void);


/**
 * @brief Get limit RF output power to 0dBm flag
 *
 * @details
 *   Function returns flag whether RF output power needs to be limited to 0dBm
 *
 * @return TRUE, when RF output power needs to be limited, FALSE otherwise
 */
bool VoltMon_GetFlagLimitRf0dBm(void);


/**
 * @brief Get system shut down flag
 *
 * @details
 *   Function returns flag whether system needs to be shut down.
 *
 * @return TRUE, when system needs to be shut down, FALSE otherwise
 */
bool VoltMon_GetFlagSystemShutDown(void);


/**
 * @brief Get current application mode
 *
 * @details
 *   Function returns application mode saved in global variable
 *
 * @return 0x00 (StepDown), 0x01 (Wired), 0x02 (StepUp), ExternalUp (0x03)
 */
uint8_t VoltMon_GetAppMode(void);


/** @} */ // End of group pml_voltmon
/** @} */ // End of addtogroup drivers


#endif // _VOLT_MON_H
