////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/fwu/profile/target/source/fwup_target.c
///
/// @project    T9305
///
/// @brief      Firmware Update Profile - Server side - source file.
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
#include <att_defs.h>
#include <dm_api.h>
#include <emb_dm_api.h>
#include <fwu_cmd.h>
#include <fwu_crypto.h>
#include <fwu_error_code.h>
#include <fwu_svc.h>
#include <fwup_target.h>
#include <hci_api.h>
#include <hci_defs.h>
#include <stdint.h>
#include <wsf_os.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Maximum length in bytes for the ATT indication of the firmware update service.
#define MAX_INDICATION_LEN (247)

/// Minimum requested length for firmware update command
#define EMSFU_REQUEST_LEN_HEAD 1

/******************************************************************************\
 *  Types
\******************************************************************************/

/** @brief Control block type definition */
typedef struct
{
    FwupTarget_Cfg_t   cfg;   /**< Configurable parameters */
    FwupTarget_Cback_t cback; /**< Registered user callback */
    bool rebootAsked;
    bool performRebootAfterDisconnect;
} FwupTarget_Cb_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief Control block */
static FwupTarget_Cb_t gFwupTarget_Cb;

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static void FwupTarget_ProcAttsHandleValueCnf(attEvt_t *pMsg)
{
    if ((pMsg->handle == FWUSVC_CMD_CHARAC_VALUE_HDL) &&
        (gFwupTarget_Cb.rebootAsked == true))
    {
        gFwupTarget_Cb.rebootAsked = false;
        gFwupTarget_Cb.performRebootAfterDisconnect = true;
        // Disconnect first and then reboot.
        DmConnClose(DM_CLIENT_ID_APP,
                    (dmConnId_t)pMsg->hdr.param,
                    HCI_ERR_REMOTE_TERMINATED);
    }
}

static void FwupTarget_ProcConnClose(hciDisconnectCmplEvt_t *pMsg)
{
    (void)pMsg;

    if (gFwupTarget_Cb.performRebootAfterDisconnect)
    {
        FWU_ExecuteReboot();

        // In case the reboot could not be done.
        gFwupTarget_Cb.performRebootAfterDisconnect = false;
    }
}

static void
FwupTarget_SendEvent(dmConnId_t connId, uint8_t *response, uint16_t responseLen)
{
    // Check that Indication are enabled.
    if(AttsCccEnabled(connId, gFwupTarget_Cb.cfg.cmdCharCccIdx))
    {
        // Send indication
        AttsHandleValueInd(connId,
                           FWUSVC_CMD_CHARAC_VALUE_HDL,
                           responseLen,
                           response);
    }
}

static void FwupTarget_CmdCback(FWU_Event_t *event, void *pUserData)
{
    if (event == NULL)
    {
        return;
    }

    switch (event->type)
    {
        case FWU_EVENT_VALIDATE_SIGNATURE:
        {
            if (gFwupTarget_Cb.cback != NULL)
            {
                // Signature validation takes a lot of time.
                // It is transfered to the application.
                FwupTarget_Event_t evt =
                {
                    .type = FWUPTARGET_VERIFY_SIGNATURE_EVENT,
                    .params.verifySignature.material = event->params.signatureVerif.material
                };

                gFwupTarget_Cb.cback(&evt, gFwupTarget_Cb.cfg.pUserData);
            }

            break;
        }

        case FWU_EVENT_REBOOT_ASKED:
            // Wait for the response to be sent by notification.
            gFwupTarget_Cb.rebootAsked = true;
            break;

        case FWU_EVENT_UPLOAD_INIT:
        //lint -fallthrough
        case FWU_EVENT_STORE_FIRMWARE_BLOCK:
        //lint -fallthrough
        case FWU_EVENT_VALIDATE_FIRMWARE_UPDATE:
        //lint -fallthrough
        case FWU_EVENT_INVALIDATE_FW_AREA:
        {
            dmConnId_t connId;

            connId = (dmConnId_t)((uint32_t)pUserData);

            // NVM operation is finished, we can send the indication.
            if (event->response)
            {
                FwupTarget_SendEvent(connId,
                                     *(event->response),
                                     event->responseLen);
            }
            break;
        }

        default:
            // nothing
            break;
    }
}

static void FwupTarget_ProcCmd(dmConnId_t connId,
                               uint8_t    cmdOpcode,
                               uint16_t   cmdParamsLength,
                               uint8_t   *pCmdParams,
                               uint16_t  *pResponseParamsLength,
                               uint8_t   *pResponseParams,
                               uint16_t   maxResponseParamsLength)
{
    switch (cmdOpcode)
    {
        case FWU_CMD_OPCODE_GET_FWU_INFORMATION:
        {
            FWU_GetFirmwareUpdateInformationResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength);
            break;
        }
        case FWU_CMD_OPCODE_GET_FW_AREA_CNT:
        {
            FWU_GetFirmwareAreaCountResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength);

            break;
        }

        case FWU_CMD_OPCODE_GET_FIRMWARE_INFORMATION:
        {
            FWU_GetFirmwareInformationResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength);
            break;
        }

        case FWU_CMD_OPCODE_GET_CURRENT_FIRMWARE_INFO:
        {
            FWU_GetCurrentFirmwareInfoResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength);
            break;
        }

        case FWU_CMD_OPCODE_REBOOT:
        {
            FWU_RebootResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }

#ifndef LIGHT_VARIANT
        case FWU_CMD_OPCODE_INVALIDATE_FW_AREA:
        {
            FWU_InvalidateFwAreaResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }

        case FWU_CMD_OPCODE_CRYPTO_ENGINE_INIT:
        {
            FWU_CryptoEngineInitResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength);
            break;
        }

        case FWU_CMD_OPCODE_CRYPTO_ENGINE_UPLOAD_SIG_MAT:
        {
            FWU_CryptoEngineUploadSignatureMaterialResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }

        case FWU_CMD_OPCODE_UPLOAD_INIT:
        {
            FWU_UploadInitResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }

        case FWU_CMD_OPCODE_STORE_FIRMWARE_BLOCK:
        {
            FWU_StoreFirmwareBlockResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }

        case FWU_CMD_OPCODE_VALIDATE_FIRMWARE_UPDATE:
        {
            FWU_ValidateFirmwareUpdateResponse(
                    pCmdParams,
                    cmdParamsLength,
                    pResponseParams,
                    pResponseParamsLength,
                    maxResponseParamsLength,
                    FwupTarget_CmdCback,
                    (void *)((uint32_t)connId));
            break;
        }
#endif // LIGHT_VARIANT

        default:
            // Return error in case no opcode is available
            pResponseParams[0] = cmdOpcode;
            *pResponseParamsLength += 1;
            pResponseParams[1] = FWU_ERROR_CODE_UNKNOWN_COMMAND;
            *pResponseParamsLength += 1;
            break;
    }
}

/******************************************************************************\
 *  Public functions definitions
\******************************************************************************/

/**
 * @brief Firmware Update ATTS Attribute Write Callback
 *
 * @param connId    DM connection ID.
 * @param handle    Attribute handle.
 * @param operation Operation type.
 * @param offset    Write data offset.
 * @param len       Length of data to write.
 * @param pValue    Pointer to data to write.
 * @param pAttr     Pointer to attribute structure.
*/
uint8_t FwupTarget_AttsWriteCBack(dmConnId_t  connId,
                                  uint16_t    handle,
                                  uint8_t     operation,
                                  uint16_t    offset,
                                  uint16_t    len,
                                  uint8_t    *pValue,
                                  attsAttr_t *pAttr)
{
    uint16_t responseParamsLength = 0;
    uint8_t responseParams[MAX_INDICATION_LEN];

    uint8_t attErrorCode = ATT_SUCCESS;

    switch(handle)
    {
        case FWUSVC_CMD_CHARAC_VALUE_HDL:
            // Check that the PDU is a Write Request
            if(operation == ATT_PDU_WRITE_REQ)
            {
                // Check that an opcode is available
                if (EMSFU_REQUEST_LEN_HEAD <= len)
                {
                    // Process the firmware update command
                    FwupTarget_ProcCmd(connId,
                                       pValue[0],
                                       len - 1,
                                       pValue + 1,
                                       &responseParamsLength,
                                       responseParams,
                                       MAX_INDICATION_LEN);
                }
                else
                {
                    // Return error in case no opcode is available
                    responseParams[0] = 0xFF;
                    responseParamsLength++;
                    responseParams[1] = FWU_ERROR_CODE_UNKNOWN_COMMAND;
                    responseParamsLength++;
                }

                // Need to send an indication ?
                if (responseParamsLength != 0u)
                {
                    // Check that Indication are enabled before sending response
                    if(AttsCccEnabled(connId, gFwupTarget_Cb.cfg.cmdCharCccIdx))
                    {
                        AttsHandleValueInd(connId,
                                           handle,
                                           responseParamsLength,
                                           responseParams);
                    }
                }
            }
            else
            {
                // The PDU is not supported by the characteristic
                attErrorCode = ATT_ERR_NOT_SUP;
            }
            break;

#ifndef LIGHT_VARIANT
        case FWUSVC_DATA_CHARAC_VALUE_HDL:
            // Check that the PDU is a Write Command
            if(operation == ATT_PDU_WRITE_CMD)
            {
                (void)FWU_DataChunkResponse(pValue,
                                            len,
                                            responseParams,
                                            &responseParamsLength,
                                            MAX_INDICATION_LEN);
            }
            else
            {
                // The PDU is not supported by the characteristic
                attErrorCode = ATT_ERR_NOT_SUP;
            }
            break;
#endif // LIGHT_VARIANT

        default:
            // The handler is not supported by the characteristic
            attErrorCode = ATT_ERR_HANDLE;
            break;
    }

    return attErrorCode;
}

void FwupTarget_HandleSignatureVerificationResult(dmConnId_t      connId,
                                                  FWU_ErrorCode_t fwuErrorCode)
{
    uint16_t responseParamsLength = 0;
    uint8_t responseParams[MAX_INDICATION_LEN];

    FWU_NotifySignatureMaterialVerificationResult(responseParams,
                                                  &responseParamsLength,
                                                  MAX_INDICATION_LEN,
                                                  fwuErrorCode);


    FwupTarget_SendEvent(connId, responseParams, responseParamsLength);
}

void FwupTarget_ProcMsg(wsfMsgHdr_t *pMsg)
{
    switch (pMsg->event)
    {
        case ATTS_HANDLE_VALUE_CNF:
            FwupTarget_ProcAttsHandleValueCnf((attEvt_t *)pMsg);
            break;

        case DM_CONN_CLOSE_IND:
            FwupTarget_ProcConnClose((hciDisconnectCmplEvt_t *)pMsg);
            break;

        default:
            break;
    }
}

void FwupTarget_Init(const FwupTarget_Cfg_t *pCfg, FwupTarget_Cback_t callback)
{
    gFwupTarget_Cb.cfg = *pCfg;
    gFwupTarget_Cb.cback = callback;
}
