Overview
--------

The Quadrature decoder (QDEC) driver allows to configure the peripheral and
reads movements from mechanical or optical encoder.

An incremental encoder has two output signals, A and B, which issue pulses
when device is moved. Together, the A and B signals indicate both the occurence
of and direction of movement.

It does not indicate absolute position, it only reports changes in position and,
for each reported position change, the direction of the movement.

Incremental encoders report position changes nearly instantaneously, which allows
them to monitor the movements of high speed mechanisms in near real-time.
Because of this, incremental encoders are commonly used in applications that
require precise measurement and control of position and velocity.

If the QDEC is used with an optical sensor, the led shall be configured.
The peripheral allows to set the polarity and led time.

Each channel (A, B) has separate debouncer, this debouncer has same debounce
time as sampling time.

The Led driver for optical sensor is synchronous with sampling period.

How does it work ?
^^^^^^^^^^^^^^^^^^

The two phases have a 90° phase difference and generate square signals.

.. list-table:: Phase A & B output signals
   :widths: 20 20 20 20 20 20 20 20
   :header-rows: 1

   * - Prev A
     - Prev B
     - Curr A
     - Curr B
     - Sample
     - acc
     - acc_dbl
     - Description

   * - 0
     - 0
     - 0
     - 0
     - 0
     - acc + 0
     - acc_dbl + 0
     - No movement

   * - 0
     - 0
     - 0
     - 1
     - 1
     - acc + 1
     - acc_dbl + 0
     - Movement in positive direction

   * - 0
     - 0
     - 1
     - 0
     - -1
     - acc - 1
     - acc_dbl + 0
     - Movement in negative direction

   * - 0
     - 0
     - 1
     - 1
     - 2
     - acc + 0
     - acc_dbl + 1
     - Error (double movement)

   * - 0
     - 1
     - 0
     - 0
     - -1
     - acc - 1
     - acc_dbl + 0
     - Movement in negative direction

   * - 0
     - 1
     - 0
     - 1
     - 0
     - acc + 0
     - acc_dbl + 0
     - No movement

   * - 0
     - 1
     - 1
     - 0
     - 2
     - acc + 0
     - acc_dbl + 1
     - Error (double movement)

   * - 0
     - 1
     - 1
     - 1
     - 1
     - acc + 1
     - acc_dbl + 0
     - Movement in positive direction

   * - 1
     - 0
     - 0
     - 0
     - 1
     - acc + 1
     - acc_dbl + 0
     - Movement in positive direction

   * - 1
     - 0
     - 0
     - 1
     - 2
     - acc + 0
     - acc_dbl + 1
     - Error (double movement)

   * - 1
     - 0
     - 1
     - 0
     - 0
     - acc + 0
     - acc_dbl + 0
     - No movement

   * - 1
     - 0
     - 1
     - 1
     - -1
     - acc - 1
     - acc_dbl + 0
     - Movement in negative direction

   * - 1
     - 1
     - 0
     - 0
     - 2
     - acc + 0
     - acc_dbl + 1
     - Error (double movement)

   * - 1
     - 1
     - 0
     - 1
     - -1
     - acc - 1
     - acc_dbl + 0
     - Movement in negative direction

   * - 1
     - 1
     - 1
     - 0
     - 1
     - acc + 1
     - acc_dbl + 0
     - Movement in positive direction

   * - 1
     - 1
     - 1
     - 1
     - 0
     - acc + 0
     - acc_dbl + 0
     - No movement

GPIOs for Phase A, B and Led
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The QDEC peripheral is able to work with any of the GPIOs available.

The driver allows to enable pullup on phase A and B.

QDEC Clock source
^^^^^^^^^^^^^^^^^

QDEC is clocked by LF-RC clock at 500 kHz.

QDEC Sample
^^^^^^^^^^^

The QDEC peripheral is able to compute the movement from phase A and phase B
signals. The value stored in the sample can be -1 (movement in negative direction),
0 (no movement), +1 (movement in positive direction) or 2 (double movement error).

For each sample, the value is added to the accumulator, if a double movement occurs,
the value is added to the double movement accumulator.

The sampling period can be configured from 128 us to 131072 us.

QDEC with polling mechanism
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The QDEC driver can be used to read the current accumulator and double movement accumulator
at any time. This operation just reads the current values from the register without any
interruption from the hardware.

QDEC with hardware detection and interruptions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The QDEC driver can be used with hardware detection of movements. In case of
movement detection, the peripheral trigger an ISR. The driver reads the status of the ISR
and detect if new samples are available, if there is an overflow of the accumulator or
if a doublement movement error has been detected.

Then the driver calls the user callback, with the status and the two accumulators values
as user callback parameter.

Report Period
^^^^^^^^^^^^^

When the driver is used with hardware detection of movement, the driver shall be initialized
with a report period.

This specifies how many sample reports are generated if one or both conditions occur.

The two conditions are:
- movement is not null
- double movement is detected

The report period can be configured from 1 sample / report to 320 samples / report.

.. list-table:: Report period configuration
   :widths: 40 40
   :header-rows: 1

   * - Report period
     - Enumeration
   * - 1 sample / report
     - REPORT_PERIOD_1_SAMPLE
   * - 5
     - REPORT_PERIOD_5_SAMPLE
   * - 10
     - REPORT_PERIOD_10_SAMPLE
   * - 20
     - REPORT_PERIOD_20_SAMPLE
   * - 40
     - REPORT_PERIOD_40_SAMPLE
   * - 80
     - REPORT_PERIOD_80_SAMPLE
   * - 160
     - REPORT_PERIOD_160_SAMPLE
   * - 320
     - REPORT_PERIOD_320_SAMPLE

Wake up source
^^^^^^^^^^^^^^

Different wakeup source can be configured. They are used only when the driver is used with
hardware detection and interruptions.

- Report Ready (accumulator is available with number of samples configured by report)
- Overflow (overflow of the accumulator)
- Double movement (movement error detection)

When the SOC is in sleep mode, the PML and the QDEC are powered. It allows the QDEC
peripheral to detect a movement and wakeup the system.

When the system wakes up, it checks the wakeup source and then the QDEC condition
and generates the corresponding ISR which will call the user callback with the status and
accumulator values.

LED Driver
^^^^^^^^^^

The LED driver shall be enabled in case of optical sensor usage. The Led polarity shall be
set. A polarity of 1 means that the GPIO controlling the LED is set to one for the configured
duration when the LED shall be ON.

The Led time shall also be configured. The clock of the QDEC is 500 kHz, meaning that
each LSB for the led time is equivalent to about 2 us.

Since the LED active duration value is stored in 16 bits, this duration ranges
from 2µs to 131070µs with a resolution of 2µs.

If Led time is set to 0, it means that the Led is always on.

QDEC Driver registration
^^^^^^^^^^^^^^^^^^^^^^^^

QDEC Driver shall be registered in NVM_ConfigModules() function.

.. code-block:: c

    #include "qdec.h"

    void NVM_ConfigModules(void)
    {
        // Register the modules.
        QDEC_RegisterModule();

        ...
    }

QDEC Driver configuration
^^^^^^^^^^^^^^^^^^^^^^^^^

The QDEC driver has a configuration structure that shall be declared as extern.
Then the configuration structure shall be initialized in NVM_ConfigModules()
in all conditions of boot because the driver doesn't use persistent memory.

.. code-block:: c

    extern volatile QDEC_Configuration_t gQDEC_Config;

    void NVM_ConfigModules(void)
    {
        ...

        // Enable QDEC peripheral
        gQDEC_Config.enabled = true;
        // Led is enabled
        gQDEC_Config.configBits.bits.led_en = 1;
        // Led polarity set to zero.
        gQDEC_Config.configBits.bits.led_pol = 0;
        // Phase A & B Debouncer enabled
        gQDEC_Config.configBits.bits.debounce_en = 1;
        // Wakeup when report ready is available
        gQDEC_Config.configBits.bits.wakeup_rpt_rdy_en = 1;
        // Wakeup when accumulator overflow occurs
        gQDEC_Config.configBits.bits.wakeup_ovf_en = 1;
        // Wakeup when double movement is detected
        gQDEC_Config.configBits.bits.wakeup_dbl_mvt_en = 1;
        // Phase A input signal connected to GPIO 9
        gQDEC_Config.pinBits.bits.phase_a = 9;
        // Phase B input signal connected to GPIO 11
        gQDEC_Config.pinBits.bits.phase_b = 11;
        // Led output driver connected to GPIO 5
        gQDEC_Config.pinBits.bits.led = 5;

        // Sampling period set to 512 us
        gQDEC_Config.periodBits.bits.sample_period = (uint32_t)SAMPLING_PERIOD_512_US;
        // Report period set to 80 samples
        gQDEC_Config.periodBits.bits.report_period = (uint32_t)REPORT_PERIOD_80_SAMPLE;

        // Led time on is set to 16 * 2 us (500 kHz clock)
        gQDEC_Config.ledTimeBits.bits.led_time_on = 16;

QDEC usage with polling
^^^^^^^^^^^^^^^^^^^^^^^

The QDEC accumulator values can be read at any moment by calling QDEC_StartSingleCapture().
It returns the accumulator value for movement detection and the double movement accumulator
for error detection.

.. code-block:: c

    int32_t acc;
    uint8_t dbl_acc;

    QDEC_StartSingleCapture(&acc, &dbl_acc);

In this case wake up condition should be disabled, because there is no need to wakeup the
system in polling mode.

.. code-block:: c

    // Wakeup when report ready is available
    gQDEC_Config.configBits.bits.wakeup_rpt_rdy_en = 0;
    // Wakeup when accumulator overflow occurs
    gQDEC_Config.configBits.bits.wakeup_ovf_en = 0;
    // Wakeup when double movement is detected
    gQDEC_Config.configBits.bits.wakeup_dbl_mvt_en = 0;

QDEC usage with hardware detection of movement and errors
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The QDEC can be used with interruptions mechanism. It allows the system to enter sleep mode
when there is no movement detected. The QDEC is powered like the PML during sleep mode and
is able to detect any movement or error. In this case, it wakes up the system and generates
an ISR. Then the user callback is called with the status and accumulator values.

QDEC ISR Callback prototype.

.. code-block:: c

    /**
     * @brief Callback for QDEC driver
     * @param status status of the QDEC, report ready, overflow or double movement error
     * @param values accumulator values
     */
    static void QDEC_ISR_Callback(QDEC_Status_t status, QDEC_Values_t values)
    {
        QK_ISR_ENTRY();

        int32_t acc     = values.accumulator;
        uint8_t dbl_acc = values.dbl_mvt_accumulator;

        switch (status)
        {
        case REPORT_READY:

            break;
        case OVERFLOW_ERROR:

            break;
        case DOUBLE_MOVEMENT_ERROR:

            break;
        }

        QK_ISR_EXIT();
    }

The callback shall be registered first and then capture can be enabled.

.. code-block:: c

    QDEC_RegisterCallback(QDEC_ISR_Callback);

    QDEC_StartContinuousCapture();

When the capture needs to be disabled.

.. code-block:: c

    QDEC_StopContinuousCapture();
