////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_hw_api/tests/test_common_hw_crc32/tasks.h
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

#ifndef __TASKS_H
#define __TASKS_H

#include "bsp.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Maximum number of events for the Task1 task.
#define NUM_TASK1_TASK_EVENTS               (4u)

/// Maximum number of events for the Task2 task.
#define NUM_TASK2_TASK_EVENTS               (4u)

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     ( NUM_TASK1_TASK_EVENTS + \
                                  NUM_TASK2_TASK_EVENTS )


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

/// Task1 active object.
extern QL_Task_t AO_Task1;

/// Task2 active object.
extern QL_Task_t AO_Task2;


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Create the QPC tasks.
 */
void TASKS_CreateTasks(void);

/**
 * @brief Start the QPC tasks.
 */
void TASKS_StartTasks(void);

/**
 * @brief Post an event to a QPC task.
 * @param pTask Task to which we want to post the event.
 * @param signal Signal number of the event.
 * @param params Event parameters (can be NULL).
 */
void TASKS_PostEvent(QActive* const pTask, QSignal signal, void* params);


/******************************************************************************\
 *  Inline functions declarations
\******************************************************************************/

/**
 * @brief Post an event to the Task1 task.
 * @see TASKS_PostEvent
 */
static ALWAYS_INLINE void TASK1_PostEvent(QSignal signal, void* params)
{
    TASKS_PostEvent(&AO_Task1.super, signal, params);
}

/**
 * @brief Post an event to the Task2 task.
 * @see TASKS_PostEvent
 */
static ALWAYS_INLINE void TASK2_PostEvent(QSignal signal, void* params)
{
    TASKS_PostEvent(&AO_Task2.super, signal, params);
}

#endif // __TASKS_H
