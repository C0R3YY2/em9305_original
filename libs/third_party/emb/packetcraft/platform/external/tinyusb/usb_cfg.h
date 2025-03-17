/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  USB configuration.
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

#include "tusb.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]       MIDI | HID | MSC | CDC          [LSB]
 */
#define _USB_CFG_PID_MAP(itf, n)  ((CFG_TUD_##itf) << (n))
#define USB_CFG_PID               (0x4000 | _USB_CFG_PID_MAP(CDC, 0) | _USB_CFG_PID_MAP(MSC, 1) | \
                                  _USB_CFG_PID_MAP(HID, 2) | _USB_CFG_PID_MAP(MIDI, 3) | \
                                  _USB_CFG_PID_MAP(VENDOR, 4) )

/*! \brief Nordic Semiconductor ASA Vendor ID. */
#define USB_CFG_VENDOR_ID 0x1915

/*! \brief Specification release number in Binary-Coded Decimal. */
#define USB_CFG_SPEC_BCD   0x0200

/*! \brief Supported language: English (0x0409) */
#define USB_CFG_SUPP_LANG ((const char[]) { 0x09, 0x04 })

/*! \brief FIXME: Serial number should be got from the HW. */
#define USB_CFG_SERIAL "123456"

/*! \brief CDC interface type. */
#define USB_CFG_CDC_INTERFACE "TinyUSB CDC"

/*! \brief Audio interface type. */
#define USB_CFG_AUDIO_INTERFACE "TinyUSB UAC2"

/*! \brief Manufacturer. */
#define USB_CFG_MANUFACTURER "Packetcraft"

/*! \brief Product. */
#define USB_CFG_PRODUCT "Packetcraft device"
