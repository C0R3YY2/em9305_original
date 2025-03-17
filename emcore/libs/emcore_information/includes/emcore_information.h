////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emcore_information/includes/emcore_information.h
///
/// @project    T9305
///
/// @brief      Emcore Information API.
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

#ifndef __EMCORE_INFORMATION_H
#define __EMCORE_INFORMATION_H


/******************************************************************************\
 *  Macros
\******************************************************************************/
// Converts compiler define to a string.
#define STR_IMPLICIT(x) #x
#define STR(x) STR_IMPLICIT(x)


/******************************************************************************\
 *  Compiler Information
\******************************************************************************/
// Git version string, set as a compiler define by the cmake build system.
#ifndef EMCORE_GIT_VERSION_STR
#define EMCORE_GIT_VERSION_STR NotAvailable
#endif

// Emcore flavor, set as a compiler define by the cmake build system when emcore
// gets built. Please see emcore.cmake.
#ifndef EMCORE_FLAVOR
#define EMCORE_FLAVOR 0xBADDu
#endif


/******************************************************************************\
 *  Public Function Prototypes
\******************************************************************************/

/**
 * @brief Get the flavor as a string describing which emcore is loaded.
 * @return flavor
 */
const char *EMCoreInformation_getFlavorStr(void);

/**
 * @brief Get the flavor as a enum.
 * @return flavor
 */
uint16_t EMCoreInformation_getFlavorID(void);

/**
 * @brief Get the git describe string.
 * @return git describe string
 */
const char *EMCoreInformation_getGitVersionStr(void);

#endif
