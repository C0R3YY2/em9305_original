////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_shared.h
///
/// @project    T9305
///
/// @brief      Power manager shared
///
/// This peripheral is responsible of voltage monitor.
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

#ifndef _PML_SHARED_H
#define _PML_SHARED_H

#include <arc.h>
#include <types.h>


/**
 * @addtogroup drivers
 * @{
 */

/**
 * @addtogroup PML_SHARED
 * @brief PML Power manager Shared variables
 * @{
 */


/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief Type for a voltage value.
 */
typedef int8_t Voltage_t;


/**
 * @brief  Application modes
 */
typedef enum
{
    PML_APP_MODE_STEP_DOWN      = 0,    /**< Step-down configuration */
    PML_APP_MODE_DCDC_OFF       = 1,    /**< DCDC-off configuration */
    PML_APP_MODE_STEP_UP        = 2,    /**< Step-up configuration */
    PML_APP_MODE_EXTERNAL_UP    = 3,    /**< External-up configuration */
} PML_AppMode_t;


/**
 * @brief Data structure holding the voltage monitor configuration
 */
typedef struct PACKED_STRUCT
{
    /// Actual voltage level corresponding to application mode - VBAT1/VCC [level]
    Voltage_t voltage;
    //PML_LdoPaLvl_t ldoPa;
    //PML_LdoVccLvl_t ldoVcc;
    //PML_LdoDigLvl_t ldoDig;
} PML_Voltage_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

extern volatile PML_AppMode_t gPmlAppMode;
extern volatile PML_Voltage_t gPmlVoltage;


/** @} (end addtogroup PML_PWR) */
/** @} (end addtogroup drivers) */

#endif // _PML_PWR_H
