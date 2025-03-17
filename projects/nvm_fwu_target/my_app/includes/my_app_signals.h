////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app_signals.h
///
/// @project    T9305
///
/// @brief      Application signals list and post mechanism.
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

#ifndef MY_APP_SIGNALS_H
#define MY_APP_SIGNALS_H

#include <qep.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// QPC user signal start value.
#define MY_APP_TASK_SIG_START   (Q_USER_SIG)

/// WSF message event starting value.
#define BLE_TASK_SIG_START      (0xA0u)

/******************************************************************************\
 *  Types
\******************************************************************************/

/// Application task signals enum.
typedef enum
{
    // Signals for the application task.

    MY_APP_INIT_SIG = MY_APP_TASK_SIG_START,
    MY_APP_ERROR_SIG,

    /* Add other signals for the application task here. */

    // Signals for the BLE task.
    // Those signals cannot be published with QF_PUBLISH() or subscribed to
    // with QActive_subscribe().

    /// Sent from App to BLE to start advertising.
    BLE_START_ADV_SIG = BLE_TASK_SIG_START,

    /* Add other signals for the BLE task here. */
} MyApp_Signals_t;

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

#endif // MY_APP_SIGNALS_H
