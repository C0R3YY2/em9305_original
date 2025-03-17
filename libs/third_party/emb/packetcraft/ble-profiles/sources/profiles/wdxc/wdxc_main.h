/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange profile implementation.
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

#ifndef WDXC_MAIN_H
#define WDXC_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WIRELESS_DATA_EXCHANGE_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* File transfer data block length */
#ifndef WDXC_FTD_PAYLOAD_MAX
#define WDXC_FTD_PAYLOAD_MAX          WDXS_FTD_PAYLOAD_MAX
#endif

/* File Operations */
#define WDXC_FILE_OP_NONE             0
#define WDXC_FILE_OP_PUT              1
#define WDXC_FILE_OP_GET              2
#define WDXC_FILE_OP_STREAM           3
#define WDXC_FILE_OP_DISCOVERY        4

/* File PUT Operation States */
#define WDXC_FILE_PUT_STATE_PUT       0
#define WDXC_FILE_PUT_STATE_WAIT_FS   2
#define WDXC_FILE_PUT_STATE_DATA      3

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief WDXC Connection Control Block */
typedef struct
{
  uint16_t          *pHdlList;              /*!< Attrtibute Handles */

  /* Operation in progress Control */
  uint8_t           operation;              /*!< File operation */
  uint16_t          opLocalHandle;          /*!< Local file handle */
  uint16_t          opTargetHandle;         /*!< Target file handle */
  uint32_t          opOffset;               /*!< Operation file offset */

  /* File Listing Control */
  wsfEfsFileInfo_t  *pFileList;             /*!< File Listing storage */
  uint16_t          maxFiles;               /*!< Size of pFileList in number of wsfEfsFileInfo_t objects */
  uint16_t          fileCount;              /*!< Number of files on peer device */
  uint16_t          fDlPos;                 /*!< Position in the download of file information */
} wdxcConnCb_t;

/*! \brief WDXC Control Block */
typedef struct
{
  wdxcConnCb_t      conn[DM_CONN_MAX];      /*!< Connection control */
  uint8_t           blockData[WDXC_FTD_PAYLOAD_MAX]; /*!< Buffer for file transfer block */
  uint16_t          blockDataLen;           /*!< Len of blockData in bytes */
  bool              blockConnId;            /*!< Conn ID that is waiting for filesystem to complete operation */
  WdxcFtdCallback_t *pFtdCallback;          /*!< File Transfer Data Application Callback */
  WdxcFtcCallback_t *pFtcCallback;          /*!< File Transfer Control Application Callback */
} wdxcCb_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Send a request to abort a wdx operation.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to abort operation on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendAbort(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to erase a file.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to erase on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendEraseFile(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to verify a file.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to verify on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendVerifyFile(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to put a block of data into a file on the peer device
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file on peer device
 *  \param  offset          The offset from the beginning of the file in bytes
 *  \param  len             The number of bytes to put
 *  \param  fileSize        The size of the file in bytes
 *  \param  type            reserved
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendPutReq(dmConnId_t connId, uint16_t fileHdl, uint32_t offset,
                       uint32_t len, uint32_t fileSize, uint8_t type);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to perform a get a block of data from a file on the peer device
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to verify on peer device
 *  \param  offset          The offset from the beginning of the file in bytes
 *  \param  len             The number of bytes to get
 *  \param  type            reserved
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendGetReq(dmConnId_t connId, uint16_t fileHdl, uint32_t offset, uint32_t len, uint8_t type);

/*! \} */    /* WIRELESS_DATA_EXCHANGE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* WDXC_MAIN_H */
