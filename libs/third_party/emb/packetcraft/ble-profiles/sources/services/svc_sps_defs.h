/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief Serial Porl Protocol Definitions.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_SPS_DEFS_H
#define SVC_SPS_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup SERIAL_PORT_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Base UUID:  005fXXXX-2ff2-4ed5-b045-230163617865. */
#define SPS_UUID_PART1                0x65, 0x78, 0x61, 0x63, 0x01, 0x23, 0x45, 0xb0, \
                                      0xd5, 0x4e, 0xf2, 0x2f
/*! \brief Base UUID Part 2. */
#define SPS_UUID_PART2                0x5f, 0x00

/*! \brief Macro for building UUIDs. */
#define SPS_UUID_BUILD(part)          SPS_UUID_PART1, UINT16_TO_BYTES(part), SPS_UUID_PART2

/*! \brief SPS Service. */
#define SPS_SVC_UUID                  0x1101

/*! \brief SPS TX DATA. */
#define SPS_TX_DATA_UUID              SPS_UUID_BUILD(0x0001)

/*! \brief SPS RX DATA. */
#define SPS_RX_DATA_UUID              SPS_UUID_BUILD(0x0002)

/*! \brief SPS TX READ. */
#define SPS_TX_READ_UUID              SPS_UUID_BUILD(0x0003)

/*! \brief SPS RX READ. */
#define SPS_RX_READ_UUID              SPS_UUID_BUILD(0x0004)

/*! \} */    /* SERIAL_PORT_PROFILE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_SPS_DEFS_H */
