/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      DAA (Detect And Avoid) interface file.
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

#ifndef DAA_API_H
#define DAA_API_H

#include "ll_api.h"
#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief   DAA mode. */
enum
{
  LL_DAA_SLOW_MODE,         /*!< DAA slow mode. */
  LL_DAA_FAST_MODE          /*!< DAA fast mode. */
};
typedef uint8_t DaaMode_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize DAA EDS.
 */
/*************************************************************************************************/
void DaaEdsInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Start DAA EDS.
 *
 *  \param      durUsec     Scan duration (single channel) in microseconds.
 *  \param      chanMask    Channels to scan.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t DaaEdsStart(uint32_t durUsec, uint64_t chanMask);

/*************************************************************************************************/
/*!
 *  \brief      Stop DAA EDS.
 */
/*************************************************************************************************/
void DaaEdsStop(void);

/*************************************************************************************************/
/*!
 *  \brief      Reads current DAA EDS results.
 *
 *  \param      antId     Antenna ID.
 *  \param      pEdsData  EDS data buffer (must store LL_NUM_CHAN bytes).
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t DaaEdsGetCurrentAssessment(uint8_t antId, int32_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Initialize DAA CL.
 */
/*************************************************************************************************/
void DaaClosedLoopInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Start DAA closed loop.
 *
 *  \param      handle          Connection handle.
 *  \param      mode            DAA mode.
 *  \param      updateIntervMs  DAA update interval
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t DaaClosedLoopStart(uint16_t handle, DaaMode_t mode, uint32_t updateIntervMs);

/*************************************************************************************************/
/*!
 *  \brief      Stop DAA closed loop.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t DaaClosedLoopStop(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief      Avoid Wi-Fi channels.
 *
 *  \param      pBuf          Array of EDS values.
 *  \param      pChanMap      Pointer to channel map.
 *
 *  \return     Number of BTLE channels removed from the channel map
 */
/*************************************************************************************************/
uint8_t DaaAvoidWifiChannels(int8_t *pBuf, uint64_t *pChanMap);

#ifdef __cplusplus
};
#endif

#endif /* DAA_API_H */
