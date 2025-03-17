Overview
--------

The SPI master driver has 2 modes: 4-wires and 3-wires.

The clock can be configured from 750kHz up to 24MHz.

The configuration of the SPI master is not saved during sleep. It needs to be reconfigured after each wake-up.


SPI Master configuration structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SPI Master module needs to be registered in order to be used.

.. code-block:: c

  // Register SPI Master module
  SPIM_RegisterModule();

  // Enable SPI master.
  gSPIM_Config.enabled = true;

The module needs to be registered in NVM_ConfigModules and to be enabled.

The default configuration of the SPI master is:

.. list-table:: Default SPI master configuration
   :widths: 20 50 30
   :header-rows: 1

   * - Configuration
     - Description
     - Value
   * - enabled
     - SPI master module enable/disable
     - false
   * - gpioSck
     - The SPI clock GPIO
     - GPIO9
   * - gpioMiso
     - The SPI MISO GPIO
     - GPIO10 (not used for 3-wires mode)
   * - gpioMosi
     - The SPI MOSI GPIO
     - GPIO11
   * - transactionDepth
     - The size of the transaction queue
     - 4
   * - rxPhase
     - Invert the RX phase clock
     - 0
   * - spiMode
     - The SPI mode
     - SPIM_MODE_4WIRES (0)
   * - msbFirst
     - Most significant bit first
     - 1
   * - cpol
     - Clock polarity
     - 0 (low when inactive)
   * - cpha
     - Clock phase
     - 0
   * - prescaler
     - The clock prescaler
     - SPIM_PRESCALER_32


The CSN GPIO is set for each transaction.

The SPI Master configuration structure is located in non-persistent memory and is lost during sleep.
It needs to be reinitialized at each wake-up (not only after POR).

The modifications of the default configuration can be done in NVM_ConfigModules in order to be applied during the initialization of the modules.

.. code-block:: c

  void NVM_ConfigModules(void)
  {

      //...

      // Register the modules.
      SPIM_RegisterModule();

      //...

      // Enable SPI master.
      gSPIM_Config.enabled = true;

      // SPI Master config
      gSPIM_Config.configBits.bits.rxPhase = 0;
      gSPIM_Config.configBits.bits.spiMode = SPIM_MODE_4_WIRES;
      gSPIM_Config.configBits.bits.msbFirst = 1;
      gSPIM_Config.configBits.bits.cpol = 0;
      gSPIM_Config.configBits.bits.cpha = 0;
      gSPIM_Config.configBits.bits.prescaler = SPIM_PRESCALER_3; // 48MHz/3 = 16MHz clock

      // SPI Master GPIO configuration.
      gSPIM_Config.gpioSck  = 9;
      gSPIM_Config.gpioMiso = 10;
      gSPIM_Config.gpioMosi = 11;

      //...
  }

.. warning::

  The configuration of the SPI master driver has to be done after each wake-up (not only POR). The configuration is lost during sleep.


SPI Master CSN
^^^^^^^^^^^^^^

The SPI master CSN is given as a parameter to the transfer functions. It needs to be configured as a GPIO output.


.. code-block:: c

  // SPI master write using GPIO_SPIM_CSN
  (void) SPIM_WriteBytes(GPIO_SPIM_CSN, false, gWriteBuf, 2, NULL, NULL);


The second parameter of SPIM_WriteBytes (bool keepChipSelectLow) can be set to true if you want to keep the CSN asserted after the transaction.

.. code-block:: c

  // SPI master write using GPIO_SPIM_CSN, keep GPIO_SPIM_CSN low after the transaction
  (void) SPIM_WriteBytes(GPIO_SPIM_CSN, true, gWriteBuf, 2, NULL, NULL);

  // Schedule a read transaction. GPIO_SPIM_CSN is set high after the transaction.
  (void) SPIM_ReadBytes(GPIO_SPIM_CSN, false, gReadBuf, 1, NULL, NULL);


It is also possible to set manually the CSN. In this case, the transfer functions have to be called with SPIM_CSN_GPIO_NONE
(keepChipSelectLow is ignored).



SPIM clock frequency selection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The clock frequency is defined like this:

clock freq = 48MHz / prescaler


.. list-table:: SPI master clock prescaler
   :widths: 30 20 30
   :header-rows: 1

   * - Enum
     - Prescaler
     - Frequency
   * - SPIM_PRESCALER_2
     - 2
     - 24MHz
   * - SPIM_PRESCALER_3
     - 3
     - 16MHz
   * - SPIM_PRESCALER_4
     - 4
     - 12MHz
   * - SPIM_PRESCALER_6
     - 6
     - 8MHz
   * - SPIM_PRESCALER_8
     - 8
     - 6MHz
   * - SPIM_PRESCALER_16
     - 16
     - 3MHz
   * - SPIM_PRESCALER_32
     - 32
     - 1.5MHz
   * - SPIM_PRESCALER_64
     - 64
     - 750kHz

.. warning::

  For SPIM_PRESCALER_3 , rxPhase cannot be modified and is set to rxPhase = 0 (Sampling Issue)

Update the configuration on the fly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It is possible to change the configuration on the fly using the function SPIM_SetConfiguration.

If you are using multiple devices connected to the SPI master GPIOs, in some case you would like to update the configuration
between transactions.

The function SPIM_SetConfiguration is done for this purpose. This function sets the configuration for the next transactions. It has no effect on the
transactions already waiting in the queue.

.. code-block:: c

  // Init the configuration to apply
  SPIM_Config_t conf = {
      .bits = {
          .cpha = 0,
          .cpol = 0,
          .msbFirst = 1,
          .prescaler = SPIM_PRESCALER_4,
          .rxPhase = 0,
          .spiMode = SPIM_MODE_3_WIRES,
      }
  };

  // Apply the configuration for the next transaction
  SPIM_SetConfiguration(conf);

  // SPI master write using the configuration conf
  (void) SPIM_WriteBytes(GPIO_SPIM_CSN, false, gWriteBuf, 2, NULL, NULL);


The configuration is reset to default after a sleep.


SPI master blocking functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The standard functions (non-blocking) use internally a transaction queue.
Interrupts are called each time a transaction is done (after maximum 16 bytes).
This mechanism is very useful when the SPI master frequency is low because the CPU can do something else during the transaction.
But is adds delays between each transaction.

When the SPI master clock frequency is high, the delays become important compared to transaction time.

Blocking functions are available to reduce the delays between the transactions:

- SPIM_WriteBytesBlocking
- SPIM_ReadBytesBlocking
- SPIM_TransferBytesBlocking

For a 24MHz clock frequency, a write of 100 bytes is almost 2 times faster with the blocking function (40us instead of 77us).

.. note::

  Do not mix blocking and non-blocking functions. If a blocking function is called when the transaction queue is not empty,
  it will return false (transaction not started).
