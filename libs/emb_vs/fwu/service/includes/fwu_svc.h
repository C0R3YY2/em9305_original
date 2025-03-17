////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/fwu/service/includes/fwu_svc.h
///
/// @project    T9305
///
/// @brief      Firmware Update Service header file.
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

#ifndef FWUSVC_H_
#define FWUSVC_H_

#include <att_api.h>
#include <util/bstream.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup fwu_svc EM Firmware Update Service
 * @brief Firmware Update GATT based service for EM Bleu Bluetooth Stack
 * @{
 */

/******************************************************************************\
 *  Constants
\******************************************************************************/

/** @brief  Firmware Update Proprietary Service Start Handle */
#define FWUSVC_START_HDL 0x200

/** @brief  Proprietary Service Handles */
enum
{
    FWUSVC_SVC_DECL_HDL = FWUSVC_START_HDL, /*!< Proprietary service declaration */
    FWUSVC_CMD_CHARAC_DECL_HDL,             /*!< Proprietary command characteristic declaration */
    FWUSVC_CMD_CHARAC_VALUE_HDL,            /*!< Proprietary command characteristic value */
    FWUSVC_CMD_CHARAC_CCC_HDL,              /*!< Proprietary command client characteristic configuration */
#ifndef LIGHT_VARIANT
    FWUSVC_DATA_CHARAC_DECL_HDL,            /*!< Proprietary data characteristic declaration */
    FWUSVC_DATA_CHARAC_VALUE_HDL,           /*!< Proprietary data characteristic value */
#endif // LIGHT_VARIANT
    FWUSVC_MAX_HDL                          /*!< Maximum handle. */
};

/** @brief 16-bit UUID for Firmware Update service */
#define UUID16_FWU_SERVICE 0xA888u

/** @brief 16-bit UUID for Firmware Update Command characteristic */
#define UUID16_FWU_CMD  0xA9A0u

/** @brief 16-bit UUID for Firmware Update Data characteristic */
#define UUID16_FWU_DATA 0xAAB8u

/** @brief Base UUID:  81cfXXXX-454D-11e8-adc0-fa7ae01bd428 */
/** @brief Base UUID PART1":  454D-11e8-adc0-fa7ae01bd428 */
#define UUID_EM_BASE_PART1 0x28u, 0xD4u, 0x1Bu, 0xE0u, 0x7Au, 0xFAu, 0xC0u, 0xADu, 0xE8u, 0x11u, 0x4Du, 0x45u
/** @brief Base UUID PART2":  81cf */
#define UUID_EM_BASE_PART2 0xCFu, 0x81u

/** @brief Macro for building UUIDs */
#define UUID_EM_BUILD(uuid16) UUID_EM_BASE_PART1, UINT16_TO_BYTES(uuid16), UUID_EM_BASE_PART2

/** @brief Proprietary services */
#define UUID_FWU_SERVICE UUID_EM_BUILD(UUID16_FWU_SERVICE)

/** @brief Proprietary Command characteristics */
#define UUID_FWU_CMD UUID_EM_BUILD(UUID16_FWU_CMD)

/** @brief Proprietary Data characteristics */
#define UUID_FWU_DATA UUID_EM_BUILD(UUID16_FWU_DATA)

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Add the service to the attribute server.
 */
void FwuSvc_AddGroup(void);

/**
 * @brief Remove the service from the attribute server.
 */
void FwuSvc_RemoveGroup(void);

/**
 * @brief Register callbacks for the service.
 * @param readCback   Read callback function.
 * @param writeCback  Write callback function.
 */
void FwuSvc_RegisterCback(attsReadCback_t readCback, attsWriteCback_t writeCback);

/** @} */ // End of group fwu_svc
/** @} */ // End of addtogroup emb_vs

#endif /* FWUSVC_H_ */
