////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/qdec/tests/test_qdec_emsas/signals.h
///
/// @project    T9305
///
/// @brief      QPC signals
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
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

#ifndef __SIGNALS_H
#define __SIGNALS_H

#include "qep.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Add an offset to all signals to avoid QPC reserved signals.
#define FIRST_SIG_OFFSET        (Q_USER_SIG)

/// QDEC stop
#define SIG_EVENT_QDEC_STOP           (FIRST_SIG_OFFSET + 0x01u)
/// QDEC start
#define SIG_EVENT_QDEC_START          (FIRST_SIG_OFFSET + 0x02u)
/// QDEC Report Ready accumulator triggered
#define SIG_QDEC_REPORT_READY_ACC     (FIRST_SIG_OFFSET + 0x03u)
/// QDEC Report Ready double mvt accumulator triggered
#define SIG_QDEC_REPORT_READY_ACC_DBL (FIRST_SIG_OFFSET + 0x04u)
/// QDEC Overflow triggered
#define SIG_QDEC_OVERFLOW             (FIRST_SIG_OFFSET + 0x05u)
/// QDEC Double Movement triggered
#define SIG_QDEC_DBL_MVT              (FIRST_SIG_OFFSET + 0x06u)

#endif // __SIGNALS_H
