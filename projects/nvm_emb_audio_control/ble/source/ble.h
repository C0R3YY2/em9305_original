////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.h
///
/// @project    T9305
///
/// @brief      Bluetooth LE management header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
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

#ifndef BLE_H_
#define BLE_H_

#include <wsf_os.h>
#include <bap_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/
#ifndef AUDCTUS_CONN_MAX
#define AUDCTUS_CONN_MAX        1
#endif

/******************************************************************************\
 *  Data Structure
\******************************************************************************/

/******************************************************************************\
 *  Exported protected global variables
\******************************************************************************/

/// WSF handler ID.
extern wsfHandlerId_t gBleHandlerId;

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

void BLE_Init(void);

/*************************************************************************************************/
/*!
 *  \brief  Start advertising.
 *
 *  \param  mode   Advertising mode.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BLE_AudctusStartAdv(uint8_t mode);

/*************************************************************************************************/
/*!
 *  \brief  set advertising ext adv or not
 *
 *  \param  isLegacy   mode. - TRUE - Legacy, FALSE - Extended
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BLE_AudctusSetAdvExtType(bool isLegacy);

#ifdef __cplusplus
};
#endif

#endif // BLE_H_
