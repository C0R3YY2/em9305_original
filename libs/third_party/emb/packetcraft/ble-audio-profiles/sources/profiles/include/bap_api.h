/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) API.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef BAP_API_H
#define BAP_API_H

#include "bap_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_API
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief  Pack a given Codec ID into a byte stream.
 *
 *  \param  pValue    Pointer to Codec ID value to pack to.
 *  \param  pCodecId  Pointer to Codec ID to be packed.
 *
 *  \return Pointer to octet following codec id in byte stream.
 */
/*************************************************************************************************/
uint8_t *BapPackCodecId(uint8_t *pValue, bapCodecId_t *pCodecId);

/*************************************************************************************************/
/*!
 *  \brief  Unpack a Codec ID from a given byte stream.
 *
 *  \param  pCodecId  Pointer to Codec ID to be unpacked.
 *  \param  pValue    Pointer to Codec ID value to unpacked to.
 *
 *  \return Pointer to octet following codec id in byte stream.
 */
/*************************************************************************************************/
uint8_t *BapUnpackCodecId(bapCodecId_t *pCodecId, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Check to see if two Codec IDs match.
 *
 *  \param  pCodecId1 Pointer to first Codec ID.
 *  \param  pCodecId2 Pointer to second Codec ID.
 *
 *  \return true if Codec IDs match. false, otherwise.
 */
/*************************************************************************************************/
uint8_t BapCodecIdMatch(bapCodecId_t *pCodecId1, bapCodecId_t *pCodecId2);

/*************************************************************************************************/
/*!
 *  \brief Return the total length of the LTV elements present in a given value.
 *
 *  \param  len           Length of the value.  Maximum length is 256 bytes.
 *  \param  pValue        Pointer to the value in UTF-8 format.
 *
 *  \return Total length of LTV elements.
 */
/*************************************************************************************************/
uint8_t BapLtvTotalLen(uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Find a codec specific configuration data element in the given codec specific
 *          configuration data.
 *
 *  \param  cfgType Configration data element type to find.
 *  \param  len     Length of configration data.
 *  \param  pValue  Pointer to configration data value.
 *
 *  \return Pointer to configuration data element byte array or NULL if not found.
 */
/*************************************************************************************************/
uint8_t *BapFindCfgType(uint8_t cfgType, uint16_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of a configuration data element in the given configuration data. The
 *          element is appended to the configuration data, space permitting.
 *
 *  \param  cfgType   Configuration data element type.
 *  \param  len       Length of the value.
 *  \param  pValue    Pointer to the value.
 *  \param  pCfgLen   Configuration data length.  The new length is returned in this parameter.
 *  \param  pCfg      Pointer to configuration data.
 *  \param  cfgBufLen Maximum length of the configuration data buffer maintained by caller.
 *
 *  \return true if the element was successfully added to the data, false otherwise.
 */
 /*************************************************************************************************/
bool BapSetCfgValue(uint8_t cfgType, uint8_t len, uint8_t *pValue, uint8_t *pCfgLen, uint8_t *pCfg,
                    uint16_t cfgBufLen);

/*************************************************************************************************/
/*!
 *  \brief  Delete the value of an advertising data element in the given advertising or scan
 *          response data.
 *
 *  \param  adType        Advertising data element type.
 *  \param  pAdvDataLen   Advertising or scan response data length.  The new length is returned
 *                        in this parameter.
 *  \param  pAdvData      Pointer to advertising or scan response data.
 *
 *  \return true if element was successfully deleted from data, false if element was not found.
 */
/*************************************************************************************************/
bool BapDeleteCfgValue(uint8_t adType, uint16_t *pAdvDataLen, uint8_t *pAdvData);

/*************************************************************************************************/
/*!
 *  \brief  Find a Service UUID contained in the Service Data AD data type in a given advertising
 *          or scan response data.
 *
 *  \param  svcUuid  Service UUID to look for.
 *  \param  dataLen  Data length.
 *  \param  pData    Pointer to advertising or scan response data.
 *
 *  \return Pointer to Service Data byte array or NULL if not found.
 */
/*************************************************************************************************/
uint8_t *BapFindSvcUuid(uint16_t svcUuid, uint16_t dataLen, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Find Service UUIDs contained in the list of 16-bit UUIDs AD data type in a given advertising
 *          or scan response data.
 *
 *  \param  numUuid   Number of entries in Service UUID list.
 *  \param  pSvcUuid  Service UUIDs to look for.
 *  \param  dataLen   Data length.
 *  \param  pData     Pointer to advertising or scan response data.
 *
 *  \return true if any of Service UUIDs found. false, otherwise.
 */
/*************************************************************************************************/
bool BapFindSvcUuidList(uint8_t numUuid, uint16_t *pSvcUuid, uint16_t dataLen, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Validate the Broadcast Audio Source Endpoint (BASE) structure in a Basic Audio
 *          Announcement.
 *
 *  \param  pData     Buffer with encoded Subgroup data.
 *  \param  pBisLen   Length of BASE structure.
 *
 *  \return true of encoded Subgroup record valid. false, otherwise.
 */
/*************************************************************************************************/
bool BapBaseValid(uint8_t *pData, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Return the Audio Channel Counts present in a given codec specific capabilities.
 *
 *  \param  codecCaplen   Length of codec specific capabilities.
 *  \param  pCodecCap     Pointer to codec specific capabilities.
 *
 *  \return Audio Channel Counts.
 */
 /*************************************************************************************************/
uint8_t BapGetAudioChCnt(uint8_t codecCaplen, uint8_t *pCodecCap);

/*************************************************************************************************/
/*!
 *  \brief  Check if the server supports the required audio location(s).
 *
 *  \param  audioChAlloc  Audio channel allocation.
 *  \param  audioLoc      Audio location(s).
 *  \param  codecCaplen   Length of codec specific capabilities.
 *  \param  pCodecCap     Pointer to codec specific capabilities.
 *
 *  \return true if audio locations supported. false, otherwise.
 */
 /*************************************************************************************************/
uint8_t BapAudioLocsSupported(uint32_t audioChAlloc, uint32_t audioLoc, uint8_t codecCaplen,
                              uint8_t *pCodecCap);

/*************************************************************************************************/
/*!
 *  \brief  Return the number of bits which are set in a given number.
 *
 *  \param  num       Input number.
 *
 *  \return Number of bits which are set.
 */
/*************************************************************************************************/
uint8_t BapGetNumBitsSet(uint32_t num);

/*! \} */    /* BASIC_AUDIO_PROFILE_API */

#ifdef __cplusplus
};
#endif

#endif /* BAP_API_H */
