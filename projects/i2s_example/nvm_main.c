////////////////////////////////////////////////////////////////////////////////
///
/// @file       i2s_example/nvm_main.c
///
/// @project    T9305
///
/// @brief      Simple I2S output of 2 sine waves.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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

#include "types.h"
#include "nvm_entry.h"
#include "i2s.h"
#include "sleep_manager.h"
#include "irq.h"
#include "math.h"

/******************************************************************************\
 *  Definitions
\******************************************************************************/

#define M_PI 3.141592f

/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

void NVM_ConfigModules(void)
{
    // Register the modules
    I2S_RegisterModule();

    // Enable I2S
    gI2S_Config.enabled = true;

    // I2S GPIO configuration
    gI2S_Config.gpioSclk = 8u;
    gI2S_Config.gpioSdataOut = 9u;
    gI2S_Config.gpioSdataIn = 9u; // Unused in TX_ONLY
    gI2S_Config.gpioFsync = 10u;
    gI2S_Config.gpioMclk = 11u;

    // I2S Config bits
    gI2S_Config.config.bits.protocol = I2S_PROTOCOL_I2S;
    gI2S_Config.config.bits.mode = I2S_MASTER_MODE;
    gI2S_Config.config.bits.cpha = 0;
    gI2S_Config.config.bits.fsyncPolarity = 0;
    gI2S_Config.config.bits.phaseLengthMode = I2S_PHASE0_LENGTH_FSYNC_EDGE;

    // I2S Phase0 config
    gI2S_Config.phase0.bits.delay = 1;
    gI2S_Config.phase0.bits.sampleSize = I2S_SAMPLE_FORMAT_16BIT;

    // I2S Phase1 config
    gI2S_Config.phase1.bits.delay = 1;
    gI2S_Config.phase1.bits.sampleSize = I2S_SAMPLE_FORMAT_16BIT;

    // I2S transfer mode
    gI2S_Config.transferMode = I2S_TX_ONLY;

    // Stereo mode
    gI2S_Config.phase0TxChannels = 1;
    gI2S_Config.phase1TxChannels = 1;

    // I2S Clock frequencies
    gI2S_Config.samplingRate = I2S_SAMPLE_RATE_44KHZ; // 44100Hz
    gI2S_Config.serialClockRatio = 64, // 2.8224MHz
    gI2S_Config.masterClockRatio = 512, // 22.5792MHz

    // Phase0 and Fsync lengths
    gI2S_Config.phase0Length = 32;
    gI2S_Config.fsyncLength = 32;

    // Let the driver manages the clocks
    gI2S_Config.autoClk = true;
}

NO_RETURN void NVM_ApplicationEntry(void)
{
    // Enable interrupts (threshold set during IRQ module initialization)
    IRQ_EnableInterrupts();

    // Signals buffer
    int16_t sinus[400];

    // Generate the sine waves
    for(uint32_t i = 0 ; i < 200 ; ++i)
    {
        // Channel 1: 220.5Hz (left)
        sinus[2*i] = (int16_t) (32000.0f * sinf((2.0f * M_PI / 200.0f) * (float) i));

        // Channel 2: 441Hz (right)
        sinus[2*i+1] = (int16_t) (32000.0f * sinf((2.0f * M_PI / 100.0f) * (float) i));
    }

    // Prepare the transactions
    int32_t status = 0;
    status += I2S_ClearTransactions(); // Ensure queue is empty
    // Add one TX transaction, no callback needed as we just send infinite data
    status += I2S_AddTxTransaction((uint8_t*) sinus, sizeof(sinus), NULL, NULL);

    // Start I2S for indefinite time
    status += I2S_StartTx(0);

    // Check if an error occurred
    if(status)
    {
        ResetCPU();
    }

    // Main loop
    // It does nothing as we cannot go to sleep while I2S is running
    while(true)
    {
        // Disable interrupts
        uint32_t irqThreshold = IRQ_DisableAndSaveInterrupts();
        // Nothing to do, go to sleep
        irqThreshold = SLEEP_MANAGER_GoToSleep(irqThreshold);
        // Restore interrupts
        IRQ_RestoreInterrupts(irqThreshold);
    }
}
