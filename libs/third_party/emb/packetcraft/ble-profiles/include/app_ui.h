/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Application framework user interface.
 *
 *  Copyright (c) 2011-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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
#ifndef APP_UI_H
#define APP_UI_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup APP_FRAMEWORK_UI_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief UI event enumeration  */
enum
{
  APP_UI_NONE,                            /*!< No event */
  APP_UI_RESET_CMPL,                      /*!< Reset complete */
  APP_UI_DISCOVERABLE,                    /*!< Enter discoverable mode */
  APP_UI_ADV_START,                       /*!< Advertising started */
  APP_UI_ADV_STOP,                        /*!< Advertising stopped */
  APP_UI_SCAN_START,                      /*!< Scanning started */
  APP_UI_SCAN_STOP,                       /*!< Scanning stopped */
  APP_UI_SCAN_REPORT,                     /*!< Scan data received from peer device */
  APP_UI_CONN_OPEN,                       /*!< Connection opened */
  APP_UI_CONN_CLOSE,                      /*!< Connection closed */
  APP_UI_SEC_PAIR_CMPL,                   /*!< Pairing completed successfully */
  APP_UI_SEC_PAIR_FAIL,                   /*!< Pairing failed or other security failure */
  APP_UI_SEC_ENCRYPT,                     /*!< Connection encrypted */
  APP_UI_SEC_ENCRYPT_FAIL,                /*!< Encryption failed */
  APP_UI_PASSKEY_PROMPT,                  /*!< Prompt user to enter passkey */
  APP_UI_DISPLAY_PASSKEY,                 /*!< Display passkey */
  APP_UI_DISPLAY_CONFIRM,                 /*!< Display confirm value */
  APP_UI_DISPLAY_RSSI,                    /*!< Display rssi */
  APP_UI_ALERT_CANCEL,                    /*!< Cancel a low or high alert */
  APP_UI_ALERT_LOW,                       /*!< Low alert */
  APP_UI_ALERT_HIGH,                      /*!< High alert */
  APP_UI_ADV_SET_START,                   /*!< Advertising set(s) started */
  APP_UI_ADV_SET_STOP,                    /*!< Advertising set(s) stopped */
  APP_UI_SCAN_REQ_RCVD,                   /*!< Scan request received */
  APP_UI_EXT_SCAN_START,                  /*!< Extended scanning started */
  APP_UI_EXT_SCAN_STOP,                   /*!< Extended scanning stopped */
  APP_UI_EXT_SCAN_REPORT,                 /*!< Extended scan data received from peer device */
  APP_UI_PER_ADV_SET_START,               /*!< Periodic advertising set started */
  APP_UI_PER_ADV_SET_STOP,                /*!< Periodic advertising set stopped */
  APP_UI_PER_ADV_SYNC_EST,                /*!< Periodic advertising sync established */
  APP_UI_PER_ADV_SYNC_EST_FAIL,           /*!< Periodic advertising sync establishment failed */
  APP_UI_PER_ADV_SYNC_LOST,               /*!< Periodic advertising sync lost */
  APP_UI_PER_ADV_SYNC_TRSF_EST,           /*!< Periodic advertising sync transfer established */
  APP_UI_PER_ADV_SYNC_TRSF_EST_FAIL,      /*!< Periodic advertising sync transfer establishment failed */
  APP_UI_PER_ADV_SYNC_TRSF,               /*!< Periodic advertising sync info transferred */
  APP_UI_PER_ADV_SET_INFO_TRSF,           /*!< Periodic advertising set sync info transferred */
  APP_UI_PER_ADV_REPORT,                  /*!< Periodic advertising data received from peer device */
  APP_UI_CIS_CIG_CONFIG,                  /*!< CIS CIG configure complete */
  APP_UI_CIS_CIG_REMOVE,                  /*!< CIS CIG remove complete */
  APP_UI_CIS_REQ,                         /*!< CIS request */
  APP_UI_CIS_OPEN,                        /*!< CIS connection opened */
  APP_UI_CIS_CLOSE,                       /*!< CIS connection closed */
  APP_UI_REQ_PEER_SCA,                    /*!< Request peer SCA complete */
  APP_UI_ISO_DATA_PATH_SETUP,             /*!< ISO data path setup complete */
  APP_UI_ISO_DATA_PATH_REMOVE,            /*!< ISO data path remove complete */
  APP_UI_DATA_PATH_CONFIG,                /*!< Data path configure complete */
  APP_UI_READ_LOCAL_SUP_CODECS,           /*!< Local supported codecs read */
  APP_UI_READ_LOCAL_SUP_CODEC_CAP,        /*!< Local supported codec capabilities read */
  APP_UI_READ_LOCAL_SUP_CTR_DLY,          /*!< Local supported controller delay read */
  APP_UI_BIG_START,                       /*!< BIG started */
  APP_UI_BIG_STOP,                        /*!< BIG stopped */
  APP_UI_BIG_SYNC_EST,                    /*!< BIG sync established */
  APP_UI_BIG_SYNC_EST_FAIL,               /*!< BIG sync establishment failed */
  APP_UI_BIG_SYNC_LOST,                   /*!< BIG sync lost */
  APP_UI_BIG_SYNC_STOP,                   /*!< BIG sync stopped */
  APP_UI_BIG_INFO_ADV_REPORT,             /*!< BIG Info advertising data received from peer device */
  APP_UI_HW_ERROR                         /*!< Hardware error */
};

/*! \brief Button press enumeration */
enum
{
  APP_UI_BTN_NONE,                        /*!< No button press */
  APP_UI_BTN_1_DOWN,                      /*!< Button 1 on down press */
  APP_UI_BTN_1_SHORT,                     /*!< Button 1 short press */
  APP_UI_BTN_1_MED,                       /*!< Button 1 medium press */
  APP_UI_BTN_1_LONG,                      /*!< Button 1 long press */
  APP_UI_BTN_1_EX_LONG,                   /*!< Button 1 extra long press */
  APP_UI_BTN_2_DOWN,                      /*!< Button 2 on down press */
  APP_UI_BTN_2_SHORT,                     /*!< Button 2 short press */
  APP_UI_BTN_2_MED,                       /*!< Button 2 medium press */
  APP_UI_BTN_2_LONG,                      /*!< Button 2 long press */
  APP_UI_BTN_2_EX_LONG                    /*!< Button 2 extra long press */
};

/*! \brief LED values */
#define APP_UI_LED_NONE     0x00          /*!< No LED */
#define APP_UI_LED_1        0x01          /*!< LED 1 */
#define APP_UI_LED_2        0x02          /*!< LED 2 */
#define APP_UI_LED_3        0x04          /*!< LED 3 */
#define APP_UI_LED_4        0x08          /*!< LED 4 */
#define APP_UI_LED_WRAP     0xFF          /*!< Wrap to beginning of sequence */

/*! \brief Sound tone value for wrap/repeat */
#define APP_UI_SOUND_WRAP   0xFFFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Button press callback */
typedef void (*appUiBtnCback_t)(uint8_t btn);

/*! \brief Action event callback */
typedef void (*appUiActionCback_t)(uint8_t event, uint32_t param);

/*! \brief Button Poll callback */
typedef void (*appUiBtnPollCback_t)(void);

/*! \brief Print callback */
typedef void (*appUiPrintFunc_t)(const char *txt);

/*! \brief Sound data structure */
typedef struct
{
  uint16_t      tone;                     /*!< Sound tone in Hz.  Use 0 for silence. */
  uint16_t      duration;                 /*!< Sound duration in milliseconds */
} appUiSound_t;

/*! \brief LED data structure */
typedef struct
{
  uint8_t       led;                      /*!< LED to control */
  uint8_t       state;                    /*!< On or off */
  uint16_t      duration;                 /*!< duration in milliseconds */
} appUiLed_t;

/*! \brief Callback structure */
typedef struct
{
  appUiBtnCback_t       btnCback;         /*!< Called when button pressed */
  appUiActionCback_t    actionCback;      /*!< Called when action event received */
  appUiBtnPollCback_t   btnPollCback;     /*!< Called to poll button hardware */
} appUiCback_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name APP User Interface
 * Commands that may be sent via terminal to the application.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Perform a user interface action based on the event value passed to the function.
 *
 *  \param  event   User interface event value.
 *  \param  status  User interface status value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiAction(uint8_t event, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Display a passkey.
 *
 *  \param  passkey   Passkey to display.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiDisplayPasskey(uint32_t passkey);

/*************************************************************************************************/
/*!
*  \brief  Display a confirmation value.
*
*  \param  confirm    Confirm value to display.
*
*  \return None.
*/
/*************************************************************************************************/
void AppUiDisplayConfirmValue(uint32_t confirm);

/*************************************************************************************************/
/*!
 *  \brief  Display an RSSI value.
 *
 *  \param  rssi   Rssi value to display.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiDisplayRssi(int8_t rssi);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive button presses.
 *
 *  \param  btnCback    Callback function.
 *
 *  \return None.
 *
 *  \note   Registered by application to receive button events
 */
/*************************************************************************************************/
void AppUiBtnRegister(appUiBtnCback_t btnCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive action events.
 *
 *  \param  actionCback   Callback function.
 *
 *  \return None.
 *
 *  \note   Registered by platform
 */
/*************************************************************************************************/
void AppUiActionRegister(appUiActionCback_t actionCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive APP_BTN_POLL_IND events.
 *
 *  \param  btnPollCback   Callback function.
 *
 *  \return None.
 *
 *  \note   Registered by platform
 */
/*************************************************************************************************/
void AppUiBtnPollRegister(appUiBtnPollCback_t btnPollCback);

/*************************************************************************************************/
/*!
 *  \brief  Handle a hardware button press.  This function is called to handle WSF
 *          event APP_BTN_DOWN_EVT.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiBtnPressed(void);

/*************************************************************************************************/
/*!
 *  \brief  Play a sound.
 *
 *  \param  pSound   Pointer to sound tone/duration array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiSoundPlay(const appUiSound_t *pSound);

/*************************************************************************************************/
/*!
 *  \brief  Stop the sound that is currently playing.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiSoundStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Start LED blinking.
 *
 *  \param  pLed   Pointer to LED data structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiLedStart(const appUiLed_t *pLed);

/*************************************************************************************************/
/*!
 *  \brief  Stop LED blinking.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppUiLedStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Button test function-- for test purposes only.
 *
 *  \param  btn   button press
 *  \return None.
 */
/*************************************************************************************************/
void AppUiBtnTest(uint8_t btn);

/**@}*/

/*! \} */    /*! APP_FRAMEWORK_UI_API */

#ifdef __cplusplus
};
#endif

#endif /* APP_UI_H */
