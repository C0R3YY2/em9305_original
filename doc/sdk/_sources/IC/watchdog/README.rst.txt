Overview
--------

The watchdog can reset the device in case there is an unexpected software issue.

The driver is a simple header file with a few inline functions.


Watchdog usage
^^^^^^^^^^^^^^

The watchdog driver provides 3 functions: One to start the watchdog timer, one
to stop it, and a last one to reset it and set a new period.

The watchdog period defines the maximum time allowed before the device is
considered as stucked and shall be reset.

The application shall reset the watchdog timer before it times out. If the
watchdog timer is not reset or stopped before it times out, the device is
reset and the application restarts.

.. note::

  The watchdog timer is automatically reset and stopped in sleep mode. It has
  to be restarted manually after each wake-up.

**Configure and start the watchdog timer:**

.. code-block:: c

  WATCHDOG_ResetAndSetPeriodUs(XXX);
  WATCHDOG_Enable();

With XXX = Desired watchdog period in microseconds.

**Reset the watchdog timer:**

.. code-block:: c

  WATCHDOG_ResetAndSetPeriodUs(XXX);

With XXX = Desired watchdog period in microseconds.
