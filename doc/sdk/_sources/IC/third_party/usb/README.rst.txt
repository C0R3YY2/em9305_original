Introduction
------------

The USB drivers are provided by a 3rd-party, HCC Embedded, that has provided USB drivers for many years.  They have created a custom USB stack that works with our QPC framework.  They have provided sample drivers to emmulated various USB devices:  CDCACM (serial port), KEYBOARD, MOUSE, GENERIC HID (Human interface device), and a combination driver that support CDCACM, KEYBOARD and MOUSE.  At this time these devices perform simple tasks to demonstate that USB is highly functional on the EM9305.

There are also several example application that showcase the interaction between the USB drivers and the BLE stack: `nvm_emb_controller_usb_tl` and `nvm_emb_hid_usb`


Overview
--------

The current available certified drivers communicate at USB 2.0 FullSpeed.

.. note::
    Sleep is not available while the USB drivers are running on the EM9305. Sleep must be forbidden in the global PML configuration struct.

To integrate the USB drivers into an application, several configurations must be added or changed in nvm_main.c. These are not shared with example that utilize the BLE stack without USB, as the USB drivers and the BLE stack must share QPC event pools.

- Include the pml.h header file to access the global PML configuration

- Forbid Sleep Mode in NVM_ConfigModules

.. code-block:: c

    void NVM_ConfigModules(void)
    {
        // Other module registrations...

        gPML_Config.sleepModeForbiden = true;
    }

.. note::
    Ensure you do not call WsfOsInit(); in NVM_ConfigModules when you are using USB drivers. The initialize sequence for the BLE stack must be split later in NVM_ApplicationEntry

- Allocate a specific QPC Event Pool size that accounts for both BLE stack tasks and USB tasks. (See `nvm_emb_hid_usb` for an example)

- Declare the QPC event pool with this size

.. code-block:: c

    #define QPC_EVENT_POOL_SIZE (BLE_APP_TASK_EVENTS_QUEUE_SIZE + USB_TASK_EVENT_QUEUE_SIZE)
    static SECTION_NP_NOINIT uint8_t gQpcEventPool[QPC_EVENT_POOL_SIZE * sizeof(QEventParams)];

- Create a private function MAIN_InitEventPool that zeros and initializes the QPC event pool

.. code-block:: c

    static void MAIN_InitEventPool(void)
    {
        QF_bzero(&gQpcEventPool[0], sizeof(gQpcEventPool));
        QF_poolInit(&gQpcEventPool[0], sizeof(gQpcEventPool), sizeof(QEventParams));
    }


- Call the following initializations in the following order in NVM_ApplicationEntry

.. code-block:: c

    NO_RETURN void NVM_ApplicationEntry(void)
    {
        BSP_Init();

        IRQ_EnableInterrupts();

        QF_init();

        MAIN_InitEventPool();

        //This is defined by the application, and will include initializations for the specfic USB drivers used for the application
        //See nvm_emb_hid_usb for an example that uses the HID drivers
        APPLICATION_USB_TASK_INIT();

        //This is only used when using USB drivers in combination with the EM-Bleu BLE stack
        WsfOsInitOnly();

        //This is defined by the application, please see any nvm_emb_* example for specifics
        APPLICATION_BLE_TASK_INIT();

        PalSysInit();

        // Remaining stack setups... please see BLE documentation or examples for specfics

        //This is defined by the application, and will start the USB drivers
        //See nvm_emb_hid_usb for an example
        APPLICATION_USB_TASK_START();

        //This is only used when using USB drivers in combination with the EM-Bleu BLE stack
        WsfOsStartOnly();

        //This is defined by the application, please see any nvm_emb_* example for specifics
        APPLICATION_BLE_TASK_START();

        //This finally enters the main QPC execution loop
        QF_run();
    }

`nvm_emb_controller_usb_tl` and `nvm_emb_hid_usb` are the best examples for integrating these drivers with the BLE stack.
