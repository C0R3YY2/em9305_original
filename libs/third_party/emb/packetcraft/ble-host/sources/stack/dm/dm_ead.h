#ifndef DM_EAD_H
#define DM_EAD_H

#include "dm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! AES-128 block size in bytes */
#define DM_EAD_SEC_TOOL_AES_BLOCK_SIZE       16

/*! Nonce size for Ead CCM operations is fixed: 13 bytes */
#define DM_EAD_SEC_TOOL_CCM_NONCE_SIZE       13

/*! Maximum CCM MAC size for Ead Security operations */
#define DM_EAD_SEC_TOOL_CCM_MAX_MAC_SIZE     4


/*! Request queue size for AES-CCM requests */
#ifndef DM_EAD_SEC_TOOL_CCM_REQ_QUEUE_SIZE
#define DM_EAD_SEC_TOOL_CCM_REQ_QUEUE_SIZE   1
#endif

/*! Request queue size for Kx derivation requests */
#ifndef DM_EAD_SEC_TOOL_KX_REQ_QUEUE_SIZE
#define DM_EAD_SEC_TOOL_KX_REQ_QUEUE_SIZE    6
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Ead Security Toolbox return value type. See ::eadReturnValues */
typedef uint16_t dmEadSecToolRetVal_t;

/*! Ead Security CCM operation types */
enum dmEadSecToolCcmOperationType
{
  DM_EAD_SEC_TOOL_CCM_ENCRYPT = 0x00, /*!< Encrypt operation */
  DM_EAD_SEC_TOOL_CCM_DECRYPT = 0x01, /*!< Decrypt operation */
};

/*! Ead Security CCM operation type. See ::eadSecToolCcmOperationType */
typedef uint8_t dmEadSecToolCcmOperation_t;

/*************************************************************************************************/
/*!
 *  \brief  dmEadEnCrData_t for input API.
 *
 *  \param  pData           Pointer to text to be Decrypted.
 *  \param  pKey            Pointer to encryption key (16 bytes).
 *  \param  pIv             Pointer to encryption iv (8 bytes).
 *  \param  len             Length of pIn in bytes.
 *
 */
 /*************************************************************************************************/
typedef struct
{
  uint8_t* pData;
  uint8_t* pKey;
  uint8_t* pIv;
  uint16_t len;
} dmEadEnCrData_t;

/*! Ead Security Toolbox CCM request parameter structure */
typedef struct dmEadSecToolCcmParams_tag
{
  uint8_t* pIn;            /*!< Pointer to input buffer */
  uint8_t* pOut;           /*!< Pointer output buffer */
  uint8_t* pAuthData;      /*!< Pointer to authentication data */
  uint8_t* pCbcMac;        /*!< Pointer to CBC-MAC in/out buffer */
  uint8_t* pCcmKey;        /*!< Pointer to 128-bit AES CCM Key */
  uint8_t* pNonce;         /*!< 13-byte Nonce for counter */
  uint16_t inputLen;       /*!< Input/Output buffer length */
  uint16_t authDataLen;    /*!< Authentication data length */
  uint8_t  cbcMacSize;     /*!< Size of the CBC-MAC */
} dmEadSecToolCcmParams_t;

/*! Ead Security CCM Encrypt operation result */
typedef struct dmEadSecToolCcmEncryptResult_tag
{
  uint8_t* pCipherText;    /*!< Pointer to buffer storing the ciphertext
                            *   (passed as pOut in the request)
                            */
  uint16_t cipherTextSize; /*!< Size of the ciphertext */
  uint8_t* pCbcMac;        /*!< Pointer to buffer storing the CBC-MAC calculation
                            *   (passed as parameter in the request)
                            */
  uint8_t  cbcMacSize;     /*!< Size in bytes of the CBC-MAC */
} dmEadSecToolCcmEncryptResult_t;

/*! Ead CCM Decrypt operation result */
typedef struct dmEadSecToolCcmDecryptResult_tag
{
  uint8_t* pPlainText;    /*!< Pointer to buffer storing the plaintext
                            *   (passed as pOut in the request)
                            */
  uint16_t plainTextSize;  /*!< Size of the plaintext */
  bool     isAuthSuccess;  /*!< true if PDU is authenticated */
} dmEadSecToolCcmDecryptResult_t;

typedef union dmEadSecToolCcmResults_tag
{
  dmEadSecToolCcmEncryptResult_t encryptResult;  /*!< Encryption result */
  dmEadSecToolCcmDecryptResult_t decryptResult;  /*!< Decryption result */
} dmEadSecToolCcmResults_t;

/*! Ead CCM operation result */
typedef struct edmEadSecToolCcmResult_tag
{
  dmEadSecToolCcmOperation_t op;      /*!< Operation identifier */
  dmEadSecToolCcmResults_t   results; /*!< Results union */
} dmEadSecToolCcmResult_t;

typedef void (*dmEadOperationCb)(dmEadSecToolCcmResult_t* pCcmResult);

/*************************************************************************************************/
/*!
 *  \brief     Callback for AES-CCM operation.
 *
 *  \param[in] pCmacResult  Pointer to CCM response structure.
 *  \param[in] pParam       Pointer to generic parameter provided in request.
 *
 *  \return    None.
 *
 *  \see       eadSecToolCcmResult_t
 */
 /*************************************************************************************************/
typedef void (*dmEadSecToolCcmCback_t)(dmEadSecToolCcmResult_t* pCcmResult, void* pParam);


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief     Ead Security Toolbox AES-CMAC primitive.
 *
 *  \param[in] pKey          Pointer to 128-bit AES Key.
 *  \param[in] pPlainText    Pointer to plain text array.
 *  \param[in] plainTextLen  Length of the plain text.
 *  \param[in] cmacCback     Callback invoked after CMAC calculation is complete.
 *  \param[in] pParam        Pointer to generic parameter for the callback.
 *
 *  \see       eadSecToolCmacCback_t
 *
 *  \retval    DM_EAD_SUCCESS                  Request processed successfully.
 *  \retval    DM_EAD_SEC_TOOL_INVALID_PARAMS  Invalid parameters in request.
 *  \retval    DM_EAD_SEC_TOOL_OUT_OF_MEMORY   No resources to process request.
 *  \retval    DM_EAD_SEC_TOOL_UNINITIALIZED   The algorithm used by this request is not initialized.
 *  \retval    DM_EAD_SEC_TOOL_UNKNOWN_ERROR   An error occurred in the PAL layer.
 *
 *  \remarks   The caller should not overwrite the memory referenced by input pointers until the
 *             callback is triggered.
 */
 /*************************************************************************************************/
dmEadSecToolRetVal_t dmEadSecToolCcmEncryptDecrypt(dmEadSecToolCcmOperation_t opType,
  dmEadSecToolCcmParams_t* pOpParams,
  dmEadSecToolCcmCback_t ccmCback, void* pParam);

enum dmEadReturnValues
{

  /*! General return value base */
  DM_EAD_RETVAL_BASE = 0x0000,
  DM_EAD_SEC_TOOL_RETVAL_BASE = 0x0100,
  /*! Operation completed without errors */
  DM_EAD_SUCCESS = 0x0000,
  /*! A general failure.  A more specific reason may be defined within event. */
  DM_EAD_FAILURE = 0x0001,
  DM_EAD_SEC_TOOL_INVALID_PARAMS = DM_EAD_SEC_TOOL_RETVAL_BASE + 0x0001,
  DM_EAD_SEC_TOOL_OUT_OF_MEMORY,
  DM_EAD_SEC_TOOL_UNINITIALIZED,
  DM_EAD_SEC_TOOL_UNKNOWN_ERROR
};


/*************************************************************************************************/
/*!
 *  \brief  DM provides API for application to encrpt data.
 *
 *  \param  pParm           Pointer to dmEadEnCrData_t of input parameters.
 *  \param  cb              parameter passed in complete event.
 *
 *  \return true if successful, else false.
 */
 /*************************************************************************************************/
bool DmEadEnccryption(dmEadEnCrData_t* pParm, void* cb);

/*************************************************************************************************/
/*!
 *  \brief  DM provides API for application to decrpt data.
 *
 *  \param  pParm           Pointer to dmEadEnCrData_t of input parameters.
 *  \param  cb              parameter passed in complete event.
 *
 *  \return true if successful, else false.
 */
 /*************************************************************************************************/
bool DmEadDecryption(dmEadEnCrData_t* pParm, void* cb);

#ifdef __cplusplus
};
#endif

#endif /* DM_EAD_H */
