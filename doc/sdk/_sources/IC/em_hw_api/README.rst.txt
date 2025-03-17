Overview
--------

EM Core handle system interruption by itself, but application code can set a callback to be called
when an ISR occurs.

System interruptions
^^^^^^^^^^^^^^^^^^^^

* SWI - Software Interrupt 0 to 9
* GPIO
* Sleep Timer Out Cmp
* Sleep Timer Full Value
* Protocol Timer Out Cmp
* Protocol Timer Sync
* ARC Timer 0
* ARC Timer 1

Callback for User interruptions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A callback can be provided to EM Core for these interruptions.

You can find in the API section the functions to set the callback for each kind of ISR.

The callback shall have the following prototypes.

.. code-block:: c

  // Prototype of SWI User callback
  void IRQUserHandler_SWI(uint32_t swi);

  // Prototype of GPIO User callback
  void IRQUserHandler_GPIO(uint32_t gpio);

  // Prototype of Sleep Timer out cmp User callback
  void IRQUserHandler_SleepTimerOutCmp(uint8_t channel);

  // Prototype of Sleep Timer full value User callback
  void IRQUserHandler_SleepTimerFullValue(void);

  // Prototype of Protocol Timer out cmp User callback
  void IRQUserHandler_ProtoTimerOutCmp(uint8_t channel);

  // Prototype of Protocol Timer sync User callback
  void IRQUserHandler_ProtoTimerSync(void);

  // Prototype of ARC Timer 0 User callback
  void IRQUserHandler_ARCTimer0(void);

  // Prototype of ARC Timer 1 User callback
  void IRQUserHandler_ARCTimer1(void);


Example
^^^^^^^

Here is an example how to set SWI interrupt call.

.. code-block:: c

  // User SWI interrupt callback
  void IRQUserHandler_SWI(uint32_t swi)
  {
      // user code
  }

  // Application code to set isr callback
  IRQ_SetIRQUserHandlerSW1(IRQUserHandler_SWI);

Interrupts enable
^^^^^^^^^^^^^^^^^
Enabling a specific interrupt in an interrupt group means that the corresponding
status bit is set to 1 when the interrupt occurs. If the interrupt in the
interrupt group is not enabled, then the corresponding status bit will not be
changed, even if the interrupt is triggered.

Interrupts mask
^^^^^^^^^^^^^^^
If one of the status bits corresponding to the interrupt is 1 and the interrupt
is unmasked, the interrupt is handled by the CPU. The interrupt is not handled
by the CPU if it is masked.

Software interrupt mask
^^^^^^^^^^^^^^^^^^^^^^^
Unlike the other interrupt sources, the software interrupt mask is managed
differently. In fact, it is not saved and restored when the device wakes from
sleep, but the software interrupt are always masked when the device wakes from
sleep. Therefore, it is the user's responsibility to unmask the software
interrupt before using it each time the device wakes from sleep.

Critical Sections and Radio ISR
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SDK provides a set of drivers that might execute critical sections
which must not be interrupted during their execution, mainly to fulfill
timing constraints and to avoid concurrent access to a single resource.

This is why interruptions are masked at the beginning of such sections
and unmasked when completed.

However, for the sake of keeping any on-going BLE connection and limiting
the timing impacts on the radio IP, the radio driver has the highest priority
level compared to other drivers.

Consequently, a critical section in a driver will mask all interruptions
except the ones that are of the highest priority level, that is the radio
driver interruptions.

EM recommends not to change the radio driver priority level and not to push
another driver than the radio driver at this higher priority level as well.
In such a case, EM does not guarantee a nominal behavior of the device.
