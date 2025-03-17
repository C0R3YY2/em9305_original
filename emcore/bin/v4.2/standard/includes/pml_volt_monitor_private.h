////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_volt_monitor_private.h
///
/// @project    T9305
///
/// @brief      Voltage Monitor driver internal function prototypes
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


#ifndef _VOLT_MON_PRIVATE_H
#define _VOLT_MON_PRIVATE_H


/******************************************************************************\
 *  INTERNAL FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief VoltageMonitor pre-initialization
 * It only measures voltage according to appl mode
 */
void PML_VoltMonitorPreInit(void);


/**
 * @brief VoltageMonitor initialization
 */
void PML_VoltMonitorInit(void);


/**
 * @brief VoltageMonitor store configuration before sleep
 */
void PML_VoltMonitorStoreConfig(void);


/**
 * @brief VoltageMonitor restore configuration
 */
void PML_VoltMonitorReStoreConfig(void);


#endif // _VOLT_MON_PRIVATE_H
