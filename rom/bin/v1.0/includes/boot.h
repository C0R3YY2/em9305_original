////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/boot.h
///
/// @project    T9305
///
/// @brief      Boot functions in ROM
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


#ifndef _BOOT_H_
#define _BOOT_H_

#include <types.h>
#include <t9305_periph.h>


/******************************************************************************\
 *  MACRO
\******************************************************************************/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief Boot configuration structure
 * @ingroup BOOT
 */
typedef struct
{
    uint8_t configModeEnablePin;        /// Configuration mode entry pin
} BOOT_Config_t;


/**
 * @brief Module configuration
 * @ingroup BOOT
 */
typedef enum
{
    BOOT_CONFIG_TYPE_INIT       = 0,    /// Initialize modules after power up
    BOOT_CONFIG_TYPE_SAVE       = 1,    /// Save parameters before sleep
    BOOT_CONFIG_TYPE_RESTORE    = 2,    /// Restore parameters after sleep
} BOOT_ConfigType_t;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * Main boot function
 * - clears reset reason flags (POR,Sleep)
 * - call initialization of peripherals
 * - starts sleep timer
 * - continue protocol timer
 */
void BOOT_BootUp(void);


/**
 * @brief Check if the configuration mode is requested (GPIO5 toggles at 32kHz)
 * @return TRUE if configuration mode is requested, FALSE otherwise
 */
bool BOOT_IsConfigModeEntryRequested(void);


#endif // _BOOT_H_
