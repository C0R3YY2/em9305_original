////////////////////////////////////////////////////////////////////////////////
///
/// @file       watchdog.h
///
/// @project    T9305
///
/// @brief      Watchdog driver.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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


#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include <arc.h>
#include "pml.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup watchdog Watchdog driver
 * @brief Driver for the Watchdog timer.
 *
 * This module contains all the code required to configure and control the
 * Watchdog timer.
 * @{
*/


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Enable the Watchdog timer.
 */
/*LDRA_NOANALYSIS*/
static ALWAYS_INLINE void WATCHDOG_Enable(void)
{
    // Watchdog password to access the CTRL register.
    WriteAUX(0xAA, (ARC_AUX_Register*)EWDT_PASSWD);
    // Enable the Watchdog timer.
    WriteAUX(0x05, (ARC_AUX_Register*)EWDT_CTRL);
}
/*LDRA_ANALYSIS*/

/**
 * @brief Disable the Watchdog timer.
 */
static ALWAYS_INLINE void WATCHDOG_Disable(void)
{
    // Watchdog password to access the CTRL register.
    WriteAUX(0xAA, (ARC_AUX_Register*)EWDT_PASSWD);
    // Disable the Watchdog timer.
    WriteAUX(0x00, (ARC_AUX_Register*)EWDT_CTRL);
}

/**
 * @brief Reset and set the Watchdog timer period in microseconds.
 * @param period Watchdog timer period in microseconds.
 * @note Maximum Watchdog timer period:
 *   - CPU clock @ 24MHz: 178'956'970 us = ~3.0 min
 *   - CPU clock @ 48MHz:  89'478'485 us = ~1.5 min
 */
static ALWAYS_INLINE void WATCHDOG_ResetAndSetPeriodUs(uint32_t period)
{
    // Watchdog password to access the PERIOD register.
    WriteAUX(0x55, (ARC_AUX_Register*)EWDT_PASSWD);
    // Set the Watchdog timer period.
    WriteAUX(period * (PML_GetHfClkFrequency() == PML_HF_CLK_FREQ_48MHZ ?
        48 : 24), (ARC_AUX_Register*)EWDT_PERIOD);
}


/** @} */ // End of group watchdog
/** @} */ // End of addtogroup drivers


#endif /* _WATCHDOG_H */
