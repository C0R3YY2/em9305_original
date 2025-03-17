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

#ifndef __NVMBL_HEADER_ACCESS_H
#define __NVMBL_HEADER_ACCESS_H

#include "types.h"


/******************************************************************************\
 *  Symbolic constants
\******************************************************************************/
#define NVM_START_ADDR 0x300000U
#define NVM_END_ADDR NVM_START_ADDR + (NVM_PAGE_COUNT_MAIN * NVM_PAGE_SIZE_BYTE)
#define HEADER_ADDRESS_ERROR 0xFFFFFFFFU
// CRC
#define CRC_PASSED 0U
#define CRC_FAILURE 1U

/******************************************************************************\
 *  Public Function Prototypes
\******************************************************************************/
/*
 * \brief: returns the highest version of the firmware header that this code
 * supports.  If a new firmware header's version number is greater than the
 * returned SupportedHdrVersion then incompatibility issues may occur.  See EM
 * Support for compatibility.
 */
uint8_t nvmbl_getEmHdr_SupportedVersion(void);

/*
 * \brief: returns the address of the first firmware block found at or after
 * the provided address (this is refered to below as headerAddress). The
 * returned address (NVM Address) is the actual start address of firmware Hdr
 * within firmware block, or 0xFFFFFFFF (not found ndicator) when the next
 * firmware block is not found. Note: this will verify the CRC of the header
 * and report an error if corrupt.
 * \param: searchForHeader this is the address where this function will start
 * searching NVM.
*/
uint32_t nvmbl_getEmHdr_FindHdr( uint32_t searchForHeader );

/*
 * \brief: returns the size of header found.  When calculating the full size
 * of this firmware block, the firmware header size must be added to the
 * firmware size.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
 */
uint8_t nvmbl_getEmHdr_HdrVersionSize(uint32_t headerAddress);

/*
 * \brief: returns version of this header. Verify that this Identifier matches
 * the version supported by this code (see nvmbl_getEmHdr_SupportedVersion).
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint8_t nvmbl_getEmHdr_HdrVersion( uint32_t headerAddress );

/*
 * \brief: reports the type of firmware block. If this is not the type of
 * firmware block required then use nvmbl_getEmHdr_FindHdr with the input
 * 'headerAddress + 4'. If it returns a valid starting address goto step X,
 * otherwise repeat step Y.  The headerAddress and sectionCode should be kept
 * by the user for quick look-up.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint8_t nvmbl_getEmHdr_SectionCode( uint32_t headerAddress );

/*
 * \brief: returns the firmwareVersion number (set by the user).  This can be
 * used to verify if a new firmware block (e.g. customer's application) is the
 * same as an existing firmware block.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint16_t nvmbl_getEmHdr_FirmwareVersion( uint32_t headerAddress );

/*
 * \brief: this does a CRC check over the firmware payload data.  Returns::
 *  0 = CRC passed
 *  1 = CRC failure, the firmware is fully written
 *  0x<EndOfPayloadInNVM> = firmware payload is not fully written and this
 *                          provides the NVM address of the last written
 *                          (non-0xFFFFFFFF) DWord (uint32_t).  See definition
 *                          of the search algorithm, in the documentation.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint32_t nvmbl_getEmHdr_VerifyFirmwarePayload( uint32_t headerAddress );

/*
 * \brief: returns the executable starging address within this firmware block.
 * BootLoaders can use this address to start code execution.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint32_t nvmbl_getEmHdr_ExeAddress( uint32_t headerAddress);

/*
 * \brief: returns the CRC of the EM Core that this firmware block is
 * associated with.  This may be 0xFFFFFFFF if this firmware block does not
 * have an EM Core that is was built against.
*/
uint32_t nvmbl_getEmHdr_EMCoreCRC( uint32_t headerAddress );

/*
 * \brief: returns the firmware size so the user to compute the location of the
 * next block.
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint32_t nvmbl_getEmHdr_FirmwareSize( uint32_t headerAddress );

/*
 * \brief: returns the firmware options so the user can verify compatibility
 * (if the value is 0xFFFFFFFF then this may not be currently supported, TBD).
 * \param: headerAddress, this is the address of the start of the firmware
 * header (obtained by calling nvmbl_getEmHdr_FindHdr).
*/
uint32_t nvmbl_getEmHdr_FirmwareOptions( uint32_t headerAddress );

#endif
