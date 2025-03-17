////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/security/includes/hw_aes.h
///
/// @project    T9305
///
/// @brief      HW AES and AES key containers driver.
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

#ifndef _HW_AES_H
#define _HW_AES_H

#include <types.h>


/**
 * @defgroup aes AES Driver
 * @ingroup drivers
 * @brief AES AHB drivers and AES key containers drivers
 *
 */

/******************************************************************************\
* Hardware AES registers macros
\******************************************************************************/
//Modes
#define HW_AES_DECRYPTION           (0)
#define HW_AES_ENCRYPTION           (1)

//key source
#define HW_AES_KEY_SOURCE_KEY_CONTAINER     (0)
#define HW_AES_KEY_SOURCE_AHB               (1)

//Key index
#define HW_AES_DEFAULT_KEY_INDEX    (0)

#define HW_AES_START                (1)
#define HW_AES_STOP                 (0)

//Load key
#define HW_AES_LOAD_KEY_START       (1)
#define HW_AES_LOAD_KEY_DONE        (0)


/******************************************************************************\
* DEFINES
\******************************************************************************/

/**
 * @brief Number of 32-bit words in a 128-bit AES block.
 * @ingroup aes
 */
#define AES_BLOCK_WORD_SIZE (4)

/**
 * @brief Number of 8-bit words in a 128-bit AES block.
 * @ingroup aes
 */
#define AES_BLOCK_BYTE_SIZE (16)


/******************************************************************************\
* TYPE DEFINES
\******************************************************************************/

/**
 * @brief AES modes of operation.
 * @ingroup aes
 */
typedef enum
{
    /// Encrypt the given data.
    AES_MODE_ENCRYPT = 0,
    /// Decrypt the given data.
    AES_MODE_DECRYPT
} AES_Mode_t;


/**
 * @brief AES Error status words
 * @ingroup aes
 */
typedef enum
{
    ///No Error- Sucesss
    AES_STATUS_SUCCESS=0,
    ///Unknown error
    AES_STATUS_ERROR=1,
    ///Incorrect conditions. Incorrect parameters
    AES_STATUS_WRONG_PARAM=2,
    ///Error writing in nvm. Key or the page may be locked.
    AES_STATUS_NVM_ERROR=3,
    ///Key is not lockable. Only key ID in  [0..7] can be locked.
    AES_STATUS_NOT_LOCKABLE=4,
    ///Key is locked.
    AES_STATUS_KEY_LOCKED=5

} AES_Error_t;


/**
 * @brief AES key container types(enc/dec,enc,dec)
 * @ingroup aes
 */
typedef enum
{
    ///Key type is encryption and decryption
    AES_HW_ENC_DEC = 0,
    ///Key type is only decryption
    AES_HW_DEC_ONLY= 1,
    ///Key type is only encrytion
    AES_HW_ENC_ONLY=2

} AES_KeyContainerType_t;


/**
 * @brief AES key container validity
 * @ingroup aes
 */
typedef enum
{
    ///Key is invalid
    AES_HW_KEY_INVALID = 0x00000000,
    ///Key is valid
    AES_HW_KEY_VALID= 0xFFFFFFFF

} AES_KeyContainerValidity_t;


/**
 * @brief Set the key to be used by AES for encryption and decryption.
 * @param key 128-bit AES key for encryption and decryption.
 * @param mode Mode of operation (encrypt or decrypt).
 * @return status of the operation
 * @note This key should be aligned.
 * @note For user code, the mutex must be locked before using the AES
 *   block. Failure to do so could corrupt the result if the firmware
 *   interrupted the user application and used the AES block.
 * @ingroup aes
 */
AES_Error_t AES_SetKey(const uint32_t key[AES_BLOCK_WORD_SIZE], AES_Mode_t mode);

/**
 * @brief Clear the AES key stores in the AES hardware.
 * @note For user code, the mutex must be locked before using the AES
 *   block. Failure to do so could corrupt the result if the firmware
 *   interrupted the user application and used the AES block.
 * @ingroup aes
 */
AES_Error_t AES_ClearKey(void);

/**
 * @brief Encrypt or decrypt a block of AES data with the explicit key provided
 * by AES_SetKey function
 * @param pInData Data to encrypt or decrypt.
 * @param pOutData Result of encryption or decryption operation.
 * @param mode Mode of operation (encrypt or decrypt).
 * @return status of the operation
 * @note For user code, the mutex must be locked before using the AES
 *   block. Failure to do so could corrupt the result if the firmware
 *   interrupted the user application and used the AES block.
 * @ingroup aes
 */
AES_Error_t AES_ProcessBlock(const void *pInData, void *pOutData, AES_Mode_t mode);

/**
 * @brief Set a 128-bit AES key in a key container
 * @param key 16-byte key to set
 * @param keyIndex keyID to store the key [0..255]
 * @param keyType Type of the key. Encryption/decryption, encryption only, or
 * decryption only
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_SetKeyContainer(const uint32_t key[AES_BLOCK_WORD_SIZE],
    uint8_t keyIndex, AES_KeyContainerType_t keyType);

/**
 * @brief Invalidate a key container
 * @param keyIndex keyID of the key to invalidate [0..255]
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_InvalidateKeyContainer(uint8_t keyIndex);

/**
 * @brief Execute AES-128 with a implicit key given by its ID
 * @param keyIndex keyID of the key to use
 * @param pInData 16-byte data to encrypt or decrypt
 * @param pOutData 16-byte result data
 * @param mode Encryption or decryption mode
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_ProcessBlockKeyContainer(uint8_t keyIndex,const void *pInData,
    void *pOutData, AES_Mode_t mode);

/**
 * @brief Write lock of a given key container
 * @param keyIndex keyID of the key to lock
 * @ingroup aes
 */
AES_Error_t AES_LockKeyContainer(uint8_t keyIndex);

/**
 * @brief Lock the key containers page(Info page) 0 against erase
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_KeyContainersEraseLock(void);

/**
 * @brief Lock the key containers page(Info page) 0 against write
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_KeyContainersWriteLock(void);

/**
 * @brief Lock the key containers page(Info page) 0 against write and erase
 * @return status of the operation
 * @ingroup aes
 */
AES_Error_t AES_KeyContainersLockPage(void);


#endif /* _HW_AES_H */
