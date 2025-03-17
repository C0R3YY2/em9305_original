////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_system/includes/em_transport_manager.h
///
/// @project    T9305
///
/// @brief      EM Transport Manager.
///
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
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

#ifndef __EM_TRANSPORT_MANAGER_H
#define __EM_TRANSPORT_MANAGER_H

#include "types.h"
#include "interrupts.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * Default EM Transport RX/TX buffers size
 */
/// Default size of the EM Transport RX buffer -> Packet indicator (1) + Command Opcode (2) + Length (1) + Data (255)
#define CONFIG_EMTRANSPORTMANAGER_RX_BUFFER_SIZE            259u
/// Default size of the EM Transport TX buffer -> Packet indicator (1) + Event code (1) + Length (1) + Data (255)
#define CONFIG_EMTRANSPORTMANAGER_TX_BUFFER_SIZE            258u

/**
 * Packet identifier
 */
/// Command packet
#define PACKET_IDENTIFIER_COMMAND                           1u
/// Event packet
#define PACKET_IDENTIFIER_EVENT                             4u

/**
 * Common offsets in all the command packets
 */
/// Offset of the packet identifier in a command packet
#define COMMAND_OFFSET_PACKET_IDENTIFIER                    0u
/// Offset of the opcode LSB in a command packet
#define COMMAND_OFFSET_OPCODE_LSB                           1u
/// Offset of the opcode MSB in a command packet
#define COMMAND_OFFSET_OPCODE_MSB                           2u
/// Offset of the length of the parameters in a command packet
#define COMMAND_OFFSET_PARAMETER_TOTAL_LENGTH               3u
/// Offset of the first parameter in a command packet
#define COMMAND_OFFSET_PARAMETER_0                          4u

/**
 * Common offsets in all the event packets
 */
/// Offset of the packet identifier in a event packet
#define EVENT_OFFSET_PACKET_IDENTIFIER                      0u
/// Offset of the event code in a event packet
#define EVENT_OFFSET_EVENT_CODE                             1u
/// Offset of the parameters total length in a event packet
#define EVENT_OFFSET_PARAMETER_TOTAL_LENGTH                 2u
/// Offset of the first parameter in a event packet
#define EVENT_OFFSET_PARAMETER_0                            3u

/**
 * Offsets in a Command Complete Event
 */
/// Offset of the number of HCI command packets which are allowed to be sent.
#define EVENT_COMMAND_COMPLETE_OFFSET_NUM_HCI_CMD           0u
/// Offset of the command opcode LSB.
#define EVENT_COMMAND_COMPLETE_OFFSET_OPCODE_LSB            1u
/// Offset of the command opcode MSB.
#define EVENT_COMMAND_COMPLETE_OFFSET_OPCODE_MSB            2u
/// Offset of the return parameters.
#define EVENT_COMMAND_COMPLETE_OFFSET_PARAMETER_0           3u

/**
 * Offsets in a Command Status Event
 */
/// Offset of the status of the command.
#define EVENT_COMMAND_STATUS_OFFSET_STATUS                  0u
/// Offset of the number of HCI command packets which are allowed to be sent.
#define EVENT_COMMAND_STATUS_OFFSET_NUM_HCI_CMD             1u
/// Offset of the command opcode LSB.
#define EVENT_COMMAND_STATUS_OFFSET_OPCODE_LSB              2u
/// Offset of the command opcode MSB.
#define EVENT_COMMAND_STATUS_OFFSET_OPCODE_MSB              3u

/**
 * Offsets in a Hardware Error Event
 */
/// Offset of the Hardware Code.
#define EVENT_HARDWARE_ERROR_OFFSET_HW_CODE                 0u

/**
 * Constants for Command Complete and Command Status events.
 */
/// Maximum number of HCI command packets which are allowed to be sent.
#define EVENT_MAX_NUM_HCI_CMD                               1u

/**
 * Events
 */
/// Event - Command Complete
#define EVENT_COMMAND_COMPLETE                              0x0Eu
/// Event - Command Status
#define EVENT_COMMAND_STATUS                                0x0Fu
/// Event - Hardware Error
#define EVENT_HARDWARE_ERROR                                0x10u
/// Event - Hardware Error
#define EVENT_VENDOR_SPECIFIC                               0xFFu

/// Maximum parameters length of an event
#define MAXIMUM_EVENT_PARAMETERS_LENGTH                     0xFFu

/**
 * Error Codes
 */
/// Error Code - Success
#define ERROR_CODE_SUCCESS                                  0x00u
/// Error Code - Unknown HCI command
#define ERROR_CODE_UNKNOWN_HCI_COMMAND                      0x01u
/// Error Code - Hardware Failure
#define ERROR_CODE_HARDWARE_FAILURE                         0x03u
/// Error Code - Authentication Failure
#define ERROR_CODE_AUTHENTICATION_FAILURE                   0x05u
/// Error Code - PIN of KEY missing
#define ERROR_CODE_PIN_OR_KEY_MISSING                       0x06u
/// Error Code - Command Disallowed
#define ERROR_CODE_COMMAND_DISALLOWED                       0x0Cu
/// Error Code - Invalid HCI Command Parameters
#define ERROR_CODE_INVALID_HCI_COMMAND_PARAMETERS           0x12u
/// Error Code - Unspecified Error
#define ERROR_CODE_UNSPECIFIED_ERROR                        0x1Fu
/// Error Code - Insufficient Security
#define ERROR_CODE_INSUFFICIENT_SECURITY                    0x2Fu
/// Error Code - Controller Busy
#define ERROR_CODE_CONTROLLER_BUSY                          0x3Au

/**
 * Vendor Hardware Codes
 */
/// Vendor Hardware Code - Synchronization lost
#define EVENT_HARDWARE_ERROR_HW_CODE_SYNC_LOST              1u

/**
 * Vendor Events Codes
 */
/// Vendor Events Codes - Enter Active Mode.
#define EVENT_VENDOR_CODE_ENTER_ACTIVE_MODE                 0x01u
/// Vendor Events Codes - HAL Notification.
#define EVENT_VENDOR_CODE_HAL_NOTIFICATION                  0x02u
/// Vendor Events Codes - Enter Configuration Mode.
#define EVENT_VENDOR_CODE_ENTER_CONFIGURATION_MODE          0x03u


/******************************************************************************\
 *  Macros
\******************************************************************************/

/**
 * Helper macros for commands opcodes.
 */
/// Create an opcode from the OGF and the OCF
#define OPCODE_FROM_OGF_OCF(ogf, ocf)                       ((uint16_t)((uint16_t)ogf << 10u) | ((uint16_t)ocf & 0x3FFu))
/// Extract the OGF from an opcode
#define OGF_FROM_OPCODE(opcode)                             ((uint8_t)((uint16_t)opcode >> 10u))
/// Extract the OCF from an opcode
#define OCF_FROM_OPCODE(opcode)                             ((uint16_t)opcode & 0x03FFu)
/// Create an opcode from the MSB and the LSB
#define OPCODE_FROM_MSB_LSB(msb, lsb)                       ((uint16_t)((uint16_t)msb << 8u) | ((uint16_t)lsb & 0xFFu))
/// Extract the MSB from an opcode
#define MSB_FROM_OPCODE(opcode)                             ((uint8_t)((uint16_t)opcode >> 8u))
/// Extract the LSB from an opcode
#define LSB_FROM_OPCODE(opcode)                             ((uint8_t)((uint16_t)opcode & 0xFFu))


/******************************************************************************\
 *  Types
\******************************************************************************/

/// RX state machine states type
typedef uint8_t     EMTransportManager_RxState_t;
/// TX state machine states type
typedef uint8_t     EMTransportManager_TxState_t;

/// Flags for pending tasks to process
typedef uint8_t     EMTransportManager_TaskToProcessFlags_t;

/// TX Complete Callback function type
typedef void        (*EMTransportManager_TxCpltCallback_t)(void);

/// Request To Process function type
typedef void        (*EMTransportManager_RequestToProcessFct_t)(void);

/// Send Event function type
typedef bool        (*EMTransportManager_SendEventFct_t)(
                        uint8_t eventCode,
                        uint16_t paramsLength,
                        const uint8_t *pParams,
                        EMTransportManager_TxCpltCallback_t pCallback);

/// EM Transport Manager memory structure.
typedef struct
{
    /// RX data length
    uint16_t    currentRxDataLength;
    /// Expected RX data length
    uint16_t    expectedRxDataLength;
    /// Number of pending RX bytes
    uint16_t    pendingRxDataLength;

    /// Current RX state
    EMTransportManager_RxState_t    rxState;
    /// Current TX state
    EMTransportManager_TxState_t    txState;

    /// TX Callback.
    EMTransportManager_TxCpltCallback_t         pTxCallback;

    /// Flags for pending tasks to process
    EMTransportManager_TaskToProcessFlags_t     taskToProcessFlags;

    /// Pointer to the RX buffer
    uint8_t     *pRxBuffer;
    /// Pointer to the TX buffer
    uint8_t     *pTxBuffer;
    /// Size of the RX buffer
    uint16_t    rxBufferSize;
    /// Size of the TX buffer
    uint16_t    txBufferSize;

    /// Pointer to the Request To Process function.
    EMTransportManager_RequestToProcessFct_t    pRequestToProcessFct;
    /// Pointer to the Send Event function.
    EMTransportManager_SendEventFct_t           pSendEventFct;
} EMTransportManager_Memory_t;


/******************************************************************************\
 *  External global variables
\******************************************************************************/

/// Global memory structure for the EM Transport Manager layer.
extern EMTransportManager_Memory_t  gEMTransportManagerMemory;


/******************************************************************************\
 *  Inline functions declarations
\******************************************************************************/

/**
 * @brief Send an event using the configured function.
 * @param eventCode Event code.
 * @param paramsLength Length of the event parameters.
 * @param pParams Pointer to the event parameters.
 * @param pCallback Pointer to a callback function that will be called at the end of the transaction (can be NULL).
 * @returns Status of the operation.
 */
static ALWAYS_INLINE bool EMTransportManager_SendEvent(
        uint8_t eventCode,
        uint16_t paramsLength,
        const uint8_t *pParams,
        EMTransportManager_TxCpltCallback_t pCallback)
{
    bool status = FALSE;

    if(gEMTransportManagerMemory.pSendEventFct != NULL)
    {
        status = (*gEMTransportManagerMemory.pSendEventFct)(eventCode, paramsLength, pParams, pCallback);
    }

    return status;
}


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Start the packet parser.
 */
void EMTransportManager_StartParser(void);

/**
 * @brief Determine if the EM Transport Manager is busy.
 * @return TRUE is the EM Transport Manager is busy, FALSE otherwise.
 */
bool EMTransportManager_IsBusy(void);

/**
 * @brief Process the pending internal tasks.
 * @param savedInterrupts IRQ threshold returned from the IRQ_DisableAndSaveInterrupts function.
 * @return IRQ threshold.
 * @note This function shall be called if the EMTransportManager_RequestToProcess has been called.
 * Shall NOT be called in an interrupt context.
 * This function must be called with interrupts disabled and return with interrupts disabled.
 */
uint32_t EMTransportManager_Process(uint32_t savedInterrupts);

/**
 * @brief Send a Command Complete event.
 * @param cmdOpcode Operation code of the command that triggered this event.
 * @param returnParamsLength Length of the return parameters.
 * @param maxParamsLength Maximum length of the event parameters.
 * @param pParams Pointer to the event parameters.
 * @param pCallback Pointer to a callback function that will be called at the end of the transaction (can be NULL).
 * @returns Status of the operation.
 */
bool EMTransportManager_SendCommandCompleteEvent(
        uint16_t cmdOpcode,
        uint16_t returnParamsLength,
        uint16_t maxParamsLength,
        uint8_t *pParams,
        EMTransportManager_TxCpltCallback_t pCallback);

/**
 * @brief Send a Command Status event.
 * @param cmdOpcode Operation code of the command that triggered this event.
 * @param status Status of the command.
 * @param maxParamsLength Maximum length of the event parameters.
 * @param pParams Pointer to the event parameters.
 * @param pCallback Pointer to a callback function that will be called at the end of the transaction (can be NULL).
 * @returns Status of the operation.
 */
bool EMTransportManager_SendCommandStatusEvent(
        uint16_t cmdOpcode,
        uint8_t status,
        uint16_t maxParamsLength,
        uint8_t *pParams,
        EMTransportManager_TxCpltCallback_t pCallback);

/**
 * @brief Send a Hardware Error event.
 * @param hardwareCode Harware Code that trigger this event.
 * @param maxParamsLength Maximum length of the event parameters.
 * @param pParams Pointer to the event parameters.
 * @param pCallback Pointer to a callback function that will be called at the end of the transaction (can be NULL).
 * @returns Status of the operation.
 */
bool EMTransportManager_SendHardwareErrorEvent(
        uint8_t hardwareCode,
        uint16_t maxParamsLength,
        uint8_t *pParams,
        EMTransportManager_TxCpltCallback_t pCallback);


#endif // __EM_TRANSPORT_MANAGER_H
