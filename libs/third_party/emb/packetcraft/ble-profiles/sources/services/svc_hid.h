/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Human Interface Device service implementation.
 *
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_HID_H
#define SVC_HID_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup HUMAN_INTERFACE_DEVICE_SERVICE
 *  \{ */

/**************************************************************************************************
Macros
**************************************************************************************************/

/*! \brief HID Service */
#define HID_SVC_UUID                  ATT_UUID_HID_SERVICE

/** \name HID Spec Version
 *
 */
/**@{*/
/*! \brief HID Spec Version: 1.11 */
#define HID_VERSION                   0x0111
/**@}*/

/** \name HID Report Types
 *
 */
/**@{*/
#define HID_REPORT_TYPE_INPUT         0x01  /*!< Input type. */
#define HID_REPORT_TYPE_OUTPUT        0x02  /*!< Output type. */
#define HID_REPORT_TYPE_FEATURE       0x03  /*!< Feature type. */
/**@}*/

/** \name HID Protocol Mode Types
 *
 */
/**@{*/
#define HID_PROTOCOL_MODE_BOOT        0x00  /*!< Boot mode. */
#define HID_PROTOCOL_MODE_REPORT      0x01  /*!< Report mode. */
/**@}*/

/** \name HID Control Point Values
 *
 */
/**@{*/
#define HID_CONTROL_POINT_SUSPEND     0x00  /*!< Suspend. */
#define HID_CONTROL_POINT_RESUME      0x01  /*!< Resume. */
/**@}*/

/*! \brief Max length of the report map value */
#define HID_MAX_REPORT_MAP_LEN        512

/*! \brief Max length of an output report value */
#define HID_MAX_REPORT_LEN            32

/*! \brief Initial length of the report map value */
#define HID_INIT_REPORT_MAP_LEN       1

/*! \brief Length of the control point value */
#define HID_CONTROL_POINT_LEN         1

/*! \brief Length of the protocol mode value */
#define HID_PROTOCOL_MODE_LEN         1

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name HID Service Handles
 *
 */
/**@{*/
#define HID_START_HDL                 0x80              /*!< Start handle. */
#define HID_END_HDL                   (HID_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/
/*! \brief Proprietary Service Handles Common to HID Devices */
enum
{
  HID_SVC_HDL = HID_START_HDL,        /*!< Proprietary Service Declaration */
  HID_INFO_CH_HDL,                    /*!< HID Information Characteristic Declaration */
  HID_INFO_HDL,                       /*!< HID Information Value */
  HID_REPORT_MAP_CH_HDL,              /*!< HID Report Map Characteristic Declaration */
  HID_REPORT_MAP_HDL,                 /*!< HID Report Map Value */
  HID_EXTERNAL_REPORT_HDL,            /*!< HID External Report Descriptor */
  HID_CONTROL_POINT_CH_HDL,           /*!< HID Control Point Characteristic Declaration */
  HID_CONTROL_POINT_HDL,              /*!< HID Control Point Value */
  HID_KEYBOARD_BOOT_IN_CH_HDL,        /*!< HID Keyboard Boot Input Characteristic Declaration */
  HID_KEYBOARD_BOOT_IN_HDL,           /*!< HID Keyboard Boot Input Value */
  HID_KEYBOARD_BOOT_IN_CH_CCC_HDL,    /*!< HID Keyboard Boot Input CCC Descriptor */
  HID_KEYBOARD_BOOT_OUT_CH_HDL,       /*!< HID Keyboard Boot Output Characteristic Declaration */
  HID_KEYBOARD_BOOT_OUT_HDL,          /*!< HID Keyboard Boot Output Value */
  HID_MOUSE_BOOT_IN_CH_HDL,           /*!< HID Mouse Boot Input Characteristic Declaration */
  HID_MOUSE_BOOT_IN_HDL,              /*!< HID Mouse Boot Input Value */
  HID_MOUSE_BOOT_IN_CH_CCC_HDL,       /*!< HID Mouse Boot Input CCC Descriptor */
  HID_INPUT_REPORT_1_CH_HDL,          /*!< HID Input Report Characteristic Declaration */
  HID_INPUT_REPORT_1_HDL,             /*!< HID Input Report Value */
  HID_INPUT_REPORT_1_CH_CCC_HDL,      /*!< HID Input Report CCC Descriptor */
  HID_INPUT_REPORT_1_REFERENCE_HDL,   /*!< HID Input Report Reference Descriptor */
  HID_INPUT_REPORT_2_CH_HDL,          /*!< HID Input Report Characteristic Declaration */
  HID_INPUT_REPORT_2_HDL,             /*!< HID Input Report Value */
  HID_INPUT_REPORT_2_CH_CCC_HDL,      /*!< HID Input Report CCC Descriptor */
  HID_INPUT_REPORT_2_REFERENCE_HDL,   /*!< HID Input Report Reference Descriptor */
  HID_INPUT_REPORT_3_CH_HDL,          /*!< HID Input Report Characteristic Declaration */
  HID_INPUT_REPORT_3_HDL,             /*!< HID Input Report Value */
  HID_INPUT_REPORT_3_CH_CCC_HDL,      /*!< HID Input Report CCC Descriptor */
  HID_INPUT_REPORT_3_REFERENCE_HDL,   /*!< HID Input Report Reference Descriptor */
  HID_OUTPUT_REPORT_CH_HDL,           /*!< HID Output Report Characteristic Declaration */
  HID_OUTPUT_REPORT_HDL,              /*!< HID Output Report Value */
  HID_OUTPUT_REPORT_REFERENCE_HDL,    /*!< HID Output Report Reference Descriptor */
  HID_FEATURE_REPORT_CH_HDL,          /*!< HID Feature Report Characteristic Declaration */
  HID_FEATURE_REPORT_HDL,             /*!< HID Feature Report Value */
  HID_FEATURE_REPORT_REFERENCE_HDL,   /*!< HID Feature Report Reference Descriptor */
  HID_PROTOCOL_MODE_CH_HDL,           /*!< HID Protocol Mode Characteristic Declaration */
  HID_PROTOCOL_MODE_HDL,              /*!< HID Protocol Mode Value */
  HID_MAX_HDL                         /*!< Maximum handle. */
};
/**@}*/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcHidAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcHidRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register a read and write callback functions for the ATT Group.
 *
 *  \param  writeCb   Write callback function
 *  \param  readCb    Read callback function
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcHidRegister(attsWriteCback_t writeCb, attsReadCback_t readCb);

/*************************************************************************************************/
/*!
 *  \brief  Add the Hid Service using the dynamic attribute subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void *SvcHidAddGroupDyn(void);

/*! \} */    /* HUMAN_INTERFACE_DEVICE_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_HID_H */
