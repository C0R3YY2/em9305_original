////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/config.h
///
/// @project    T9305
///
/// @brief      Master module configuration data.
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


#ifndef _CONFIG_H
#define _CONFIG_H


#include "types.h"
#include "transport.h"


// Default FPGA mode
#ifndef FPGA_BUILD
#define FPGA_BUILD 0
#endif


/// Pointer to function to call to initialize a module after a cold boot.
typedef void (*Config_ModuleInitFunction_t)(void);


/// Pointer to function to call to restore the state of a module after wake.
typedef void (*Config_ModuleSaveFunction_t)(void);


/// Pointer to function to call to save the state of a module before sleep.
typedef void (*Config_ModuleRestoreFunction_t)(void);


/// Pointer to function to call any of above config functions.
typedef void (*Config_ModuleConfigFunction_t)(void);


/**
 * @brief Base structure for module configuration data.
 */
typedef struct
{
    /// Function to call to initialize the module after a cold boot.
    Config_ModuleInitFunction_t    pInitFunction;

    /// Function to call to restore the state of the module after wake.
    Config_ModuleSaveFunction_t    pSaveFunction;

    /// Function to call to save the state of the module before sleep.
    Config_ModuleRestoreFunction_t pRestoreFunction;

    /// Pointer to the persistent memory structure of the module.
    void *pPersistentMemory;

    /// Pointer to the non-persistent memory structure of the module.
    void *pNonPersistentMemory;

} Config_ModuleConfiguration_t;


/**
 * @brief Index of each module within the configuration.
 * @{
 */
enum {
    CONFIG_MODULE_PML = 0,
    CONFIG_MODULE_SYSTEM,
    CONFIG_MODULE_GPIO,
    CONFIG_MODULE_PROTTIMER,
    CONFIG_MODULE_SLEEPTIMER,
    CONFIG_MODULE_IRQ,
    CONFIG_MODULE_UART,
    CONFIG_MODULE_SPIS,
    CONFIG_MODULE_RADIO,
    CONFIG_MODULE_UNITIMER,
    CONFIG_MODULE_I2C,
    CONFIG_MODULE_SPIM,
    CONFIG_MODULE_I2S,
    CONFIG_MODULE_QDEC,
    CONFIG_MODULE_AOAD,
    CONFIG_MODULE_RTC,
    CONFIG_MODULE_TI,
    CONFIG_MODULE_SIZE,
};

/**
 * @}
 */


/**
 * @brief Master configuration data structure.
 *
 * This structure contains all global configuration data as well as pointers
 * to the configuration data for all modules.
 */
typedef struct
{
    /// Array of pointers to the configuration for each module (driver).
    const Config_ModuleConfiguration_t *pModuleConfig[CONFIG_MODULE_SIZE];

    /// HCI interface.
    const Transport_t *pTransport;
} Config_Configuration_t;


// Reference to the master configuration structure (for access by modules).
extern volatile Config_Configuration_t gConfig;


#endif /* _CONFIG_H */
