Overview
--------

The EMB Database provides a way to persistently store BLE key material exchanged during pairing. The keys are stored in NVM Info Page 1 by default and persist across power cycles. It is designed to securely store keys when the devices resets for any reason including thermal shutdown.

Example Usage
-------------
.. warning::

    The EMB Database is not designed to be used with the Packetcraft Application Framework profile (AF). The AF has a RAM database that stores similar device records which do not persist across power cycles.

.. warning::

    This example is not to a complete application. It is only meant to point the user in the right direction to use this library.

The user application is responsible for handling receiving keys from the stack in the app and passing them to the EMB Database API. The user must initialize the record first, before handling key info from the DM event, storing into the record, before passing to the EMB Database API.

From a high-level perspective, this code snippet does the following:

- Creates `EMBDB_Record_t` (pairing info) in RAM and populates various key fields during pairing.

- Stores pairing info in NVM after pairing completes successfully.

- Loads pairing info from NVM when the BLE stack initializes.

- Accepts encryption requests from the peer device if that peer has previously paired.

- Pairing info persists after power on resets.

.. warning::

    Depending on the user application's security needs, it may be necessary to perform additional security checks on the peer device before accepting LTK requests.

.. code-block:: c

    #include "emb_database_api.h"

    // Database record in RAM to be stored in NVM after pairing completes.
    static EMBDB_Record_t pairingInfo;
    // Connection ID to be stored when a connection is established.
    static dmConnId_t     connId;
    // Storage backend for EMB Database to use. In this example, NVM Info P1 is used.
    static EMBDB_StorageBackend_t storageBackend;

    static void getReceivedKeys(dmEvt_t *pDmEvt)
    {
        // The emb_database has fields for each type of BLE key exchanged during
        // pairing (LTK, IRK, and CSRK).
        switch (pDmEvt->keyInd.type)
        {
            case DM_KEY_LOCAL_LTK:
                pairingInfo.localLtk = pDmEvt->keyInd.keyData.ltk;
                pairingInfo.localLtkSecLevel = pDmEvt->keyInd.secLevel;
                pairingInfo.localLtkValid = 1;
                break;

            case DM_KEY_PEER_LTK:
                pairingInfo.peerLtk = pDmEvt->keyInd.keyData.ltk;
                pairingInfo.peerLtkSecLevel = pDmEvt->keyInd.secLevel;
                pairingInfo.peerLtkValid = 1;
                break;

            case DM_KEY_IRK:
                pairingInfo.peerIrk = pDmEvt->keyInd.keyData.irk;
                BdaCpy(pairingInfo.peerIrk.bdAddr, pDmEvt->keyInd.keyData.irk.bdAddr);
                pairingInfo.peerIrkValid = 1;
                break;

            case DM_KEY_CSRK:
                pairingInfo.peerCsrk = pDmEvt->keyInd.keyData.csrk;
                pairingInfo.peerCsrkSecLevel = pDmEvt->keyInd.secLevel;
                pairingInfo.peerCsrkValid = 1;
                break;

            default:
                break;
        }
    }

    // This function is called by the user application's message handler when a
    // Device Manager (DM) event has been received. The DM communicates back to
    // the user application by sending various events. In the emb_fit example,
    // the application's message handler is APP_FitHandler.
    void userDMEventHandler(dmEvt_t *pDmEvt)
    {
        switch (pDmEvt->hdr.event)
        {
          case DM_RESET_CMPL_IND:
              // Initialize the EMB Database to use NVM Info P1 backend (default).
              // By default EMBDB_LoadNVMInfoP1Backend sets the maxRecords to 8,
              // but this can be changed to a maximum of 69 records if desired.
              // storageBackend.maxRecords = 69;
              EMBDB_LoadNVMInfoP1Backend(&storageBackend);
              EMBDB_RegisterBackend(&storageBackend);
              // Load the stored pairing info if it exists, otherwise create empty record.
              if (EMB_DB_GET_RECORD_SUCCESS == EMBDB_GetLastRecordStored(&pairingInfo))
              {
                  // Could setup device filtering and IRK resolution here.
              }
              else
              {
                  EMBDB_InitializeRecord(&pairingInfo);
              }
              break;

          case DM_CONN_OPEN_IND:
              connId = (dmConnId_t)pDmEvt->hdr.param;
              break;

          case DM_SEC_KEY_IND:
              getReceivedKeys(pDmEvt);
              break;

          case DM_SEC_PAIR_CMPL_IND:
              EMBDB_StoreRecord(&pairingInfo);
              break;

          // This example showcases using the stored LTK for encrypting the connection.
          // A similar process is applied for the other BLE keys.
          case DM_SEC_LTK_REQ_IND:
              if (pairingInfo.localLtkValid == 1)
              {
                  // Key has already been exchanged and stored.
                  DmSecLtkRsp(connId, TRUE, pairingInfo.localLtkSecLevel, pairingInfo.localLtk.key);
              }
              else
              {
                  // Key not found.
                  DmSecLtkRsp(connId, FALSE, 0, NULL);
              }
              break;

          default:
              break;

        };
    }

.. warning::

    `recordCrc` must be the last field in the EMBDB_Record_t structure.

.. warning::

    It is forbidden to store an empty record. `EMBDB_StoreRecord` will return a failure status if attempted.

Changing Storage Locations
--------------------------

The location in which the database records are stored is configurable by changing
the read/write backend of the database. This can be done using the `EMBDB_RegisterBackend`
API. in the example above, records are stored in NVM Info page 1, but can be changed to store
in different locations.

Below is an example of how to tell the EMB Database to store records in DRAM.

.. code-block:: c

    #include "emb_database_api.h"
    #include "errno.h"

    // We are 8 using storage records here, but this can be configured with respect
    // to available space in the storage backend. If using NVM, the maximum is
    // 69 records using database v1.0 as this is the number of records that will
    // fit in an 8kB page.
    #define EMB_DB_MAX_RECORDS 8U 

    // DRAM storage implemented using an array.
    static EMBDB_Record_t databaseDRAM[EMB_DB_MAX_RECORDS];
    // Storage backend for EMB Database to use. In this example, DRAM is used.
    static EMBDB_StorageBackend_t backendDRAM;

    // Function to erase the data. Data is only erased when requested by the user.
    static int32_t eraseDbDRAM(void)
    {
        memset(databaseDRAM, 0x00U, sizeof(databaseDRAM));

        return 0;
    }

    // Function to invalidate records.
    static int32_t invalidateRecordAtAddressDRAM(EMBDB_Record_t *address)
    {
        address->metadata.recordValid = EMB_DB_RECORD_INVALID;

        return 0;
    }

    // Function to write records into the DRAM array.
    static int32_t writeRecordAtAddressDRAM(const EMBDB_Record_t *record, const void *storeAddress)
    {
        EMBDB_Record_t *pRecord = databaseDRAM;
        int32_t         status  = -ENOSPC;

        for (uint8_t i = 0U; i < EMB_DB_MAX_RECORDS; i++)
        {
            if (storeAddress == pRecord)
            {
                databaseDRAM[i] = *record;
                status          = 0;
                break;
            }
            pRecord++;
        }

        return status;
    }

    // In user function where database is to be initialized...
    static void userInitializeBondingDatabase(void)
    {
        memset(databaseDRAM, 0x00U, sizeof(databaseDRAM));

        backendDRAM.storageStartAddress       = databaseDRAM;
        backendDRAM.databaseSize              = EMB_DB_MAX_RECORDS * sizeof(EMBDB_Record_t);
        backendDRAM.writeRecordAtAddress      = writeRecordAtAddressDRAM;
        backendDRAM.invalidateRecordAtAddress = invalidateRecordAtAddressDRAM;
        backendDRAM.eraseDatabase             = eraseDbDRAM;
        backendDRAM.erasedValue               = 0x00000000;
        backendDRAM.maxRecords                = EMB_DB_MAX_RECORDS;

        // Check the registerStatus is 0 to ensure backendDRAM structure is setup
        // correctly and the read/write location is suitable for the EMB Database.
        // This function may return -ENXIO if the storage location is unknown to
        // the SoC, but the backend may still work.
        int32_t validationStatus = EMBDB_ValidateBackend(&backendDRAM);

        // If this status is 0, one of the function pointers are NULL.
        int32_t registerStatus = EMBDB_RegisterBackend(&backendDRAM);
    }

