////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_rscps.c
///
/// @project    T9305
///
/// @brief      Running Speed and Cadence Profile Sensor
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-2024, EM Microelectronic-Marin SA
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

#include <wsf_types.h>
#include <att_api.h>
#include <dm_api.h>
#include <rscp/rscp_api.h>
#include <em_rscps.h>
#include <svc_rscs.h>
#include <types.h>
#include <wsf_os.h>
#include <wsf_timer.h>


/******************************************************************************\
 *  Types
\******************************************************************************/

/**
 * @brief RSC Connection Control Block.
 */
typedef struct
{
    /// Connection ID.
    dmConnId_t  connId;
    /// RSC Speed Measurement ready to be sent on this channel.
    bool        measureToSend;
} RscpsConn_t;


/******************************************************************************\
 *  Global variables
\******************************************************************************/

/**
 * @brief Running Speed and Cadence Profile Sensor Control Block.
 */
static struct
{
    /// Connections Control Block.
    RscpsConn_t conn[DM_CONN_MAX];
    /// Periodic Measurement Timer.
    wsfTimer_t  measTimer;
    /// Configurable parameters.
    RscpsCfg_t  cfg;
    /// Status (WALKING or RUNNING).
    uint8_t     status;
    /// Instantaneous Cadence.
    uint8_t     cadence;
    /// Instantaneous Speed.
    uint16_t    speed;
    /// Total Distance.
    uint32_t    distance;
    /// Instantaneous Stride Length.
    uint16_t    stride;
    /// true if ready to send notifications.
    bool        txReady;
} gRscpsCb;


/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/**
 * @brief Return true if no connections with active measurements.
 * @return true if no connections active.
 */
static bool RSCPS_NoConnActive(void);

/**
 * @brief Setup to send measurements on active connections.
 */
static void RSCPS_SetupToSend(void);

/**
 * @brief Find next connection with measurement to send.
 * @param cccIdx Running Speed and Cadence measurement CCC descriptor index.
 * @return Connection control block.
 */
static RscpsConn_t *RSCPS_FindNextToSend(uint8_t cccIdx);

/**
 * @brief Handle connection open.
 * @param pMsg Event message.
 */
static void RSCPS_ConnOpen(dmEvt_t *pMsg);

/**
 * @brief Handle a received ATT handle value confirm.
 * @param pMsg Event message.
 */
static void RSCPS_HandleValueCnf(attEvt_t *pMsg);

/**
 * @brief This function is called by the application when the periodic
 *        measurement timer expires.
 * @param pMsg Event message.
 */
static void RSCPS_MeasTimerExp(wsfMsgHdr_t *pMsg);

/**
 * @brief Perform a speed measurement.
 */
static void RSCPS_PerformSpeedMeasurement(void);

/**
 * @brief Send a speed measurement.
 * @param connId DM connection identifier.
 */
static void RSCPS_SendSpeedMeasurement(dmConnId_t connId);


/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static bool RSCPS_NoConnActive(void)
{
    RscpsConn_t *pConn = gRscpsCb.conn;
    for (uint8_t i = 0; i < DM_CONN_MAX; i++, pConn++)
    {
        if (pConn->connId != DM_CONN_ID_NONE)
        {
            return false;
        }
    }
    return true;
}

static void RSCPS_SetupToSend(void)
{
    RscpsConn_t *pConn = gRscpsCb.conn;
    for (uint8_t i = 0; i < DM_CONN_MAX; i++, pConn++)
    {
        if (pConn->connId != DM_CONN_ID_NONE)
        {
            pConn->measureToSend = true;
        }
    }
}

static RscpsConn_t *RSCPS_FindNextToSend(uint8_t cccIdx)
{
    RscpsConn_t *pConn = gRscpsCb.conn;
    for (uint8_t i = 0; i < DM_CONN_MAX; i++, pConn++)
    {
        if (pConn->connId != DM_CONN_ID_NONE && pConn->measureToSend)
        {
            if (AttsCccEnabled(pConn->connId, cccIdx))
            {
                return pConn;
            }
        }
    }
    return NULL;
}

static void RSCPS_ConnOpen(dmEvt_t *pMsg)
{
    gRscpsCb.txReady = true;
}

static void RSCPS_HandleValueCnf(attEvt_t *pMsg)
{
    if (pMsg->hdr.status == ATT_SUCCESS && pMsg->handle == RSCS_RSM_HDL)
    {
        gRscpsCb.txReady = true;

        // Find the next connection to send (note the CCC idx is stored in
        // timer status).
        RscpsConn_t *pConn = RSCPS_FindNextToSend(
            gRscpsCb.measTimer.msg.status);
        if (pConn)
        {
            RSCPS_SendSpeedMeasurement(pConn->connId);
            gRscpsCb.txReady = false;
            pConn->measureToSend = false;
        }
    }
}

static void RSCPS_MeasTimerExp(wsfMsgHdr_t *pMsg)
{
    // Check if there are active connections.
    if (!RSCPS_NoConnActive())
    {
        // Setup to send measurements on active connections.
        RSCPS_SetupToSend();

        // Perform a speed measurement.
        RSCPS_PerformSpeedMeasurement();

        // Check if ready to send measurements.
        if (gRscpsCb.txReady)
        {
            // Find the next connection to send (note the CCC idx is stored in
            // timer status).
            RscpsConn_t *pConn = RSCPS_FindNextToSend(pMsg->status);
            if (pConn)
            {
                RSCPS_SendSpeedMeasurement(pConn->connId);
                gRscpsCb.txReady = false;
                pConn->measureToSend = false;
            }
        }

        // Restart the timer.
        WsfTimerStartMs(&gRscpsCb.measTimer, gRscpsCb.cfg.period);
    }
}

static void RSCPS_PerformSpeedMeasurement(void)
{
    // Perform a measurement.
    // DEMO: Fixed values and toggle status (WALKING / RUNNING)
    gRscpsCb.speed      = 1;
    gRscpsCb.cadence    = 2;
    gRscpsCb.stride     = 3;
    gRscpsCb.distance   = 4;
    gRscpsCb.status     = gRscpsCb.status == RSCP_SM_STATUS_WALKING ?
        RSCP_SM_STATUS_RUNNING : RSCP_SM_STATUS_WALKING;
}

static void RSCPS_SendSpeedMeasurement(dmConnId_t connId)
{
    RscpsSetParameter(RSCP_SM_PARAM_SPEED,          gRscpsCb.speed);
    RscpsSetParameter(RSCP_SM_PARAM_CADENCE,        gRscpsCb.cadence);
    RscpsSetParameter(RSCP_SM_PARAM_STRIDE_LENGTH,  gRscpsCb.stride);
    RscpsSetParameter(RSCP_SM_PARAM_TOTAL_DISTANCE, gRscpsCb.distance);
    RscpsSetParameter(RSCP_SM_PARAM_STATUS,         gRscpsCb.status);

    RscpsSendSpeedMeasurement(connId);
}


/******************************************************************************\
 *  Public functions declarations
\******************************************************************************/

void RSCPS_Init(wsfHandlerId_t handlerId, RscpsCfg_t *pCfg)
{
    gRscpsCb.measTimer.handlerId = handlerId;
    gRscpsCb.cfg = *pCfg;

    // Set Running Speed and Cadence features.
    RscpsSetFeatures(RSCS_ALL_FEATURES);
}

void RSCPS_MeasStart(dmConnId_t connId, uint8_t timerEvt, uint8_t rscsmCccIdx)
{
    // Check if this is the first connection.
    if (RSCPS_NoConnActive())
    {
        // Initialize the control block.
        gRscpsCb.measTimer.msg.event = timerEvt;
        gRscpsCb.measTimer.msg.status = rscsmCccIdx;

        // start the timer.
        WsfTimerStartMs(&gRscpsCb.measTimer, gRscpsCb.cfg.period);
    }

    // Set the connection ID.
    gRscpsCb.conn[connId-1].connId = connId;
}

void RSCPS_MeasStop(dmConnId_t connId)
{
    // Clear connection.
    gRscpsCb.conn[connId-1].connId = DM_CONN_ID_NONE;
    gRscpsCb.conn[connId-1].measureToSend = false;

    // Check if there are no remaining connections.
    if (RSCPS_NoConnActive())
    {
        // Stop the timer.
        WsfTimerStop(&gRscpsCb.measTimer);
    }
}

void RSCPS_ProcMsg(wsfMsgHdr_t *pMsg)
{
    if (pMsg->event == DM_CONN_OPEN_IND)
    {
        RSCPS_ConnOpen((dmEvt_t*)pMsg);
    }
    else if (pMsg->event == ATTS_HANDLE_VALUE_CNF)
    {
        RSCPS_HandleValueCnf((attEvt_t*)pMsg);
    }
    else if (pMsg->event == gRscpsCb.measTimer.msg.event)
    {
        RSCPS_MeasTimerExp(pMsg);
    }
}
