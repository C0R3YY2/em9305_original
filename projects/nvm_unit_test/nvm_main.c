////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm/nvm_unit_test/nvm_main.c
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
#include "pml_power.h"
#include "uart.h"
#include "spi_slave.h"
#include "gpio.h"
#include "sleep_manager.h"

#include "unit_test.h"


/******************************************************************************\
 *  External variables
\******************************************************************************/

extern const EMSystem_CommandParser_t gEMSCmdRomCommandParsers[];
extern const uint16_t gEMSCmdRomNumberOfCommandParsers;
extern const EMSystem_CommandParser_t gEMSCmdNvmCommandParsers[];
extern const uint16_t gEMSCmdNvmNumberOfCommandParsers;
extern const EMSystem_CommandParser_t gEMSCmdUnitTestCommandParsers[];
extern const uint16_t gEMSCmdUnitTestNumberOfCommandParsers;

/// List of GROUP_01 tests.
extern test_list_t GROUP_01_tests[];
/// List of GROUP_02 tests.
extern test_list_t GROUP_02_tests[];


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Test groups.
DECLARE_TEST_GROUPS = {
    INIT_TEST_GROUP(GROUP_01),
    INIT_TEST_GROUP(GROUP_02),
    END_TEST_GROUP(),
};


/******************************************************************************\
 *  Protected functions declarations
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

    if (!PML_DidBootFromSleep())
    {
#ifdef BUILD_NVM_UART
        GPIO_LoadDefaultConfigUART();
#else
        GPIO_LoadDefaultConfigSPI();
#endif
    }

    // Disable sleep mode if JTAG debug is used.
    // gPML_Config.sleepModeForbiden = true;

    // Enable JTAG.
    // GPIO_EnableJtag4Wires();
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    // Check if it is not wake-up from sleep.
    if (!PML_DidBootFromSleep())
    {
        // Initialize the EM System layer.
        (void)EMSystem_Init();

        // Register the commands handlers for EMS commands.
        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdUnitTestCommandParsers,
            gEMSCmdUnitTestNumberOfCommandParsers);
        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdNvmCommandParsers, gEMSCmdNvmNumberOfCommandParsers);
        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdRomCommandParsers, gEMSCmdRomNumberOfCommandParsers);

        // Initialize and start the EM Transport Manager.
        (void)EMTransportManager_InitWithSleepCB(NULL,
            EMTRANSPORTMANAGER_RX_BUFFER_MAX_SIZE, 4u,
            EMTRANSPORTMANAGER_TX_BUFFER_MAX_SIZE, 4u);

        // Send the "Enter Active Mode" event.
        uint8_t eventParams[] = { EVENT_VENDOR_CODE_ENTER_ACTIVE_MODE };
        (void)EMTransportManager_SendEvent(EVENT_VENDOR_SPECIFIC,
                sizeof(eventParams), eventParams, NULL);
    }
    else
    {
        // Resume the EM System layer.
        EMSystem_Resume();

        // Resume the EM Transport Manager.
        EMTransportManager_Resume();
    }

    // Unblock the communication is case the chip was sleeping.
    Transport_BlockCommunication(false);

    // Application simple scheduler.
    while(true)
    {
        // Disable interrupts.
        uint32_t irqThreshold = IRQ_DisableAndSaveInterrupts();

        // Process all the EM Transport Manager requests.
        irqThreshold = EMTransportManager_Process(irqThreshold);

        // Nothing to do, go to sleep.
        irqThreshold = SLEEP_MANAGER_GoToSleep(irqThreshold);

        // Restore interrupts.
        IRQ_RestoreInterrupts(irqThreshold);
    }
}
