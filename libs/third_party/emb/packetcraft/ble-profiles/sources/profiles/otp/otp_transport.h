#ifndef OTPC_TRANSPORT_H
#define OTPC_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dm_api.h"
#include "wsf_types.h"

#define OTS_LOCAL_MTU                           128
#define OTS_LOCAL_MPS                           23

typedef struct
{
  uint16_t          length;
  uint8_t           *data;
} otsL2capPacket_t;

typedef struct {
  uint8_t           status;
  uint16_t          cid;
  otsL2capPacket_t  packet;
}otsData_t;

typedef void (*otsTransCback_t)(uint16_t connId, uint8_t eventId, uint8_t* pParm);

/*************************************************************************************************/
/*!
 *  \brief  The API to write to peer device
 *
 *  \param  cid       conn id
 *          len       length of tx data
 *          pPayload  pointer to the tx data
 */
 /*************************************************************************************************/
void OtsTransportWriteReq(uint16_t cid, uint16_t len, uint8_t* pPayload);

/*************************************************************************************************/
/*!
 *  \brief  The API to provide the l2cap mtu
 *
 *  \param  connId    connection id
 *
 *  \return MTU.
 */
 /*************************************************************************************************/
uint16_t OtsTransportGetTxMTU(uint16_t cid);

/*************************************************************************************************/
/*!
 *  \brief  The API to close l2cap channel
 *
 *  \param  cid       conn id
 */
 /*************************************************************************************************/
uint16_t OtsTransportCloseReq(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  The API to open l2cap channel
 *
 *  \param  cid       conn id
 */
 /*************************************************************************************************/
uint16_t OtsTransportConnectReq(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  The Init function of OtsTransport
 *
 *  \param  role      L2C_COC_ROLE_INITIATOR or L2C_COC_ROLE_ACCEPTOR
 *  \param  transCb   Callback function
 *
 *  \return None.
 */
 /*************************************************************************************************/
void OtsTransportInit(uint8_t role, otsTransCback_t transCb);

#ifdef __cplusplus
};
#endif

#endif
