////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/QPC/ports/arcv2em/bsp.h
///
/// @project    T9305
///
/// @brief      Board Support Package for QPC
///
/// @classification  Internal Use
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

#ifndef __BSP_H
#define __BSP_H

#include "types.h"
#include "qf_port.h"
#include "qf.h"
#include "qk.h"


/// Task priorities.
typedef enum
{
        // Task_0 (AO) priority of zero is reserved for QPC
        TASK_PRIORITY_1 = 1,
        TASK_PRIORITY_2,
        TASK_PRIORITY_3,
        TASK_PRIORITY_4,
        TASK_PRIORITY_5,
        TASK_PRIORITY_6,
        TASK_PRIORITY_7,
        TASK_PRIORITY_8,
        TASK_PRIORITY_9,
        TASK_PRIORITY_10,
        TASK_PRIORITY_11,
        TASK_PRIORITY_12,
        TASK_PRIORITY_13,
        TASK_PRIORITY_14,
        TASK_PRIORITY_15,
        TASK_PRIORITY_16,
        TASK_PRIORITY_MAX = TASK_PRIORITY_16
} AO_TASKS;

/// Next available priority.
extern AO_TASKS gNextAvailablePriority;


/// QPC event with parameter.
typedef struct {
    QEvt    super;
    void    *params;
} QEventParams;


/*! @deprecated QMActive Control Block; instead use: ::QActiveCB. */
#define QMActiveCB    QActiveCB


static ALWAYS_INLINE int_t QF_resume(void)
{
    QF_INT_DISABLE_GLOBAL();
    extern void QF_onResume(void);
    QF_onResume();  /* application-specific resume callback */
    QF_INT_ENABLE_GLOBAL();

    /* the QK idle loop */
    for (;;) {
        QK_onIdle(); /* invoke the on-idle callback */
    }
#ifdef __GNUC__  /* GNU compiler? */
    return (int_t)0;
#endif
}


void BSP_Init(void);


#endif // __BSP_H
