////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/qdec/includes/qdec_hal.h
///
/// @project    T9305
///
/// @brief      QDEC hardware abstraction layer.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2023 EM Microelectronic
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


#ifndef QDEC_HAL_H
#define QDEC_HAL_H


#include <t9305_periph.h>
#include "qdec_private.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup qdec_hal QDEC HAL Driver
 * @brief QDEC Driver for the QDEC.
 *
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Enable QDEC peripheral
 * @param enable true to enable QDEC
 */
static ALWAYS_INLINE void QDEC_HAL_Enable(uint8_t enable)
{
    QDEC->RegQDecEn.r32 = (QDEC->RegQDecEn.r32 & ~QDEC_EN_MASK) | QDEC_EN(enable);
}

/**
 * @brief Is QDEC Running
 * @return true if QDEC is running
 */
static ALWAYS_INLINE bool QDEC_HAL_IsRunning(void)
{
    return GET_QDEC_RUNNING(QDEC->RegQDecEn.r32);
}

/**
 * @brief Set QDEC Configuration
 * @param cfg configuration of QDEC
 */
static ALWAYS_INLINE void QDEC_HAL_SetConfiguration(uint32_t cfg)
{
    QDEC->RegQDecCfg.r32 = cfg;
}

/**
 * @brief Get QDEC Configuration
 * @return configuration of QDEC
 */
static ALWAYS_INLINE uint32_t QDEC_HAL_GetConfiguration(void)
{
    return QDEC->RegQDecCfg.r32;
}

/**
 * @brief Enable QDEC Wakeup double movement
 * @param enable true to enable wakeup double movement
 */
static ALWAYS_INLINE void QDEC_HAL_EnableWakeupDoubleMovement(uint8_t enable)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_WAKE_DBL_EN_MASK) | QDEC_WAKE_DBL_EN(enable);
}

/**
 * @brief Enable QDEC Wakeup overflow
 * @param enable true to enable wakeup overflow
 */
static ALWAYS_INLINE void QDEC_HAL_EnableWakeupOverflow(uint8_t enable)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_WAKE_OVF_EN_MASK) | QDEC_WAKE_OVF_EN(enable);
}

/**
 * @brief Enable QDEC Wakeup report ready
 * @param enable true to enable wakeup ready
 */
static ALWAYS_INLINE void QDEC_HAL_EnableWakeupReportReady(uint8_t enable)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_WAKE_REP_RDY_EN_MASK) | QDEC_WAKE_REP_RDY_EN(enable);
}

/**
 * @brief Enable QDEC Debounce
 * @param enable true to enable debounce
 */
static ALWAYS_INLINE void QDEC_HAL_EnableDebounce(uint8_t enable)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_DBC_EN_MASK) | QDEC_DBC_EN(enable);
}

/**
 * @brief Set QDEC Led Polarity
 * @param polarity led polarity
 */
static ALWAYS_INLINE void QDEC_HAL_SetLedPolarity(uint8_t polarity)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_LED_POL_MASK) | QDEC_LED_POL(polarity);
}

/**
 * @brief Enable QDEC Led
 * @param enable true to enable Led
 */
static ALWAYS_INLINE void QDEC_HAL_EnableLed(uint8_t enable)
{
    QDEC->RegQDecCfg.r32 = (QDEC->RegQDecCfg.r32 & ~QDEC_LED_EN_MASK) | QDEC_LED_EN(enable);
}

/**
 * @brief Set QDEC Pin Selection
 * @param pin_sel pin selection
 */
static ALWAYS_INLINE void QDEC_HAL_SetPinSelection(uint32_t pin_sel)
{
    QDEC->RegQDecPinSel.r32 = pin_sel;
}

/**
 * @brief Get QDEC Pin Selection
 * @return pin selection
 */
static ALWAYS_INLINE uint32_t QDEC_HAL_GetPinSelection(void)
{
    return QDEC->RegQDecPinSel.r32;
}

/**
 * @brief Set QDEC Led Pin
 * @param pin pin led
 */
static ALWAYS_INLINE void QDEC_HAL_SetPinLed(uint32_t pin)
{
    QDEC->RegQDecPinSel.r32 = (QDEC->RegQDecPinSel.r32 & ~QDEC_OUT_SEL_LED_MASK) | QDEC_OUT_SEL_LED(pin);
}

/**
 * @brief Set QDEC Phase B Pin
 * @param pin pin phase B
 */
static ALWAYS_INLINE void QDEC_HAL_SetPinPhaseB(uint32_t pin)
{
    QDEC->RegQDecPinSel.r32 = (QDEC->RegQDecPinSel.r32 & ~QDEC_IN_SEL_PHASE_B_MASK) | QDEC_IN_SEL_PHASE_B(pin);
}

/**
 * @brief Set QDEC Phase A Pin
 * @param pin pin phase A
 */
static ALWAYS_INLINE void QDEC_HAL_SetPinPhaseA(uint32_t pin)
{
    QDEC->RegQDecPinSel.r32 = (QDEC->RegQDecPinSel.r32 & ~QDEC_IN_SEL_PHASE_A_MASK) | QDEC_IN_SEL_PHASE_A(pin);
}

/**
 * @brief Set QDEC Capture Control
 * @warning Do not set QDEC_CAPTURE_ACC_SW(1) and QDEC_CAPTURE_ACC_DBL_SW(1) at
 *          the same time.
 *          Separate the two captures into two function calls.
 * @param ctrl capture control
 */
static ALWAYS_INLINE void QDEC_HAL_SetCaptureControl(uint32_t ctrl)
{
    QDEC->RegQDecCaptureCtrl.r32 = ctrl;
}

/**
 * @brief Set QDEC Sample & Report Period Configuration
 * @param period period configuration
 */
static ALWAYS_INLINE void QDEC_HAL_SetSampleReportPeriod(uint32_t period)
{
    QDEC->RegQDecPeriodCfg.r32 = period;
}

/**
 * @brief Get QDEC Sample & Report Period Configuration
 * @return period configuration
 */
static ALWAYS_INLINE uint32_t QDEC_HAL_GetSampleReportPeriod(void)
{
    return QDEC->RegQDecPeriodCfg.r32;
}

/**
 * @brief Set QDEC Report Period Configuration
 * @param period period configuration
 */
static ALWAYS_INLINE void QDEC_HAL_SetReportPeriod(QDEC_ReportPeriod_t period)
{
    QDEC->RegQDecPeriodCfg.r32 = (QDEC->RegQDecPeriodCfg.r32 & ~QDEC_REPORT_PER_MASK) | QDEC_REPORT_PER((uint32_t)period);
}

/**
 * @brief Set QDEC Sample Period Configuration
 * @param period period configuration
 */
static ALWAYS_INLINE void QDEC_HAL_SetSamplePeriod(QDEC_SamplingPeriod_t period)
{
    QDEC->RegQDecPeriodCfg.r32 = (QDEC->RegQDecPeriodCfg.r32 & ~QDEC_SAMPLE_PER_MASK) | QDEC_SAMPLE_PER((uint32_t)period);
}

/**
 * @brief Set QDEC Led Time On Configuration
 * @param time_on led time on configuration
 */
static ALWAYS_INLINE void QDEC_HAL_SetLedTimeOn(uint32_t time_on)
{
    QDEC->RegQDecTimeCfg.r32 = (QDEC->RegQDecTimeCfg.r32 & ~QDEC_LED_TIME_MASK) | QDEC_LED_TIME(time_on);
}

/**
 * @brief Get QDEC Led Time On Configuration
 * @return led time on configuration
 */
static ALWAYS_INLINE uint32_t QDEC_HAL_GetLedTimeOn(void)
{
    return GET_QDEC_LED_TIME(QDEC->RegQDecTimeCfg.r32);
}

/**
 * @brief Read Sample
*/
static ALWAYS_INLINE int32_t QDEC_HAL_ReadSample(void)
{
    return (int32_t) QDEC->RegQDecSample.r32;
}

/**
 * @brief Read Accumulator
*/
static ALWAYS_INLINE int32_t QDEC_HAL_ReadAccumulator(void)
{
    return (int32_t) QDEC->RegQDecAcc.r32;
}

/**
 * @brief Read Double Movement Accumulator
*/
static ALWAYS_INLINE uint8_t QDEC_HAL_ReadDblMvtAccumulator(void)
{
    return (uint8_t) GET_QDEC_ACC_DBL(QDEC->RegQDecAccDbl.r32);
}

/**
 * @brief Read accumulator Capture
*/
static ALWAYS_INLINE int32_t QDEC_HAL_ReadAccumulatorCapture(void)
{
    return (int32_t) QDEC->RegQDecAccCapture.r32;
}

/**
 * @brief Read double movement accumulator Capture
*/
static ALWAYS_INLINE uint8_t QDEC_HAL_ReadDblMvtAccumulatorCapture(void)
{
    return (uint8_t) GET_QDEC_ACC_DBL_CAPTURE(QDEC->RegQDecAccDblCapture.r32);
}


/** @} */ // End of group qdec_hal
/** @} */ // End of addtogroup drivers


#endif /* QDEC_HAL_H */
