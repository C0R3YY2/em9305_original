Overview
--------

The SPI slave driver allows to use the SPI Slave with some other peripherals
using 4 wire SPI interface (SCK, CSN, MISO, MOSI) with flow control (RDY signal).

The SPI slave interface can be configured in half-duplex mode or in full-duplex mode.

Half-duplex mode is the default one for HCI communication.
In this mode, the SPI flow control is available.

.. warning::

  Full-duplex mode is not intended for HCI, as HCI requires flow control (RDY)

SPI Slave and Sleep mode
^^^^^^^^^^^^^^^^^^^^^^^^^

The SPI Slave shall be registered in order to be usable after wakeup.
This shall be done at the time the application calls NVM_ConfigModules() function.
Note that the configuration of this module is saved in the persistent memory.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Register the modules
        UART_RegisterModule();
        SPIS_RegisterModule();

        // Enable UART.
        gUART_Config.enabled = true;

        // Enable SPIS
        gSPIS_Config.enabled = true;

        // SPI Slave config
        gSPIS_Config.configBits.bits.msbFirst = 1;
        gSPIS_Config.configBits.bits.cpol = 0;
        gSPIS_Config.configBits.bits.cpha = 0;
        gSPIS_Config.configBits.bits.mode = SPIS_MODE_FULL;

        // Check if it wakes up from sleep.
        if (PML_DidBootFromSleep())
        {
            // Re-initialize the QPC event pool.
            extern uint_fast8_t QF_maxPool_;
            QF_maxPool_ = (uint_fast8_t)0;
        }
        else
        {
            // Initialize QPC
            QF_init();
        }

        // Initialize the QPC event pool.
        Main_InitEventPool();

        // Disable sleep mode if JTAG debug is used.
        // gPML_Config.sleepModeForbiden = true;

        // Enable JTAG.
        // GPIO_EnableJtag4Wires();
    }

When in half duplex mode, the communication is blocked by the driver when the system comes from the sleep mode. So the application shall restart the communication when ready to communicate.

.. code-block:: c

    NO_RETURN void NVM_ApplicationEntry(void)
    {
        // Initialize the board support package.
        BSP_Init();

        // Enable interrupts (threshold set during IRQ module initialization).
        IRQ_EnableInterrupts();

        // Check if it is wake-up from sleep.
        if (PML_DidBootFromSleep())
        {
            // Post an event to the application task that we woke up from sleep.
            MyAppTask_PostEvent(BOOT_FROM_SLEEP_SIG, NULL);

            // Resume QPC
            (void)QF_resume();
        }
        else
        {
            // Setup GPIOs.
            Main_SetupUartGpio();
            Main_SetupSpiGpio();

            // Configure wake up.
            Main_SetupWakeUpByGpio(GPIO_SPIS_CSN);

            // Create and start the application task.
            MyAppTask_Create();
            MyAppTask_Start();

            // Run QPC
            (void)QF_run();
        }

        // Initialization failed!
        while (true)
        {
            HaltCPU();
        }
    }

In order to support the sleep mode, the chip select pin shall be configured to be able to wakeup
the system.

.. code-block:: c

    static void MyApp_PrepareNewSpiTransaction(void)
    {
        (void)SPIS_ReceiveDataEx(gInputBuffer,
                                1,
                                MyApp_Callback,
                                (void*)SPIS_DATA_SIG);
    }

    static void Main_SetupWakeUpByGpio(uint8_t gpio)
    {
        PML_ConfigWakeUpByGpio(gpio, false, true, false);
        PML_RegisterWakeUpAction(PML_WAKEUP_ACTION_RUN_PROT_TIM,
            (uint32_t)SET_BOOT_ACTION_FLAGS_GPIO(1u << gpio), true);
        PML_EnableWakeUpFlags(false);
    }


SPIS Configuration
^^^^^^^^^^^^^^^^^^

The following configurations for the SPIS are available, check code block for C code example.

- MSB First      : Set to 1 to send the most significant bits first.
- Clock polarity : Set to 1 for clock inactive high or 0 for clock inactive low
- Clock Phase    : Set to 1 for data is valid on clock falling edge or 0 for data valid on clock rising edge
- Duplex Mode    : Set to 1 to use full-duplex mode or 0 to use half-duplex mode

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        SPIS_RegisterModule();

        // Enable SPIS
        gSPIS_Config.enabled = true;

        // SPI Slave config
        gSPIS_Config.configBits.bits.msbFirst = 1; // Enable MSB first
        gSPIS_Config.configBits.bits.cpol = 0;     // Clock inactive low
        gSPIS_Config.configBits.bits.cpha = 0;     // Data valid on clock rising edge
        gSPIS_Config.configBits.bits.mode = SPIS_MODE_HALF; // Half-duplex mode


SPIS Flow control management on RDY signal
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In half-duplex mode, SPI flow control over the RDY signal shall be managed.

As described in the chapter on "SPI slave and sleep mode", the flow control is there
to address the control transition from Sleep mode to Active mode.
Generally, the flow control can be used to notify the external Host that it must wait
until EM9305 is ready for communication. Use the following code to force the RDY signal to 0:

.. code-block:: c

   SPIS_BlockCommunication(true); // force the RDY signal to 0


SPI Slave Usage
^^^^^^^^^^^^^^^^

The SPIS can be used with or without using ISR with callback.

When a callback is provided, the driver calls it at the end of each transaction.

The callback function shall be defined with the following prototype:

.. code-block:: c

    static void SPIS_TxCallback(Driver_Status_t status, void* pUserData)
    {
        // When callback is called, it means that all data have been sent by SPIS
    }

In order to send data over the SPIS, the following code could be used:

.. code-block:: c

    // Use random data for SPI transcation
    memcpy(gTxBufferSpiS, (uint8_t*)gRandom[1], SPI_TRANS_LENGTH);

    if (!SPIS_SendDataWithCallback(gTxBufferSpiS, SPI_TRANS_LENGTH, SPIS_TxCallback, NULL))
    {
        // An error occured, it can occurs if data length is null or
        // the buffer for tx operation is full.
    }

Data can also be sent without using a callback. In this case, there is no acknowledgement that data has been actually sent or not:

.. code-block:: c

    // Use random data for SPI transaction
    memcpy(gTxBufferSpiS, (uint8_t*)gRandom[1], SPI_TRANS_LENGTH);

    if (!SPIS_SendData(gTxBufferSpiS, SPI_TRANS_LENGTH))
    {
        // An error occured, it can occurs if data length is null or
        // the buffer for tx operation is full.
    }
