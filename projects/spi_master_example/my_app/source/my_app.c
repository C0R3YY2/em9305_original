////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app.c
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
#include <printf.h>
#include <qf_port.h>
#include <spi_master.h>
#include <timer.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// SPI clock frequency.
#define ADXL362_SPI_FREQ                (4000000u)  // 4MHz

/// Command WRITE.
#define ADXL362_SPI_CMD_REG_WRITE       (0x0Au)
/// Command READ.
#define ADXL362_SPI_CMD_REG_READ        (0x0Bu)

/// This register contains the Analog Devices device ID.
#define ADXL362_REGISTER_DEVID_AD       (0x00u)
/// The power control register
#define ADXL362_REGISTER_POWER_CTL      (0x2Du)

/// The Analog Devices ID for Register 0x00.
#define ADXL362_ANALOG_DEVICES_ID       (0xADu)

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Timer index.
static int8_t gTimerIdx = -1;

/// Buffer for write operations of SPI master driver.
static uint8_t gWriteBuf[3];

/// Buffer for read operations of SPI master driver.
static uint8_t gReadBuf[6];

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static void MyApp_Callback(Driver_Status_t status, void* pUserData)
{
    QK_ISR_ENTRY();

    // Post the signal.
    MyAppTask_PostEvent((MyApp_Signals_t)pUserData, NULL);

    QK_ISR_EXIT();
}

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

            printf("EM9305 SPI Master demo. Starting...\r\n");

            // Configure and start a 1Hz timer (1s).
            gTimerIdx = Timer_SchedulePolling(1.0, MyApp_Callback,
                    (void *)TIMER_READ_ACCEL_DATA_SIG);

            Timer_Enable(gTimerIdx);

            // Prepare to read the device ID
            gWriteBuf[0] = ADXL362_SPI_CMD_REG_READ;
            gWriteBuf[1] = (uint8_t) ADXL362_REGISTER_DEVID_AD;

            // Write the command and keep CS low
            (void) SPIM_WriteBytes(GPIO_SPIM_CSN, true, gWriteBuf, 2, NULL, NULL);

            // Read device ID
            (void) SPIM_ReadBytes(GPIO_SPIM_CSN, false, gReadBuf, 1, MyApp_Callback, (void *)READ_ACCEL_ID_SIG);
        }
        break;

        case MY_APP_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

        case READ_ACCEL_ID_SIG:
        {
            // Check if the device ID is correct
            if(gReadBuf[0] == ADXL362_ANALOG_DEVICES_ID)
            {
                printf("DeviceID OK.\r\n");
            }
            else
            {
                printf("ERROR: Not able to read the correct DeviceID. Read 0x%X.\r\n", gReadBuf[0]);
            }

            // Prepare to start the measurements
            gWriteBuf[0] = ADXL362_SPI_CMD_REG_WRITE;
            gWriteBuf[1] = ADXL362_REGISTER_POWER_CTL;
            gWriteBuf[2] = 0x02;

            // Write the command
            (void) SPIM_WriteBytes(GPIO_SPIM_CSN, false, gWriteBuf, 3, NULL, NULL);
        }
        break;

        case TIMER_READ_ACCEL_DATA_SIG:
        {
            // Prepare to read the register containing the data
            gWriteBuf[0] = ADXL362_SPI_CMD_REG_READ;
            gWriteBuf[1] = 0x0E; // 12 bits data (x,y,z)

            // Write the register address of the data (0x0E), keep the CSN low after the transaction
            (void) SPIM_WriteBytes(GPIO_SPIM_CSN, true, gWriteBuf, 2, NULL, NULL);

            // Read the 6 bytes of the accelerometer data (12bits x,y and z)
            (void) SPIM_ReadBytes(GPIO_SPIM_CSN, false, gReadBuf, 6, MyApp_Callback, (void *)READ_ACCEL_DATA_DONE_SIG);
        }
        break;

        case READ_ACCEL_DATA_DONE_SIG:
        {
            // Bytes to int16_t for the 3 axes
            int16_t x = (int16_t)(gReadBuf[1]<<8) + gReadBuf[0];
            int16_t y = (int16_t)(gReadBuf[3]<<8) + gReadBuf[2];
            int16_t z = (int16_t)(gReadBuf[5]<<8) + gReadBuf[4];

            // Print the 3 axes values
            printf("x = %d, y = %d, z = %d\r\n", x, y, z);
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
