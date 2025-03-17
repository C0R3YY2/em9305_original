////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/system.h
///
/// @project    T9305
///
/// @brief      System driver.
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


#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <arc.h>
#include <t9305_periph.h>
#include <config.h>


/**
 * @defgroup drivers Hardware Drivers
 */


/**
 * @defgroup system System Driver
 * @ingroup drivers
 * @brief Driver for the System module.
 *
 * This module contains all the code required to control system behavior like
 * system frequency, clock request force/disable, and debug signal selection.
 */



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief System configuration data structure.
 * @ingroup system
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t    moduleConfig;

    /// Initial (boot) state of the System hardware.
    // @todo LRC: do we need full system register map or selected registers would be enough?
    System_RegMap_t                 hardwareState;
} System_Configuration_t;



/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
 * @brief Initialize the System module after boot.
 * @ingroup system
 */
void System_Init(void);


/**
 * @brief Save the state of the System module before sleep.
 * @ingroup system
 */
void System_Save(void);


/**
 * @brief Restore the state of the System module after wake.
 * @ingroup system
 */
void System_Restore(void);


#endif /* _SYSTEM_H */
