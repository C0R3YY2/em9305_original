/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 baseband interface file.
 *
 *  Copyright (c) 2016-2019 ARM Ltd. All Rights Reserved.
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

#ifndef BB_154_API_OP_H
#define BB_154_API_OP_H

#include "bb_api.h"
#include "pal_bb_154.h"
#include "mac_154_int.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MAC_154_API
 *  \{ */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Indirect buffer queue size. */
#define BB_154_DATA_IND_BUF_CNT                 5                                     /*!< TBD */

/*! \brief      Symbol durations. */
#define BB_154_BASE_SUPERFRAME_DURATION_SYMB    (PAL_BB_154_aBaseSuperframeDuration)  /*!< TBD */
#define BB_154_ED_DURATION_SYMB                 (PAL_BB_154_ED_DURATION)              /*!< TBD */

/*! \brief      Operation types. */
enum
{
  BB_154_OP_TEST_TX,                    /*!< Continuous Tx test mode. */
  BB_154_OP_TEST_RX,                    /*!< Continuous Rx test mode. */
  BB_154_OP_SCAN,                       /*!< Scan mode. */
  BB_154_OP_ASSOC,                      /*!< Association mode. */
  BB_154_OP_DATA_TX,                    /*!< Single frame data transmit mode. */
  BB_154_OP_DATA_RX,                    /*!< Data receive mode. */
  BB_154_OP_DATA_POLL,                  /*!< Data poll mode. */
  BB_154_OP_NUM                         /*!< TBD */
};

/*! \brief      ED/CCA test modes. */
enum
{
  BB_154_ED_SCAN_TEST_MODE_NONE = 0,    /*!< ED scan no test mode. */
  BB_154_ED_SCAN_TEST_MODE_ED = 1,      /*!< ED scan ED test mode. */
  BB_154_ED_SCAN_TEST_MODE_CCA = 2,     /*!< ED scan CCA test mode. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Forward reference */
typedef struct Bb154Data_tag Bb154Data_t;

/*! \brief      Execution operation function. */
typedef void (*bb154ExecOpFn_t)(BbOpDesc_t *pBod, Bb154Data_t *p154);

/*
 * TODO: It's not clear we need a separate entry for each operation
 * type as they are mutually exclusive at present. This might be needed
 * for nested operations.
 */

/*! \brief      BB control block. */
typedef struct
{
  struct
  {
    bb154ExecOpFn_t execOpCback;        /*!< Execute operation handler. */
  } opCbacks[BB_154_OP_NUM];            /*!< Operation handlers. */
} bb154CtrlBlk_t;

/*! \brief      Test completion callback signature. */
typedef void (*Bb154TestComp_t)(BbOpDesc_t *pBod, bool ack, bool success);

/*! \brief      Test transmit operation data. */
typedef struct
{
  Bb154TestComp_t         testCback;          /*!< Test callback. */
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Transmit data buffer descriptor. */
  uint16_t                rxLen;              /*!< Receive data buffer length. */
  uint32_t                pktInterUsec;       /*!< Transmit packet interval. */
  bool                    rxAck;              /*!< true if ACK should be received. */
} Bb154TestTx_t;

/*! \brief      Test receive operation data. */
typedef struct
{
  Bb154TestComp_t         testCback;          /*!< Test callback. */
  uint8_t                 *pRxFrame;          /*!< Received frame. */
  uint16_t                rxLen;              /*!< Received frame length. */
  bool                    txAck;              /*!< true if ACK should be transmitted. */
} Bb154TestRx_t;

/*! \brief      Scan operation data. */
typedef struct
{
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Beacon request/orphan notification buffer. */
  uint32_t                snapshot;           /*!< BB timer snapshot. */
  uint32_t                elapsed;            /*!< BB timer elapsed time. */
  uint32_t                duration;           /*!< Scan Duration. */
  uint32_t                remaining;          /*!< Scan Remaining time. */
  uint32_t                channels;           /*!< Channel bitmap to scan. */
  uint8_t                 channel;            /*!< Intermediate channel value. */
  uint8_t                 type;               /*!< Type of Scan. */
  bool                    terminate;          /*!< Flag to force termination. */
  uint8_t                 testMode;           /*!< ED/CCA scan test mode - 10 consecutive ED/CCA scans on one channel. */
  uint8_t                 listSize;           /*!< List size. */
  Mac154ScanResults_t     results;            /*!< Scan results. */
} Bb154Scan_t;

/*! \brief      Association request operation data. */
typedef struct
{
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Associated transmit frame buffer. */
  uint8_t                 *pRxFrame;          /*!< Pointer to received frame. */
  uint16_t                rxLen;              /*!< Received frame length. */
  uint8_t                 cmd;                /*!< MAC command frame (assoc req, assoc rsp, disassoc). */
  uint8_t                 status;             /*!< Confirm status. */
} Bb154Assoc_t;

/*! \brief      Association request operation data. */
typedef struct
{
  Bb154Assoc_t            assoc;              /*!< Common association data. */
  Mac154Addr_t            coordAddr;          /*!< Coordinator address. */
  uint16_t                addr;               /*!< Association allocated short address. */
  uint8_t                 status;             /*!< Status. */
} Bb154AssocReq_t;

/*! \brief      Association response operation data. */
typedef struct
{
  Bb154Assoc_t            assoc;              /*!< Common association data. */
} Bb154AssocRsp_t;

/*! \brief      Disassociation operation data. */
typedef struct
{
  Bb154Assoc_t            assoc;              /*!< Common association data. */
  uint8_t                 txIndirect;         /*!< Sending indirect. */
  Mac154Addr_t            deviceAddr;         /*!< Device address. */
  uint8_t                 reason;             /*!< Reason. */
} Bb154Disassoc_t;

/*! \brief      Data transmit operation data. */
typedef struct
{
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Associated frame buffer. */
  uint32_t                snapshot;           /*!< BB timer snapshot. */
  uint32_t                timestamp;          /*!< When frame was transmitted. */
  uint8_t                 status;             /*!< Status. */
} Bb154DataTx_t;

/*! \brief      Start operation data. Note same initial elements as Bb154DataTx_t. */
typedef struct
{
  /* Same fields as Bb154DataTx_t. */
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Associated frame buffer. */
  uint32_t                snapshot;           /*!< BB timer snapshot. */
  uint32_t                timestamp;          /*!< When frame was transmitted. */
  uint8_t                 status;             /*!< Status. */
  /* Additional fields. */
  uint16_t                panId;              /*!< PAN ID to start. */
  uint8_t                 panCoord;           /*!< Start as PAN coordinator. */
  uint8_t                 logChan;            /*!< Channel to start on. */
  int8_t                  txPower;            /*!< Tx power to start on. */
} Bb154Start_t;

/*! \brief      Data poll operation data. Note same initial elements as Bb154DataTx_t. */
typedef struct
{
  /* Same fields as Bb154DataTx_t. */
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Associated frame buffer. */
  uint8_t                 status;             /*!< Status. */
} Bb154Poll_t;

/*! \brief      Data receive operation data. */
typedef struct
{
  PalBb154TxBufDesc_t     *pTxDesc;           /*!< Frame buffer which may be transmitted. */
  uint8_t                 msduHandle;         /*!< MSDU handle. */
} Bb154DataRx_t;

/*! \brief      802.15.4 protocol specific operation parameters. */
struct Bb154Data_tag
{
  Mac154ParamTimer_t      guardTimer;         /*!< Guard timer. */
  uint8_t                 opType;             /*!< Operation type. */
  PalBb154Chan_t          chan;               /*!< Channel. */
  PalBb154OpParam_t       opParam;            /*!< Operation parameters. */

  union
  {
    Bb154TestTx_t         testTx;             /*!< Transmit test data. */
    Bb154TestRx_t         testRx;             /*!< Receive test data. */
    Bb154Assoc_t          assoc;              /*!< Common association data. */
    Bb154AssocReq_t       assocReq;           /*!< Association request data. */
    Bb154AssocRsp_t       assocRsp;           /*!< Association response data. */
    Bb154Disassoc_t       disassoc;           /*!< Disassociation notification data. */
    Bb154Scan_t           scan;               /*!< Scan data. */
    Bb154DataTx_t         dataTx;             /*!< Single frame tx data. */
    Bb154Start_t          start;              /*!< Start data. */
    Bb154Poll_t           poll;               /*!< Poll. */
    Bb154DataRx_t         dataRx;             /*!< Rx data. */
  } op;                                       /*!< Operation specific data. */
};

/*************************************************************************************************/
/*!
 *  \brief      Register operation handlers.
 *
 *  \param      opType          Operation type.
 *  \param      execOpCback     Execute operation callback.
 */
/*************************************************************************************************/
void bb154RegisterOp(uint8_t opType, bb154ExecOpFn_t execOpCback);


/*! \} */    /* MAC_154_API */

#ifdef __cplusplus
};
#endif

#endif /* BB_154_API_OP_H */
