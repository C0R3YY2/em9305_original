////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/rtc/tests/test_rtc_emsas/signals.h
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

/// RTC Start Test
#define SIG_EVENT_RTC_START           (FIRST_SIG_OFFSET + 0x01u)
/// RTC Event when test is success
#define SIG_RTC_TEST_SUCCESS          (FIRST_SIG_OFFSET + 0x02u)
/// RTC Event when test fail
#define SIG_RTC_TEST_FAIL             (FIRST_SIG_OFFSET + 0x03u)
/// RTC Event callback
#define RTC_EVENT_SIG                 (FIRST_SIG_OFFSET + 0x04u)


#define RTC_TEST_TIME                 (FIRST_SIG_OFFSET + 0x05u)
#define RTC_TEST_TIME_STEP2           (FIRST_SIG_OFFSET + 0x06u)
#define RTC_TEST_ALM                  (FIRST_SIG_OFFSET + 0x07u)
#define RTC_TEST_ALM_STEP2            (FIRST_SIG_OFFSET + 0x08u)
#define RTC_TEST_ALM_STEP3            (FIRST_SIG_OFFSET + 0x09u)
#define RTC_TEST_DIS_ALM              (FIRST_SIG_OFFSET + 0x0Au)
#define RTC_TEST_DIS_ALM_STEP2        (FIRST_SIG_OFFSET + 0x0Bu)
#define RTC_TEST_DIS_ALM_STEP3        (FIRST_SIG_OFFSET + 0x0Cu)
#define RTC_TEST_SHORT_ALM            (FIRST_SIG_OFFSET + 0x0Du)
#define RTC_TEST_SHORT_ALM_STEP2      (FIRST_SIG_OFFSET + 0x0Eu)
#define RTC_TEST_SHORT_ALM_STEP3      (FIRST_SIG_OFFSET + 0x0Fu)
#define RTC_TEST_SHORT_ALM_STEP4      (FIRST_SIG_OFFSET + 0x10u)
#define RTC_TEST_WEEKDAY              (FIRST_SIG_OFFSET + 0x11u)
#define RTC_TEST_END_OF_DAY           (FIRST_SIG_OFFSET + 0x12u)
#define RTC_TEST_END_OF_DAY_STEP2     (FIRST_SIG_OFFSET + 0x13u)
#define RTC_TEST_ALL_ALM              (FIRST_SIG_OFFSET + 0x14u)
#define RTC_TEST_ALL_ALM_STEP2        (FIRST_SIG_OFFSET + 0x15u)
#define RTC_TEST_ALL_ALM_STEP3        (FIRST_SIG_OFFSET + 0x16u)
#define RTC_TEST_RTC_DISABLE          (FIRST_SIG_OFFSET + 0x17u)
#define RTC_TEST_RTC_DISABLE_STEP2    (FIRST_SIG_OFFSET + 0x18u)
#define RTC_TEST_RTC_DISABLE_STEP3    (FIRST_SIG_OFFSET + 0x19u)
#define RTC_TEST_RTC_DISABLE_STEP4    (FIRST_SIG_OFFSET + 0x1Au)

#endif // __SIGNALS_H
