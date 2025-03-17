/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal diagnostics scheduler interface file.
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

#ifndef SCH_DIAG_H
#define SCH_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Reader friendly event triggered trace log.
 *
 *  \param      curTime   Current time.
 *  \param      state     State.
 *  \param      event     Event.
 */
/*************************************************************************************************/
static inline void schTraceEventTriggered(uint32_t curTime, uint8_t state, uint8_t event)
{
  switch (state)
  {
  case SCH_STATE_IDLE:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=IDLE    event=EXECUTE  curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=IDLE    event=COMPLETE curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=IDLE    event=ABORT    curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=IDLE    event=RELOAD   curTime=%u", curTime);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_LOAD:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=LOAD    event=EXECUTE  curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=LOAD    event=COMPLETE curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=LOAD    event=ABORT    curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=LOAD    event=RELOAD   curTime=%u", curTime);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_EXECUTE:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=EXECUTE event=EXECUTE  curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=EXECUTE event=COMPLETE curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=EXECUTE event=ABORT    curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=EXECUTE event=RELOAD   curTime=%u", curTime);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_UNLOAD:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=UNLOAD  event=EXECUTE  curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=UNLOAD  event=COMPLETE curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=UNLOAD  event=ABORT    curTime=%u", curTime);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    +++ SCH SM +++  state=UNLOAD  event=RELOAD   curTime=%u", curTime);
      break;
    default:
      break;
    }
    break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Reader friendly state/event trace log.
 *
 *  \param      pBod      BOD pointer.
 *  \param      state     State.
 *  \param      event     Event.
 */
/*************************************************************************************************/
static inline void schTraceStateEvent(void *pBod, uint8_t state, uint8_t event)
{
  switch (state)
  {
  case SCH_STATE_IDLE:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=IDLE    event=EXECUTE  pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=IDLE    event=COMPLETE pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=IDLE    event=ABORT    pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=IDLE    event=RELOAD   pBod=0x%08x", pBod);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_LOAD:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=LOAD    event=EXECUTE  pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=LOAD    event=COMPLETE pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=LOAD    event=ABORT    pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=LOAD    event=RELOAD   pBod=0x%08x", pBod);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_EXECUTE:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=EXECUTE event=EXECUTE  pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=EXECUTE event=COMPLETE pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=EXECUTE event=ABORT    pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=EXECUTE event=RELOAD   pBod=0x%08x", pBod);
      break;
    default:
      break;
    }
    break;

  case SCH_STATE_UNLOAD:
    switch (event)
    {
    case SCH_EVENT_BOD_EXECUTE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=UNLOAD  event=EXECUTE  pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_COMPLETE:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=UNLOAD  event=COMPLETE pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_ABORT:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=UNLOAD  event=ABORT    pBod=0x%08x", pBod);
      break;
    case SCH_EVENT_BOD_RELOAD:
      SCH_DBG_TRACE_INFO1("    /// SCH SM ///  state=UNLOAD  event=RELOAD   pBod=0x%08x", pBod);
      break;
    default:
      break;
    }
    break;
  }
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_DIAG_H */
