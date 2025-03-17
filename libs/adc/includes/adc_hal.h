////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/adc/includes/adc_hal.h
///
/// @project    T9305
///
/// @brief      ADC hardware abstraction layer.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2022 EM Microelectronic
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


#ifndef ADC_HAL_H
#define ADC_HAL_H


#include <t9305_periph.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup adc_hal ADC HAL Driver
 * @brief ADC Driver for the ADC.
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
 * @brief Set configuration of ADC
 *
 * @param config full configuration of ADC
 */
static ALWAYS_INLINE void ADC_HAL_SetConfig(uint32_t config)
{
    ADC->RegAdcCfg.r32 = config;
}

/**
 * @brief Enable ADC peripheral
 *
 * @param enable true to enable ADC
 */
static ALWAYS_INLINE void ADC_HAL_Enable(uint8_t enable)
{
    ADC->RegAdcCtrl.r32 = (ADC->RegAdcCtrl.r32 & ~ADC_EN_MASK) | ADC_EN(enable);
}

/**
 * @brief Check if ADC is running in single or continuous mode
 * @return true if a convertion is in progress
 */
static ALWAYS_INLINE bool ADC_HAL_IsBusy(void)
{
    uint32_t reg = ADC->RegAdcSts.r32;

    return (bool)(GET_ADC_STS_CONT(reg) | GET_ADC_STS_SINGLE(reg));
}

/**
 * @brief Clear Data Valid status, also clear overrun status bit
 */
static ALWAYS_INLINE void ADC_HAL_ClearDataValid(void)
{
    // Write 1 in Bit 3 AdcClearDataValid
    ADC->RegAdcCtrl.r32 = (ADC->RegAdcCtrl.r32 & ~ADC_CLEAR_DATA_VALID_MASK) | ADC_CLEAR_DATA_VALID(1);
}

/**
 * @brief Stop continuous convertion for ADC.
 *
 */
static ALWAYS_INLINE void ADC_HAL_StopContinuousConversion(void)
{
    // Set bit 2 AdcStop: 1=stop continuous conversion
    ADC->RegAdcCtrl.r32 |= (uint32_t)ADC_STOP(1);
}

/**
 * @brief Start continuous conversion for ADC.
 * @note This function is asynchronous, a callback need to be provided
 * to be informed when a sample is available.
 */
static ALWAYS_INLINE void ADC_HAL_StartContinuousConversion(void)
{
    // Set bit 1 AdcStartCont: 1=start continuous conversion
    ADC->RegAdcCtrl.r32 |= (uint32_t)ADC_START_CONT(1);
}

/**
 * @brief Start single conversion for ADC.
 * @note This function start the conversion and wait for the result.
 */
static ALWAYS_INLINE void ADC_HAL_StartSingleConversion(void)
{
    // Set bit 0 AdcStartSingle: 1=start single conversion
    ADC->RegAdcCtrl.r32 |= (uint32_t)ADC_START_SINGLE(1);
}

/**
 * @brief Get comparator output status
 *
 * @retval true if comparator output status is set
 * @retval false if comparator output status is set
 */
static ALWAYS_INLINE bool ADC_HAL_ComparatorOutputStatus(void)
{
    return GET_ADC_STS_COMP_OUT(ADC->RegAdcSts.r32);
}

/**
 * @brief Get Data Overrun status
 *
 * @retval true if data is overrun mean that new data is available
 *         before precedent data has been read
 * @retval false if data is not overrun
 */
static ALWAYS_INLINE bool ADC_HAL_IsDataOverrun(void)
{
    return GET_ADC_STS_OVERRUN(ADC->RegAdcSts.r32);
}

/**
 * @brief Get Data Valid status
 *
 * @retval true if data is available in conversion result
 * @retval false if data is not yet available
 */
static ALWAYS_INLINE bool ADC_HAL_IsDataValid(void)
{
    return GET_ADC_STS_VALID(ADC->RegAdcSts.r32);
}

/**
 * @brief Get Continuous Conversion running status
 *
 * @retval true if continuous conversion is running
 * @retval false if continuous conversion is not running
 */
static ALWAYS_INLINE bool ADC_HAL_IsContinuousConvRunning(void)
{
    return GET_ADC_STS_CONT(ADC->RegAdcSts.r32);
}

/**
 * @brief Get Single Conversion running status
 *
 * @retval true if single conversion is running
 * @retval false if single conversion is not running
 */
static ALWAYS_INLINE bool ADC_HAL_IsSingleConvRunning(void)
{
    return GET_ADC_STS_SINGLE(ADC->RegAdcSts.r32);
}

/**
 * @brief Set Clock config to set sampling rate
 * @note This function has effect only when sample & hold is disabled
 *
 * @param config samplig rate
 */
static ALWAYS_INLINE void ADC_HAL_SetClockConfig(uint32_t config)
{
    // Set Bit 18:16 AdcClkCfg
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_CLK_CFG_MASK) | ADC_CLK_CFG(config);
}

/**
 * @brief Set resolution of ADC, can be 8 or 9 bits
 *
 * @param resolution resolution of ADC
 */
static ALWAYS_INLINE void ADC_HAL_SetResolution(uint32_t resolution)
{
    // Set Bit 12 AdcResolution
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_RESOLUTION_MASK) | ADC_RESOLUTION(resolution);
}

/**
 * @brief Get resolution of ADC, can bit 8 or 9 bits
 */
static ALWAYS_INLINE uint32_t ADC_HAL_GetResolution(void)
{
    return GET_ADC_RESOLUTION(ADC->RegAdcCfg.r32);
}

/**
 * @brief Enable Sel Vhi
 * @note This function should be used only with GPIO5 source selection without
 * sample & hold
 *
 * @param enable true to enable sel vhi
 */
static ALWAYS_INLINE void ADC_HAL_SelVhi(uint8_t enable)
{
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_SEL_VHI_MASK) | ADC_SEL_VHI(enable);
}

/**
 * @brief Enable Sample & Hold
 * @note This function should be used only with GPIO5 source measurement
 * on sample & hold
 *
 * @param enable enable sample & hold
 */
static ALWAYS_INLINE void ADC_HAL_EnableSampleAndHold(uint8_t enable)
{
    // Set Bit 10 AdcShEn
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_SH_EN_MASK) | ADC_SH_EN(enable);
}

/**
 * @brief Set Sample & Hold duration
 * @note This function should be used only with GPIO5 source measurement
 * on sample & hold
 *
 * @param duration duration of sample & hold
 */
static ALWAYS_INLINE void ADC_HAL_SampleAndHoldDuration(uint32_t duration)
{
    // Set Bit 9:8 AdcShDur
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_SH_DUR_MASK) | ADC_SH_DUR(duration);
}

/**
 * @brief Set range of voltage measurement
 * @note This function should be used only with GPIO5 source selection
 * on sample & hold
 *
 * @param range range of voltage measurement
 */
static ALWAYS_INLINE void ADC_HAL_SetRange(uint32_t range)
{
    // Set Bit 4:3 AdcRange
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_RANGE_MASK) | ADC_RANGE(range);
}

/**
 * @brief Set source for ADC measurement
 * can be VBAT1, VCC, GPIO5 with S&H, GPIO5 without S&H
 *
 * @param source source of measurement
 */
static ALWAYS_INLINE void ADC_HAL_SetSourceSelection(uint32_t source)
{
    // Set Bit 1:0 AdcSource
    ADC->RegAdcCfg.r32 = (ADC->RegAdcCfg.r32 & ~ADC_SOURCE_MASK) | ADC_SOURCE(source);
}

/**
 * @brief Get ADC measurement
 *
 * @return uint16_t 8 or 9 bits value from ADC convertion
 */
static ALWAYS_INLINE uint16_t ADC_HAL_GetConversionResult(void)
{
    return (uint16_t)GET_ADC_DATA(ADC->RegAdcData.r32);
}

/** @} */ // End of group adc_hal
/** @} */ // End of addtogroup drivers


#endif /* ADC_HAL_H */
