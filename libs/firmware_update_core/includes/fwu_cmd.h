////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_update_core/includes/fwu_cmd.h
///
/// @project    T9305
///
/// @brief      Firmware update commands header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
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

#ifndef _FWU_CMD_H
#define _FWU_CMD_H

#include "types.h"
#include "firmware_header.h"
#include "fwu_error_code.h"
#include "ECCTypedef.h"
#include "SHA256.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup fwu_core Firmware Update Core
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @def FWU_CMD_OPCODE_GET_FWU_INFORMATION
 * Get various information on the device to be updated.
 *
 * See function @ref FWU_GetFirmwareUpdateInformationResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_GET_FWU_INFORMATION            0x01u

/**
 * @def FWU_CMD_OPCODE_GET_FW_AREA_CNT
 * Get the number of firmware images stored in the device.
 *
 * See function @ref FWU_GetFirmwareAreaCountResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_GET_FW_AREA_CNT                0x02u

/**
 * @def FWU_CMD_OPCODE_GET_FIRMWARE_INFORMATION
 * Get a specific firmware image identification information.
 *
 * See function @ref FWU_GetFirmwareInformationResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_GET_FIRMWARE_INFORMATION       0x03u

/**
 * @def FWU_CMD_OPCODE_REBOOT
 * Trigger a reboot in a specific mode.
 *
 * See function @ref FWU_RebootResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_REBOOT                         0x04u

/**
 * @def FWU_CMD_OPCODE_GET_CURRENT_FIRMWARE_INFO
 * Get the current executing firmware image information.
 *
 * See function @ref FWU_GetFirmwareInformationResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_GET_CURRENT_FIRMWARE_INFO      0x05u

/**
 * @def FWU_CMD_OPCODE_INVALIDATE_FW_AREA
 * Invalidate a firmware area and make it unusable by the device.
 *
 * See function @ref FWU_InvalidateFwAreaResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_INVALIDATE_FW_AREA             0x06u

/**
 * @def FWU_CMD_OPCODE_UPLOAD_INIT
 * Initialize the firmware upload procedure by erasing the memory location where the new
 * firmware image will be stored.
 *
 * See function @ref FWU_UploadInitResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_UPLOAD_INIT                    0x10u

/**
 * @def FWU_CMD_OPCODE_STORE_FIRMWARE_BLOCK
 * Stores one firmware block in memory.
 *
 * See function @ref FWU_StoreFirmwareBlockResponse() details for parameters and returned values.
*/
#define FWU_CMD_OPCODE_STORE_FIRMWARE_BLOCK           0x20u

/**
 * @def FWU_CMD_OPCODE_VALIDATE_FIRMWARE_UPDATE
 * Finalize a new firmware image once it has been uploaded and written in memory.
 *
 * See function @ref FWU_ValidateFirmwareUpdateResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_VALIDATE_FIRMWARE_UPDATE       0x21u

/**
 * @def FWU_CMD_OPCODE_CRYPTO_ENGINE_INIT
 * Initialize the cryto-engine.
 *
 * See function @ref FWU_CryptoEngineInitResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_CRYPTO_ENGINE_INIT             0x30u

/**
 * @def FWU_CMD_OPCODE_CRYPTO_ENGINE_UPLOAD_SIG_MAT
 * Upload materials (elliptic key and digest) that will be used to verify a firmware image validity.
 *
 * See function @ref FWU_CryptoEngineUploadSignatureMaterialResponse() details for parameters and returned values.
 */
#define FWU_CMD_OPCODE_CRYPTO_ENGINE_UPLOAD_SIG_MAT   0x31u

/// Firmware update command opcodes typedef
typedef uint8_t FWU_CmdOpCode_t;

/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/**
 * @brief Firmware Update reboot mode definition
 */
typedef enum
{
    FWU_REBOOT_MODE_UNSPECIFIED = 0,
    FWU_REBOOT_MODE_EMCORE,
    FWU_REBOOT_MODE_APPLICATION,
    FWU_REBOOT_MODE_FIRMWARE_UPDATER,
} FWU_RebootModes_t;

/**
 * @brief Firmware Update Event Type
 */
typedef enum
{
    FWU_EVENT_VALIDATE_SIGNATURE = 0,
    FWU_EVENT_UPLOAD_INIT,
    FWU_EVENT_STORE_FIRMWARE_BLOCK,
    FWU_EVENT_VALIDATE_FIRMWARE_UPDATE,
    FWU_EVENT_INVALIDATE_FW_AREA,
    FWU_EVENT_REBOOT_ASKED,
    FWU_EVENT_NOT_VALID
} FWU_EventType_t;

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief Firmware Update Header
 */
typedef bool (*FWU_UserCheckFirmwareHeader_t)(const FIRMWARE_HEADER_T *pFirmwareHeader);

/**
 * @brief Firmware Update Event Response
 */
struct FWU_EventResponse_s
{
    /// Response Length
    uint16_t responseLen;

    /// Pointer on the response data
    uint8_t (*response)[];
};

/**
 * @brief Firmware Update Event for UploadInit
 */
typedef struct FWU_EventResponse_s FWU_EventUploadInit_t;

/**
 * @brief Firmware Update Event for StoreFwBlock
 */
typedef struct FWU_EventResponse_s FWU_EventStoreFwBlock_t;

/**
 * @brief Firmware Update Event for ValidateFwUpdate
 */
typedef struct FWU_EventResponse_s FWU_EventValidateFwUpdate_t;

/**
 * @brief Firmware Update Signature Data
 */
typedef struct
{
    /// Signature
    PointCertificate signature;

    /// Public Key
    POINT            publicKey;

    /// Digest
    uint8_t          digest[SHA256_DIGEST_SIZE];
} FWU_SignatureVerif_t;

/**
 * @brief Firmware Update Signature Event
 */
typedef struct
{
    /// Pointer on the signature material
    FWU_SignatureVerif_t *material;
} FWU_EventSignatureVerif_t;

/**
 * @brief Event type definition.
 */
typedef struct
{
    /// Type of fwu event
    FWU_EventType_t type;

    /// Status of fwu event
    FWU_ErrorCode_t status;

    /// Parameter of the fwu event
    union
    {
        FWU_EventUploadInit_t uploadInit;
        FWU_EventStoreFwBlock_t storeFwBlock;
        FWU_EventValidateFwUpdate_t validateFwUpdate;
        FWU_EventSignatureVerif_t signatureVerif;
    } params;

    /// Response Length
    uint16_t responseLen;

    /// Response Data
    uint8_t (*response)[];
} FWU_Event_t;

/**
 * @brief Callback function type definition.
 */
typedef void (*FWU_CmdCallback_t)(FWU_Event_t *event, void *pUserData);

/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
 * @brief Retrieve device informations preliminary to a firmware image update.
 * @details This function retrieves detailed information on the device and on the
 * firmware currently under execution (application or FW updater). Such information
 * is helpful to decide if a reboot to the FW updater mode shall be requested, and
 * if the device is compliant with the new firmware image that is to be uploaded.
 *
 * @note  The command execution is immediate.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   No command parameters are needed for this function.
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 *              - IC family ID (4 bytes)
 *              - IC Design iteration (2 bytes)
 *              - Firmware version (2 bytes)
 *              - Firmware type (1 byte), see FwHeader_SectionCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_GetFirmwareUpdateInformationResponse(const uint8_t *pCmdParams,
                                              uint16_t       cmdParamsLength,
                                              uint8_t       *pBuff,
                                              uint16_t      *pBuffLength,
                                              uint16_t       maxBuffLength);

/**
 * @brief Gets the number of firmware image stored in the device.
 * @note  The command execution is immediate.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   No command parameters are needed for this function.
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 *              - Number of different firmwares in the target (1 byte)
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_GetFirmwareAreaCountResponse(const uint8_t *pCmdParams,
                                      uint16_t       cmdParamsLength,
                                      uint8_t       *pBuff,
                                      uint16_t      *pBuffLength,
                                      uint16_t       maxBuffLength);

/**
 * @brief Gets a specific firmware image header informations.
 * @details The firmware updater device can enumerate the different
 *          firmware information with this function.
 *          The maximum firmware index is determined by the response to the
 *          Get_Firmware_Area_Count command.
 * @note  The command execution is immediate.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Firmware Index (1 byte), first firmware is at index 0
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 *              - Firmware header, see FIRMWARE_HEADER_T
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_GetFirmwareInformationResponse(const uint8_t *pCmdParams,
                                        uint16_t       cmdParamsLength,
                                        uint8_t       *pBuff,
                                        uint16_t      *pBuffLength,
                                        uint16_t       maxBuffLength);

/**
 * @brief Get the current executing firmware image information.
 * @note  The command execution is immediate.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   There are no command parameters.
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 *              - Firmware header, see FIRMWARE_HEADER_T
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_GetCurrentFirmwareInfoResponse(const uint8_t *pCmdParams,
                                        uint16_t       cmdParamsLength,
                                        uint8_t       *pBuff,
                                        uint16_t      *pBuffLength,
                                        uint16_t       maxBuffLength);

/**
 * @brief Invalidate a firmware area and make it unusable by the device.
 * @note  The command execution is not immediate.
 * @note  The user can get the maximum firmware area index with the command
 *        Get_Firmware_Area_Count.
 *
 * @note    If an error is immediately detected, the immediate response buffer
 *          will be filled with the error response.
 *          If the command execution was registered, the immediate response buffer
 *          is untouched and the @p cmdCallback function will be called at
 *          the end of the command execution.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Firmware Index (1 byte), first firmware is at index 0
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback User callback that will be called with the event
 *                    FWU_EVENT_INVALIDATE_FW_AREA ( see FWU_EventType_t ) when
 *                    the command is completed.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_InvalidateFwAreaResponse(const uint8_t    *pCmdParams,
                                  uint16_t          cmdParamsLength,
                                  uint8_t          *pBuff,
                                  uint16_t         *pBuffLength,
                                  uint16_t          maxBuffLength,
                                  FWU_CmdCallback_t cmdCallback,
                                  void             *pUserData);

/**
 * @brief Initialize the cryptographic engine for firmware image signature.
 * @note  The command execution is immediate.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Crypot mode, see FWU_CRYPTO_Pub_Key_Type_t
 *                   - Init Vector (16 bytes) optional, only for AES_CBC and AES_CTR
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_CryptoEngineInitResponse(const uint8_t *pCmdParams,
                                  uint16_t       cmdParamsLength,
                                  uint8_t       *pBuff,
                                  uint16_t      *pBuffLength,
                                  uint16_t       maxBuffLength);

/**
 * @brief Uploads the cryptographic keys and all relevant information.
 * @note  The command execution is not immediate.
 *
 * @note    If an error is immediately detected, the immediate response buffer
 *          will be filled with the error response.
 *          If the command execution was registered, the immediate response buffer
 *          is untouched and the @p cmdCallback function will be called at
 *          the end of the command execution.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Signature X (32 bytes)
 *                   - Signature Y (32 bytes)
 *                   - Digest (32 bytes)
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              In case of an error, the immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback User callback that will be called with the event
 *                    FWU_EVENT_VALIDATE_SIGNATURE ( see FWU_EventType_t ) when
 *                    a signature needs to be verified. Signature verification
 *                    does take a good amount of time (~200ms), it is preferred
 *                    that the user offloads this task to a less priority task.
 *                    see FWUCrypto_VerifyFirmwareSignature has to be used to
 *                    verify the signature.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_CryptoEngineUploadSignatureMaterialResponse(const uint8_t *pCmdParams,
                                                     uint16_t       cmdParamsLength,
                                                     uint8_t       *pBuff,
                                                     uint16_t      *pBuffLength,
                                                     uint16_t       maxBuffLength,
                                                     FWU_CmdCallback_t cmdCallback,
                                                     void             *pUserData);

/**
 * @brief FWU_CryptoEngineUploadSignatureMaterial command response.
 * @note  The command execution is immediate.
 *
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum response buffer length.
 * @param[in] fwuErrorCode Result of the FWUCrypto_VerifyFirmwareSignature function.
 */
void FWU_NotifySignatureMaterialVerificationResult(uint8_t    *pBuff,
                                                   uint16_t   *pBuffLength,
                                                   uint16_t    maxBuffLength,
                                                   FWU_ErrorCode_t fwuErrorCode);

/**
 * @brief Initialize the firmware update process.
 * @note  The command execution is not immediate.
 * @details The following actions will be done:
 *          - Check the header that was sent with this command
 *          - Erase the necessary NVM pages for the new firmware
 *          - Write the header in NVM without the "FHDR" magic number.
 *
 * @note    If an error is immediately detected, the immediate response buffer
 *          will be filled with the error response.
 *          If the command execution was registered, the immediate response buffer
 *          is untouched and the @p cmdCallback function will be called at
 *          the end of the command execution.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Header of the firmware that will be uploaded, see FIRMWARE_HEADER_T
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              In case of an error, the immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback If no immediate error was detected, the @p cmdCallback
 *                    function will be called with the event
 *                    FWU_EVENT_UPLOAD_INIT ( see FWU_EventType_t )
 *                    when the command is completed.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_UploadInitResponse(const uint8_t    *pCmdParams,
                            uint16_t          cmdParamsLength,
                            uint8_t          *pBuff,
                            uint16_t         *pBuffLength,
                            uint16_t          maxBuffLength,
                            FWU_CmdCallback_t cmdCallback,
                            void             *pUserData);

/**
 * @brief FWU_DataChunk command response.
 * @note  The command execution is immediate.
 * @note  If an error is detected, the @p pBuff immediate response buffer will
 *        contain the error message. If the command is successful, the immediate
 *        response buffer is not touched.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Firmware chunk data (encrypted or not)
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              In case of an error, the immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 */
void FWU_DataChunkResponse(const uint8_t *pCmdParams,
                           uint16_t       cmdParamsLength,
                           uint8_t       *pBuff,
                           uint16_t      *pBuffLength,
                           uint16_t       maxBuffLength);

/**
 * @brief FWU_StoreFirmwareBlock command response.
 * @note  The command execution is not immediate.
 * @note  If an error is immediately detected, the immediate response buffer
 *        will be filled with the error response.
 *        If the command execution was registered, the immediate response buffer
 *        is untouched and the @p cmdCallback function will be called at
 *        the end of the command execution.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   No command parameters are needed for this function.
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              In case of an error, the immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback If no immediate error was detected, the @p cmdCallback
 *                    function will be called with the event
 *                    FWU_EVENT_STORE_FIRMWARE_BLOCK ( see FWU_EventType_t )
 *                    when the command is completed.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_StoreFirmwareBlockResponse(const uint8_t    *pCmdParams,
                                    uint16_t          cmdParamsLength,
                                    uint8_t          *pBuff,
                                    uint16_t         *pBuffLength,
                                    uint16_t          maxBuffLength,
                                    FWU_CmdCallback_t cmdCallback,
                                    void             *pUserData);

/**
 * @brief Finalize a completed firmware update process.
 * @note  The command execution is not immediate.
 * @note  If an error is immediately detected, the immediate response buffer
 *        will be filled with the error response.
 *        If the command execution was registered, the immediate response buffer
 *        is untouched and the @p cmdCallback function will be called at
 *        the end of the command execution.
 * @details The following actions will be done:
 *          - Finalize the digest calculation.
 *          - Check that the digest matches the one received at the beginning of
 *            the firmware update procedure.
 *          - Verify the payload of the received firmware.
 *          - Write the magic word in the firmware header.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              In case of an error, the immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback If no immediate error was detected, the @p cmdCallback
 *                    function will be called with the event
 *                    FWU_EVENT_VALIDATE_FIRMWARE_UPDATE ( see FWU_EventType_t )
 *                    when the command is completed.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_ValidateFirmwareUpdateResponse(const uint8_t    *pCmdParams,
                                        uint16_t          cmdParamsLength,
                                        uint8_t          *pBuff,
                                        uint16_t         *pBuffLength,
                                        uint16_t          maxBuffLength,
                                        FWU_CmdCallback_t cmdCallback,
                                        void             *pUserData);

/**
 * @brief Trigger a reboot in the specified mode (EM-Core, FW updater or application).
 * @note  The command execution is immediate.
 * @details This function generates a response but doesn't execute the reboot.
 *          The reboot shall be handled by the application using the
 *          FWU_ExecuteReboot() function after the response has been sent.
 *
 * @param pCmdParams Pointer to the start of the command parameters.
 *                   The command parameters are:
 *                   - Reboot mode (1 byte), see FWU_RebootModes_t
 * @param cmdParamsLength Length of the command parameters.
 * @param pBuff Pointer to the immediate response buffer.
 *              The immediate response buffer will be filled with:
 *              - Command opcode (1 byte), see FWU_CmdOpCode_t
 *              - Command status (1 byte), see FWU_ErrorCode_t
 * @param pBuffLength Pointer to the immediate response buffer length.
 * @param maxBuffLength Maximum length of the immediate response buffer.
 * @param cmdCallback If no immediate error was detected, the @p cmdCallback
 *                    function will be called with the event
 *                    FWU_EVENT_REBOOT_ASKED ( see FWU_EventType_t ). The user
 *                    must send the response and wait for the transport to
 *                    complete its send task, and the user may want to gracefully
 *                    disconnect from the communication link before reboot.
 * @param pUserData User data that will be given back when the user callback is
 *                  called by the library.
 */
void FWU_RebootResponse(const uint8_t    *pCmdParams,
                        uint16_t          cmdParamsLength,
                        uint8_t          *pBuff,
                        uint16_t         *pBuffLength,
                        uint16_t          maxBuffLength,
                        FWU_CmdCallback_t cmdCallback,
                        void             *pUserData);

/**
 * @brief Execute a reboot in the mode previouly set by function @ref FWU_RebootResponse.
 * @note  This function shall be executed after getting the response with
 *        FWU_RebootResponse() function. It allows to wait the necessary
 *        amount of time to be sure that the response is correctly sent by
 *        the transport.
 */
void FWU_ExecuteReboot(void);

/**
 * @brief Set the user-defined firmware header verification function.
 * @note If this function is not defined, only the EM-defined checks will be
 *      performed on the firmware header.
 *
 * @param pUserCheckFirmwareHeader Pointer to the user-defined header verification function.
 */
void FWU_SetUserCheckFirmwareHeader(FWU_UserCheckFirmwareHeader_t pUserCheckFirmwareHeader);

/**
 * @}
 * @}
*/
#endif // _FWU_CMD_H
