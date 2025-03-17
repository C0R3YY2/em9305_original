Overview
--------

The GPIOs are standard input/output structures with high-drive, pull-up or pull-down capability.

An optional input debouncer is clocked by the LF RC oscillator divided by a selectable value between 16 and 2048. Any GPIO can interrupt the CPU on any input value and
are maskable.


GPIO configuration structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The GPIO module is always used and is initialized before NVM_ApplicationEntry function call.

The GPIO configuration structure is saved in persistent memory before sleep. It is restored after a wake up.

The default configuration of the GPIOs is:

.. list-table:: Initial GPIO configuration
   :widths: 30 70
   :header-rows: 1

   * - GPIO
     - Function
   * - GPIO0
     - SPI slave CSN
   * - GPIO1
     - SPI slave SCK
   * - GPIO2
     - SPI slave MISO
   * - GPIO3
     - SPI slave MOSI
   * - GPIO4
     - SPI slave RDY
   * - GPIO5-11
     - Not used


In more details, the default configuration structure of the GPIO driver is:

.. list-table:: Default GPIO configuration structure (gGPIO_Config.hardwareState)
   :widths: 20 50 30
   :header-rows: 1

   * - Configuration
     - Description
     - Value
   * - | RegGPIOCfgDeb.regs.
       | GPIODebEn
     - Debouncer enabled
     - 0, Disabled for all GPIO
   * - | RegGPIOCfgDeb.regs.
       | GPIODebClock
     - Debouncer prescaler / time
     - GPIO_DEBOUNCE_PRESCALER_16
   * - RegGPIOInputEn.r32
     - Input enable
     - | GPIO_MASK_SPIS_CSN \|
       | GPIO_MASK_SPIS_SCK \|
       | GPIO_MASK_SPIS_MOSI
   * - RegGPIOOutputEn.r32
     - Output enable
     - | GPIO_MASK_SPIS_MISO \|
       | GPIO_MASK_SPIS_RDY
   * - RegGPIOPuEn.r32
     - Pull-up enable
     - 0, Disabled for all GPIO
   * - RegGPIOPdEn.r32
     - Pull-down enable
     - 0, Disabled for all GPIO
   * - RegGPIOOdEn.r32
     - Open-drain enable
     - 0, Disabled for all GPIO
   * - | RegGPIOOutSel0.regs.
       | GPIOOutSel0
     - Output selection 0
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel0.regs.
       | GPIOOutSel1
     - Output selection 1
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel0.regs.
       | GPIOOutSel2
     - Output selection 2
     - GPIO_PIN_FUNC_OUT_SPIS_MISO
   * - | RegGPIOOutSel0.regs.
       | GPIOOutSel3
     - Output selection 3
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel1.regs.
       | GPIOOutSel4
     - Output selection 4
     - GPIO_PIN_FUNC_OUT_SPIS_RDY
   * - | RegGPIOOutSel1.regs.
       | GPIOOutSel5
     - Output selection 5
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel1.regs.
       | GPIOOutSel6
     - Output selection 6
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel1.regs.
       | GPIOOutSel7
     - Output selection 7
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel2.regs.
       | GPIOOutSel8
     - Output selection 8
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel2.regs.
       | GPIOOutSel9
     - Output selection 9
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel2.regs.
       | GPIOOutSel10
     - Output selection 10
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOOutSel2.regs.
       | GPIOOutSel11
     - Output selection 11
     - GPIO_PIN_FUNC_OUT_GPIO
   * - | RegGPIOInpSel0.regs.
       | GPIOInSelSSCSN
     - | Input function selection
       | SPI slave CSN
     - GPIO_PIN_SPIS_CSN
   * - | RegGPIOInpSel0.regs.
       | GPIOInSelSSSCK
     - | Input function selection
       | SPI slave SCK
     - GPIO_PIN_SPIS_SCK
   * - | RegGPIOInpSel0.regs.
       | GPIOInSelSSMOSI
     - | Input function selection
       | SPI slave MOSI
     - GPIO_PIN_SPIS_MOSI
   * - | RegGPIOInpSel0.regs.
       | GPIOInSelSMMISO
     - | Input function selection
       | SPI master MISO
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel1.regs.
       | GPIOInSelUARTRDX
     - | Input function selection
       | UART RX
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel1.regs.
       | GPIOInSelUARTCTS
     - | Input function selection
       | UART CTS
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel1.regs.
       | GPIOInSelTim2Start
     - | Input function selection
       | Timer2 start
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel1.regs.
       | GPIOInSelTim2Cap
     - | Input function selection
       | Timer2 cap
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel2.regs.
       | GPIOInSelTim3Start
     - | Input function selection
       | Timer3 start
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel2.regs.
       | GPIOInSelTim3Cap
     - | Input function selection
       | Timer3 cap
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel2.regs.
       | GPIOInSelTim2Clk
     - | Input function selection
       | Timer2 clk
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel2.regs.
       | GPIOInSelTim3Clk
     - | Input function selection
       | Timer3 clk
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel3.regs.
       | GPIOInSelI2sSClk
     - | Input function selection
       | I2S sclk
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel3.regs.
       | GPIOInSelI2sFsync
     - | Input function selection
       | I2S fsync
     - GPIO_FUNCTION_NOT_MAPPED
   * - | RegGPIOInpSel3.regs.
       | GPIOInSelI2sSData
     - | Input function selection
       | I2S sdata
     - GPIO_FUNCTION_NOT_MAPPED
   * - RegGPIOCfg.r32
     - Configuration register
     - REG_GPIO_CFG_RESET_VALUE


Configure the GPIOs
^^^^^^^^^^^^^^^^^^^

There are 2 ways to configure the GPIOs:

* Modify the default configuration structure in NVM_ConfigModules.
* Use the GPIO API to modify the configuration.


If you choose to modify the default configuration, the GPIOs will be set after the NVM_ConfigModules function call.

This default configuration has to be modified only once because the GPIO config is kept in persistent memory.
You can update it only after a POR (power on reset).

Here is an example (copied from printf_example) of how to access directly the GPIO config to update
the default configuration (in this case UART TX on GPIO7):

.. code-block:: c

  void NVM_ConfigModules(void)
  {
      // Register the modules.
      UART_RegisterModule();

      // Enable UART.
      gUART_Config.enabled = true;

      // Do not modify the GPIO config after a wake-up from sleep because
      // the configuration is kept in persistent memory.
      if (!PML_DidBootFromSleep())
      {
          // Update the GPIO default configuration

          // Disable all inputs
          gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)0x000;

          // Enable outputs on GPIO7 for UART TX,
          // disable output on other pins
          gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 =
              (uint32_t)GPIO_MASK_PIN_7;

          // Disable pull-downs on all pins
          gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

          // Enable pull-up on GPIO7 only
          gGPIO_Config.hardwareState.RegGPIOPuEn.r32 =
              (uint32_t)GPIO_MASK_PIN_7;

          // Set the UART TX output function on GPIO7
          gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
              (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

          // Disable the UART RX input function
          gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
              (uint8_t)GPIO_FUNCTION_NOT_MAPPED;
      }
  }

.. note::

  The configuration of the GPIOs may be different from the default configuration. It depends on
  which driver module is registered and enabled.

  During the module initialization, the other drivers configure the GPIOs they are using.

If you want to use the GPIO driver API to update the GPIO configuration, you can do it where you want except
in NVM_ConfigModules (because the configuration is overwritten by the default configuration).

Here is an example (copied from timer_example) of how to modify the GPIO configuration with the GPIO API
(the config is done in a QPC task, it is setting 2 GPIOs to be GPIO output):

.. code-block:: c

  // Configure TIMER1_GPIO
  GPIO_DisablePullDown(TIMER1_GPIO);
  GPIO_DisablePullUp(TIMER1_GPIO);
  GPIO_DisableInput(TIMER1_GPIO);
  GPIO_DisableOpenDrain(TIMER1_GPIO);
  GPIO_EnableOutput(TIMER1_GPIO);
  GPIO_SetOutputPinFunction(TIMER1_GPIO, GPIO_PIN_FUNC_OUT_GPIO);
  GPIO_SetLow(TIMER1_GPIO);

  // Configure TIMER2_GPIO
  GPIO_DisablePullDown(TIMER2_GPIO);
  GPIO_DisablePullUp(TIMER2_GPIO);
  GPIO_DisableInput(TIMER2_GPIO);
  GPIO_DisableOpenDrain(TIMER2_GPIO);
  GPIO_EnableOutput(TIMER2_GPIO);
  GPIO_SetOutputPinFunction(TIMER2_GPIO, GPIO_PIN_FUNC_OUT_GPIO);
  GPIO_SetLow(TIMER2_GPIO);


Examples
^^^^^^^^

In this section, simple examples are provided in order to demonstrate how to use the GPIO driver.


**GPIO output:**

Set the value of a GPIO configured as output.

.. code-block:: c

  // Configure GPIO8

  // Disable pull-down on GPIO8
  GPIO_DisablePullDown(8);
  // Disable pull-up on GPIO8
  GPIO_DisablePullUp(8);
  // Disable open-drain on GPIO8
  GPIO_DisableOpenDrain(8);
  // Set GPIO8 function to GPIO
  GPIO_SetOutputPinFunction(8, GPIO_PIN_FUNC_OUT_GPIO);
  // Disable input on GPIO8
  GPIO_DisableInput(8);
  // Set GPIO8 low
  GPIO_SetLow(8);
  // Enable output on GPIO8
  GPIO_EnableOutput(8);


  // Examples

  // Set GPIO8 high
  GPIO_SetHigh(8);
  // Set GPIO8 low
  GPIO_SetLow(8);
  // Toggle GPIO8
  GPIO_Toggle(8);



**GPIO input:**

Read the value of a GPIO configured as input.

.. code-block:: c

  // Configure GPIO8

  // Enable pull-down on GPIO8
  GPIO_EnablePullDown(8);
  // Disable pull-up on GPIO8
  GPIO_DisablePullUp(8);
  // Disable open-drain on GPIO8
  GPIO_DisableOpenDrain(8);
  // Set GPIO8 function to GPIO
  GPIO_SetOutputPinFunction(8, GPIO_PIN_FUNC_OUT_GPIO);
  // Disable output on GPIO8
  GPIO_DisableOutput(8);
  // Enable input on GPIO8
  GPIO_EnableInput(8);

  // Wait minimum 4 clock cycles before reading the value
  // of GPIO8
  COMMON_WaitUs(1);

  // Example

  // Get GPIO8 value
  if(GPIO_GetValue(8))
  {
    // GPIO8 is 1
  }


**GPIO interrupt:**

Configure a rising interrupt on a GPIO and capture the callback.

This example is using QPC. It sends a QPC event when an interrupt on GPIO8 is catched.

The callback:

.. code-block:: c

  void IRQUserHandler_GPIO(uint8_t gpio)
  {
      QK_ISR_ENTRY();

      if(gpio == 8)
      {
          // Post an event to the task.
          QEventParams *pEvent = (QEventParams*)Q_NEW(QEventParams, SIG_GPIO_INT);
          QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);
      }

      QK_ISR_EXIT();
  }

The configuration of GPIO8 to catch interrupts and wake up the EM9305:

.. code-block:: c

  // Enable pull down only
  GPIO_DisablePullUp(8);
  GPIO_EnablePullDown(8);
  GPIO_DisableOpenDrain(8);

  // Set polarity rising, ISR will be triggered in active mode
  // when a rising edge is detected.
  GPIO_SetPolarityRising(8);
  // Enable input
  GPIO_EnableInput(8);

  // Set the callback for user ISR
  IRQ_SetIRQUserHandlerGPIO(IRQUserHandler_GPIO);

  // GPIO 8 wake-up configuration. 
  // Set wakePolarity to true to wake up EM9305 on GPIO high (level sensitive).
  // Set wakeEnable to true to enable wake-up on GPIO.
  // Set irqEnable to true to generate an ISR in active mode when GPIO sensitive edge is triggered.
  PML_ConfigWakeUpByGpio((uint8_t)8, true, true, true);

  // Configure the HF XTAL to run as a wake-up action (if required).
  PML_RegisterWakeUpAction(PML_WAKEUP_ACTION_RUN_HF_XTAL, GPIO_MASK(8), true);

  // The protocol timer must be started after wakeup if you are using a BLE
  // stack or anything that requires the protocol timer to run. If not, this
  // line can be removed.
  PML_RegisterWakeUpAction(PML_WAKEUP_ACTION_RUN_PROT_TIM, GPIO_MASK(8), true);
