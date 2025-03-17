////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_qpc_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware
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

#include <bsp.h>
#include <em_system.h>
#include <em_transport_manager.h>
#include <gpio.h>
#include <macros.h>
#include <nvm_entry.h>
#include <pml.h>
#include <spi_slave.h>
#include <types.h>
#include <uart.h>
#include <my_app_1_task.h>
#include <my_app_2_task.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

#define QPC_EVENT_POOL_SIZE (MY_APP_1_TASK_EVENTS_QUEUE_SIZE + MY_APP_2_TASK_EVENTS_QUEUE_SIZE)

/******************************************************************************\
 *  External variables
\******************************************************************************/

extern const EMSystem_CommandParser_t gEMSCmdRomCommandParsers[];
extern const uint16_t gEMSCmdRomNumberOfCommandParsers;
extern const EMSystem_CommandParser_t gEMSCmdNvmCommandParsers[];
extern const uint16_t gEMSCmdNvmNumberOfCommandParsers;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static void Main_InitEventPool(void);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
#ifdef BUILD_NVM_UART
    UART_RegisterModule();
    gConfig.pTransport = &gUART_Transport;
#else
    SPIS_RegisterModule();
    gConfig.pTransport = &gSPIS_Transport;
#endif

    // Check if it wakes up from sleep.
    if (PML_DidBootFromSleep())
    {
        // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;
    }
    else
    {
#ifdef BUILD_NVM_UART
        GPIO_LoadDefaultConfigUART();
#else
        GPIO_LoadDefaultConfigSPI();
#endif

        // Initialize QPC
        QF_init();
    }

    // Initialize the QPC event pool.
    Main_InitEventPool();

    // Disable sleep mode if JTAG debug is used.
    // gPML_Config.sleepModeForbiden = true;

    // Enable JTAG.
    // GPIO_EnableJtag4Wires();
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
        // Resume the EM System layer.
        EMSystem_Resume();

        // Resume the EM Transport Manager.
        EMTransportManager_Resume();

        // Resume QPC
        (void)QF_resume();
    }
    else
    {
        // Initialize the EM System layer.
        (void)EMSystem_Init();

        // Register the commands handlers for EMS commands.
        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdNvmCommandParsers, gEMSCmdNvmNumberOfCommandParsers);
        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdRomCommandParsers, gEMSCmdRomNumberOfCommandParsers);

        // Create and start the application task 1.
        MyApp1Task_Create();
        MyApp1Task_Start();

        // Create and start the application task 2.
        MyApp2Task_Create();
        MyApp2Task_Start();

        // Initialize and start the EM Transport Manager.
        (void)EMTransportManager_InitWithSleepCB(NULL,
            EMTRANSPORTMANAGER_RX_BUFFER_MAX_SIZE, 4u,
            EMTRANSPORTMANAGER_TX_BUFFER_MAX_SIZE, 4u);

        // Send the "Enter Active Mode" event.
        uint8_t eventParams[] = { EVENT_VENDOR_CODE_ENTER_ACTIVE_MODE };
        (void)EMTransportManager_SendEvent(EVENT_VENDOR_SPECIFIC,
                sizeof(eventParams), eventParams, NULL);

        // Run QPC
        (void)QF_run();
    }

    // Initialization failed!
    while (true)
    {
        HaltCPU();
    }
}


/******************************************************************************\
 *  Private functions definition
\******************************************************************************/

static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}
