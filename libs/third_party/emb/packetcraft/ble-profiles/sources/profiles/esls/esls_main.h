#ifndef ESL_MAIN_H
#define ESL_MAIN_H
#include <stdint.h>

#include "dm_api.h"

#include "esls_api.h"
#include "svc_esl.h"


#define ESL_ADDRESS_ID_BITMASK 0xFF   /*!< ESL ID bitmask */
#define ESL_GROUP_ID_BITMASK   0x7F   /*!< Group ID bitmask */
#define ESL_RFU_BITMASK        0x80    /*!< Reserved for future use bitmask */

#define ESL_MAX_TLV_SIZE  17  /*!< Maximum Tag Length Value size */
#define ESL_MIN_TLV_SIZE  2   /*!< Minimum Tag Length Value size */

#define ESL_BASIC_STATE_RSP_LEN 2

#define ESL_OP_PING_LEN                 0x02U  /*!< Ping Operation size */
#define ESL_OP_UNASSOCIATE_LEN          0x02U  /*!< Unassociate Operation size */
#define ESL_OP_SERVICE_RESET_LEN        0x02U  /*!< Service Reset Operation size */
#define ESL_OP_FACTORY_RESET_LEN        0x02U  /*!< Factory Reset Operation size */
#define ESL_OP_UPDATE_COMPLETE_LEN      0x02U  /*!< Update Complete Operation size */
#define ESL_OP_READ_SENSOR_DATA_LEN     0x03U  /*!< Read Sensor Data Operation size */
#define ESL_OP_REFRESH_DISPLAY_LEN      0x03U  /*!< Refresh Display Operation size */
#define ESL_OP_DISPLAY_IMAGE_LEN        0x04U  /*!< Display Image Operation size */
#define ESL_OP_DISPLAY_TIMED_IMAGE_LEN  0x08U  /*!< Display Timed Image Operation size */
#define ESL_OP_LED_CTRL_LEN             0x0AU  /*!< LED Control Operation size */
#define ESL_OP_LED_TIMED_CTRL_LEN       0x0EU  /*!< LED Timed Control Operation size */

#define ESL_MAX_OP_QUEUE_SIZE  (ESL_CONTROL_POINT_QUEUE_LEN)  /*!< Maximum Operation Queue size */

/* ESL Address */
typedef struct
{
  uint8_t       eslId;      /*!< ESL ID */
  uint8_t       groupId;    /*!< Group ID */
  uint8_t       rfu;        /*!< Reserved for future use */
} eslsAddress_t;

typedef struct
{
  uint8_t       sessionKey[DM_EAD_SHARE_KEY_LEN];   /*!< Shared session key */
  uint8_t       iv[DM_EAD_IV_LEN];                  /*!< Initialization Vector */
} eslsKeyMaterial_t;

typedef struct
{
  uint8_t       sensorIdx;  /*!< Sensor Index */
} eslsReadSensorDataOpParams_t;

typedef struct
{
  uint8_t       displayIdx;   /*!< Display Index */
} eslsRefreshDisplayOpParams_t;

typedef struct
{
  uint8_t       displayIdx;   /*!< Display Index */
  uint8_t       imageIdx;     /*!< Image Index */
} eslsDisplayImageOpParams_t;

typedef struct
{
  uint8_t       displayIdx;   /*!< Display Index */
  uint8_t       imageIdx;     /*!< Image Index */
  uint32_t      absTime;      /*!< Time when display changes state */
} eslsDisplayTimedImageOpParams_t;

typedef struct
{
  uint8_t             idx;       /*!< LED Index */
  eslsLedConfig_t     config;    /*!< LED Configuration */
} eslsLedControlOpParams_t;


typedef struct
{
  uint8_t             ledIdx;       /*!< LED Index */
  eslsLedConfig_t     config;    /*!< LED Configuration */
  uint32_t            absTime;      /*!< Time when LED changes state */
} eslsLedTimedControlOpParams_t;

typedef struct
{
  uint8_t       vendorSpecificData[ESL_MAX_TLV_SIZE];  /*!< Vendor Specific Data */
} eslsVendorSpecificOpParams_t;

typedef union
{
  eslsReadSensorDataOpParams_t readSensorData;        /*!< Read Sensor Data Operation Parameters */
  eslsRefreshDisplayOpParams_t refreshDisplay;        /*!< Refresh Display Operation Parameters */
  eslsDisplayImageOpParams_t displayImage;            /*!< Display Image Operation Parameters */
  eslsDisplayTimedImageOpParams_t displayTimedImage;  /*!< Display Timed Image Operation Parameters */
  eslsLedControlOpParams_t ledControl;                /*!< LED Control Operation Parameters */
  eslsLedTimedControlOpParams_t ledTimedControl;      /*!< LED Timed Control Operation Parameters */
  eslsVendorSpecificOpParams_t vendorSpecific;        /*!< Vendor Specific Operation Parameters */
} opParams_t;

typedef struct
{
  uint8_t       opCode;   /*!< Operation Code */
  uint8_t       eslId;    /*!< ESL ID */
  opParams_t    opParams; /*!< Operation Data */
  bool          pending;  /*!< Timed Operation Pending for execution*/
} eslsOp_t;

// Define the structure for a queue node
typedef struct eslsOpQueueNode
{
  eslsOp_t op;
  struct eslsOpQueueNode *next;
} eslsOpQueueNode_t;

// Define the structure for the queue
typedef struct
{
  eslsOpQueueNode_t *front;
  eslsOpQueueNode_t *rear;
} eslsOpQueue_t;

typedef enum{
  ESL_STATE_UNASSOCIATED,       /*!< Unassociated State */
  ESL_STATE_CONFIGURED,         /*!< Configured State */
  ESL_STATE_SYNCHRONIZED,       /*!< Synchronized State */
  ESL_STATE_UPDATING,           /*!< Updating State */
  ESL_STATE_FACTORY_RESET_OP    /*!< Factory Reset Operation State */
} eslsState_t;

typedef void EslsAppCback_t(dmConnId_t connId, uint8_t operation, uint8_t *pBuf, uint16_t len);

/* Control block */
typedef struct
{
  eslsAddress_t       address;                            /*!< ESL Address */
  eslsKeyMaterial_t   apSyncKey;                          /*!< AP Sync Key Material */
  eslsKeyMaterial_t   responseKey;                        /*!< Response Key Material */
  uint32_t            currentTime;                        /*!< Current Absolute Time */
  eslsOpQueue_t       opQueue;                            /*!< Operation Queue */
  eslsOpQueue_t       timedOpQueue;                       /*!< Timed Operation Queue */
  uint16_t            prepWriteLength;                    /*!< Length of the current long write */
  uint8_t             eslCcdIdx;                          /*!< Control Point CCD Handle Index */
  eslsState_t         state;                              /*!< State Machine for ESL */
  bool                serviceNeededFlag;                  /*!< Service Needed Flag */;
  bool                serviceNeededState;                 /*!< Service Needed State */;
  dmConnId_t          connId;                             /*!< Connection ID */
  EslsAppCback_t*     appCback;                           /*!< Callback function */
  eslsLedConfig_t     ledInfo[ESL_NUM_LEDS];         /*!< LED Control Parameters */
} eslsCb_t;

/*************************************************************************************************/
/*!
 *  \brief  Handle Write request for the ESL Address characteristic.
 *
 *  \param[in]  connId    DM connection identifier.
 *  \param[in]  offset    Write offset.
 *  \param[in]  len       Write length.
 *  \param[in]  pValue    Value to write.
 *  \param[out] pAttr     Attribute to write.
 *  \param[in]  operation ATT operation.
 *  \param[in]  paddress Pointer to the ESL Address structure.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsWriteAddress(dmConnId_t connId, uint16_t offset, uint16_t len, uint8_t *pValue,
    attsAttr_t *pAttr, uint8_t operation, eslsAddress_t *paddress);

/*************************************************************************************************/
/*!
 *  \brief  Handle Write requests for Response Key Material and Sync Key Material
 *
 *  \param[in]  connId              DM connection identifier.
 *  \param[in]  offset              Write offset.
 *  \param[in]  len                 Write length.
 *  \param[in]  pValue              Value to write.
 *  \param[out] pAttr               Attribute to write.
 *  \param[in]  operation           ATT operation.
 *  \param[in]  pKeyMaterial        Pointer to the Key Material structure.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsWriteKeyMaterial(dmConnId_t connId, uint16_t handle, uint16_t offset, uint16_t len,
  uint8_t *pValue, attsAttr_t *pAttr, uint8_t operation, eslsKeyMaterial_t *pKeyMaterial);

/*************************************************************************************************/
/*!
 *  \brief  Handle Write request for the Current Absolute Time characteristic.
 *
 *  \param[in]  connId              DM connection identifier.
 *  \param[in]  offset              Write offset.
 *  \param[in]  len                 Write length.
 *  \param[in]  pValue              Value to write.
 *  \param[out] pAttr               Attribute to write.
 *  \param[in]  operation           ATT operation.
 *  \param[in]  pCurrentTime        Pointer to the current type value in the ESL control block.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsWriteAbsoluteTime(dmConnId_t connId, uint16_t offset, uint16_t len, uint8_t *pValue,
    attsAttr_t *pAttr, uint8_t operation, uint32_t *pCurrentTime);

/*************************************************************************************************/
/*!
 *  \brief  Handle Write requests for the ESL Control Point characteristic.
 *
 *  \param[in]  connId              DM connection identifier.
 *  \param[in]  handle              Attribute handle.
 *  \param[in]  operation           ATT operation.
 *  \param[in]  offset              Write offset.
 *  \param[in]  len                 Write length.
 *  \param[in]  pValue              Value to write.
 *  \param[out] pAttr               Attribute to write.
 *  \param[in]  eslsCb              Pointer to the ESL control block.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t EslsWriteOpCode(dmConnId_t connId, uint16_t handle, uint16_t offset, uint16_t len, uint8_t *pValue,
    attsAttr_t *pAttr, uint8_t operation, eslsCb_t *eslsCb);

/*************************************************************************************************/
/*!
 *  \brief  Notify the response to the AP.
 *
 *  \param  connId    DM connection identifier.
 *  \param  buf       Pointer to the buffer.
 *  \param  len       Length of the buffer.
 *  \param  cccIdx    Index of the CCC descriptor.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EslsNotifyResponse(dmConnId_t connId, uint8_t* buf, uint16_t len, uint16_t cccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Handle Unassociate operation.
 */
/*************************************************************************************************/
void EslsUnassociate(void);

/*************************************************************************************************/
/*!
 *  \brief  Handle Factory Reset operation.
 */
/*************************************************************************************************/
void EslsFactoryReset(void);

/*************************************************************************************************/
/*!
 * \brief   Handle the operation queue
 *
 * \param   connId - DM connection ID
 * \param   queue - pointer to the operation queue
 * \param   pCb - pointer to the ESL Control Block
 */
/*************************************************************************************************/
void EslsHandleOpQueue(eslsOpQueue_t *queue, eslsCb_t *pCb);

/*************************************************************************************************/
/*!
 * \brief   Handle the timed operation queue
 *
 * \param   queue - pointer to the timed operation queue
 */
/*************************************************************************************************/
void EslsHandleTimedOpQueue(eslsOpQueue_t *queue, eslsCb_t *pCb);

void utilityDumpData(uint8_t* pInput, uint16_t size);
#endif /* ESL_MAIN_H */
