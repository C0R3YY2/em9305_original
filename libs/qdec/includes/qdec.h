////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/qdec/includes/qdec.h
///
/// @project    T9305
///
/// @brief      QDEC drivers
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


#ifndef _QDEC_H
#define _QDEC_H

#include "driver.h"
#include "config.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup qdec QDEC Driver
 * @brief Driver for the QDEC.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// QDEC configuration.
typedef union
{
    /// QDEC config byte.
    uint32_t config;

    /// bit access
    struct
    {
        /// Led Enable bit.
        uint32_t led_en            : 1;
        /// Led Polarity bit.
        uint32_t led_pol           : 1;
        /// Not used bits.
        uint32_t reserved          : 6;
        /// Debounce enable bit.
        uint32_t debounce_en       : 1;
        /// Not used bits.
        uint32_t reserved2         : 7;
        /// Wakeup report ready enable bit.
        uint32_t wakeup_rpt_rdy_en : 1;
        /// Wakeup overflow enable bit.
        uint32_t wakeup_ovf_en     : 1;
        /// Wakeup double movement enable bit.
        uint32_t wakeup_dbl_mvt_en : 1;
        /// Not used bits.
        uint32_t reserved3         : 13;
    } bits;
} QDEC_Config_t;

/// QDEC pin selection.
typedef union
{
    /// QDEC pin selection.
    uint32_t pin_selection;

    /// bit access
    struct
    {
        /// Pin selection phase A.
        uint32_t phase_a   : 4;
        /// Not used.
        uint32_t reserved1 : 4;
        /// Pin selection phase B.
        uint32_t phase_b   : 4;
        /// Not used.
        uint32_t reserved2 : 4;
        /// Pin selection Led.
        uint32_t led       : 4;
        /// Not used.
        uint32_t reserved3 : 12;
    } bits;
} QDEC_PinSelection_t;

/// QDEC sample and report period configuration.
typedef union
{
    /// QDEC period config.
    uint32_t period_config;

    /// bit access
    struct
    {
        /// Sample Period.
        uint32_t sample_period : 4;
        /// Not Used.
        uint32_t reserved1     : 12;
        /// Report Period.
        uint32_t report_period : 4;
        /// Not used.
        uint32_t reserved2     : 12;

    } bits;
} QDEC_PeriodConfig_t;

/// QDEC Debounce and LED timing configuration.
typedef union
{
    /// QDEC Debounce and LED timing configuration.
    uint32_t led_time;

    /// bit access
    struct
    {
        /// Not used.
        uint32_t reserved    : 16;
        /// LED Time On by step of 2 us, 0 = always on.
        uint32_t led_time_on : 16;
    } bits;
} QDEC_LedTimeConfig_t;

/**
 * @brief QDEC configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// QDEC Configuration bits.
    QDEC_Config_t configBits;

    /// QDEC Pin Selection
    QDEC_PinSelection_t pinBits;

    /// QDEC Sample and Report Period Configuration.
    QDEC_PeriodConfig_t periodBits;

    /// QDEC Led Time On
    QDEC_LedTimeConfig_t ledTimeBits;

    /// If the driver is enabled or not.
    uint8_t enabled;

    /// Enable pullp
    bool enable_pullup;
} QDEC_Configuration_t;

/**
 * brief QDEC Values containing accumulator and double movement
*/
typedef struct
{
    /// Captured accumulator value
    int32_t accumulator;
    /// Captured double movement accumulator value
    uint8_t dbl_mvt_accumulator;
} QDEC_Values_t;

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/**
 * @brief Status returned by the driver to the callback function.
 */
typedef enum
{
    /// The driver is ready to report values.
    REPORT_READY = 0,
    /// The driver report an overflow error.
    OVERFLOW_ERROR = 1,
    /// The driver report a movement error.
    DOUBLE_MOVEMENT_ERROR = 2,
} QDEC_Status_t;

/**
 * @brief Sampling period values
 */
typedef enum
{
    /// Sampling Period of 128 us
    SAMPLING_PERIOD_128_US = 0,
    /// Sampling Period of 256 us
    SAMPLING_PERIOD_256_US = 1,
    /// Sampling Period of 512 us
    SAMPLING_PERIOD_512_US = 2,
    /// Sampling Period of 1024 us
    SAMPLING_PERIOD_1024_US = 3,
    /// Sampling Period of 2048 us
    SAMPLING_PERIOD_2048_US = 4,
    /// Sampling Period of 4096 us
    SAMPLING_PERIOD_4096_US = 5,
    /// Sampling Period of 8192 us
    SAMPLING_PERIOD_8192_US = 6,
    /// Sampling Period of 16384 us
    SAMPLING_PERIOD_16384_US = 7,
    /// Sampling Period of 32768 us
    SAMPLING_PERIOD_32768_US = 8,
    /// Sampling Period of 65536 us
    SAMPLING_PERIOD_65536_US = 9,
    /// Sampling Period of 131072 us
    SAMPLING_PERIOD_131072_US = 10,
} QDEC_SamplingPeriod_t;

/**
 * @brief Report period, number of sample(s)
 * for each report
 */
typedef enum
{
    /// Report Period with 1 sample
    REPORT_PERIOD_1_SAMPLE = 0,
    /// Report Period with 5 sample
    REPORT_PERIOD_5_SAMPLE = 1,
    /// Report Period with 10 sample
    REPORT_PERIOD_10_SAMPLE = 2,
    /// Report Period with 20 sample
    REPORT_PERIOD_20_SAMPLE = 3,
    /// Report Period with 40 sample
    REPORT_PERIOD_40_SAMPLE = 4,
    /// Report Period with 80 sample
    REPORT_PERIOD_80_SAMPLE = 5,
    /// Report Period with 160 sample
    REPORT_PERIOD_160_SAMPLE = 6,
    /// Report Period with 320 sample
    REPORT_PERIOD_320_SAMPLE = 7,
} QDEC_ReportPeriod_t;

/******************************************************************************\
 *  STRUCTURES
\******************************************************************************/

/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/

/// QDEC Configuration
extern volatile QDEC_Configuration_t gQDEC_Config;

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Callback function to be called by a driver.
 */
typedef void (*QDEC_Driver_Callback_t)(QDEC_Status_t status, QDEC_Values_t values);

/**
 * @brief Add the QDEC module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void QDEC_RegisterModule(void);

/**
 * @brief Register callback for ISR interruptions when samples are ready
 * @param callbackFunction callback to be registered
*/
void QDEC_RegisterCallback(QDEC_Driver_Callback_t callbackFunction);

/**
 * @brief Start continuous capture
*/
void QDEC_StartContinuousCapture(void);

/**
 * @brief Stop continuous capture
*/
void QDEC_StopContinuousCapture(void);

/**
 * @brief Start single capture
 * @param acc accumulator value
 * @param dbl_acc double movement accumulator value
*/
void QDEC_StartSingleCapture(int32_t* acc, uint8_t* dbl_acc);

/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/** @} */ // End of group qdec
/** @} */ // End of addtogroup drivers


#endif /* _QDEC_H */
