////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.h
///
/// @project    T9305
///
/// @brief      Bluetooth LE management header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef BLE_H_
#define BLE_H_

#include <wsf_os.h>

/******************************************************************************\
 *  #defines
\******************************************************************************/

// String to set for device name characteristic in GAP service
#define ANCS_GAP_DEVICE_NAME_STR    "EM9305_ANCS"
#define ANCS_GAP_DEVICE_NAME_CHARS  'E', 'M', '9', '3', '0', '5', '_', 'A', 'N', 'C', 'S'
#define ANCS_GAP_DEVICE_NAME_LEN    11

#if 0
#define ANCS_CMD_ID_GET_NOTIF_ATTR 0
#define ANCS_CMD_ID_PERFORM_NOTIF_ACTION 2

#define ANCS_ID_ACT_POS 0
#define ANCS_ID_ACT_NEG 1

enum
{
  ANCS_ATTR_APP_ID,

  // The following 3 Attribute IDs are followed by a 2 byte max length parameter
  ANCS_ATTR_TITLE,
  ANCS_ATTR_SUBTITLE,
  ANCS_ATTR_MESSAGE,

  ANCS_ATTR_MSG_SIZE,
  ANCS_ATTR_DATE,
  ANCS_ATTR_POS_ACT_LABEL,
  ANCS_ATTR_NEG_ACT_LABEL
};
#endif

typedef struct
{
  /// Header structure.
  wsfMsgHdr_t           hdr;
  /// Signal parameter / user data.
  void                 *pParams;
} BleTask_Evt_t;


/******************************************************************************\
 *  Exported protected global variables
\******************************************************************************/

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

void BLE_Init(void);

#endif // BLE_H_
