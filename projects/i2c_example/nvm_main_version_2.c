////////////////////////////////////////////////////////////////////////////////
///
/// @file       i2c_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Demonstrate how to use the I2C driver with an EEPROM and printf
///             over UART on GPIO 7
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
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
#include "i2c.h"
#include "i2c_hal.h"

#include "qep.h"
#include "qf_port.h"
#include "bsp.h"

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

/// I2C operation
#define SIG_I2C                 (FIRST_SIG_OFFSET + 0x01u)

/// I2C Clock GPIO
#define GPIO_I2C_SCK            10

/// I2C SDA GPIO
#define GPIO_I2C_SDA            11

#define REG_ADDRESS             0x00

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

/// I2C Device address
static uint8_t device = 0x50;

/// I2C Buffer for read / write operation
static uint8_t buffer[1];

/// Current state machine for read / write operations
uint8_t current_state = 0;

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
    I2C_RegisterModule();

    // Enable UART.
    gUART_Config.enabled = true;

    gI2C_Config.clockFrequency = (uint8_t)ClockFrequency100;
    gI2C_Config.clockStretching = true;
    gI2C_Config.gpioSck = GPIO_I2C_SCK;
    gI2C_Config.gpioSda = GPIO_I2C_SDA;
    gI2C_Config.enabled = true;

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
        // Enable inputs for SDA and SCL
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = //lint !e655 bit-wise operation uses (compatible) enum's
            (uint32_t) GPIO_MASK(GPIO_I2C_SCK) | GPIO_MASK(GPIO_I2C_SDA);

        // Enable outputs for UART TX, SDA, and SCL
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = //lint !e655 bit-wise operation uses (compatible) enum's
            (uint32_t) GPIO_MASK_PIN_7 | GPIO_MASK(GPIO_I2C_SCK) | GPIO_MASK(GPIO_I2C_SDA);

        // Disable pull-downs on all pins
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x0000;

        // Enable pull-up on UART TX and I2C SCL and SDA
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = (uint32_t)(GPIO_MASK_PIN_7);

        // Set GPIO0 function to I2C Slave SCL/SCK
        gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel10 =
            (uint8_t)GPIO_PIN_FUNC_OUT_I2C_SCK;

        // Set GPIO1 function to I2C Slave SDA
        gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel11 =
            (uint8_t)GPIO_PIN_FUNC_OUT_I2C_SDA;

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

static QState TASK_initial(QL_Task_t * const me)
{
    return Q_TRAN(&TASK_idle);
}

static void I2C_Callback(Driver_Status_t status, void* pUserData)
{
    printf("inside i2c_callback function \r\n");

    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_I2C);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();

    printf("exitting i2c_callback function \r\n");
}

static QState TASK_idle(QL_Task_t * const me, QEvt *pEvt)
{
    printf("inside the TASK_idle function\r\n");

    QState qstatus = Q_HANDLED();
    printf("QState qstatus = Q_HANDLED();\r\n");

    QEventParams *pEvent = ((QEventParams*)pEvt);
    printf("QEventParams *pEvent = ((QEventParams*)pEvt);\r\n");

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            printf("inside Q_ENTRY_SIG\r\n");

            // Write 1 byte in EEPROM at adress 0x00
            buffer[0] = 0x11;

            printf("before Q_ENTRY_SIG I2C_ReadRegister\r\n");
            if (!I2C_WriteRegister(device, REG_ADDRESS, buffer, 1, I2C_Callback, NULL))
            {
                printf("I2C WriteRegister Error !\r\n");
            }
            printf("after Q_ENTRY_SIG I2C_ReadRegister\r\n");

            printf("exitting Q_ENTRY_SIG\r\n");
        }
        break;

        case SIG_I2C:
        {
            printf("inside SIG_I2C\r\n");

            if (current_state == 0)
            {
                printf("inside current_state 0 \r\n");

                current_state++;
                printf("0x11 written to EEPROM at address 0x00\r\n");

                printf("before current_state 0 I2C_ReadRegister \r\n");
                if (!I2C_ReadRegister(device, REG_ADDRESS, buffer, 1, I2C_Callback, NULL))
                {
                    printf("I2C ReadRegister Error !\r\n");
                }
                printf("after current_state 0 I2C_ReadRegister \r\n");

                printf("exiting current_state 0 \r\n");
            }
            else if (current_state == 1)
            {
                printf("inside current_state 1 \r\n");

                current_state++;
                printf("Data read from EEPROM at address 0x00 - 0x%02X\r\n", buffer[0]);

                if (buffer[0] != 0x11)
                {
                    printf("Error - data not written correctly to EEPROM\r\n");
                }

                // Write 1 byte in EEPROM
                buffer[0] = 0xAA;

                printf("before current_state 1 I2C_WriteRegister \r\n");
                if (!I2C_WriteRegister(device, REG_ADDRESS, buffer, 1, I2C_Callback, NULL))
                {
                    printf("I2C WriteRegister Error !\r\n");
                }
                printf("after current_state 1 I2C_WriteRegister \r\n");

                printf("exiting current_state 1 \r\n");
            }
            else if (current_state == 2)
            {
                printf("inside current_state 2 \r\n");

                current_state++;
                printf("0xAA written to EEPROM at address 0x00\r\n");

                printf("before current_state 2 I2C_ReadRegister \r\n");
                if (!I2C_ReadRegister(device, REG_ADDRESS, buffer, 1, I2C_Callback, NULL))
                {
                    printf("I2C ReadRegister Error !\r\n");
                }
                printf("after current_state 2 I2C_ReadRegister \r\n");

                printf("exiting current_state 2 \r\n");
            }
            else if (current_state == 3)
            {
                printf("inside current_state 3 \r\n");

                current_state++;
                printf("Data read from EEPROM at address 0x00 - 0x%02X\r\n", buffer[0]);

                if (buffer[0] != 0xAA)
                {
                    printf("Error - data not written correctly to EEPROM\r\n");
                }

                printf("exiting current_state 3 \r\n");
            }
        }
        break;

        default:
        {
            printf("exiting the TASK_idle function\r\n");
            qstatus = Q_SUPER(&QHsm_top);
        }
        break;
    }

    return qstatus;
}
