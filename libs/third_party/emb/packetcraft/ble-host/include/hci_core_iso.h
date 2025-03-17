/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI core ISO interfaces.
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
#ifndef HCI_CORE_ISO_H
#define HCI_CORE_ISO_H

#include "hci_core_ps.h"
#include "wsf_queue.h"
#include "wsf_os.h"
#include "hci_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief  ISO Function table indicies. */
enum
{
  HCI_CORE_CIS,                     /*!<  HCI core for CIS. */
  HCI_CORE_BIS,                     /*!<  HCI core for BIS. */

  HCI_CORE_TOTAL_ISO_TYPES          /*!<  Total number of HCI core types. */
};

/*! \brief  Get ISO header length. */
#define HCI_GET_ISO_DATA_HDR_LEN(pIsoHdr) (HCI_ISO_HDR_LEN + (((pIsoHdr)->tsFlag) ? HCI_ISO_DL_MAX_LEN : HCI_ISO_DL_MIN_LEN)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Data structure for unpacked ISO data packet. */
typedef struct
{
  uint8_t             *pData;       /*!< Beginning of data buffer. */

  /* ISO Header. */
  uint16_t            handle;       /*!< ISO Handle. */
  uint8_t             pb;           /*!< Packet boundary. */
  bool                tsFlag;       /*!< Timestamp flag. */
  uint16_t            isoLen;       /*!< ISO data load length. */

  /* Data load. */
  uint32_t            ts;           /*!< Timestamp (If exists). */
  uint16_t            sn;           /*!< Packet sequence number. */
  uint16_t            sduLen;       /*!< SDU Length. */
  uint8_t             ps;           /*!< Packet status. */
  uint8_t             *pIsoSdu;     /*!< ISO SDU buffer. */
} hciIsoDataPkt_t;

/*! \brief  Data structure for transmit control. */
typedef struct
{
  uint8_t             outBufs;      /*!< Currently outgoing buffers. */
  bool                fragmenting;  /*!< Currently fragmenting. */
  hciIsoDataPkt_t     *pCurPkt;     /*!< Currently transmitting ISO packet if fragmenting. */

  wsfQueue_t          sduQueue;     /*!< Queue of SDUs to be transmitted. */
} hciIsoTxCb_t;

/*! \brief  CIG structure to account for CIS. */
typedef struct
{
  uint8_t            cigId;         /*!< CIG ID. */
  uint8_t            cisCount;      /*!< Number of CIS handles in the CIG. */
  uint8_t            activeCis;     /*!< Current active CISes on this CIG. */
  uint8_t            role;          /*!< Role of CIG. */
  uint16_t           cisHandles[HCI_MAX_CIS_COUNT]; /*!< Associated CIS handles. */
} hciCoreCig_t;

/*! \brief  CIS control block. */
typedef struct
{
  uint16_t            handle;       /*!< Connection handle */
  hciIsoTxCb_t        txCb;         /*!< Transmit control block. */
  hciCoreCig_t        *pCig;        /*!< Associated CIG. */
} hciCoreCis_t;

/*! \brief  BIG structure to account for BIS. */
typedef struct
{
  uint8_t             bigId;        /*!< BIG identifier. */
  uint8_t             bigCount;     /*!< Number of BIG handles in the BIG. */
  uint8_t             activeBis;    /*!< Number of active BISes on this BIG. */
  uint8_t             role;         /*!< Role of BIG. */
  uint16_t            bigHandles[HCI_MAX_BIS_COUNT]; /*!< Associated BIS handles. */
} hciCoreBig_t;

/*! \brief  BIS structure for packet handling. */
typedef struct
{
  uint16_t            handle;       /*!< Connection handle. */
  hciCoreBig_t        *pBig;        /*!< Associated BIG. */

  /*! \brief role specific data. */
  union
  {
    struct
    {
      hciIsoTxCb_t    txCb;         /*!< Transmit control block. */
    } slv;                          /*!< Slave specific role data. */
    /* struct mst; */               /*!< Master specific role data. */
  } roleData;
} hciCoreBis_t;

/*! \brief  ISO Rx reassembly function signature. */
typedef uint8_t *(*hciIsoReassembleCb_t)(uint8_t *pData);

/*! \brief  ISO Tx complete call back function signature. */
typedef void (*hciIsoTxReadyCb_t)(uint8_t bufs);

/*! \brief  ISO open group function signature. */
typedef void (*hciIsoGroupOpenCb_t)(uint8_t id, uint8_t count, uint16_t *pHandles, uint8_t role);

/*! \brief  ISO close group function signature. */
typedef void (*hciIsoGroupCloseCb_t)(uint8_t id);

/*! \brief  ISO open function signature. */
typedef void (*hciCoreIsoConnOpenClose_t)(uint16_t handle);

/*! \brief  ISO get control block function signature. */
typedef void *(*hciCoreIsoFindByHandle_t)(uint16_t handle);

/*! \brief  HCI completed packets function signature. */
typedef void (*hciCoreIsoEvtCmplPkts_t)(uint8_t bufs);

/* ISO connection open and close, and get a control block function interface */
typedef struct
{
  hciCoreIsoConnOpenClose_t connClose;
  hciCoreIsoConnOpenClose_t connOpen;
  hciCoreIsoFindByHandle_t  findByHandle;
  hciIsoGroupOpenCb_t       groupOpen;
  hciIsoGroupCloseCb_t      groupClose;
} isoFcnIf_t;

/* \brief   ISO component function interface table. */
extern isoFcnIf_t *isoFcnIfTbl[HCI_CORE_TOTAL_ISO_TYPES];

/*! \brief CIG control blocks. */
extern hciCoreCig_t hciCoreCig[HCI_MAX_CIG_COUNT];

/*! \brief  CIS control blocks. */
extern hciCoreCis_t hciCoreCis[HCI_MAX_CIS_COUNT];

/*! \brief  BIG control blocks. */
extern hciCoreBig_t hciCoreBig[HCI_MAX_BIG_COUNT];

/*! \brief  BIS control blocks. */
extern hciCoreBis_t hciCoreBis[HCI_MAX_BIS_COUNT];

/*! \brief  ISO reassemble Rx packet function. */
extern hciIsoReassembleCb_t hciIsoReassembleCb;

/*! \brief  ISO Tx complete datapath handler function. */
extern hciIsoTxReadyCb_t hciIsoTxReadyCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************/
/*!
 *  \brief  Initialize HCI ISO resources.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciCoreIsoInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Allocate a Big connection structure.
 *
 *  \param  BigId        Big identifier.
 *  \param  bisCount     Count of BIS associated with this Big.
 *  \param  pBisHandles  List of BIS handles asoociated with this Big.
 *  \param  role         Role of Big.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreBigOpen(uint8_t bigId, uint8_t bigCount, uint16_t *pBigHandles, uint8_t role);

/*************************************************************************************************/
/*!
 *  \brief  Free a Big connection structure.
 *
 *  \param  BigId        Big identifier.
 */
/*************************************************************************************************/
void hciCoreBigClose(uint8_t bigId);

/*************************************************************************************************/
/*!
 *  \brief  Perform internal processing on HCI BIS open.
 *
 *  \param  handle  Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreBisOpen(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Perform internal processing on HCI BIS close.
 *
 *  \param  handle  Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreBisClose(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get a BIS structure by handle
 *
 *  \param  handle  Connection handle.
 *
 *  \return Pointer to BIS connection structure or NULL if not found.
 */
/*************************************************************************************************/
void* hciCoreBisByHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Open CIG.
 *
 *  \param  cigId        CIG identifier.
 *  \param  numCis       Number of CIS in this CIG.
 *  \param  pCisHandles  Handle list of CIS in this CIG.
 *  \param  role         Role of CIG.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreCigOpen(uint8_t cigId, uint8_t numCis, uint16_t *pCisHandles, uint8_t role);

/*************************************************************************************************/
/*!
 *  \brief  Close CIG context.
 *
 *  \param  cigId     CIG identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreCigClose(uint8_t cigId);

/*************************************************************************************************/
/*!
 *  \brief  Open CIS.
 *
 *  \param  handle  Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreCisOpen(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Close CIS.
 *
 *  \param  handle  Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreCisClose(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get a CIS connection structure by handle
 *
 *  \param  handle  Connection handle.
 *
 *  \return Pointer to CIS connection structure or NULL if not found.
 */
/*************************************************************************************************/
void* hciCoreCisByHandle(uint16_t handle);

/**************************************************************************************************/
/*!
 *  \brief  Initialize HCI ISO common resources.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreIsoCommonInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Send ISO data.
 *
 *  \param  handle ISO Handle to send data to.
 *  \param  len    Length of data.
 *  \param  pData  Data buffer.
 *
 *  \return true if data was succefully sent. false if not.
 */
/*************************************************************************************************/
bool hciSendIsoData(uint16_t handle, uint16_t len, uint8_t *pData, bool useTs, uint32_t ts);

/*************************************************************************************************/
/*!
 *  \brief  Unpack an ISO header.
 *
 *  \param  pData       Packet buffer.
 *  \param  pIsoPkt     Unpacked packet header.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreUnpackIsoDataPkt(uint8_t *pData, hciIsoDataPkt_t *pIsoPkt);

/*************************************************************************************************/
/*!
 *  \brief  Pack a ISO data packet into data buffer for Tx.
 *
 *  \param  pData   Data buffer.
 *  \param  pIsoPkt ISO Packet.
 *
 *  \return total bytes used by ISO Header.
 */
/*************************************************************************************************/
uint8_t hciCorePackIsoDataPkt(uint8_t *pBuf, hciIsoDataPkt_t *pIsoPkt);

/*************************************************************************************************/
/*!
 *  \brief  Reassembled Rx ISO packet.
 *
 *  \param  pData   Data buffer.
 *
 *  \return Reassembled packet if finished reassembling. NULL if packet is still pending reassembly.
 */
/*************************************************************************************************/
uint8_t *hciCoreIsoReassembly(uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Service TX Datapath.
 *
 *  \param  bufs  Freed buffers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreIsoTxReady(uint8_t bufs);

/*************************************************************************************************/
/*!
 *  \brief  Register completed packets event.
 *
 *  \param  cmplCback  Completion callback.
 */
/*************************************************************************************************/
void hciCoreIsoRegisterCmplPkts(hciCoreIsoEvtCmplPkts_t cmplCback);

/*************************************************************************************************/
/*!
 *  \brief  Close Tx control block.
 *
 *  \param  pTxCb  Tx control block.
 *  \param  handle ISO handle.
 */
/*************************************************************************************************/
void hciCoreClearIsoTxCb(hciIsoTxCb_t *pTxCb, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get ISO buffer size.
 *
 *  \param  len     Length of ISO data.
 *
 *  \return ISO buffer size.
 */
/*************************************************************************************************/
uint16_t HciGetIsoBufSize(uint16_t len);

#ifdef __cplusplus
};
#endif

#endif /* HCI_CORE_ISO_H */
