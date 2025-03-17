////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/fwu/profile/target/includes/fwup_target.h
///
/// @project    T9305
///
/// @brief      Firmware Update Profile - Target side - header file.
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

#ifndef FWUP_TARGET_H
#define FWUP_TARGET_H

#include <att_api.h>
#include <emb_dm_api.h>
#include <fwu_cmd.h>
#include <fwu_error_code.h>
#include <stdint.h>
#include <wsf_os.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup fwup_target Firmware Update Profile - Target side
 * @brief Proprietary profile definition example.
 * @{
 */

/******************************************************************************\
 *  Types
\******************************************************************************/

/** @brief Module configuration structure. */
typedef struct
{
    uint8_t cmdCharCccIdx; /**< Index of the data characteristic in the client characteristic configuration table (attsCccSet_t) */
    void   *pUserData;     /**< User-defined parameter that is returned when the callback is invoked. */
} FwupTarget_Cfg_t;

/** @brief Modules events enumeration. */
typedef enum
{
    FWUPTARGET_VERIFY_SIGNATURE_EVENT, /**< Event generated when the application needs to verify a signature. */
} FwupTarget_EventType_t;

/** @brief Structure of the notification started event parameters. */
typedef struct
{
    /// Pointer on the signature material
    FWU_SignatureVerif_t *material;
} FwupTarget_VerifySignatureEvt_t;

/**
 * @brief Module event structure.
 */
typedef struct
{
    /** Type of profile event */
    FwupTarget_EventType_t type;

    /** Parameters of the profile event */
    union
    {
        FwupTarget_VerifySignatureEvt_t verifySignature;
    } params;
} FwupTarget_Event_t;

/**
 * @brief Type definition of the module callback function.
 */
typedef void (*FwupTarget_Cback_t)(FwupTarget_Event_t *pEvt, void *pUserData);

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the Firmware Update Profile, Target side.
 * @param pCfg Configuration of the module.
 * @param callback Callback function provided by the user.
 */
void FwupTarget_Init(const FwupTarget_Cfg_t *pCfg, FwupTarget_Cback_t callback);

/**
 * @brief Internal processing of some messages.
 * @param pMsg Pointer to message.
 */
void FwupTarget_ProcMsg(wsfMsgHdr_t *pMsg);

/**
 * @brief Process the result of the signature verification.
 * @note  With the 'light' variant of the library, this should not be used.
 * @param connId Connection ID.
 * @param fwuErrorCode Result of the signature verification.
 */
void FwupTarget_HandleSignatureVerificationResult(dmConnId_t      connId,
                                                  FWU_ErrorCode_t fwuErrorCode);

/**
 * @brief  ATTS write callback for firmware update service.
 * @note   Use this function as a parameter to FwuSvc_RegisterCback().
 *
 * @param  connId      DM connection identifier.
 * @param  handle      ATT handle.
 * @param  operation   ATT operation.
 * @param  offset      Write offset.
 * @param  len         Write length.
 * @param  pValue      Value to write.
 * @param  pAttr       Attribute to write.
 *
 * @return ATT status.
 */
uint8_t FwupTarget_AttsWriteCBack(dmConnId_t  connId,
                                  uint16_t    handle,
                                  uint8_t     operation,
                                  uint16_t    offset,
                                  uint16_t    len,
                                  uint8_t    *pValue,
                                  attsAttr_t *pAttr);

/** @} */ // End of group fwup_target
/** @} */ // End of addtogroup emb_vs

#endif /* FWUP_TARGET_H */
