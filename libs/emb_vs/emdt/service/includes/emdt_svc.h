////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/service/includes/emdt_svc.h
///
/// @project    T9305
///
/// @brief      EM Data Transfer Service header file.
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

#ifndef EMDTSVC_H_
#define EMDTSVC_H_

#include <att_api.h>
#include <util/bstream.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup emdt_svc EM Data Transfer Service
 * @brief Proprietary service definition example.
 * @{
 */

/******************************************************************************\
 *  Constants
\******************************************************************************/

/** @brief  EM Data Transfer Proprietary Service Start Handle */
#define EMDTSVC_START_HDL 0x200

/** @brief  Proprietary Service Handles */
enum
{
    EMDTSVC_SVC_DECL_HDL = EMDTSVC_START_HDL, /*!< Proprietary service declaration */
    EMDTSVC_DATA_CHARAC_DECL_HDL,             /*!< Proprietary data characteristic declaration */
    EMDTSVC_DATA_CHARAC_VALUE_HDL,            /*!< Proprietary data characteristic value */
    EMDTSVC_DATA_CHARAC_CCC_HDL,              /*!< Proprietary data client characteristic configuration */
    EMDTSVC_MAX_HDL                           /*!< Maximum handle. */
};

/** @brief 16-bit UUID for EM data transfer service */
#define UUID16_EM_DATA_TRANSFER_SERVICE 0x6F08u

/** @brief 16-bit UUID for EM data transfer data characteristic */
#define UUID16_EM_DATA_TRANSFER_DATA 0x7192u

/** @brief Base UUID:  81cfXXXX-454D-11e8-adc0-fa7ae01bd428 */
/** @brief Base UUID PART1":  454D-11e8-adc0-fa7ae01bd428 */
#define UUID_EM_BASE_PART1 0x28u, 0xD4u, 0x1Bu, 0xE0u, 0x7Au, 0xFAu, 0xC0u, 0xADu, 0xE8u, 0x11u, 0x4Du, 0x45u
/** @brief Base UUID PART2":  81cf */
#define UUID_EM_BASE_PART2 0xCFu, 0x81u

/** @brief Macro for building UUIDs */
#define UUID_EM_BUILD(uuid16) UUID_EM_BASE_PART1, UINT16_TO_BYTES(uuid16), UUID_EM_BASE_PART2

/** @brief Proprietary services */
#define UUID_EM_DATA_TRANSFER_SERVICE UUID_EM_BUILD(UUID16_EM_DATA_TRANSFER_SERVICE)

/** @brief Proprietary characteristics */
#define UUID_EM_DATA_TRANSFER_DATA UUID_EM_BUILD(UUID16_EM_DATA_TRANSFER_DATA)

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Add the service to the attribute server.
 */
void EmdtSvc_AddGroup(void);

/**
 * @brief Remove the service from the attribute server.
 */
void EmdtSvc_RemoveGroup(void);

/**
 * @brief Register callbacks for the service.
 * @param readCback   Read callback function.
 * @param writeCback  Write callback function.
 */
void EmdtSvc_RegisterCback(attsReadCback_t readCback, attsWriteCback_t writeCback);

/** @} */ // End of group emdt_svc
/** @} */ // End of addtogroup emb_vs

#endif /* EMDTSVC_H_ */
