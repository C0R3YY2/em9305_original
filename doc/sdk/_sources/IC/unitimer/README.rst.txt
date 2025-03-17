Overview
--------

The universal timer driver provides functionality to start and stop one-shot, expresso, 
and periodic timers.

The accuracy of the timer is directly affected by the accuracy of the high frequency clock 
source. However, the universal timer does not explicitly set the clock source. The clock source 
has to be set by the application in the initialization function by calling the ``PML_SetHfClkSource()``
function.

All timers use an ISR mechanism that implies a user callback definition. This callback is executed 
each time the corresponding timer expired.

The prototype of this user callback is be the following:

.. code-block:: c

  // User defined data structure to be used within the Timer user callback.
  typedef struct 
  {
    ...
  } UserDefinedType_t;

  void Timer_ISR_Callback(Driver_Status_t status, void *pUserData)
  {
      // User Code
      if (status == DRIVER_STATUS_OK)
      {
        // Cast the data pointer to the user defined data structure.
        UserDefinedType_t *data = (UserDefinedType_t*)pUserData;

        // Do any relevant processing here.
        ...
      }
  }

The ``Driver_Status_t`` type is defined in ``<SDK>\libs\em_hw_api\includes\driver.h``. If everything is OK,
the ``status`` variable shall be equal to ``DRIVER_STATUS_OK``. Otherwise, when an error occurred in the
driver, this variable should then be equal to ``DRIVER_STATUS_ERROR``.

The ``pUserData`` parameter can be a pointer to a memory area that has been passed to the Timer driver at timer
creation. This memory area shall exist at the time the timer callback function is called.
Thus, in the user defined timer callback, this pointer can be cast to a user defined data structure (see code above) 
or simply to an integer value that is passed when creating the timer like in the following example:

.. code-block: c

  NO_RETURN void NVM_ApplicationEntry() 
  {
    
    // Create a new timer with one single value as the user parameter.
    uint8_t idx = Timer_OneshotDelay(5, &Timer_ISR_Callback, (void *)1);

  }

Then in the timer callback, the parameter value can be retrieved with the following code snippet:

.. code-block: c

  void Timer_ISR_Callback(Driver_Status_t status, void *pUserData) 
  {

    uint8_t data = (uint8_t)pUserData;

  }


One-shot timer
^^^^^^^^^^^^^^

The one-shot timer is a timer that executes a callback function once after a specified amount of time. 
This timer is started immediately and automatically after it has been created. After the callback 
function has been executed, the timer is automatically deleted.

Example of one-shot timer usage:

.. code-block:: c

  // start a one-shot timer of 5ms
  int8_t idx = Timer_OneshotDelay(5, &Timer_ISR_Callback, NULL); // 5ms

  // Wait 6 ms for the timer callback to be executed.
  COMMON_WaitUs(6000);


Expresso timer
^^^^^^^^^^^^^^

The expresso timer is also a timer that executes a callback function after a specified amount of 
time. But unlike the one-shot timer, the expresso timer has to be manually started after it has been created. 
After the callback function has been executed, the expresso timer is not deleted and can be reused at will by calling 
again the ``Timer_Enable()`` function. When not needed anylonger, the timer can be manually deleted using the 
``Timer_UnschedulePolling()`` function.

Example of expresso timer usage:

.. code-block:: c

  // start an expresso timer of 5ms
  int8_t idx = Timer_ExpressoDelay(5, &Timer_ISR_Callback, NULL);

  // enable the timer for one execution
  Timer_Enable(idx);

  // Wait 6 ms for the timer callback to be executed.
  COMMON_WaitUs(6000);

  ... // the timer could be reused several times using the Timer_Enable function

  // Delete the timer once it is not used anymore.
  Timer_UnschedulePolling(idx);


Periodic timer
^^^^^^^^^^^^^^

The periodic timer is a timer that executes a callback function at a specified frequency on a periodic basis. 
This timer shall also be started manually after its creation since it is not automatically started. 
It can be stopped using the ``Timer_Disable()`` function and deleted with the ``Timer_UnschedulePolling()`` function. 
Using this type of timer optimizes the power consumption by synchronising it with other timers running at the same 
frequency.

Example of periodic timer usage:

.. code-block:: c

  // start a periodic timer of 5ms (200Hz). It will be started in sync with other already
  // created timers with the same frequency.
  int8_t idx = Timer_SchedulePolling(200, &Timer_ISR_Callback, NULL);

  // enable the timer
  Timer_Enable(idx);

  // Wait 6 ms for the timer callback to be executed.
  COMMON_WaitUs(6000);

  ...

  // disable the timer
  Timer_Disable(idx);


  ... // the timer could be reused several times using the Timer_Enable and Timer_Disable function

  // Delete the timer once it is not used anymore.
  Timer_UnschedulePolling(idx);


It is also possible to create periodic timer that will not be synchronised with other timers running at the same frequency. 

When the ``Timer_Enable()`` function is called, the timer is immediately started. 

.. code-block:: c

  // start a periodic timer of 5ms (200Hz)
  int8_t idx = Timer_SchedulePollingImmediately(200, &Timer_ISR_Callback, NULL);

  // enable the timer
  Timer_Enable(idx);

  // Wait 6 ms for the timer callback to be executed.
  COMMON_WaitUs(6000);

  ...

  // disable the timer
  Timer_Disable(idx);

  ... // the timer could be reused several times using the Timer_Enable and Timer_Disable function

  // Delete the timer once it is not used anymore.
  Timer_UnschedulePolling(idx);

.. warning::
  Contrary to the ``Timer_SchedulePolling()`` function, The ``Timer_SchedulePollingImmediately()`` function doesn't synchronize 
  the new timer with other timers running at the same frequency. This leads to a power consumption that will not be optimized.


Timers and sleep mode
^^^^^^^^^^^^^^^^^^^^^

When a periodic timer is used, its activity has to be resumed when waking up from sleep mode, otherwise the timer will not be 
reactivated. This is why the function ``Timer_Resume()`` shall be called in the ``NVM_ApplicationEntry()`` function. 
An example of this is shown in the following code snippet.

.. code-block:: c

  void NVM_ConfigModules() 
  {
    
    // Register the timer module before actually using it.
    Timer_RegisterModule();
    
    // Enable timer usage in persistent memory.
    gTimer_Config.enabled = true;

    // Do here other first stage initializations
  }
	
  NO_RETURN void NVM_ApplicationEntry() 
  {
    
    // Enable interruptions (seti instruction)
    IRQ_EnableInterrupts();

    // Resume already created timer(s). It will be effective only when resuming from sleep.
    Timer_Resume();     

    // Do other initializations depending on where does the system come from.
    if(PML_DidBootFromSleep()) 
    {
      // Do any initialization when resuming from sleep.
    }
    else
    {
      // Do any initialization after a POR or SW reset.
    }
  }

There is no need to test if the system is resuming from sleep or not before calling the ``Timer_Resume()`` function. 
This check is automatically integrated into this function and in case the system does not resume from sleep, then calling
this function will have no effect.
