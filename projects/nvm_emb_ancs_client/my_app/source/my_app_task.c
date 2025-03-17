////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app_task.c
///
/// @project    T9305
///
/// @brief      Application task
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

#include <my_app_signals.h>
#include <my_app_task.h>
#include <wsf_types.h>
#include <wsf_buf.h>
#include <bsp.h>
#include <dm_api.h>
#include <dm_handler.h>

#include "my_app.h"


/******************************************************************************\
 *  Types
\******************************************************************************/

/// Application task type.
typedef struct
{
    QActive super;
} MyAppTask_AO_t;

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// QPC task name.
Q_DEFINE_THIS_MODULE("MyApp")

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Application task active object.
static MyAppTask_AO_t gMyAppTask_AO;

/// Events queue for the application task.
static SECTION_NP_NOINIT const QEvt* gMyAppTask_EventsQueue[MY_APP_TASK_EVENTS_QUEUE_SIZE];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static QState MyAppTask_Initial(MyAppTask_AO_t * const me);
static QState MyAppTask_Idle(MyAppTask_AO_t * const me, QEvt *pEvt);
static QState MyAppTask_Error(MyAppTask_AO_t * const me, QEvt *pEvt);

/******************************************************************************\
 *  External functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static QState MyAppTask_Initial(MyAppTask_AO_t * const me)
{
    return Q_TRAN(&MyAppTask_Idle);
}

static QState MyAppTask_Idle(MyAppTask_AO_t * const me, QEvt *pEvt)
{
    QState qstatus = Q_HANDLED();
    QEventParams *pEvent = (QEventParams*)pEvt;

    switch (pEvent->super.sig)
    {
        case 0:
        case Q_EXIT_SIG:
        case Q_INIT_SIG:
        case 0xFF:
            qstatus = Q_SUPER(&QHsm_top);
        break;

        case Q_ENTRY_SIG:
        {
            // Initialize the application.
            MyAppTask_PostEvent(MY_APP_INIT_SIG, NULL);
        }
        break;

        default:
        {
            // Handle the application event.
            MyApp_EvtStates_t eventStatus = MyApp_HandleEvent(
                    (MyApp_Signals_t)pEvent->super.sig, pEvent->params);

            // Unknonw event?
            if (MY_APP_EVT_ST_UNKNOWN == eventStatus)
            {
                qstatus = Q_SUPER(&QHsm_top);
            }
            // Error?
            else if (MY_APP_EVT_ST_ERROR == eventStatus)
            {
                qstatus = Q_TRAN(&MyAppTask_Error);
            }
        }
        break;
    }

    return qstatus;
}

static QState MyAppTask_Error(MyAppTask_AO_t * const me, QEvt *pEvt)
{
    QState qstatus = Q_HANDLED();
    QEventParams *pEvent = (QEventParams*)pEvt;

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            // An error occured! Try to reset the device...
            ResetCPU();
        }
        break;

        default:
        {
            qstatus = Q_SUPER(&QHsm_top);
        }
        break;
    }

    return qstatus;
}

/******************************************************************************\
 *  Public functions definitions
\******************************************************************************/

void MyAppTask_Create(void)
{
    QActive_ctor(&gMyAppTask_AO.super, Q_STATE_CAST(&MyAppTask_Initial));
}

void MyAppTask_Start(void)
{
    // The priority of the application task must be lower than other higher
    // priority tasks (e.g. USB, BLE, etc.)! The variable
    QACTIVE_START( &gMyAppTask_AO.super, (uint_fast8_t)gNextAvailablePriority--,
                   &gMyAppTask_EventsQueue[0], MY_APP_TASK_EVENTS_QUEUE_SIZE,
                   0, 0, NULL );
}

void MyAppTask_PostEvent(MyApp_Signals_t signal, void *pParams)
{
    QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams, (enum_t)signal);
    pEvent->params = pParams;
    QACTIVE_POST( &gMyAppTask_AO.super, (QEvt*)pEvent, NULL );
}
