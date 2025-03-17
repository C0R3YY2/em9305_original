/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 baseband interface file: Internal functions
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef BB_154_INT_H
#define BB_154_INT_H

#include "wsf_types.h"
#include "pal_bb_154.h"
#include "bb_154_api_op.h"
#include "mac_154_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize the 802.15.4 BB.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void Bb154Init(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for test operations.
 *
 *  Update the operation table with test operations routines.
 */
/*************************************************************************************************/
void Bb154TestInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for scan operations.
 *
 *  Update the operation table with scan operations routines according to scan type
 */
/*************************************************************************************************/
void Bb154ScanInit(uint8_t scanType);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for association operations.
 *
 *  Update the operation table with data transmit operations routines
 */
/*************************************************************************************************/
void Bb154AssocInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for data baseband operations.
 *
 *  Update the operation table with data transmit operations
 */
/*************************************************************************************************/
void Bb154DataInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Reset the baseband.
 *
 *  Resets the baseband for MAC initialization.
 */
/*************************************************************************************************/
void Bb154DataReset(void);

/*************************************************************************************************/
/*!
 *  \brief      Check if 15.4 Rx is in progress.
 *
 *  \return     BbOpDesc_t*   Pointer to BOD if in progress.
 */
/*************************************************************************************************/
BbOpDesc_t *Bb154RxInProgress(void);

/*************************************************************************************************/
/*!
 *  \brief      Cleanup test BOD.
 *
 *  \param      pOp    Pointer to the BOD to execute.
 */
/*************************************************************************************************/
void Bb154TerminateBbOp(BbOpDesc_t *pOp);

/*************************************************************************************************/
/*!
 *  \brief      Sync MAC PIB and driver PIB.
 *
 *  \param      flush   Update radio parameters if true.
 */
/*************************************************************************************************/
void Bb154SyncDrvPib(bool flush);

/**** Frame operations ****/

/*************************************************************************************************/
/*!
 *  \brief      Build Beacon frame.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildBeacon(void);

/*************************************************************************************************/
/*!
 *  \brief      Build data frame.
 *
 *  \param      len           Length.
 *  \param      srcAddrMode   Source address mode.
 *  \param      pDstAddr      Pointer to destination address.
 *  \param      txOptions     Bitmap of transmit options.
 *  \param      msduLen       Length of MSDU.
 *  \param      pMsdu         Pointer to MSDU.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildDataFrame(uint8_t len,
                                         uint8_t srcAddrMode,
                                         Mac154Addr_t *pDstAddr,
                                         uint8_t txOptions,
                                         uint8_t msduLen,
                                         uint8_t *pMsdu);

/*************************************************************************************************/
/*!
 *  \brief      Build raw frame.
 *
 *  \param      len           Length.
 *  \param      mpduLen       Length of MPDU.
 *  \param      pMpdu         Pointer to MPDU.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildRawFrame(uint8_t len,
                                        uint8_t mpduLen,
                                        uint8_t *pMpdu);

/*************************************************************************************************/
/*!
 *  \brief      Build Association Request MAC command frame.
 *
 *  \param      pCoordAddr    Address of coordinator.
 *  \param      capInfo       Capability information.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildAssocReq(Mac154Addr_t *pCoordAddr, uint8_t capInfo);

/*************************************************************************************************/
/*!
 *  \brief      Build Association Response MAC command frame.
 *
 *  \param      dstAddr   Destination extended address.
 *  \param      addr      Allocated short address.
 *  \param      status    Status from coordinator.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildAssocRsp(uint64a_t dstAddr, uint16a_t addr, uint8_t status);

#if MAC_154_OPT_DISASSOC

/*************************************************************************************************/
/*!
 *  \brief      Build Disassociation Notification MAC command frame.
 *
 *  \param      pDstAddr  Pointer to destination address.
 *  \param      reason    Disassociation reason.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildDisassocNtf(Mac154Addr_t *pDstAddr, uint8_t reason);

#endif /* MAC_154_OPT_DISASSOC */

/*************************************************************************************************/
/*!
 *  \brief      Build Data Request MAC command frame.
 *
 *  \param      pCoordAddr        Pointer to coordinator address.
 *  \param      forceSrcExtAddr   Force source address to be extended.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildDataReq(Mac154Addr_t *pCoordAddr, bool forceSrcExtAddr);

#ifdef MAC_154_OPT_ORPHAN

/*************************************************************************************************/
/*!
 *  \brief      Build Orphan Notification Request MAC command frame.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154ScanBuildOrphanNtf(void);

#endif /* MAC_154_OPT_ORPHAN */

/*************************************************************************************************/
/*!
 *  \brief      Build Beacon Request MAC command frame.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154ScanBuildBeaconReq(void);

#ifdef MAC_154_OPT_ORPHAN

/*************************************************************************************************/
/*!
 *  \brief      Build Coordinator Realignment frame.
 *
 *  \param      orphanAddr  Extended address of orphaned device.
 *  \param      panId       New PAN ID.
 *  \param      shtAddr     Existing allocated short address.
 *
 *  \return     Pointer to buffer or NULL if failed to allocate.
 */
/*************************************************************************************************/
PalBb154TxBufDesc_t *Bb154BuildCoordRealign(uint64_t orphanAddr, uint16_t panId, uint16_t shtAddr);

#endif /* MAC_154_OPT_ORPHAN */

/*************************************************************************************************/
/*!
 *  \brief      Get addresses from frame.
 *
 *  \param      pFrame        Frame buffer pointing to first octet of address field.
 *  \param      fctl          Frame control field.
 *  \param      pSrcAddr      Pointer to source address (passed by reference) - may be NULL.
 *  \param      pDstAddr      Pointer to destination address (passed by reference) - may be NULL.
 *
 *  \return     Buffer pointer advanced past address fields.
 *
 *  Obtains the source and destination addresses from the frame. If either parameter is NULL,
 *  simply skips past the fields
 */
/*************************************************************************************************/
uint8_t *Bb154GetAddrsFromFrame(uint8_t *pBuf, uint16_t fctl, Mac154Addr_t *pSrcAddr, Mac154Addr_t *pDstAddr);

/*************************************************************************************************/
/*!
 *  \brief      Process Beacon
 *
 *  \param      pRxFrame      Pointer to received frame.
 *  \param      len           Received frame length.
 *  \param      rssi          Received signal strength indicator.
 *  \param      timestamp     Timestamp of received frame.
 *  \param      pPanDescr     PAN descriptor to fill.
 *  \param      channel       Channel received on.
 */
/*************************************************************************************************/
void  Bb154ProcessBeacon(uint8_t *pRxFrame, uint16_t len, int8_t rssi, uint32_t timestamp,
                         Mac154PanDescr_t *pPanDescr, uint8_t channel);

/*************************************************************************************************/
/*!
 *  \brief      Queue the Tx indirect packet.
 *
 *  \param      pTxDesc   Transmit descriptor.
 *
 *  \return     Total number of buffers queued.
 *
 *  Calling this routine will queue a Tx indirect frame to the Tx indirect queue
 */
/*************************************************************************************************/
uint8_t Bb154QueueTxIndirectBuf(PalBb154TxBufDesc_t *pTxDesc);

/*************************************************************************************************/
/*!
 *  \brief      Purge a tx indirect packet.
 *
 *  \param      msduHandle    MSDU handle to purge.
 *
 *  \return     true if purged, false otherwise.
 *
 *  Calling this routine will purge a Tx indirect frame from the Tx indirect queue
 */
/*************************************************************************************************/
bool Bb154PurgeTxIndirectBuf(uint8_t msduHandle);

/*************************************************************************************************/
/*!
 *  \brief      Handle transaction persistence timer timeout.
 *
 *  \param      pMsg    Associated timer.
 *
 *  Calling this routine will queue a Tx indirect frame to the Tx indirect queue
 */
/*************************************************************************************************/
void Bb154HandleTPTTimeout(void *pMsg);


#ifdef __cplusplus
};
#endif

#endif /* BB_154_API_H */
