////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware - RPA Advertiser Application
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-24, EM Microelectronic-Marin SA
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

#include "ll_init_api.h"
#include "pal_cfg.h"
#include "pal_sys.h"
#include "wsf_buf.h"
#include "wsf_heap.h"
#include "wsf_os.h"
#include "wsf_timer.h"
#include "gpio.h"
#include "nvm_entry.h"
#include "pml.h"
#include "radio.h"
#include "types.h"
#include "app_task.h"
#if defined(DEBUG_MODE)
#include "printf.h"
#include "uart.h"
#endif

/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    RF_RegisterModule();

#if defined(DEBUG_MODE)
    // uart configuration for printf
    UART_RegisterModule();
    if (!PML_DidBootFromSleep())
    {
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32                   = (uint32_t)0x000;
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32                   = (uint32_t)GPIO_MASK_PIN_6;
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32                  = (uint32_t)GPIO_MASK_PIN_7;
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32                      = (uint32_t)0x000;
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32                      = (uint32_t)GPIO_MASK_PIN_6 | (uint32_t)GPIO_MASK_PIN_7;
        gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7      = (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;
        gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX = (uint8_t)GPIO_PIN_UART_RXD;
    }
#endif
    // Initialize the OS.
    WsfOsInit();

    // GPIO_EnableJtag2Wires();
    // gPML_Config.sleepModeForbiden = true;
}


NO_RETURN void NVM_ApplicationEntry(void)
{
    IRQ_EnableInterrupts();

    BbRtCfg_t bbRtCfg;
    LlRtCfg_t llRtCfg;

    PalBbLoadCfg((PalBbCfg_t *)&bbRtCfg);
    LlGetDefaultRunTimeCfg(&llRtCfg);
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_LL_PARAM, &llRtCfg.maxAdvSets,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, maxAdvSets)));

    // Custom LL configuration.
    llRtCfg.maxConn       = 1;
    llRtCfg.maxAclLen     = 128;
    llRtCfg.numTxBufs     = 1;
    llRtCfg.numRxBufs     = 1;
    llRtCfg.maxAdvReports = 0;

    if (!PML_DidBootFromSleep())
    {
        PalSysInit();

        wsfBufPoolDesc_t gMainPoolDesc[] =
            {
                {24, 5},
                {56, 4},
                {88, 3},
                {112, 2},
                {152, 2},
                {168, 1},
                {272, 3},
                {350, 1}
            };

        const uint8_t numPools =
            sizeof(gMainPoolDesc) / sizeof(gMainPoolDesc[0]);
        uint32_t memUsed = WsfBufInit(numPools, gMainPoolDesc);
        WsfHeapAlloc(memUsed);

        // Initialize OS timer.
        WsfTimerInit();

        uint8_t *pHeadFreeStartAddress = WsfHeapGetFreeStartAddress();
        uint32_t heapCountAvailable    = WsfHeapCountAvailable();

        LlInitRtCfg_t llCfg =
            {
                .pBbRtCfg     = &bbRtCfg,
                .wlSizeCfg    = 1,
                .rlSizeCfg    = 1,
                .plSizeCfg    = 1,
                .pLlRtCfg     = &llRtCfg,
                .pFreeMem     = pHeadFreeStartAddress,
                .freeMemAvail = heapCountAvailable};

        memUsed = LlInit(&llCfg);
        WsfHeapAlloc(memUsed);

        bdAddr_t bdAddr;
        (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr,
                             sizeof(bdAddr_t));
        LlSetBdAddr((uint8_t *)bdAddr);

        // Create and start the application task.
        APP_TASK_Create();
        APP_TASK_Start();
    }

    // Enter the OS main loop.
    WsfOsEnterMainLoop();

    // Initialization failed!
    while (true)
    {
        HaltCPU();
    }
}
