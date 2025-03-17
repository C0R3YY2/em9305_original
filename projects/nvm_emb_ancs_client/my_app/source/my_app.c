////////////////////////////////////////////////////////////////////////////////
///
/// @file       app.c
///
/// @project    T9305
///
/// @brief      Application
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

#include <my_app_signals.h>
#include <my_app_task.h>
#include <ble_task.h>
#include <types.h>
#include <uart.h>
#include <puts.h>
#include <wsf_types.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <bsp.h>
#include <dm_api.h>
#include <dm_handler.h>

#include "my_app.h"
#include "..\..\ble\source\ble.h"

#define MESSAGE_DEBUGx

// The following zaps the printf() calls in this file for clarity
#ifdef MESSAGE_DEBUG
#include <printf.h>
#define PRINTF printf
#else
#define PRINTF(FORMAT, ...) ((void)0)
#endif

extern  MyApp_Cb_t gCb;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/******************************************************************************\
 *  Function prototypes
\******************************************************************************/
void UART_RxCallback(Driver_Status_t status, void* pUserData);
void Process_ANCS(ANCS_Evt_t *evt);
void Process_ANCS_InfoRsp(ANCS_Info_Evt_t *info);
static void ANCSRequestNotifTitle(void);
static void ANCSSendActionID(uint32_t targetUID, uint8_t action);

/*
 * Primary event handler
 *
 */
MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            PRINTF("\r\n\n\nANCS Demo - use the spacebar to answer or hang up\r\n\n");

            // Instruct the BLE task to advertise.
            BleTask_PostEvent(BLE_START_ADV_SIG, NULL);

             // Enable the ability to receive a single serial character
            UART_ReceiveDataWithCallback(&gCb.dataInput, 1, UART_RxCallback, NULL);
        }
        break;

        // Attached to this signal is a void* which contains the received UART character
        // from the ISR in the LSB.
        case MY_APP_UART_RXD_DATA:
        {
            // Extract the received character from the void* that came with the event
            uint8_t ch = ((uint32_t)pParams) & 0xFF;

            // If it is the keypress we care about, process it
            if(ch == BUTTON_PRESS_CODE)
            {
                if(gCb.connected == false)
                {
                    PRINTF("- Not connected to a host\r\n");
                }
                else
                {
                    switch(gCb.callState)
                    {
                        case MY_APP_CS_IDLE:
                            if(gCb.msgUID == EMPTY_UID)
                            {
                                PRINTF("- Nothing to do\r\n");
                            }
                            else
                            {
                                PRINTF("- Message dismissed\r\n");
                                ANCSSendActionID(gCb.msgUID, ANCS_ID_ACT_NEG);
                            }
                            break;

                        // Call management decisions
                        case MY_APP_CS_INCOMING_CALL:
                        case MY_APP_CS_ACTIVE_CALL:
                            {
                                if(gCb.callState == MY_APP_CS_ACTIVE_CALL)
                                {
                                    // Hang up the call
                                    ANCSSendActionID(gCb.msgUID, ANCS_ID_ACT_NEG);   // Action ID negative
                                }
                                else
                                {
                                    // Answer the call
                                    ANCSSendActionID(gCb.msgUID, ANCS_ID_ACT_POS);  // Action ID positive
                                }
                            }
                            break;

                        default:
                            break;
                    }
                }
            }
        }
        break;

        case MY_APP_ANCS_MSG:
            Process_ANCS((ANCS_Evt_t *)pParams);
            break;

        case MY_APP_ANCS_INFO_RSP:
            Process_ANCS_InfoRsp((ANCS_Info_Evt_t *)pParams);
            break;

        case MY_APP_ERROR_SIG:
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
            break;

        /* Add other cases to handle other application signals here. */
        default:
            // Unknown event!
            ret = MY_APP_EVT_ST_UNKNOWN;
            break;
    }

    return ret;
}

/*
 * This should be tight code since it is called from the harware ISR
 * handler of the UART. Send the event and the character and get out.
 *
 */
void UART_RxCallback(Driver_Status_t status, void* pUserData)
{
    if ((status == DRIVER_STATUS_RX) || (status == DRIVER_STATUS_OK))
    {
        // Send the character attached to an event for task-level processing
        uint32_t data = (uint32_t)gCb.dataInput;
        MyAppTask_PostEvent(MY_APP_UART_RXD_DATA, (void *)data);
    }

    // Re-prime the ability to receive another single character
    UART_ReceiveDataWithCallback(&gCb.dataInput, 1, UART_RxCallback, NULL);
}

/*
 * This is called from the GPIO edge triggered ISR. It generates the same message
 * as pressing the key on the keyboard sent through the UART did for testing.
 *
 */
void ButtonHandler(void)
{
    // Send a spacebar character event to mimic that input method
    uint32_t data = BUTTON_PRESS_CODE;
    MyAppTask_PostEvent(MY_APP_UART_RXD_DATA, (void *)data);
}

/*
 * Do something with the incoming ANCS message and be sure to free the buffer when done
 *
 */
void Process_ANCS(ANCS_Evt_t *evt)
{
    // Sanity check before we dereference it
    if(evt == NULL)
    {
        return;
    }

#ifdef MESSAGE_DEBUG
    static char *Events[] = { "Added", "Modified", "Removed" };
    PRINTF("Category: %d, %s, UID: %X\r\n", evt->categoryID, Events[evt->eventID], evt->notifUID);
#endif

    // Now decode added/removed events (no example code for modified events)
    switch(evt->categoryID)
    {
        // Process incoming call messages
        case ANCS_CATEGORY_ID_INCOMING_CALL:
            // Idle -> Incoming Call
            if((evt->eventID == ANCS_EVENT_ADDED) && (gCb.msgUID == EMPTY_UID))
            {
                PRINTF("\r\nIncoming call\r\n");
                gCb.callState = MY_APP_CS_INCOMING_CALL;
                gCb.msgUID = evt->notifUID;
                ANCSRequestNotifTitle();    // Ask for the phone number or matched name if in Contacts
            }

            // Incoming Call -> Idle
            if((evt->eventID == ANCS_EVENT_REMOVED) && (evt->notifUID == gCb.msgUID))
            {
                gCb.callState = MY_APP_CS_IDLE;
                gCb.msgUID = EMPTY_UID;
            }
            break;

        // This undocumented category actually tells you the call has been answered and provides
        // the UID that you can then use in a "negative action" GATT message to hang up.
        case ANCS_CATEGORY_ID_RESERVED:
            // Idle -> Answered Call
            if((evt->eventID == ANCS_EVENT_ADDED) && (gCb.msgUID == EMPTY_UID))
            {
                PRINTF("- Call answered\r\n");
                gCb.callState = MY_APP_CS_ACTIVE_CALL;
                gCb.msgUID = evt->notifUID;
            }

            // Answered Call -> Idle
            if((evt->eventID == ANCS_EVENT_REMOVED) && (gCb.msgUID == evt->notifUID))
            {
                if(gCb.callState == MY_APP_CS_ACTIVE_CALL)
                {
                    PRINTF("- Call ended\r\n");
                    gCb.callState = MY_APP_CS_IDLE;
                    gCb.msgUID = EMPTY_UID;
                }
            }
            break;

        // Text message, etc.
        case ANCS_CATEGORY_ID_SOCIAL:
            if((evt->eventID == ANCS_EVENT_ADDED) && (gCb.msgUID == EMPTY_UID))
            {
                PRINTF("\r\nText message\r\n");
                if(gCb.callState == MY_APP_CS_IDLE)
                {
                    gCb.msgUID = evt->notifUID;
                    ANCSRequestNotifTitle();    // As an example, ask for the sender
                }
            }

            if((evt->eventID == ANCS_EVENT_REMOVED) && (gCb.msgUID == evt->notifUID))
            {
                gCb.msgUID = EMPTY_UID;
            }
            break;

        // An alarm from the clock app will generate this event for easy testing
        case ANCS_CATEGORY_ID_OTHER:
            if((evt->eventID == ANCS_EVENT_ADDED) && (gCb.msgUID == EMPTY_UID))
            {
                PRINTF("\r\nOther event\r\n");
                if(gCb.callState == MY_APP_CS_IDLE)
                {
                    gCb.msgUID = evt->notifUID;
                    ANCSRequestNotifTitle();    // Ask for the title
                }
            }

            if((evt->eventID == ANCS_EVENT_REMOVED) && (gCb.msgUID == evt->notifUID))
            {
                gCb.msgUID = EMPTY_UID;
            }
            break;


        case ANCS_CATEGORY_ID_SCHEDULE:
        case ANCS_CATEGORY_ID_MISSED_CALL:
        case ANCS_CATEGORY_ID_VOICE_MAIL:
        case ANCS_CATEGORY_ID_EMAIL:
        case ANCS_CATEGORY_ID_NEWS:
        case ANCS_CATEGORY_ID_HEALTH_AND_FITNESS:
        case ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE:
        case ANCS_CATEGORY_ID_LOCATION:
        case ANCS_CATEGORY_ID_ENTERTAINMENT:
        default:
            break;
    }

    WsfBufFree(evt);    // Return the ANCS buffer to the pool
}

/*
 * Extract data from a request we sent to ANCS for more information
 * on an incoming notification.
 *
 */
void Process_ANCS_InfoRsp(ANCS_Info_Evt_t *info)
{
    // Sanity check before we dereference it
    if(info == NULL)
    {
        return;
    }

    // See if this is an answer to retrive sender info on an incoming phone call
    // or text message matching the current UID. If so, print it out.
    if((gCb.msgUID == info->notifUID) &&
       ((gCb.callState == MY_APP_CS_INCOMING_CALL) ||  (gCb.callState == MY_APP_CS_IDLE)))

    {
        if(info->attrID == ANCS_ATTR_TITLE)
        {
            uint8_t len = info->attrLen[0];
            memcpy(gCb.workBuff, info->attr, len);
            gCb.workBuff[len] = 0;      // Null terminate the message
            PRINTF("- From \"%s\"\r\n", gCb.workBuff);
        }
    }

    // Return the ANCS buffer to the pool
    WsfBufFree(info);
}

/*
 * Example of asking for more information after an async notification arrives.
 * This one is tailored for incoming phone calls and text messages.
 *
 */
static void ANCSRequestNotifTitle(void)
{
    ANCS_notif_action_t *pMsg;
    uint32_t irqThreshold;

    // Critical section since it can be called from another task.
    irqThreshold = IRQ_DisableAndSaveInterrupts();

    // Allocate a buffer to send to the BLE subsystem to write the request
    //   to the ANCS control point.
    if((pMsg = WsfBufAlloc(sizeof(ANCS_notif_action_t))) != NULL)
    {
        pMsg->hdr.event = MY_APP_WRITE_ANCS_ATTR_REQ;
        pMsg->commandID = ANCS_CMD_ID_GET_NOTIF_ATTR;
        memcpy(pMsg->notificationUID, (uint8_t *)&gCb.msgUID, sizeof(gCb.msgUID));
        pMsg->payload[0] = ANCS_ATTR_TITLE;     // Attribute ID: Title
        pMsg->payload[1] = WORK_BUFF_SZ;        // Max buff size for printf() includes 8 bytes header overhead
        pMsg->payload[2] = 0;
        WsfMsgSend(gCb.BLE_SubsysHandlerId, pMsg);
    }

    IRQ_RestoreInterrupts(irqThreshold);
}

/*
 * Generic wrapper for sending a notification action ID.
 *
 */
static void ANCSSendActionID(uint32_t targetUID, uint8_t action)
{
    ANCS_notif_action_t *pMsg;
    uint32_t irqThreshold;

    // Critical section since it can be called from another task.
    irqThreshold = IRQ_DisableAndSaveInterrupts();

    if((pMsg = WsfBufAlloc(sizeof(ANCS_notif_action_t))) != NULL)
    {
        pMsg->hdr.event = MY_APP_WRITE_ANCS_USR_ACT;    // Send a user Action ID
        pMsg->payload[0] = action;
        pMsg->commandID = ANCS_CMD_ID_PERFORM_NOTIF_ACTION;
        memcpy(pMsg->notificationUID, (uint8_t *)&targetUID, sizeof(uint32_t));
        WsfMsgSend(gCb.BLE_SubsysHandlerId, pMsg);
    }

    IRQ_RestoreInterrupts(irqThreshold);
}
