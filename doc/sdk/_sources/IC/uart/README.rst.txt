Overview
--------

This driver allows to use the UART to communicate with some other peripheral
using 2 wire (Tx, Rx) or 4 wire (Tx, Rx, CTS, RTS).

It allows the following features :

- Full duplex communication.
- 1 start bit, 8 data bits, 1 stop bit.
- Even / odd parity mode or without parity.
- HW generation and checking of parity.
- Configurable speed from 9600 to 1'843'200 bauds, other
  "non standard" speeds can be also supported by defining divider value in register.

  - 9600 Bauds
  - 14400 Bauds
  - 19200 Bauds
  - 28800 Bauds
  - 38400 Bauds
  - 57600 Bauds
  - 76800 Bauds
  - 115200 Bauds
  - 230400 Bauds
  - 460800 Bauds
  - 921600 Bauds
  - 1843200 Bauds

- Configurable divider
- Configurable oversampling factor in range from 4 to 16
- Flow control (CTS, RTS signals) enable / disable
- Separate Tx/Rx enable
- 16 bytes long Rx FIFO for reception and 16 bytes long Tx FIFO for transmission.
- FIFO status available for CPU

  - Tx FIFO empty.
  - Tx FIFO limit (less bytes in Tx FIFO than user defined limit).
  - Rx FIFO limit (more bytes in Rx FIFO than user defined limit).
  - Rx FIFO full.
  - Number of bytes in each FIFO.
- FIFO flush to remove all content. Each FIFO independently.
- Configurable bit order (LSB first or MSB first).


UART and Sleep mode
^^^^^^^^^^^^^^^^^^^

The UART shall be registered in order to be usable after wakeup. This shall be done
at the time the application calls NVM_ConfigModules() function.

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        UART_RegisterModule();

        ...

        // Do not modify the GPIO config after a wake-up from sleep because
        // the configuration is kept in persistent memory.
        if (PML_DidBootFromSleep())
        {
            // Re-initialize the QPC event pool.
            extern uint_fast8_t QF_maxPool_;
            QF_maxPool_ = (uint_fast8_t)0;
            MAIN_InitEventPool();
        }
        else
        {
            // Update the GPIO default configuration
            // The UART is automatically enabled when the Module is registered

            // Disable all inputs
            gGPIO_Config.hardwareState.RegGPIOInputEn.r32 = (uint32_t)0x000;

            // Enable outputs on GPIO7 for UART TX, disable output on other pins
            gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

            // Disable pull-downs on all pins
            gGPIO_Config.hardwareState.RegGPIOPdEn.r32 = (uint32_t)0x000;

            // Enable pull-up on GPIO7 only
            gGPIO_Config.hardwareState.RegGPIOPuEn.r32 = (uint32_t)GPIO_MASK_PIN_7;

            // Set the UART TX output function on GPIO7
            gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
			    (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;

            // Disable the UART RX input function
            gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
                (uint8_t)GPIO_FUNCTION_NOT_MAPPED;

            // Initialize QPC
            QF_init();

            // Initialize the QPC event pool.
            MAIN_InitEventPool();
        }
    }

UART GPIOs
^^^^^^^^^^

Only some GPIOs can be used for the UART.

- RX 0, 1, 2, 3, 4 and 6
- TX 0, 1, 2, 3, 7 and 8
- CTS 1, 2, 3, 4, 8, 9 and 10
- RTS 0, 1, 2, 3, 7 and 10

UART Configuration
^^^^^^^^^^^^^^^^^^

The following configurations for the UART are available, check code block for C code example.

- MSB First    : Set to 1 to send the most significant bits first.
- Parity Mode  : Set to 1 to enable parity or 0 to disable parity check.
- Event Parity : Set to 1 to use Even parity or 0 to use Odd parity (only valid if Parity Mode is enabled).
- Flow Ctrl En : Set to 1 to enable flow control or 0 to disable flow control.
- Majority dis : Per default each baud shall be decoded using majority over 3 samples.
  Majority voting can be disabled by setting it to 1 (only one sample will be used).
- Loopback En  : Set to 1 to enable loopback or 0 to disable loopback.

.. code-block:: c

    UART_Config_t config;

    config.bits.msbFirst    = 1; // Enable MSB first
    config.bits.parityMode  = 0; // Don't use parity
    config.bits.eventParity = 0; // Not used when parity is disabled
    config.bits.flowCtrlEn  = 0; // Disable flow control
    config.bits.majorityDis = 0; // Oversampling using 3 samples is enabled
    config.bits.loopBackEn  = 0; // Disable loopback

    UART_Enable(UART_DISABLE);
    UART_SetConfiguration(config);
    UART_SetBaudRate(115200);
    UART_BlockCommunication(false);

    // IRQ is generated when there are 1 byte in FIFO. This will also cause DMA transfer
    UART_SetTxFifoLimit(1);
    UART_Enable(UART_ENABLE);

UART Custom baud rate configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Specific baud rate can be configured using

.. code-block:: c

    /**
     * @brief Set the baud rate manually.
     * @param clkDivider Clock divider for UART. Valid values: <1, 16383>.
     * @param oversampling Oversampling factor. Valid values: <4, 16>.
     * @returns UART_OK if the baud rate was set, UART_BUSY otherwise.
     * @note The UART baud rate can be set only if UART is not enabled. If UART is
     * running, the request is rejected.
     */
    UART_Status_t UART_SetBaudRateEx(uint16_t clkDivider, uint8_t oversampling);

UART speed can be computed using baud_rate = sys_clock_freq (48 MHz) / (clkDivider * oversampling)

- clkDivider is a 14 bits value from 1 to 16383.
- oversampling is a 5 bits value from 4 to 16.

UART Flow control
^^^^^^^^^^^^^^^^^

Hardware flow control can be enabled by setting 1 to the flow control bit.

.. code-block:: c

    config.bits.flowCtrlEn = 1; // Enable flow control

In this case, UART will use 4 wires (Tx, Rx, CTS, RTS).

- Tx (transmit data)
- Rx (receive data)
- RTS (request to send)
- CTS (clear to send)

In this case control flow GPIOs shall be configured for CTS and RTS.

RTS is an output signal and can be configured like that.

.. code-block:: c

    GPIO_EnablePullDown(rts_pin);
    GPIO_DisablePullUp(rts_pin);
    GPIO_DisableInput(rts_pin);
    GPIO_EnableOutput(rts_pin);
    GPIO_SetOutputPinFunction(rts_pin, GPIO_PIN_FUNC_OUT_UART_RTS);

CTS is an input signal and can be configured like that.

.. code-block:: c

    GPIO_EnablePullDown(cts_pin);
    GPIO_DisablePullUp(cts_pin);
    GPIO_DisableOutput(cts_pin);
    GPIO_EnableInput(cts_pin);
    GPIO_SetInputFunctionPin(GPIO_PIN_FUNC_IN_UART_CTS, cts_pin);

.. note::

  Wake up from Sleep mode is started by given edge on selected GPIO pin and it is managed by PML. PML
  ensures that UART pins are driven to inactive value during waking up. Once system is fully woken up and initialized
  (UART is ready to communicate) CPU releases UART pin configuration and UART pins are driven from UART block
  again.
  If flow control is enabled flow control signals are set to value preventing UART communication during waking up and
  they are released once UART communication can start. If flow control is not used chip should indicate finished wake
  up sequence by HCI event.

UART Usage
^^^^^^^^^^

The UART can be used with or without using ISR with callback.

For sending data asynchronously, a callback shall be provided for end of transaction.

Callback function prototype when using asynchronous sending.

.. code-block:: c

    static void UART_TxCallback(Driver_Status_t status, void* pUserData)
    {
        // When callback is called, it means that all data have been sent by UART
    }

Code to send data over the UART using a string message.

.. code-block:: c

    const char msg[] = "EM Microelectronic";

    uint16_t msg_len = (uint16_t) strlen(msg);

    if (!UART_SendDataWithCallback(msg, msg_len, UART_TxCallback, NULL))
    {
        // An error occured, it can occurs if data length is null or
        // the buffer for tx operation is full.
    }

Data can also be sent without using a callback:

.. code-block:: c

    const char msg[] = "EM Microelectronic";

    uint16_t msg_len = (uint16_t) strlen(msg);

    if (!UART_SendData(msg, msg_len))
    {
        // An error occured, it can occurs if data length is null or
        // the buffer for tx operation is full.
    }
