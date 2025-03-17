////////////////////////////////////////////////////////////////////////////////
///
/// @file       bl_nvm_main.c
///
/// @project    T9305
///
/// @brief      Bootloader - Main Firmware
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

#include "types.h"
#include "macros.h"
#include "nvm_entry.h"
#include "pml.h"
#include "bsp.h"
#include "gpio.h"
#include "spi_slave.h"
#include "uart.h"
#include "bl_app_task.h"
#include "em_system.h"
#include "em_transport_manager.h"


/******************************************************************************\
 *  External functions prototypes
\******************************************************************************/

extern void HCI_Reset(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSG_ReadProductInformation(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSG_CpuReset(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSG_SetSleepOptions(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSG_CalculateCrc32(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_ReadAtAddress(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_ReadContinue(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_WriteAtAddress(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_WriteContinue(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_NVMErasePage(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_NVMGetLockPage(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSMM_NVMLockPage(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

extern void EMSG_EnterConfigurationMode(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);


/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * @brief Sorted array of command parsers.
 *
 * This is an array of function pointers to functions that parse a specific
 * command packet. Each function pointer is associated with a command opcode.
 * These parsers are sorted in ascending opcode order (for a binary search of
 * the list).
 */
const EMSystem_CommandParser_t gEMSCmdUBLCommandParsers[] =
{
    {
        .opcode     = HCI_OPCODE_RESET, // 0x0C03
        .function   = HCI_Reset,
        .authMin    = HCI_AUTHMIN_RESET,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_READ_PRODUCT_INFORMATION, // 0xFC01
        .function   = EMSG_ReadProductInformation,
        .authMin    = EMSG_AUTHMIN_READ_PRODUCT_INFORMATION,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_CPU_RESET, // 0xFC42
        .function   = EMSG_CpuReset,
        .authMin    = EMSG_AUTHMIN_CPU_RESET,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_SET_SLEEP_OPTIONS, // 0xFC49
        .function   = EMSG_SetSleepOptions,
        .authMin    = EMSG_AUTHMIN_SET_SLEEP_OPTIONS,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_CALCULATE_CRC32, // 0xFC4E
        .function   = EMSG_CalculateCrc32,
        .authMin    = EMSG_AUTHMIN_CALCULATE_CRC32,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_ENTER_CONFIGURATION_MODE, // 0xFC4F
        .function   = EMSG_EnterConfigurationMode,
        .authMin    = EMSG_AUTHMIN_ENTER_CONFIGURATION_MODE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_SET_BOOT_MODE_FLAGS, // 0xFC54
        .function   = EMSG_SetBootModeFlags,
        .authMin    = EMSG_AUTHMIN_SET_BOOT_MODE_FLAGS,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSG_OPCODE_CLEAR_BOOT_MODE_FLAGS, // 0xFC55
        .function   = EMSG_ClearBootModeFlags,
        .authMin    = EMSG_AUTHMIN_CLEAR_BOOT_MODE_FLAGS,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_READ_AT_ADDRESS, // 0xFD01
        .function   = EMSMM_ReadAtAddress,
        .authMin    = EMSMM_AUTHMIN_READ_AT_ADDRESS,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_READ_CONTINUE, // 0xFD02
        .function   = EMSMM_ReadContinue,
        .authMin    = EMSMM_AUTHMIN_READ_CONTINUE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_WRITE_AT_ADDRESS, // 0xFD03
        .function   = EMSMM_WriteAtAddress,
        .authMin    = EMSMM_AUTHMIN_WRITE_AT_ADDRESS,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_WRITE_CONTINUE, // 0xFD04
        .function   = EMSMM_WriteContinue,
        .authMin    = EMSMM_AUTHMIN_WRITE_CONTINUE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_NVM_ERASE_PAGE, // 0xFD07
        .function   = EMSMM_NVMErasePage,
        .authMin    = EMSMM_AUTHMIN_NVM_ERASE_PAGE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_NVM_GET_LOCK_PAGE, // 0xFD08
        .function   = EMSMM_NVMGetLockPage,
        .authMin    = EMSMM_AUTHMIN_NVM_GET_LOCK_PAGE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_NVM_LOCK_PAGE, // 0xFD09
        .function   = EMSMM_NVMLockPage,
        .authMin    = EMSMM_AUTHMIN_NVM_LOCK_PAGE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_WRITE_AT_ADDRESS_WITHOUT_RESPONSE, // 0xFD0B
        .function   = EMSMM_WriteAtAddress,
        .authMin    = EMSMM_AUTHMIN_WRITE_AT_ADDRESS_WITHOUT_RESPONSE,
        .reserved   = 0u,
    },
    {
        .opcode     = EMSMM_OPCODE_WRITE_CONTINUE_WITHOUT_RESPONSE, // 0xFD0C
        .function   = EMSMM_WriteContinue,
        .authMin    = EMSMM_AUTHMIN_WRITE_CONTINUE_WITHOUT_RESPONSE,
        .reserved   = 0u,
    },
};

/// Number of command parsers in the array of nvm emcore.
const uint16_t gEMSCmdUBLNumberOfCommandParsers =
    ARRAY_DIM(gEMSCmdUBLCommandParsers);

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[
    APP_TASK_EVENTS_QUEUE_SIZE * sizeof(QEventParams)];


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

static void MAIN_InitEventPool(void);


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
#ifdef BUILD_NVM_UART
        GPIO_LoadDefaultConfigUART();
#else
        GPIO_LoadDefaultConfigSPI();
#endif

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
        // Resume the EM System layer.
        EMSystem_Resume();

        // Resume the EM Transport Manager.
        EMTransportManager_Resume();

        // Resume QPC.
        (void)QF_resume();
    }
    else
    {
        // Initialize the EM System layer.
        (void)EMSystem_Init();

        (void)EMSystem_RegisterCommandsHandler(
            gEMSCmdUBLCommandParsers, gEMSCmdUBLNumberOfCommandParsers);

        // Create the application task.
        APP_TASK_Create();

        // Start the application task.
        APP_TASK_Start();

        // Initialize and start the EM Transport Manager.
        (void)EMTransportManager_InitWithSleepCB(NULL,
            EMTRANSPORTMANAGER_RX_BUFFER_MAX_SIZE, 4u,
            EMTRANSPORTMANAGER_TX_BUFFER_MAX_SIZE, 4u);

        // Send the "Enter Bootloader Mode" event.
        uint8_t eventParams[] = { EVENT_VENDOR_CODE_ENTER_BOOTLOADER_MODE };
        (void)EMTransportManager_SendEvent(EVENT_VENDOR_SPECIFIC,
                sizeof(eventParams), eventParams, NULL);

        // Example for changing boot mode to application mode using PML boot
        // mode API. Use case: bootloader performs application update, now need
        // to start new application.
        //
        // Note: this can be any of the 3 modes
        // - PML_SetBootloaderModeFlag(),
        // - PML_SetEMCoreModeFlag(), or
        // - PML_SetApplicationModeFlag()
        // depending on what you want to jump into.
        // PML_SetApplicationModeFlag();
        // ResetCPU();

        // Run QPC.
        (void)QF_run();
    }

    // Initialization failed!
    while (true)
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
