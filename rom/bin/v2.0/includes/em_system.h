////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_system/includes/em_system.h
///
/// @project    T9305
///
/// @brief      EM System Layer.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
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

#ifndef __EM_SYSTEM_H
#define __EM_SYSTEM_H

#include "types.h"
#include "macros.h"
#include "ECCTypedef.h"
#include "hw_aes.h"


/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * Authentication flag values.
 */
/// No authentication.
#define EMS_AUTH_FLAG_VALUE_NONE                            0x00000000UL
/// USER authentication.
#define EMS_AUTH_FLAG_VALUE_USER                            0x53535353UL
/// EM authentication.
#define EMS_AUTH_FLAG_VALUE_EM                              0xA9A9A9A9UL

/**
 * Key containers indexes.
 */
/// User authentication private key.
#define KEY_CONTAINER_INDEX_AUTH_USER                       0u

/**
 * HCI Commands
 */
/// HCI Command - HCI Reset
#ifndef HCI_OPCODE_RESET
#define HCI_OPCODE_RESET                                    0x0C03u
#endif
#define HCI_AUTHMIN_RESET                                   EMS_AUTH_FLAG_VALUE_NONE

/**
 * EMS Commands
 * Opcode: OGF (6bits) + OCF (10 bits)
 *         --> OGF = 0x3F (Vendor HCI Commands)
 *         --> OCF: EMOGF (4 bits) + EMOCF (6bits)
 */
/// EMS Command - General - Read Product Information
#define EMSG_OPCODE_READ_PRODUCT_INFORMATION                0xFC01u
#define EMSG_AUTHMIN_READ_PRODUCT_INFORMATION               EMS_AUTH_FLAG_VALUE_NONE

/// EMS Command - Alias - Set Public Address
#define EMS_OLD_OPCODE_SET_PUBLIC_ADDRESS                   0xFC02u // Alias of 0xFC43u
/// EMS Command - Alias - Set UART Baud Rate
#define EMS_OLD_OPCODE_SET_UART_BAUD_RATE                   0xFC07u // Alias of 0xFC44u
/// EMS Command - Alias - Transmitter Test
#define EMS_OLD_OPCODE_TRANSMITTER_TEST                     0xFC11u // Alias of 0xFCC1u
/// EMS Command - Alias - Transmitter Test End
#define EMS_OLD_OPCODE_TRANSMITTER_TEST_END                 0xFC12u // Alias of 0xFCC2u
/// EMS Command - Alias - Read At Address
#define EMS_OLD_OPCODE_READ_AT_ADDRESS                      0xFC20u // Alias of 0xFD01u
/// EMS Command - Alias - Read Continue
#define EMS_OLD_OPCODE_READ_CONTINUE                        0xFC21u // Alias of 0xFD02u
/// EMS Command - Alias - Write At Address
#define EMS_OLD_OPCODE_WRITE_AT_ADDRESS                     0xFC22u // Alias of 0xFD03u
/// EMS Command - Alias - Write Continue
#define EMS_OLD_OPCODE_WRITE_CONTINUE                       0xFC23u // Alias of 0xFD04u
/// EMS Command - Alias - Set Power Mode
#define EMS_OLD_OPCODE_SET_POWER_MODE                       0xFC24u // Alias of 0xFC48u
/// EMS Command - Alias - Set RF Activity
#define EMS_OLD_OPCODE_SET_RF_ACTIVITY                      0xFC25u // Alias of 0xFCC3u
/// EMS Command - Alias - Set RF Power
#define EMS_OLD_OPCODE_SET_RF_POWER                         0xFC26u // Alias of 0xFCC4u
/// EMS Command - Alias - Write Patch Start
#define EMS_OLD_OPCODE_WRITE_PATCH_START                    0xFC27u // Alias of 0xFD81u
/// EMS Command - Alias - Write Patch Continue
#define EMS_OLD_OPCODE_WRITE_PATCH_CONTINUE                 0xFC28u // Alias of 0xFD82u
/// EMS Command - Alias - Write Patch Abort
#define EMS_OLD_OPCODE_WRITE_PATCH_ABORT                    0xFC29u // Alias of 0xFD83u
/// EMS Command - Alias - Set Clock Source
#define EMS_OLD_OPCODE_SET_CLOCK_SOURCE                     0xFC2Au // Alias of 0xFC45u
/// EMS Command - Alias - Set Memory Mode
//#define EMS_OLD_OPCODE_SET_MEMORY_MODE                      0xFC2Bu // Not implemented in the 9305!
/// EMS Command - Alias - Get Memory Usage
#define EMS_OLD_OPCODE_GET_MEMORY_USAGE                     0xFC2Cu // Alias of 0xFC47u
/// EMS Command - Alias - Set Sleep Options
#define EMS_OLD_OPCODE_SET_SLEEP_OPTIONS                    0xFC2Du // Alias of 0xFC49u
/// EMS Command - Alias - SVLD Measurement
#define EMS_OLD_OPCODE_SVLD_MEASUREMENT                     0xFC2Eu // Alias of 0xFC4Au
/// EMS Command - Alias - Set Event Mask
//#define EMS_OLD_OPCODE_SET_EVENT_MASK                       0xFC2Fu // Not implemented in the 9305!
/// EMS Command - Alias - Public Reserved 1
//#define EMS_OLD_OPCODE_PUBLIC_RESERVED_1                    0xFC30u // Not implemented in the 9305!
/// EMS Command - Alias - Public Reserved 2
//#define EMS_OLD_OPCODE_PUBLIC_RESERVED_2                    0xFC31u // Not implemented in the 9305!
/// EMS Command - Alias - CPU Reset
#define EMS_OLD_OPCODE_CPU_RESET                            0xFC32u // Alias of 0xFC42u
/// EMS Command - Alias - Calculate CRC32
#define EMS_OLD_OPCODE_CALCULATE_CRC32                      0xFC33u // Alias of 0xFC4Eu
/// EMS Command - Alias - Patch Query
#define EMS_OLD_OPCODE_PATCH_QUERY                          0xFC34u // Alias of 0xFD84u

/// EMS Command - General - Read Supported Features
#define EMSG_OPCODE_READ_SUPPORTED_FEATURES                 0xFC41u
#define EMSG_AUTHMIN_READ_SUPPORTED_FEATURES                EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - CPU Reset
#define EMSG_OPCODE_CPU_RESET                               0xFC42u
#define EMSG_AUTHMIN_CPU_RESET                              EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Set Public Address
#define EMSG_OPCODE_SET_PUBLIC_ADDRESS                      0xFC43u
#define EMSG_AUTHMIN_SET_PUBLIC_ADDRESS                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Set UART Baud Rate
#define EMSG_OPCODE_SET_UART_BAUD_RATE                      0xFC44u
#define EMSG_AUTHMIN_SET_UART_BAUD_RATE                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Set Clock Source
#define EMSG_OPCODE_SET_CLOCK_SOURCE                        0xFC45u
#define EMSG_AUTHMIN_SET_CLOCK_SOURCE                       EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - General - Set HF Clock Frequency
#define EMSG_OPCODE_SET_HF_CLOCK_FREQUENCY                  0xFC46u
#define EMSG_AUTHMIN_SET_HF_CLOCK_FREQUENCY                 EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - General - Get Memory Usage
#define EMSG_OPCODE_GET_MEMORY_USAGE                        0xFC47u
#define EMSG_AUTHMIN_GET_MEMORY_USAGE                       EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - General - Set Power Mode
#define EMSG_OPCODE_SET_POWER_MODE                          0xFC48u
#define EMSG_AUTHMIN_SET_POWER_MODE                         EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - General - Set Sleep Options
#define EMSG_OPCODE_SET_SLEEP_OPTIONS                       0xFC49u
#define EMSG_AUTHMIN_SET_SLEEP_OPTIONS                      EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - SVLD Measurement
#define EMSG_OPCODE_SVLD_MEASUREMENT                        0xFC4Au
#define EMSG_AUTHMIN_SVLD_MEASUREMENT                       EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Execute JLI Function
#define EMSG_OPCODE_EXECUTE_JLI_FUNCTION                    0xFC4Bu
#define EMSG_AUTHMIN_EXECUTE_JLI_FUNCTION                   EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - General - Execute Function
#define EMSG_OPCODE_EXECUTE_FUNCTION                        0xFC4Cu
#define EMSG_AUTHMIN_EXECUTE_FUNCTION                       EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - General - Jump To Function
#define EMSG_OPCODE_JUMP_TO_FUNCTION                        0xFC4Du
#define EMSG_AUTHMIN_JUMP_TO_FUNCTION                       EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - General - Calculate CRC32
#define EMSG_OPCODE_CALCULATE_CRC32                         0xFC4Eu
#define EMSG_AUTHMIN_CALCULATE_CRC32                        EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - General - Enter Configuration Mode
#define EMSG_OPCODE_ENTER_CONFIGURATION_MODE                0xFC4Fu
#define EMSG_AUTHMIN_ENTER_CONFIGURATION_MODE               EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Leave Configuration Mode
#define EMSG_OPCODE_LEAVE_CONFIGURATION_MODE                0xFC50u
#define EMSG_AUTHMIN_LEAVE_CONFIGURATION_MODE               EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Call weak function
#define EMSG_OPCODE_CALL_WEAK_FUNCTION                      0xFC51u
#define EMSG_AUTHMIN_CALL_WEAK_FUNCTION                     EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - General - Read MAC Address
#define EMSG_OPCODE_READ_MAC_ADDRESS                        0xFC52u
#define EMSG_AUTHMIN_READ_MAC_ADDRESS                       EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Get emcore information
#define EMSG_OPCODE_GET_EMCORE_INFORMATION                  0xFC53u
#define EMSG_AUTHMIN_GET_EMCORE_INFORMATION                 EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Set Boot Mode Flags
#define EMSG_OPCODE_SET_BOOT_MODE_FLAGS                     0xFC54u
#define EMSG_AUTHMIN_SET_BOOT_MODE_FLAGS                    EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - General - Clear Boot Mode Flags
#define EMSG_OPCODE_CLEAR_BOOT_MODE_FLAGS                   0xFC55u
#define EMSG_AUTHMIN_CLEAR_BOOT_MODE_FLAGS                  EMS_AUTH_FLAG_VALUE_NONE

/// EMS Command - Security - EM Get Challenge
#define EMSS_OPCODE_EM_GET_CHALLENGE                        0xFC81u
#define EMSS_AUTHMIN_EM_GET_CHALLENGE                       EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - EM Authenticate
#define EMSS_OPCODE_EM_AUTHENTICATE                         0xFC82u
#define EMSS_AUTHMIN_EM_AUTHENTICATE                        EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - USER Write Private Key
#define EMSS_OPCODE_USER_WRITE_PRIVATE_KEY                  0xFC83u
#define EMSS_AUTHMIN_USER_WRITE_PRIVATE_KEY                 EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Security - USER Get Challenge
#define EMSS_OPCODE_USER_GET_CHALLENGE                      0xFC84u
#define EMSS_AUTHMIN_USER_GET_CHALLENGE                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - USER Authenticate
#define EMSS_OPCODE_USER_AUTHENTICATE                       0xFC85u
#define EMSS_AUTHMIN_USER_AUTHENTICATE                      EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - EM Read Public Key
#define EMSS_OPCODE_EM_READ_PUBLIC_KEY                      0xFC86u
#define EMSS_AUTHMIN_EM_READ_PUBLIC_KEY                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - LE Rand
#define EMSS_OPCODE_LE_RAND                                 0xFC87u
#define EMSS_AUTHMIN_LE_RAND                                EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Security - LE Encrypt
#define EMSS_OPCODE_LE_ENCRYPT                              0xFC88u
#define EMSS_AUTHMIN_LE_ENCRYPT                             EMS_AUTH_FLAG_VALUE_NONE

/// EMS Command - Radio Control - Transmitter Test
#define EMSRC_OPCODE_TRANSMITTER_TEST                       0xFCC1u
#define EMSRC_AUTHMIN_TRANSMITTER_TEST                      EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Transmitter Test End
#define EMSRC_OPCODE_TRANSMITTER_TEST_END                   0xFCC2u
#define EMSRC_AUTHMIN_TRANSMITTER_TEST_END                  EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Set RF Activity
#define EMSRC_OPCODE_SET_RF_ACTIVITY                        0xFCC3u
#define EMSRC_AUTHMIN_SET_RF_ACTIVITY                       EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Set RF Power
#define EMSRC_OPCODE_SET_RF_POWER                           0xFCC4u
#define EMSRC_AUTHMIN_SET_RF_POWER                          EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Receiver Test
#define EMSRC_OPCODE_RECEIVER_TEST                          0xFCC5u
#define EMSRC_AUTHMIN_RECEIVER_TEST                         EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Receiver Test End
#define EMSRC_OPCODE_RECEIVER_TEST_END                      0xFCC6u
#define EMSRC_AUTHMIN_RECEIVER_TEST_END                     EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Received Test End Debug
#define EMSRC_OPCODE_RECEIVER_TEST_END_DEBUG                0xFCC7u
#define EMSRC_AUTHMIN_RECEIVER_TEST_END_DEBUG               EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Set RSSI Offset
#define EMSRC_OPCODE_SET_RSSI_OFFSET                         0xFCC8u
#define EMSRC_AUTHMIN_SET_RSSI_OFFSET                        EMS_AUTH_FLAG_VALUE_NONE
/// EMS Command - Radio Control - Start advertising pattern
#define EMSRC_OPCODE_START_ADV_PATTERN                       0xFCC9u
#define EMSRC_AUTHMIN_START_ADV_PATTERN                      EMS_AUTH_FLAG_VALUE_NONE


/// EMS Command - Memory Management - Read At Address
#define EMSMM_OPCODE_READ_AT_ADDRESS                        0xFD01u
#define EMSMM_AUTHMIN_READ_AT_ADDRESS                       EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - Read Continue
#define EMSMM_OPCODE_READ_CONTINUE                          0xFD02u
#define EMSMM_AUTHMIN_READ_CONTINUE                         EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - Write At Address
#define EMSMM_OPCODE_WRITE_AT_ADDRESS                       0xFD03u
#define EMSMM_AUTHMIN_WRITE_AT_ADDRESS                      EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - Write Continue
#define EMSMM_OPCODE_WRITE_CONTINUE                         0xFD04u
#define EMSMM_AUTHMIN_WRITE_CONTINUE                        EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - NVM Erase Full
#define EMSMM_OPCODE_NVM_ERASE_FULL                         0xFD05u
#define EMSMM_AUTHMIN_NVM_ERASE_FULL                        EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - Memory Management - NVM Erase Main
#define EMSMM_OPCODE_NVM_ERASE_MAIN                         0xFD06u
#define EMSMM_AUTHMIN_NVM_ERASE_MAIN                        EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - NVM Erase Page
#define EMSMM_OPCODE_NVM_ERASE_PAGE                         0xFD07u
#define EMSMM_AUTHMIN_NVM_ERASE_PAGE                        EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - NVM Get Lock Page
#define EMSMM_OPCODE_NVM_GET_LOCK_PAGE                      0xFD08u
#define EMSMM_AUTHMIN_NVM_GET_LOCK_PAGE                     EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - NVM Lock Page
#define EMSMM_OPCODE_NVM_LOCK_PAGE                          0xFD09u
#define EMSMM_AUTHMIN_NVM_LOCK_PAGE                         EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Memory Management - NVM Power Control
#define EMSMM_OPCODE_NVM_POWER_CONTROL                      0xFD0Au
#define EMSMM_AUTHMIN_NVM_POWER_CONTROL                     EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - Memory Management - Write At Address Without Response
#define EMSMM_OPCODE_WRITE_AT_ADDRESS_WITHOUT_RESPONSE      0xFD0Bu
#define EMSMM_AUTHMIN_WRITE_AT_ADDRESS_WITHOUT_RESPONSE     EMSMM_AUTHMIN_WRITE_AT_ADDRESS
/// EMS Command - Memory Management - Write Continue Without Response
#define EMSMM_OPCODE_WRITE_CONTINUE_WITHOUT_RESPONSE        0xFD0Cu
#define EMSMM_AUTHMIN_WRITE_CONTINUE_WITHOUT_RESPONSE       EMSMM_AUTHMIN_WRITE_CONTINUE

/// EMS Command - Register Management - Write AUX Register
#define EMSRM_OPCODE_WRITE_AUX_REGISTER                     0xFD41u
#define EMSRM_AUTHMIN_WRITE_AUX_REGISTER                    EMS_AUTH_FLAG_VALUE_EM
/// EMS Command - Register Management - Read AUX Register
#define EMSRM_OPCODE_READ_AUX_REGISTER                      0xFD42u
#define EMSRM_AUTHMIN_READ_AUX_REGISTER                     EMS_AUTH_FLAG_VALUE_EM

/// EMS Command - Patch Management - Write Patch Start
#define EMSPM_OPCODE_WRITE_PATCH_START                      0xFD81u
#define EMSPM_AUTHMIN_WRITE_PATCH_START                     EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Patch Management - Write Patch Continue
#define EMSPM_OPCODE_WRITE_PATCH_CONTINUE                   0xFD82u
#define EMSPM_AUTHMIN_WRITE_PATCH_CONTINUE                  EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Patch Management - Write Patch Abort
#define EMSPM_OPCODE_WRITE_PATCH_ABORT                      0xFD83u
#define EMSPM_AUTHMIN_WRITE_PATCH_ABORT                     EMS_AUTH_FLAG_VALUE_USER
/// EMS Command - Patch Management - Patch Query
#define EMSPM_OPCODE_PATCH_QUERY                            0xFD84u
#define EMSPM_AUTHMIN_PATCH_QUERY                           EMS_AUTH_FLAG_VALUE_USER


/// First EMS old opcode
#define EMS_OLD_OPCODE__FIRST       EMS_OLD_OPCODE_SET_PUBLIC_ADDRESS
/// Last EMS old opcode
#define EMS_OLD_OPCODE__LAST        EMS_OLD_OPCODE_PATCH_QUERY


/******************************************************************************\
 *  Types
\******************************************************************************/

/**
 * @brief Type for a function pointer to a command parser.
 * @param opcode Operation code of the command that trigger this parser.
 * @param cmdParamsLength Length of the command parameters.
 * @param pCmdParams Pointer to the command parameters.
 * @param maxEventParamsLength Maximum length of the event parameters.
 * @param pEventParams Pointer to a buffer for the event parameters.
 */
typedef void (*EMSystem_CommandParserFunction_t)(const uint16_t opcode, \
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams, \
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

/**
 * @brief Command parser structure type.
 */
typedef struct
{
    /// Pointer to the command parser function
    EMSystem_CommandParserFunction_t function;
    /// Minimum required authentication (NONE, USER, or EM)
    uint32_t authMin;
    /// Opcode for this command parser
    uint16_t opcode;
    /// Reserved for future use
    uint16_t reserved;
} EMSystem_CommandParser_t;

/**
 * @brief Commands handler structure type.
 */
typedef struct _EMSystem_CommandsHandler_t
{
    /// Pointer to a sorted array of command parsers.
    const EMSystem_CommandParser_t *pCommandParsers;

    /// Number of command parsers in the given array.
    uint16_t numberOfCommandParsers;

    /// Pointer to the next commands handler.
    struct _EMSystem_CommandsHandler_t *pNext;
} EMSystem_CommandsHandler_t;

/**
 * @brief Security context structure type.
 */
typedef struct
{
    /// EM or USER Authentication union
    union
    {
        /// EM Authentication
        struct
        {
            /// Challenge
            uint32_t challenge[SCALAR_SIZE_P256];

            /// Commitment
            POINT commitment;
        } em;

        /// USER Authentication
        struct
        {
            /// Challenge
            uint32_t challenge[AES_BLOCK_WORD_SIZE];
        } user;
    };

    /// Flag indicating if the security info are valid and for whom (EM or USER).
    uint32_t dataValidFlag;
} EMSystem_SecurityCtx_t;

/**
 * @brief EM System persistent memory structure.
 */
typedef struct
{
    /// Pointer to the first item of the commands handlers linked list.
    EMSystem_CommandsHandler_t *pFirstCommandsHandler;

    /// Security context.
    EMSystem_SecurityCtx_t securityCtx;

    /// Indicates the current system authentication.
    uint32_t authFlag;

    /// Address after the last byte that was read by the last
    /// EMSMM_ReadAtAddress or EMSMM_ReadContinue function that executed.
    Pointer_t readAddress;

    /// Address after the last byte that was written by the last
    /// EMSMM_WriteAtAddress or EMSMM_WriteContinue function that executed.
    Pointer_t writeAddress;
} EMSystem_PRAM_t;

/**
 * @brief EM System non-persistent memory structure.
 */
typedef struct
{
    /// Execute function commands parameters.
    struct
    {
        uint32_t address;
        uint32_t arguments[4u];
        uint8_t numberOfArguments;
        uint16_t emsCmdOpcode;
    } executeFunctionParams;

    /// Sleep Mode Request.
    uint8_t sleepModeRequest;

    /// Set Power Mode command paramater.
    uint8_t newPowerMode;
} EMSystem_NPRAM_t;


/******************************************************************************\
 *  External global variables
\******************************************************************************/

/// Pointer to the persistent memory structure or the EM System Layer.
extern EMSystem_PRAM_t *gpEMSystemPRAM;
/// Pointer to the non-persistent memory structure of the EM System Layer.
extern EMSystem_NPRAM_t *gpEMSystemNPRAM;
/// Pseudo DTM running flag
extern bool gPseudoDTMRunning;
/// RF
extern volatile bool gPseudoDTMPacketReceived;
extern volatile bool gPseudoDTMRxCrcError;
extern volatile uint32_t gPseudoDTMRxCnt;
extern volatile uint32_t gPseudoDTMTxCnt;
extern volatile uint8_t gPseudoDTMRxDone;
extern volatile uint8_t gPseudoDTMTxDone;


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the EM System layer.
 * @return Initialization status.
 */
bool EMSystem_Init(void);

/**
 * @brief Resume the EM System layer.
 */
void EMSystem_Resume(void);

/**
 * @brief Register a commands handler.
 * @param pCommandParsers Pointer to a sorted array of command parsers.
 * @param numberOfCommandParsers Number of command parsers in the given array.
 * @return Status of the operation.
 */
bool EMSystem_RegisterCommandsHandler(
    const EMSystem_CommandParser_t *pCommandParsers,
    uint16_t numberOfCommandParsers);

/**
 * @brief Get the sleep mode request.
 * @return Sleep mode request.
 */
uint8_t EMSystem_GetSleepModeRequest(void);

/**
 * @brief Clear the sleep mode request.
 */
void EMSystem_ClearSleepModeRequest(void);

/**
* @brief start transmitting a continuously modulated signal given the input
* parameters
* @param transmitPowerLevel: power level in dBm to transmit
* @param transmitterTestMode: which type of continuous modulation to use
* @param channel: RF channel to use
* @param phy: which physical layer to use
* @param pEventParams: return event parameters of the HCI command
* @param pReturnParamsLength: Length of HCI command return parameters
**/
uint8_t EMSystem_TransmitCM(uint8_t transmitPowerLevel,
                            uint8_t transmitterTestMode,
                            uint8_t channel, uint8_t phy,
                            uint8_t *pEventParams,
                            uint8_t *pReturnParamsLength);


/**
 * @brief noop version of this function if em_system_stack is not a linked lib
 * for the application. This removes the QPC dependency from em_system.
 * **/
__attribute__((weak)) uint8_t
EMSystemStack_TransmitStart(uint8_t transmitPowerLevel,
                            uint8_t packetPayload,
                            uint8_t testDataLength, uint8_t phy,
                            uint8_t channel, uint8_t *pEventParams,
                            uint8_t *pReturnParamsLength);

/**
 * @brief noop version of this function if em_system_stack is not a linked lib
 * for the application. This removes the QPC dependency from em_system.
 * **/
__attribute__((weak)) uint8_t EMSystemStack_TransmitStop(void);

/**
 * @brief noop version of this function if em_system_stack is not a linked lib
 * for the application. This removes the QPC dependency from em_system.
 * **/
__attribute__((weak)) uint8_t
EMSystemStack_ReceiveStart(uint8_t channel, uint8_t phy, uint32_t syncword);

/**
 * @brief noop version of this function if em_system_stack is not a linked lib
 * for the application. This removes the QPC dependency from em_system.
 * **/
__attribute__((weak)) uint8_t EMSystemStack_ReceiveStop(int16_t *pRssiAvg);

/**
 * @brief noop version of this function if em_system_stack is not a linked lib
 * for the application. This removes the QPC dependency from em_system.
 * **/
__attribute__((weak)) uint8_t EMSystemStack_StartAdvertising(void);

/******************************************************************************\
 *  Inline functions declarations
\******************************************************************************/

/**
 * @brief Check is the security is sufficient to execute a command.
 * @param cmdAuthMin Minimum required authentication of the command.
 * @return TRUE is the security is sufficient, FALSE otherwise.
 * @note One of the following conditions shall be valid to be allowed to execute a command:
 *     (1) No authentication required for the command.
 *     (2) EM is authenticated.
 *     (3) Required authentication and system authentication match.
 */
static ALWAYS_INLINE bool EMSystem_IsSecuritySufficient(uint32_t cmdAuthMin)
{
    return (( EMS_AUTH_FLAG_VALUE_NONE == cmdAuthMin               ) || // (1)
            ( EMS_AUTH_FLAG_VALUE_EM   == gpEMSystemPRAM->authFlag ) || // (2)
            ( cmdAuthMin               == gpEMSystemPRAM->authFlag ));  // (3)
}


void EMSRC_EmCoreTransmitterTest(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSRC_EmCoreTransmitterTestEnd(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSRC_EmCoreReceiverTest(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSRC_EmCoreReceiverTestEnd(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSRC_EmCoreStartAdvPattern(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSG_GetEmCoreInformation(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

// TODO: all function prototypes need to be put here because defining them in
// em_system_cmd.c's is not MISRA compliant.
void EMSG_CpuReset(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSG_SetBootModeFlags(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSG_ClearBootModeFlags(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSS_LeEncrypt(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

void EMSS_LeRand(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams);

#endif // __EM_SYSTEM_H
