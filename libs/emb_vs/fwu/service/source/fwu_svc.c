////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/fwu/service/source/fwu_svc copy.c
///
/// @project    T9305
///
/// @brief      Firmware Update Service source file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic-Marin SA
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

#include <att_api.h>
#include <att_uuid.h>
#include <util/bstream.h>
#include <fwu_svc.h>

/**************************************************************************************************
 Constant definition
**************************************************************************************************/

/** @brief Firmware Update Proprietary Service Maximum Handle */
#define FWUSVC_END_HDL (FWUSVC_MAX_HDL - 1)

/**************************************************************************************************
 Service variables
**************************************************************************************************/

// Proprietary service declaration
static const uint8_t gcServiceUuid[ATT_128_UUID_LEN] = {UUID_FWU_SERVICE}; //lint !e572
static const uint16_t gcServiceUuidLen = sizeof(gcServiceUuid);

// Proprietary Command characteristic declaration
static const uint8_t gcCmdCharacDecl[] = {
    ATT_PROP_INDICATE | ATT_PROP_WRITE,
    UINT16_TO_BYTES(FWUSVC_CMD_CHARAC_VALUE_HDL),
    UUID_FWU_CMD
}; //lint !e572
static const uint16_t gcCmdCharacDeclLen = sizeof(gcCmdCharacDecl);

// Proprietary Command characteristic UUID
static const uint8_t gcCmdCharacUuid[ATT_128_UUID_LEN] = {UUID_FWU_CMD}; //lint !e572

// Proprietary Command characteristic value - variable length
static uint8_t gCmdCharacValue[] = {0};
static uint16_t gCmdCharacValueLen = sizeof(gCmdCharacValue);

// Proprietary Command client characteristic configuration
static uint8_t gCmdCharacCcc[] = {UINT16_TO_BYTES(0x0000)}; //lint !e572
static const uint16_t gcCmdCharacCccLen = sizeof(gCmdCharacCcc);

// Do not include Data characteristic in the light variant
#ifndef LIGHT_VARIANT

// Proprietary Data characteristic declaration
static const uint8_t gcDataCharacDecl[] = {
    ATT_PROP_WRITE_NO_RSP,
    UINT16_TO_BYTES(FWUSVC_DATA_CHARAC_VALUE_HDL),
    UUID_FWU_DATA
}; //lint !e572
static const uint16_t gcDataCharacDeclLen = sizeof(gcDataCharacDecl);

// Proprietary Data characteristic UUID
static const uint8_t gcDataCharacUuid[ATT_128_UUID_LEN] = {UUID_FWU_DATA}; //lint !e572

// Proprietary Data characteristic value - variable length
static uint8_t gDataCharacValue[] = {0};
static uint16_t gDataCharacValueLen = sizeof(gDataCharacValue);

#endif /*LIGHT_VARIANT*/

/** @brief Attribute list for FWUS group */
static const attsAttr_t gcFwuSvc_List[] =
{
    // Firmware Update Service - Primary Service Declaration
    {
        .pUuid       = attPrimSvcUuid,
        .pValue      = (uint8_t *)&gcServiceUuid[0],
        .pLen        = (uint16_t *)&gcServiceUuidLen,
        .maxLen      = sizeof(gcServiceUuid),
        .settings    = 0,
        .permissions = ATTS_PERMIT_READ,
    },
    // Firmware Update Service - Command Characteristic Declaration
    {
        .pUuid       = attChUuid,
        .pValue      = (uint8_t *)&gcCmdCharacDecl[0],
        .pLen        = (uint16_t *)&gcCmdCharacDeclLen,
        .maxLen      = sizeof(gcCmdCharacDecl),
        .settings    = 0,
        .permissions = ATTS_PERMIT_READ
    },
    // Firmware Update Service - Command Characteristic Value
    {
        .pUuid       = gcCmdCharacUuid,
        .pValue      = (uint8_t *)&gCmdCharacValue[0],
        .pLen        = (uint16_t *)&gCmdCharacValueLen,
        .maxLen      = ATT_VALUE_MAX_LEN,
        .settings    = (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK | ATTS_SET_ALLOW_OFFSET),
        .permissions = ATTS_PERMIT_WRITE
    },
    // Firmware Update Service - Command Characteristic CCCD
    {
        .pUuid       = attCliChCfgUuid,
        .pValue      = (uint8_t *)&gCmdCharacCcc[0],
        .pLen        = (uint16_t *)&gcCmdCharacCccLen,
        .maxLen      = sizeof(gCmdCharacCcc),
        .settings    = ATTS_SET_CCC,
        .permissions = (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
    },

#ifndef LIGHT_VARIANT
    // Firmware Update Service - Data Characteristic Declaration
    {
        .pUuid       = attChUuid,
        .pValue      = (uint8_t *)&gcDataCharacDecl[0],
        .pLen        = (uint16_t *)&gcDataCharacDeclLen,
        .maxLen      = sizeof(gcDataCharacDecl),
        .settings    = 0,
        .permissions = ATTS_PERMIT_READ
    },
    // Firmware Update Service - Data Characteristic Value
    {
        .pUuid       = gcDataCharacUuid,
        .pValue      = (uint8_t *)&gDataCharacValue[0],
        .pLen        = (uint16_t *)&gDataCharacValueLen,
        .maxLen      = ATT_VALUE_MAX_LEN,
        .settings    = (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK | ATTS_SET_ALLOW_OFFSET),
        .permissions = ATTS_PERMIT_WRITE
    }
#endif // LIGHT_VARIANT
};

/** @brief Firmware Update service group structure */
static attsGroup_t gFwuSvc_Group =
{
    NULL,
    (attsAttr_t *) gcFwuSvc_List,
    NULL,
    NULL,
    FWUSVC_START_HDL,
    FWUSVC_END_HDL
};

/**
 * @brief Add the services from the attribute server.
 */
void FwuSvc_AddGroup(void)
{
    AttsAddGroup(&gFwuSvc_Group);
}

/**
 * @brief Remove the services from the attribute server.
 */
void FwuSvc_RemoveGroup(void)
{
    AttsRemoveGroup(FWUSVC_START_HDL);
}

/**
 * @brief Register read and write callbacks for the ATT group.
 *
 * @param writeCb Write callback function
 * @param readCb Read callback function
 *
 * @return None.
 *
 */
void FwuSvc_RegisterCback(attsReadCback_t readCback, attsWriteCback_t writeCback)
{
    gFwuSvc_Group.writeCback = writeCback;
    gFwuSvc_Group.readCback = readCback;
}
