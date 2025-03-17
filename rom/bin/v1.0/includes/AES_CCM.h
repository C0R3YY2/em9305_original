////////////////////////////////////////////////////////////////////////////////
///
/// @file       AES_CCM.h
///
/// @project    T9304
///
/// @author     SAS
///
/// @brief      AES CCM authentication and encryption functions
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2015 EM Microelectronic
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


#ifndef _AESCCM_H
#define _AESCCM_H


/**
 * @defgroup AES_CCM AES_CCM
 */

//--------------------------------------------------------------------------------------------
//Exported functions
//--------------------------------------------------------------------------------------------

/**
 * \fn u8 AES_CCM_GENERATION_ENCRYPTION_FUNC  (u8 *MK,u8 LenMAC, u8 LenNonce ,u8 * Nonce,  u8 LenA[8], u8 * A, u8 LenP[8], u8* P, u8*CipherMAC)
 * \brief  Computes the CCM MAC of a message and encrypts it
 * \param[in] MK A 16 bytes key
 * \param[in] LenMAC Length of the MAC in bytes
 * \param[in] LenNonce Length of the Nonce in bytes
 * \param[in] Nonce the nonce, a buffer of LenNonce bytes
 * \param[in] LenA a 8-byte buffer indicating the length in bytes of the associated data
 * \param[in] A a buffer of LenA bytes containing the associated data
 * \param[in] LenP a 8-byte buffer indicating the length of the message to encrypt
 * \param[in] P  a buffer of LenP bytes,the message to encrypt
 * \param[out] CipherMAC a buffer of LenP+LenMAC bytes containing the encrypted message concatenated with its MAC
 * \return AES_CCM_OK: OK, the message was encrypted and the MAC computed
 * \return AES_CCM_NOK: NOK, the provided lengths are not correct
 * \note
 *
 * LenMAC should be among the set {4, 6, 8, 10, 12, 14, 16}\n
 * LenNonce should be among the set {7, 8, 9, 10, 11, 12, 13}\n
 * P should be strictly smaller than 2^(15-LenNonce), i.e , the significant of LenP should be smaller or equal to 15-Nonce bytes\n
 * Remarks: Possibly LenA(Respectively LenP) can be null.\n
 * In this case LenA(respectively LenP) should be  {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}\n
 *  \ingroup AES_CCM
 */
extern uint8_t AES_CCM_GENERATION_ENCRYPTION  (uint8_t *MK,uint8_t LenMAC, uint8_t LenNonce ,uint8_t * Nonce,  uint8_t LenA[8], uint8_t * A, uint8_t PLen[8], uint8_t* P, uint8_t*CipherMAC);
/**
 * \fn u8 AES_CCM_DECRYPTION_VERIFICATION_FUNC(u8 *MK,u8 LenMAC, u8 LenNonce ,u8 * Nonce,  u8 LenA[8], u8 * A, u8 LenP[8], u8* P, u8*Decrypted);
 * \brief Verifies the CCM MAC of a message and decrypts it

 * \param[in] MK A 16-byte key
 * \param[in] LenMAC Length of the MAC in bytes
 * \param[in] LenNonce Length of the Nonce in bytes
 * \param[in] Nonce The nonce, a buffer of LenNonce bytes
 * \param[in] LenA  A 8-byte buffer indicating the length in bytes of the associated data
 * \param[in] A  A buffer of LenA bytes containing the associated data
 * \param[in] LenP A 8-byte buffer indicating the length of the message to decrypt and verify the ;AC
 * \param[in] P A buffer of LenP bytes,the message to decrypt and verify
 * \param[out] Decrypted A buffer of LenP-LenMAC bytes containing the decrypted message
 * \return AES_CCM_VALID    : OK, the message was decrypted  and the submitted MAC is valid
 * \return AES_CCM_INVALID  : NOK, the MAC is not valid
 * \return AES_CCM_NOK  : NOK , the provided lengths are not correct
 * \note
 * LenMAC should be among the set {4, 6, 8, 10, 12, 14, 16}\n
 * LenNonce should be among the set {7, 8, 9, 10, 11, 12, 13}\n
 * LenP should be bigger or equal to LenMAC\n
 * P should be strictly smaller than 2^(15-LenNonce)-2^LenMAC\n
 * Remarks: Possibly LenA(Respectively LenP) can be null\n
 * In this case the A(respectively P) should be  {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}\n
 * \ingroup AES_CCM
 */

extern uint8_t AES_CCM_DECRYPTION_VERIFICATION(uint8_t *MK,uint8_t LenMAC, uint8_t LenNonce ,uint8_t * Nonce,  uint8_t LenA[8], uint8_t * A, uint8_t LenP[8], uint8_t* P, uint8_t*Decrypted);

//--------------------------------------------------------------------------------------------
//MACROS
//--------------------------------------------------------------------------------------------

/** Status word OK
* \ingroup AES_CCM
*/
#define AES_CCM_OK      0           //OK
/** Status word NOK
* \ingroup AES_CCM
*/
#define AES_CCM_NOK     1           //Parameters NOK
/** Status word Valid
* \ingroup AES_CCM
*/
#define AES_CCM_VALID   2           //OK Verification of MAC is successful
/** Status word Invalid
* \ingroup AES_CCM
*/
#define AES_CCM_INVALID 3           //NOK, the MAC is not valid

#endif
