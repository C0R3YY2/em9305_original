Overview
--------

The AoAD driver enables the configuration of hardware for *Angle of Departure* (AoD) and *Angle of Arrival* (AoA) in BLE operations.

The chapter *Direction Finding (AOA/AOD)* in the EM9305 datasheet explains how the lookup table (LUT) and GPIOs work together for controlling the antenna switching.
Please refer to this chapter for more details on LUT mapping with the GPIO matrix.

Configuration of AoAD
---------------------

First of all, the AoAD header file shall be included to get access to the AoAD API as shown in the following code snippet.

.. code-block:: c

    #include "aoad.h"

The AoAD module shall first be registered in the `NVM_ConfigModules()` function before the GPIOs used for antenna switching can be configured.

The AoAD configuration relies on a globally defined structure that shall be declared as extern in any application that use this feature.

.. code-block:: c

    extern AOAD_Configuration_t gAOAD_Config;

Once done, the AoAD driver can be registered and configured.

In this example, GPIO 8, 9, 10 and 11 are used for antenna switching.

    * Bit 0 of LUT (Lookup table) will drive GPIO 8 state.
    * Bit 1 of LUT (Lookup table) will drive GPIO 9 state.
    * Bit 2 of LUT (Lookup table) will drive GPIO 10 state.
    * Bit 3 of LUT (Lookup table) will drive GPIO 11 state.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Register the modules.
        AOAD_RegisterModule();

        // Configure GPIOs for AOAD
        // GPIO 8 - 11 used for AOAD
        gAOAD_Config.gpio_for_aoad[8] = true;
        gAOAD_Config.gpio_for_aoad[9] = true;
        gAOAD_Config.gpio_for_aoad[10] = true;
        gAOAD_Config.gpio_for_aoad[11] = true;

        gAOAD_Config.gpio_sel2.bits.gpio_out_sel8  = 0x00; // Bit 0 of Lut on GPIO 8
        gAOAD_Config.gpio_sel2.bits.gpio_out_sel9  = 0x01; // Bit 1 of Lut on GPIO 9
        gAOAD_Config.gpio_sel2.bits.gpio_out_sel10 = 0x02; // Bit 2 of Lut on GPIO 10
        gAOAD_Config.gpio_sel2.bits.gpio_out_sel11 = 0x03; // Bit 3 of Lut on GPIO 11

        gAOAD_Config.enabled = true;


Once the hardware configuration is complete, the AoA/AoD feature can be used.
