////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/nvm_entry/includes/nvm_entry.h
///
/// @project    T9305
///
/// @brief      NVM Entry function
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


#ifndef _NVM_ENTRY_H
#define _NVM_ENTRY_H


#include "types.h"

#ifdef HEADER_ENABLED
#include "firmware_header_symbol.h"
#endif


/******************************************************************************\
 *  NVM ENTRY FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Function called during the NVM boot sequence, just before the modules
 * initialization. It allows the application to register the needed modules and
 * eventually update their configuration.
 * @note This function has to be implemented in the application code.
 */
void NVM_ConfigModules(void);


/**
 * @brief Entry point of the application. This function is called at the end of
 * the NVM boot sequence.
 * @note This function has to be implemented in the application code.
 * @note This function must never return.
 */
NO_RETURN void NVM_ApplicationEntry(void);


#endif // _NVM_ENTRY_H
