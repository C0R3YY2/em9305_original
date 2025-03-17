////////////////////////////////////////////////////////////////////////////////
///
/// @file       adc_single_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Demonstrate how to use the ADC driver in single mode and printf
///             over UART on GPIO 7
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

#include "types.h"
#include "macros.h"
#include "nvm_entry.h"
#include "pml.h"
#include "uart.h"
#include "gpio.h"
#include "common.h"
#include "printf.h"
#include "puts.h"
#include "adc.h"

#include "qep.h"
#include "qf_port.h"
#include "bsp.h"

#include "timer.h"
#include "timer_hal.h"

Q_DEFINE_THIS_MODULE("Task")

/******************************************************************************\
 *  Definitions
\******************************************************************************/

/// Maximum number of events for the task.
#define NUM_TASK_EVENTS         (8u)

/// Global event pool size.
#define QPC_EVENT_POOL_SIZE     (NUM_TASK_EVENTS)

/// Add an offset to all signals to avoid QPC reserved signals.
#define FIRST_SIG_OFFSET        (Q_USER_SIG)

/// ADC ISR triggered
#define SIG_ADC_ISR             (FIRST_SIG_OFFSET + 0x01u)

/// Timer triggered
#define SIG_TIMER_TICK          (FIRST_SIG_OFFSET + 0x02u)

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
static QState TASK_initial(QL_Task_t* const me);


/**
 * @brief QPC idle task
 */
static QState TASK_idle(QL_Task_t* const me, QEvt* pEvt);

/******************************************************************************\
 *  External variables
\******************************************************************************/

extern volatile UART_Configuration_t gUART_Config;

/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    UART_RegisterModule();
    Timer_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    // Enable unitimer.
    gTimer_Config.enabled = true;

    // Do not modify the GPIO config after a wake-up from sleep because
    // the configuration is kept in persistent memory.
    if (PML_DidBootFromSleep())
    {
        // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
        MAIN_InitEventPool();
    }
    else
    {
        // Update the GPIO default configuration

        // Disable all inputs
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)0x000;

        // Enable outputs on GPIO7 for UART TX, disable output on other pins
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

        // Disable pull-downs on all pins
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

        // Enable pull-up on GPIO7 only
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

        // Set the UART TX output function on GPIO7
        gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
            (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

        // Disable the UART RX input function
        gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
            (uint8_t)GPIO_FUNCTION_NOT_MAPPED;

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

        // Resume ADC
        ADC_Resume();

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Initialize ADC
        ADC_Init();

        // Set HF clock source Xtal.
        PML_SetHfClkSourceNonBlocking(PML_HF_CLK_XTAL);

        // Create the QPC task.
        QActive_ctor(&AO_Task.super, Q_STATE_CAST(&TASK_initial));

        // Start QPC task.
        QACTIVE_START(&AO_Task.super, (uint_fast8_t)gNextAvailablePriority--,
               &gTaskEventQueue[0], NUM_TASK_EVENTS, 0, 0, NULL);

        // Run QPC
        (void)QF_run();
    }

    while (TRUE)
    {
        HaltCPU();
    }
}

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void ADC_ConfigSourceVBAT1(void)
{
    // Set Source from VBAT1
    ADC_SetSourceSelection(ADC_SOURCE_VBAT1);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceVCC(void)
{
    // Set Source from VCC
    ADC_SetSourceSelection(ADC_SOURCE_VCC);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceGPIO5WithSampleAndHold(void)
{
    // Set Source from GPIO5 with Sample & Hold
    ADC_SetSourceSelection(ADC_SOURCE_GPIO5_WITH_SH);

    // Set Range from 0.05 V to 0.5 V
    ADC_SetRange(ADC_RANGE_0_05V_TO_0_5V);

    // Enable Sample & Hold
    ADC_EnableSampleAndHold();

    // Set Sample & Hold clock config
    ADC_SetSHClockConfig(ADC_SH_CLOCK_60000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceGPIO5(void)
{
    // Set Source from GPIO5 without Sample & Hold
    ADC_SetSourceSelection(ADC_SOURCE_GPIO5_WITHOUT_SH);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Enable Vhi for range from 1.8 V to 3.6 V instead
    // of range from 0.95 V to 1.9 V
    ADC_EnableVhi();

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ISR_Callback(Driver_Status_t status, void* pUserData)
{
    // Callback for ADC in single mode is called when a sample
    // is available.
    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_ADC_ISR);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();
}

/**
 * @brief Timer callback
 *
 * This callback is called when the timer triggers an interrupt.
 */
static void Timer_Callback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_TIMER_TICK);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();
}

static void MAIN_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}

static QState TASK_initial(QL_Task_t* const me)
{
    return Q_TRAN(&TASK_idle);
}

static QState TASK_idle(QL_Task_t* const me, QEvt* pEvt)
{
    QState qstatus = Q_HANDLED();
    QEventParams* pEvent = ((QEventParams*)pEvt);

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            const char string[] = "EM Microelectronic";

            // Print EM Microelectronic
            printf("%s\r\n", string);

            // Print EM9305
            printf("EM%d\r\n", 9305);

            // Change source_selection from 0 to 3 to
            // use the ADC with the desired source
            // 0 : VBAT1 with a range of 1.8 V to 3.6 V
            // 1 : VCC with a range of 0.95 V to 1.9 V
            // 2 : GPIO5 with Sample & Hold
            //     with this source, S&H can be enabled or not
            //     If S&H is enabled, duration should be configured
            //     Range should be selected
            //     - 0.05 V to 0.5 V
            //     - 0.05 V to 1 V
            //     - 0.05 V to 2 V
            // 3 : GPIO5 without Sample & Hold with Vhi selection
            //     If Vhi is enabled, range is 1.8 V to 3.6 V
            //     If Vhi is disabled, range if 0.95 V to 1.9 V
            uint8_t source_selection = 0;

            switch (source_selection)
            {
            case 0:
                ADC_ConfigSourceVBAT1();
                break;
            case 1:
                ADC_ConfigSourceVCC();
                break;
            case 2:
                ADC_ConfigSourceGPIO5WithSampleAndHold();
                break;
            case 3:
                ADC_ConfigSourceGPIO5();
                break;
            }

            // ADC shall be enabled after full configuration
            ADC_Enable();

            // In order to use single convertion with interrupt, a callback should be set
            ADC_RegisterCallback(ADC_ISR_Callback);

            // Configure 10Hz timer (100ms)
            int8_t idx = Timer_SchedulePolling(10, Timer_Callback, NULL);

            // Start timer
            Timer_Enable(idx);
        }
        break;

        case SIG_TIMER_TICK:
        {
            // Start a single convertion on the ADC and return back the
            // raw value.
            ADC_StartWithCallback();
        }
        break;

        case SIG_ADC_ISR:
        {
            uint16_t adc;

            // Read Data from ADC
            (void) ADC_GetValue(&adc);

            uint16_t mv = ADC_ValueToMillivolt(adc);

            printf("ADC %d -> %d mV\r\n", adc, mv);
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
