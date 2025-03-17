////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware - Watchdog Example
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
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
#include "nvm_entry.h"
#include "pml.h"
#include "uart.h"
#include "gpio.h"
#include "prot_timer.h"
#include "sleep_timer.h"
#include "timer.h"
#include "timer_hal.h"
#include "printf.h"
#include "puts.h"
#include "bsp.h"
#include "watchdog.h"


/******************************************************************************\
 *  Types
\******************************************************************************/

/// Watchdog application task type.
typedef struct
{
    QActive super;
} WD_APP_TASK_AO_t;

/// Watchdog application task signals.
typedef enum
{
    TIMER_TICK_SIG = Q_USER_SIG,
} WD_APP_TASK_SIG_t;


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Watchdog period (5 seconds).
#define WATCHDOG_PERIOD     (5000000ul)

/// Maximum number of events for the watchdog task.
#define NUM_WD_TASK_EVENTS  (4u)


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Module name.
Q_DEFINE_THIS_MODULE("WD")

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[NUM_WD_TASK_EVENTS *
    sizeof(QEventParams)];

/// Event queue for the watchdog task.
static SECTION_NP_NOINIT const QEvt* gWDTaskEventQueue[NUM_WD_TASK_EVENTS];

/// Watchdog task active object.
static WD_APP_TASK_AO_t AO_WD_Task;

/// Watchdog example counter.
static uint32_t gCounter = 0ul;


/******************************************************************************\
 *  External global variables
\******************************************************************************/

/// PML configuration structure.
extern volatile PML_Configuration_t gPML_Config;


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Init the QPC event pool.
 */
static void MAIN_InitEventPool(void);

/**
 * @brief Watchdog task INITIAL state.
 */
static QState WD_TASK_initial(WD_APP_TASK_AO_t * const me);

/**
 * @brief Watchdog task IDLE state.
 */
static QState WD_TASK_idle(WD_APP_TASK_AO_t * const me, QEvt *pEvt);

/**
 * @brief Timer callback.
 */
static void Timer_Callback(Driver_Status_t status, void *pUserData);


/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void MAIN_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}

static QState WD_TASK_initial(WD_APP_TASK_AO_t * const me)
{
    return Q_TRAN(&WD_TASK_idle);
}

static QState WD_TASK_idle(WD_APP_TASK_AO_t * const me, QEvt *pEvt)
{
    QState qstatus = Q_HANDLED();
    QEventParams *pEvent = ((QEventParams*)pEvt);

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            // Configure a 1Hz timer.
            int8_t idx = Timer_SchedulePolling(1.0f, Timer_Callback, NULL);
            Timer_Enable(idx);
        }
        break;

        case TIMER_TICK_SIG:
        {
            // Print the counter value.
            printf("Counter: %u\r\n", ++gCounter);

            //-------------------------------
            // Simulate a SW issue after 10s.
            //-------------------------------
            if (gCounter == 10)
            {
                while (true);
            }
            //-------------------------------
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

static void Timer_Callback(Driver_Status_t status, void *pUserData)
{
    QK_ISR_ENTRY();

    // Post an event to the watchdog task.
    QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams,
        (enum_t)TIMER_TICK_SIG);
    QACTIVE_POST(&AO_WD_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();
}


/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Set the Watchdog Timer period.
    WATCHDOG_ResetAndSetPeriodUs(WATCHDOG_PERIOD);
    // Enable the Watchdog Timer.
    WATCHDOG_Enable();

    // Register the modules.
    Timer_RegisterModule();
    UART_RegisterModule();

    // Enable the unitimer.
    gTimer_Config.enabled = true;

    if (!PML_DidBootFromSleep())
    {
        // Initialize QPC.
        QF_init();

        // Initialize the QPC event pool.
        MAIN_InitEventPool();

        // Update the GPIO default configuration.

        // Disable all inputs.
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 =
            (uint32_t)0x000;

        // Enable outputs on GPIO7 for UART TX, disable output on other pins.
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 =
            (uint32_t)GPIO_MASK_PIN_7;

        // Disable pull-downs on all pins.
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32 =
            (uint32_t)0x000;

        // Enable pull-up on GPIO7 only.
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32 =
            (uint32_t)GPIO_MASK_PIN_7;

        // Set the UART TX output function on GPIO7.
        gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
            (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;
    }
    else
    {
        // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
        MAIN_InitEventPool();
    }

    // Disable sleep mode.
    //gPML_Config.sleepModeForbiden = true;
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    if (!PML_DidBootFromSleep())
    {
        // Reset by Watchdog?
        if (PML_GetResetFlags() & PML_WD_FLG_MASK)
        {
            // Print the watchdog reset message.
            printf("\r\n");
            printf("!!! RESET BY WATCHDOG !!!\r\n");
            printf("\r\n");
        }
        else
        {
            // Print the start-up message.
            printf("*****************************\r\n");
            printf("* EM9305 - Watchdog example *\r\n");
            printf("*****************************\r\n");
            printf("\r\n");
        }

        // Create the watchdog task.
        QActive_ctor(&AO_WD_Task.super, Q_STATE_CAST(&WD_TASK_initial));

        // Start the watchdog task.
        QACTIVE_START(&AO_WD_Task.super, (uint_fast8_t)gNextAvailablePriority--,
            &gWDTaskEventQueue[0], NUM_WD_TASK_EVENTS, 0, 0, NULL);

        // Run QPC.
        (void)QF_run();
    }
    else
    {
        // Resume unitimer.
        Timer_Resume();

        // Resume QPC
        (void)QF_resume();
    }

    // Initialization failed!
    while (true)
    {
        HaltCPU();
    }
}

/**
 * @brief This is called by QPC when there are no more tasks to run.
 * @note Application specific part called from the QK_onIdle function.
 */
void QK_onIdleExt(void)
{
    // Reset the Watchdog Timer in case sleep mode cannot be entered.
    WATCHDOG_ResetAndSetPeriodUs(WATCHDOG_PERIOD);
}
