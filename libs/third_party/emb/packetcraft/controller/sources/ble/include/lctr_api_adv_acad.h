/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller ACAD definitions.
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

#ifndef LCTR_API_ADV_ACAD_H
#define LCTR_API_ADV_ACAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ACAD header */
typedef struct
{
  uint8_t       state;              /*!< State of ACAD. */
  uint8_t       opcode;             /*!< Opcode of ACAD. */
  uint8_t       len;                /*!< Length of ACAD data field. */
} LctrAcadHdr_t;

/*! \brief      ACAD data field for channel map update */
typedef struct
{
  LctrAcadHdr_t hdr;                /*!< ACAD header. */
  uint64_t      chanMask;           /*!< Channel mask for the update. */
  uint16_t      instant;            /*!< Instant for the update. */
} LctrAcadChanMapUpd_t;

/*! \brief      ACAD data field for BIG Info */
typedef struct
{
  LctrAcadHdr_t hdr;                /*!< ACAD header. */
  /* Reference values. */
  uint32_t      bigAnchorPoint;     /*!< BIG Anchor Point time. */
  /* Peer periodic advertising parameter. */
  uint8_t       sca;                /*!< Sleep clock accuracy from SyncInfo. */
  /* ACAD fields. */
  uint16_t      bigOffs;            /*!< BIG offset. */
  uint8_t       bigOffsUnits;       /*!< BIG offset units. */
  uint16_t      isoInter;           /*!< ISO interval in units of 1.25ms. */
  uint8_t       numBis;             /*!< Number of BISs. */
  uint8_t       nse;                /*!< Number of subevents. */
  uint8_t       bn;                 /*!< Burst number. */
  uint32_t      subEvtInterUsec;    /*!< Subevent interval in microseconds. */
  uint8_t       pto;                /*!< Pre-transmission offset. */
  uint32_t      bisSpaceUsec;       /*!< BIS spacing in microseconds. */
  uint8_t       irc;                /*!< Immediate repetition count. */
  uint16_t      maxPdu;             /*!< Maximum PDU size. */
  uint32_t      seedAccAddr;        /*!< Seed access address. */
  uint32_t      sduInterUsec;       /*!< SDU interval in microseconds. */
  uint16_t      maxSdu;             /*!< Maximum SDU size. */
  uint16_t      baseCrcInit;        /*!< Base CRC init. */
  uint64_t      chanMap;            /*!< Channel map. */
  uint8_t       phy;                /*!< PHY used by BIG. */
  uint64_t      bisPldCtr;          /*!< BIS payload counter. */
  LlFraming_t   framing;            /*!< BIG carries framed or unframed data. */
  uint8_t       encrypt;            /*!< Encryption mode of the BISes in the BIG. */
  uint8_t       giv[LL_GIV_LEN];    /*!< GIV. */
  uint8_t       gskd[LL_GSKD_LEN];  /*!< GSKD. */
} LctrAcadBigInfo_t;

/*! \brief      ACAD data field for periodic advertising response timing info. */
typedef struct
{
  LctrAcadHdr_t hdr;                /*!< ACAD header. */
  uint32_t      rspAA;              /*!< Access address to be used by device when transmitting response to periodic advertiser. */
  uint8_t       numSubevents;       /*!< Number of subevents. */
  uint8_t       subeventInterv;     /*!< Time from the start of one subevent to the start of the next one. */
  uint8_t       rspSlotDelay;       /*!< Time from the start of one subevent to the first response slot. */
  uint8_t       rspSlotSpacing;     /*!< Time from the start of one response slot to the start of the next one. */
} LctrAcadPerAdvRspTimingInfo_t;

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_ACAD_H */
