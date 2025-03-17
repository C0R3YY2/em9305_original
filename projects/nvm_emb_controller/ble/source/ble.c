////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.c
///
/// @project    T9305
///
/// @brief      Bluetooth LE management file.
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

#include "ble.h"

#include <bb_api.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <l2c_defs.h>
#include <lhci_api.h>
#include <lhci_int.h>
#include <ll_api.h>
#include <ll_init_api.h>
#include <my_app_signals.h>
#include <pal_bb.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <stdint.h>
#include <util/bda.h>
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/** @brief Maximum number of connections.
 *  @note  Must be smaller than DM_CONN_MAX.
 */
#define BLE_CONN_MAX    (4u)

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= DM_CONN_MAX,
               "BLE_CONN_MAX cannot be larger than DM_CONN_MAX.");

/** @brief ATT Desired MTU */
#define ATT_ACCEPTABLE_MTU          (247)
/** @brief Link layer runtime configuration - TX buffers. */
#define LL_RT_CFG_NUM_TX_BUFS       (2u)
/** @brief Link layer runtime configuration - RX buffers. */
#define LL_RT_CFG_NUM_RX_BUFS       (2u)

// Sanity check on the number of runtime buffers.
_Static_assert((LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS) <= UINT8_MAX,
               "Number of runtime buffers has to less than 255.");

/** @brief Link layer runtime configuration - Maximum number of pending advertising reports. */
#define LL_RT_CFG_MAX_ADV_REPORTS   (3u)

/** @brief Buffer Pool runtime configuration.
 *  @details Buffers will be allocated with the first possible match.
 */
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                        .num = 8                                             },
    { .len = 32,                        .num = 5 + 2 * BLE_CONN_MAX                          },
    { .len = 64,                        .num = 2 * BLE_CONN_MAX                              },
    { .len = 112,                       .num = BLE_CONN_MAX                                  },
    { .len = ATT_ACCEPTABLE_MTU + 24,   .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 272,                       .num = LL_RT_CFG_MAX_ADV_REPORTS                     },
    { .len = 288,                       .num = 1                                             }
};

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief WSF handler ID. */
wsfHandlerId_t gBLE_SubsysHandlerId;

/** @brief Persistent BB runtime configuration. */
static BbRtCfg_t gBbRtCfg;

/** @brief Persistent LL runtime configuration. */
static LlRtCfg_t gLlRtCfg;

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pEvt Pointer to WSF message.
 */
static void BLE_SubsysHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt)
{
    // unused
    (void)(event);

    if (pEvt != NULL)
    {
        // Perform profiles, subsystems and other tasks related operations.
        BLE_ProcSubsysMsg(pEvt);
    }
}

/**
 * @brief Process messages from profiles, subsystems and other tasks.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg)
{
    switch (pMsg->event)
    {
        // External signals posted to the BLE task.
        case BLE_USER_SIG_1:
            /* User may perform actions in response to this signal */
            break;

        case BLE_USER_SIG_2:
            /* User may perform actions in response to this signal */
            break;
    }
}

/**
 * @brief Buffer initialization.
 * @details gcBuffPoolDesc variable can be modified to suit the user's needs.
 */
static void BLE_InitBuffer(void)
{
    uint32_t memUsed;

    const uint8_t numPools = sizeof(gcBuffPoolDesc) / sizeof(gcBuffPoolDesc[0]);

    memUsed = WsfBufInit(numPools, (wsfBufPoolDesc_t *)&gcBuffPoolDesc[0]);

    WsfHeapAlloc(memUsed);
}

/**
 * @brief Get the current Link Layer runtime configuration.
 * @param[out] llRtCfg returned Link Layer runtime configuration.
 */
static void BLE_GetCurrentLlRtConfig(LlRtCfg_t *llRtCfg)
{
    LlGetDefaultRunTimeCfg(llRtCfg);
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_LL_PARAM,
                         &llRtCfg->maxAdvSets,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, maxAdvSets)));

#if BT_VER >= LL_VER_BT_CORE_SPEC_5_0
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BLE_PHY,
                         &llRtCfg->phy2mSup,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, phy2mSup)));
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_CTE,
                         &llRtCfg->cteUnused,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, cteUnused)));
#endif
}

/**
 * @brief Get the current Baseband runtime configuration.
 * @param[out] bbRtCfg returned Baseband runtime configuration.
 */
static inline void BLE_GetCurrentBbRtConfig(BbRtCfg_t *bbRtCfg)
{
    PalBbLoadCfg((PalBbCfg_t*)bbRtCfg);
}

/**
 * @brief Link Layer initialization function.
 * @param[in] llRtCfg Link Layer runtime configuration.
 * @param[in] bbRtCfg Baseband runtime configuration.
 */
static void BLE_InitLl(LlRtCfg_t *llRtCfg, BbRtCfg_t *bbRtCfg)
{
    uint32_t memUsed;

    BLE_GetCurrentBbRtConfig(bbRtCfg);

    BLE_GetCurrentLlRtConfig(llRtCfg);

    // Custom LL configuration.

    llRtCfg->maxConn       = BLE_CONN_MAX;
    llRtCfg->maxAclLen     = ATT_ACCEPTABLE_MTU + L2C_HDR_LEN;
    llRtCfg->numTxBufs     = LL_RT_CFG_NUM_TX_BUFS;
    llRtCfg->numRxBufs     = LL_RT_CFG_NUM_RX_BUFS;
    llRtCfg->maxAdvReports = LL_RT_CFG_MAX_ADV_REPORTS;
    llRtCfg->embSpecLlRtCfg.enhancedPowerControl = false;

    LlInitRtCfg_t llCfg =
    {
        .pBbRtCfg     = bbRtCfg,
        .wlSizeCfg    = 1, // Whitelist size.
        .rlSizeCfg    = 1, // Resolvable list size.
        .plSizeCfg    = 1, // Periodic list size.
        .pLlRtCfg     = llRtCfg,
        .pFreeMem     = WsfHeapGetFreeStartAddress(),
        .freeMemAvail = WsfHeapCountAvailable()
    };

    bdAddr_t bdAddr;

    memUsed = LlInitControllerInit(&llCfg);
    WsfHeapAlloc(memUsed);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));
    LlSetBdAddr((uint8_t*)bdAddr);

    // Register the EM VS extended command parser with the LL.
    LhciVsExtInit(lhciVsExtDecodeCmdPkt);
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void BLE_Init(void)
{
    wsfHandlerId_t handlerId;

    PalSysInit();

    // Initialize the Link Layer.
    BLE_InitLl(&gLlRtCfg, &gBbRtCfg);

    // Initialize the buffers.
    BLE_InitBuffer();

    // Initialize OS timer.
    WsfTimerInit();

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBLE_SubsysHandlerId = handlerId;
}
