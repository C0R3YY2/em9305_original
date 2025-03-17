/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Radio extension interface file.
 *
 *  Copyright (c) 2024 EM Microelectronic. All Rights Reserved.
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

#ifndef PAL_RADIO_EXT_H
#define PAL_RADIO_EXT_H

#include "pal_types.h"
#include <emb_dm_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_RADIO
 *  \{ */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
#if (USE_EMB_EXTENSIONS_DM)
/*************************************************************************************************/
/*!

 *  \brief      Set the absolute minimum and maximum Tx power to be used by the radio.
 *
 *  \param      minTxPwr    Minimum Transmit power that can be used in dBm.
 *  \param      maxTxPwr    Maximum Transmit power that can be used in dBm.
 *
 */
/*************************************************************************************************/
void PalRadioSetMinMaxTxPower(int8_t minTxPwr, int8_t maxTxPwr);
#endif //USE_EMB_EXTENSIONS_DM

/*! \} */    /* PAL_RADIO */

#ifdef __cplusplus
};
#endif

#endif /* PAL_RADIO_EXT_H */
