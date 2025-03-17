////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/adc/includes/adc.h
///
/// @project    T9305
///
/// @brief      ADC drivers
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


#ifndef _ADC_H
#define _ADC_H

#include "driver.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup adc ADC Driver
 * @brief Driver for the ADC.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/**
 * @brief ADC Clock Configuration which determine the sampling frequency
 * of the ADC without sample & hold.
 */
typedef enum
{
    ADC_CLK_120000_HZ = 0,
    ADC_CLK_114285_HZ,
    ADC_CLK_109090_HZ,
    ADC_CLK_104348_HZ,
    ADC_CLK_100000_HZ,
    ADC_CLK_96000_HZ,
    ADC_CLK_92307_HZ,
    ADC_CLK_88888_HZ,
} ADC_ClockConfig_t;

/**
 * @brief ADC resolution for measurement.
 */
typedef enum
{
    ADC_9_BITS = 0,
    ADC_8_BITS,
} ADC_Resolution_t;

/**
 * @brief ADC source selection.
 */
typedef enum
{
    ADC_SOURCE_GPIO5_WITH_SH    = 0, /* Measure GPIO5 input voltage with sample & hold */
    ADC_SOURCE_GPIO5_WITHOUT_SH = 3, /* Measure GPIO5 input voltage without sample & hold */
    ADC_SOURCE_VCC              = 1, /* Measure VCC input voltage */
    ADC_SOURCE_VBAT1            = 2, /* Measure VBAT1 input voltage */
} ADC_Source_t;

/**
 * @brief ADC Sample & Hold Clock Configuration.
 */
typedef enum
{
    ADC_SH_CLOCK_85714_HZ = 0,
    ADC_SH_CLOCK_75000_HZ,
    ADC_SH_CLOCK_66666_HZ,
    ADC_SH_CLOCK_60000_HZ,
} ADC_SH_ClockConfig_t;

/**
 * @brief ADC Range of Voltage measurement.
 */
typedef enum
{
    ADC_RANGE_0_05V_TO_0_5V = 0,
    ADC_RANGE_0_05V_TO_1V,
    ADC_RANGE_0_05V_TO_2V,
} ADC_Range_t;

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
 * @brief Module initialization function.
 */
void ADC_Init(void);

/**
 * @brief Enable ADC peripheral
 */
void ADC_Enable(void);

/**
 * @brief Disable ADC peripheral
 */
void ADC_Disable(void);

/**
 * @brief Check if ADC is running (sample convertion in progress)
 * in Single or Continuous mode
 * @return true if convertion is running
 */
bool ADC_IsBusy(void);

/**
 * @brief Resume ADC after exiting sleep mode
 * @note This function shall be called in NVM_ApplicationEntry
 * when the CPU resumes from sleep mode (checked by PML_DidBootFromSleep function).
 * In this case the ADC shall be resumed to be operational.
 */
void ADC_Resume(void);

/**
 * @brief Start conversion for ADC.
 * @note This function start the conversion and wait for the result (blocking).
 * @retval true in case of sucessful operation
 * @retval false in case of timeout on conversion running
 */
bool ADC_StartBlocking(void);

/**
 * @brief Start single conversion for ADC using Interruption (asynchronous).
 * @note This function start the conversion and return immediatly, the callback
 * will be called when ADC generate ISR for data ready.
 * The callback shall be set prior this call with ADC_RegisterCallback()
 */
void ADC_StartWithCallback(void);

/**
 * @brief Start continuous mode for ADC using DMA (asynchronous).
 * @param buffer buffer for all samples acquired in continuous mode using DMA
 * @param nb_samples number of samples for acquisition
 * @param dma_channel dma channel used for ADC in continuous mode
 * @note This function start the continuous mode and return immediatly, the callback
 * will be called when the DMA generate ISR for data transfer ready.
 * The callback shall be set prior this call with ADC_RegisterCallabck()
 */
void ADC_StartContinuousWithCallback(void* buffer, uint16_t nb_samples, uint8_t dma_channel);

/**
 * @brief Set ISR callback when the user want to use
 * conversion with interrupt
 *
 * @param callbackFunction callback function for interrupt
 */
void ADC_RegisterCallback(Driver_Callback_t callbackFunction);

/**
 * @brief Set Clock config to set sampling rate
 * @note This function has effect only when sample & hold is disabled
 *
 * @param config samplig rate
 */
void ADC_SetClockConfig(ADC_ClockConfig_t config);

/**
 * @brief Set resolution of ADC, can be 8 or 9 bits
 *
 * @param resolution resolution of ADC
 */
void ADC_SetResolution(ADC_Resolution_t resolution);

/**
 * @brief Set source for ADC measurement
 * can be VBAT1, VCC, GPIO5 with S&H, GPIO5 without S&H
 *
 * @param source source of measurement
 */
void ADC_SetSourceSelection(ADC_Source_t source);

/**
 * @brief Enable Sample & Hold
 * @note This function enables the sample and hold on the GPIO5 input source.
 * It has no effect on other input source.
 */
void ADC_EnableSampleAndHold(void);

/**
 * @brief Disable Sample & Hold
 * @note This function disables the sample and hold on the GPIO5 input source.
 * It has no effect on other input source.
 */
void ADC_DisableSampleAndHold(void);

/**
 * @brief Set Sample & Hold duration
 * @note This function set the sample & hold clock config when GPIO5
 * is selected as input source.
 * It has no effect on other input source.
 *
 * @param clock clock config for sample & hold
 */
void ADC_SetSHClockConfig(ADC_SH_ClockConfig_t clock);

/**
 * @brief Set range of voltage measurement
 * @note This function set the range of voltage measurement
 * on GPIO5 source with sample & hold.
 * It has no effect on other input source.
 *
 * @param range range of voltage measurement
 */
void ADC_SetRange(ADC_Range_t range);

/**
 * @brief Enable Vhi when source is on GPIO5 without sample & hold.
 *        It allow to change the voltage range for this input source.
 *        When Vhi is enabled, the range is 1.8V to 3.6V
 *        When Vhi is disabled, the range is 0.95V to 1.9V
 * @note It has no effect on other input source.
 */
void ADC_EnableVhi(void);

/**
 * @brief Disable Vhi when source is on GPIO5 without sample & hold
 *        It allow to change the voltage range for this input source.
 *        When Vhi is enabled, the range is 1.8V to 3.6V
 *        When Vhi is disabled, the range is 0.95V to 1.9V
 * @note It has no effect on other input source.
 */
void ADC_DisableVhi(void);

/**
 * @brief Get ADC measurement
 *
 * @param result uint16_t value with 8 or 9 bits resolution
 * @retval true in case of data valid
 * @retval false in case of overrun
 */
bool ADC_GetValue(uint16_t* result);

/**
 * @brief Convert ADC raw value to voltage in mV following
 * the selected source and configuration
 *
 * @param raw adc raw value
 * @return 16 bits unsigned Voltage in mV converted from raw value
 */
uint16_t ADC_ValueToMillivolt(uint16_t raw);

/**
 * @brief Return true if the current selected source use calibration data.
 * @note Calibration data are valid in User or EM info page for the selected source.
 * @return true if calibration data are used.
*/
bool ADC_IsCalibrated(void);

/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/** @} */ // End of group adc
/** @} */ // End of addtogroup drivers


#endif /* _ADC_H */
