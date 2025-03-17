Overview
--------

Temperature indicator library provides a way to get the device temperature
with an accuracy of ± 5 °C using Low Frequency RC Low Power (LF-RC LP).

It needs calibration data written in user nvm info page.

How does it work ?
^^^^^^^^^^^^^^^^^^

Due to the fact that the LF-RC LP is not temperature compensated, it is actually
used to compute the device temperature.

The Temperature Indicator uses the LF-RC Low Power oscillator and measures his frequency
with RC Calibration hardware. The number of clock of HF-XTAL is counted in order to
determine the LF-RC LP frequency.

The RC calibration stops counting the HF-XTAL clock when it reaches a certain
number of LF-RC LP clocks. It then generates an ISR. In the interrupt handler,
the driver reads the number of counted clock cycles and computes the LF-RC LP frequency.

The HF-XTAL is started by the driver automatically.

The temperature calculation relies on the use of the RC calibration HW unit.
However, this unit might also be used to periodically calibrate the LF-RC and
during this process it is not available for any other calibration.
Thus, calibrating the LF-RC LP might be delayed.

For this reason, the API is asynchronous using a callback mechanism.

HF-XTAL
^^^^^^^

The HF-XTAL is mandatory when using the temperature indicator driver.

Temperature Indicator Driver registration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Temperature Indicator Driver shall be registered in NVM_ConfigModules() function.

.. code-block:: c

    #include "temperature_indicator.h"

    void NVM_ConfigModules(void)
    {
        // Register the modules.
        TI_RegisterModule();

        ...
    }

Temperature Indicator callback
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The temperature indicator works in a asynchronous way because the frequency
measurement takes time and the calibration with LF-RC LP cannot be done when
LF-RC calibration is in progress.

A callback shall be provided to the driver which will be called when the
measurement is finished.

The callback is called from an interrupt context (RC Calibration),
in case of QP/C usage, an event shall be posted to the task to be
informed that a new measurement is available.

The temperature is provided as a parameter of the user callback.
Moreover, an API is also available to get the last measurement.

The temperature measurement is in °C.

.. code-block:: c

    #include "temperature_indicator.h"

    static float temperature = 0.0f;

    /**
     * @brief Temperature Indicator callback
     */
    static void TI_Callback(float temp)
    {
        temperature = temp;

        QK_ISR_ENTRY();

        // Post an event to the task.
        QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_TEMP_INDICATOR_ISR);
        pEvent->params = (void*)&temperature;
        QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);

        QK_ISR_EXIT();
    }

    ...

    TI_SetMeasurementCallback(TI_Callback);

    ...

Start the temperature measurement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The TI_StartMeasure API returns false if there is no callback provided
to the driver. This API just ask the driver to start the temperature
measurement. This measurement is started when the system check if it
can go to sleep and if there is no LF-RC calibration running.

.. code-block:: c

    bool success = TI_StartMeasure();

When the task receive the event, the result can be read with 
TI_GetLastTemperature() API.

.. code-block:: c

    case SIG_TEMP_INDIC_ISR:
    {
        // Get temperature from parameter of event
        float temperature = *((float*)pEvent->params);

        // The API TI_GetLastTemperature(void); can also
        // be used to get the last temperature measurement.
        printf("Temperature = %.1f C\r\n", temperature);
    }
    break;


Calibration
^^^^^^^^^^^

The Temperature Indicator is not factory calibrated. The frequency of LF-RC LP
shall be measured at two different temperature and a linear regression shall be
done to compute the coefficient and offset.

The temperature is computed from the following formula.

.. code-block:: c

    float temperature = (coefficient * frequency) + offset;

The calibration data shall be written in NVM Info Page 2 (User) and is
read by the driver at power up of the system.

There are two address definitions in nvm_info.h header file that are used
to locate the calibration data.

.. code-block:: c

    #define NVM_INFO_P2_TI_CALIBRATION_ADDR       (MEMORY_NVMINFO_START + 0x4F58)
    #define NVM_INFO_P2_TI_CALIBRATION_BLOCK_ADDR (MEMORY_NVMINFO_START + 0x4F50)

Calibration data address table.

.. list-table:: NVM Info page 2 calibration data
   :widths: 40 40 40
   :header-rows: 1

   * - Data
     - Address
     - Format
   * - Length
     - 0x4F50
     - uint32_t
   * - CRC
     - 0x4F54
     - uint32_t
   * - Coefficient
     - 0x4F58
     - float
   * - Offset
     - 0x4F5C
     - float

Data shall be written in little endian in NVM Info page.

The float values are the IEEE-754 representation of
single precision float used by the FPU (32 bits).

A float converter is available on the following link.

https://www.h-schmidt.net/FloatConverter/IEEE754de.html

The Temperature Indicator driver provides an API to check
if calibration data are available and valid in NVM Info Page.

.. code-block:: c

    bool isCalibrated = TI_IsCalibrated();

When executing calibration, the frequency of LF-RC LP can be
read with the TI_GetFrequency() API. The frequency is in fixed
point 24.8 format.

.. code-block:: c

    // Frequency in fixed point format 24.8
    uint32_t frequency_fp = TI_GetFrequency();

    // Frequency in Hz in single precision float
    float frequency = (float)(frequency_fp >> 8) + (float)((frequency_fp & 0xFF) / 256.0f);
