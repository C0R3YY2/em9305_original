////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm/timer_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Timer example to demonstrate how to use the timer library
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
/// This example starts 2 timers. The timer1 toggles GPIO6 and timer2 toggles
/// GPIO7.
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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

#include "types.h"
#include "macros.h"
#include "t9305_periph.h"
#include "nvm_entry.h"
#include "em_system.h"
#include "em_transport_manager.h"
#include "nvm_info.h"
#include "pml.h"
#include "prot_timer.h"
#include "sleep_timer.h"
#include "gpio.h"
#include "timer.h"
#include "timer_hal.h"
#include "qep.h"
#include "qf_port.h"
#include "bsp.h"


Q_DEFINE_THIS_MODULE("Task")


/******************************************************************************\
 *  Definitions
\******************************************************************************/

/// Maximum number of events for the task.
#define NUM_TASK_EVENTS               (4u)

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     (NUM_TASK_EVENTS)

/// Add an offset to all signals to avoid QPC reserved signals.
#define FIRST_SIG_OFFSET        (Q_USER_SIG)

/// Timer1 triggered
#define SIG_TIMER1_TICK         (FIRST_SIG_OFFSET + 0x01u)
/// Timer2 triggered
#define SIG_TIMER2_TICK         (FIRST_SIG_OFFSET + 0x02u)

/// Timer1 GPIO
#define TIMER1_GPIO (6)

/// Timer2 GPIO
#define TIMER2_GPIO (7)

/// Common type for all tasks.
typedef struct
{
    QActive super;
} QL_Task_t;


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE *
        sizeof(QEventParams)];

/// Event queue for the Task task.
static SECTION_NP_NOINIT const QEvt* gTaskEventQueue[NUM_TASK_EVENTS];

/// Task active object.
QL_Task_t AO_Task;


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Init the QPC event pool
 */
static void MAIN_InitEventPool(void);


/**
 * @brief QPC initial task
 */
static QState TASK_initial(QL_Task_t * const me);


/**
 * @brief QPC idle task
 */
static QState TASK_idle(QL_Task_t * const me, QEvt *pEvt);


/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    Timer_RegisterModule();

    // Enable unitimer.
    gTimer_Config.enabled = true;

    // Check if it wakes up from sleep.
    if (PML_DidBootFromSleep())
    {
        // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
        MAIN_InitEventPool();
    }
    else
    {
        // Initialize QPC
        QF_init();

        // Initialize the QPC event pool.
        MAIN_InitEventPool();
    }
}


NO_RETURN void NVM_ApplicationEntry(void)
{
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // Check if it is wake-up from sleep.
    if (PML_DidBootFromSleep())
    {
        // Resume unitimer
        Timer_Resume();

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Set HF clock source Xtal.
        PML_SetHfClkSourceNonBlocking(PML_HF_CLK_XTAL);

        // Create the QPC task.
        QActive_ctor(&AO_Task.super, Q_STATE_CAST(&TASK_initial));

        // Start QPC task.
        QACTIVE_START( &AO_Task.super, (uint_fast8_t)gNextAvailablePriority--,
               &gTaskEventQueue[0], NUM_TASK_EVENTS, 0, 0, NULL);

        // Run QPC
        (void)QF_run();
    }

    // Initialization failed!
    while (TRUE)
    {
        HaltCPU();
    }
}


/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void MAIN_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}


/**
 * @brief Timer1 callback
 *
 * This callback is called when the timer1 triggers an interrupt.
 */
static void Timer1_Callback(Driver_Status_t status, void *pUserData)
{
    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_TIMER1_TICK);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();
}


/**
 * @brief Timer2 callback
 *
 * This callback is called when the timer2 triggers an interrupt.
 */
static void Timer2_Callback(Driver_Status_t status, void *pUserData)
{
    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_TIMER2_TICK);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();
}


static QState TASK_initial(QL_Task_t * const me)
{
    return Q_TRAN(&TASK_idle);
}


static QState TASK_idle(QL_Task_t * const me, QEvt *pEvt)
{
    QState qstatus = Q_HANDLED();
    QEventParams *pEvent = ((QEventParams*)pEvt);

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            // Configure TIMER1_GPIO
            GPIO_DisablePullDown(TIMER1_GPIO);
            GPIO_DisablePullUp(TIMER1_GPIO);
            GPIO_DisableInput(TIMER1_GPIO);
            GPIO_EnableOutput(TIMER1_GPIO);
            GPIO_SetOutputPinFunction(TIMER1_GPIO, GPIO_PIN_FUNC_OUT_GPIO);
            GPIO_SetLow(TIMER1_GPIO);

            // Configure TIMER2_GPIO
            GPIO_DisablePullDown(TIMER2_GPIO);
            GPIO_DisablePullUp(TIMER2_GPIO);
            GPIO_DisableInput(TIMER2_GPIO);
            GPIO_EnableOutput(TIMER2_GPIO);
            GPIO_SetOutputPinFunction(TIMER2_GPIO, GPIO_PIN_FUNC_OUT_GPIO);
            GPIO_SetLow(TIMER2_GPIO);

            // Configure 10Hz timer (100ms)
            int8_t idx1 = Timer_SchedulePolling(10, Timer1_Callback, NULL);

            // Configure 2Hz timer (500ms)
            int8_t idx2 = Timer_SchedulePolling(2, Timer2_Callback, NULL);

            // Start timers
            Timer_Enable(idx1);
            Timer_Enable(idx2);
        }
        break;

        case SIG_TIMER1_TICK:
        {
            // Toggle TIMER1_GPIO
            GPIO_Toggle(TIMER1_GPIO);
        }
        break;

        case SIG_TIMER2_TICK:
        {
            // Toggle TIMER2_GPIO
            GPIO_Toggle(TIMER2_GPIO);
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
