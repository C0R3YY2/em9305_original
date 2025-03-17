////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_main.c
///
/// @project    T9305
///
/// @brief      Main Firmware - EMB DTS application
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

////////////(server example)//////////////
#include <ble_task.h>
#include <bsp.h>
#include <gpio.h>
#include <my_app_task.h>
#include <nvm_entry.h> // Must be included to add the header of the firmware.
#include <pml.h>
#include <radio.h>
#include <types.h>

///////////(adc)///////////////
#include "macros.h"
#include "uart.h"
#include "common.h"
#include "printf.h"
#include "puts.h"
#include "adc.h"
#include "qep.h"
#include "qf_port.h"
#include "timer.h"
#include "timer_hal.h"

///////////(i2c)///////////////
#include "i2c.h"
#include "i2c_hal.h"

Q_DEFINE_THIS_MODULE("Task")

/******************************************************************************\
 *  Constants
\******************************************************************************/
//////////////////(server's QPC_EVENT_POOL_SIZE)/////////////////////
#define QPC_EVENT_POOL_SIZE (BLE_TASK_EVENTS_QUEUE_SIZE + MY_APP_TASK_EVENTS_QUEUE_SIZE)

////////////////////////(adc)///////////////////////////////
/// Maximum number of events for the task.
#define NUM_TASK_EVENTS         (8u)

/// Add an offset to all signals to avoid QPC reserved signals.
#define FIRST_SIG_OFFSET        (Q_USER_SIG)

/// ADC ISR triggered
#define SIG_ADC_ISR             (FIRST_SIG_OFFSET + 0x01u)

/// Timer triggered
#define SIG_TIMER_TICK          (FIRST_SIG_OFFSET + 0x02u)

////////////////////////(i2c)///////////////////////////////
/// I2C operation
#define SIG_I2C                 (FIRST_SIG_OFFSET + 0x03u)


/// I2C Clock GPIO
#define GPIO_I2C_SCK            0

/// I2C SDA GPIO
#define GPIO_I2C_SDA            8

// MCP9808 I2C Address
#define MCP9808_I2C_ADDR        0x18

// MCP9808's Temperature register
#define MCP9808_REG_TEMP        0x05
// MCP9808's Resolution register
#define MCP9808_REG_RES         0x08

/// Common type for all tasks.
typedef struct
{
    QActive super;
} QL_Task_t;


/******************************************************************************\
 *  Global variables
\******************************************************************************/
///////////////////////(server)/////////////////////
/// QPC event pool.
static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

////////////////////(adc and i2c)////////////////
/// Event queue for the task.
static SECTION_NP_NOINIT const QEvt* gTaskEventQueue[NUM_TASK_EVENTS]; //adc and i2c have the same declaration

/// Task active object.
QL_Task_t AO_Task; //adc and i2c have the same declaration

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/
/////////////////(server/adc/i2c)//////////////////
static void Main_InitEventPool(void); //server and ADC and i2c all have the same Main_InitEventPool()

////////////////////////////////////(adc/i2c)//////////////////////////////////////
/**
 * @brief QPC initial task
 */
static QState TASK_initial(QL_Task_t* const me); //ADC and I2C have the same TASK_initial()

/**
 * @brief QPC idle task
 */
static QState TASK_idle(QL_Task_t* const me, QEvt* pEvt); //ADC and I2C both need a TASK_idle() even if
                                                          //the definitions are different

/**
 * @brief I2C Callback function for temperature sensor
 */
static void I2C_Callback(Driver_Status_t status, void* pUserData);


/******************************************************************************\
 *  External variables
\******************************************************************************/
///////////////////(adc)////////////////
extern volatile UART_Configuration_t gUART_Config; //adc and i2c both have gUART_Config

// Combined data buffer [temp_hi, temp_lo, 0000, adc_hi, adc_lo]
// Format: [16-bit temp data][4-bit zero delimiter][16-bit ADC data]
uint8_t combinedDataBuffer[5]; 

///////////////////(i2c)////////////////
static volatile uint8_t tempBuffer[2]; // temperature buffer


/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules.
    /////////////(server)////////////
    RF_RegisterModule();

    /////////(adc/i2c)///////////
    UART_RegisterModule();

    /////////(adc)///////////
    Timer_RegisterModule();

    /////////(i2c)///////////
    I2C_RegisterModule();


    //////////(adc/i2c)//////////
    // Enable UART.
    gUART_Config.enabled = true;

    /////////(adc)///////////
    // Enable unitimer.
    gTimer_Config.enabled = true;

    /////////(i2c)///////////
    gI2C_Config.clockFrequency = (uint8_t)ClockFrequency100;
    gI2C_Config.clockStretching = true;
    gI2C_Config.gpioSck = GPIO_I2C_SCK;
    gI2C_Config.gpioSda = GPIO_I2C_SDA;
    gI2C_Config.enabled = true;

    // Check if it wakes up from sleep.
    if (PML_DidBootFromSleep())
    {
        //////////(adc/i2c)//////////
        
        // Re-initialize the QPC event pool.
        extern uint_fast8_t QF_maxPool_;
        QF_maxPool_ = (uint_fast8_t)0;

        // Initialize the QPC event pool.
        Main_InitEventPool();
    }
    else
    {
        ////////////////////(i2c: NEED TO DISABLE JTAG TO USE GPIO 8 for SDA)////////////////////////
        GPIO_DisableJtag();

        ////////////////////(adc/i2c)////////////////////////

        // ENABLE INPUTs:
            // adc: GPIO5d
            // i2c: GPIO0 (for SCK), GPIO8 (for SDA)
        // (leave other inputs disabled)
        gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = 
            (uint32_t)(GPIO_MASK_PIN_5 | GPIO_MASK(GPIO_I2C_SCK) | GPIO_MASK(GPIO_I2C_SDA));

        // ENABLE OUTPUTs:
            // adc: GPIO7 (for UART TX)
            // i2c: GPIO0 (for SCK), GPIO8 (for SDA)
        // (disable output on other pins)
        gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = 
            (uint32_t) (GPIO_MASK_PIN_7 | GPIO_MASK(GPIO_I2C_SCK) | GPIO_MASK(GPIO_I2C_SDA));

        // Disable pull-downs on all pins
        // used in adc and i2c
        gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

        // adc/i2c:
        // Enable pull-up on GPIO7 (adc), GPIO0 (SCK), GPIO8 (SDA)
        gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = 
            (uint32_t)(GPIO_MASK_PIN_7 | GPIO_MASK(GPIO_I2C_SCK) | GPIO_MASK(GPIO_I2C_SDA));

        // i2c:
        // Set I2C Slave SCK
        gGPIO_Config.hardwareState.RegGPIOOutSel0.regs.GPIOOutSel0 =
            (uint8_t)GPIO_PIN_FUNC_OUT_I2C_SCK;

        // i2c:
        // Set I2C Slave SDA
        gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel8 =
            (uint8_t)GPIO_PIN_FUNC_OUT_I2C_SDA;

        // adc/i2c:
        // Set the UART TX output function on GPIO7
        gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
            (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

        // adc/i2c:
        // Disable the UART RX input function
        gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
            (uint8_t)GPIO_FUNCTION_NOT_MAPPED;

        ///////////////(server and adc and i2c both have the two functions below)/////////////
        // Initialize QPC
        QF_init();

        // Initialize the QPC event pool.
        Main_InitEventPool();
    }

    

    // Disable sleep mode if JTAG debug is used.
    // gPML_Config.sleepModeForbiden = true;

    // Enable JTAG.
    // GPIO_EnableJtag2Wires();
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    ///////////////(server and adc both have functions below)///////////////////
    // Initialize the board support package.
    BSP_Init();

    // Enable interrupts (threshold set during IRQ module initialization).
    IRQ_EnableInterrupts();

    if (PML_DidBootFromSleep())
    {
        /////////////(adc)/////////////
        // Resume unitimer
        Timer_Resume();
        // Resume ADC
        ADC_Resume();

        //////////////(server and adc and i2c all have the function below)/////////////////////
        // Resume QPC.
        (void)QF_resume();
    }
    else
    {
        /////i put adc's stuff BEFORE server's stuff... not sure if it should be the reverse order

        ///////////(adc)/////////////////////////
        // Initialize ADC
        ADC_Init();


        ///////////(adc/i2c)/////////////////////////
        // Set HF clock source Xtal.
        PML_SetHfClkSourceNonBlocking(PML_HF_CLK_XTAL);

        // Create the QPC task.
        QActive_ctor(&AO_Task.super, Q_STATE_CAST(&TASK_initial));

        // Start QPC task.
        QACTIVE_START(&AO_Task.super, (uint_fast8_t)gNextAvailablePriority--,
               &gTaskEventQueue[0], NUM_TASK_EVENTS, 0, 0, NULL);


        ////////////(server)///////////////////////
        // Create and start the Bluetooth task.
        BleTask_Create();
        BleTask_Start();

        // Create and start the application task.
        // The application task must be started after the BLE task is started.
        MyAppTask_Create();
        MyAppTask_Start();

        //////////////////(server and adc and i2c all have this)//////////////////
        // Run QPC.
        (void)QF_run();
    }

    // Initialization failed!
    while (true)
    {
        HaltCPU();
    }
}

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/
/////////////////(server and adc both has the same Main_InitEventPool() definition)//////////////
static void Main_InitEventPool(void)
{
    QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
    QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool),
        sizeof(QEventParams));
}

/////////////////////////////////////////////////(adc)//////////////////////////////////////
static void ADC_ConfigSourceVBAT1(void)
{
    // Set Source from VBAT1
    ADC_SetSourceSelection(ADC_SOURCE_VBAT1);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceVCC(void)
{
    // Set Source from VCC
    ADC_SetSourceSelection(ADC_SOURCE_VCC);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceGPIO5WithSampleAndHold(void)
{
    // Set Source from GPIO5 with Sample & Hold
    ADC_SetSourceSelection(ADC_SOURCE_GPIO5_WITH_SH);

    // Set Range from 0.05 V to 2 V as requested
    ADC_SetRange(ADC_RANGE_0_05V_TO_2V);

    // Enable Sample & Hold
    ADC_EnableSampleAndHold();

    // Set Sample & Hold clock config
    ADC_SetSHClockConfig(ADC_SH_CLOCK_60000_HZ);

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ConfigSourceGPIO5(void)
{
    // Set Source from GPIO5 without Sample & Hold
    ADC_SetSourceSelection(ADC_SOURCE_GPIO5_WITHOUT_SH);

    // Set Clock of ADC to 120 kHz
    ADC_SetClockConfig(ADC_CLK_120000_HZ);

    // Enable Vhi for range from 1.8 V to 3.6 V instead
    // of range from 0.95 V to 1.9 V
    ADC_EnableVhi();

    // Set ADC resolution of 9 bits
    ADC_SetResolution(ADC_9_BITS);
}

static void ADC_ISR_Callback(Driver_Status_t status, void* pUserData)
{
    //printf("entered ADC_ISR_Callback \r\n");

    // Callback for ADC in single mode is called when a sample
    // is available.
    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_ADC_ISR);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    // Also post an event to handle the temperature data
    QEventParams* pTempEvent = (QEventParams*)Q_NEW(QEventParams, SIG_I2C);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pTempEvent, NULL);

    QK_ISR_EXIT();

    //printf("exited ADC_ISR_Callback \r\n");
}

static void I2C_Callback(Driver_Status_t status, void* pUserData)
{
    //printf("entered I2C_Callback \r\n");

    QK_ISR_ENTRY();
    
    // Post an event to the task to handle the temperature data
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_I2C);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();

    //printf("exited I2C_Callback \r\n");
}

/**
 * @brief Timer callback
 *
 * This callback is called when the timer triggers an interrupt.
 */
static void Timer_Callback(Driver_Status_t status, void* pUserData)
{
    //printf("entered Timer_Callback \r\n");

    QK_ISR_ENTRY();

    // Post an event to the task.
    QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_TIMER_TICK);
    QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

    QK_ISR_EXIT();

    //printf("exited Timer_Callback \r\n");
}

static QState TASK_initial(QL_Task_t* const me)
{
    return Q_TRAN(&TASK_idle);
}

static QState TASK_idle(QL_Task_t* const me, QEvt* pEvt)
{
    //printf("entered TASK_idle \r\n");

    QState qstatus = Q_HANDLED();
    QEventParams* pEvent = ((QEventParams*)pEvt);

    switch (pEvent->super.sig)
    {
        case Q_ENTRY_SIG:
        {
            //printf("entered Q_ENTRY_SIG \r\n");

            const char string[] = "EM Microelectronic";

            // Print EM Microelectronic
            //printf("%s\r\n", string);

            // Print EM9305
            //printf("EM%d\r\n", 9305);
            
            // Print configuration information
            //printf("ADC Configuration: GPIO5 with Sample & Hold\r\n");
            //printf("Voltage Range: 0.05V to 2V\r\n");
            //printf("Sampling Rate: 20Hz\r\n");
            //printf("I2C Configuration: MCP9808 temperature sensor\r\n");
            //printf("Combined data format: [16-bit temp][4-bit zeros][16-bit ADC]\r\n");

            // Configure ADC
            // 2 : GPIO5 with Sample & Hold
            //     with this source, S&H can be enabled or not
            //     If S&H is enabled, duration should be configured
            //     Range should be selected
            //     - 0.05 V to 0.5 V
            //     - 0.05 V to 1 V
            //     - 0.05 V to 2 V
            ADC_ConfigSourceGPIO5WithSampleAndHold();

            // ADC shall be enabled after full configuration
            ADC_Enable();

            // In order to use single convertion with interrupt, a callback should be set
            //printf("before ADC_RegisterCallback \r\n");
            ADC_RegisterCallback(ADC_ISR_Callback);
            //printf("after ADC_RegisterCallback \r\n");

            // Configure 20Hz timer (50ms) for continuous voltage monitoring
            //printf("before Timer_SchedulePolling \r\n");
            int8_t idx = Timer_SchedulePolling(20, Timer_Callback, NULL); //calls Timer_Callback at 20Hz
            //printf("after Timer_SchedulePolling \r\n");
            
            // Start timer
            //printf("before Timer_Enable \r\n");
            Timer_Enable(idx); //begin counting the timer for the 20 Hz
            //printf("after Timer_Enable \r\n");

            // Initialize the combined data buffer
            combinedDataBuffer[0] = 0x00; // temp high byte
            combinedDataBuffer[1] = 0x00; // temp low byte
            combinedDataBuffer[2] = 0x00; // zero delimiter (4 bits)
            combinedDataBuffer[3] = 0x00; // ADC high byte
            combinedDataBuffer[4] = 0x00; // ADC low byte
            
            // Configure MCP9808 resolution to 0x03 (4 samples per second)
            /*uint8_t resolution = 0x03;
            if (!I2C_WriteRegister(MCP9808_I2C_ADDR, MCP9808_REG_RES, &resolution, 1, NULL, NULL))
            {
                printf("ERROR: Failed to configure MCP9808 resolution\r\n");
            }
            
            // Start first temperature reading
            //printf("before I2C_ReadRegister\r\n");
            if (!I2C_ReadRegister(MCP9808_I2C_ADDR, MCP9808_REG_TEMP, (uint8_t*)tempBuffer, 2, I2C_Callback, NULL))
            {
                printf("ERROR: Failed to start I2C temperature reading\r\n");
            }*/
            //printf("before I2C_ReadRegister\r\n");

            //printf("exited Q_ENTRY_SIG \r\n");
        }
        break;

        case SIG_TIMER_TICK:
        {
            //printf("entered SIG_TIMER_TICK \r\n");

            // Start a single convertion on the ADC and return back the
            // raw value.
            ADC_StartWithCallback();

            //printf("exited SIG_TIMER_TICK \r\n");
        }
        break;

        case SIG_ADC_ISR:
        {
            //printf("entered SIG_ADC_ISR \r\n");

            uint16_t adc;
            
            // Read Data from ADC
            (void) ADC_GetValue(&adc);

            uint16_t mv = ADC_ValueToMillivolt(adc);
            
            // Store ADC data in combined buffer (big-endian format)
            combinedDataBuffer[3] = (uint8_t)((mv >> 8) & 0xFF);  // high byte
            combinedDataBuffer[4] = (uint8_t)(mv & 0xFF);         // low byte

            // Store ADC value in static variable for printing
            static uint16_t current_mv = 0;
            current_mv = mv;
            
            // Print the complete buffer and sensor values in one line
            // Format: Buffer[5 bytes hex] | Temp[°C] | ADC[mV]
            
            // Get the temperature value from the buffer
            uint16_t temp_val = ((uint16_t)combinedDataBuffer[0] << 8) | (uint16_t)combinedDataBuffer[1];
            float temp_celsius = (temp_val & 0x0FFF) / 16.0f;
            
            // Print all data in one line
            /*printf("Buffer: 0x%02X%02X%02X%02X%02X | Temp: %.2f °C | Voltage: %d mV\r\n",
                   combinedDataBuffer[0], combinedDataBuffer[1], combinedDataBuffer[2], 
                   combinedDataBuffer[3], combinedDataBuffer[4],
                   temp_celsius, mv);*/
            
            //printf("exited SIG_ADC_ISR \r\n");
        }
        break;
        
        case SIG_I2C:
        {
            //printf("entered SIG_I2C \r\n");
            
            // Make a local copy of the temperature buffer for safety
            uint8_t local_buffer[2];
            local_buffer[0] = tempBuffer[0];
            local_buffer[1] = tempBuffer[1];
            
            // Process temperature data
            uint16_t raw_temp = ((uint16_t)local_buffer[0] << 8) | (uint16_t)local_buffer[1];
            
            // Convert raw temperature to Celsius
            // Clear flag bits (bits 12-15)
            float temp_celsius = (raw_temp & 0x0FFF) / 16.0f;
            
            // Handle negative temperatures (bit 12 set)
            if (raw_temp & 0x1000)
            {
                temp_celsius = -temp_celsius;
            }
            
            // Store temperature data in combined buffer (bits 0-11 of raw_temp)
            // We only care about the actual temperature value, not the flags
            uint16_t temp_value = raw_temp & 0x0FFF;
            combinedDataBuffer[0] = (uint8_t)((temp_value >> 8) & 0xFF); // high byte
            combinedDataBuffer[1] = (uint8_t)(temp_value & 0xFF);        // low byte
            // The delimiter byte is already set to 0
            
            // Get the ADC value from the buffer
            uint16_t adc_val = ((uint16_t)combinedDataBuffer[3] << 8) | (uint16_t)combinedDataBuffer[4];
            uint16_t mv = adc_val; // ADC value in mV is already stored in the buffer
            
            // Print all data in one line
            /*printf("Buffer: 0x%02X%02X%02X%02X%02X | Temp: %.2f °C | Voltage: %d mV\r\n",
                   combinedDataBuffer[0], combinedDataBuffer[1], combinedDataBuffer[2], 
                   combinedDataBuffer[3], combinedDataBuffer[4],
                   temp_celsius, mv);*/
                              
            // Request next temperature reading
            //printf("before I2C_ReadRegister\r\n");
            if (!I2C_ReadRegister(MCP9808_I2C_ADDR, MCP9808_REG_TEMP, (uint8_t*)tempBuffer, 2, I2C_Callback, NULL))
            {
                //printf("ERROR: Failed to request next temperature reading\r\n");
            }
            //printf("after I2C_ReadRegister\r\n");
            
            //printf("exited SIG_I2C \r\n");
        }
        break;

        default:
        {
            //printf("exited TASK_idle \r\n");
            qstatus = Q_SUPER(&QHsm_top);
        }
        break;
    }

    return qstatus;
}


