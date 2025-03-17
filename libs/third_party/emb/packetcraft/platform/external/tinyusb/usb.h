/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Audio USB interface to TinyUSB.
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

#include "wsf_types.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Call signature of a message handler. */
typedef void (*UsbCdcReceiveDataHandler_t)(const uint8_t *pData, uint32_t len);

/*************************************************************************************************/
/*!
 *  \brief      Write data to USB-CDC device.
 *
 *  \param      pData   Write buffer.
 *  \param      len     Number of bytes to write.
 */
/*************************************************************************************************/
bool UsbCdcWrite(const uint8_t *pBuf, uint32_t len);

/*************************************************************************************************/
/*!
 *  \brief      De-Initialize USB.
 */
/*************************************************************************************************/
void UsbDeInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize USB device.
 *
 *  \param  handler   USB-CDC receive data handler.
 */
/*************************************************************************************************/
void UsbInit(UsbCdcReceiveDataHandler_t handler);
