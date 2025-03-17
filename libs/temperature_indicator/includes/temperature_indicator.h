////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/temperature_indicator/includes/temperature_indicator.h
///
/// @project    T9305
///
/// @brief      Temperature Indicator driver
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2024, EM Microelectronic
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


#ifndef _TEMPERATURE_INDICATOR_H
#define _TEMPERATURE_INDICATOR_H

/**
 * @addtogroup drivers
 * @{
 * @defgroup temperature_indicator Temperature Indicator Library
 * @brief Library for Temperature Indicator.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @brief Type for any functions that are called due to a TI event.
 * @note This callback is called in an interrupt context.
 * @param temperature Temperature in degree celsius computed by the library.
 */
typedef void (*TI_Callback_t)(float temperature);

/**
 * @brief TI configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// LF Counter Limit used for LF-RC LP calibration
    uint32_t lfCounterLimit;

    /// HF Counter Limit used for LF-RC LP calibration
    uint32_t hfCounterLimit;
} TI_Configuration_t;

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
 * @brief Add the TI (Temperature Indicator) module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void TI_RegisterModule(void);

/**
 * @brief Set Measurement callback which will be called when temperature
 * has been computed from LF-RC LP calibration.
 * @param callback A user defined callback function that is called when
 * the temperature measurement is complete.
 */
void TI_SetMeasurementCallback(TI_Callback_t callback);

/**
 * @brief Start a measurement
 * @return true if successfully started
 */
bool TI_StartMeasure(void);

/**
 * @brief Get the last temperature measurement
 * @return The last temperature measurement in degree celsius
 */
float TI_GetLastTemperature(void);

/**
 * @brief Get the LF-RC LP frequency measured. Used
 * when doing calibration to compute coefficient and offset.
 * The frequency is in fixed point 24.8 format in Hz unit.
 * @return The last frequency measurement in fixed point 24.8 (Hz)
 */
uint32_t TI_GetFrequency(void);

/**
 * @brief Get the calibration state, return true if valid calibration
 * data have been found in nvm info page.
 * @return true if driver has found valid calibration data in
 * nvm info page
 */
bool TI_IsCalibrated(void);

/** @} */ // End of group temperature_indicator
/** @} */ // End of addtogroup drivers

#endif //_TEMPERATURE_INDICATOR_H
