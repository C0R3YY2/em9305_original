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

#include "my_app.h"

#include <my_app_signals.h>
#include <my_app_task.h>
#include <app_api.h>
#include <ble_task.h>
#include <ccpc/ccpc_api.h>
#include <types.h>
#include <string.h>
#include <uart.h>
#ifdef DBG_PRINT
#include <printf.h>
#endif
/******************************************************************************\
 *  Constants
\******************************************************************************/

/******************************************************************************\
 *  Types
\******************************************************************************/
typedef enum
{
    MY_APP_ACTIVECASE_CCP_TEST = 1,
    MY_APP_ACTIVECASE_DEFAULT = 0xFF,
} MyApp_ActiveCase_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/
const bdAddr_t gPTSDongleAddr = {0x77, 0xd0, 0xe8, 0xe8,0x07,0xC0};

uint8_t gDataInput = 0;
static uint8_t gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
static uint8_t idx = 0;
char szRxData[5] = {0};

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/
void UART_RxCallback(Driver_Status_t status, void* pUserData);
static void MyApp_CLIInput(uint8_t data);
/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            /* Write your application initialization code here. */
            // Instruct the BLE task to Initialize.
            BleTask_PostEvent(BLE_INIT_SIG, NULL);
            // Setup Receive on UART
            UART_ReceiveDataWithCallback(&gDataInput, 1, UART_RxCallback, NULL);
        }
        break;
        case MY_APP_UART_RXD_DATA:
        {
            MyApp_CLIInput(*(uint8_t*)pParams);
        }
        break;
        case MY_APP_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

        /* Add other cases to handle other application signals here. */
        default:
        {
            // Unknown event!
            ret = MY_APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}

void UART_RxCallback(Driver_Status_t status, void* pUserData)
{
    if ((status == DRIVER_STATUS_RX || status == DRIVER_STATUS_OK) )
    {
        uint8_t tempData = gDataInput;
        MyAppTask_PostEvent(MY_APP_UART_RXD_DATA, (void*)&tempData);
    }
    else
    {
        #ifdef DBG_PRINT
        printf("Drv Sts: %d \n", status);
        #endif
        //Reset the variables due to driver error
        gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
        idx=0;
    }

    UART_ReceiveDataWithCallback(&gDataInput, 1, UART_RxCallback, NULL);
}

static void MyApp_CLIInput(uint8_t data)
{
    dmConnId_t connId = AppConnIsOpen();
    szRxData[idx++] = data;
    if(gActiveCase == MY_APP_ACTIVECASE_DEFAULT)
    {
    #ifdef DBG_PRINT
        printf("Press 'C' or 'c' for CCP Testing\r\n");
    #endif
    }
    if(szRxData[0] == 'C' || szRxData[0] == 'c')
    {
        if(gActiveCase == MY_APP_ACTIVECASE_DEFAULT)
        {
            gActiveCase = MY_APP_ACTIVECASE_CCP_TEST;
            #ifdef DBG_PRINT
            printf("CCP Selected. 'M' for menu\r\n");
            #endif
        }

        else if(gActiveCase == MY_APP_ACTIVECASE_CCP_TEST)
        {
            uint8_t svcId;

            if(idx > 2)
            {
                svcId = szRxData[2] - '0';
            #ifdef DBG_PRINT
                printf("SvcId: %d\r\n", svcId);
            #endif
                if(svcId >= 2)
                {
                    gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
                    idx = 0;
                }
            }

            switch(szRxData[1])
            {
                case 'M':
                {
                    gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
                    #ifdef DBG_PRINT
                    printf("a/A - Write Accept Call\r\n");
                    printf("t/T - Write Terminate Call\r\n");
                    printf("h/H - Write Hold Call\r\n");
                    printf("j/J - Write Join Call\r\n");
                    printf("r - Write Retrieve Call\r\n");
                    printf("o - Write Originate Call\r\n");
                    printf("p/P - Read Provider Name\r\n");
                    printf("u - Read UCI\r\n");
                    printf("U - Read URI list\r\n");
                    printf("b - Read bearer tech\r\n");
                    printf("s - Read state\r\n");
                    printf("S - Read signal strength\r\n");
                    printf("L - Read Current Calls List\r\n");
                    printf("c - Read CCID\r\n");
                    printf("i - Read Incoming Call\r\n");
                    printf("I - Read Incoming Call URI\r\n");
                    printf("O - Read Ctrl Opcodes Supported\r\n");
                    printf("F - Read Status flags\r\n");
                    printf("f - Read Caller Friendly Name\r\n");
                    printf("q - Quit out of current case\r\n");

                    #endif
                    break;
                }
                //CCP Write
                case 'a':
                case 'A':
                {
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Accept Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcAcceptCall(connId, svcId, 1);
                    }
                    break;
                }
                case 't':
                case 'T':
                {
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Terminate Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcTerminateCall(connId, svcId, 1);
                    }
                    break;
                }
                case 'h':
                case 'H':
                {
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Hold Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcHoldCall(connId, svcId, 1);
                    }
                    break;
                }
                case 'j':
                case 'J':
                {
                    uint8_t cid[2] = {1, 2};

                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Join Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcJoinCall(connId, svcId, (uint8_t*)cid, 2);
                    }
                    break;
                }
                case 'r':
                {
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Retrieve Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcRetrieveCall(connId, svcId, 1);
                    }
                    break;
                }
                case 'o':
                {
                    char uri[] = "tel";

                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Originate Call, Enter SvcId from 0-1\r\n");
                #endif
                    }
                    else
                    {
                        CcpcOriginateCall(connId, svcId, (uint8_t*)uri, (uint8_t)strlen(uri));
                    }
                    break;
                }
                //CCP Read
                case 'p':
                case 'P':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Provider Name\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadProviderName(connId, svcId, 0);
                    }
                    break;
                case 'u':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read UCI\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadUci(connId, svcId);
                    }
                    break;
                case 'U':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read URI list\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadUriSchemesList(connId, svcId, 0);
                    }
                    break;
                case 'b':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read bearer tech\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadBearerTechnology(connId, svcId);
                    }
                    break;
                case 's':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read state\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadState(connId, svcId, 0);
                    }
                    break;
                case 'S':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read signal strength\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadSignalStrength(connId, svcId);
                    }
                    break;
                case 'L':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Current Calls List\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadListCurrentCalls(connId, svcId, 0);
                    }
                    break;
                case 'c':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read CCID\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadCcid(connId, svcId);
                    }
                    break;
                case 'I':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Incoming Call URI\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadIncomingCallBearerUri(connId, svcId, 0);
                    }
                    break;
                case 'i':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Incoming call\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadIncomingCall(connId, svcId, 1);
                    }
                    break;
                case 'O':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Ctrl Opcodes Supported\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadCtrlPtOpcodesSupported(connId, svcId);
                    }
                    break;
                case 'F':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Status flags\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadStatusFlags(connId, svcId);
                    }
                    break;
                case 'f':
                    if(idx <= 2)
                    {
                #ifdef DBG_PRINT
                    printf("Read Caller Friendly Name\r\n");
                #endif
                    }
                    else
                    {
                        CcpcReadCallFriendlyName(connId, svcId, 0);
                    }
                    break;
                case 'q':
                    gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
                    idx = 0;
                default:
                    break;

            }
            if(idx >= 3)
            {
                gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
                idx=0;
            }
        }

    }
    else
    {
        gActiveCase = MY_APP_ACTIVECASE_DEFAULT;
        idx=0;
    }
}
