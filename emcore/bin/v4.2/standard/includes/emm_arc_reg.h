
////////////////////////////////////////////////////////////////////////////////
///
/// @file       common/9305/includes/emm_arc_reg.h
///
/// @project    T9305
///
/// @brief  EM header file to deal with name collisions between the
///         MetaWare arc/arc_reg.h and components of the T9305 software.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2021 EM Microelectronic
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

#ifndef _EMM_ARC_REG_H
#define _EMM_ARC_REG_H

/*
 * 09/17/2021 - Gerald Stanton
 * Fix for name conflict between the MetaWare arc/arc_reg.h definition for DEBUG and
 * the DEBUG build of the Packetcraft BLE stack.
 */


// Save macros with conflicting names.
#pragma push_macro("DEBUG")
#undef  DEBUG


// Load default ARC register definition.
#include <arc/arc_reg.h>

// Make new definition for registers with conflicting name.
#define AUX_DEBUG   0x0005


// Restore macros with conflicting names.
#undef  DEBUG
#pragma pop_macro("DEBUG")

#endif  /* _EMM_ARC_REG_H */
