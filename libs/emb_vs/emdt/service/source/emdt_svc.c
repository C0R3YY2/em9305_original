////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/service/source/emdt_svc.c
///
/// @project    T9305
///
/// @brief      EM Data Transfer Service source file.
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
#include <emdt_svc.h>

/**************************************************************************************************
 Constant definition
**************************************************************************************************/

/** @brief EM DTS Proprietary Service Maximum Handle */
#define EMDTSVC_END_HDL (EMDTSVC_MAX_HDL - 1)

/**
 * @brief EM Data Transfer Data characteristic maximum length.
 * @note The characteristic length is variable.
 *       Cannot be larger than ATT_MTU - ATT_HDR_LEN - 2 (ATT_HANDLE_LEN).
 */
#define EMDTSVC_DATA_CHARAC_MAX_LEN (20)

/**************************************************************************************************
 Service variables
**************************************************************************************************/

// Proprietary service declaration
static const uint8_t gcServiceUuid[ATT_128_UUID_LEN] = {UUID_EM_DATA_TRANSFER_SERVICE}; //lint !e572
static const uint16_t gcServiceUuidLen = sizeof(gcServiceUuid);

// Proprietary Data characteristic declaration
static const uint8_t gcDataCharacDecl[] =
{
    ATT_PROP_NOTIFY | ATT_PROP_WRITE_NO_RSP,
    UINT16_TO_BYTES(EMDTSVC_DATA_CHARAC_VALUE_HDL),
    UUID_EM_DATA_TRANSFER_DATA
}; //lint !e572
static const uint16_t gcDataCharacDeclLen = sizeof(gcDataCharacDecl);

// Proprietary Data characteristic UUID
static const uint8_t gcDataCharacUuid[ATT_128_UUID_LEN] = {UUID_EM_DATA_TRANSFER_DATA}; //lint !e572

// Proprietary Data characteristic value
static uint8_t gDataCharacValue[EMDTSVC_DATA_CHARAC_MAX_LEN] = {0};
static uint16_t gDataCharacValueLen = sizeof(gDataCharacValue);

// Proprietary Data client characteristic configuration
static uint8_t gDataCharacCcc[] = {UINT16_TO_BYTES(0x0000)}; //lint !e572
static const uint16_t gcDataCharacCccLen = sizeof(gDataCharacCcc);

/** @brief Attribute list for EMDTS group */
static const attsAttr_t gcEmdtSvc_List[] =
{
    // EM Data Transfer Service - Primary Service Declaration
    {
        .pUuid       = attPrimSvcUuid,
        .pValue      = (uint8_t *)&gcServiceUuid[0],
        .pLen        = (uint16_t *)&gcServiceUuidLen,
        .maxLen      = sizeof(gcServiceUuid),
        .settings    = 0,
        .permissions = ATTS_PERMIT_READ,
    },
    // EM Data Transfer Service - Data Characteristic Declaration
    {
        .pUuid       = attChUuid,
        .pValue      = (uint8_t *)&gcDataCharacDecl[0],
        .pLen        = (uint16_t *)&gcDataCharacDeclLen,
        .maxLen      = sizeof(gcDataCharacDecl),
        .settings    = 0,
        .permissions = ATTS_PERMIT_READ
    },
    // EM Data Transfer Service - Data Characteristic Value
    {
        .pUuid       = gcDataCharacUuid,
        .pValue      = (uint8_t *)&gDataCharacValue[0],
        .pLen        = (uint16_t *)&gDataCharacValueLen,
        .maxLen      = sizeof(gDataCharacValue),
        .settings    = (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK),
        .permissions = ATTS_PERMIT_WRITE
    },
    // EM Data Transfer Service - Data Characteristic CCCD
    {
        .pUuid       = attCliChCfgUuid,
        .pValue      = (uint8_t *)&gDataCharacCcc[0],
        .pLen        = (uint16_t *)&gcDataCharacCccLen,
        .maxLen      = sizeof(gDataCharacCcc),
        .settings    = ATTS_SET_CCC,
        .permissions = (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
    }
};

/** @brief EM Data Transfer service group structure */
static attsGroup_t gEmdtSvc_Group =
{
    NULL,
    (attsAttr_t *) gcEmdtSvc_List,
    NULL,
    NULL,
    EMDTSVC_START_HDL,
    EMDTSVC_END_HDL
};

/**
 * @brief Add the services from the attribute server.
 */
void EmdtSvc_AddGroup(void)
{
    AttsAddGroup(&gEmdtSvc_Group);
}

/**
 * @brief Remove the services from the attribute server.
 */
void EmdtSvc_RemoveGroup(void)
{
    AttsRemoveGroup(EMDTSVC_START_HDL);
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
void EmdtSvc_RegisterCback(attsReadCback_t readCback, attsWriteCback_t writeCback)
{
    gEmdtSvc_Group.writeCback = writeCback;
    gEmdtSvc_Group.readCback = readCback;
}
