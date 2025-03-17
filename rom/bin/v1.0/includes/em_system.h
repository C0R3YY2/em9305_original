////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_system/includes/em_system.h
///
/// @project    T9305
///
/// @brief      EM System Layer.
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
#define EMS_AUTH_FLAG_VALUE_NONE                            0x00000000ul
/// USER authentication.
#define EMS_AUTH_FLAG_VALUE_USER                            0x53535353ul
/// EM authentication.
#define EMS_AUTH_FLAG_VALUE_EM                              0xA9A9A9A9ul

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
#define EMSG_AUTHMIN_GET_MEMORY_USAGE                       EMS_AUTH_FLAG_VALUE_NONE
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
    uint32_t    authMin;
    /// Opcode for this command parser
    uint16_t    opcode;
    /// Reserved for future use
    uint16_t    reserved;
} EMSystem_CommandParser_t;

/**
 * @brief Commands handler structure type.
 */
typedef struct _EMSystem_CommandsHandler_t
{
    /// Pointer to a sorted array of command parsers.
    const EMSystem_CommandParser_t      *pCommandParsers;

    /// Number of command parsers in the given array.
    uint16_t                            numberOfCommandParsers;

    /// Pointer to the next commands handler.
    struct _EMSystem_CommandsHandler_t  *pNext;
} EMSystem_CommandsHandler_t;

/**
 * @brief Security context structure type.
 */
typedef struct
{
    union
    {
        /// EM Authentication
        struct
        {
            /// Challenge
            uint32_t    challenge[SCALAR_SIZE_P256];

            /// Commitment
            POINT       commitment;
        } em;

        /// USER Authentication
        struct
        {
            /// Challenge
            uint32_t    challenge[AES_BLOCK_WORD_SIZE];
        } user;
    };

    /// Flag indicating if the security info are valid and for whom (EM or USER).
    uint32_t dataValidFlag;
} EMSystem_SecurityCtx_t;

/// EM System memory structure.
typedef struct
{
    /// Pointer to the first item of the commands handlers linked list.
    EMSystem_CommandsHandler_t  *pFirstCommandsHandler;

    /// Security context.
    EMSystem_SecurityCtx_t      securityCtx;

    /// Indicates the current system authentication.
    uint32_t                    authFlag;
} EMSystem_Memory_t;


/******************************************************************************\
 *  External global variables
\******************************************************************************/

/// Global memory structure for the EM System layer.
extern EMSystem_Memory_t    gEMSystemMemory;


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Register a commands handler.
 * @param pCommandsHandler Pointer to the commands handler to be registered.
 */
void EMSystem_RegisterCommandsHandler(EMSystem_CommandsHandler_t *pCommandsHandler);


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
            ( EMS_AUTH_FLAG_VALUE_EM   == gEMSystemMemory.authFlag ) || // (2)
            ( cmdAuthMin               == gEMSystemMemory.authFlag ));  // (3)
}


#endif // __EM_SYSTEM_H
