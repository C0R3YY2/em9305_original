////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_header/includes/header_access.h
///
/// @project    T9305
///
/// @brief      Access firmware headers in NVM
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

#ifndef __HEADER_ACCESS_H
#define __HEADER_ACCESS_H

#include "types.h"

/**
 * @addtogroup firmware_headers
 * @{
 * @defgroup firmware_header Firmware_Header firmware_headers
 * @brief Toolkit function to manipulate firmware headers.
 *
 * @{
*/



/******************************************************************************\
 *  Symbolic constants
\******************************************************************************/
/**
 * NVM start address definition.
 */
#define NVM_START_ADDR  0x300000U

/**
 * NVM end address definition.
 */
#define NVM_END_ADDR    NVM_START_ADDR + (NVM_PAGE_COUNT_MAIN * NVM_PAGE_SIZE_BYTE)

/**
 * Error code definition for the getEmHdr_FindHdr() if it reaches the end of the NVM
 * without having found any valid header.
 */
#define HEADER_ADDRESS_ERROR    0xFFFFFFFFU

/**
 * Error code returned when a requested CRC check on a firmware image is successful.
 */
#define CRC_PASSED  0U

/**
 * Error code returned when a requested CRC check on a firmware image fails.
 */
#define CRC_FAILURE 1U

/******************************************************************************\
 *  Public Function Prototypes
\******************************************************************************/
/**
 * @brief This function returns the highest version of the firmware header that
 * this code supports.
 * If a new firmware header's version number is greater than the
 * returned SupportedHdrVersion then incompatibility issues may occur.  See EM
 * Support for compatibility.
 * @retval version highest supported version
 */
uint8_t getEmHdr_SupportedVersion(void);

/**
 * @brief Find the first valid header.
 * This function scans the NVM starting from the provided address until it finds
 * a valid firmware image header or if the end of the NVM it reached.
 * It stops and returns as soon as one of these two conditions is met.
 * The returned address is the actual start address of the header for this
 * firmware block, or 0xFFFFFFFF (not found indicator) when the end of
 * the NVM has been reached without finding any valid header.
 * @param searchForHeader this is the address from which the function will start
 * searching for valid header in the NVM.
 * @retval address the address of the first valid header found, otherwise returns
 * 0xFFFFFFFF in case the end of the NVM has been reached without having found any
 * valid header.
*/
uint32_t getEmHdr_FindHdr( uint32_t searchForHeader );

/**
 * @brief This function returns the size of the header found at the specified address,
 * knowing this size depends on the header version.
 * When calculating the full size of this complete firmware block (header + image),
 * the firmware header size must be added to the firmware image size.
 * @param headerAddress this is the address of the beginning of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @retval size the size of the header found at specified address.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint8_t getEmHdr_HdrVersionSize(uint32_t headerAddress);

/**
 * @brief This function returns the version of the header. It can be used to verify
 * that the found header is known and is supported by the code which attempt to manipulate it (see
 * getEmHdr_SupportedVersion).
 * @param headerAddress this is the address of the start of the firmware header (obtained by
 * calling getEmHdr_FindHdr).
 * @retval version the version of the header
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint8_t getEmHdr_HdrVersion( uint32_t headerAddress );

/**
 * @brief This function reports the firmware block type.
 * If returned block type is not the expected one, then a new search shall be
 * started again with the header address + 4 as parameter.
 * The headerAddress and sectionCode should be kept by the user for quick look-up.
 * @param headerAddress this is the address of the start of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @retval section_code the section code value (1: EM-Core app, 3: user app, 4: user bootloader,
 * other values: reserved for future use)
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint8_t getEmHdr_SectionCode( uint32_t headerAddress );

/**
 * @brief This function returns the firmwareVersion number (set by the user).  This can be
 * used to verify if a new firmware block (e.g. customer's application) is the
 * same as an existing firmware block.
 * @param headerAddress this is the address of the start of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @retval version the version of the firmware image identified by the header
 * found at the specified address.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint16_t getEmHdr_FirmwareVersion( uint32_t headerAddress );

/**
 * @brief This function does a CRC check of the firmware image attached to the found header.
 * @param headerAddress this is the address of the firmware image header for which
 * the CRC check is requested.
 * @retval status the returned status is 0 in case of success, otherwise the function
 * returns 1 in case of CRC failure.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint32_t getEmHdr_VerifyFirmwarePayload( uint32_t headerAddress );

/**
 * @brief This function returns the executable starging address within this firmware block.
 * BootLoaders can use this address to start code execution by jumping to it.
 * @param headerAddress this is the address of the start of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @retval address the address of the entry point of the firmware image.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint32_t getEmHdr_ExeAddress( uint32_t headerAddress);

/**
 * @brief This function returns the CRC of the EM Core that this firmware block is
 * associated with. This may be 0xFFFFFFFF if this firmware block does not
 * have an EM Core that is was built against.
 * @param headerAddress the address of a valid firmware image header
 * @retval crc the CRC of the EM-Core version that is compatible with the current
 * application software, or 0xFFFFFFFF if EM-Core compatibility is not relevant.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint32_t getEmHdr_EMCoreCRC( uint32_t headerAddress );

/**
 * @brief This function returns the size of the firmware image so the user can
 * compute the location of the next block after the current firmware.
 * @param headerAddress this is the address of the start of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
 * @retval size the firmware image size in bytes
*/
uint32_t getEmHdr_FirmwareSize( uint32_t headerAddress );

/**
 * @brief This function returns the firmware options so the user can verify any
 * relevant compatibility with the programmed image.
 * (if the value is 0xFFFFFFFF then this may not be currently supported, TBD).
 * @param headerAddress this is the address of the start of the firmware
 * header (obtained by calling getEmHdr_FindHdr).
 * @retval option this returned word contains the description of the firmware image.
 * @note The provided address shall be relevant to a valid header otherwise the returned
 * information might not be reliable.
*/
uint32_t getEmHdr_FirmwareOptions( uint32_t headerAddress );

#endif

/** @} */ // End of group firmware_header
/** @} */ // End of addtogroup firmware_header
