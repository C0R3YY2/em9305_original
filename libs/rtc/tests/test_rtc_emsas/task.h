////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/qdec/tests/test_qdec_emsas/task.h
///
/// @project    T9305
///
/// @brief      QPC tasks
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

#ifndef __TASK_H
#define __TASK_H

#include "bsp.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Use printf for debugging using trace on UART
#define DEBUG_PRINTF (1)

/// Maximum number of events for the task.
#define NUM_TASK_TASK_EVENTS               (8u)

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     ( NUM_TASK_TASK_EVENTS )


/******************************************************************************\
 *  Types
\******************************************************************************/

/// Common type for all tasks.
typedef struct
{
    QActive super;
} QL_Task_t;


/******************************************************************************\
 *  External global variables
\******************************************************************************/

/// Task active object.
extern QL_Task_t AO_Task;


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Create the QPC tasks.
 */
void TASK_Create(void);

/**
 * @brief Start the QPC tasks.
 */
void TASK_Start(void);

/******************************************************************************\
 *  Inline functions declarations
\******************************************************************************/


#endif // __TASK_H
