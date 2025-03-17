Overview
--------

The I2C driver allows to configure and use the I2C master bus.

It supports only master mode from 100 kbit/s to 1 Mbit/s.

Clock stretching allows the slave to hold down the clock if it needs
to reduce the bus speed. Please read I2C specification
(chapter 3.1.9, Rev. 6 - 4 April 2014) for more details.

7-bit device addressing mode.

16 bytes long RX buffer for reception and 16 bytes long TX buffer for transmission.

I2C configuration structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The I2C module needs to be registered in order to be used.
Then the configuration structure can be set to the needed configuration.

- Clock Frequency
- Clock Stretching
- GPIO SCK
- GPIO SDA

Different clock speed can be used.

.. code-block:: c

  /**
   * @brief Possible frequency rates supported by the I2C master.
   */
  typedef enum {
      ClockFrequency1000 = 0, /**< I2C Fast+ mode, 1000Khz. */
      ClockFrequency833  = 1, /**< I2C Fast+ mode, 833Khz. */
      ClockFrequency400  = 2, /**< I2C Fast mode, 400Khz. */
      ClockFrequency333  = 3, /**< I2C Fast mode, 333Khz. */
      ClockFrequency100  = 4, /**< I2C Standard mode, 100Khz. */
      ClockFrequency83   = 5, /**< I2C Standard mode, 83Khz. */
  } I2C_ClockFrequency_t;

Clock stretching allow to enable / disable the feature using a boolean.

The configuration structure contain also GPIO for SCK and SDA.

- Clock of I2C can be set on GPIO 0, 4, 9 or 10 only.
- SDA of I2C can be set on GPIO 1, 5, 8, 10 or 11 only.

I2C shall be registered in NVM_ConfigModules() and then configuration
structure can be set with the needed settings.

.. code-block:: c

  void NVM_ConfigModules(void)
  {
      //...

      // Register I2C module
      I2C_RegisterModule();

      //...

      // Enable I2C peripheral
      gI2C_Config.clockFrequency = ClockFrequency100;
      gI2C_Config.clockStretching = true;
      gI2C_Config.gpioSck = GPIO_I2C_SCK;
      gI2C_Config.gpioSda = GPIO_I2C_SDA;
      gI2C_Config.enabled = true;

      //...
  }


I2C Read / Write operations
^^^^^^^^^^^^^^^^^^^^^^^^^^^

There is four functions to read and write data on the I2C slave peripheral.

All these functions are non-blocking, the input buffer cannot be modified until
the transaction is complete. The callback is called when the operation finish.

I2C Device address shall be 7 bits.

Performs an I2C write operation on the selected register of the I2C device.

.. code-block:: c

  bool I2C_WriteRegister(uint8_t i2cDevice, uint8_t reg, uint8_t* pBuffer,
      uint32_t bytes, Driver_Callback_t callbackFunction, void* pUserData);

Performs one or more I2C read operations on the selected register of the I2C device.

.. code-block:: c

  bool I2C_ReadRegister(uint8_t i2cDevice, uint8_t reg, uint8_t* pBuffer,
      uint32_t bytes, Driver_Callback_t callbackFunction, void* pUserData);

Performs one or more I2C read operations on the selected I2C device.

.. code-block:: c

  bool I2C_Read(uint8_t i2cDevice, uint8_t* pBuffer, uint32_t bytes,
      Driver_Callback_t callbackFunction, void* pUserData);

Performs one or more I2C read operations on the selected I2C device.

.. code-block:: c

  bool I2C_Write(uint8_t i2cDevice, uint8_t* pBuffer, uint32_t bytes,
      Driver_Callback_t callbackFunction, void* pUserData);

.. warning::

  If the buffer is on the stack, you must ensure that the transaction
  completes before the function exits.

The prototype of the callback for end of operation shall be

.. code-block:: c

  void I2C_Callback(Driver_Status_t status, void* pUserData)
  {
      // I2C operation is finished
  }


