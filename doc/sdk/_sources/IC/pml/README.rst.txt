Overview
--------

The PML (*Power Management Logic*) driver provides functionalities to configure the power management logic unit of the EM9305.

This includes management of power domains, sleep mode, wakeup and flags handling for EM Core, Bootloader or Application boot.


Global configuration structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The PML driver defines a global volatile structure named ``gPML_Config``. This structure can be directly accessed after having
included the ``pml.h`` file from within an application.

This global structure gives access to several fields that are used to configure the device behavior like source or accuracy of the clock for example, or anything related to clock calibration and sleep mode. Refer to the ``pml.h`` file provided in the SDK for more details on this structure.
The Implementer’s Guide also contains a comprehensive description of all this structure’s fields in the PML dedicated chapter.


.. ########   #######  ##      ## ######## ########     ########   #######  ##     ##    ###    #### ##    ##
.. ##     ## ##     ## ##  ##  ## ##       ##     ##    ##     ## ##     ## ###   ###   ## ##    ##  ###   ##
.. ##     ## ##     ## ##  ##  ## ##       ##     ##    ##     ## ##     ## #### ####  ##   ##   ##  ####  ##
.. ########  ##     ## ##  ##  ## ######   ########     ##     ## ##     ## ## ### ## ##     ##  ##  ## ## ##
.. ##        ##     ## ##  ##  ## ##       ##   ##      ##     ## ##     ## ##     ## #########  ##  ##  ####
.. ##        ##     ## ##  ##  ## ##       ##    ##     ##     ## ##     ## ##     ## ##     ##  ##  ##   ###
.. ##         #######   ###  ###  ######## ##     ##    ########   #######  ##     ## ##     ## #### ##    ##

Power domain
^^^^^^^^^^^^

There are two power domains which can be enabled / disabled by the PML:

- NVM
- QDEC

For each domain, there are two functions available to power ON the corresponding domain.
One function activates a domain and actively waits for it to be ON (synchronous activation function),
and one function that activates a domain but immediatly returns without waiting for the domain to be ON
(asynchronous function).

The following power domain ``define`` statements shall be used for identifying the power domain to control
through the use of the two above mentioned functions:

.. code-block:: c

    /// Enable VDD NVM domain
    #define PML_VDD_NVM    PML_NVM_REQ_ON_R(1)

    /// Enable VDD QDEC domain
    #define PML_VDD_QDEC   PML_QDEC_REQ_ON_R(1)

The function ``PML_EnableVddDomain()`` waits for the power domain to be ready. Its prototype is the following:

.. code-block:: c

    /**
     * @brief Turn on/off selected voltage domain.
     * @param domain PML_VDD_NVM - select VDD9 domain
     *               PML_VDD_QDEC - select VDD0 domain
     *               note that input domain can be ORed
     *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
     * @param enable TRUE switch selected domain on
     *               FALSE switch selected domain off
     * @note
     *   This function is blocking, it waits till the selected
     *   domain is switched on or off.
     */
    void PML_EnableVddDomain(uint8_t domain, bool enable)

It switches ON/OFF the power domain and waits for it to reach the requested state before returning.

The function ``PML_EnableVddDomainNonBlocking()`` is another way to enable or disable a power domain without
waiting. Once called, this function will immediatly return back to the caller. It can be used when some initialization
actions can be done before a power domain is fully activated. For example, the NVM power domain can be switched ON but
knowing it takes some time, the initialization process can continue to do something else not related to the NVM.
Here is the prototype of this function:

.. code-block:: c

    /**
     * @brief Turn on/off selected voltage domain.
     * @param domain PML_VDD_NVM - select VDD9 domain
     *               PML_VDD_QDEC - select VDD0 domain
     *               note that input domain can be ORed
     *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
     * @param enable TRUE switch selected domain on
     *               FALSE switch selected domain off
     * @note
     *   This function is non-blocking.
     */
    void PML_EnableVddDomainNonBlocking(uint8_t domain, bool enable)

When using the asynchronous domain control function, at some point, it might be requested to actually know if the
power domain has been fully enabled before accessing the device (e.g. the NVM). If there are no other operation that
could be done in parallel, then the function ``PML_EnableVddDomainWait()`` can be called. It will actively wait for
the clock domain to be in the requested state. If this never happens, then the device will be stuck in this function
since there is no timeout mechanism to force exiting it.

Its prototype is the following:

.. code-block:: c

    /**
     * @brief Wait till domain is really turn on/off.
     * @param domain PML_VDD_NVM - select VDD9 domain
     *               PML_VDD_QDEC - select VDD0 domain
     *               note that input domain can be ORed
     *                   (e.g. PML_VDD_NVM | PML_VDD_QDEC)
     * @param enable TRUE switch selected domain on
     *               FALSE switch selected domain off
     * @note
     *   This function just waits till the selected
     *   domain is switch on.
     */
    void PML_EnableVddDomainWait(uint8_t domain, bool enable);


Coil configuration
^^^^^^^^^^^^^^^^^^

By default the software use a Coil configuration of 4.7 uH. If the system use a coil of 2.2 uH,
this configuration shall be set in NVM_ConfigModules function of the application.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // System use a Coil of 2.2 uH, by default software use 4.7 uH.
        PML_SetDcdcCoil(COIL_2_2_UH);

        ...
    }

.. ######  ##        #######   ######  ##    ##    ########   #######  ##     ##    ###    #### ##    ##
.. ##    ## ##       ##     ## ##    ## ##   ##     ##     ## ##     ## ###   ###   ## ##    ##  ###   ##
.. ##       ##       ##     ## ##       ##  ##      ##     ## ##     ## #### ####  ##   ##   ##  ####  ##
.. ##       ##       ##     ## ##       #####       ##     ## ##     ## ## ### ## ##     ##  ##  ## ## ##
.. ##       ##       ##     ## ##       ##  ##      ##     ## ##     ## ##     ## #########  ##  ##  ####
.. ##    ## ##       ##     ## ##    ## ##   ##     ##     ## ##     ## ##     ## ##     ##  ##  ##   ###
..  ######  ########  #######   ######  ##    ##    ########   #######  ##     ## ##     ## #### ##    ##

Clock domain
^^^^^^^^^^^^

The HF (high frequency) Clock frequency can be set to run at 24 MHz or 48 MHz. By default, the CPU starts executing the ROM code at the
frequency of 24 MHz and before switching to the NVM, it switches at a frequency of 48 MHz. This is done automatically
by the ROM software. But in case changing the core frequency is needed, the function ``PML_SetHfClkFrequency()`` can be
used.

.. code-block:: c

    /**
     * @brief  HF clock frequency
     */
    typedef enum
    {
        PML_HF_CLK_FREQ_24MHZ = 0,      /**< 24MHz */
        PML_HF_CLK_FREQ_48MHZ = 1,      /**< 48MHz */
    } PML_HfClkFrequency_t;

.. code-block:: c

    /**
     * @brief Set HF clock frequency configuration.
     * @param freq HF clock frequency to be set.
     */
    void PML_SetHfClkFrequency(PML_HfClkFrequency_t freq);


.. warning::

    It is not recommended to switch frequency back to 24 MHz when using the BLE stack since
    the protocol timer has been sized to reach the BLE requirements with a 48 MHz clock.


There are two source clocks that can be used for HF Clock:

* HF-RC
* HF-XTAL

The ``PML_HfClkSource_t`` type shall be used to select the source clock:

.. code-block:: c

    /**
     * @brief  HF clock source
     */
    typedef enum
    {
        /// RC
        PML_HF_CLK_RC   = PML_SYS_MODE_ACTIVE_RC,
        /// XTAL
        PML_HF_CLK_XTAL = PML_SYS_MODE_ACTIVE_XTAL,
    } PML_HfClkSource_t;

Two functions are available to select the HF clock source.

The first one ``PML_SetHfClkSourceNonBlocking()`` just starts the clock but doesn't wait to be up and running.

.. code-block:: c

    /**
     * @brief Select HF clock oscillator: XTAL or RC
     * @param hfClkSrc select oscillator:
     *            PML_HF_CLK_RC   = RC
     *            PML_HF_CLK_XTAL = XTAL
     * @note This function is non blocking
     *          - it does not wait till the clock source is switched
     */
     void PML_SetHfClkSourceNonBlocking(PML_HfClkSource_t hfClkSrc);

The second one ``PML_SetHfClkSource()`` waits for the clock source to be ready and then returns.

.. code-block:: c

    /**
     * @brief Select HF clock oscillator: XTAL or RC
     * @param hfClkSrc select oscillator:
     *            PML_HF_CLK_RC   = RC
     *            PML_HF_CLK_XTAL = XTAL
     * @note This function is blocking
     *          - it waits till the clock source is switched
     */
    void PML_SetHfClkSource(PML_HfClkSource_t hfClkSrc);

Different LF (low frequency) Clock Source can be used, their IDs are defined in ``PML_LfClkSource_t`` structure.

.. code-block:: c

    /**
     * @brief  LF Clk source modes
     */
    typedef enum
    {
        /// LF XTAL disabled
        PML_LF_CLK_XTAL_DIS   = 0,
        /// LF XTAL enabled
        PML_LF_CLK_XTAL_EN    = 1,
        /// External square wave clock is used
        PML_LF_CLK_XTAL_SQ    = 2,
        /// External sine wave clock is used
        PML_LF_CLK_XTAL_SINE  = 3,
    } PML_LfClkSource_t;

The function ``PML_SetLfClkSource()`` selects the LF clock source and waits for the requested source to be up and running.

.. code-block:: c

    /**
     * @brief Select LF clock oscillator: XTAL or RC, SINE, SQUARE
     * @param lfClkSrc select clk source:
     *            PML_LF_CLK_XTAL_DIS  = Internal RC
     *            PML_LF_CLK_XTAL_EN   = External XTAL
     *            PML_LF_CLK_XTAL_SQ   = External source of square wave signal
     *            PML_LF_CLK_XTAL_SINE = External source of sine wave signal
     * @note This function is non blocking
     */
    void PML_SetLfClkSource(PML_LfClkSource_t lfClkSrc);

..  ######  ##        #######   ######  ##    ##     ######   #######  ##     ## ########   ######  ########     ######  ######## ##       ########  ######  ######## ####  #######  ##    ##
.. ##    ## ##       ##     ## ##    ## ##   ##     ##    ## ##     ## ##     ## ##     ## ##    ## ##          ##    ## ##       ##       ##       ##    ##    ##     ##  ##     ## ###   ##
.. ##       ##       ##     ## ##       ##  ##      ##       ##     ## ##     ## ##     ## ##       ##          ##       ##       ##       ##       ##          ##     ##  ##     ## ####  ##
.. ##       ##       ##     ## ##       #####        ######  ##     ## ##     ## ########  ##       ######       ######  ######   ##       ######   ##          ##     ##  ##     ## ## ## ##
.. ##       ##       ##     ## ##       ##  ##            ## ##     ## ##     ## ##   ##   ##       ##                ## ##       ##       ##       ##          ##     ##  ##     ## ##  ####
.. ##    ## ##       ##     ## ##    ## ##   ##     ##    ## ##     ## ##     ## ##    ##  ##    ## ##          ##    ## ##       ##       ##       ##    ##    ##     ##  ##     ## ##   ###
..  ######  ########  #######   ######  ##    ##     ######   #######   #######  ##     ##  ######  ########     ######  ######## ######## ########  ######     ##    ####  #######  ##    ##

Clock Source selection in PML configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The clock source selection can be set in PML configuration structure. This is a global structure defined in ``pml.h`` header file.

.. code-block:: c

    // in pml.h
    /// PML configuration structure
    extern volatile PML_Configuration_t gPML_Config;

Then in ``NVM_ConfigModules()`` function, update the configuration of the PML.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Update gPML_Config structure here (example).
        gPML_Config.lfClkSourceType = (uint8_t)PML_LF_CLK_XTAL_EN;
        ...
    }

The default configuration uses the LF-RC with calibration. Here is a more complete example.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Use LF-RC with calibration (default configuration)
        gPML_Config.lfClkSourceType     = (uint8_t)PML_LF_CLK_XTAL_DIS;
        gPML_Config.lfClkFreqRatioHf    = 589030;   // 48MHz & 500kHz/12
        gPML_Config.lfClkFreqRatioLf    = 1023;
        gPML_Config.lfClkSourceAccuracy = 250;      // 250ppm
        gPML_Config.rcCalibSkip         = false;    // Do not skip calibration

        // LF-RC Calibration using LF Limit
        gPML_Config.rcCalibHfLimit      = 0;
        gPML_Config.rcCalibLfLimit      = 48;       // ~1200us @500kHz/12
        gPML_Config.rcCalibPeriod       = 20600;    // ~500ms for 500kHz/12
        gPML_Config.rcCalibCorrection   = -105;     // -105ppm

        ...
    }

The LF-XTAL can also be used as a clock source. To select it, the application shall change the
default PML configuration settings.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Use LF-XTAL without calibration
        gPML_Config.lfClkSourceType     = (uint8_t)PML_LF_CLK_XTAL_EN;
        gPML_Config.lfClkFreqRatioHf    = 46875;    // 48MHz & 32.768kHz
        gPML_Config.lfClkFreqRatioLf    = 64;
        gPML_Config.lfClkSourceAccuracy = 150;      // 150ppm
        gPML_Config.rcCalibSkip         = true;     // Disable calibration

        ...
    }

The LF-XTAL calibration can be enabled and configured with more fine tuning parameters.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Use LF-XTAL with calibration
        gPML_Config.lfClkSourceType     = (uint8_t)PML_LF_CLK_XTAL_EN;
        gPML_Config.lfClkFreqRatioHf    = 46875;    // 48MHz & 32.768kHz
        gPML_Config.lfClkFreqRatioLf    = 64;
        gPML_Config.lfClkSourceAccuracy = 50;       // 50ppm
        gPML_Config.rcCalibSkip         = false;    // Do not skip calibration
        gPML_Config.rcCalibHfLimit      = 38400;    // 800us @48MHz
        gPML_Config.rcCalibLfLimit      = 0;
        gPML_Config.rcCalibPeriod       = 16384;    // 500ms @32kHz
        gPML_Config.rcCalibCorrection   = 0;        // 0ppm

        ...
    }


..  ######  ##       ######## ######## ########     ######## #### ##     ## ######## ########
.. ##    ## ##       ##       ##       ##     ##       ##     ##  ###   ### ##       ##     ##
.. ##       ##       ##       ##       ##     ##       ##     ##  #### #### ##       ##     ##
..  ######  ##       ######   ######   ########        ##     ##  ## ### ## ######   ########
..       ## ##       ##       ##       ##              ##     ##  ##     ## ##       ##   ##
.. ##    ## ##       ##       ##       ##              ##     ##  ##     ## ##       ##    ##
..  ######  ######## ######## ######## ##              ##    #### ##     ## ######## ##     ##

Sleep Timer
^^^^^^^^^^^

The LF XTAL clock source can be selected for the sleep timer. It provides a better accuracy when it is used
and less power consumption compared to HF clock source.
To achieve that, the dedicated function ``PML_UseLfXtalClk()`` has called be called during the configuration
stage.

.. code-block:: c

    /**
     * @brief Use LF XTAL/External clock for sleep timer
     * @param enable TRUE use xtal/external clock, FALSE uses RC clock
     */
    void PML_UseLfXtalClk(bool enable);


.. ######## ##    ## ######## ######## ########  #### ##    ##  ######       ######  ##       ######## ######## ########     ##     ##  #######  ########  ########
.. ##       ###   ##    ##    ##       ##     ##  ##  ###   ## ##    ##     ##    ## ##       ##       ##       ##     ##    ###   ### ##     ## ##     ## ##
.. ##       ####  ##    ##    ##       ##     ##  ##  ####  ## ##           ##       ##       ##       ##       ##     ##    #### #### ##     ## ##     ## ##
.. ######   ## ## ##    ##    ######   ########   ##  ## ## ## ##   ####     ######  ##       ######   ######   ########     ## ### ## ##     ## ##     ## ######
.. ##       ##  ####    ##    ##       ##   ##    ##  ##  #### ##    ##           ## ##       ##       ##       ##           ##     ## ##     ## ##     ## ##
.. ##       ##   ###    ##    ##       ##    ##   ##  ##   ### ##    ##     ##    ## ##       ##       ##       ##           ##     ## ##     ## ##     ## ##
.. ######## ##    ##    ##    ######## ##     ## #### ##    ##  ######       ######  ######## ######## ######## ##           ##     ##  #######  ########  ########

Entering Sleep mode
^^^^^^^^^^^^^^^^^^^

There are different possibilities to enter sleep mode and there are three levels of sleep.

- CPU sleep

  In this mode, the processor executes the ``sleep`` instruction. It is kept powered ON but it does nothing else.

- Sleep

  In this mode, the SoC (CPU + internals with exception of the PML) is powered OFF. Some RAM blocks might be powered
  ON mainly for data retention depending on the RAM blocks configuration. The system is able to wake-up through an
  interrupt triggered by some events (GPIO, timers, …). In this mode, the saved power is higher than in CPU sleep mode
  and it is the most used sleep mode.
  If case an application uses QP/C, the system will automatically switch to sleep mode when in idle task and if the sleep
  mode has not been forbidden.

- Deep Sleep

  In this mode, the SoC is powered OFF and by default all RAM blocks are also powered OFF. This is the mode where the
  saved powered is the highest. In ROM v1.0 (DI01 to DI03), the only way to resume from this mode is to issue a complete
  POR cycle (e.g. by removing and inserting again the battery).
  However, in ROM v2.0 and v3.0 (respectively, DI04 and DI05), it is possible to activate an event that will wake-up the SoC when triggered.
  Thus, before switching to this mode, it is also possible to powered ON one or more RAM blocks for data retention.

These three modes are defined in the ``PML_SleepMode_t`` structure:

.. code-block:: c

    /**
     * @brief  Sleep modes
     */
    typedef enum
    {
        /// Sleep CPU - least power saving
        PML_SLEEP_CPU   = 1,
        /// Sleep mode
        PML_SLEEP       = 5,
        /// Deep Sleep mode
        PML_DEEP_SLEEP  = 6,
    } PML_SleepMode_t;

Then, the function ``PML_GoToSleep()`` instructs the system to enter the selected sleep mode. However, this function
does some check to verify that it is possible to switch to sleep mode. If yes, then it enters this mode and never returns.
Otherwise, the function returns ``False`` meaning that switching to sleep mode has been canceled.

The conditions to allow switching to the sleep mode are:

* sleep mode is not forbidden
* sleep timer is up and running

.. code-block:: c

    /**
     * @brief Go to sleep
     * @param sleepMode 5 = PML_SLEEP
     *                  6 = PML_DEEP_SLEEP   (relaxed RC Osc)
     * @return FALSE if sleep mode entry is denied, otherwise system goes to sleep
     *               and never return from this function.
     */
    bool PML_GoToSleep(PML_SleepMode_t sleepMode);

The function ``PML_GoToSleepWithoutChecks`` enters in selected sleep mode and doesn't check
any condition. Sleep mode is forced!

.. code-block:: c

    /**
     * @brief Go to sleep without check (Sleep mode is forced)
     * @param sleepMode 5 = PML_SLEEP
     *                  6 = PML_DEEP_SLEEP   (relaxed RC Osc)
     * @return FALSE if sleep mode entry is denied, otherwise system goes to sleep
     *               and never return from this function.
     */
    bool PML_GoToSleepWithoutChecks(PML_SleepMode_t sleepMode);

The function ``PML_SystemShutDown`` shuts down the system by making it entering in deep sleep mode.
Remind that with ROM v2.0 and above (starting with DI04), the system can resume from this mode through a GPIO signal assertion.

.. code-block:: c

    /**
     * @brief System shut-down
     * @note Function trigger system shutting-down. Shut-down means that
     *       sleep timer is disabled, all IRQs are disabled and system goes to
     *       deep sleep mode.
     */
    void PML_SystemShutDown(void);

.. ##      ##    ###    ##    ## ########    ##     ## ########
.. ##  ##  ##   ## ##   ##   ##  ##          ##     ## ##     ##
.. ##  ##  ##  ##   ##  ##  ##   ##          ##     ## ##     ##
.. ##  ##  ## ##     ## #####    ######      ##     ## ########
.. ##  ##  ## ######### ##  ##   ##          ##     ## ##
.. ##  ##  ## ##     ## ##   ##  ##          ##     ## ##
..  ###  ###  ##     ## ##    ## ########     #######  ##

Wakeup
^^^^^^

The system can be woken up by a GPIO. The ``PML_ConfigWakeUpByGpio()`` function is then used to configure this GPIO.
It has to be called before actually switching to sleep mode and after a POR or software reset. When resuming from sleep mode,
there is no need to configure it again since the GPIO configuration is saved in retention memory.

.. code-block:: c

    /**
     * @brief Configuration of wake-up by GPIO pad
     * @note Function configures wake-up event triggered by GPIO pad
     * @param gpio gpio pad used to wakeing-up
     * @param wakePolarity gpio pad wake-up polarity
     * @param wakeEnable gpio pad wake-up enable
     * @param irqEnable enable IRQ (it substitutes wakeup function in active mode)
     */
    void PML_ConfigWakeUpByGpio(uint8_t gpio, bool wakePolarity, bool wakeEnable, bool irqEnable);


Here is an example of use:

.. code-block:: c

    void NVM_ApplicationEntry()
    {
        /* Not resuming from sleep: POR or SW reset */
        if(!PML_DidBootFromSleep())
        {
            PML_ConfigWakeUpByGpio(gpio, true, true, true);
        }
    }


.. ########   #######   #######  ########
.. ##     ## ##     ## ##     ##    ##
.. ##     ## ##     ## ##     ##    ##
.. ########  ##     ## ##     ##    ##
.. ##     ## ##     ## ##     ##    ##
.. ##     ## ##     ## ##     ##    ##
.. ########   #######   #######     ##

Boot
^^^^

At startup, it is important to know from which state the system is resuming. It can be a POR (e.g. battery insertion),
a software reset or resuming from a sleep period. Knowing that has an impact on the initialization process.
As an example, a GPIO configuration will have to be done when starting-up from POR or SW reset while it might not be
necessary when resuming from a sleep period because this configuration might have been saved in a retention memory.

Three functions provide a way to know from which state the device is starting-up.

For Power on Reset check (power supply is applied):

.. code-block:: c

    /**
     * @brief Get if the chip has booted from POR.
     * @return TRUE if the chip booted from POR, FALSE otherwise.
     */
    static ALWAYS_INLINE uint8_t PML_DidBootFromPOR(void);


For resuming from sleep mode check:

.. code-block:: c

    /**
     * @brief Get if the chip has booted from sleep.
     * @return TRUE if the chip booted from sleep, FALSE otherwise.
     */
    static ALWAYS_INLINE uint8_t PML_DidBootFromSleep(void);

For resuming after a software reset check:

.. code-block:: c

    /**
     * @brief Get if the chip has booted from reset.
     * @return TRUE if the chip booted from reset, FALSE otherwise.
     */
    static ALWAYS_INLINE uint8_t PML_DidBootFromReset(void);


.. ########     ###    ##     ##    ########  ######## ######## ######## ##    ## ######## ####  #######  ##    ##
.. ##     ##   ## ##   ###   ###    ##     ## ##          ##    ##       ###   ##    ##     ##  ##     ## ###   ##
.. ##     ##  ##   ##  #### ####    ##     ## ##          ##    ##       ####  ##    ##     ##  ##     ## ####  ##
.. ########  ##     ## ## ### ##    ########  ######      ##    ######   ## ## ##    ##     ##  ##     ## ## ## ##
.. ##   ##   ######### ##     ##    ##   ##   ##          ##    ##       ##  ####    ##     ##  ##     ## ##  ####
.. ##    ##  ##     ## ##     ##    ##    ##  ##          ##    ##       ##   ###    ##     ##  ##     ## ##   ###
.. ##     ## ##     ## ##     ##    ##     ## ########    ##    ######## ##    ##    ##    ####  #######  ##    ##

RAM Retention
^^^^^^^^^^^^^

The memory manager configures any required number of RAM blocks as retention RAM according the needs of the system or the application.

But the RAM retention can also be configured manually using the PML driver.

.. code-block:: c

    // Enable DRAM0 and DRAM1 retention
    PML_SetRamRetentionEnable(PML_DRAM0_RETENTION | PML_DRAM1_RETENTION);

The RAM retention can also be checked by calling ``PML_IsDramRetOn()`` with the index of the RAM block which needs to be checked.

.. code-block:: c

    /**
     * @brief Is DRAMx retention on
     * @param dram DRAM block number: 0..6
     * @return TRUE, if retention of DRAMx is on, FALSE otherwise
     */
    static ALWAYS_INLINE bool PML_IsDramRetOn(uint8_t dram)

At boot, the system can also check if the RAM retention was disabled.

.. code-block:: c

    /**
     * @brief Indicates if the DRAM retention was disabled during last chip reset.
     * @return TRUE if the DRAM retention was disabled, FALSE otherwise.
     */
    bool PML_DidBootWithoutDRAMRetention(void);
