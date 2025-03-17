////////////////////////////////////////////////////////////////////////////////
///
/// @file       emcore_app_task.c
///
/// @project    T9305
///
/// @brief      EMCore - Application task
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic-Marin SA
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

#include "emcore_app_task.h"


/******************************************************************************\
 *  Types
\******************************************************************************/

/// Application task type.
typedef struct
{
    QActive super;
} APP_TASK_AO_t;


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// QPC task name.
Q_DEFINE_THIS_MODULE("App")


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Application task active object.
static APP_TASK_AO_t gAppTaskAO;

/// Events queue for the application task.
static SECTION_NP_NOINIT const QEvt* gAppTaskEventsQueue[
        APP_TASK_EVENTS_QUEUE_SIZE];


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static QState APP_TASK_initial(APP_TASK_AO_t * const me);
static QState APP_TASK_idle(APP_TASK_AO_t * const me, QEvt *pEvt);
static QState APP_TASK_error(APP_TASK_AO_t * const me, QEvt *pEvt);


/******************************************************************************\
 *  External functions prototypes
\******************************************************************************/

extern APP_TASK_EVT_ST_t APP_HandleEvent(APP_TASK_SIG_t signal, void *pParams);


/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static QState APP_TASK_initial(APP_TASK_AO_t * const me)
{
    return Q_TRAN(&APP_TASK_idle);
}

static QState APP_TASK_idle(APP_TASK_AO_t * const me, QEvt *pEvt)
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
            APP_TASK_PostEvent(APP_INIT_SIG, NULL);
        }
        break;

        default:
        {
            // Handle the application event.
            APP_TASK_EVT_ST_t eventStatus = APP_HandleEvent(
                    (APP_TASK_SIG_t)pEvent->super.sig, pEvent->params);

            // Unknonw event?
            if (APP_EVT_ST_UNKNOWN == eventStatus)
            {
                qstatus = Q_SUPER(&QHsm_top);
            }
            // Error?
            else if (APP_EVT_ST_ERROR == eventStatus)
            {
                qstatus = Q_TRAN(&APP_TASK_error);
            }
        }
        break;
    }

    return qstatus;
}

static QState APP_TASK_error(APP_TASK_AO_t * const me, QEvt *pEvt)
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
 *  Public functions declarations
\******************************************************************************/

/**
 * @brief Create the application task.
 */
void APP_TASK_Create(void)
{
    QActive_ctor( &gAppTaskAO.super, Q_STATE_CAST(&APP_TASK_initial) );
}

/**
 * @brief Start the application task.
 * @note The task priority shall be smaller than the BLE and USB tasks! The
 * variable gNextAvailablePriority is used to get the next available
 * priority. So this function shall be called after BLE and USB tasks have been
 * started to be sure the priorities are correct.
 */
void APP_TASK_Start(void)
{
    // The task priority shall be below the BLE and USB tasks! The variable
    // gNextAvailablePriority is used to get the next available priority.
    QACTIVE_START( &gAppTaskAO.super, (uint_fast8_t)gNextAvailablePriority--,
                   &gAppTaskEventsQueue[0], APP_TASK_EVENTS_QUEUE_SIZE,
                   0, 0, NULL );
}

/**
 * @brief Post an event to the application task.
 * @param signal Signal number of the event.
 * @param params Pointer to the event parameters (can be NULL).
 */
void APP_TASK_PostEvent(APP_TASK_SIG_t signal, void *pParams)
{
    QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams, (enum_t)signal);
    pEvent->params = pParams;
    QACTIVE_POST( &gAppTaskAO.super, (QEvt*)pEvent, NULL );
}
