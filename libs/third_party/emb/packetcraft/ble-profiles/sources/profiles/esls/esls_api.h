/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Electronic Shelf Label - Server.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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
#ifndef ESLS_API_H
#define ESLS_API_H

#include <stdint.h>

#include "att_api.h"
#include "dm_api.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ELECTRONIC_SHELF_LABEL_SERVER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define ESL_LED_CTRL_FLASH_PATTERN_LEN  0x05U  /*!< Flashing pattern size */

#define ESL_LED_TYPE_BITMASK    0xC0U  /*!< LED type bitmask */
#define ESL_LED_BLUE_BITMASK    0x30U  /*!< LED blue bitmask */
#define ESL_LED_GREEN_BITMASK   0x0CU  /*!< LED green bitmask */
#define ESL_LED_RED_BITMASK     0x03U  /*!< LED red bitmask */

#define ESL_TYPE_SRGB           0x00U  /*!< Color changing RGB LED */
#define ESL_TYPE_MONO           0x01U  /*!< Monochromatic LED */

#define ESL_LED_CTRL_RED_BITMASK          0x03U  /*!< Red LED bitmask */
#define ESL_LED_CTRL_GREEN_BITMASK        0x0CU  /*!< Green LED bitmask */
#define ESL_LED_CTRL_BLUE_BITMASK         0x30U  /*!< Blue LED bitmask */
#define ESL_LED_CTRL_BRIGHTNESS_BITMASK   0xC0U  /*!< Brightness bitmask */

#define ESL_LED_CTRL_BRIGHTNESS_100   0x03U  /*!< Brightness 100% */
#define ESL_LED_CTRL_BRIGHTNESS_75    0x02U  /*!< Brightness 75% */
#define ESL_LED_CTRL_BRIGHTNESS_50    0x01U  /*!< Brightness 50% */
#define ESL_LED_CTRL_BRIGHTNESS_25    0x00U  /*!< Brightness 25% */

#define ESL_LED_CTRL_REPEAT_NUMBER    0x00U  /*!< Repeat a number of times */
#define ESL_LED_CTRL_REPEAT_TIME      0x01U  /*!< Repeat for a time duration */

#define ESL_SENSOR_TYPE_16BIT         0x00U  /*!< 16-bit sensor */
#define ESL_SENSOR_TYPE_32BIT         0x01U  /*!< 32-bit sensor */

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/** \name Electronic Shelf Label Display Types
 *
 */
/**@{*/
typedef enum
{
  ESL_DISP_TYPE_BLACK_WHITE     = 0x01U,  /*!< Black and white display. */
  ESL_DISP_TYPE_3_GRAY_SCALE    = 0x02U,  /*!< Three gray scale display. */
  ESL_DISP_TYPE_4_GRAY_SCALE    = 0x03U,  /*!< Four gray scale display. */
  ESL_DISP_TYPE_8_GRAY_SCALE    = 0x04U,  /*!< Eight gray scale display. */
  ESL_DISP_TYPE_16_GRAY_SCALE   = 0x05U,  /*!< Sixteen gray scale display. */
  ESL_DISP_TYPE_RBW             = 0x06U,  /*!< Red, black, white display. */
  ESL_DISP_TYPE_YBW             = 0x07U,  /*!< Yellow, black, white display. */
  ESL_DISP_TYPE_7_COLOR         = 0x08U,  /*!< Seven color display. */
  ESL_DISP_TYPE_16_COLOR        = 0x09U,  /*!< Sixteen color display. */
  ESL_DISP_TYPE_FULL_RGB        = 0x0AU,  /*!< Full RGB display. */
  ESL_DISP_TYPE_MAX
} eslsDisplayType_t;

/**@}*/

typedef struct
{
  uint16_t      width;        /*!< Width */
  uint16_t      height;       /*!< Height */
  uint8_t       displayType;  /*!< Display Type */
} eslsDisplayInfo_t;

typedef struct{
  uint16_t      sensorType;  /*!< Sensor Code */
} esls16bitSensor_t; /*!< 32-bit sensor */

typedef struct{
  uint16_t      companyId;   /*!< Company ID */
  uint16_t      sensorCode;  /*!< Sensor Code */
} esls32bitSensor_t; /*!< 32-bit sensor */

typedef union
{
  esls16bitSensor_t  sensor16; /*!< 16-bit sensor */
  esls32bitSensor_t  sensor32; /*!< 32-bit sensor */
} eslsSensorType_t;

typedef struct
{
  uint8_t           size; /*!< Size */
  eslsSensorType_t  type; /*!< Type */
} eslsSensorInfo_t;

typedef struct
{
  uint8_t       ledType;      /*!< LED Type */
  uint8_t       ledBlueLvl;   /*!< LED Blue Level */
  uint8_t       ledGreenLvl;  /*!< LED Green Level */
  uint8_t       ledRedLvl;    /*!< LED Red Level */
} eslsLedInfo_t;

typedef struct
{
  uint8_t pattern[ESL_LED_CTRL_FLASH_PATTERN_LEN];  /*!< Flashing Pattern */
  uint8_t bitOffPeriod;                             /*!< Bit Off Period */
  uint8_t bitOnPeriod;                              /*!< Bit On Period */
} eslsFlashPattern_t;

typedef struct
{
  uint8_t             ledRedLvl;    /*!< LED Red Level */
  uint8_t             ledGreenLvl;  /*!< LED Green Level */
  uint8_t             ledBlueLvl;   /*!< LED Blue Level */
  uint8_t             brightness;   /*!< Brightness */
  eslsFlashPattern_t  flashPattern; /*!< Flashing Pattern */
  uint8_t             repeatType;   /*!< Repeat Type */
  uint16_t            repeatDuration; /*!< Repeat Duration */
} eslsLedConfig_t;

typedef enum{
  ESL_OPCODE_PING = 0x00U,                /*!< Ping */
  ESL_OPCODE_UNASSOCIATE = 0x01U,         /*!< Unassociate from AP */
  ESL_OPCODE_SERVICE_RESET = 0x02U,       /*!< Service Reset */
  ESL_OPCODE_FACTORY_RESET = 0x03U,       /*!< Factory Reset */
  ESL_OPCODE_UPDATE_COMPLETE = 0x04U,     /*!< Update Complete */
  ESL_OPCODE_READ_SENSOR_DATA = 0x10U,    /*!< Read Sensor Data */
  ESL_OPCODE_REFRESH_DISPLAY = 0x11U,     /*!< Refresh Display */
  ESL_OPCODE_DISPLAY_IMAGE = 0x20U,       /*!< Display Image */
  ESL_OPCODE_DISPLAY_TIMED_IMAGE = 0x60U, /*!< Display Timed Image */
  ESL_OPCODE_LED_CONTROL = 0xB0U,         /*!< LED Control */
  ESL_OPCODE_LED_TIMED_CONTROL = 0xF0U,   /*!< LED Timed Control */
  ESL_OPCODE_VENDOR_SPECIFIC = 0xFFU      /*!< Vendor-specific Tag */
} eslsOpcode_t;

typedef enum
{
  ESL_RSP_ERROR           = 0x00U,  /*!< Error */
  ESL_RSP_LED_STATE       = 0x01U,  /*!< LED State */
  ESL_RSP_BASIC_STATE     = 0x10U,  /*!< Basic State */
  ESL_RSP_DISPLAY_STATE   = 0x11U,  /*!< Display State */
  ESL_RSP_SENSOR_VALUE    = 0x0EU,  /*!< Sensor Value */
  ESL_RSP_VENDOR_SPECIFIC = 0x0FU   /*!< Vendor-specific Tag */
} eslsResponse_t;

typedef enum{
  ESL_ERR_RFU = 0x00U,                    /*!< RFU */
  ESL_ERR_UNSPECIFIED = 0x01U,            /*!< Unspecified Error */
  ESL_ERR_INVALID_OPCODE = 0x02U,         /*!< Invalid Opcode */
  ESL_ERR_INVALID_STATE = 0x03U,          /*!< Invalid State */
  ESL_ERR_INVALID_IMAGE_INDEX = 0x04U,    /*!< Invalid Image Index */
  ESL_ERR_IMAGE_NOT_AVAILABLE = 0x05U,    /*!< Image Not Available */
  ESL_ERR_INVALID_PARAMETER = 0x06U,      /*!< Invalid Parameter(s) */
  ESL_ERR_CAPACITY_LIMIT = 0x07U,         /*!< Capacity Limit */
  ESL_ERR_INSUFFICIENT_BATTERY = 0x08U,   /*!< Insufficient Battery */
  ESL_ERR_INSUFFICIENT_RESOURCES = 0x09U, /*!< Insufficient Resources */
  ESL_ERR_RETRY = 0x0AU,                  /*!< Retry */
  ESL_ERR_QUEUE_FULL = 0x0BU,             /*!< Queue Full */
  ESL_ERR_IMPLAUSIBLE_ABS_TIME = 0x0CU,   /*!< Implausible Absolute Time */
  ESL_ERR_RFU_START = 0x0DU,              /*!< RFU Start */
  ESL_ERR_RFU_END = 0xEFU,                /*!< RFU End */
  ESL_ERR_VENDOR_SPECIFIC_START = 0xF0U,  /*!< Vendor-specific Start */
  ESL_ERR_VENDOR_SUCCESS        = 0xF0U,  /*!< Vendor-specific Success Code */
  ESL_ERR_VENDOR_SPECIFIC_END = 0xFFU     /*!< Vendor-specific End */
} eslsErrorCode_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the ESL Server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EslsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by the application when a message that requires
 *          processing by the ESL Server is received.
 *
 *  \param  pMsg     Event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EslsProcMsg(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
*  \brief  Handle a button press.
*
*  \param  connId    Connection identifier.
*  \param  btn       Button press.
*
*  \return None.
*/
/*************************************************************************************************/
void EslsBtn(dmConnId_t connId, uint8_t btn);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for glucose service record access control point.  Use this
 *          function as a parameter to SvcEslCbackRegister().
 *
 *  \param  connId       DM connection identifier.
 *  \param  handle       ATT handle.
 *  \param  operation    ATT operation.
 *  \param  offset       Write offset.
 *  \param  len          Write length.
 *  \param  pValue       Value to write.
 *  \param  pAttr        Attribute to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                           uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for ESL Server. Use this
 *          function as a parameter to SvcEslCbackRegister().
 *
 *  \param  connId       DM connection identifier.
 *  \param  handle       ATT handle.
 *  \param  operation    ATT operation.
 *  \param  offset       Write offset.
 *  \param  len          Write length.
 *  \param  pValue       Value to write.
 *  \param  pAttr        Attribute to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                          attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  Set the supported features of the ESL Server.
 *
 *  \param  feature   Feature bitmask.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EslsSetFeature(uint16_t feature);

/*************************************************************************************************/
/*!
 *  \brief  Set the CCCD handle list index used by the application for ESL service characteristics.
 *
 *  \param  eslCpCccIdx   ESL control point CCCD index.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EslsSetCccIdx(uint8_t eslCpCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Encode the display information from a given display information struct to a buffer then
 *          set the attribute value.
 *
 *  \param  pDisplayInfo  Pointer to the display information structure.
 */
/*************************************************************************************************/
void EslUpdateDisplayInfo(const eslsDisplayInfo_t *pDisplayInfo);

/*************************************************************************************************/
/*!
 *  \brief  Encode the sensor information from a given sensor information struct to a buffer then
 *          set the attribute value.
 *
 *  \param  pSensorInfo  Pointer to the sensor information structure.
 */
/*************************************************************************************************/
void EslUpdateSensorInfo(const eslsSensorInfo_t *pSensorInfo);

/*************************************************************************************************/
/*!
 *  \brief  Encode the LED information from a given led information struct to a buffer then
 *          set the attribute value.
 *
 *  \param  pLedInfo  Pointer to the LED information structure.
 */
/*************************************************************************************************/
void EslUpdateLedInfo(const eslsLedInfo_t *pLedInfo);

/*************************************************************************************************/
/*!
 *  \brief  Set configuration for an LED in the ESL.
 *
  *  \param  ledConfig    LED configuration parameters.
 *
 *  \return
 */
/*************************************************************************************************/
uint8_t EslsConfigureLed(uint16_t ledIdx, eslsLedConfig_t *ledConfig);

/*! \} */    /* ELECTRONIC_SHELF_LABEL_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* ESLS_API_H */
