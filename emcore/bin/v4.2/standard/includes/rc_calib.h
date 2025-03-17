////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/rc_calib/includes/rc_calib.h
///
/// @project    T9305
///
/// @brief      RC Calibration drivers
///
/// This peripheral is responsible of RC calibration.
///  It uses IRQs ......
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

#ifndef _RCCAL_H
#define _RCCAL_H

#include <types.h>
#include <t9305_periph.h>

/**
 * @addtogroup drivers
 * @{
 */

/**
 * @addtogroup RCCAL
 * @brief RC Calibration peripheral API
 * @{
 */


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/



/******************************************************************************\
 *  MACROS
\******************************************************************************/



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/
/**
 * @brief Data structure holding the RC Calibration setting
 * @ingroup rccal
 */
typedef struct
{
    /// HF Counter limit
    uint32_t hfCounterLimit;
    /// LF Counter limit
    uint32_t lfCounterLimit;
} RCCAL_CalibCfg_t;

/**
 * @brief Data structure holding the actual RC Calibration values
 * These data should be store in persistent memory.
 * @ingroup rccal
 */
typedef struct
{
    /// Actual HF Counter value
    uint32_t hfCounterValue;
    /// Actual LF Counter value
    uint32_t lfCounterValue;
} RCCAL_CalibValues_t;

/**
 * @brief Callback function for counter ready when calibration is done.
 * @param hfCounterValue HF counter
 * @param lfCounterValue LF counter
 */
typedef void (*RCCAL_CounterReadyCallback_t)(uint32_t hfCounterValue, uint32_t lfCounterValue);

/**
 * @brief RC Calibration clock source selection.
 */
typedef enum
{
    /// Sleep Timer clock source for RC Calibration
    RCCAL_CLK_SOURCE_SLEEP_TIMER = 0x00,   // 00b Sleep Timer
    // LF-RC clock source for RC Calibration
    RCCAL_CLK_SOURCE_LF_RC       = 0x01,   // 01b LF-RC
    // LF-RC LP clock source for RC Calibration
    RCCAL_CLK_SOURCE_LF_RC_LP    = 0x02,   // 10b LF-RC LP
    // LF-XTAL clock source for RC Calibration
    RCCAL_CLK_SOURCE_LF_XTAL     = 0x03,   // 11b LF-XTAL
} RCCAL_ClockSourceSel_t;

/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/
/// RCCAL Configuration
extern RCCAL_CalibCfg_t gRCCAL_cfg;
/// RCCAL values
extern RCCAL_CalibValues_t gRCCAL_values;



/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Set the HF Counter limit.
 * The HF Counter limit defines a target value of HF counter.
 * @param hfCounterLimit HF Counter limit. Valid range: <0, 16777215 (2^24-1)>.
 * @ingroup rccal
 */
static ALWAYS_INLINE void RCCAL_SetHFCounterLimit(uint32_t hfCounterLimit)
{
    RCCAL->RegRCCalibHFCounterLimit.r32 = hfCounterLimit;
}

/**
 * @brief Get the HF Counter limit.
 * The HF Counter limit defines a target value of HF counter.
 * @return Actual value of HF Counter limit.
 * @ingroup rccal
 */
static ALWAYS_INLINE uint32_t RCCAL_GetHFCounterLimit(void)
{
    uint32_t var = RCCAL->RegRCCalibHFCounterLimit.r32;
    return var;
}

/**
 * @brief Get the HF Counter value.
 * The HF Counter value reflects the actual value of HF Counter.
 * @return Actual value of HF Counter.
 * @ingroup rccal
 */
static ALWAYS_INLINE uint32_t RCCAL_GetHFCounterValue(void)
{
    uint32_t var = RCCAL->RegRCCalibHFCounterVal.r32;
    return var;
}

/**
 * @brief Set the LF Counter limit.
 * The LF Counter limit defines a target value of LF counter.
 * @param lfCounterLimit LF Counter limit. Valid range: <0, 16383 (2^14-1)>.
 * @ingroup rccal
 */
static ALWAYS_INLINE void RCCAL_SetLFCounterLimit(uint32_t lfCounterLimit)
{
    RCCAL->RegRCCalibLFCounterLimit.r32 = lfCounterLimit;
}

/**
 * @brief Get the LF Counter limit.
 * The LF Counter limit defines a target value of LF counter.
 * @return Actual value of LF Counter limit.
 * @ingroup rccal
 */
static ALWAYS_INLINE uint32_t RCCAL_GetLFCounterLimit(void)
{
    uint32_t var = RCCAL->RegRCCalibLFCounterLimit.r32;
    return var;
}

/**
 * @brief Get the LF Counter value.
 * The LF Counter value reflects the actual value of LF Counter.
 * @return Actual value of LF Counter.
 * @ingroup rccal
 */
static ALWAYS_INLINE uint32_t RCCAL_GetLFCounterValue(void)
{
    uint32_t var = RCCAL->RegRCCalibLFCounterVal.r32;
    return var;
}

/**
 * @brief Check RC Calibration running status.
 * Check whether the RC Calibration is running.
 * @returns true If calibration is running or is in start or stop phase, false
 * otherwise.
 * @ingroup rccal
 */
static ALWAYS_INLINE bool RCCAL_IsCalibrationRunning(void)
{
    bool var = (bool) GET_RC_CAL_RUNNING(RCCAL->RegRCCalibStat.r32);
    return var;
}

/**
 * @brief Check RC Calibration start status.
 * Check whether the RC Calibration is going to start.
 * @returns true If calibration is starting, false otherwise.
 * @ingroup rccal
 */
static ALWAYS_INLINE bool RCCAL_IsCalibrationStarting(void)
{
    bool var = (bool) GET_RC_CAL_START_STS(RCCAL->RegRCCalibStat.r32);
    return var;
}

/**
 * @brief Check RC Calibration stop status.
 * Check whether the RC Calibration is going to stop.
 * @returns true If calibration is stopping, false otherwise.
 * @ingroup rccal
 */
static ALWAYS_INLINE bool RCCAL_IsCalibrationStopping(void)
{
    bool var = (bool) GET_RC_CAL_STOP_STS(RCCAL->RegRCCalibStat.r32);
    return var;
}

/**
 * @brief Set the RC calibration to stop at predefined HF Counter limit.
 * The RC Calibration is stopped after HF counter reaches the predefined
 * value saved in RegRCCalibHFCounterLimit register.
 * @ingroup rccal
 */
static ALWAYS_INLINE void RCCAL_SetStopAtHFLimit(void)
{
    RCCAL->RegRCCalibCfg.r32 = RC_CAL_STOP_AT_LF_LIMIT(0) | RC_CAL_STOP_AT_HF_LIMIT(1);
}

/**
 * @brief Set the RC calibration to stop at predefined LF Counter limit.
 * The RC Calibration is stopped after LF counter reaches the predefined
 * value saved in RegRCCalibLFCounterLimit register.
 * @ingroup rccal
 */
static ALWAYS_INLINE void RCCAL_SetStopAtLFLimit(void)
{
    RCCAL->RegRCCalibCfg.r32 = RC_CAL_STOP_AT_LF_LIMIT(1) | RC_CAL_STOP_AT_HF_LIMIT(0);
}

 /**
 * @brief Set the RC calibration to stop at predefined HF/LF Counter limit.
 * The RC Calibration is stopped after HF/LF counter reaches the predefined
 * value saved in RegRCCalib(HF/LF)CounterLimit register.
 * @param bitMask Bit mask indicating whether to stop at LF limit or HF limit or
 * both.
 * @ingroup rccal
 */
static ALWAYS_INLINE void RCCAL_SetStopAtLimit(uint32_t bitMask)
{
    // Clear Stop at LF & HF Limit bits
    RCCAL->RegRCCalibCfg.r32 = (RCCAL->RegRCCalibCfg.r32 & ~(RC_CAL_STOP_AT_HF_LIMIT_MASK | RC_CAL_STOP_AT_LF_LIMIT_MASK));

    // Set Bit 0 & 1 for Stop at Limit for LF & HF
    RCCAL->RegRCCalibCfg.r32 |= (bitMask & 0x03);
}

/**
 * @brief Get the RC calibration configuration.
 * The RC Calibration configuration register is read out.
 * @returns Bit mask indicating whether to stop at LF limit or HF limit or both.
 * @ingroup rccal
 */
static ALWAYS_INLINE uint32_t RCCAL_GetStopAtLimit(void)
{
    uint32_t var = RCCAL->RegRCCalibCfg.r32;
    return var;
}

/**
 * @brief Stop RC Calibration.
 * @returns true If calibration stop bit is set properly, false otherwise.
 * @note Typically it returns false and user is responsible to wait:
 * while(RCCAL_IsCalibrationRunning()){}
 * @ingroup rccal
 */
static bool ALWAYS_INLINE RCCAL_StopCalibration(void)
{
    // Stop RC calibration
    RCCAL->RegRCCalibCtrl.r32 = RC_CAL_STOP(TRUE);
    bool var = !RCCAL_IsCalibrationRunning();
    return var;
}

/**
 * @brief Get actual HF counter value saved in internal structure.
 * Note it is user responsibility to check that
 * RCCAL_IsCalibrationRunning(void) == FALSE
 *
 * @returns Actual HF counter value
 * @ingroup rccal
 */
static uint32_t ALWAYS_INLINE RCCAL_GetActualCounterValueHf(void)
{
    uint32_t var = gRCCAL_values.hfCounterValue;
    return var;
}

/**
 * @brief Get actual LF counter value saved in internal structure.
 * Note it is user responsibility to check that
 * RCCAL_IsCalibrationRunning(void) == FALSE
 *
 * @returns Actual LF counter value
 * @ingroup rccal
 */
static uint32_t ALWAYS_INLINE RCCAL_GetActualCounterValueLf(void)
{
    return gRCCAL_values.lfCounterValue;
}

/**
 * @brief Set LF Clock source for RC Calibration
 * @param lfClkSel LF Clock source
 */
static void ALWAYS_INLINE RCCAL_SetLfClockSelection(RCCAL_ClockSourceSel_t lfClkSel)
{
    RCCAL->RegRCCalibCfg.r32 = (RCCAL->RegRCCalibCfg.r32 & ~RC_CAL_LF_CLK_SEL_MASK) | RC_CAL_LF_CLK_SEL((uint32_t)lfClkSel);
}

/**
 * @brief Get LF Clock source for RC Calibration
 * @return LF Clock source
 */
static RCCAL_ClockSourceSel_t ALWAYS_INLINE RCCAL_GetLfClockSelection(void)
{
    return (RCCAL_ClockSourceSel_t) GET_RC_CAL_LF_CLK_SEL(RCCAL->RegRCCalibCfg.r32);
}

/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/
/**
 * @brief RCCAL initialization function
 *
 * @ingroup rccal
 */
void RCCAL_Init(void);

/**
 * @brief Register calibration limits of both counters
 *  This function writes limits of LF and HF counters into internal variables
 *  stored in RCCAL_CalibCfg_t.
 *
 * @param hfCounterLimit HF counter limit to stop at
 * @param lfCounterLimit LF counter limit to stop at
 * @ingroup rccal
 */
void RCCAL_RegisterCalibrationLimits(uint32_t hfCounterLimit, uint32_t lfCounterLimit);

/**
 * @brief Change calibration limits of both counters
 *  This function updates limits of LF and HF counters - the limits are directly written into
 *  calibration registers whithout changing values of limits stored in RCCAL_CalibCfg_t.
 *
 *  Can be used for limits changing while calibration is running.
 *
 * @param hfCounterLimit HF counter limit to stop at
 * @param lfCounterLimit LF counter limit to stop at
 * @ingroup rccal
 */
void RCCAL_ChangeCalibrationLimits(uint32_t hfCounterLimit, uint32_t lfCounterLimit);

/**
 * @brief Do RC Calibration.
 * You can then check RCCAL_IsCalibrationRunning(void) when false you can read
 *   calibration values using:
 *     RCCAL_GetActualCounterValue_HF
 *     RCCAL_GetActualCounterValue_LF
 * Note: If you called RCCAL_RegisterHFCounterLimit()
 *  and/or RCCAL_RegisterLFCounterLimit() before then you can call directly
 *  RCCAL_ReStartCalibration()
 *
 * @param hfCounterLimit HF counter limit to stop at (0=ignores)
 * @param lfCounterLimit LF counter limit to stop at (0=ignores)
 * @returns true If calibration is started, false otherwise.
 * @ingroup rccal
 */
bool RCCAL_StartCalibrationWithLimits(uint32_t hfCounterLimit, uint32_t lfCounterLimit);

/**
 * @brief Start RC Calibration.
 * @returns true If calibration start bit is set properly, false otherwise.
 * @ingroup rccal
 */
bool RCCAL_StartCalibration(void);

/**
 * @brief Do RC calibration, use previous parameters from
 * RCCAL_StartCalibrationWithLimits(uint32_t hfCounterLimit, uint32_t lfCounterLimit),
 * or with limites registered with RCCAL_RegisterCalibrationLimits().
 * @returns true If calibration start bit is set properly, false otherwise.
 */
bool RCCAL_ReStartCalibration(void);

/**
 * @brief Set the RC Calibration IRQ Handler callback function pointer.
 * @param pCallback function pointer of the user callback
 */
void RCCAL_SetISRCallback(void* pCallback);

/**
 * @brief Set the RC Calibration IRQ Handler callback function pointer for LF-RC LP.
 * @param pCallback function pointer of the user callback
*/
void RCCAL_SetISRLfRcLpCallback(void* pCallback);

/**
 * @brief Set Counter Ready callback
 * @param callback callback routine
*/
void RCCAL_SetCounterReadyCallback(RCCAL_CounterReadyCallback_t callback);

/** @} (end addtogroup RCCAL) */
/** @} (end addtogroup drivers) */

#endif //_RCCAL_H
